<?php

// tm_status_updates.display.inc - status update display methods

/**
 * Approval callback for test callback
 */
function tm_status_updates_display_callback() {

	// need to be logged in
	if (user_is_logged_in()) {
		return true;
	}

	return true;
}

/**
 * Status updates render
 */
function tm_status_updates_display() {
 
 	global $user;
 	global $conf;
 	global $tm_status_updates_template;
 	global $tm_status_updates_css;

 	// Add js for posting update
	drupal_add_js(drupal_get_path('module', 'tm_status_updates') . '/js/tm_status_updates.js');

	// Help text
	$help_html = "";
	$help_html .= "<li><a href='javascript:jq_user_status_update(" . $user->uid . ");'>Post update</a></li>";
	$help_html .= "<li>Your profile stats";
	$help_html .= "<li>Upcoming events in chapters that you are a member of (and conferences)";
	$help_html .= "<li>Recent blog articles (via API)";
	$help_html .= "<li>Popular discussion articles (via API)";
	$help_html .= "<li>Members who followed you";
	$help_html .= "<li>New members in your chapter(s)";
	$help_html .= "<li>New companies created by members in your chapter(s)";
	$help_html .= "<li>If you have a company then show new followers of company (or separate company report..?)";
	$help_html .= "<li>Top members in your chapter (by number of followers)";


	// get recent flag items
	$result = db_query("select * from tm_newsfeed_status_updates where entity_type = 'user' order by created desc limit 20");
	$results = $result->fetchAll();

	$html_items = array();
	$feed_item_count = 1;
	foreach($results as $result) {

		// get posting user
		$posting_user = user_load($result->entity_id);
		$posting_user_name = $posting_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $posting_user->field_user_last_name[LANGUAGE_NONE][0]['value'];
		$posting_user_url = drupal_get_path_alias("user/" . $posting_user->uid);
      	$posted_time = format_interval(time() - strtotime($result->created), 1) . " ago";

      	// ensure posting user is approved
		if (!in_array("approved user", $posting_user->roles))  {
			continue;
		}

		// get image url
		if (isset($posting_user->field_avatar[LANGUAGE_NONE][0]['uri'])) {
			$image_url = image_style_url("avatar", $posting_user->field_avatar[LANGUAGE_NONE][0]['uri']);
		} else {
			$default_image_uri = _tm_users_get_default_avatar($conf["tm_images_default_field_avatar"], $posting_user->uid);
			$image_url = image_style_url("avatar", $default_image_uri);
		}
	
		$feed_info = "<span class='status_update'>" . strip_tags($result->status_update) . " <span class='status_update_ago'>" . $posted_time . "</span>";
		$feed_title = l($posting_user_name, $posting_user_url) . " posted an update.";
		$feed_html = tm_status_updates_render_feed($posting_user_url, $image_url, $feed_title, $feed_info, $feed_item_count, 100);

		$html_items[] = $feed_html;
		$feed_item_count++;

	}

	// render items into feed
	$feed_html = implode("\n", $html_items);
	$html = str_replace("__FEED_ITEMS__", $feed_html, $tm_status_updates_template);
	$html = str_replace("__FEED_TITLE__", "Status updates", $html);
	$html = str_replace("__FEED_VIEW_MORE_URL__", "/newsfeed", $html);
	$html = str_replace("__FEED_VIEW_MORE_TEXT__", "View more", $html);

	return $tm_status_updates_css . $help_html . "<br>" . $html;

}

function tm_status_updates_render_feed($feed_url, $feed_image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items) {
	global $tm_status_update_template;
	$html = str_replace("__FEED_URL__", $feed_url, $tm_status_update_template);
	$html = str_replace("__FEED_IMAGE_URL__", $feed_image_url, $html);
	$html = str_replace("__FEED_TITLE__", $feed_title, $html);
	$html = str_replace("__FEED_INFO__", $feed_info, $html);
	// hide items if we are displaying js paging
	if ($feed_item_count < $display_num_items) {
		$html = str_replace("__FEED_LI_STYLE__", "display: block;", $html);	
	} else {
		$html = str_replace("__FEED_LI_STYLE__", "display: none;", $html);	
	}
	$html = str_replace("__FEED_LI_CLASS__", "feeditem-" . $feed_item_count, $html);
	return $html;
}


/* Templates */
global $tm_status_updates_template;
$tm_status_updates_template = <<<EOT

<section class="contained contained-block status-update">
	<header class="contained-head">
		<h1 class="prime-title status_update top">__FEED_TITLE__</h1>
	</header>
	<div class="contained-body status-update">
		<ul class="user-list related-list">
			__FEED_ITEMS__
		</ul>
	</div>
	<div class="more-link" style="font-size: 14px;">
		<a href='__FEED_VIEW_MORE_URL__'>__FEED_VIEW_MORE_TEXT__</a>&nbsp;
	</div>
</section>
EOT;

global $tm_status_update_template;
$tm_status_update_template = <<<EOT
<li class="status-update __FEED_LI_CLASS__" style="__FEED_LI_STYLE__">
   <article class="card contained view-mode-grid status-update clearfix">
    	<div style="padding-left: 2em; padding-right: 4em;">
    	
	        <div class="media">
		        <div class="avatar">
		        	<span class="badge-status-update">
		        		 <a href="__FEED_URL__" class="status-update"><img class="status-update-image" typeof="foaf:Image" src="__FEED_IMAGE_URL__" width="256" height="256" alt=""></a>
		        	</span>
		        </div>
	        </div>

	    	<div class="teaser">
	      		<span class="prime-title status-update">__FEED_TITLE__</span>
	      		<p class="meta status-update"><span class="role">__FEED_INFO__</span>
	      	</div>
	     
      	</div>
      	
	</article>
</li>
EOT;

global $tm_status_updates_css;
$tm_status_updates_css = <<<EOT
<style>
.prime-title.status-update a:hover { color: #3080b2;}
.prime-title.status-update a { text-decoration: underline;}
.card.status-update { padding-top: 0.5rem; padding-bottom: 0.5rem;}
.card.status-update:hover { background-color: #e7f2f7;}
img.status-update-image { max-height: 64px; max-width: 64px;}
</style>
EOT;

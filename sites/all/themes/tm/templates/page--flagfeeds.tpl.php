<style>
.prime-title.flagfeed a:hover { color: #3080b2;}
.prime-title.flagfeed a { text-decoration: underline;}
.card.flagfeed { padding-top: 0.5rem; padding-bottom: 0.5rem;}
.card.flagfeed:hover { background-color: #e7f2f7;}
img.flagfeed-image { max-height: 64px;}
/*.contained.contained-block.flagfeed { border-left: 8px solid #3080b2; }*/
</style>

<script type="text/javascript">
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

	flagfeedShowAll = function() {
		$("#flagfeedShowAll").hide();
		$("#flagfeedViewCommunity").show();
		$("li.flagfeed").show();
	}

});})(jQuery, Drupal, this, this.document);
</script>

<?php

function tm_show_flagfeeds($display_num_items = 5, $display_max_items = 20, $cache_key = "page-flagfeeds", $cache_time = 120, $show_unapproved = false, $show_repeat_user = false) {

	// cache for logged out users
	if (!user_is_logged_in()) {
		$cache_value = cache_get($cache_key, 'cache');
		if (!empty($cache_value)) {
			print($cache_value->data);
			return;
		}
	}

	global $conf;
	global $tm_feeds_template;

	// get recent flag items
	$fetch_num_items = $display_max_items * 4; // get more items than we need as we dont display all items
	$result = db_query("SELECT * FROM flagging ORDER BY timestamp DESC LIMIT " . $fetch_num_items);
	$results = $result->fetchAll();

	// load flag into array by id
	$flag_types = array();
	$all_flags = flag_get_flags();
	foreach ($all_flags as $flag) {
		$flag_types[$flag->fid] = $flag;
	}

	$feed_html_items = array();
	$last_flagging_user = null;
	$feed_item_count = 0;
	foreach($results as $result) {

		// reset vars
		$html = "";
		$flagged_node = null;
		$flagged_user = null;

		// see if we've reached the number of flags we want to display
		if ($feed_item_count >= $display_max_items) { 
			continue;
		}

		$flag = $flag_types[$result->fid];

		if ($result->entity_type == "node") {
			$flagged_node = node_load($result->entity_id);
			$flagged_node_url = drupal_get_path_alias("node/" . $flagged_node->nid);
		}
		if ($result->entity_type == "user") {
			$flagged_user = user_load($result->entity_id);
			$flagged_user_name = $flagged_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $flagged_user->field_user_last_name[LANGUAGE_NONE][0]['value'];
			$flagged_user_url = drupal_get_path_alias("user/" . $flagged_user->uid);
		}

		$flagging_user = user_load($result->uid);
		$flagging_user_name = $flagging_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $flagging_user->field_user_last_name[LANGUAGE_NONE][0]['value'];
		$flagging_user_url = drupal_get_path_alias("user/" . $flagging_user->uid);
      	$flagged_time = format_interval(time() - $result->timestamp, 1) . " ago";

		// skip showing feed for non-approved users
		if (!$show_unapproved) {
			if (!in_array("approved user", $flagging_user->roles))  {
				continue;
			}
		}

		// skip showing if same user has flagged multiple times
		if (!$show_repeat_user) {
      		if ($last_flagging_user == $flagging_user) {
      			$last_flagging_user = $flagging_user;
      			continue;
      		}
      	}
      		
      	// keep track of last flagged user
      	$last_flagging_user = $flagging_user;

		//print($flag->name);
		switch ($flag->name) {

			// NEW MEMBER APPROVED
			case "approval_approved_by":

		    	if (isset($flagged_user->field_avatar[LANGUAGE_NONE][0]['uri'])) {
		    		$image_url = file_create_url($flagged_user->field_avatar[LANGUAGE_NONE][0]['uri']);
		    	} else {
		    		$image_url = file_create_url("public://default_images/avatar-default.png");
		    	}

		    	if (isset($flagged_user->field_location_city[LANGUAGE_NONE][0]['value'])) {
		    		$flagged_user_location = $flagged_user->field_location_city[LANGUAGE_NONE][0]['value'];
		    		$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>, " . strip_tags($flagged_user_location);
		    	} else {
		    		$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>";
		    	}

				$feed_title = l($flagged_user_name, $flagged_user_url) . " joined the community";
		        $html = tm_render_flag_feed($flagged_user_url, $image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items);
		        break;

		   	// MEMBER REGISTERED FOR EVENT
		    case "event_register":

		    	$event = $flagged_node;
		    	if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
		    		$image_url = file_create_url($event->field_image[LANGUAGE_NONE][0]['uri']);
		    	} else {
		    		$image_url = file_create_url("public://default_images/cover-default.png");
		    	}

		    	// if event has chapter
  				if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
    				$event_chapter = node_load($event->field_chapter[LANGUAGE_NONE][0]['target_id']);
    				$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>, " . $event_chapter->title;
  				} else {
  					$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>";
  				}

		    	$feed_title = l($flagging_user_name, $flagging_user_url) . " registered for " . l($event->title, $flagged_node_url);
		        $html = tm_render_flag_feed($flagged_node_url, $image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items);
		        break;

		    // MEMBER REGISTERED FOR WAITLIST
		    case "event_waitlist":

		    	$event = $flagged_node;
		    	if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
		    		$image_url = file_create_url($event->field_image[LANGUAGE_NONE][0]['uri']);
		    	} else {
		    		$image_url = file_create_url("public://default_images/cover-default.png");
		    	}

		    	// if event has chapter
  				if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
    				$event_chapter = node_load($event->field_chapter[LANGUAGE_NONE][0]['target_id']);
    				$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>, " . $event_chapter->title;
  				} else {
  					$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>";
  				}

		    	$feed_title = l($flagging_user_name, $flagging_user_url) . " joined the waitlist for " . l($event->title, $flagged_node_url);
		        $html = tm_render_flag_feed($flagged_node_url, $image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items);
		        break;

		    // MEMBER JOINED CHAPTER
		   	case "signup":

		   		$chapter = $flagged_node;
		    	if (isset($chapter->field_image[LANGUAGE_NONE][0]['uri'])) {
		    		$image_url = file_create_url($chapter->field_image[LANGUAGE_NONE][0]['uri']);
		    	} else {
		    		$image_url = file_create_url("public://default_images/cover-default.png");
		    	}
		    	$feed_title = l($flagging_user_name, $flagging_user_url) . " joined " . l($chapter->title . " " . $conf["tm_site_name"], $flagged_node_url);
		        $html = tm_render_flag_feed($flagged_node_url, $image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items);
		        break;

		    // MEMBER FOLLOWED SOMEONE
		    case "follow_members":

		    	if (isset($flagging_user->field_avatar[LANGUAGE_NONE][0]['uri'])) {
		    		$image_url = file_create_url($flagging_user->field_avatar[LANGUAGE_NONE][0]['uri']);
		    	} else {
		    		$image_url = file_create_url("public://default_images/avatar-default.png");
		    	}

		    	$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>";
				$feed_title = l($flagging_user_name, $flagging_user_url) . " followed " . l($flagged_user_name, $flagged_user_url);
		        //$feed_title = l($flagged_user_name, $flagged_user_url) . " was followed by " . l($flagging_user_name, $flagging_user_url);
		        $html = tm_render_flag_feed($flagged_user_url, $image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items);
		        break;

		   	// MEMBER FOLLOWED AN ORGANIZATION
		    case "follow_organizations":
		    	$organization = $flagged_node;
		    	if (isset($organization->field_avatar[LANGUAGE_NONE][0]['uri'])) {
		    		$image_url = file_create_url($organization->field_avatar[LANGUAGE_NONE][0]['uri']);
		    	} else {
		    		$image_url = file_create_url("public://default_images/avatar-default.png");
		    	}

		    	$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>";
				$feed_title = l($flagging_user_name, $flagging_user_url) . " followed " . l($organization->title, $flagged_node_url);
		        $html = tm_render_flag_feed($flagged_node_url, $image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items);
		        break;

		   	// ORGANIZATION FLAGGED AS A SPONSOR
		   	case "tm_sponsor":
		   		$organization = $flagged_node;
		    	if (isset($organization->field_avatar[LANGUAGE_NONE][0]['uri'])) {
		    		$image_url = file_create_url($organization->field_avatar[LANGUAGE_NONE][0]['uri']);
		    	} else {
		    		$image_url = file_create_url("public://default_images/avatar-default.png");
		    	}

		    	$feed_info = "<span class='flagfeed_ago'>" . $flagged_time . "</span>";
				$feed_title = l($organization->title, $flagged_node_url) . " just became a sponsor, thanks!";
		        $html = tm_render_flag_feed($flagged_node_url, $image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items);
		        break;
		}

		$feed_html_items[] = $html;
		$feed_item_count++;

	}

	
	$feed_html = implode("\n", $feed_html_items);
	$tm_feeds_template = str_replace("__FEED_ITEMS__", $feed_html, $tm_feeds_template);
	
	// store in cache
	// for public users
	if (!user_is_logged_in()) {
		if ($cache_time > 0) {
			cache_set($cache_key, $tm_feeds_template, 'cache', time() + $cache_time);
		}
	}

	// output
	print($tm_feeds_template);
}

function tm_render_flag_feed($feed_url, $feed_image_url, $feed_title, $feed_info, $feed_item_count, $display_num_items) {
	global $tm_feed_template;
	$html = str_replace("__FEED_URL__", $feed_url, $tm_feed_template);
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

global $tm_feeds_template;
$tm_feeds_template = <<<EOT
<!--<div class="row" style="margin-top: 1em; margin-bottom: 0px;">
	<div class="column first" style="float: right;">-->
		<section class="contained contained-block flagfeed">
			<header class="contained-head">
				<h1 class="prime-title flagfeed top">COMMUNITY FEED</h1>
			</header>
			<div class="contained-body flagfeed">
				<ul class="user-list related-list">
					__FEED_ITEMS__
				</ul>
			</div>
			<div class="more-link" style="font-size: 14px;">
  				<a id='flagfeedShowAll' href='javascript:flagfeedShowAll();'>Show more</a>&nbsp;
				<a id='flagfeedViewCommunity' style='display:none;' href='/community'>View the community</a>&nbsp;
			</div>
		</section>
	<!--</div>
</div>-->
EOT;

global $tm_feed_template;
$tm_feed_template = <<<EOT
<li class="flagfeed __FEED_LI_CLASS__" style="__FEED_LI_STYLE__">
   <article class="card contained view-mode-grid flagfeed clearfix">
    <!-- Needed to activate contextual links -->
    	<div style="padding-left: 2em; padding-right: 4em;">
    	
	        <div class="media">
		        <div class="avatar">
		        	<span class="badge-flagfeed">
		        		 <a href="__FEED_URL__" class="flagfeed"><img class="flagfeed-image" typeof="foaf:Image" src="__FEED_IMAGE_URL__" width="256" height="256" alt=""></a>
		        	</span>
		        </div>
	        </div>

	    	<div class="teaser">
	      		<span class="prime-title flagfeed">__FEED_TITLE__</span>
	      		<p class="meta flagfeed"><span class="role">__FEED_INFO__</span>
	      	</div>
	     
      	</div>
      	
	</article>
</li>
EOT;

tm_show_flagfeeds();

?>




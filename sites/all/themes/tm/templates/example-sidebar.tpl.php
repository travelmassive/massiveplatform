<?php

    // This is a custom sidebar

    // HOW TO ENABLE THE SIDEBAR
    // Edit your settings.php:
    // $conf['tm_theme_custom_sidebar_template'] = 'custom-sidebar.tpl.php';

    // HOW TO EDIT THE SIDEBAR
    // Make a copy of this file (ie: 'your-custom-sidebar.tpl.php') and update $conf['tm_theme_custom_sidebar_template']

    global $conf;
    global $user;

    $account = null;
    $user_score = 0;
    $reason_for_joining = "";

    // Load account
    if (user_is_logged_in($user->uid)) {
        $account = user_load($user->uid);

        $reason_for_joining = "";
        if (isset($account->field_reason_for_joining[LANGUAGE_NONE][0]['value'])) {
          $reason_for_joining = $account->field_reason_for_joining[LANGUAGE_NONE][0]['value'];
        }

        $user_score = tm_users_signup_score();
    }
?>
<div id="tm_sidenav_toggle">
    <a style="text-decoration: none;" href="#top" onClick="javascript:toggleSideBar();"><span class="tm_sidenav_toggle_icon"></span></a>
</div>

<div id="tm_sidenav" <?php if (user_is_logged_in()) { if (in_array("administrator", $account->roles)) { print("style='top: 64px;'"); } } ?>>

	<a id="tm_sidenav_logo_link" href="/"><img id="tm_sidenav_logo" src="<?php echo tm_branding_get_element("header_logo");?>" alt="Logo"></a>   

	<div id='tm-user-hidden-fields' style='display: none;'>
		<input style='display: none;' id='reason_for_joining' value='<?php print urlencode($reason_for_joining);?>'>
		<input style='display: none;' id='current_user_score' value='<?php print($user_score); ?>'>
		<input style='display: none;' id='current_user_uid' value='<?php print($user->uid); ?>'>
	</div>

	<ul id="tm_sidenav_menu">
		<li><form method="GET" action="/search"><input id="tm_sidenav_search" name="query" type="text" placeholder="Search" style="" autocomplete="off"></input></form></li>
		<li><a href="/"><span class="tm_sidenav_home_icon tm_sidenav_icon"></span>Home</a></li>
		<li><a href="/events"><span class="tm_sidenav_events_icon tm_sidenav_icon"></span>Events</a></li>
        <li><a href="/chapters">Chapters</a>
        <?php 
		// add link to home chapter
		if (user_is_logged_in()) { 
			if (isset($account->field_home_chapter[LANGUAGE_NONE][0]['target_id'])) {
				$home_chapter = node_load($account->field_home_chapter[LANGUAGE_NONE][0]['target_id']);
				$home_chapter_url = drupal_get_path_alias('node/' . $home_chapter->nid);
				$home_chapter_code = $home_chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['value'];
				$home_chapter_color = $home_chapter->field_chapter_color[LANGUAGE_NONE][0]['rgb'];
				echo "<a style='font-size: smaller; color: #fff; background-color: " . $home_chapter_color . "; border-radius: 4px;' href='/" . $home_chapter_url . "'>" . $home_chapter_code . "</a>";
			} // end if set home chapter
		} // end if logged in
		?>
		</li>
		<li><a href="/companies">Companies</a></li>
		<li><a href="/blog/">Blog</a></li>
		<li><span style="margin-left: 10px; font-weight: 200; color: #808080;">—</span></li>
		<?php if (user_is_logged_in()) { ?>
		<li><a href="/user"><span class="tm_sidenav_profile_icon tm_sidenav_icon"></span>Profile</a>
		<?php
		if (user_is_logged_in()) { 

			// show badge for chapter leader
			if (in_array("chapter leader", $account->roles)) {
					echo "<a class='tm-sidebar-leader-badge' href='/leaders'>LEADER</a>";
			} 

			// show badge for subscriptions
			else if (module_exists("tm_subscriptions_user")) {
				if ($conf["tm_subscriptions_user_enabled"]) {
					if (tm_subscriptions_is_user_subscription_enabled($account->uid)) {
						echo "<a class='tm-sidebar-pro-member-badge' href='/" . $conf['tm_checkout_subscription_confirm_page'] . "'>Pro</a>";
					} 
					else {
						echo "<a class='tm-sidebar-pro-member-upgrade' href='/checkout/upgrade'>Get Pro</a>";
					}
				}
			}
		}	
		?>
		</li>
		<li><a href="/user/<?php print($account->uid);?>/edit"><span class="tm_sidenav_settings_icon tm_sidenav_icon"></span>Settings</a></li>

		<?php if (!in_array("approved user", $account->roles)) { 
			// show last time request info was flagged
			$who_flagged = flag_get_entity_flags("user", $account->uid, "approval_requested_by_user");
			if (sizeof($who_flagged) > 0) {
				foreach ($who_flagged as $flagger) {
				$difference = time() - $flagger->timestamp;
			}
			$flagged_time = format_interval($difference, 1) . " ago";
		?>
		<li><?php print l(t('Verification'), 'javascript:jq_approval_already_requested();', array('fragment' => '','external'=>true, 'attributes' => array('class' => 'tm-account-menu-verification'))); ?></li>
			<?php } else { ?>
		<li><?php
			print l('Verification', 'javascript:jq_request_approval(' . $account->uid . ')', array('fragment' => '','external'=>true, 'attributes' => array('class' => array('approval-link', 'tm-account-menu-verification')))); ?></li>
		<?php
			} // end if flagged
		} // end if not approved
		?>
        
		<li><a href="/user/logout">Log out</a></li>
		<?php } ?>
		<?php if (!user_is_logged_in()) { ?>
		<li class="tm_sidenav_login"><span class="tm_sidenav_sign_in_icon tm_sidenav_icon"></span><a rel="nofollow" href="/user/login?destination=<?php print(drupal_get_path_alias());?>">Log in</a>/<a rel="nofollow" href="/user/register?destination=<?php print(drupal_get_path_alias());?>">Sign up</a></li>
		<?php } ?>
		<li><a href="#footer-more"><span class="tm_sidenav_more_icon tm_sidenav_icon"></span>More</a></li>

	    <?php

	    // join user subscriber and company subscriber links
		$menu_links = array();

	    if (user_is_logged_in()) {

			// user links
			if (isset($conf["tm_users_account_menu_links"])) {
				foreach($conf["tm_users_account_menu_links"] as $account_menu_title => $account_menu_link) {
					$menu_links[$account_menu_title] = $account_menu_link;
				}
			}

			// chapter leader links
			if (in_array("chapter leader", $account->roles) or in_array("moderator", $account->roles)) {
	     		if (isset($conf["tm_users_chapter_leader_menu_links"])) {
	            	foreach($conf["tm_users_chapter_leader_menu_links"] as $account_menu_title => $account_menu_link) {
	            		$menu_links[$account_menu_title] = $account_menu_link;
	            	}
	            }
	        }

			// subscriber links
			if (module_exists("tm_subscriptions_user")) {
				if (tm_subscriptions_is_user_subscription_enabled($account->uid)) { 
					if (isset($conf["tm_users_subscriber_menu_links"])) {
						foreach($conf["tm_users_subscriber_menu_links"] as $account_menu_title => $account_menu_link) {
							$menu_links[$account_menu_title] = $account_menu_link;
						}
					}
				}
			}

			// print links
			if (sizeof($menu_links) > 0) {
			?>
			<li><span style="margin-left: 10px; font-weight: 200; color: #808080;">—</span></li>
			<?php
			}
			foreach($menu_links as $account_menu_title => $account_menu_link) {
				print "<li>" . l(t($account_menu_title), $account_menu_link, array('fragment' => '', 'external' =>true, 'html' => true)) . "</li>";
			}

			?>

			<?php if ((in_array("moderator", $account->roles)) or (in_array("administrator", $account->roles))) : ?>
     
	      	<select name="tm_sidenav_moderator_options" id="tm_sidenav_moderator_options" onChange="this.options[this.selectedIndex].value && (window.location = this.options[this.selectedIndex].value);">
	      		<option value=''>Moderation</option>
	      		<?php if (module_exists("tm_checkout")) { ?>
          			<option value='/checkout/reports/all-payments/30'>Payments</option>
       			<?php } // end if ?>
	      		<option value='/invite'>Invite members</option>
		        <option value='/node/new/event'>Add event</option>
		        <option value='/node/add/chapter'>Add chapter</option>
		        <option value='/events/list/past'>Past events</option>
		        <option value='/events/list/preview'>Preview events</option>
		        <option value='/admin/unapproved-members'>All guests</option>
		        <option value='/admin/tm_reports'>Chapter leaders</option>
		        <option value='/admin/global_insights'>Global insights</option>
		        <option value='/feedback/results/all'>Member feedback</option>
		        <option value='/admin/member-testimonials'>Member testimonials</option>
		        <option value='/admin/member-covid19-messages'>Member COVID-19 messages</option>
		        <option value='/admin/company-covid19-messages'>Company COVID-19 messages</option>
	    	
		        <?php if (in_array("brand-editor", $account->roles) or in_array("administrator", $account->roles)): ?>
		        <option value='/admin/branding'>Manage branding</option>
		        <?php endif; ?>
		        <option value='/admin/export_chapter_insights'>Download chapter insights</option>
		        <?php if (tm_users_download_global_newsletter_csv_check()): ?>
		        <option value='/admin/export_global_newsletter'>Download global newsletter</option>
		        <?php endif; ?>
		        <?php if (tm_users_download_chapter_leaders_csv_check()): ?>
		        <option value='/admin/export_chapter_leaders'>Download chapter leaders</option>
		        <?php endif; ?>
		        <?php if (tm_users_download_facebook_csv_check()): ?>
		        <option value='/admin/export_facebook_customers'>Export Facebook customer data</option>
		        <?php endif; ?>
	      	</select>
	  		
	  		<?php endif; ?>

  		<?php } // end if logged in ?>

	</ul>

	<ul id='tm_sidenav_icons' class="<?php if (user_is_logged_in()) { print('signed_in'); }?>">
        <li><a href='http://twitter.com/' title='Massive Platform on Twitter' class='twitter tm_icon' target='_blank' rel='nofollow,noopener'></a></li>
        <li><a href='https://www.facebook.com/' title='Massive Platform on Facebook' class='facebook tm_icon' target='_blank' rel='nofollow,noopener'></a></li>
        <li><a href='http://instagram.com/' title='Massive Platform on Instagram' class='instagram tm_icon' target='_blank' rel='nofollow,noopener'></a></li>
        <li><a href='http://www.linkedin.com/' title='Massive Platform on Linkedin' class='linkedin tm_icon' target='_blank' rel='nofollow,noopener'></a></li>
        <li><a href='https://www.youtube.com/' title='Massive Platform on YouTube' class='youtube tm_icon' target='_blank' rel='nofollow,noopener'></a></li>
    </ul>

</div>
<div id="account-menu-blk" class="tm-sidebar-login" style="display: none;">
	<span style="font-size: larger; font-weight: bold;"><a href="/user/login?destination=<?php print(current_path());?>" style="color: #176CF6;">Log in</a> to continue...</span><br>
	<hr style="font-weight: 200; color: #808080; border: none; border-bottom: 1px solid #808080; margin: 16px;">
	<span><b>New to Massive Platform?</b> It only takes a few minutes to <a href="/user/register?destination=<?php print(current_path());?>" style="color: #176CF6;">sign up</a>.</span>
</div>

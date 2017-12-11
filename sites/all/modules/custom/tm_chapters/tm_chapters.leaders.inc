<?php

// tm_chapters.leaders.inc - chapter leaders management and notification methods

/**
 * Handle change to chapter leaders
 * - Add chapter leader role
 * - Revoke chapter leader role
 * - Send notifications
*/
function tm_chapters_leaders_chapter_save($chapter) {

	global $user;

	$user_who_changed = user_load($user->uid);

	// STEP 1. Add chapter leader roles
	$chapter_leader_role = user_role_load_by_name('chapter leader');
	if (isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
		foreach ($chapter->field_chapter_leaders[LANGUAGE_NONE] as $target){

			// don't edit admin or moderator roles unless editing themselves
			$account = user_load($target['target_id']);
			if (($account->uid == $user->uid) or 
				(!(in_array("moderator", $account->roles) or in_array("administrator", $account->roles)))) {
					user_multiple_role_edit(array($account->uid), 'add_role', $chapter_leader_role->rid);
			}
		}
	}

	// STEP 2. Determine new and retired chapter leaders
	$original = array();
	if (isset($chapter->original->field_chapter_leaders[LANGUAGE_NONE])) {
		$original = flatten_chapter_leaders($chapter->original->field_chapter_leaders[LANGUAGE_NONE]);
	}
	$new = array();
	if (isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
		$new = flatten_chapter_leaders($chapter->field_chapter_leaders[LANGUAGE_NONE]);
	}

	$new_leaders = array_diff($new, $original);
	$retired_leaders = array_diff($original, $new);

	// STEP 3. Remove role of any old chapter leaders
	foreach ($retired_leaders as $retired_leader) {

		// check first that leader is not in other chapters
		$chapters = db_select('field_data_field_chapter_leaders', 'd')
		->fields('d')
		->condition('field_chapter_leaders_target_id',$retired_leader,'=')
		->condition('deleted',0,'=')
		->execute();
		if ($chapters->rowCount() == 1) {

			// don't edit admin or moderator roles unless editing themselves
			$account = user_load($retired_leader);
			if (($account->uid == $user->uid) or 
				(!(in_array("moderator", $account->roles) or in_array("administrator", $account->roles)))) {
					user_multiple_role_edit(array($retired_leader), 'remove_role', $chapter_leader_role->rid);
			}
		}
	}

	// STEP 4. Notify new chapter leaders
	foreach ($new_leaders as $new_leader) {
		$account = user_load($new_leader);
		tm_chapters_leaders_notify_add_leader($account, $chapter);
	}

	// STEP 5. Notify retired chapter leaders
	foreach ($retired_leaders as $retired_leader) {
		$account = user_load($retired_leader);
		if ($user_who_changed->uid == $account->uid) {
			$retired_intro = "You've removed yourself as a chapter leader of";
		} else {
			$retired_intro = "Due to account inactivity, we've retired your leadership of";
			if (!drupal_is_cli()) {
				$retired_intro = strip_tags($user_who_changed->field_user_first_name[LANGUAGE_NONE][0]['value']) . " has retired your leadership of";
			}
		}
		tm_chapters_leaders_notify_retire_leader($account, $chapter, $retired_intro);
	}

	// STEP 6. Notify chapter leaders
	if ((sizeof($new_leaders) > 0) or (sizeof($retired_leaders) > 0)) {
		tm_chapters_leaders_notify_changes($user_who_changed, $chapter);
	}
	
}

/**
 * Notify chapter leaders, community manager and leaders of new change to chapter
 */
function tm_chapters_leaders_notify_changes($user_who_changed, $chapter) {

	global $conf;

	$subject = 'Chapter leader changes: __CHAPTER_NAME__';
	$body = 'Hi __SALUTATION__,
<br>
<br>__USER_FIRST_NAME__ __USER_LAST_NAME__ has made changes to your chapter: __CHAPTER_NAME__.
<br>__REASON__
<br>The new chapter leadership team is:
<br>__CHAPTER_LEADERS_LIST__
<br>
<br>View your chapter here:
<br>
<a href="__SERVER_HTTP__://__SERVER_NAME__/__CHAPTER_URL__">__SERVER_NAME__/__CHAPTER_URL__</a> 
<br>';

	if ($conf["tm_chapters_allow_edit_chapter_leaders"]) {
		$body .= "<br>You can add or remove chapter leaders at any time from your chapter settings.<br>";
	}

	// get reason
	$reason = "";
	if (isset($chapter->field_chapter_management_reason[LANGUAGE_NONE][0]['value'])) {
		$reason = "<br>Reason: " . strip_tags($chapter->field_chapter_management_reason[LANGUAGE_NONE][0]['value']) . "<br>";
	}

	// get username who made changes
	if (drupal_is_cli()) {
		$first_name = "Our";
		$last_name = "community management team";
		$reason = "<br>Reason: Inactive account<br>";
	} else {
		$first_name = strip_tags($user_who_changed->field_user_first_name[LANGUAGE_NONE][0]['value']);
		$last_name = strip_tags($user_who_changed->field_user_last_name[LANGUAGE_NONE][0]['value']);
	}
	

	// get formal chapter name
	$chapter_name = tm_chapters_formal_name($chapter);
	$chapter_url = drupal_get_path_alias('node/' . $chapter->nid);

	// notify chapter leaders
	$notify_uids = array();
	if (isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
		foreach ($chapter->field_chapter_leaders[LANGUAGE_NONE] as $chapter_leader) {
			$notify_uids[] = $chapter_leader['target_id'];
		}
	}

	// generate chapter leader list
	$chapter_leaders_list = "";
	foreach ($notify_uids as $notify_uid) {
		$chapter_leader = user_load($notify_uid);
		$chapter_leaders_list .= "<br>&middot; " . strip_tags($chapter_leader->field_user_first_name[LANGUAGE_NONE][0]['value']) . " " . strip_tags($chapter_leader->field_user_last_name[LANGUAGE_NONE][0]['value']); 
	}
	if ($chapter_leaders_list == "") {
		$chapter_leaders_list = "&middot; No chapter leaders";
	}

	// notify community managers
	if (isset($chapter->field_community_managers[LANGUAGE_NONE])) {
		foreach ($chapter->field_community_managers[LANGUAGE_NONE] as $community_manager) {
			$notify_uids[] = $community_manager['target_id'];
		}
	}

	// unique ids
	$notify_uids = array_unique($notify_uids);

	// salutation
	$salutation = "team";
	if (sizeof($notify_uids) == 1) {
		$single_user = user_load($notify_uids[0]); 
		$salutation = strip_tags($single_user->field_user_first_name[LANGUAGE_NONE][0]['value']);
	}

	// get addreses
	$notify_emails = array();
	foreach ($notify_uids as $uid) {
		$notify_user = user_load($uid);
		$notify_emails[] = $notify_user->mail;
	}

	// if no chapter leaders or community managers, send to support email
	if (sizeof($notify_emails) == 0) {
		$notify_emails[] = $conf['tm_chapters_support_email'];
	}

	// replace template vars
	$subject = str_replace("__CHAPTER_NAME__", $chapter_name, $subject);
	$body = str_replace("__SALUTATION__", $salutation, $body);
	$body = str_replace("__CHAPTER_NAME__", $chapter_name, $body);
	$body = str_replace("__CHAPTER_URL__", $chapter_url, $body);
	$body = str_replace("__USER_FIRST_NAME__", $first_name, $body);
	$body = str_replace("__USER_LAST_NAME__", $last_name, $body);
	$body = str_replace("__REASON__", $reason, $body);
	$body = str_replace("__CHAPTER_LEADERS_LIST__", $chapter_leaders_list, $body);


	// notify chapter leaders and community managers
	tm_chapters_leaders_send_notification($notify_emails, $subject, $body, $chapter);
}

/**
 * Notify chapter leader that they've been added to a chapter
 */
function tm_chapters_leaders_notify_add_leader($new_leader, $chapter) {

	global $conf;

	$subject = 'You\'re now a chapter leader of __CHAPTER_NAME__';
	$body = 'Hi __FIRST_NAME__,
<br>
<br>Congratulations, you have been appointed as a chapter leader of __CHAPTER_NAME__.
<br>
<br>__CHAPTER_LEADER_WELCOME_MESSAGE__
<br>
<br>View your chapter here:
<br>
<a href="__SERVER_HTTP__://__SERVER_NAME__/__CHAPTER_URL__">__SERVER_NAME__/__CHAPTER_URL__</a> 
<br>';

	// get first name of new leader
	$first_name = strip_tags($new_leader->field_user_first_name[LANGUAGE_NONE][0]['value']);

	// chapter welcome message
	$chapter_leader_welcome_message = $conf["tm_chapters_leaders_welcome_message"];

	// get formal chapter name
	$chapter_name = tm_chapters_formal_name($chapter);
	$chapter_url = drupal_get_path_alias('node/' . $chapter->nid);

	// cc community managers
	$cc_uids = array();
	if (isset($chapter->field_community_managers[LANGUAGE_NONE])) {
		foreach ($chapter->field_community_managers[LANGUAGE_NONE] as $community_manager) {
			$cc_uids[] = $community_manager['target_id'];
		}
	}

	// unique ids
	$cc_uids = array_unique($cc_uids);

	// get addreses
	$cc_emails = array();
	foreach ($cc_uids as $uid) {
		$cc_user = user_load($uid);
		$cc_emails[] = $cc_user->mail;
	}

	// get addreses
	$notify_emails = array();
	$notify_emails[] = $new_leader->mail;

	// replace template vars
	$subject = str_replace("__CHAPTER_NAME__", $chapter_name, $subject);
	$body = str_replace("__CHAPTER_LEADER_WELCOME_MESSAGE__", $chapter_leader_welcome_message, $body);
	$body = str_replace("__CHAPTER_NAME__", $chapter_name, $body);
	$body = str_replace("__CHAPTER_URL__", $chapter_url, $body);
	$body = str_replace("__FIRST_NAME__", $first_name, $body);

	// notify chapter leaders and community managers
	tm_chapters_leaders_send_notification($notify_emails, $subject, $body, $chapter, $cc_emails);
}

/**
 * Notify chapter leader that they've been retired as a chapter
 */
function tm_chapters_leaders_notify_retire_leader($retired_leader, $chapter, $retired_intro = "", $retired_message = "") {

	global $conf;

	$subject = 'Thanks for being a chapter leader of __CHAPTER_NAME__';
	$body = 'Hi __FIRST_NAME__,
<br>
<br>__RETIRED_INTRO__ __CHAPTER_NAME__.
<br>
<br>__CHAPTER_LEADER_RETIRED_MESSAGE__
<br>
<br>View the chapter here:
<br>
<a href="__SERVER_HTTP__://__SERVER_NAME__/__CHAPTER_URL__">__SERVER_NAME__/__CHAPTER_URL__</a> 
<br>';

	// who retired text
	if ($retired_intro == "") {
		$retired_intro = "You've been retired as chapter leader of"; // We've retired your chapter leadership of X
	}

	// chapter retire message
	if ($retired_message == "") {
		$retired_message = $conf["tm_chapters_leaders_retired_message"];
	}

	// get first name of leader
	$first_name = strip_tags($retired_leader->field_user_first_name[LANGUAGE_NONE][0]['value']);

	// get formal chapter name
	$chapter_name = tm_chapters_formal_name($chapter);
	$chapter_url = drupal_get_path_alias('node/' . $chapter->nid);

	// cc community managers
	$cc_uids = array();
	if (isset($chapter->field_community_managers[LANGUAGE_NONE])) {
		foreach ($chapter->field_community_managers[LANGUAGE_NONE] as $community_manager) {
			$cc_uids[] = $community_manager['target_id'];
		}
	}

	// unique ids
	$cc_uids = array_unique($cc_uids);

	// get addreses
	$cc_emails = array();
	foreach ($cc_uids as $uid) {
		$cc_user = user_load($uid);
		$cc_emails[] = $cc_user->mail;
	}

	// get addreses
	$notify_emails = array();
	$notify_emails[] = $retired_leader->mail;

	// replace template vars
	$subject = str_replace("__CHAPTER_NAME__", $chapter_name, $subject);
	$body = str_replace("__RETIRED_INTRO__", $retired_intro, $body);
	$body = str_replace("__CHAPTER_LEADER_RETIRED_MESSAGE__", $retired_message, $body);
	$body = str_replace("__CHAPTER_NAME__", $chapter_name, $body);
	$body = str_replace("__CHAPTER_URL__", $chapter_url, $body);
	$body = str_replace("__FIRST_NAME__", $first_name, $body);

	// notify chapter leaders and community managers
	tm_chapters_leaders_send_notification($notify_emails, $subject, $body, $chapter, $cc_emails);
}

/**
 * Email sending function
 */
function tm_chapters_leaders_send_notification($notification_emails, $subject, $body, $chapter, $cc_emails = array()) {

	global $conf;

	// SEND EMAIL
	$module = 'tm_chapters';
	$key = 'key';
	$language = language_default();
	$cc = "";
	$params = array();
	if (sizeof($cc_emails) > 0) {
		$cc = "," . implode(",", $cc_emails);
		$params["headers"] = array("Cc" => $cc);
	}
	$from = $conf['tm_chapters_support_email'];
	// $reply = $conf['tm_chapters_support_email'];
	$send = FALSE;

	// CONSTRUCT MESSAGE
	$message = $body;
	$message .= "<br>__EMAIL_SIGNOFF__";

	// STEP 1. LOAD HTML TEMPLATE
	$template_file = drupal_get_path('module', 'tm_notifications') . '/templates/tm_notifications-email_template.tpl.php';
	$template = file_get_contents($template_file);

	// delimit chapter leader emails
	$email = implode(",", $notification_emails);

	// STEP 2. SET MAIL HTML HEADERS
	$email_message = drupal_mail($module, $key, $email, $language, $params, NULL, $send);
	$email_message['headers']['MIME-Version'] = '1.0';
	$email_message['headers']['Content-Type'] = 'multipart/mixed;';
	$email_message['headers']['Content-Type'] = 'text/html;';
	// $email_message['headers']['Reply-to'] = $reply; // set reply-to to the user
	$email_message['headers']['From'] = $from;

	// STEP 3. SET SUBJECT 
	$email_message['subject'] = check_plain($subject);

	// STEP 4. REPLACE BODY AND FOOTER IN HTML TEMPLATE
	$template = str_replace("__EMAIL_BODY__", $message, $template);
	$footer = $conf["tm_email_default_footer"];
	if (isset ($params['footer'])) {
		$footer = $params['footer'];
	}
	$template = str_replace("__EMAIL_FOOTER__", $footer, $template);

	// Chapter hero image
	$chapter_image_uri = null;

	// check chapter->original if saving a node
	if (isset($chapter->original->field_image[LANGUAGE_NONE][0]['uri'])) {
		$chapter_image_uri = $chapter->original->field_image[LANGUAGE_NONE][0]['uri'];
	}

	// check chapter object
	if (isset($chapter->field_image[LANGUAGE_NONE][0]['uri'])) {
		$chapter_image_uri = $chapter->field_image[LANGUAGE_NONE][0]['uri'];
	}

	if ($chapter_image_uri != null) {
	  $chapter_image_src = image_style_url("banner", $chapter_image_uri);
	} else {
	  $default_image_uri = _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $chapter->nid);
	  $chapter_image_src = image_style_url("banner", $default_image_uri);
	}

	$chapter_name = tm_chapters_formal_name($chapter);
	$chapter_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $chapter->nid);

	$chapter_image_html = tm_notifications_render_hero_image_html($chapter_image_src, $chapter_url, $chapter_name);

	// Other vars
	$template = str_replace("__UNSUBSCRIBE_LINK__", "", $template); 
	$template = str_replace("__SERVER_NAME__", $conf["tm_email_server_url_domain"], $template); 
	$template = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $template);  
	$template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $template);  
	$template = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $template); 
	$template = str_replace("__EMAIL_BRAND_MESSAGE__", "", $template); // disable brand message
	$template = str_replace("__HERO_IMAGE__", $chapter_image_html, $template); // remove hero image

	// STEP 5. LOAD HTML TEMPLATE INTO EMAIL BODY
	$email_message['body'] = $template;

	// STEP 6. SEND EMAIL
	// Retrieve the responsible implementation for this message.
	$system = drupal_mail_system($module, $key);

	// Format the message body.
	$format_message = $system->format($email_message);

	// Send e-mail.
	$result = $system->mail($format_message);
}

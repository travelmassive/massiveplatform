<?php


// tm_subscriptions.notifications.inc - notification methods

// TODO

// email to notify of subscription upgrade
// email to send receipt of payment
// email with monthly stats of views?

/**
 * Sends out a single email
 */
function tm_subscriptions_send_email($uid, $email, $subject, $message, $chapter_image_src, $chapter_image_link, $chapterid, $replyto="") {

	global $conf;

	$module = 'tm_subscriptions';
	$key = 'key';
	$language = language_default();
	$params = array();
	$send = FALSE;


	// STEP 1. LOAD HTML TEMPLATE
	$template_file = drupal_get_path('module', 'tm_notifications') . '/templates/tm_notifications-email_template.tpl.php';
	$template = file_get_contents($template_file);


	// STEP 2. SET MAIL HTML HEADERS
	$email_message = drupal_mail($module, $key, $email, $language, $params, NULL, $send);
	$email_message['headers']['MIME-Version'] = '1.0';
	$email_message['headers']['Content-Type'] = 'multipart/mixed;';
	$email_message['headers']['Content-Type'] = 'text/html;';


	// STEP 3. SET SUBJECT
	global $conf;
	$subject = str_replace("__SERVER_NAME__", $_SERVER["SERVER_NAME"], $subject);
	$subject = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $subject);  
	$subject = str_replace("__SITE_NAME__", $conf["tm_site_name"], $subject);  
	$email_message['subject'] = check_plain($subject);


	// STEP 4. REPLACE BODY AND FOOTER IN HTML TEMPLATE
	$template = str_replace("__EMAIL_BODY__", $message, $template);
	global $conf;
	$footer = $conf["tm_email_default_footer"];
	if (isset ($params['footer'])) {
	  $footer = $params['footer'];
	}
	$template = str_replace("__EMAIL_FOOTER__", $footer, $template);

	// STEP 5. ADD UNSUBSCRIBE LINK
	if ($uid != null) {
	 $secret_hash = tm_notifications_generate_secret_hash($uid, "user_chapter_announcements");
	 $unsubscribe_url = "user/" . $uid . "/unsubscribe/" . "user_chapter_announcements" . "/" . $secret_hash;
	 $unsubscribe_link = "<a href='__SERVER_HTTP__://__SERVER_NAME__/" . $unsubscribe_url . "'>Unsubscribe from chapter announcements</a>";
	} else {
	$unsubscribe_link = "Update Your Notification Settings at <a href='__SERVER_HTTP__://__SERVER_NAME__'>__SERVER_NAME__</a>"; 
	}
	$template = str_replace("__UNSUBSCRIBE_LINK__", $unsubscribe_link, $template);


	$secret_hash = tm_notifications_generate_secret_hash($account->uid, $notification_name); 
	$tokens["UNSUBSCRIBE_NOTIFICATION_LINK"] = "user/" . $account->uid . "/unsubscribe/" . $notification_name . "/" . $secret_hash;

	$unsubscribe_link = "<a href='__SERVER_HTTP__://__SERVER_NAME__/" . $params['tokens']['UNSUBSCRIBE_NOTIFICATION_LINK'] . "'>Unsubscribe from this notification</a>";
	$template = str_replace("__UNSUBSCRIBE_LINK__", $unsubscribe_link, $template);

	// Other vars
	$template = str_replace("__SERVER_NAME__", $conf["tm_email_server_url_domain"], $template); 
	$template = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $template);  
	$template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $template);  
	$template = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $template);  
	$template = str_replace("__EMAIL_BRAND_MESSAGE__", "", $template); // disable brand message

	// Chapter image
	// Render with tm_chapters_render_hero_image_html()
	if ($chapter_image_src != null) {
	$chapter_image_html = tm_notifications_render_hero_image_html($chapter_image_src, $chapter_image_link, "Chapter image");
		$template = str_replace("__HERO_IMAGE__", $chapter_image_html, $template);
	} else {
		$template = str_replace("__HERO_IMAGE__", "", $template); // disable brand message
	}

	// STEP 5. LOAD HTML TEMPLATE INTO EMAIL BODY
	$email_message['body'] = $template;


	// STEP 6. SET REPLY TO
	// If blank, use tm_announcement_reply_email
	if ($replyto == "") {
	  $replyto = $conf["tm_announcement_reply_email"];
	}
	$email_message['headers']['Reply-To'] = $replyto;

	// STEP 6. SEND EMAIL
	// Retrieve the responsible implementation for this message.
	$system = drupal_mail_system($module, $key);

	// Format the message body.
	$format_message = $system->format($email_message);

	// Send e-mail.
	$result = $system->mail($format_message);

	// Send 1 email
	return 1;
}

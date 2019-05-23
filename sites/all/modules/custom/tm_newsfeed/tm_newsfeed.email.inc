<?php

// tm_newsfeed.email.inc - mail methods

/**
 * Send test newsfeed notificiation
 * Notification emails are sent via scheduled notifications
 */
function tm_newsfeed_notification_send_test_email($email, $subject, $body, $preheader_text = "", $headline_text = "") {

	global $conf;

	// SEND EMAIL
	$module = 'tm_newsfeed';
	$key = 'key';
	$language = language_default();
	$params = array();
	$send = FALSE;

	$message = $body;

	// STEP 1. LOAD HTML TEMPLATE
	$template_file = drupal_get_path('module', 'tm_notifications') . '/templates/tm_notifications-email_template.tpl.php';
	$template = file_get_contents($template_file);


	// STEP 2. SET MAIL HTML HEADERS
	$email_message = drupal_mail($module, $key, $email, $language, $params, NULL, $send);
	$email_message['headers']['MIME-Version'] = '1.0';
	$email_message['headers']['Content-Type'] = 'multipart/mixed;';
	$email_message['headers']['Content-Type'] = 'text/html;';
	$email_message['headers']['Reply-to'] = $reply; // set reply-to to the user

	// STEP 3. SET SUBJECT 
	$email_message['subject'] = '[Test] ' . check_plain($subject);

	// STEP 4. REPLACE BODY AND FOOTER IN HTML TEMPLATE
	$template = str_replace("__EMAIL_BODY__", $message, $template);
	$footer = $conf["tm_email_default_footer"];
	if (isset ($params['footer'])) {
		$footer = $params['footer'];
	}
	$template = str_replace("__EMAIL_FOOTER__", $footer, $template);

	// Other vars
	$template = str_replace("__UNSUBSCRIBE_LINK__", "", $template); // this is a test email
	$template = str_replace("__SERVER_NAME__", $conf["tm_email_server_url_domain"], $template); 
	$template = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $template);  
	$template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $template);  
	$template = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $template);
	$template = str_replace("__EMAIL_BRAND_MESSAGE__", "", $template); // disable site branding
	$template = str_replace("__HERO_IMAGE__", "", $template); // remove hero image
  	$template = str_replace("__PREHEADER_TEXT__", $preheader_text, $template); // Preheader text

  	// Headline
  	$headline_html = tm_notifications_render_hero_headline_html($headline_text);
  	$template = str_replace("__HERO_HEADLINE__", $headline_html, $template); // Hero headline

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


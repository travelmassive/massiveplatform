<?php


// tm_subscriptions_user.notifications.inc - notification methods

// TODO

// email to notify of subscription upgrade
// email to send receipt of payment
// email with monthly stats of views?

function tm_subscriptions_user_send_notification($account, $notification_type, $first_name = "") {

	global $conf;
	$uid = null;
	$subject = "";
	$message = "";

	// get country code
	$country_code = tm_subscriptions_user_get_country($account);

	// get account url
	$account_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('user/' . $account->uid);

	// get account uid url
	$account_uid_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . '/user/' . $account->uid;

	// payment receipt url
	$view_payments_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/checkout/my-payments/" . $account->uid;

	// first name of owner of account
	if ($first_name == "") {
		$first_name = $account->field_user_first_name[LANGUAGE_NONE][0]['value'];
	}

	// create email
	$account_emails = array($account->mail);

	// purchase template
	if ($notification_type == "purchase") {
		$subject = tm_subscriptions_get_user_subscription_field($account, "email_purchase_subject");
		$message = tm_subscriptions_get_user_subscription_field($account, "email_purchase_message");
	}

	// renewal template
	if ($notification_type == "renewed") {
		$subject = tm_subscriptions_get_user_subscription_field($account, "email_renewed_subject");
		$message = tm_subscriptions_get_user_subscription_field($account, "email_renewed_message");
	}

	// renewal template (reminder to renew)
	// todo: schedule this message once a week during renewal period
	if ($notification_type == "reminder") {
		$subject = tm_subscriptions_get_user_subscription_field($account, "email_reminder_subject");
		$message = tm_subscriptions_get_user_subscription_field($account, "email_reminder_message");
	}

	// expiry template
	if ($notification_type == "expired") {
		$subject = tm_subscriptions_get_user_subscription_field($account, "email_expired_subject");
		$message = tm_subscriptions_get_user_subscription_field($account, "email_expired_message");
	}

	// message template fields
	$message = str_replace("__FIRST_NAME__", strip_tags($first_name), $message); // first name
	$message = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $message); // email sign off
	$message = str_replace("__USER_PROFILE_URL__", $account_url, $message); // account url
	$message = str_replace("__USER_UID_URL__", $account_uid_url, $message); // account uid url
	$message = str_replace("__USER_ID__", $account->uid, $message); // account uid
	$message = str_replace("__VIEW_PAYMENTS_URL__", $view_payments_url, $message); // payments url
	$message = str_replace("\n", "<br>", $message); // line breaks

	// expiry date
	$subscription_date = tm_subscriptions_user_get_subscription_date($account);
	$subscription_date_text = date('Y-m-d', strtotime($subscription_date));
	$expires_text = tm_subscriptions_user_get_subscription_expires_text($account);
	$message = str_replace("__SUBSCRIPTION_EXPIRES_DATE__", $subscription_date_text, $message); // subscription expiry date
	$message = str_replace("__SUBSCRIPTION_EXPIRES_TEXT__", $expires_text, $message); // days until subscription expiry

	// reply to, cc, bcc
	// TODO: mail to chapter leader / community managers
	$replyto = tm_subscriptions_get_user_subscription_field($account, "email_replyto");
	$cc = "";
	$bcc = tm_subscriptions_get_user_subscription_field($account, "email_bcc");

	// hide hero image
	$account_image_src = "";
	$account_image_link = "";

	// send notification
	tm_subscriptions_user_send_email($account_emails, $subject, $message, $account_image_src, $account_image_link, $account->uid, $replyto, $cc, $bcc);

}

/**
 * Sends out a single email
 */
function tm_subscriptions_user_send_email($account_emails, $subject, $message, $account_image_src, $account_image_link, $uid, $replyto="", $cc="", $bcc="") {

	global $conf;

	$module = 'tm_subscriptions_user';
	$key = 'key';
	$language = language_default();
	$params = array();
	$send = FALSE;

	if ($cc != "") {
		$params["headers"] = array("Cc" => $cc);
	}
	if ($bcc != "") {
		$params["headers"] = array("Bcc" => $bcc);
	}


	// STEP 1. LOAD HTML TEMPLATE
	$template_file_name = 'tm_notifications-email_template.tpl.php';
	if (isset($conf["tm_email_custom_template"])) {
		$template_file_name = $conf["tm_email_custom_template"];
	}
	$template_file = drupal_get_path('module', 'tm_notifications') . '/templates/' . $template_file_name;
	$template = file_get_contents($template_file);

	// delimit account emails
	$email = implode(",", $account_emails);

	// STEP 2. SET MAIL HTML HEADERS
	$email_message = drupal_mail($module, $key, $email, $language, $params, NULL, $send);
	$email_message['headers']['MIME-Version'] = '1.0';
	$email_message['headers']['Content-Type'] = 'multipart/mixed;';
	$email_message['headers']['Content-Type'] = 'text/html;';


	// STEP 3. SET SUBJECT
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
	$template = str_replace("__UNSUBSCRIBE_LINK__", "", $template);

	// Other vars
	$template = str_replace("__SERVER_NAME__", $conf["tm_email_server_url_domain"], $template); 
	$template = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $template);  
	$template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $template);  
	$template = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $template);  
	$template = str_replace("__EMAIL_BRAND_MESSAGE__", "", $template); // disable brand message
 	$template = str_replace("__HERO_HEADLINE__", "", $template); // remove hero headline
    $template = str_replace("__PREHEADER_TEXT__", "", $template); // preheader text

	// Account image
	// Render with tm_notifications_render_hero_image_html()
	if ($account_image_src != null) {
	$account_image_html = tm_notifications_render_hero_image_html($account_image_src, $account_image_link, "Account image");
		$template = str_replace("__HERO_IMAGE__", $account_image_html, $template);
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

<?php


// tm_subscriptions.notifications.inc - notification methods

// TODO

// email to notify of subscription upgrade
// email to send receipt of payment
// email with monthly stats of views?

function tm_subscriptions_organization_send_notification($organization, $notification_type, $first_name = "", $show_hero_image = false) {

	global $conf;
	$uid = null;
	$subject = "";
	$message = "";

	// get country code
	$country_code = $organization->field_country[LANGUAGE_NONE][0]['iso2'];

	// get company url
	$company_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $organization->nid);

	// payment receipt url
	$view_payments_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/payment/reports/subscription-payments/" . $organization->nid;

	// first name of owner of account
	if ($first_name == "") {
		$first_name = tm_subscriptions_organization_get_owner_first_names($organization);
	}

	// create email
	$organization_emails = tm_subscriptions_organization_get_emails($organization, $country_code);

	// purchase template
	if ($notification_type == "purchase") {
		$subject = tm_subscriptions_get_organization_subscription_field($organization, "email_purchase_subject");
		$message = tm_subscriptions_get_organization_subscription_field($organization, "email_purchase_message");
	}

	// renewal template
	if ($notification_type == "renewed") {
		$subject = tm_subscriptions_get_organization_subscription_field($organization, "email_renewed_subject");
		$message = tm_subscriptions_get_organization_subscription_field($organization, "email_renewed_message");
	}

	// renewal template (reminder to renew)
	// todo: schedule this message once a week during renewal period
	if ($notification_type == "reminder") {
		$subject = tm_subscriptions_get_organization_subscription_field($organization, "email_reminder_subject");
		$message = tm_subscriptions_get_organization_subscription_field($organization, "email_reminder_message");
	}

	// expiry template
	if ($notification_type == "expired") {
		$subject = tm_subscriptions_get_organization_subscription_field($organization, "email_expired_subject");
		$message = tm_subscriptions_get_organization_subscription_field($organization, "email_expired_message");
	}

	// message template fields
	$message = str_replace("__FIRST_NAME__", strip_tags($first_name), $message); // first name
	$message = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $message); // email sign off
	$message = str_replace("__ORGANIZATION_PROFILE_URL__", $company_url, $message); // organization url
	$message = str_replace("__ORGANIZATION_ID__", $organization->nid, $message); // organization nid
	$message = str_replace("__VIEW_PAYMENTS_URL__", $view_payments_url, $message); // payments url
	$message = str_replace("\n", "<br>", $message); // line breaks

	// expiry date
	$subscription_date = tm_subscriptions_get_subscription_date($organization);
	$subscription_date_text = date('Y-m-d', strtotime($subscription_date));
	$expires_text = tm_subscriptions_get_subscription_expires_text($organization);
	$message = str_replace("__SUBSCRIPTION_EXPIRES_DATE__", $subscription_date_text, $message); // subscription expiry date
	$message = str_replace("__SUBSCRIPTION_EXPIRES_TEXT__", $expires_text, $message); // days until subscription expiry

	// reply to, cc, bcc
	$replyto = tm_subscriptions_get_organization_subscription_field($organization, "email_replyto");
	$cc = "";
	$bcc = tm_subscriptions_get_organization_subscription_field($organization, "email_bcc");

	// get company image
	if (isset($organization->field_image[LANGUAGE_NONE][0]['uri'])) {
	  $company_image_src = image_style_url("banner", $organization->field_image[LANGUAGE_NONE][0]['uri']);
	} else {
	  $default_image_uri = _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $organization->nid);
	  $company_image_src = image_style_url("banner", $default_image_uri);
	}

	// hide hero image
	if (!$show_hero_image) {
		$company_image_src = "";
		$company_image_link = "";
	}

	// send notification
	tm_subscriptions_organization_send_email($organization_emails, $subject, $message, $company_image_src, $company_image_link, $organization->nid, $replyto, $cc, $bcc);

}


/**
 * Determine who to send emails to
 */
function tm_subscriptions_organization_get_emails($organization) {

	global $conf;

	$organization_emails = array();

	// account contact emails
	if (isset($organization->field_company_account_email[LANGUAGE_NONE][0]['email'])) {
		$organization_emails[] = trim($organization->field_company_account_email[LANGUAGE_NONE][0]['email']);
	}

	// get organization owner emails
	if (isset($organization->field_owner[LANGUAGE_NONE])) {
	    foreach($organization->field_owner[LANGUAGE_NONE] as $manager){
	        $manager = user_load($manager['target_id']);
	        $organization_emails[] = $manager->mail;
	    }

	    $organization_emails = array_unique($organization_emails);
	}

	// if no-one to contact, send to contact page email
	if (sizeof($organization_emails) == 0) { 
	    $organization_emails[] = tm_subscriptions_get_organization_subscription_field($organization, "email_from");
	}

	return $organization_emails;
}

/**
 * Get the first names of the organization
 */
function tm_subscriptions_organization_get_owner_first_names($organization) {

	$first_names = "";

	// get manager names
	$manager_names = array();
	if (isset($organization->field_owner[LANGUAGE_NONE])) {
	    foreach($organization->field_owner[LANGUAGE_NONE] as $manager){
	        $manager = user_load($manager['target_id']);
	        $manager_names[] = strip_tags($manager->field_user_first_name[LANGUAGE_NONE][0]['value']); 
	    }
	    $manager_names = array_unique($manager_names);
	} else {
		// use owner of node
		$manager = user_load($organization->uid);
		$manager_names[] = strip_tags($manager->field_user_first_name[LANGUAGE_NONE][0]['value']); 
	}

	// format
	if (sizeof($manager_names) > 0) {
	  $last = array_pop($manager_names);
	  $first_names = (count($manager_names) ? implode(", ", $manager_names) . " and " . $last : $last);
	}

	return $first_names;
}

/**
 * Sends out a single email
 */
function tm_subscriptions_organization_send_email($organization_emails, $subject, $message, $company_image_src, $company_image_link, $companyid, $replyto="", $cc="", $bcc="") {

	global $conf;

	$module = 'tm_subscriptions';
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
	$template_file = drupal_get_path('module', 'tm_notifications') . '/templates/tm_notifications-email_template.tpl.php';
	$template = file_get_contents($template_file);

	// delimit chapter leader emails
	$email = implode(",", $organization_emails);

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

	// Company image
	// Render with tm_notifications_render_hero_image_html()
	if ($company_image_src != null) {
	$company_image_html = tm_notifications_render_hero_image_html($company_image_src, $company_image_link, "Organization image");
		$template = str_replace("__HERO_IMAGE__", $company_image_html, $template);
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

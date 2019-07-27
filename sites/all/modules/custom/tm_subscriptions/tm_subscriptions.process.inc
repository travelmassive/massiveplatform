<?php

// tm_subscriptions.process.inc - payment processing methods

/*
 * Process payment
 */
function tm_subscriptions_process_organization_payment($node, $member) {

	global $conf;
	global $user;

	// STEP 1. VALIDATTION
	if ($node == null) {
		echo "Invalid organization.";
		return;
	}

	// validate node
	// currently only organizations supported
	if ($node->type != "organization") {
		echo "Invalid organization. ";
		echo $conf["tm_subscriptions_error_message"];
		return;
	}

	// check user is logged in user
	if ($user->uid != $member->uid) {
		echo "Invalid user. ";
		echo $conf["tm_subscriptions_error_message"];
		return;
	}

	if (!isset($_POST["stripe_token"])) {
		echo "Invalid token id. ";
		echo $conf["tm_subscriptions_error_message"];
		return;
	}

	if (!isset($_POST["stripe_email"])) {
		echo "Invalid email. ";
		echo $conf["tm_subscriptions_error_message"];
		return;
	}

	if (!isset($_POST["stripe_currency"])) {
		echo "Invalid currency. ";
		echo $conf["tm_subscriptions_error_message"];
		return;
	}

	if (!isset($_POST["stripe_amount"])) {
		echo "Invalid amount. ";
		echo $conf["tm_subscriptions_error_message"];
		return;
	}

	// STEP 2. CHECK PAYMENT AMOUNT
	$pricing = tm_subscriptions_get_organization_subscription_price($node); // pricing data
	$subscription = tm_subscriptions_get_organization_subscription($node); // subscription data

	if ($pricing["stripe_currency"] != $_POST["stripe_currency"]) {
		print_r($_POST["stripe_currency"]);
		echo "Invalid currency check. ";
		echo $conf["tm_subscriptions_error_message"];
		return;
	}

	// Soft fail if amounts are different, and redirect user back to company page
	if ($pricing["stripe_amount"] != $_POST["stripe_amount"]) {

		// Possible causes:
		// 1. changing price while a customer is in the middle of purchase.

		// Send a message anyway so we can track how often this happens
		$error_message = "Check payment amount failed: stripe_amount=" . $pricing["stripe_amount"] . ", post_amount=" . $_POST["stripe_amount"];
		$error_message .= "<br>Note: The user was redirected back to the organization page.";
		tm_subscriptions_notify_payment_error($error_message);

		// Redirect user back to company page 
		$entity_path_alias = drupal_get_path_alias("node/" . $node->nid);
		drupal_set_message("Oops, the subscription is no longer available.", 'warning');
        drupal_goto($entity_path_alias);

		return;
	}

	// STEP 3. PROCESS PAYMENT
	$url = $conf["tm_subscriptions_process_url"] . "?entity=" . $node->nid . "&user=" . $user->uid;

	// stripe details
	$stripe_token = $_POST["stripe_token"];
	$stripe_email = $_POST["stripe_email"];
	$stripe_currency = $_POST["stripe_currency"];
	$stripe_amount = $_POST["stripe_amount"];

	// company and user details
	$display_amount = $pricing["display_amount"];
	$entity_id = $node->nid;
	$entity_type = $node->type;
	$entity_title = $node->title;
	$subscription_label = $pricing["subscription_label"];

	$subscription_type = $pricing["subscription_type"];
	$user_id = $user->uid;
	$entity_path_alias = drupal_get_path_alias("node/" . $entity_id);
	$user_email = $user->mail;
	$user_loaded = user_load($member->uid);
	$user_first_name = $user_loaded->field_user_first_name[LANGUAGE_NONE][0]['value'];
  	$user_last_name = $user_loaded->field_user_last_name[LANGUAGE_NONE][0]['value'];

  	// Check if is renewed
  	$is_renewed = tm_subscriptions_check_organization_renewal_period($node);

  	// get referer code from home chapter of organization owner
  	$referer_code = tm_subscriptions_get_chapter_code($node);
  	
  	// set partner code
  	$partner_code = $pricing["partner_code"];

  	// country code
  	$country_code = tm_subscriptions_get_country_code($node);

  	// log info
  	$log_user_ip = tm_geoip_get_real_visitor_ip(); // $_SERVER["REMOTE_ADDR"];
	$log_user_http_user_agent = "none";
	if (isset($_SERVER['HTTP_USER_AGENT'])) {
		$log_user_http_user_agent = $_SERVER['HTTP_USER_AGENT'];
	}

	// url-ify the data for the POST
	$fields = array("stripe_token" => $stripe_token, "stripe_email" => $stripe_email, "stripe_currency" => $stripe_currency, "stripe_amount" => $stripe_amount, "entity_id" => $entity_id, "entity_type" => $entity_type, "user_id" => $user_id, "user_email" => $stripe_email, "log_user_ip" => $log_user_ip, "log_user_http_user_agent" => $log_user_http_user_agent, "user_first_name" => $user_first_name, "user_last_name" => $user_last_name, "entity_title" => $entity_title, "display_amount" => $display_amount, "subscription_type" => $subscription_type, "subscription_label" => $subscription_label, "referer_code" => $referer_code, "partner_code" => $partner_code, "country_code" => $country_code);

	$fields_string = http_build_query($fields);

	//open connection
	$ch = curl_init();

	//set the url, number of POST vars, POST data
	$CURLOPT_SSL_VERIFYHOST = 2;
	$CURLOPT_SSL_VERIFYPEER = true;
	if ($conf["tm_subscriptions_process_check_certificate"] == false) {
		$CURLOPT_SSL_VERIFYHOST = 0;
		$CURLOPT_SSL_VERIFYPEER = false;
	}
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_POST, count($fields));
	curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_VERBOSE, false);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, $CURLOPT_SSL_VERIFYPEER);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
	curl_setopt($ch, CURLOPT_TIMEOUT, $conf["tm_subscriptions_process_timeout"]); // timeout in seconds

	//execute post
	$result = curl_exec($ch);

	if ($result === false) {
		drupal_set_message("Payment gateway error. " . $conf["tm_subscriptions_error_message"], 'error');
		tm_subscriptions_notify_payment_error("CURL error: " . curl_error($ch));
        drupal_goto($entity_path_alias);
		return;
	}

	//close connection
	curl_close($ch);

	// STEP 4. GET PAYMENT RESULTS
	$res = json_decode($result);

	if ($res == null) {
		drupal_set_message($conf["tm_subscriptions_error_message"], 'warning');
		tm_subscriptions_notify_payment_error("CURL json return null");
        drupal_goto($entity_path_alias);
		return;
	}

	// check for errors
	if ($res->success == false) {
		drupal_set_message($res->error . " " . $conf["tm_subscriptions_error_message"], 'warning');
		tm_subscriptions_notify_payment_error("CURL json return false: " . print_r($res, true));
		drupal_goto($entity_path_alias);
		return;
	}

	// STEP 5. IF SUCCESSFUL PAYMENT, SET SUBSCRIPTION FLAGS
	if ($res->success == true) {

		// Set pricing into static variable so we can pick it up in the notifications hook
  		$hook_static_var_pricing = &drupal_static("tm_subscriptions_pricing");
  		$hook_static_var_pricing = $pricing;

		// Get subscription flags
	  	$organization_subscription_enable = flag_get_flag('organization_subscription_enable', NULL);
	  	$organization_subscription_paid = flag_get_flag('organization_subscription_paid', NULL);

	  	// Flag
		$organization_subscription_enable->flag('flag', $node->nid, $user, TRUE);
		$organization_subscription_paid->flag('flag', $node->nid, $user, TRUE);

		// Set subscription type
		$node->field_company_subscription_type[LANGUAGE_NONE][0]['value'] = $subscription_type;

		// Set subscription expiry date
		// Default expire date set from now
		$start_date = "";
		if (isset($node->field_company_subscription_date[LANGUAGE_NONE][0]['value'])) {

			$existing_expiry_date = $node->field_company_subscription_date[LANGUAGE_NONE][0]['value'];
			if (strtotime($existing_expiry_date) > strtotime("now")) {
				// Expiry date in future, add to expiry date
				$start_date = date("Y-m-d", strtotime($existing_expiry_date));
			} else {
				// Expiry date in past, set from now
				$start_date = ""; // now
			}
		}

		// Set expiry date
		$subscription_expires_date = date('Y-m-d 00:00:00', strtotime($start_date . " " . $subscription['subscription_expires']));
		$node->field_company_subscription_date[LANGUAGE_NONE][0]['value'] = $subscription_expires_date;
		
		// Set email
		$node->field_company_account_email[LANGUAGE_NONE][0]['email'] = $stripe_email;

		// Save changes to organization
		node_save($node);

		// Send notification
		if ($is_renewed) {
			$success_message = $conf["tm_subscriptions_organization_renewed_success_message"];
			tm_subscriptions_organization_send_notification($node, "renewed", $user_first_name);
		} else {
			$success_message = $conf["tm_subscriptions_organization_payment_success_message"];
			tm_subscriptions_organization_send_notification($node, "purchase", $user_first_name);
		}

		// Set success message and redirect to page
		$success_message .= " <a href='/payment/reports/subscription-payments/" . $node->nid . "'>View payment receipt</a>";
		drupal_set_message($success_message);
		drupal_goto($entity_path_alias);
		return;
	}

	// Shouldn't get here
	drupal_set_message($conf["tm_subscriptions_error_message"], 'error');
	drupal_goto($entity_path_alias);

}

/**
 * Notify of payment error
 */
function tm_subscriptions_notify_payment_error($error_message) {

	global $conf;
	global $user;

	$log_user_ip = tm_geoip_get_real_visitor_ip(); // $_SERVER["REMOTE_ADDR"];
	$log_user_http_user_agent = "none";
	if (isset($_SERVER['HTTP_USER_AGENT'])) {
		$log_user_http_user_agent = $_SERVER['HTTP_USER_AGENT'];
	}
	$log_request_uri = $_SERVER['REQUEST_URI'];

	$message = 'A subscription payment processor error occured:';
	$message .= '<br>Error message: ' . $error_message;
	$message .= '<br>User ID: ' . $user->uid;
	$message .= "<br>Request URI: " . $log_request_uri;
	$message .= '<br>IP Address: ' . $log_user_ip;
	$message .= '<br>User Agent: ' . $log_user_http_user_agent;
	$message .= "<br>Log date: " . date(DATE_RFC2822);

	$subject = "Payment process error";
	$recipient = $conf["tm_subscriptions_process_error_email"];
	$from = $conf["tm_subscriptions_process_error_email"];

	$headers = 'From: ' . $from . "\r\n";
    $headers .= 'Reply-To: ' . $from . "\r\n";
    $headers .= 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
	$result = mail($recipient, $subject, $message, $headers);
	return $result;

}

<?php

// tm_payments.process.inc - payment processing methods

/*
 * Process payment
 */
function tm_payments_process_event_payment($event, $member) {

	global $conf;
	global $user;

	// STEP 1. VALIDATTION
	if ($event == null) {
		echo "Invalid event.";
		return;
	}

	// validate event
	if ($event->type != "event") {
		echo "Invalid event type. ";
		echo $conf["tm_payments_error_message"];
		return;
	}

	// check user is logged in user
	if ($user->uid != $member->uid) {
		echo "Invalid user. ";
		echo $conf["tm_payments_error_message"];
		return;
	}

	if (!isset($_POST["stripe_token"])) {
		echo "Invalid token id. ";
		echo $conf["tm_payments_error_message"];
		return;
	}

	if (!isset($_POST["stripe_email"])) {
		echo "Invalid email. ";
		echo $conf["tm_payments_error_message"];
		return;
	}

	if (!isset($_POST["stripe_currency"])) {
		echo "Invalid currency. ";
		echo $conf["tm_payments_error_message"];
		return;
	}

	if (!isset($_POST["stripe_amount"])) {
		echo "Invalid amount. ";
		echo $conf["tm_payments_error_message"];
		return;
	}

	// check seats still available
	// note: will not prevent concurrenct purchase of last ticket
	$num_seats_left = _tm_events_seats_left($event);
	if ($num_seats_left == 0) {

		// Send a message anyway so we can track how often this happens
		// ie: two people try to purchase the last ticket
		$error_message = "No more seats available.";
		$error_message .= "<br>Note: The user was redirected back to the event page.";
		tm_payments_notify_payment_error($error_message);

		// Redirect user back to event
		$event_path_alias = drupal_get_path_alias("node/" . $event->nid);
		drupal_set_message("Sorry, this event has sold out.", 'warning');
        drupal_goto($event_path_alias);

		return;
	}

	// STEP 2. CHECK PAYMENT AMOUNT
	$pricing = _tm_payments_get_registration_price($event);

	if ($pricing["stripe_currency"] != $_POST["stripe_currency"]) {
		echo "Invalid currency check.";
		echo $conf["tm_payments_error_message"];
		return;
	}

	// Soft fail if amounts are different, and redirect user back to event page
	if ($pricing["stripe_amount"] != $_POST["stripe_amount"]) {

		// Possible causes:
		// 1. concurrent purchase of early bird ticket, 
		// 2. changing price while a customer is in the middle of purchase.

		// Send a message anyway so we can track how often this happens
		$error_message = "Check payment amount failed: stripe_amount=" . $pricing["stripe_amount"] . ", post_amount=" . $_POST["stripe_amount"];
		$error_message .= "<br>Note: The user was redirected back to the event page.";
		tm_payments_notify_payment_error($error_message);

		// Redirect user back to event
		$event_path_alias = drupal_get_path_alias("node/" . $event->nid);
		drupal_set_message("Oops, the ticket price you requested is no longer available.", 'warning');
        drupal_goto($event_path_alias);

		return;
	}

	// STEP 3. PROCESS PAYMENT
	$url = $conf["tm_payments_process_url"] . "?event=" . $event->nid . "&user=" . $user->uid;

	// stripe details
	$stripe_token = $_POST["stripe_token"];
	$stripe_email = $_POST["stripe_email"];
	$stripe_currency = $_POST["stripe_currency"];
	$stripe_amount = $_POST["stripe_amount"];

	// event and user details
	$display_amount = $pricing["display_amount"];
	$event_id = $event->nid;
	$event_title = $event->title;
	$event_label = $pricing["event_label"];

	$event_path_alias = drupal_get_path_alias("node/" . $event->nid);
	$event_url = $profile_url = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . $event_path_alias;
	$user_id = $user->uid;
	$user_email = $user->mail;
	$user_loaded = user_load($member->uid);
	$user_first_name = $user_loaded->field_user_first_name[LANGUAGE_NONE][0]['value'];
  	$user_last_name = $user_loaded->field_user_last_name[LANGUAGE_NONE][0]['value'];

  	// log info
  	$log_user_ip = tm_geoip_get_real_visitor_ip(); // $_SERVER["REMOTE_ADDR"];
	$log_user_http_user_agent = "none";
	if (isset($_SERVER['HTTP_USER_AGENT'])) {
		$log_user_http_user_agent = $_SERVER['HTTP_USER_AGENT'];
	}

	// url-ify the data for the POST
	$fields = array("stripe_token" => $stripe_token, "stripe_email" => $stripe_email, "stripe_currency" => $stripe_currency, "stripe_amount" => $stripe_amount, "event_id" => $event_id, "user_id" => $user_id, "user_email" => $user->mail, "log_user_ip" => $log_user_ip, "log_user_http_user_agent" => $log_user_http_user_agent, "user_first_name" => $user_first_name, "user_last_name" => $user_last_name, "event_title" => $event_title, "display_amount" => $display_amount, "event_url" => $event_url, "event_label" => $event_label);

	$fields_string = http_build_query($fields);

	//open connection
	$ch = curl_init();

	//set the url, number of POST vars, POST data
	$CURLOPT_SSL_VERIFYHOST = 2;
	$CURLOPT_SSL_VERIFYPEER = true;
	if ($conf["tm_payments_process_check_certificate"] == false) {
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
	curl_setopt($ch, CURLOPT_TIMEOUT, $conf["tm_payments_process_timeout"]); // timeout in seconds

	//execute post
	$result = curl_exec($ch);

	if ($result === false) {
		drupal_set_message("Payment gateway error. " . $conf["tm_payments_error_message"], 'error');
		tm_payments_notify_payment_error("CURL error: " . curl_error($ch));
        drupal_goto($event_path_alias);
		return;
	}

	//close connection
	curl_close($ch);

	// STEP 4. GET PAYMENT RESULTS
	$res = json_decode($result);

	if ($res == null) {
		drupal_set_message($conf["tm_payments_error_message"], 'warning');
		tm_payments_notify_payment_error("CURL json return null");
        drupal_goto($event_path_alias);
		return;
	}

	// check for errors
	if ($res->success == false) {
		drupal_set_message($res->error . " " . $conf["tm_payments_error_message"], 'warning');
		tm_payments_notify_payment_error("CURL json return false: " . print_r($res, true));
		drupal_goto($event_path_alias);
		return;
	}

	// STEP 5. IF SUCCESSFUL PAYMENT, REGISTER USER AND SET FLAGS
	if ($res->success == true) {

		// Set pricing into static variable so we can pick it up in the notifications hook
  		$hook_static_var_pricing = &drupal_static("tm_payments_pricing");
  		$hook_static_var_pricing = $pricing;

		// Get event flags
	  	$rsvpflag = flag_get_flag('event_register', NULL);
	  	$paidflag = flag_get_flag('event_paid', NULL);
	  	$waitflag = flag_get_flag('event_waitlist', NULL);

	  	// Add paid flag
	  	// Note: We could send customer a receipt here or in the rsvpflag notification
		$paidflag->flag('flag', $event->nid, $user, TRUE);

		// Add to rsvp first, unflag waitlist second
		// this will trigger a single rsvp notification
		// (note: we don't expect people to waitlisted here)
		$rsvpflag->flag('flag', $event->nid, $user, TRUE);
		$waitflag->flag('unflag', $event->nid, $user, TRUE);

		drupal_set_message("Thanks for your payment. You are now registered for this event.");
		drupal_goto($event_path_alias);
		return;
	}

	// Shouldn't get here
	drupal_set_message($conf["tm_payments_error_message"], 'error');
	drupal_goto($event_path_alias);

}

/**
 * Notify of payment error
 */
function tm_payments_notify_payment_error($error_message) {

	global $conf;
	global $user;

	$log_user_ip = $_SERVER["REMOTE_ADDR"];
	$log_user_http_user_agent = "none";
	if (isset($_SERVER['HTTP_USER_AGENT'])) {
		$log_user_http_user_agent = $_SERVER['HTTP_USER_AGENT'];
	}
	$log_request_uri = $_SERVER['REQUEST_URI'];

	$message = 'A payment processor error occured:';
	$message .= '<br>Error message: ' . $error_message;
	$message .= '<br>User ID: ' . $user->uid;
	$message .= "<br>Request URI: " . $log_request_uri;
	$message .= '<br>IP Address: ' . $log_user_ip;
	$message .= '<br>User Agent: ' . $log_user_http_user_agent;
	$message .= "<br>Log date: " . date(DATE_RFC2822);

	$subject = "Payment process error";
	$recipient = $conf["tm_payments_process_error_email"];
	$from = $conf["tm_payments_process_error_email"];

	$headers = 'From: ' . $from . "\r\n";
    $headers .= 'Reply-To: ' . $from . "\r\n";
    $headers .= 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
	$result = mail($recipient, $subject, $message, $headers);
	return $result;

}

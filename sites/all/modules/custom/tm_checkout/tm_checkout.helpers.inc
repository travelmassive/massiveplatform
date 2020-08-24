<?php 

// tm_checkout.helpers.inc

/**
 * Notify of payment
 */
function tm_checkout_notify_payment($timestamp, $payment_id, $checkout_action, $checkout_item, $payment_email, $stripe_currency,$stripe_amount, $display_amount, $entity_id, $entity_type, $entity_title, $user_id, $user_email, $user_first_name, $user_last_name, $log_user_ip, $log_user_http_user_agent, $refunded, $paid_out, $referer_code, $partner_code, $partner_commission) {

	global $conf;

	// check $conf['tm_checkout_stripe_payment_email']
	if (!isset($conf['tm_checkout_stripe_payment_email'])) {
		return;
	}

	$message = 'A Stripe payment was processed:<br>';
	$message .= '<br>timestamp: ' . date(DATE_RFC2822, $timestamp);
	$message .= '<br>payment_id: ' . trim($payment_id);
	$message .= '<br>checkout_action: ' . trim($checkout_action);
	$message .= '<br>checkout_item: ' . trim($checkout_item); 
	$message .= '<br>payment_email: ' . trim($payment_email); 
	$message .= '<br>stripe_currency: ' . $stripe_currency;
	$message .= '<br>stripe_amount: ' . $stripe_amount; 
	$message .= '<br>display_amount: ' . $display_amount; 
	$message .= '<br>entity_id: ' . $entity_id; 
	$message .= '<br>entity_type: ' . trim($entity_type); 
	$message .= '<br>entity_title: ' . trim($entity_title); 
	$message .= '<br>user_id: ' . $user_id;
	$message .= '<br>user_email: ' . trim($user_email); 
	$message .= '<br>user_first_name: ' . trim($user_first_name); 
	$message .= '<br>user_last_name: ' . trim($user_last_name); 
	$message .= '<br>log_user_ip: ' . $log_user_ip; 
	$message .= '<br>log_user_http_user_agent: ' . trim($log_user_http_user_agent); 
	$message .= '<br>refunded: ' . $refunded; 
	$message .= '<br>paid_out: ' . $paid_out; 
	$message .= '<br>referer_code: ' . trim($referer_code);
	$message .= '<br>partner_code: ' . trim($partner_code);
	$message .= '<br>partner_commission: ' . $partner_commission;

	$subject = "Payment processed";
	$recipient = $conf["tm_checkout_stripe_payment_email"];
	$from = $conf["tm_checkout_stripe_payment_email"];

	$headers = 'From: ' . $from . "\r\n";
    $headers .= 'Reply-To: ' . $from . "\r\n";
    $headers .= 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
	$result = mail($recipient, $subject, $message, $headers);
	return $result;

}

/**
 * Helper method to get partner commission
 * ie: return 10 for 10%
 */
function tm_checkout_get_partner_commission($partner_code, $checkout_action = null) {

	global $conf;

	// if no partner code, no commission
	if ($partner_code == null)  {
		return 0;
	}

	// if no partner code, no commission
	if (trim($partner_code) == "") {
		return 0;
	}

	// uppercase
	$partner_code = strtoupper($partner_code);

	// default commission
	$partner_commission = $conf["tm_checkout_partner_commission_default"]; 

	// check for specific partner code
	if (isset($conf["tm_checkout_partner_commission"][$partner_code])) {
		$partner_commission = $conf["tm_checkout_partner_commission"][$partner_code];
	}

	// return commission	
	return $partner_commission;
}

/**
 * Notify of payment error
 */
function tm_checkout_notify_payment_error($error_message) {

	global $conf;
	global $user;

	$log_user_ip = tm_geoip_get_real_visitor_ip(); // $_SERVER["REMOTE_ADDR"];
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

/*
 * Helper method to return a default currency, based on location
 */
function tm_checkout_get_user_currency($currency) {

	global $conf;

	// set default currency from geoip
	if (!module_exists("tm_geoip")) {
		return $curency;
	}

	// visitor and geoip
	$visitor_ip = tm_geoip_get_real_visitor_ip();
	$geoip_info = tm_geoip_lookup($visitor_ip);
	
	// special case for iso code
	$iso_codes = array();
	$iso_codes["GB"] = "gbp";
	$iso_codes["CA"] = "cad";
	$iso_codes["TH"] = "thb";
	$iso_codes["JP"] = "jpy";
	$iso_codes["SG"] = "sgd";
	foreach ($iso_codes as $iso_code => $iso_currency) {
		if (isset($geoip_info["country"]["iso_code"])) {
			if ($geoip_info["country"]["iso_code"] == $iso_code) {
				if (isset($conf["tm_payments_currencies"][$iso_currency])) {
					return $iso_currency;
				}
			}
		}
	}

	// check for some common regions
	if (isset($geoip_info["continent"]["code"])) {
    	if ($geoip_info["continent"]["code"] == "NA") {
			if (isset($conf["tm_payments_currencies"]["usd"])) {
				return "usd";
			}
		}
		if ($geoip_info["continent"]["code"] == "EU") {
			if (isset($conf["tm_payments_currencies"]["eur"])) {
				return "eur";
			}
		}
		if ($geoip_info["continent"]["code"] == "OC") {
			if (isset($conf["tm_payments_currencies"]["aud"])) {
				return "aud";
			}
		}
		if ($geoip_info["continent"]["code"] == "AF") {
			if (isset($conf["tm_payments_currencies"]["zar"])) {
				return "zar";
			}
		}
    }

    // no match
	return $currency;
}

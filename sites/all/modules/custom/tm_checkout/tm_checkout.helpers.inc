<?php 

// tm_checkout.helpers.inc

/**
 * Notify of payment
 */
function tm_checkout_notify_payment($timestamp, $payment_id, $checkout_action, $checkout_item, $payment_email, $stripe_currency,$stripe_amount, $display_amount, $entity_id, $entity_type, $entity_title, $user_id, $user_email, $user_first_name, $user_last_name, $log_user_ip, $log_user_http_user_agent, $refunded, $paid_out, $referer_code, $partner_code) {

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

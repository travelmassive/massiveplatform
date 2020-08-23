<?php 

// tm_checkout.log.inc

/**
 * Log a payment in tm_checkout_payments
 */
function tm_checkout_log_payment($timestamp, $payment_id, $checkout_action, $checkout_item, $payment_email, $stripe_currency, $stripe_amount, $display_amount, $entity_id, $entity_type, $entity_title, $user_id, $user_email, $user_first_name, $user_last_name, $log_user_ip, $log_user_http_user_agent, $refunded, $paid_out, $referer_code, $partner_code, $partner_commission) {

	if ($timestamp == null) {
		$timestamp = time();
	}

	$fields = array(
		'timestamp' => $timestamp,
		'payment_id' => trim($payment_id), 
		'checkout_action' => $checkout_action, 
		'checkout_item' => $checkout_item, 
		'payment_email' => trim($payment_email), 
		'stripe_currency' => $stripe_currency,
		'stripe_amount' => $stripe_amount, 
		'display_amount' => $display_amount, 
		'entity_id' => $entity_id, 
		'entity_type' => trim($entity_type), 
		'entity_title' => trim($entity_title), 
		'user_id' => $user_id, 
		'user_email' => trim($user_email), 
		'user_first_name' => trim($user_first_name), 
		'user_last_name' => trim($user_last_name), 
		'log_user_ip' => $log_user_ip, 
		'log_user_http_user_agent' => trim($log_user_http_user_agent), 
		'refunded' => $refunded, 
		'paid_out' => $paid_out, 
		'referer_code' => trim($referer_code), 
		'partner_code' => trim($partner_code),
		'partner_commission' => $partner_commission
	);

	// write payment
	$result = drupal_write_record('tm_checkout_payments', $fields);

	// notify account that we got a payment, woohoo!
	tm_checkout_notify_payment($timestamp, $payment_id, $checkout_action, $checkout_item, $payment_email, $stripe_currency,$stripe_amount, $display_amount, $entity_id, $entity_type, $entity_title, $user_id, $user_email, $user_first_name, $user_last_name, $log_user_ip, $log_user_http_user_agent, $refunded, $paid_out, $referer_code, $partner_code, $partner_commission);

	return $result;

}

/**
 * Get a payment record with a payment_id
 */
function tm_checkout_get_payment($payment_id) {

	$query = "SELECT * FROM tm_checkout_payments WHERE payment_id = :payment_id";

	// fetch result
	$result = db_query($query, array(':payment_id' => $payment_id))->fetch();

	if ($result == null) {
		return null;
	}

	return $result;
}

/**
 * Get user payments
 */
function tm_checkout_get_user_payments($uid) {

	$query = "SELECT * FROM tm_checkout_payments WHERE user_id = :uid ORDER BY timestamp DESC";

	// fetch result
	$results = db_query($query, array(':uid' => $uid))->fetchAll();

	$results_array = array();
	foreach ($results as $result) {
		$results_array[] = $result;
	}

	return $results_array;

}

/**
 * Check if a user has made a payment
 */
function tm_checkout_user_has_made_payment($uid) {

	$query = "SELECT COUNT(*) AS total FROM tm_checkout_payments WHERE user_id = :uid";

	// fetch result
	$result = db_query($query, array(':uid' => $uid))->fetch();
	return ($result->total > 0);

}

/**
 * Check if event has made a payment
 */
function tm_checkout_event_has_payments($event_id) {

	$query = "SELECT COUNT(*) AS total FROM tm_checkout_payments WHERE entity_type = 'event' AND entity_id = :event_id";

	// fetch result
	$result = db_query($query, array(':event_id' => $event_id))->fetch();
	return ($result->total > 0);

}

/**
 * Get event payments
 */
function tm_checkout_get_event_payments($event_id) {

	$query = "SELECT * FROM tm_checkout_payments WHERE entity_type = 'event' AND entity_id = :event_id ORDER BY timestamp DESC";

	// fetch result
	$results = db_query($query, array(':event_id' => $event_id))->fetchAll();

	$results_array = array();
	foreach ($results as $result) {
		$results_array[] = $result;
	}

	return $results_array;

}

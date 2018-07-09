<?php

// tm_payments.display.inc - display methods

/* 
 * Render payment field
 */
function _tm_payments_price($field, $title = NULL) {

	global $user;
	global $conf;

	// check if event is valid
	$node = $field['entity'];
	if ($node->type != "event") {
		return;
	}

	// check if payment is enabled
	if (!_tm_payments_is_payment_enabled($node)) {
		return;
	}

	// If logged in, check if registered or waitlisted
	$registered_for_event = false; // default if not logged in
	$waitlist_for_event = false; // default if not logged in
	$paid_for_event = false;
	if (user_is_logged_in()) {
		$registered_flag = flag_get_flag('event_register');
		$registered_for_event = ($registered_flag && $registered_flag->is_flagged($node->nid, $user->uid));
		$waitlist_flag = flag_get_flag('event_waitlist');
		$waitlist_for_event = ($waitlist_flag && $waitlist_flag->is_flagged($node->nid, $user->uid));
		$paid_flag = flag_get_flag('event_paid');
		$paid_for_event = ($paid_flag && $paid_flag->is_flagged($node->nid, $user->uid));
	}

	$pricing = _tm_payments_get_registration_price($node);

	// Check valid price
	if ($pricing["display_amount"] == "") { 
		return;
	}

	$event_type = $node->field_event_type[LANGUAGE_NONE][0]['value'];
	$event_type_label = "event";
	if (isset($conf['tm_event_types_labels'][$event_type])) {
		$event_type_label = $conf['tm_event_types_labels'][$event_type];
	}

	// Option 1. Paid and registered
	if ($paid_for_event && $registered_for_event) {
		$html = '<h2><span>Price</span></h2>';
		$html .= '<div class="detail-price">You have purchased a ticket to this ' . t($event_type_label) . '</div>';
		return $html;
	}

	// Option 2. Paid and not-registered (cancelled ticket)
	if ($paid_for_event && !$registered_for_event) {
		global $conf;
		$html = '<h2><span>Price</span></h2>';
		$html .= "<div class='detail-price'>Your ticket has been cancelled<br><a target='_blank' href='" . $conf["tm_payments_refund_url"] . "'>How to request a refund</a></div>";
		return $html;
	}

	// If event is full, don't show payment options
	if (_tm_events_seats_left($node) == 0) {
		return;
	}

	// Option 3. Not registered and not paid
	if (!$paid_for_event && !$registered_for_event) {
		$html = '<h2><span>Price</span></h2>';

		if ($pricing["early_bird_enabled"]) {

			$html .= '<div class="detail-price">';
			$html .= '<span style="font-weight: bold;">' . $pricing["early_bird_label"] . ' </span>';
			$html .= 'Attend this ' . t($event_type_label) . ' for ' . $pricing["currency_prefix"] . str_replace(".00", "", $pricing["display_amount"]);
			if ($pricing["saving_message"] != "") {
				$html .= ' (<span style="font-weight: bold;">' . $pricing["saving_message"] . '</span>)';
			}
			$html .= '<br><span style="font-style: italic;">' . $pricing["early_bird_message"] . '</span>';
			$html .= '</div>';
		} else {
			$html .= '<div class="detail-price">Attend this ' . t($event_type_label) . ' for ' . $pricing["currency_prefix"] . str_replace(".00", "", $pricing["display_amount"]) . '</div>';
		}
		
		return $html;
	}

}

/* 
 * Render refund policy 
 */
function _tm_payments_refund_policy($field, $title = NULL) {

	global $conf;

	// check if event is valid
	$node = $field['entity'];
	if ($node->type != "event") {
		return;
	}

	// check if payment is enabled
	if (!_tm_payments_is_payment_enabled($node)) {
		return;
	}

	// get refund policy
	$policy = "";
	if (isset($node->field_event_payment_refund_text[LANGUAGE_NONE][0]['safe_value'])) {
		$policy = $node->field_event_payment_refund_text[LANGUAGE_NONE][0]['safe_value'];
	}

	// default policy
	if (trim($policy) == "") {
		$policy = $conf["tm_payments_refund_policy"];
	}
	
	return $policy;
}

/**
 * Implement hook_node_view
 */
function tm_payments_node_view($node, $view_mode, $langcode) {
	
	// display partner code applied message
	if ($node->type == 'event' && $view_mode == 'full') {
		$partner_code = _tm_payments_is_payment_enabled($node, true);
	}
}


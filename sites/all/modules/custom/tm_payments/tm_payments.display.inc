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

	// if user is signed in, check they can make a payment 
	if (user_is_logged_in()) {
		$event_display_flag = _tm_event_display_flag($node->nid, $user);
		if ($event_display_flag == "show_closed") {
			return;
		}
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

	// get pricing
	$pricing = _tm_payments_get_registration_price($node);

	// payment type
	$payment_type = _tm_payments_get_payment_type($node);

	// event type
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
	if (!$paid_for_event) {

		// label
		// future: Attend this event
		// past tense: Get a ticket
		$attend_this_label = "Attend this " . t($event_type_label);
		if (_tm_event_is_past($node)) {
			$attend_this_label = "Purchase a ticket";
			if (user_is_logged_in()) {
				$attend_this_label = "Purchase a <a href='/checkout/event/" . $node->nid . "'>ticket</a>";
			}
		}

		// fixed price
		if ($payment_type == "fixed_price") {
			$html = '<h2><span>Price</span></h2>';

			if ($pricing["early_bird_enabled"]) {

				$html .= '<div class="detail-price">';
				$html .= '<span style="font-weight: bold;">' . $pricing["early_bird_label"] . ' </span>';
				$html .= $attend_this_label . ' for ' . $pricing["currency_prefix"] . str_replace(".00", "", $pricing["display_amount"]);
				if ($pricing["saving_message"] != "") {
					$html .= ' (<span style="font-weight: bold;">' . $pricing["saving_message"] . '</span>)';
				}
				$html .= '<br><span style="font-style: italic;">' . $pricing["early_bird_message"] . '</span>';
				$html .= '</div>';
			} else {
				$html .= '<div class="detail-price">' . $attend_this_label . ' for ' . $pricing["currency_prefix"] . str_replace(".00", "", $pricing["display_amount"]) . '</div>';
			}

			// free
			if ($pricing["display_amount"] == "") { 
				return;
			}
		}

		// pay what you wish
		if ($payment_type == "pay_what_you_wish") {

			// get event currency
			$currency = $node->field_event_payment_currency[LANGUAGE_NONE][0]['value'];

			// see if we can match user's local currency
			if (user_is_logged_in()) {
				$currency = tm_checkout_get_user_currency($currency);
			}

			// currency prefix
			$currency_prefix = $conf["tm_payments_currencies"][$currency];

			// get lowest price
			$lowest_price = _tm_payments_get_lowest_price($node, $user, $currency);
			
			// label
			$attend_this_label = "Support this event with a ";
			if ($lowest_price > 0) {
				if ($registered_for_event) {
					$attend_this_label .= "donation of "; 
				} else {
					$attend_this_label .= "ticket for ";
				}
				$attend_this_label .= $currency_prefix . str_replace(".00", "", $lowest_price);
			} else {
				$attend_this_label .= "donation"; 
			}
			
			// add links for loged in
			if (user_is_logged_in()) {
				$attend_this_label = str_replace("ticket", "<a href='/checkout/event/" . $node->nid .  "'>ticket</a>", $attend_this_label);
				$attend_this_label = str_replace("donation", "<a href='/checkout/event/" . $node->nid .  "'>donation</a>", $attend_this_label);
			}

			// render html
			$html = '<h2><span>Price</span></h2>';
			$html .= '<div class="detail-price">' . $attend_this_label . '</div>';
			
		}

		// past event with no post payments
		if (_tm_event_is_past($node)) {
			$payment_post_event = false;
			if (isset($node->field_event_payment_post_event[LANGUAGE_NONE][0]['value'])) {
				$payment_post_event = $node->field_event_payment_post_event[LANGUAGE_NONE][0]['value'];
			}
			if (!$payment_post_event) {
				return "";
			}
		}
		
		// don't index prices
		$html = '<!--googleoff: all-->' . $html . '<!--googleon: all-->';

		// return html
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

/*
 * Render total events
 */
function _tm_payments_show_total($field, $title = NULL) {

	global $conf;

	// check if event is valid
	$event = $field['entity'];
	if ($event->type != "event") {
		return;
	}

	// must have field_event_payments_show_total
	if (!isset($event->field_event_payments_show_total[LANGUAGE_NONE][0]['value'])) {
		return;
	}

	// must be enabled
	if ($event->field_event_payments_show_total[LANGUAGE_NONE][0]['value'] == false) {
		return;
	}

	// display in user's currency
	$display_currency = $event->field_event_payment_currency[LANGUAGE_NONE][0]['value'];
	$display_currency = tm_checkout_get_user_currency($display_currency);

	// check currency
	if (!isset($conf["tm_payments_currencies_fx"][$display_currency])) {
		return;
	}

	// check cache
	cache_clear_all('tm-payments-show-total-' . $event->nid . '-', 'cache', TRUE);
	$cache = cache_get('tm-payments-show-total-' . $event->nid . '-' . $display_currency, 'cache');
	if (!empty($cache)) {
		return $cache->data;
	}

	// get event payments
	$event_payments = tm_checkout_get_event_payments($event->nid);
	
	// no payments
	$num_payments = sizeof($event_payments);
	if ($num_payments == 0) {
		return "";
	}

	// total payments
	$total_payments = 0;
	$total_amount = 0;
	foreach($event_payments as $payment) {
			
		// check refunded
		if ($payment->refunded) {
			continue;
		}

		// get amount in payment currency
		$price_amount = $payment->display_amount;

		// fx
		$stripe_currency = $payment->stripe_currency;
		if ($stripe_currency != $display_currency) {

			// calculate conversion
			$to_usd = floatval(1.0 / $conf["tm_payments_currencies_fx"][$stripe_currency]);
			$from_usd = floatval($conf["tm_payments_currencies_fx"][$display_currency]);

			// convert fx and round
			$price_amount = ceil($price_amount * $to_usd * $from_usd);
		}

		// sum
		$total_amount = $total_amount + $price_amount;
	
		// count payments
		$total_payments++;
	}

	// must have payments
	if ($total_payments == 0) {
		return;
	}

	// must have amount
	if ($total_amount == 0) {
		return;
	}

	// render message
	$display_total_amount = $conf["tm_payments_currencies"][$display_currency] . ($total_amount + 0);

	$message_template = $conf["tm_payments_show_payments_message"];
	if ($total_payments == 1) {
		// fix plurals
		$message_template = str_replace("members", "member", $message_template);
		$message_template = str_replace("people", "person", $message_template);
	}
	$message_template = str_replace("__TOTAL_AMOUNT__", $display_total_amount, $message_template);
	$message_template = str_replace("__TOTAL_PAYMENTS__", $total_payments, $message_template);

	// don't index payments
	$html = '<!--googleoff: all-->' . $message_template . '<!--googleon: all-->';

	// cache result
	cache_set('tm-payments-show-total-' . $event->nid . '-' . $display_currency, $html, 'cache', time() + 300); // 5 minutes

	// return  html
	return $html;

}

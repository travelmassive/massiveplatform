<?php

// tm_subscriptions_user.pricing.inc - pricing methods

/* 
 * Return registration price and currency of user subscription
 * Will return pricing even if payment is not enabled
 */
function tm_subscriptions_get_user_subscription_price($account) {

	global $conf;

	$subscription = tm_subscriptions_get_user_subscription($account);
	if ($subscription == null) {
		return array();
	}

	// STEP 1. GET REGISTRATION CURRENCY AND PRICE
	$display_amount = $subscription["price"];
	$is_renewal_period = tm_subscriptions_check_user_renewal_period($account);
	if ($is_renewal_period) {
		$display_amount = $subscription["price_renew"]; // if in renewal period, show renewal price
	} 
	$regular_amount = $display_amount; // keep track of regular price in case we have early bird price
	$currency = $subscription["currency"];
	$currency_symbol = $subscription["currency_symbol"];
	$currency_prefix = $subscription["currency_symbol"];
	$subscription_label = $subscription["subscription_label"];
	$subscription_type = $subscription["subscription_type"];
	$stripe_description = $subscription["stripe_description"];

	// STEP 2. GET DISCOUNT PRICING
	$partner_enabled = false;
	$partner_message = "Partner Discount";
	$partner_label = "Partner Discount";
	$partner_code = tm_subscriptions_user_get_partner_code();
	if ($partner_code != "") {
		// todo, validate partner code
		$partner_code = strtoupper($partner_code);
		$partner_enabled = true;
		if (isset($subscription["partner_codes"][$partner_code])) {
			$partner_enabled = true;
			$display_amount = $subscription["partner_codes"][$partner_code];
			$purchase_action_text = "Join";
			if ($is_renewal_period) { 
				$purchase_action_text = "Renew";
			}
			$subscription_label = $purchase_action_text . " today for " . $subscription["currency_prefix"] . $display_amount;
		}
	}

	// STEP 4. CALCULATE SAVING MESSAGE
	$saving_message = "";
	if ($regular_amount > $display_amount) {
		$discount_amt = $regular_amount - $display_amount;
		$discount_pct = ($discount_amt / $regular_amount) * 100;
		$saving_message = "Save " . floor($discount_pct) . "%";
	}

	// STEP 5. CALCULATE STRIPE AMOUNT
	$stripe_amount = _tm_subscriptions_user_get_api_amount($display_amount, $currency);
	
	// return pricing array
	return array("display_amount" => $display_amount, "stripe_currency" => $currency, "stripe_amount" => $stripe_amount, "currency_symbol" => $currency_symbol, "currency_prefix" => $currency_prefix, "regular_amount" => $regular_amount, "partner_enabled" => $partner_enabled, "partner_message" => $partner_message,  "partner_label" => $partner_label, "saving_message" => $saving_message, "subscription_label" => $subscription_label, "partner_code" => $partner_code, "subscription_type" => $subscription_type, "stripe_description" => $stripe_description);
}

/**
 * Return amount to use with stripe api
 */
function _tm_subscriptions_user_get_api_amount($amount, $currency) {

	// zero-decimal currencies
    // https://support.stripe.com/questions/which-zero-decimal-currencies-does-stripe-support
    $zero_decimal_currencies = array("bif", "clp", "djf", "gnf", "jpy", "kmf", "krw", "mga", "pyg", "rwf", "vnd", "vuv", "xaf", "xof", "xpf");

    $stripe_amount = (int)$amount; // integer
    if (!in_array(strtolower($currency), $zero_decimal_currencies)) {
    	$stripe_amount = $stripe_amount * 100; // add cents
    }

    return $stripe_amount;
}

/**
 * Helper method to get pricing label
 * ie: Upgrade to Pro Membership for $99.
 */
function tm_subscriptions_get_user_pricing_label($account) {

	$pricing_label = tm_subscriptions_get_user_subscription_field($account, "price_label");
	if (tm_subscriptions_check_user_renewal_period($account)) {
		$pricing_label = tm_subscriptions_get_user_subscription_field($account, "price_label_renew");
	}
	
	$country_pricing = tm_subscriptions_get_user_subscription_price($account);
	
	if ($country_pricing["saving_message"] != "") {
		$pricing_label = $country_pricing["subscription_label"] . " — " . $country_pricing["saving_message"]; // use saving message if partner code applied
	}

	return $pricing_label;
}

/**
 * Helper method to get price display 
 * ie: $99 
 */
function tm_subscriptions_get_user_pricing_amount($account) {

	$country_pricing = tm_subscriptions_get_user_subscription_price($account);
	$pricing_amount = $country_pricing["currency_prefix"] . $country_pricing["display_amount"];
	return $pricing_amount;
}


<?php

// tm_subscriptions.display.inc - display methods

/**
 * Impement Hook node view
 * Expire premium account if it has expired and manager of account views it
 */
function tm_subscriptions_node_view($node, $view_mode, $langcode) {

	global $user;

	if (!user_is_logged_in()) {
		return;
	}

	if ($view_mode != "full") {
		return;
	}

	if ($node->type != "organization") {
		return;
	}

	// check if user is a manager of the organization
	if (!tm_organizations_is_company_profile_manager($user->uid, $node->nid)) {
		return false;
	}

	// remove subscription if it has expired
	// this will happen if a manager of the organization views their own profile
	if (tm_subscriptions_has_subscription_expired($node)) {

		global $conf;

		$flag = flag_get_flag('organization_subscription_enable');

		if ($flag->is_flagged($node->nid)) {
			$flag->flag('unflag', $node->nid, null, true);
			drupal_set_message($conf["tm_subscriptions_organization_expired_message"], "help-tip");
			drupal_goto("node/" . $node->nid);
		}
	}

}


/**
 * Render subscription CTA banner
 * Called from tilithon.tpl.php
 */
function tm_subscriptions_organization_cta_banner($node_id) {

	global $conf;
	global $user;

	$node = node_load($node_id);
	if ($node == null) {
		return;
	}

	if ($node->type != "organization") {
		return;
	}

	if (!tm_subscriptions_check_show_organization_cta($node->nid, $user->uid)) {
		return;
	}

	// get country pricing and subscription
	$country_pricing = tm_subscriptions_get_organization_subscription_price($node);
	$country_subscription = tm_subscriptions_get_organization_subscription($node);

	// get cookie
	$wrapper_style = "";
	if (isset($_COOKIE["tm_subscriptions_hide_organization_cta"])) {
		if ($_COOKIE["tm_subscriptions_hide_organization_cta"] == 1) {
			$wrapper_style = "display: none";
		}
	}

	$wrapper = "<div id='tm-subscriptions-cta-banner' style='" . $wrapper_style . "' class='messages--page_notice messages page_notice'>__CONTENT__</div>";

	$inner_html = $country_subscription["html"];

	// get pricing label
	$pricing_label = tm_subscriptions_get_organization_subscription_field($node, "price_label");
	if ($country_pricing["saving_message"] != "") {
		$pricing_label = $country_pricing["subscription_label"] . " - " . $country_pricing["saving_message"]; // use saving message if partner code applied
	}
	$inner_html = str_replace("__PRICING_LABEL__", $pricing_label, $inner_html);

	// insert into inner html
	$html = str_replace("__CONTENT__", $inner_html, $wrapper);

	return $html;
}

/**
 * Render subscription CTA display suite
 */
function _tm_subscriptions_organization_cta_ds($field, $title = NULL) {

	global $conf;
	global $user;

	$node_id = $field['entity']->nid;
	$organization = $field['entity'];


	// check if we show cta
	$html = "";
	if (tm_subscriptions_check_show_organization_cta($node_id, $user->uid)) {

		$html = $conf["tm_subscriptions_organization_cta_text"];
		$html .= " &middot; <a class='show-cta-banner' target='_blank' href='" . tm_subscriptions_get_organization_subscription_field($organization, "learn_more_url") . "'>Learn more</a>";
	}

	return $html;
}

/**
 * Render subscription CTA display suite
 */
function _tm_subscriptions_organization_badge_ds($field, $title = NULL) {

	global $conf;
	global $user;
	$node_id = $field['entity']->nid;
	$organization = $field['entity'];

	$html = "";
	if (tm_subscriptions_is_organization_subscription_enabled($node_id)) {
		$html = $conf["tm_subscriptions_organization_label"];

		// show extra information to other users
		// check if user is a manager of the organization
		if (tm_organizations_is_company_profile_manager($user->uid, $node_id)) {

			// show support link
			$html .= " &middot; <a target='_blank' href='" . tm_subscriptions_get_organization_subscription_field($organization, "support_url") . "'>Customer Support</a>";
		} else {

			// show learn more link
			$html .= " &middot; <a target='_blank' href='" . tm_subscriptions_get_organization_subscription_field($organization, "learn_more_url") . "'>Learn more</a>";

		}
	}

	return $html;
}

/**
 * Create CTA Stipe Payment JS
 */
function tm_subscriptions_organization_cta_js($node_id) {

	global $conf;
	global $user;

	// validate company
	$company = node_load($node_id);
	if ($company == null) {
		return;
	}

	// STEP 1. GET PRICING
	$pricing = tm_subscriptions_get_organization_subscription_price($company);
	if ($pricing == null) {
		// problem calculating pricing
		return;
	}

	// STEP 2. CREATE STRIPE JS
	$stripe_currency = $pricing["stripe_currency"];	
	$stripe_amount = $pricing["stripe_amount"];
	$user_email = $user->mail;
	if ($conf["tm_subscriptions_stripe_email_field_enabled"]) {
		$user_email = ""; // Allow user enter it as it could be different for accounts
	}
	$stripe_pub_key = $conf["tm_subscriptions_stripe_publishable_key"];
	$stripe_bg_image = $conf["tm_subscriptions_stripe_image_url"];
	$entity_id = $node_id;
	$user_id = $user->uid;
	$partner_code = "";
	if (isset($_GET["partner"])) {
		$partner_code = $_GET["partner"];
	}

	$tm_subscriptions_stripe_logo = $conf["tm_subscriptions_stripe_logo"];

	$tm_subscriptions_handler_name = $conf["tm_subscriptions_handler_name"]; // ie: "Travel Massive";
	$tm_subscriptions_handler_description = tm_subscriptions_get_organization_subscription_field($company, "stripe_description"); // ie: "Company Subscription";

	// Enable billing address
	$enable_billing_address = "false";
	if ($conf["tm_subscriptions_enable_billing_address"]) {
		$enable_billing_address = "true";
	}

	$stripe_js = <<<EOT

jQuery(document).ready(function($) {
	
	var handler = StripeCheckout.configure({
		key: '{$stripe_pub_key}',
		image: '{$stripe_bg_image}',
		locale: 'auto',	
		token: function(token) {

			$('.payment-button').addClass('disabled');

			$('.payment-button').append('<form id="process-stripe-payment"></form>');
			$('#process-stripe-payment').attr("action","/payment/subscription-organization/{$entity_id}/{$user_id}?partner={$partner_code}");
			$('#process-stripe-payment').attr("method","post");
			$('#process-stripe-payment').append('<input type="hidden" name="stripe_token" id="stripe_token" value="">');
			$('#process-stripe-payment').append('<input type="hidden" name="stripe_email" id="stripe_email" value="">');
			$('#process-stripe-payment').append('<input type="hidden" name="stripe_amount" id="stripe_amount" value="{$stripe_amount}">');
			$('#process-stripe-payment').append('<input type="hidden" name="stripe_currency" id="stripe_currency" value="{$stripe_currency}">');


			$('#stripe_token').val(token.id);
			$('#stripe_email').val(token.email);

			$('#process-stripe-payment').submit();
		}
	});

	$('.payment-button').on('click', function(e) {
		// Open Checkout with further options
		handler.open({
			name: '{$tm_subscriptions_handler_name}',
			description: '{$tm_subscriptions_handler_description}',
			amount: $stripe_amount,
			currency: '{$stripe_currency}',
			billingAddress: {$enable_billing_address},
			email: '{$user_email}'
		});
		e.preventDefault();
	});

	// Close Checkout on page navigation
	$(window).on('popstate', function() {
		handler.close();
	});

	// Add stripe logo to information panel
	$('.detail-item.price').after("<div class='detail-item tm-payment-gateway-logo'>{$tm_subscriptions_stripe_logo}</div>");
	
	// Hide the CTA banner
	$('.hide-cta-banner').on('click', function(e) {
		$("#tm-subscriptions-cta-banner").hide();
		var cookie = ['tm_subscriptions_hide_organization_cta=1; domain=.', window.location.host.toString(), '; path=/;'].join('');
		document.cookie = cookie;
	});

	// Show the CTA banner
    $('.show-cta-banner').on('click', function(e) {
    	console.log("here");
		$("#tm-subscriptions-cta-banner").show();
		var cookie = ['tm_subscriptions_hide_organization_cta=0; domain=.', window.location.host.toString(), '; path=/;'].join('');
		document.cookie = cookie;
	});


});

EOT;

	// STEP 3. MODIFY BUTTON AND INCLUDE STRIPE JS
	drupal_add_js('https://checkout.stripe.com/checkout.js', 'external'); // add stripe library
	drupal_add_js($stripe_js, 'inline'); // add inline js
	return true;

}

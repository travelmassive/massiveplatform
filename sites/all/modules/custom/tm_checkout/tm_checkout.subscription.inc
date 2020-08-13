<?php

// tm_checkout.upgrade.inc

/** 
 * Public landing page for upgrade subscription
 * /membership
 */
function tm_checkout_public_subscription_page() {

	global $conf;
	global $user;

	// process partner cookie
	tm_subscriptions_user_set_partner_cookie();

	// handle some directs for signed in members:
	// * if user is logged in and subscribed, and in renewal period go to /checkout/renew
	// * if user is logged in and subscribed, and in not renewal period go to subscription confirm page
	// * if user is logged out, and can upgrade, go to /checkout/upgrade page
	if ((user_is_logged_in() and (!isset($_GET["public_view"])))) {

		// check if user is already subscribed
		if (tm_subscriptions_is_user_subscription_enabled($user->uid)) {

			$account = user_load($user->uid);
		
			// if in renewal period, go to renewal page
			if (tm_subscriptions_check_user_renewal_period($account)) {
				drupal_goto("checkout/renew/" . $account->uid);
			}

			// go to checkout landing
			drupal_goto($conf['tm_checkout_subscription_confirm_page'] . "/" . $account->uid);

		} else {

			// check if only verified accounts can upgrade
			if ($conf["tm_subscriptions_user_cta_approved_account"]) {
				if (in_array("approved user", $user->roles)) {
					drupal_goto("checkout/upgrade");
				}
			} else {
				drupal_goto("checkout/upgrade");
			}
		}

	}

	// check if user is already subscribed
	if (tm_subscriptions_is_user_subscription_enabled($user->uid)) {

		$account = user_load($user->uid);

		// if in renewal period, go to renewal page
		if (tm_subscriptions_check_user_renewal_period($account)) {
			drupal_goto("checkout/renew/" . $account->uid);
		}

	}

	// include css
	drupal_add_css(drupal_get_path('module', 'tm_checkout') . '/css/tm_checkout.css');

	// get title and message
	// default
	$default_country = $conf["tm_subscriptions_user_default"];
	$title = $conf["tm_subscriptions_user"][$default_country]["checkout_public_title"];
	$message = $conf["tm_subscriptions_user"][$default_country]["checkout_public_message"];

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_subscription.tpl.php');

	// lookup and replace stats
	if (module_exists("tm_api")) {

		// fetch api data
		$api_data = tm_api_stats_get_data();

		// replace fields
		$title = tm_api_replace_text_fields($api_data, $title);
		$message = tm_api_replace_text_fields($api_data, $message);
	}

	// render list of subscribers
	$new_subscribers_html = tm_subscriptions_user_render_subscribers(16);
	$message = str_replace("__NEW_SUBSCRIBERS_LIST__", $new_subscribers_html, $message);

	// create html
	$html = str_replace("__TITLE__", $title, $template);
	$html = str_replace("__MESSAGE__", $message, $html);
	$html = str_replace("__CHECKOUT_BUTTON__", "", $html);
	$html = str_replace("__ERROR_MESSAGE__", "", $html);

	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
	);
	return $form;
}


/** 
 * Upgrade subscription
 * /checkout/upgrade
 * /checkout/upgrade/123
 */
function tm_checkout_upgrade_subscription_page() {

	global $conf;
	global $user;

	// if logged out, show anonymous subscription page
	if (!user_is_logged_in()) {
		drupal_goto($conf["tm_checkout_subscription_public_page"]);
	}

	// get user
	$user_id = $user->uid;
	if (arg(2) != "") {
		$user_id = intval(arg(2));
	}
	$account = user_load($user_id);
	
	// check valid user
	if ($account == null) {
		drupal_goto("/");
	}

	// check if only verified accounts can upgrade
	if ($conf["tm_subscriptions_user_cta_approved_account"]) {
		if (!in_array("approved user", $account->roles)) {
			// go to own profile page
			drupal_set_message("Only verified members can purchase a subscription.", "help-tip");
			drupal_goto($conf["tm_checkout_subscription_public_page"]);
		}
	}

	// chapter leaders notice
	if (in_array("chapter leader", $account->roles)) {
		drupal_set_message($conf["tm_checkout_subscription_leader_message"]);
	}

	// check if user is already subscribed
	if (tm_subscriptions_is_user_subscription_enabled($account->uid)) {

		// if in renewal period, go to renewal page
		if (tm_subscriptions_check_user_renewal_period($account)) {
			drupal_goto("checkout/renew/" . $account->uid);
		}

		// go to checkout landing
		drupal_goto($conf['tm_checkout_subscription_confirm_page'] . "/" . $account->uid);
	}

	// show upgrade page
	return tm_checkout_subscription_page("upgrade_subscription");
}

/** 
 * Renew subscription
 * /checkout/renew
 * /checkout/renew/123
 */
function tm_checkout_renew_subscription_page() {

	global $conf;
	global $user;

	// must be logged in to renew
	if (!user_is_logged_in()) {
		drupal_set_message("You must be <a href='/user/login?destination=/checkout/renew'>logged in</a> to renew your subscription.", "help-tip");
		drupal_goto($conf["tm_checkout_public_subscription_page"]);
	}

	// get user
	$user_id = $user->uid;
	if (arg(2) != "") {
		$user_id = intval(arg(2));
	}
	$account = user_load($user_id);
	
	// check valid user
	if ($account == null) {
		drupal_goto("/");
	}
	
	// check if renewal is needed
	$is_renewal_period = tm_subscriptions_check_user_renewal_period($account);
	if (!$is_renewal_period) {
		drupal_goto($conf['tm_checkout_subscription_confirm_page'] . "/" . $account->uid);
	}

	// in renewal period
	return tm_checkout_subscription_page("renew_subscription");
}

/** 
 * Success subscription
 * $conf['tm_checkout_subscription_confirm_page']
 * /membership/landing
 * /membership/landing/123
 */
function tm_checkout_landing_subscription_page() {

	global $conf;
	global $user;

	// check logged in
	if (!user_is_logged_in()) {
		drupal_goto($conf["tm_checkout_subscription_public_page"]);
	}

	// get user
	$user_id = $user->uid;
	if (arg(2) != "") {
		$user_id = intval(arg(2));
	}
	$account = user_load($user_id);
	
	// check valid user
	if ($account == null) {
		drupal_goto("/");
	}

	// only moderators and chapter leaders can upgrade or renew another member
	if (!in_array("moderator", $user->roles) and !in_array("chapter leader", $user->roles)) {
		if ($account->uid != $user->uid) {
			drupal_goto($conf['tm_checkout_subscription_confirm_page']);
		}
	}

	// check you have a subscription
	if (!tm_subscriptions_is_user_subscription_enabled($account->uid)) {
		if (isset($_GET["session_id"])) {
			drupal_set_message(" It looks like you made a payment, but we need to manually confirm it. Please <a href='/contact'>contact our team</a> so we can assist you.");
		}
		drupal_goto("checkout/upgrade/" . $account->uid);
	}

	// include css
	drupal_add_css(drupal_get_path('module', 'tm_checkout') . '/css/tm_checkout.css');

	// get title and message
	$title = tm_subscriptions_get_user_subscription_field($account, "checkout_landing_title");
	$message = tm_subscriptions_get_user_subscription_field($account, "checkout_landing_message");

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_subscription.tpl.php');

	// lookup and replace stats
	if (module_exists("tm_api")) {

		// fetch api data
		$api_data = tm_api_stats_get_data();

		// replace fields
		$title = tm_api_replace_text_fields($api_data, $title);
		$message = tm_api_replace_text_fields($api_data, $message);
	}

	// view payments url
	$view_payments_url = "/checkout/my-payments/" . $account->uid;
	$message = str_replace("__VIEW_PAYMENTS_URL__", $view_payments_url, $message);

	// create html
	$html = str_replace("__TITLE__", $title, $template);
	$html = str_replace("__MESSAGE__", $message, $html);
	$html = str_replace("__CHECKOUT_BUTTON__", "", $html);
	$html = str_replace("__ERROR_MESSAGE__", "", $html);

	// replace template fields
	$html = tm_subscriptions_user_replace_template_fields($account, $html);

	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
		);
	return $form;
}

/** 
 * Checkout subscription
 * Handles upgrade, renewal
 */
function tm_checkout_subscription_page($checkout_action = "upgrade_subscription") {

	global $conf;
	global $user;

	// get user
	$user_id = $user->uid;
	if (arg(2) != "") {
		$user_id = intval(arg(2));
	}
	$account = user_load($user_id);
	
	// user processing the checkout
	$user_processing = user_load($user->uid);

	// Only moderators and chapter leaders can upgrade or renew another member
	if (!in_array("moderator", $user_processing->roles) and !in_array("chapter leader", $user_processing->roles)) {
		if ($account->uid != $user_processing->uid) {
			if ($checkout_action == "upgrade_subscription") {
				drupal_goto("/checkout/upgrade");
			}
			if ($checkout_action == "renew_subscription") {
				//drupal_goto("/checkout/renew");
			}
		}
	}

	// include css
	drupal_add_css(drupal_get_path('module', 'tm_checkout') . '/css/tm_checkout.css');

	// check that we have subscription data
	$subscription = tm_subscriptions_get_user_subscription($account); // subscription data
	if ($subscription == null) {

		// prepare message
		$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_subscription.tpl.php');
		$html = str_replace("__TITLE__", "Please contact us.", $template);
		$html = str_replace("__MESSAGE__", "<p class='tm-checkout-error-message'>Membership pricing is not yet available for your country.<br>Please <a href='/contact'>contact our team</a> for assistance.</p>", $html);
		
		// render form
		$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
		);
		return $form;
	}

	// include stripe js
	drupal_add_js('https://js.stripe.com/v3/', 'external');

	// include stripe publishable key
	$strip_inline_js = "var stripe = Stripe('" . $conf["tm_checkout_stripe_publishable_key"] . "');";
	drupal_add_js($strip_inline_js, array('type' => 'inline', 'scope' => 'footer'));

	// include js
	drupal_add_js(drupal_get_path('module', 'tm_checkout') . '/js/tm_checkout_subscription.js');

	// set checkout button ajax url
	$checkout_button_ajax_url = "/checkout/ajax/checkout-button/" . $account->uid . "?checkout_action=" . $checkout_action;
	drupal_add_js(array('tm_checkout_subscription' => array('checkout_button_ajax_url' => $checkout_button_ajax_url)), array('type' => 'setting'));

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_subscription.tpl.php');

	// upgrade_subscription
	if ($checkout_action == "upgrade_subscription") {
		$title = tm_subscriptions_get_user_subscription_field($account, "checkout_upgrade_title");
		$message = tm_subscriptions_get_user_subscription_field($account, "checkout_upgrade_message");
	}

	// renew_subscription
	if ($checkout_action == "renew_subscription") {
		$title = tm_subscriptions_get_user_subscription_field($account, "checkout_renew_title");
		$message =tm_subscriptions_get_user_subscription_field($account, "checkout_renew_message");
	}

	// lookup and replace stats
	if (module_exists("tm_api")) {

		// fetch api data
		$api_data = tm_api_stats_get_data();

		// replace fields
		$title = tm_api_replace_text_fields($api_data, $title);
		$message = tm_api_replace_text_fields($api_data, $message);
	}

	// render list of subscribers
	$new_subscribers_html = tm_subscriptions_user_render_subscribers(16);
	$message = str_replace("__NEW_SUBSCRIBERS_LIST__", $new_subscribers_html, $message);

	// create html
	$html = str_replace("__TITLE__", $title, $template);
	$html = str_replace("__MESSAGE__", $message, $html);

	// replace template fields
	$html = tm_subscriptions_user_replace_template_fields($account, $html);
	
	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
		);
	return $form;
}

/**
 * Render the checkout button
 * checkout/ajax/checkout-button/123?checkout_action=upgrade_subscription
 */
function tm_checkout_subscription_button_ajax() {

	global $conf;
	global $user;

	// must be logged in
	if (!user_is_logged_in()) {
		return;

	}
	// need user id
	if (arg(3) == "") {
		return;
	}

	// need checkout_action
	if (!isset($_GET["checkout_action"])) {
		return;
	}

	// validate checkout action
	$checkout_action = $_GET["checkout_action"];
	if (($checkout_action != "upgrade_subscription") and  ($checkout_action != "renew_subscription")) {
		return;
	}

	// get user	
	$user_id = intval(arg(3));
	$account = user_load($user_id);

	// user processing the checkout
	$user_processing = user_load($user->uid);

	// Only moderators and chapter leaders can upgrade or renew another member
	if (!in_array("moderator", $user_processing->roles) and !in_array("chapter leader", $user_processing->roles)) {
		if ($account->uid != $user_processing->uid) {
			return "";
		}
	}

	// check if user  is reported
	if (tm_users_is_member_reported($account->uid)) {
		print("<p class='tm-checkout-error-message'>Looks like we're unable to accept payments right now.<br>Please <a href='/contact'>contact our team</a> for assistance.</p>");
		return;
	}

	// get pricing and subscription
	$pricing = tm_subscriptions_get_user_subscription_price($account); // pricing data
	$pricing_label = tm_subscriptions_get_user_pricing_label($account); // get pricing label
	$subscription = tm_subscriptions_get_user_subscription($account); // subscription data

	// create meta data
	$metadata = array();
	$metadata['tm_processor'] = "tm_checkout";
	$metadata['checkout_action'] = $checkout_action;
	$metadata['payment_email'] = strip_tags($account->mail);
	$metadata['stripe_amount'] = $pricing["stripe_amount"];
	$metadata['display_amount'] = $pricing["display_amount"];
	$metadata['entity_id'] = $account->uid;
	$metadata['entity_type'] = "user";
	$metadata['entity_title'] = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']) . " " . strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$metadata['user_id'] = $account->uid;
	$metadata['user_email'] = strip_tags($account->mail);
	$metadata['user_first_name'] = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$metadata['user_last_name'] = strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$metadata['log_user_ip'] = tm_geoip_get_real_visitor_ip();
	$metadata['log_user_http_user_agent'] = "none";
	if (isset($_SERVER['HTTP_USER_AGENT'])) {
		$metadata['log_user_http_user_agent'] = $_SERVER['HTTP_USER_AGENT'];
	}
	$metadata["referer_code"] = tm_subscriptions_user_get_chapter_code($account);
	$metadata["partner_code"] = $pricing["partner_code"];
	$metadata["partner_commission"] = tm_checkout_get_partner_commission($pricing["partner_code"]);
	$metadata["user_subscription_type"] = $pricing["subscription_type"];
	$metadata['user_subscription_label'] = $pricing["subscription_label"];

	// site url
	$site_url = $conf['tm_email_server_http'] . "://" . $conf['tm_email_server_url_domain'];

	// default fields
	$checkout_button = "";
	$error_message = "";

	// init Stripe
	$stripe_init_file = drupal_get_path('module', 'tm_checkout') . "/includes/" . $conf["tm_checkout_stripe_library_path"] . '/init.php';
	if (!file_exists($stripe_init_file)) {
		print("<p class='tm-checkout-error-message'>Looks like we're unable to accept payments right now.<br>Please <a href='/contact'>contact our team</a> for assistance.<br><i>(Stripe library not installed)</i></p>");
		return;
	}
	require_once($stripe_init_file);

	try {

		// See your keys here: https://dashboard.stripe.com/account/apikeys
		\Stripe\Stripe::setApiKey($conf["tm_checkout_stripe_secret_key"]);

		// Set API version
		\Stripe\Stripe::setApiVersion($conf["tm_checkout_stripe_api_version"]);

		// upgrade_subscription
		if ($checkout_action == "upgrade_subscription") {
			$success_url = $site_url . "/" . $conf["tm_checkout_subscription_confirm_page"] . "/" . $user_id . "?session_id={CHECKOUT_SESSION_ID}";
			$cancel_url = $site_url . "/checkout/upgrade/" . $user_id;
		}

		// renew_subscription
		if ($checkout_action == "renew_subscription") {
			$success_url = $site_url . "/" . $conf["tm_checkout_subscription_confirm_page"] . "/" . $user_id . "?session_id={CHECKOUT_SESSION_ID}";
			$cancel_url = $site_url . "/checkout/renew/" . $user_id;
		}
		
		// Create session
		$session = \Stripe\Checkout\Session::create([
			'payment_method_types' => ['card'],
			'line_items' => [[
				'price_data' => [
					'currency' => $pricing["stripe_currency"],
					'product_data' => [
						'name' => $pricing["stripe_description"],
					],
					'unit_amount' => $pricing["stripe_amount"],
				],
				'quantity' => 1,
			]],
			// 'customer_email' => $account->email,
			'mode' => 'payment', 
			'success_url' => $success_url, 
			'cancel_url' => $cancel_url,
			'metadata' => $metadata
		]);

		$checkout_button = '<button id="checkout-button" class="btn btn-success btn-lg" data-secret="' . $session->id . '">Checkout</button>';

	} catch (Exception $e) {
		$checkout_button = "";
		$pricing_label = "";
		$error_message = "<p class='tm-checkout-error-message'>🤔 Looks like we can't connect to our payment system right now.<br>Please <a href='/contact'>contact our team</a> for assistance.</p>";
	}

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_subscription_button.tpl.php');

	// check and display partner code
	$partner_code_label = "";
	$partner_code = strtoupper($pricing["partner_code"]);
	if ($partner_code != "") {
		if (isset($subscription["partner_codes"][$partner_code])) {
			$partner_code_label = "Partner code " . $partner_code . " applied 🎉";
		} else {
			$partner_code_label = "Partner code " . $partner_code . " is invalid or expired.";
		}
	}

	// generate html
	$html = str_replace("__CHECKOUT_BUTTON__", $checkout_button, $template);
	$html = str_replace("__PRICING_LABEL__", $pricing_label, $html);
	$html = str_replace("__PARTNER_CODE_LABEL__", $partner_code_label, $html);
	$html = str_replace("__ERROR_MESSAGE__", $error_message, $html);

	// output
	print $html;
	return;
}

/*
 * Process payment
 */
function tm_checkout_subscription_process($stripe_session) {

	global $conf;
	global $user;

	// STEP 1. Fetch Stripe metadata
	$metadata = $stripe_session->metadata;

	// STEP 2. FETCH ACCOUNT
	$entity_id = $metadata['entity_id'];
	$entity_type = $metadata['entity_type'];
	if ($entity_type != "user") {
		echo "{'success': false, 'message': 'Invalid entity_type'}";
		return;
	}
	$account = user_load($entity_id);
	if ($account == null) {
		echo "{'success': false, 'message': 'Invalid entity_id'}";
		return;
	}
	$user_processing = user_load($metadata['user_id']);
	if ($user_processing == null) {
		echo "{'success': false, 'message': 'Invalid user_id'}";
		return;
	}
	
	// STEP 2. Set payment vars
	$payment_id = $stripe_session->payment_intent;
	$checkout_action = $metadata['checkout_action']; // upgrade_membership
	$payment_email = $metadata['payment_email'];
	$stripe_currency = $stripe_session->currency;
	$stripe_amount = $metadata['stripe_amount'];
	$display_amount = $metadata['display_amount'];
	$entity_id = $metadata['entity_id'];
	$entity_type = $metadata['entity_type'];
	$entity_title = $metadata['entity_title'];
	$user_id = $metadata['user_id'];
	$user_email = $metadata['user_email'];
	$user_first_name = $metadata['user_first_name'];
	$user_last_name = $metadata['user_last_name'];
	$log_user_ip = $metadata['log_user_ip'];
	$log_user_http_user_agent = $metadata['log_user_http_user_agent'];
	$refunded = 0;
	$paid_out = 0;
	$referer_code = $metadata['referer_code'];
	$partner_code = $metadata['partner_code'];
	$partner_commission = $metadata['partner_commission'];
	$subscription_type = $metadata['user_subscription_type'];
	$checkout_item = $subscription_type;
	$subscription_label = $metadata['user_subscription_label'];

	// Check if is renewed
	$is_renewed = tm_subscriptions_check_user_renewal_period($account);
	if ($is_renewed) {
  		$checkout_action = "renew_membership";
  	}

  	// STEP 3. Log payment
	tm_checkout_log_payment(null, $payment_id, $checkout_action, $checkout_item, $payment_email, $stripe_currency, $stripe_amount, $display_amount, $entity_id, $entity_type, $entity_title, $user_id, $user_email, $user_first_name, $user_last_name, $log_user_ip, $log_user_http_user_agent, $refunded, $paid_out, $referer_code, $partner_code, $partner_commission);

	// STEP 4. Upgrade account
	$subscription = tm_subscriptions_get_user_subscription($account); // subscription data

	// Get subscription flags
	$user_subscription_enable = flag_get_flag('user_subscription_enable', NULL);
	$user_subscription_paid = flag_get_flag('user_subscription_paid', NULL);

	// Flag
	$user_subscription_enable->flag('flag', $account->uid, $user_processing, TRUE);
	$user_subscription_paid->flag('flag', $account->uid, $user_processing, TRUE);

	// Set subscription type
	$account->field_user_subscription_type[LANGUAGE_NONE][0]['value'] = $subscription_type;

	// Set subscription expiry date
	// Default expire date set from now
	$start_date = "";
	if (isset($account->field_user_subscription_date[LANGUAGE_NONE][0]['value'])) {

		$existing_expiry_date = $account->field_user_subscription_date[LANGUAGE_NONE][0]['value'];
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
	$account->field_user_subscription_date[LANGUAGE_NONE][0]['value'] = $subscription_expires_date;
	
	// Save changes to account
	// set static variable so roles are not preserved in tm_users_user_presave
	// $tm_subscriptions_user_flag = &drupal_static("tm_subscriptions_user_flag");
	// $tm_subscriptions_user_flag = true;
	user_save($account);

	// STEP 5. Send notifications
	if ($is_renewed) {
		tm_subscriptions_user_send_notification($account, "renewed", $user_first_name);
	} else {
		tm_subscriptions_user_send_notification($account, "purchase", $user_first_name);
	}

	// Send chapter leader notifications
	if (module_exists("tm_notifications_subscriptions_user")) {
		$notify_chapter = tm_subscriptions_user_get_chapter($account);
		tm_notifications_subscriptions_user_notify_chapter_leaders($account, $notify_chapter, $is_renewed, $subscription_label);
	}

	echo "{'success': true, 'message': 'Payment processed'}";

}

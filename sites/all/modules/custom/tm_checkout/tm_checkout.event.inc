<?php

// tm_checkout.event.inc

// todo
// show tickets to signed out, if enabled

/** 
 * Event payment
 * /checkout/event/123
 */
function tm_checkout_event_payment_page() {

	global $conf;
	global $user;

	// get event
	$event_id = intval(arg(2));
	$event = node_load($event_id);

	// check valid event
	if ($event == null) {
		drupal_goto("/");
	}

	// check payments module
	if (!module_exists("tm_payments")) {
		drupal_set_message("Sorry, payments are not currently available.", "help-tip");
		drupal_goto("node/" . $event->nid);
	}

	// if logged out, go to event page
	if (!user_is_logged_in()) {
		drupal_set_message("Please log in to make an event payment.", "help-tip");
		drupal_goto("node/" . $event->nid);
	}

	// check payments enabled
	if (!_tm_payments_is_payment_enabled($event)) {
		drupal_set_message("Payments are not enabled for this event.", "help-tip");
		drupal_goto("node/" . $event->nid);
	}

	// get user id
	$user_id = $user->uid;
	if (isset($_GET["uid"])) {
		$user_id = intval($_GET["uid"]);
	}

	// load account
	$account = user_load($user_id);
	if ($account == null) {
		drupal_goto("node/" . $event->nid);
	}

	// user can edit event
	$can_edit_event = tm_events_check_permission_edit_event($account, $event);

	// check that user can checkout
	// will check signup limit, if event must be verified, etc
	$display_flag = _tm_event_display_flag($event->nid, $account);
	if (($display_flag != 'show_checkout') and ($display_flag != 'show_donate')) {
		if (!$can_edit_event) {
			drupal_set_message("Sorry, you can't purchase a ticket to this event.", "help-tip");
			drupal_goto("node/" . $event->nid);
		} else if (_tm_event_is_past($event)) {
			// if past event and use can edit
			$payments_past_events = false;
			if (isset($event->field_event_payment_post_event[LANGUAGE_NONE][0]['value'])) {
				$payments_past_events = ($event->field_event_payment_post_event[LANGUAGE_NONE][0]['value'] == 1);
          	}
          	if (!$payments_past_events) {
          		drupal_set_message("To accept payments for past events, enable 'Accept payments after the event' in your event settings.", "help-tip");
				drupal_goto("node/" . $event->nid);
          	}
		}
	}

	// check user has already paid for event
	$paid_flag = flag_get_flag('event_paid');
	$register_flag = flag_get_flag('event_register');
	$paid_for_event = ($paid_flag && $paid_flag->is_flagged($event->nid, $account->uid));
	$registered_for_event = ($register_flag && $register_flag->is_flagged($event->nid, $account->uid));
	
	
	// if paid for event and regular account, redirect
	if ($paid_for_event and !$can_edit_event) {
		
		// already registered
		if ($registered_for_event) {
			drupal_set_message("You've already purchased a ticket for this event.");
			drupal_goto("node/" . $event->nid);
		} else {
			drupal_set_message("You previously purchased a ticket for this event.");
		}
	}

	// if paid event and event organizer
	if ($paid_for_event and $can_edit_event) {
		drupal_set_message("You've already purchased a ticket for this event.");
	}

	// preview mode
    if (tm_events_is_preview_mode($event->nid)) {
        drupal_set_message("This event is in Preview mode. Only people with the link can see this checkout page.", "help-tip");
    }

	// if event organizer, show without discounts
	if ($can_edit_event) {
		if (!isset($_GET["partner"])) {
			$_GET["partner"] = "PREVIEW";
		}
	} else {
		// only event organizers can use PREVIEW
		if (isset($_GET["partner"])) {
			if (strtoupper(trim($_GET["partner"])) == "PREVIEW") {
				unset($_GET["partner"]);
			}
		}
	}

	// show ticket page
	return tm_checkout_event_page($event_id, $user_id);
}

/** 
 * Success event payment
 * /checkout/event-landing/123
 */
function tm_checkout_event_landing_page() {

	global $conf;
	global $user;

	// check logged in
	if (!user_is_logged_in()) {
		drupal_goto($conf["tm_checkout_subscription_public_page"]);
	}

	// get event
	$event_id = intval(arg(2));
	$event = node_load($event_id);
	if ($event == null) {
		drupal_goto("/");
	}

	// get user
	if (!isset($_GET["uid"])) {
		drupal_goto("/");
	}
	$user_id = $_GET["uid"];
	$account = user_load($user_id);
	if ($account == null) {
		drupal_goto("/");
	}
	
	// check user is registered for event
	if (!tm_events_user_is_registered($event_id, $account->uid)) {
		if (isset($_GET["session_id"])) {
			drupal_set_message("It looks like you made a payment, but we need to manually confirm it. Please <a href='/contact'>contact our team</a> so we can assist you.");
			tm_checkout_notify_payment_error("Check Stripe webhook log. User not registered for event after checkout. event_id: " . $event_id . ", user_id: " . $account->uid);
		}
		drupal_goto("node/" . $event->nid);
	}

	// message text
	$message_text = "Thanks for your payment. You are now registered for this event.";
	if (isset($conf["tm_payments_process_successful_message_text"])) {
		$message_text = $conf["tm_payments_process_successful_message_text"];
	}

	// show donation message
	$event_register_timestamp = 0;
	$event_paid_timestamp = 0;
	$user_flags = flag_get_user_flags('node', $event_id, $account->uid);
	if (isset($user_flags["event_register"])) {
		$event_register_timestamp = $user_flags["event_register"]->timestamp;
	}
	if (isset($user_flags["event_paid"])) {
		$event_paid_timestamp = $user_flags["event_paid"]->timestamp;
	}
	
	// check difference in time between register and paid flag timestamp
	if (($event_paid_timestamp - $event_register_timestamp) > (30)) {
		$message_text = "Thanks for your donation!";
		if (isset($conf["tm_payments_process_donation_message_text"])) {
			$message_text = $conf["tm_payments_process_donation_message_text"];
		}
	}

	// message class
	$message_class = "status";
	if (isset($conf["tm_payments_process_successful_message_class"])) {
		$message_class = $conf["tm_payments_process_successful_message_class"];
	}

	drupal_set_message($message_text, $message_class);
	drupal_goto("node/" . $event->nid);
	return;
}

/** 
 * Checkout event payment
 */
function tm_checkout_event_page($event_id, $user_id) {

	global $conf;
	global $user;

	// get user
	$account = user_load($user_id);
	
	// user processing the checkout
	$user_processing = user_load($user->uid);

	// Only moderators and chapter leaders can purchase a ticket on behalf of another user
	if (!in_array("moderator", $user_processing->roles) and !in_array("chapter leader", $user_processing->roles)) {
		if ($account->uid != $user_processing->uid) {
			drupal_goto("/checkout/event/" . $event_id);
		}
	}

	// load event
	$event = node_load($event_id);

	// get event currency
	$currency = $event->field_event_payment_currency[LANGUAGE_NONE][0]['value'];

	// see if we can get a better match
	$currency = tm_checkout_get_user_currency($currency);

	// override with ?currency
	if (isset($_GET["currency"])) {
		if (trim($_GET["currency"]) != "") {
			$currency = strtolower($_GET["currency"]);
		}
	}

	// check we have a valid currency
	if (!isset($conf["tm_payments_currencies"]["usd"])) {
		$currency = $conf["tm_payments_currencies_default"];
	}

	// get partner code
	$partner_code = tm_checkout_event_get_partner_code($event);
	if ($partner_code != "PREVIEW") {
		if (isset($_GET["partner"])) {
			$partner_code_provided = htmlentities(strip_tags(strtoupper($_GET["partner"])));
			if ($partner_code != "") {
				drupal_set_message("Your partner code " . $partner_code_provided . " has been applied.");
			} elseif ($partner_code_provided != "") {
				drupal_set_message("The partner code " . $partner_code_provided . " is invalid or expired.", "help-tip");
			}
		}
	}
	
	// include css
	drupal_add_css(drupal_get_path('module', 'tm_checkout') . '/css/tm_checkout.css');

	// get payment type
	$payment_type = _tm_payments_get_payment_type($event);

	// check that we have pricing data
	$no_pricing_options = false;
	$pricing = _tm_payments_get_registration_price($event, $account, $currency, $partner_code);

	// check fixed price
	if ($payment_type == "fixed_price") {
		if ($pricing == null) {
			$no_pricing_options = true;
		}
	}

	// check pay what you wish
	if ($payment_type == "pay_what_you_wish") {
		if (!isset($event->field_event_payment_tiers[LANGUAGE_NONE][0]['value'])) {
			$no_pricing_options = true;
		} else if (trim($event->field_event_payment_tiers[LANGUAGE_NONE][0]['value']) == "") {
			$no_pricing_options = true;
		}
	}

	// if no pricing options for fixed_payment, show message
	if ($no_pricing_options and (($payment_type == "fixed_price"))) {

		// prepare message
		$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_event.tpl.php');
		$html = str_replace("__TITLE__", "Please contact us.", $template);
		$html = str_replace("__MESSAGE__", "<style>.tm-cta-intro-footer, .tm-checkout-pricing-options, .tm-checkout-currency-container, hr { display: none; }</style><p class='tm-checkout-error-message'>Ticket pricing is not available for this event.<br>Please <a href='/contact'>contact our team</a> for assistance.</p>", $html);
		$html = str_replace("__EVENT_URL__", "/contact", $html);
		$html = str_replace("__BUTTON__", "", $html);
		$html = str_replace("__PARTNER_MESSAGE__", "", $html);
		$html = str_replace("__CURRENCY__", "", $html);
		$html = str_replace("__PRICING_OPTIONS__", "", $html);
		$html = str_replace("__ATTENDEES__", "", $html);
		$html = str_replace("__REFUND_TEXT__", "", $html);

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
	drupal_add_js(drupal_get_path('module', 'tm_checkout') . '/js/tm_checkout_event.js');

	// set checkout button ajax url
	$checkout_button_ajax_url = "/checkout/ajax/checkout-event-button/" . $event->nid . "?uid=" . $account->uid . "&currency=" . $currency;
	if ($partner_code == "PREVIEW") {
		$checkout_button_ajax_url .= "&preview";
	}
	drupal_add_js(array('tm_checkout_event' => array('checkout_button_ajax_url' => $checkout_button_ajax_url)), array('type' => 'setting'));

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_event.tpl.php');

	// get message
	$message = "";
	if (isset($event->field_event_payment_message_text[LANGUAGE_NONE][0]['value'])) {
		$message = trim(strip_tags($event->field_event_payment_message_text[LANGUAGE_NONE][0]['value']));
	}

	// apply markdown
	if (tm_base_check_markdown_permission("event", $event->nid)) {
		$message = tm_base_parsedown($message, "");
	}

	// currency options
	$currency_html = 'Show prices in <select name="tm_checkout_currency_options" id="tm_checkout_currency_options" onChange="this.options[this.selectedIndex].value && (window.location = this.options[this.selectedIndex].value);">';
	$currency_uid = "";
	if ($user_processing->uid != $account->uid) {
		$currency_uid = "&uid=" . $account->uid;
	}
	foreach($conf["tm_payments_currencies"] as $choose_currency => $currency_symbol) {
		$selected = "";
		if ($choose_currency == $currency) {
			$selected = "selected='selected'";
		}

		$currency_html .= '<option value="/checkout/event/' . $event->nid . '?currency=' . $choose_currency . $currency_uid . '" ' . $selected . '>' . $conf["tm_payments_currency_symbols"][$choose_currency] . " " . strtoupper($choose_currency) . '</option>';
	}
	$currency_html .= '</select>';

	$button = "<div id='tm-checkout-event-button-container'></div>";

	// pricing options
	$pricing_options = "";

	// partner message
	$partner_message = "";
	$partner_message = tm_checkout_event_render_partner_message($event, $account, $currency, $partner_code);

	// render pricing options
	if ($payment_type == "fixed_price") {
		$title = $pricing["ticket_label"] . " &mdash; " . strip_tags($event->title);
		$pricing_options = "<br>Get a ticket for " . $pricing["currency_symbol"] . $pricing["display_amount"] . " — fast and secure.";
		if ($pricing["display_amount"] == 0) {
			$pricing_options = "<br>Hey there, rock star! 🎸";
		}
	}
	if ($payment_type == "pay_what_you_wish") {

		// show ticket
		$title = "Ticket &mdash; " . strip_tags($event->title);

		// show donate if already registered
		if ((tm_events_user_is_registered($event->nid, $account->uid)) and ($partner_code != "PREVIEW")) {
			$title = "Donate &mdash; " . strip_tags($event->title);
		}

		$pricing_options = tm_checkout_event_render_pricing_options($event, $account, $currency, $partner_code);
	}

	// refund text
	$refund_text = $conf["tm_payments_refund_policy"];
	if (isset($event->field_event_payment_refund_text[LANGUAGE_NONE][0]['value'])) {
		$refund_text = strip_tags($event->field_event_payment_refund_text[LANGUAGE_NONE][0]['value']);
	}

	// lookup and replace stats
	if (module_exists("tm_api")) {

		// fetch api data
		$api_data = tm_api_stats_get_data();

		// replace fields
		$title = tm_api_replace_text_fields($api_data, $title);
		$message = tm_api_replace_text_fields($api_data, $message);
	}

	// event url
	$event_url = "/" . drupal_get_path_alias("node/" . $event->nid);

	// render list of attendees
	$attendees_html = "";

	// get number of attendees
	$is_past_event = _tm_event_is_past($event);
	$flag = flag_get_flag('event_register');
	$number_of_attendees_flagged = $flag->get_count($event->nid);
	$number_of_attendees = $number_of_attendees_flagged;
	if (isset($event->field_event_extra_guests[LANGUAGE_NONE][0]['value'])) {
		$extra_guests_count = $event->field_event_extra_guests[LANGUAGE_NONE][0]['value'];
		$number_of_attendees = $number_of_attendees + $extra_guests_count;
	}

	// render list of attendees
	$num_attendees_to_display = 32; // default
	if (isset($conf["tm_payments_max_display_profiles"])) {
		$num_attendees_to_display = $conf["tm_payments_max_display_profiles"];
	}
	if ($number_of_attendees_flagged > 5) {
		$attendees_html = "<div class='tm-registrations-attending'>";
		$attendees_html .= "<span style='display: block; margin-bottom: 16px;'>Join <strong>" . $number_of_attendees . " members</strong> supporting this event</span>" . tm_events_render_attendees($event->nid, $num_attendees_to_display);
		if ($number_of_attendees > $num_attendees_to_display) {
			 $attendees_html .= "<span style='font-size: 11pt;'>&nbsp; &mdash; and <a target='_blank' class='tm-registrations-attending-more' href='/node/" . $event->nid . "/attendees'>" . ($number_of_attendees - $num_attendees_to_display) . " more</a></span>";
		}
		$attendees_html .= "</div>";
	}

	// create html
	$html = str_replace("__EVENT_URL__", $event_url, $template);
	$html = str_replace("__TITLE__", $title, $html);
	$html = str_replace("__PARTNER_MESSAGE__", $partner_message, $html);
	$html = str_replace("__MESSAGE__", $message, $html);
	$html = str_replace("__CURRENCY__", $currency_html, $html);
	$html = str_replace("__PRICING_OPTIONS__", $pricing_options, $html);
	$html = str_replace("__BUTTON__", $button, $html);
	$html = str_replace("__ATTENDEES__", $attendees_html, $html);
	$html = str_replace("__REFUND_TEXT__", $refund_text, $html);

	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
		);
	return $form;
}

/**
 * Render the checkout button
 * checkout/ajax/event-button/123?uid=123
 */
function tm_checkout_event_button_ajax() {

	global $conf;
	global $user;

	// must be logged in
	if (!user_is_logged_in()) {
		return;
	}

	// need event id
	if (arg(3) == "") {
		return;
	}

	// get ?uid
	if (!isset($_GET["uid"])) {
		return;
	}

	// get user_id
	$user_id = intval($_GET["uid"]);

	// load account buying the ticket
	$account = user_load($user_id);
	if ($account == null) {
		return;
	}

	// get event	
	$event_id = intval(arg(3));
	$event = node_load($event_id);

	// validate event
	if ($event == null) {
		return;
	}
	if ($event->type != "event") {
		return;
	}

	// get ?currency
	$currency = $event->field_event_payment_currency[LANGUAGE_NONE][0]['value'];
	if (isset($_GET["currency"])) {
		if (trim($_GET["currency"]) != "") {
			$currency = strtolower($_GET["currency"]);
		}
	}

	// user processing the checkout
	$user_processing = user_load($user->uid);

	// Only moderators and chapter leaders can upgrade or buy tickets for another member
	if (!in_array("moderator", $user_processing->roles) and !in_array("chapter leader", $user_processing->roles)) {
		if ($account->uid != $user_processing->uid) {
			return;
		}
	}

	// check if user is reported
	if (tm_users_is_member_reported($account->uid)) {
		print("<p class='tm-checkout-error-message'>Looks like we're unable to accept payments right now.<br>Please <a href='/contact'>contact our team</a> for assistance.</p>");
		return;
	}

	// check if sold out
	$num_seats_left = _tm_events_seats_left($event);
	if ($num_seats_left == 0) {
		print("<p class='tm-checkout-error-message'>Sorry, this event has sold out.<br>Visit the <a href='/" . drupal_get_path_alias("node/" . $event->nid) . "'>event page</a> for details.</p>");
		return;
	}

	// check that user can checkout
	$display_flag = _tm_event_display_flag($event->nid, $account);
	if (($display_flag != 'show_checkout') and ($display_flag != 'show_donate')) {
		if (!tm_events_check_permission_edit_event($user_processing, $event)) {
			print("<p class='tm-checkout-error-message'>Looks like we're unable to accept registrations right now.<br>Please <a href='/contact'>contact our team</a> for assistance.</p>");
			return;
		}
	}

	// get payment type
	// fixed_price or pay_what_you_wish
	$payment_type = _tm_payments_get_payment_type($event);

	// get partner code
	$partner_code = tm_checkout_event_get_partner_code($event);

	// check ?preview
	if (isset($_GET["preview"])) {
		$partner_code = "PREVIEW";
	}

	// fixed price
	if ($payment_type == "fixed_price") {
		$pricing = _tm_payments_get_registration_price($event, $account, $currency, $partner_code);
	}

	// pay what you wish
	if ($payment_type == "pay_what_you_wish") {
		$price = 0;
		if (isset($_GET["price"])) {
			$price = intval($_GET["price"]);
		}
		$pricing = _tm_payments_get_pay_what_you_wish_price($event, $account, $currency, $price, "Event Ticket", $partner_code);
	}

	// check price
	$lowest_price = _tm_payments_get_lowest_price($event, $account, $currency, $partner_code);
	if ($lowest_price > $pricing["display_amount"]) {
		$currency_symbol = $conf["tm_payments_currency_symbols"][$currency];
		print("<div id='tm-checkout-pricing-message'>The organizers have set a minimum price of " . $currency_symbol . $lowest_price . ".</div>");
		return;	
	}

	// check more than $1
	$to_usd = floatval(1.0 / $conf["tm_payments_currencies_fx"][$currency]);
	$usd_price = $pricing["display_amount"] * $to_usd;
	if (($usd_price > 0) and ($usd_price < 1)) {
		print("<div id='tm-checkout-pricing-message'>Please choose a payment of $1 USD or more.</div>");
		return;
	}

	// check maximum price
	// configure in $conf["tm_payments_max_price_usd"]
	if ($usd_price > $conf["tm_payments_max_price_usd"]) {
		print("<div id='tm-checkout-pricing-message'>Please choose a payment of $" . $conf["tm_payments_max_price_usd"]  . " USD or less.</div>");
		return;
	}

	// if free, show registration button instead
	if ($pricing["stripe_amount"] == 0) {

		// check we haven't already registered
		$rsvpflag = flag_get_flag('event_register');
		$registered_for_event = ($rsvpflag && $rsvpflag->is_flagged($event->nid, $account->uid));

		if (($registered_for_event) and ($partner_code != "PREVIEW")) {
			print("<div id='tm-checkout-pricing-message'>Please enter a payment of $1 USD or more.</div>");
			return;
		}

		// load template
		$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_event_button.tpl.php');

		// generate registration button
		$link_type = $rsvpflag->get_link_type();
  		$flag_link = module_invoke($link_type['module'], 'flag_link', $rsvpflag, "flag", $event->nid);
  		$register_link = $flag_link['href'] . "?destination=node/" . $event->nid . "&token=" . $flag_link['query']['token'];

		$checkout_button = "<a href='/" . $register_link . "'><button id='checkout-button' class='btn btn-success btn-lg'>Register</button></a>";
		$pricing_label = $conf["tm_payments_free_ticket_label"];

		// generate html
		$html = str_replace("__CHECKOUT_BUTTON__", $checkout_button, $template);
		$html = str_replace("__PRICING_LABEL__", $pricing_label, $html);
		$html = str_replace("__PARTNER_CODE_LABEL__", "", $html);
		$html = str_replace("__ERROR_MESSAGE__", "", $html);

		print($html);
		return;
	}
	
	// pricing label
	$pricing_label = $pricing["pricing_label"];

	// chapter code
	$chapter_code = "";
	if (isset($event->field_chapter[LANGUAGE_NONE][0])) {
		$event_chapter_nid = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
		$event_chapter = node_load($event_chapter_nid);
		if ($event_chapter != null) {
			if (isset($event_chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['safe_value'])) {
				$chapter_code = $event_chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['safe_value'];
			}
		}
	}

	// set partner code
	$partner_code = $pricing["partner_code"];
	if ($parter_code == "PREVIEW") {
		$partner_code = "";
	}

	// create meta data
	$metadata = array();
	$metadata['tm_processor'] = "tm_checkout";
	$metadata['checkout_action'] = "event_payment";
	$metadata['checkout_item'] = "ticket_event_id: " . $event->nid;
	$metadata['payment_email'] = strip_tags($account->mail);
	$metadata['stripe_amount'] = $pricing["stripe_amount"];
	$metadata['display_amount'] = $pricing["display_amount"];
	$metadata['entity_id'] = $event->nid;
	$metadata['entity_type'] = "event";
	$metadata['entity_title'] = strip_tags($event->title);
	$metadata['user_id'] = $account->uid;
	$metadata['user_email'] = strip_tags($account->mail);
	$metadata['user_first_name'] = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$metadata['user_last_name'] = strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$metadata['log_user_ip'] = tm_geoip_get_real_visitor_ip();
	$metadata['log_user_http_user_agent'] = "none";
	if (isset($_SERVER['HTTP_USER_AGENT'])) {
		$metadata['log_user_http_user_agent'] = $_SERVER['HTTP_USER_AGENT'];
	}
	$metadata["referer_code"] = $chapter_code;
	$metadata["partner_code"] = $partner_code;
	$metadata["partner_commission"] = tm_checkout_get_partner_commission($pricing["partner_code"], "event_payment");

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
		$success_url = $site_url . "/checkout/event-landing/" . $event_id . "?session_id={CHECKOUT_SESSION_ID}&uid=" . $account->uid;
		$cancel_url = $site_url . "/checkout/event/" . $event_id;
		if ($user_processing->uid != $account->uid) {
			$cancel_url = $site_url . "/checkout/event/" . $event_id . "?uid=" . $account->uid;
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
	$template = file_get_contents(drupal_get_path('module', 'tm_checkout') . '/templates/tm_checkout_event_button.tpl.php');

	// generate html
	$html = str_replace("__CHECKOUT_BUTTON__", $checkout_button, $template);
	$html = str_replace("__PRICING_LABEL__", $pricing_label, $html);
	$html = str_replace("__ERROR_MESSAGE__", $error_message, $html);

	// output
	print $html;
	return;
}

/*
 * Render pricing options
 */
function tm_checkout_event_render_pricing_options($event, $account, $currency, $partner_code) {

	global $conf;

	// get pricing and subscription
	// 0,5,10,CHOOSE

	if (!isset($event->field_event_payment_tiers[LANGUAGE_NONE][0]['value'])) {
		return "<div class='tm-checkout-pricing-options-container'>There are no payment options, please check later.</div>";
	}

	if (trim($event->field_event_payment_tiers[LANGUAGE_NONE][0]['value']) == "") {
		return "<div class='tm-checkout-pricing-options-container'>There are no payment options, please check later.</div>";
	}

	$parts = explode(",", $event->field_event_payment_tiers[LANGUAGE_NONE][0]['value']);
	
	$event_currency = "usd";
	if ($event->field_event_payment_currency[LANGUAGE_NONE][0]['value'] != null) {
		$event_currency = $event->field_event_payment_currency[LANGUAGE_NONE][0]['value'];
	}
	$currency_prefix = $conf["tm_payments_currencies"][$currency];
	$currency_symbol = $conf["tm_payments_currency_symbols"][$currency];

	// default no discount
	$partner_discount = 1;

	// apply subscriber discount
	$subscriber_discount_pct = _tm_payments_get_event_subscriber_discount($event, $account);
	if ($subscriber_discount_pct >= 0) {
		$partner_discount = (100 - $subscriber_discount_pct) / 100;
	}

	// override with partner discount
	$partner_discount_pct = _tm_payments_get_event_partner_code_discount($event, $partner_code);
	if ($partner_discount_pct >= 0) {
		$partner_discount = (100 - $partner_discount_pct) / 100;
	}

	// if lowest price is zero, don't appy any discounts
	$lowest_price = _tm_payments_get_lowest_price($event, $account, $currency);
	if ($lowest_price == 0) {
		$partner_discount = 1;
	}

	// free
	if (($partner_discount == 0) or ($subscriber_discount_pct == 100)) {
		return "<br>Hey there, rock star! 🎸";
	}

	// default is pay
	// if user is already registered, show donate
	$pay_or_donate = "Select the price you wish to pay";
	if (tm_events_user_is_registered($event->nid, $account->uid)) {
		$pay_or_donate = "Select the amount you wish to donate";
	}
	$html = "<div class='tm-checkout-pricing-options-container'>";
	$html .= "<div>" . $pay_or_donate . "</div>";
	$html .= "<ul>";

	$count = 1;
	$suggested_price = 0;
	$displayed_free = false;
	$displayed_chosen_price = false;
	$show_prices = array(); // keep track of prices we've shown

	// if user already registered, don't display the free option
	if (tm_events_user_is_registered($event->nid, $account->uid)) {
		if ($partner_code != "PREVIEW") {
			$displayed_free = true;
		}
	}

	foreach ($parts as $part) {

		// get part
		$part = trim($part);
		if ($part == "") {
			continue;
		}

		// labels
		$extra_label = "";
		if (strpos($part, "|") !== false) {
			$extra_parts = explode("|", $part);
			$part = trim($extra_parts[0]);
			$extra_label = "<span class='tm-checkout-label-extra'>" . trim(strip_tags($extra_parts[1])) . "</span>";
		}

		// check numeric
		if ((!is_numeric($part) and (strtoupper($part) != "FREE") and (strtoupper($part) != "CHOOSE"))) {
			continue;
		}

		// popular *
		$popular_label = "";
		$autofocus = "";
		if (strpos($part, "*") !== false) {
			$autofocus = "checked";
			$popular_label = "<span class='tm-checkout-label-popular'></span>";
		}
	
		// get price amount
		$price_amount = abs(intval(preg_replace("/[^0-9.]/", "", $part)));

		// apply discount
		$price_amount_before_discount = $price_amount;
		$price_amount = floor($price_amount * $partner_discount);

		// default price label 
		$price_label = $currency_symbol . $price_amount;

		// FREE
		if (strtoupper($part) == "FREE") { 
			$price_amount = 0;
		}

		// FX
		if (($part != "CHOOSE") and ($event_currency != $currency)) {
			
			// calculate conversion
			$to_usd = floatval(1.0 / $conf["tm_payments_currencies_fx"][$event_currency]);
			$from_usd = floatval($conf["tm_payments_currencies_fx"][$currency]);

			// convert fx and round
			$price_amount = ceil($price_amount * $to_usd * $from_usd);
			$price_label = $currency_symbol . $price_amount;
		}

		// check we haven't displayed this price already
		if ((isset($show_prices["price_" . $price_amount])) and ($part != "CHOOSE")) {
			continue;
		}

		// 0
		if ($price_amount == 0 and  (strtoupper($part) != "CHOOSE")) {
			if ($displayed_free) {
				continue;
			}
			$price_label = "Free";
			$displayed_free = true;
		}


		// Choose
		if (strtoupper($part) == "CHOOSE") {
			if ($displayed_chosen_price) {
				continue;
			}
			$price_label = "Choose your own price";
			$min_price = _tm_payments_get_lowest_price($event, $account, $currency, $partner_code);
			$max_price = intval($conf["tm_payments_max_price_usd"] * $conf["tm_payments_currencies_fx"][$currency]);
			$price_label .= " &mdash; " . $currency_symbol . " <input id='checkout_choose_own_price' type='number' style='width: 100px;' min='" . $min_price . "' max='" . $max_price ."'>";
			$displayed_chosen_price = true;
		}
		

		$html .= "<li>";

		$radio_id = "checkout_payment_option_" . $count;
		if (strtoupper($part) == "CHOOSE") {
			$radio_id = "checkout_payment_option_choose";
			$price_amount == 'CHOOSE';
		}

		$html .= "<input type='radio' id='" . $radio_id . "' name='checkout_payment_option' data-price='" . $price_amount . "' data-currency='" . $currency . "' value='" . $price_amount . "' " . $autofocus . ">";
		$html .= " <label for='" . $radio_id . "'>" . $price_label . $extra_label . $popular_label . "</label>";
		$html .= "</li>";

		$show_prices["price_" . $price_amount] = true;
		$count++;

	}

	$html .= "</ul>";
	$html .= "</div>";
	return $html;

}

/* 
 * Render partner message
 */
function tm_checkout_event_render_partner_message($event, $account, $currency, $partner_code) {

	global $conf;

	// currency arg
	$currency_arg = "";
	if (isset($_GET["currency"])) {
		$currency_arg = "&currency=" . strtolower($_GET["currency"]);
	}

	// if preview and registered
	if ($partner_code == "PREVIEW") {
		if (tm_events_user_is_registered($event->nid, $account->uid)) {
			$preview_message = "This is a <strong>preview</strong> of your ticket page";
			return "<div class='tm-checkout-pricing-message'>" . $preview_message . " &mdash; view <a class='tm-checkout-remove-partner-code-link' href='/checkout/event/" . $event->nid . "?partner=" . $currency_arg . "'>donation page</a></div>";
		}
	}

	// get payment type
	$payment_type = _tm_payments_get_payment_type($event);

	// subscriber discount
	$subscriber_discount = 0;
	if (module_exists("tm_subscriptions_user")) {

		if (isset($event->field_event_payment_sub_discount[LANGUAGE_NONE][0]['value'])) {
			$subscriber_discount = abs(intval($event->field_event_payment_sub_discount[LANGUAGE_NONE][0]['value']));
			if ($subscriber_discount > 100) {
				$subscriber_discount = 0;
			}	
		}
	}

	// if lowest value is zero, and you can choose your own price, don't show message
	$lowest_price = _tm_payments_get_lowest_price($event, $account, $currency);
	if (($lowest_price == 0) and ($payment_type == "pay_what_you_wish")) {
		if (isset($event->field_event_payment_tiers[LANGUAGE_NONE][0]['value'])) {
			if (strpos($event->field_event_payment_tiers[LANGUAGE_NONE][0]['value'], "CHOOSE") !== false) {
				if ($subscriber_discount != 100) {
					return "";
				}
			}
		}
	}

	// default no message
	$message = "";

	// check if event has any partner codes
	$event_partner_codes = _tm_payments_get_event_partner_codes($event);

	// apply partner code js
	$apply_partner_code_js = "javascript:tm_checkout_apply_partner_code(" . $event->nid . ",\"" . $currency  . "\");";

	// check for user subscriber discount
	if (module_exists("tm_subscriptions_user")) {

		// If subscriber discount
		if ($subscriber_discount > 0) {
			$check_chapter_leader = in_array("chapter leader", $account->roles);
			if ((tm_subscriptions_is_user_subscription_enabled($account->uid)) or $check_chapter_leader) {
				$message = "🎉 You get " . $subscriber_discount . "% off with <a href='/" . $conf["tm_checkout_subscription_public_page"] . "'>" . $conf["tm_subscriptions_user_label"] . "</a>";
			} else {
				$message = "⚡️ Get " . $subscriber_discount . "% off with <a href='/" . $conf["tm_checkout_subscription_public_page"] . "'>" . $conf["tm_subscriptions_user_label"] . "</a>";
			}

			// partner codes
			if (sizeof($event_partner_codes) > 0) {
				$message .= " &mdash; or apply a <a class='tm-checkout-apply-partner-code-link' href='" . $apply_partner_code_js . "'>partner code</a>";
			}
		}
	}

	// apply partner code
	if (($message == "") and (sizeof($event_partner_codes) > 0)) {
		$message .= "Apply a <a class='tm-checkout-apply-partner-code-link' href='" . $apply_partner_code_js . "'>partner code</a>";
	}

	// check for partner code
	$partner_code_discount = _tm_payments_get_event_partner_code_discount($event, $partner_code);
	if (($partner_code_discount >= 0) and ($partner_code != "PREVIEW")) {
		$message = "🎉 You get " . $partner_code_discount . "% off";
		$message .= " &mdash; <a class='tm-checkout-remove-partner-code-link' href='/checkout/event/" . $event->nid . "?partner=" . $currency_arg . "'>remove</a>";
	}

	// PREVIEW partner code
	if (($partner_code == "PREVIEW") and ($subscriber_discount > 0)) {
		$message = "This is a <strong>preview</strong> of pricing without discounts &mdash; view <a class='tm-checkout-remove-partner-code-link' href='/checkout/event/" . $event->nid . "?partner=" . $currency_arg . "'>live page</a>";
	}

	// no message
	if ($message == "") {
		return "";
	}

	// render message
	$html = "<div class='tm-checkout-pricing-message'>";
	$html .= $message;
	$html .= "</div>";

	return $html;
} 

/*
 * Helper method to get and set partner code
 */
function tm_checkout_event_get_partner_code($event) {

	global $conf;

	// if ?partner=PREVIEW return but skip cookie
	if (isset($_GET["partner"])) {
		if (strtoupper(trim($_GET["partner"])) == "PREVIEW") {
			return "PREVIEW";
		}
	}

	// check ?partner
	if (isset($_GET["partner"])) {

		// clear any existing partner codes
		setcookie("Drupal.visitor.event_partner_code", "", time() - 3600, "/", "." . $conf["tm_email_server_url_domain"]);

		// check partner code
		$partner_code = strtoupper(trim($_GET["partner"]));
		if ($partner_code != "") {
			if (_tm_payments_get_event_partner_code_discount($event, $partner_code) >= 0) {
				setcookie("Drupal.visitor.event_partner_code", $event->nid . "_" . $partner_code, strtotime('+30 days'), "/", "." . $conf["tm_email_server_url_domain"]);
				return $partner_code;
			}
		}

		return "";
	}

	// check cookie
	if (isset($_COOKIE['Drupal_visitor_event_partner_code'])) {

		// check cookie
		// must match event
		// ie: 1234_code
		$parts = explode("_", $_COOKIE['Drupal_visitor_event_partner_code'], 2);
		$partner_event_id = $parts[0];
		$partner_code = "";
		if (isset($parts[1])) {
			$partner_code = $parts[1];
		}
		
		// Check event_id
		if ($partner_event_id == $event->nid) {
			if (_tm_payments_get_event_partner_code_discount($event, $partner_code) >= 0) {
				return $partner_code;
			}
		}
	}

	// no partner code
	return "";

}

/*
 * Process payment
 */
function tm_checkout_event_process($stripe_session) {

	global $conf;
	global $user;

	// STEP 1. Fetch Stripe metadata
	$metadata = $stripe_session->metadata;

	// STEP 2. Fetch Event
	$entity_id = $metadata['entity_id'];
	$entity_type = $metadata['entity_type'];
	if ($entity_type != "event") {
		echo "{'success': false, 'message': 'Invalid entity_type'}";
		return;
	}
	$event = node_load($entity_id);
	if ($event == null) {
		echo "{'success': false, 'message': 'Invalid entity_id'}";
		return;
	}
	$account = user_load($metadata['user_id']);
	if ($account == null) {
		echo "{'success': false, 'message': 'Invalid user_id'}";
		return;
	}
	
	// STEP 3. Set payment vars
	$payment_id = $stripe_session->payment_intent;
	$checkout_action = $metadata['checkout_action'];
	$checkout_item = $metadata['checkout_item'];
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

	// remove PREVIEW
	if ($partner_code == "PREVIEW") {
		$partner_code = "";
	}

  	// STEP 4. Log payment
	tm_checkout_log_payment(null, $payment_id, $checkout_action, $checkout_item, $payment_email, $stripe_currency, $stripe_amount, $display_amount, $entity_id, $entity_type, $entity_title, $user_id, $user_email, $user_first_name, $user_last_name, $log_user_ip, $log_user_http_user_agent, $refunded, $paid_out, $referer_code, $partner_code, $partner_commission);

	// STEP 5. Register to event
	
	// Set pricing into static variable so we can pick it up in the notifications hook
	$hook_static_var_payment_text = &drupal_static("tm_payments_payment_text");
	$currency_symbol = "";
	if (isset($conf["tm_payments_currencies"][$stripe_currency])) {
		$currency_symbol = $conf["tm_payments_currencies"][$stripe_currency];
	}
	$hook_static_var_payment_text = $currency_symbol . $display_amount;

	// check if user already registered
	$already_registered = tm_events_user_is_registered($event->nid, $account->uid);

	// Get event flags
	$rsvpflag = flag_get_flag('event_register', NULL);
	$paidflag = flag_get_flag('event_paid', NULL);
	$waitflag = flag_get_flag('event_waitlist', NULL);

	// Add paid flag
	$paidflag->flag('flag', $event->nid, $account, TRUE);

	// Add to rsvp first, unflag waitlist second
	// this will trigger a single rsvp notification
	// (note: we don't expect people to waitlisted here)
	$rsvpflag->flag('flag', $event->nid, $account, TRUE);
	$waitflag->flag('unflag', $event->nid, $account, TRUE);

	// If already registered, send a payment notification to event organizers
	// This is because the event_register notification was already sent when the user registered
	if ($already_registered) {
		tm_checkout_event_notify_organizers_of_payment($account, $event, $hook_static_var_payment_text);
	}
	
	// Clear payments total cache for _tm_payments_show_total()
	cache_clear_all('tm-payments-show-total-' . $event->nid . '-', 'cache', TRUE);

	echo "{'success': true, 'message': 'Payment processed'}";
	return;
}

/*
 * Send a payment notification to event organizers
 */
function tm_checkout_event_notify_organizers_of_payment($account, $event, $event_payment_text) {

	// get event url
	$event_url = drupal_get_path_alias('node/' . $event->nid);

	// get user and profile url of who flagged the event
	$profile_url = drupal_get_path_alias("user/" . $account->uid);

	// get profile full name
	$profile_full_name = tm_users_get_full_name($account->uid);

	// Default text
	$register_action_headline = "A member purchased a ticket.";
	$register_action_subject = "purchased a ticket to";
	$register_action_body = "just purchased a ticket to your event";
	$register_payment_note = "<strong>Payment received:</strong> " . $event_payment_text . " (excludes processing fees)<br><br>";
	$registered_by_message = "";

	// generate member html
	$member_widget = tm_notifications_render_member_html($account);

	// get list of uids to notify
	$notify_ids = tm_notifications_events_get_organizer_notify_ids($event);

	// send notifications to event organizers
	foreach ($notify_ids as $notify_id) {

		// load the user to notify
		$notify_user = user_load($notify_id);

		// events_register_flag
		$hero_headline = tm_notifications_render_hero_headline_html($register_action_headline);
		tm_notifications_notify('events_register_flag', $notify_user, array('node' => $event, 'user' => $notify_user, '__PROFILE_URL__' => $profile_url, '__PROFILE_UID__' => $account->uid, '__EVENT_URL__' => $event_url, '__REGISTER_ACTION_SUBJECT__' => $register_action_subject, '__REGISTER_ACTION_BODY__' => $register_action_body, '__REGISTER_PAYMENT_NOTE__' => $register_payment_note, '__PROFILE_FULL_NAME__' => $profile_full_name, '__REGISTERED_BY_MESSAGE__' => $registered_by_message, '__HERO_HEADLINE__' => $hero_headline, '__MEMBER_WIDGET__' => $member_widget));
	}

}

<?php

// tm_user_subscriptions.display.inc - display methods

/**
 * Implements template_preprocess_page()
 */
function tm_subscriptions_user_preprocess_page(&$variables) {

	global $conf;
	global $user;

	// Show pro member CTA
	if (current_path() == "subscribers") {
		if (tm_subscriptions_is_user_subscription_enabled($user->uid)) {
			$message = $conf["tm_subscriptions_user_subscribers_page_message_subscribed"];
		} else {
			$message = $conf["tm_subscriptions_user_subscribers_page_message"];
		}
		drupal_set_message($message, "help-tip");
	}
}

/**
 * Implement views_post_execute
 */
function tm_subscriptions_user_views_post_execute(&$view) {

	global $conf;

	if ($view->name == 'subscribed_members') {
		if (isset($conf["tm_subscriptions_user_subscribers_page_title"])) {
			$view->build_info['title'] = $conf["tm_subscriptions_user_subscribers_page_title"];
		}
	}
}

/**
 * Implement hook form alter
 */
function tm_subscriptions_user_form_alter(&$form, $form_state, $form_id) {

  global $conf;

  // Alter subscribers view dropdowns
  if (isset($form_state['view'])) {

    if ($form_state['view']->name == 'subscribed_members') {

      // Limit countries dropdown to users countries
      $output = '';
      $output = cache_get('views-exposed-form-subscribers-countries', 'cache');
      if (empty($output)) {
        $approved_member_role = user_role_load_by_name("subscriber")->rid;
        $result = db_query("SELECT * FROM (SELECT DISTINCT iso2, name, entity_id from {field_data_field_user_country} f LEFT JOIN {countries_country} c ON f.field_user_country_iso2 = c.iso2 WHERE f.bundle = 'user' AND c.iso2 IS NOT NULL) as countries_with_users INNER JOIN {users_roles} u ON u.uid = countries_with_users.entity_id AND u.rid = " . intval($approved_member_role) . " ORDER BY name ASC");
        $defaultoptions = array('All'=>'- Any -');
        $output = $defaultoptions + $result->fetchAllKeyed();
        $form['name_list']['#options'] = $output;
      cache_set('views-exposed-form-subscribers-countries', $output, 'cache', time() + 3600);
      } else{
        $form['name_list']['#options'] = $output->data;
      }

      // Replace "All" option to more meaningful name
      $form['name_list']['#options']['All'] = "Country";

    }
  }
}


/**
 * Impement Hook node view
 * Expire pro account if it has expired and manager of account views it
 */
function tm_subscriptions_user_user_view($account, $view_mode, $langcode) {

	global $user;

	if (!user_is_logged_in()) {
		return;
	}

	if ($view_mode != "full") {
		return;
	}

	// check if user is viewing their own account
	if ($user->uid != $account->uid) {
		return false;
	}

	// remove subscription if it has expired
	// this will happen if a manager of the organization views their own profile
	if (tm_subscriptions_user_has_subscription_expired($account)) {

		global $conf;

		$flag = flag_get_flag('user_subscription_enable');

		if ($flag->is_flagged($account->uid)) {

			$flag->flag('unflag', $account->uid, null, true);

			// send expiry message
			tm_subscriptions_user_send_notification($account, "expired");

			drupal_set_message($conf["tm_subscriptions_user_expired_message"], "help-tip");
			drupal_goto("user/" . $user->uid);
		}
	}

}

/**
 * Render subscription CTA banner for nodes
 * Called from page--node.tpl.php
 */
function tm_subscriptions_user_cta_banner_node($user_id, $node_id) {

	global $conf;
	$node = node_load($node_id);

	if ($node == null) {
		return;
	}

	// show for chapters
	if ($node->type == "chapter") {
		return tm_subscriptions_user_cta_banner($user_id);
	}
}

/**
 * Render subscription CTA banner
 * Called from page--user.tpl.php
 */
function tm_subscriptions_user_cta_banner($user_id) {

	global $conf;
	global $user;

	$account = user_load($user_id);
	if ($account == null) {
		return;
	}

	// don't show if /user path shown
	// example: /user/123/edit
	if (arg(2) != null) {
		return;
	}

	// check if we can show cta to user
	if (!tm_subscriptions_check_show_user_cta($account->uid, $user->uid)) {
		return;
	}

	// get country pricing and subscription
	$country_subscription = tm_subscriptions_get_user_subscription($account);

	// retrive static variable from tm_subscriptions_user_get_partner_code if ?partner code 
	$tm_force_show_cta = &drupal_static("tm_force_show_cta");
	
	// if ?preview_subscription_cta is set
	if (isset($_GET["preview_subscription_cta"])) {
		$tm_force_show_cta = true;
	}

	// get cookie
	$wrapper_style = "";
	if ((!$tm_force_show_cta) and (isset($_COOKIE["tm_subscriptions_hide_user_cta"]))) {
		if ($_COOKIE["tm_subscriptions_hide_user_cta"] == 1) {
			$wrapper_style = "display: none";
		}
	}

	$wrapper = "<div id='tm-subscriptions-cta-banner' style='" . $wrapper_style . "' class='messages--page_notice messages page_notice'>__CONTENT__</div>";

	// show subscription html
	$inner_html = $country_subscription["html"];

	// show renewal html if in renewal period
	if (tm_subscriptions_check_user_renewal_period($account)) {
		$inner_html = $country_subscription["html_renew"];
	}

	// if account can be renewed and ?preview_subscription_cta
	if ((tm_subscriptions_user_days_until_expiry($account) > 0) and (isset($_GET["preview_subscription_cta"]))) {
		$inner_html = $country_subscription["html_renew"];
	}

	// include template fields
	$subscription_date = tm_subscriptions_user_get_subscription_date($account);
	$subscription_date_text = date('Y-m-d', strtotime($subscription_date));
	$expires_text = tm_subscriptions_user_get_subscription_expires_text($account);
	if ((strpos($expires_text, "mins")) or (strpos($expires_text, "hours"))) {
		$expires_text = "Today";
	}
	$inner_html = str_replace("__SUBSCRIPTION_EXPIRES_DATE__", $subscription_date_text, $inner_html); // subscription expiry date
	$inner_html = str_replace("__SUBSCRIPTION_EXPIRES_TEXT__", $expires_text, $inner_html); // days until subscription expiry

	// add number of members to text
	$query = "SELECT COUNT(*) amount FROM {users} u";
	$num_members = db_query($query)->fetch();
	$num_members_amount = $num_members->amount;
	if ($num_members_amount > 1000) {
		$num_members_amount = number_format(round(floor($num_members_amount / 1000)) * 1000);
	}
	$inner_html = str_replace("__NUM_MEMBERS_ROUNDED__", $num_members_amount, $inner_html);

	// add number of subscribed members to text
	$num_subscribed_members_amount = flag_get_flag_counts("user_subscription_enable");
	$inner_html = str_replace("__NUM_SUBSCRIBED_MEMBERS__", $num_subscribed_members_amount, $inner_html);

	// get pricing label
	$pricing_label = tm_subscriptions_get_user_pricing_label($account);
	$inner_html = str_replace("__PRICING_LABEL__", $pricing_label, $inner_html);

	// get pricing amount
	$pricing_amount = tm_subscriptions_get_user_pricing_amount($account);
	$inner_html = str_replace("__PRICING_AMOUNT__", $pricing_amount, $inner_html);

	// display pricing
	$payment_display = "none";
	if (!isset($_GET["preview_subscription_cta"])) {
		if (isset($_COOKIE["tm_subscriptions_show_user_payment"])) {
			if ($_COOKIE["tm_subscriptions_show_user_payment"] == 1) {
				$payment_display = "block";
			}
		}
	}
	$inner_html = str_replace("__PAYMENT_DISPLAY__", $payment_display, $inner_html);

	// insert into inner html
	$html = str_replace("__CONTENT__", $inner_html, $wrapper);

	return $html;
}

/**
 * Render subscription CTA display suite
 */
function _tm_subscriptions_user_cta_ds($field, $title = NULL) {

	global $conf;
	global $user;

	$user_id = $field['entity']->uid;
	$account = $field['entity'];

	// check if we show cta
	$html = "";
	if (tm_subscriptions_check_show_user_cta($user_id, $user->uid)) {

		// set partner code
		tm_subscriptions_user_set_partner_cookie($user_id);

		// check if we are in subscription renewal period
		if (tm_subscriptions_check_user_renewal_period($account)) {
			return "";
		}

		// Show call to action
		$html = $conf["tm_subscriptions_user_cta_text"];

		// Add learn more
		$html .= " &middot; <a class='show-cta-banner-user' target='_blank' href='" . tm_subscriptions_get_user_subscription_field($account, "learn_more_url") . "'>Learn more</a>";
	}

	return $html;
}

/**
 * Render subscription CTA display suite
 */
function _tm_subscriptions_user_badge_ds($field, $title = NULL) {

	global $conf;
	global $user;

	$user_id = $field['entity']->uid;
	$account = $field['entity'];

	$html = "";
	if (tm_subscriptions_is_user_subscription_enabled($user_id)) {
		$html = $conf["tm_subscriptions_user_label_noun"];

		// show extra information to other users
		// check if own account
		if ($user->uid == $user_id) {

			// show renewal
			if (tm_subscriptions_check_user_renewal_period($account)) {
				$html .= " (Expires in " . tm_subscriptions_user_get_subscription_expires_text($account) . ")";
			}

			// show support link
			$html .= " &middot; <a class='show-cta-banner-user' target='_blank' href='" . tm_subscriptions_get_user_subscription_field($account, "support_url") . "'>Customer Support</a>";

		} else {

			// show learn more link
			$html .= " &middot; <a class='show-cta-banner-user' target='_blank' href='" . tm_subscriptions_get_user_subscription_field($account, "learn_more_url") . "'>Learn more</a>";

		}
	}

	return $html;
}

/**
 * Create CTA Stipe Payment JS
 */
function tm_subscriptions_user_cta_js($user_id) {

	global $conf;
	global $user;

	// validate user
	$account = user_load($user_id);
	if ($account == null) {
		return;
	}

	// STEP 1. GET PRICING
	$pricing = tm_subscriptions_get_user_subscription_price($account);
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
	$entity_id = $user_id;
	$user_id = $user->uid;
	$partner_code = tm_subscriptions_user_get_partner_code();

	$tm_subscriptions_stripe_logo = $conf["tm_subscriptions_stripe_logo"];

	$tm_subscriptions_handler_name = $conf["tm_subscriptions_handler_name"]; // ie: "Travel Massive";
	$tm_subscriptions_handler_description = tm_subscriptions_get_user_subscription_field($account, "stripe_description"); // ie: "Pro Subscription";

	// Enable billing address
	$enable_billing_address = "false";
	if ($conf["tm_subscriptions_user_enable_billing_address"]) {
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
			$('#process-stripe-payment').attr("action","/payment/subscription-user/{$entity_id}/{$user_id}?partner={$partner_code}");
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
	
	// Hide the CTA banner
	$('.hide-cta-banner-user').on('click', function(e) {
		$("#tm-subscriptions-cta-banner").hide();
		var cookie = ['tm_subscriptions_hide_user_cta=1; domain=.', window.location.host.toString(), '; path=/;'].join('');
		document.cookie = cookie;
	});

	// Show the CTA banner
    $('.show-cta-banner-user').on('click', function(e) {
		$("#tm-subscriptions-cta-banner").show();
		var cookie = ['tm_subscriptions_hide_user_cta=0; domain=.', window.location.host.toString(), '; path=/;'].join('');
		document.cookie = cookie;
	});

	// Show the payment button
    $('.show-subscriptions-payment').on('click', function(e) {
    	$("#tm-subscriptions-payment").show();
    	var cookie = ['tm_subscriptions_show_user_payment=1; domain=.', window.location.host.toString(), '; path=/;'].join('');
    	document.cookie = cookie;
    });

});

EOT;

	// STEP 3. MODIFY BUTTON AND INCLUDE STRIPE JS
	drupal_add_js('https://checkout.stripe.com/checkout.js', 'external'); // add stripe library
	drupal_add_js($stripe_js, 'inline'); // add inline js
	return true;

}

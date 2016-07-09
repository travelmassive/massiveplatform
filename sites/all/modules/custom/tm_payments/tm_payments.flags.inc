<?php

// tm_payments.flags.inc - flag methods

/**
 * Implementation hook tm_payments_preprocess_flag
 */
function tm_payments_preprocess_flag(&$variables) {

	$flag = &$variables['flag'];
	$action = $variables['action'];

	// Register flag
	if (($flag->name == "event_register") && ($action == "flag")) {
		return _tm_payments_preprocess_flag_register_flag($variables);
	}

	// Register unflag
	if (($flag->name == "event_register") && ($action == "unflag")) {
		return _tm_payments_preprocess_flag_register_unflag($variables);
	}

	// Waitlist flag
	if (($flag->name == "event_waitlist") && ($action == "flag")) {
		return _tm_payments_preprocess_flag_waitlist_flag($variables);
	}

}

/**
 * Modify event waitlist flag
 */
function _tm_payments_preprocess_flag_waitlist_flag(&$variables) {

	// Some typing shotcuts:
	$flag = &$variables['flag'];
	$action = $variables['action'];
	$entity_id = $variables['entity_id'];
	$flag_css_name = str_replace('_', '-', $flag->name);
	$uid = $variables['user']->uid;

	// check if payment is enabled
	$event = node_load($entity_id);
	if (!_tm_payments_is_payment_enabled($event)) {
		return;
	}

	// check if no seats left
	$num_seats_left = _tm_events_seats_left($event);
	if ($num_seats_left != 0) {
		return; // seats available or external rsvp
	}

	// change button text
	$variables['link_text'] = 'Sold out';

	// get attendance count
	$flag = flag_get_flag('event_register');
	$count = $flag->get_count($event->nid);

	// Extra stuff. Add non-registered guests to count
	$extra_guests_count = 0;
	if (isset($event->field_event_extra_guests[LANGUAGE_NONE][0]['value'])) {
		$extra_guests_count = $event->field_event_extra_guests[LANGUAGE_NONE][0]['value'];
		$count = $count + $extra_guests_count;
	}
	$count_text = $count . " people are going";
	if ($count == 1) {
		$count_text = "1 person is going";
	}

	// show warning when unregistering
	$event = node_load($entity_id);
    $variables['flag_classes_array'][] = 'disabled';
	$variables['link_href'] = '#';
    drupal_add_js('
    jQuery(document).ready(function($) {
      $( ".flag" ).unbind("click");
      $( ".flag" ).bind("click.confirm", function(event) {
        jq_alert("This event is sold out", "There are no more tickets available for this event.");
      });
      $( ".js-tm-seats-count.meta").html("' . $count_text . '<br>No tickets available");
    });
    ', 'inline');
    return true;
}

/**
 * Modify event register flag
 */
function _tm_payments_preprocess_flag_register_flag(&$variables) {

	global $conf;

	// Some typing shotcuts:
	$flag = &$variables['flag'];
	$action = $variables['action'];
	$entity_id = $variables['entity_id'];
	$flag_css_name = str_replace('_', '-', $flag->name);
	$uid = $variables['user']->uid;

	// Check if event is valid
  	$event = node_load($entity_id);
  	if ($event->type != "event") {
    	return;
  	}

	// STEP 1. CHECK IF PAYMENT IS ENABLED
	if (!_tm_payments_is_payment_enabled($event)) {
		return;
	}

	// STEP 2. CHECK IF APPROVED MEMBERS ONLY
	if ($event->field_approved_members_only[LANGUAGE_NONE][0]['value'] == 1) {
		$check_member = user_load($uid);
		if (!in_array('approved user', $check_member->roles)) {
			$variables['link_text'] = 'Buy Ticket';
			return;
		}
    }

	// STEP 3. GET PRICING
	$member = user_load($uid);
	$user_id = $member->uid;

	$pricing = _tm_payments_get_registration_price($event);
	if ($pricing == null) {
		// problem calculating pricing
		return;
	}

	// STEP 3. CREATE STRIPE JS
	$stripe_currency = $pricing["stripe_currency"];	
	$stripe_amount = $pricing["stripe_amount"];
	$user_email = $member->mail;
	$stripe_pub_key = $conf["tm_payments_stripe_publishable_key"];
	$stripe_bg_image = $conf["tm_payments_stripe_image_url"];

	$tm_payments_stripe_logo = $conf["tm_payments_stripe_logo"];

	$tm_payments_handler_name = $conf["tm_payments_handler_name"]; // ie: "Travel Massive";
	$tm_payments_handler_description = $conf["tm_payments_handler_description"]; // ie: "Event Registration";

	// Enable billing address
	$enable_billing_address = "false";
	if (isset($event->field_event_payment_ask_address[LANGUAGE_NONE][0]['value'])) {
		if ($event->field_event_payment_ask_address[LANGUAGE_NONE][0]['value'] == 1) {
			$enable_billing_address = "true";
		}
	}

	  $stripe_js = <<<EOT

jQuery(document).ready(function($) {
	
	$('.payment-button').unbind('click');

	var handler = StripeCheckout.configure({
		key: '{$stripe_pub_key}',
		image: '{$stripe_bg_image}',
		locale: 'auto',	
		token: function(token) {

			$('.payment-button').addClass('disabled');

			$('.payment-button').append('<form id="process-stripe-payment"></form>');
			$('#process-stripe-payment').attr("action","/payment/event/{$entity_id}/{$user_id}");
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
			name: '{$tm_payments_handler_name}',
			description: '{$tm_payments_handler_description}',
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
	$('.detail-item.price').after("<div class='detail-item tm-payment-gateway-logo'>{$tm_payments_stripe_logo}</div>");
	

});

EOT;

	// STEP 4. MODIFY BUTTON AND INCLUDE STRIPE JS
	$variables['flag_classes_array'][] = 'payment-button';
	$variables['link_href'] = '#';
	$variables['link_text'] = 'Buy Ticket';
	drupal_add_js('https://checkout.stripe.com/checkout.js', 'external'); // add stripe library
	drupal_add_js($stripe_js, 'inline'); // add inline js
	return true;

}


/**
 * Modify event register unflag
 */
function _tm_payments_preprocess_flag_register_unflag(&$variables) {

	// Some typing shotcuts:
	$flag = &$variables['flag'];
	$action = $variables['action'];
	$entity_id = $variables['entity_id'];
	$flag_css_name = str_replace('_', '-', $flag->name);
	$uid = $variables['user']->uid;

	// Check if event is valid
  	$event = node_load($entity_id);
  	if ($event->type != "event") {
    	return;
  	}

	// Check if payment is enabled
	if (!_tm_payments_is_payment_enabled($event)) {
		return;
	}

	// change button text if paid for
	$paid_flag = flag_get_flag('event_paid');
	$paid_for_event = ($paid_flag && $paid_flag->is_flagged($entity_id, $uid));

	if ($paid_for_event) {
		$variables['link_text'] = 'Purchased';
	}

	// show warning when unregistering
	$event = node_load($entity_id);
    $variables['flag_button_class'] = 'flag-limit follow bttn bttn-secondary bttn-m';
    drupal_add_js('
    jQuery(document).ready(function($) {
      $( ".flag" ).unbind("click");
      $( ".flag" ).bind("click.confirm", function(event) {
        jq_confirm_unregister_paid_event(\''.htmlentities($event->title, ENT_QUOTES | ENT_HTML5).'\');
        return false;
      });
    });
    ', 'inline');
    return true;

}

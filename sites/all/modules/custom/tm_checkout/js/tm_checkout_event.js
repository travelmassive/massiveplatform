(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// set ajax button url
	var tm_checkout_button_ajax_url = Drupal.settings.tm_checkout_event.checkout_button_ajax_url;
	var tm_checkout_button_ajax_url_reload = null;
	var tm_checkout_button_timout = null;
	var tm_checkout_button_timout_xhr = []; // keep track of xhr calls
	var tm_checkout_button_message = Drupal.settings.tm_checkout_event.checkout_button_message;

	// init
	// choose your own price
	// initialise checkout button if pre selected
	if (jQuery(".tm-checkout-pricing-options-container").length != 0) {
		if (jQuery(".tm-checkout-pricing-options-container input").length != 0) {
			if (jQuery("input[type=radio][name=checkout_payment_option]:checked").data("price") != null) {
				tm_checkout_option_selected();
			} else {
				jQuery("#tm-checkout-event-button-container").html("<p>" + tm_checkout_button_message + "</p>");
			}
		}
	} else {
		// fixed price
		tm_checkout_event_button_reload(tm_checkout_button_ajax_url);
	}

	// add event listener to checkout-button
	function tm_checkout_event_button_add_listener() {
		var checkoutButton = document.getElementById('checkout-button');
		if (checkoutButton == null) {
			return;
		}
		checkoutButton.addEventListener('click', function() {
			stripe.redirectToCheckout({
				// Make the id field from the Checkout Session creation API response
				// available to this file, so you can provide it as argument here
				// instead of the {{CHECKOUT_SESSION_ID}} placeholder.
			sessionId: document.getElementById('checkout-button').getAttribute('data-secret')
			}).then(function (result) {
				// If `redirectToCheckout` fails due to a browser or network
				// error, display the localized error message to your customer
				// using `result.error.message`.
			});
		});
	}

	// reload the event checkout button
	function tm_checkout_event_button_reload(load_button_url) {

		// show loading
		jQuery("#tm-checkout-event-button-container").html("<img style='margin-top: 32px;' src='/sites/all/themes/tm/images/load-more-ajax-loader-2.gif'>");
		
		// clear timeout
		if (tm_checkout_button_timout != null) {
			clearTimeout(tm_checkout_button_timout);
		}
		
		// cancel any previous xhr calls
		for  (i = 0; i < tm_checkout_button_timout_xhr.length; i++) {
			var old_xhr = tm_checkout_button_timout_xhr[i];
			old_xhr.abort();
		}

		// set timeout
		tm_checkout_button_timout = setTimeout(function(){ 
			jQuery.ajax({
				url: load_button_url, 
				beforeSend: function(xhr) {
					// add to xhr stack
					tm_checkout_button_timout_xhr.push(xhr);
				},
				success: function(result) {
					$("#tm-checkout-event-button-container").html(result);
					tm_checkout_event_button_add_listener();
				}
			}), 100
		});

	}

	// get selected item and load pricing
	function tm_checkout_option_selected() {

		var display_amount = jQuery("input[type=radio][name=checkout_payment_option]:checked").data("price");

		// don't fire if choose own price
		if (display_amount == 'CHOOSE') {
			return;
		}

		// reload checkout button
		var url = tm_checkout_button_ajax_url + "&price=" + display_amount;
		tm_checkout_event_button_reload(url);
	};

	// apply a partner code
	tm_checkout_apply_partner_code = function(event_id, currency_code) {
		jQuery.prompt({
			state0: {
				title: 'Got a partner code?',
				html: "<input type='text' id='tm-checkout-apply-partner-code' placeholder='Enter your partner or discount code' autocomplete='off'>",
				buttons: { Cancel: -1, Apply: true },
				focus: 1,
				submit:function(e,v,m,f){
					if (v == true) {
						window.location = '/checkout/event/' + event_id + '?currency=' + currency_code + '&partner=' + document.getElementById('tm-checkout-apply-partner-code').value; 
					} 
					$.prompt.close();
				}
			}
		});
	}

	// pay what you wish message
	tm_checkout_pay_what_you_wish_message = function() {
		var alert_title = Drupal.settings.tm_checkout_event.pay_what_you_wish_title;
		var message = Drupal.settings.tm_checkout_event.pay_what_you_wish_message;
		jQuery.prompt(message, {title: alert_title, buttons: { "Got it, thanks.": true}});
	}

	// listen to radio button change
	jQuery('input[type=radio][name=checkout_payment_option]').on('change', function() {
		tm_checkout_option_selected();
	});

	// listen to choose your own price
	jQuery('#checkout_choose_own_price').bind('keyup mouseup', function() {

		// select option
		jQuery("#checkout_payment_option_choose").prop("checked", true);

		// reload checkout button
		var display_amount = jQuery(this).val();
		if (display_amount == "") {
			return;
		}
		var url = tm_checkout_button_ajax_url + "&price=" + display_amount;
		tm_checkout_event_button_reload(url);		

	});

});})(jQuery, Drupal, this, this.document);
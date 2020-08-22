(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// set ajax button url
	var tm_checkout_button_ajax_url = Drupal.settings.tm_checkout_subscription.checkout_button_ajax_url;

	// preloading image
	jQuery("#tm-checkout-button-container").html("<img style='margin-top: 32px;' src='/sites/all/themes/tm/images/load-more-ajax-loader-2.gif'>");

	// checkout button
	jQuery("#tm-checkout-button-container").load(tm_checkout_button_ajax_url, 
		function() {
			tm_checkout_button_add_listener();
		}
	);

	// add event listener to checkout-button
	function tm_checkout_button_add_listener() {
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

	// apply a partner code
	tm_checkout_subscription_apply_partner_code = function() {
		jQuery.prompt({
			state0: {
				title: 'Got a partner code?',
				html: "<input type='text' id='tm-checkout-apply-partner-code' placeholder='Enter your partner or discount code' autocomplete='off'>",
				buttons: { Cancel: -1, Apply: true },
				focus: 1,
				submit:function(e,v,m,f){
					if (v == true) {
						window.location = '?partner=' + document.getElementById('tm-checkout-apply-partner-code').value; 
					} 
					$.prompt.close();
				}
			}
		});
	}

});})(jQuery, Drupal, this, this.document);
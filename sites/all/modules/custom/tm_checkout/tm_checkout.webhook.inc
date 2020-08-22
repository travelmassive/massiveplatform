<?php

// tm_checkout.webhook.inc

/**
 * Stripe webhook
 */
function tm_checkout_stripe_webhook() {

	global $conf;

	// check for stripe signature
	if (!isset($_SERVER['HTTP_STRIPE_SIGNATURE'])) {
		drupal_goto("/");
	}

	// init Stripe
	require_once(drupal_get_path('module', 'tm_checkout') . "/includes/" . $conf["tm_checkout_stripe_library_path"] . '/init.php');

	// See your keys here: https://dashboard.stripe.com/account/apikeys
	\Stripe\Stripe::setApiKey($conf["tm_checkout_stripe_secret_key"]);

	// Set API version
	\Stripe\Stripe::setApiVersion($conf["tm_checkout_stripe_api_version"]);

	// Set endpoint secret
	// You can find your endpoint's secret in your webhook settings
	$endpoint_secret = $conf["tm_checkout_stripe_endpoint_secret"];
	$payload = @file_get_contents('php://input');
	$sig_header = $_SERVER['HTTP_STRIPE_SIGNATURE'];
	$event = null;

	try {
		$event = \Stripe\Webhook::constructEvent(
			$payload, $sig_header, $endpoint_secret
		);
	} catch(\UnexpectedValueException $e) {
		// Invalid payload
		drupal_add_http_header('Status', '400');
		exit();
	} catch(\Stripe\Exception\SignatureVerificationException $e) {
		// Invalid signature
		// Silent fail as we have multple endpoints for checkout.session.completed
		drupal_add_http_header('Status', '202');
		exit();
	}

	// Handle the checkout.session.completed event
	if ($event->type == 'checkout.session.completed') {

		// get session meta data
		$session = $event->data->object;
		$metadata = $session->metadata;

		// check we haven't already processed this payment
		// (webhooks can be replayed)
		$existing_payment = tm_checkout_get_payment($session->payment_intent);
		if ($existing_payment != null) {
			drupal_add_http_header('Status', '203');
			echo "{'success': true, 'message': 'Payment ID already processed'}";
			drupal_exit();
		}

		// need tm_processor
		if (!isset($metadata["tm_processor"])) {
			drupal_add_http_header('Status', '400');
		}

		// check tm_processor is tm_checkout
		if ($metadata["tm_processor"] != "tm_checkout") {
			drupal_add_http_header('Status', '202');
		}

		// check for action
		if (isset($metadata["checkout_action"])) {
			
			// get checkout action
			$checkout_action = $metadata["checkout_action"];

			// upgrade membership
			if (($checkout_action) == "upgrade_subscription") {
				tm_checkout_subscription_process($session);
			}

			// renew subscription
			if (($checkout_action) == "renew_subscription") {
				tm_checkout_subscription_process($session);
			}

			// event payment
			if (($checkout_action) == "event_payment") {
				tm_checkout_event_process($session);
			}

		} else {
			drupal_add_http_header('Status', '400');
		}
	}

	// successful
	drupal_add_http_header('Status', '200');
	drupal_exit();

}

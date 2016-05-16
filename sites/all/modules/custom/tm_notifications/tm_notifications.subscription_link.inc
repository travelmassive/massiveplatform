<?php

// tm_notifications.unsubscribe.inc - handle unsubscribe links

/** 
 * Form for managing one-click subscriptions
 */
function tm_notifications_confirm_subscription_form($form, &$form_state) {

	global $conf;
	global $user;

	// STEP 1. URL VALIDATION

	// get action (subscribe or unsubscribe)
	$subscription_action = arg(2);
	if (($subscription_action != "subscribe") and ($subscription_action != "unsubscribe")) {
		drupal_set_message('Sorry, the invalid subscription action', 'warning');
		drupal_goto('/');
	}

	// get account 
	// /user/10237/unsubscribe/new_organization_follower/4aa470b5147d59f09601c4588646b572
	$account = user_load(arg(1));
	if ($account == null) {
		drupal_set_message('Sorry, the ' . $subscription_action . ' link is not valid', 'warning');
		drupal_goto('/');
	}

	// get notification and secret hash
	$notification_name = arg(3);
	$secret_hash = arg(4);

	// validate notificaton
	$notification = new TMNotification($notification_name);
	if ($notification == null) {
		drupal_set_message('Sorry, the ' . $subscription_action . ' link is not valid', 'warning');
		drupal_goto('/');
	}

	// validate unsubscribe hash
	$check_secret_hash = md5($conf["tm_notifications_subscription_secret_key"] . "_" . $account->uid  . "_" . $notification_name);
	if ($secret_hash != $check_secret_hash) {
		drupal_set_message('Sorry, the ' . $subscription_action . ' link is not valid', 'warning');
		drupal_goto('/');
	}

	// validate notificaton can be unset
	$notification = new TMNotification($notification_name);
	if (!$notification->isChangeAllowed()) {
		drupal_set_message('Sorry, the ' . $subscription_action . ' link is not valid', 'warning');
		drupal_goto('/');
	}

	// notification is not already set
	if ($subscription_action == "subscribe") {
		if ($notification->isEnabled($account->uid)) {
			drupal_set_message('You are already subscribed to this notification (' . $notification->getLabel() . ')');
			drupal_goto('/');
		}
	}
	if ($subscription_action == "unsubscribe") {
		if (!$notification->isEnabled($account->uid)) {
			drupal_set_message('You are already unsubscribed from this notification (' . $notification->getLabel() . ')');
			drupal_goto('/');
		}
	}
	
	// STEP 2. GENERATE FORM
	$form = array();
	$form['#method'] = 'get';

	// if form confirmed, handle submission
	if (isset($_GET["confirm"])) {

		if ($subscription_action == "subscribe") {

			// subscribe the user from the notification
			$notification->enable($account->uid);

			// display confirmation message
			$undo_link = l("Undo", "/user/" . $account->uid . "/unsubscribe/" . $notification_name . "/" . $secret_hash);
			drupal_set_message("You have successfully subscribed to '" . $notification->getLabel() . "' (" . $undo_link . ")");
			drupal_goto('/');
		}

		if ($subscription_action == "unsubscribe") {

			// unsubscribe the user from the notification
			$notification->disable($account->uid);

			// display confirmation message
			$undo_link = l("Undo", "/user/" . $account->uid . "/subscribe/" . $notification_name . "/" . $secret_hash);

			drupal_set_message("You are now unsubscribed from '" . $notification->getLabel() . "' (" . $undo_link . ")");
			drupal_goto('/');
		}

	} else {

		// subscribe confirm text
		if ($subscription_action == "subscribe") {
			$form['notes'] = array(
				'#markup' => "<p>Please confirm your subscription to the following notification:<br><br><strong>" . $notification->getLabel() . "</strong></p>"
			);
		}

		// subscribe confirm text
		if ($subscription_action == "unsubscribe") {
			$form['notes'] = array(
				'#markup' => "<p>Please confirm that you no longer want to receive the following notifications:<br><br><strong>" . $notification->getLabel() . "</strong></p>"
			);
		}

		// confirm field
		$form['confirm'] = array(
			'#type' =>'hidden',
			'#value' => true,
		);

		// submit button
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Yes, ' . $subscription_action),
			'#id' => 'edit-submit'
		);
	}

	// provide link to notification settings
	if ($user->uid == $account->uid) {
		$form['notification_settings'] = array(
			'#markup' => "<p>Or, manage your <a href='/user/" . $user->uid . "/edit#user-notifications-options'>notification settings</a>.</p>"
		);
	}

	// show logged out user
	if (!user_is_logged_in()) {
		$form['notification_settings'] = array(
			'#markup' => "<p>To manage all your notifications, please <a href='/user/login'>sign in</a> to your account.</p>"
		);
	}

	return $form;
}

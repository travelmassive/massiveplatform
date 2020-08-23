<?php

// tm_payments.edit.inc - form edit methods

/**
 * Form alter hook
 * Modify forms if payments are enbled
 */
function tm_payments_form_alter(&$form, &$form_state, $form_id) {

	// Chapter Payment Settings
	if ($form_id == "chapter_node_form") { 
		_tm_payments_form_alter_chapter_form($form, $form_state, $form_id);
	}

	// Company Payment Settings
	if ($form_id == "organization_node_form") { 
		_tm_payments_form_alter_company_form($form, $form_state, $form_id);
	}

}

/**
 * Modify chapter form
 */
function _tm_payments_form_alter_chapter_form(&$form, &$form_state, $form_id) {

	global $conf;
  	global $user;

	// Permission variables
	$perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));

	// Only allow moderators to change payment settings
	if ($perm_is_moderator_or_admin) {

		// Default text
		$default_text = "When " . $conf["tm_payments_enable_chapter_min_members"] . " members join this Chapter (Default)";
		$form['field_chapter_payments_enabled'][LANGUAGE_NONE]['#options']['default'] = $default_text;
		
		// Set Default text
		if (!isset($form['#entity']->field_chapter_payments_enabled[LANGUAGE_NONE])) {
			$form['field_chapter_payments_enabled'][LANGUAGE_NONE]['#options']['_none'] = $default_text;
			unset($form['field_chapter_payments_enabled'][LANGUAGE_NONE]['#options']['default']);
		} else {
			unset($form['field_chapter_payments_enabled'][LANGUAGE_NONE]['#options']['_none']);
		}
			
	} else {
		// Hide fields
		field_group_hide_field_groups($form, array('group_chapter_payments'));
		unset($form['field_chapter_payments_enabled']);
	}

	// Set placeholder (default) commission
	$default_commission = _tm_payments_get_default_commission("chapter_event");
	$form['field_chapter_payment_commission'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = ($default_commission + 0); // remove .00

}

/**
 * Modify company form
 */
function _tm_payments_form_alter_company_form(&$form, &$form_state, $form_id) {

	global $conf;
  global $user;

	// Permission variables
	$perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));

	// Only allow moderators to change payment settings
	if ($perm_is_moderator_or_admin) {

		// Set Default text
		if (!isset($form['#entity']->field_company_payments_enabled[LANGUAGE_NONE])) {
			$form['field_company_payments_enabled'][LANGUAGE_NONE]['#options']['_none'] = "Payments Disabled (default)";
			unset($form['field_company_payments_enabled'][LANGUAGE_NONE]['#options']['default']);
		} else {
			unset($form['field_company_payments_enabled'][LANGUAGE_NONE]['#options']['_none']);
		}
			
	} else {
		// Hide fields
		field_group_hide_field_groups($form, array('group_company_payments'));
		unset($form['field_company_payments_enabled']);
	}

	// Set placeholder (default) commission
	$default_commission = _tm_payments_get_default_commission("company_event");
	$form['field_company_payment_commission'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = ($default_commission + 0); // remove .00

}

/**
 * Modify event form
 * Called via tm_events_form_event_node_form_alter()
 */
function _tm_payments_form_alter_event_form(&$form, &$form_state, $form_id) {

	global $conf;
 	global $user;
  
	// Permission variables
	$perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));

	// Allow to hide moderator options
	if (isset($_GET["hide_moderator_options"])) {
		$perm_is_moderator_or_admin = false;
	}

	// STEP 1. Determine if event is new or editing existing
	//////////
	$event = $form_state['node'];
	$is_new_event = (!isset($event->nid) || isset($event->is_new));

	// STEP 2. Fetch event_type and set chapter_id, company_id
	//////////

	if ($is_new_event) {

		if (isset($_GET["field_chapter"])) {
		  $typeof_event = "chapter_event";
		  $chapter_id = $_GET["field_chapter"];
		} elseif (isset($_GET["field_event_company"])) {
		  $typeof_event = "company_event";
		  $company_id = $_GET["field_event_company"];
		} else {
		  $typeof_event = "member_event";
		  $event_owner_uid = $user->uid;
		}
	}

	// Existing event
	if (!$is_new_event) {

		if (isset($event->field_chapter[LANGUAGE_NONE][0])) {
		  $typeof_event = "chapter_event";
		  $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
		} elseif (isset($event->field_event_company[LANGUAGE_NONE][0])){
		  $typeof_event = "company_event";
		  $company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
		} else {
		  $typeof_event = "member_event";
		  $event_owner_uid = $event->uid;
		}
	} 

	// Step 3. Check if payments are enabled for this event
	//////////
	$chapter_id = _tm_event_get_chapter_from_edit_form($form);
	$company_id = _tm_event_get_company_from_edit_form($form);

	$event_id = null;
	if (isset($form['#entity']->nid))  {
		$event_id = $form['#entity']->nid;
	}
	
	// Check if payments can be enabled
	$enable_payments = _tm_payments_check_enable_payments($event_id, $chapter_id, $company_id);

	// If not enabled, hide payment fields
	if (!$enable_payments) {
		field_group_hide_field_groups($form, array('group_event_payment'));
		field_group_hide_field_groups($form, array('group_event_early_bird'));
		field_group_hide_field_groups($form, array('group_event_payout'));
		$form['field_event_payment_early_date']['#access'] = false;
		return;
	}

	// Step 4. Set help and placeholder text
	//////////

	// remove empty payment type option
	unset($form['field_event_payment_type'][LANGUAGE_NONE]['#options']['_none']);

	// help text
	$field_title = $form['field_event_payment_type'][LANGUAGE_NONE]['#title'];
	$payments_help_text = $field_title . "<div class='tm-payments-edit-form-message'>" . $conf["tm_payments_enable_help_text"] . "</div>";
	$form['field_event_payment_type'][LANGUAGE_NONE]['#title'] = $payments_help_text;

	// refund policy placeholder
	$refund_policy = $conf["tm_payments_refund_policy"];
	$form['field_event_payment_refund_text'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = $refund_policy;

	// pricing tiers
	$field_event_payment_tiers_description = "Pricing options, separated by commas.
<br><strong>0</strong> will display a <strong>Free</strong> option.
<br><strong>5*</strong> will autoselect and display <strong>$5 (Popular)</strong>
<br><strong>5|Coffee</strong> will display <strong>$5 &mdash; Coffee</strong>
<br><strong>CHOOSE</strong> will display a <strong>Choose your own price</strong> option.";

	$form['field_event_payment_tiers'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = "Example: 0,5,10,CHOOSE";
	$form['field_event_payment_tiers'][LANGUAGE_NONE][0]['value']['#attributes']['autocomplete'] = "off";
	$form['field_event_payment_tiers'][LANGUAGE_NONE][0]['value']['#description'] = $field_event_payment_tiers_description;

	// subscriber discount
	if (module_exists("tm_subscriptions_user")) {
		$form['field_event_payment_sub_discount'][LANGUAGE_NONE][0]['value']['#title'] = $conf["tm_subscriptions_user_label_noun"] . " Discount";
		$form['field_event_payment_sub_discount'][LANGUAGE_NONE][0]['value']['#description'] = "Reward <a target='_blank' href='/" . $conf["tm_checkout_subscription_public_page"] . "'>" . $conf["tm_subscriptions_user_label_noun"] . "</a> subscribers who support the community.<br>Example: 25%";
	}

	// filter % from subscriber discount
	$form['field_event_payment_sub_discount'][LANGUAGE_NONE][0]['value']['#element_validate'] = array('tm_payments_sub_discount_validate');

	// partner codes
	$field_event_payment_partner_codes_description = "Include any partner discount codes, separated by commas. 
<br>Example: VIP=50%,FRIENDS=10%";
	$form['field_event_payment_partner_code'][LANGUAGE_NONE][0]['value']['#description'] = $field_event_payment_partner_codes_description;
	$form['field_event_payment_partner_code'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = "example: FRIENDS=10%";
	$form['field_event_payment_partner_code'][LANGUAGE_NONE][0]['#validated'] = true;

	// Step 5. Set available currencies
	//////////
	$currencies = array();
	$default_currency = "";
	if (isset($conf["tm_payments_currencies"])) {
		foreach ($conf["tm_payments_currencies"] as $key => $val) {
			$currencies[$key] = $conf["tm_payments_currency_symbols"][$key] . " " . strtoupper($key);
		}
	}
	if (isset($conf["tm_payments_currencies_default"])) {
		$default_currency = $conf["tm_payments_currencies_default"];
	}

	// set options
	$form['field_event_payment_currency'][LANGUAGE_NONE]['#options'] = $currencies;
	$form['field_event_payment_currency'][LANGUAGE_NONE]['#title'] = "Pricing Currency";

	// if currency type saved, select it in the dropdown
	$event = $form['#entity'];
	$currency = $form['field_event_payment_currency'][LANGUAGE_NONE]['#entity'];
	if (isset($event->field_event_payment_currency[LANGUAGE_NONE][0]['value'])) {
	$default_currency = $event->field_event_payment_currency[LANGUAGE_NONE][0]['value'];
	}
	$form['field_event_payment_currency'][LANGUAGE_NONE]['#default_value'] = $default_currency;


	// Step 6. Set payout
	//////////

	$organizer_opts = array(); // always assign a payee //'_none' => t('- None -'));
	$loaded = user_load($event->uid);
	$organizer_opts[$loaded->uid] = tm_events_username($loaded) . " (" . $event->uid . ")";

	// Get chapter organizers
	if ($chapter_id != null) {
		$opts = array('_none' => t('- None -'))  + _load_organizers_per_chapter($chapter_id);
		foreach ($opts as $uid => $name) {
			if ($uid == "_none") {
				$option_label = "- Please select a recipient -";
			} else {
				$loaded = user_load($uid);
				$option_label = $loaded->field_user_first_name[LANGUAGE_NONE][0]['safe_value'] . " " . $loaded->field_user_last_name[LANGUAGE_NONE][0]['safe_value'] . " (" . $uid . ")";
				
			}
			$organizer_opts[$uid] = $option_label;
		}
	}

	// Get company organizers
	if ($company_id != null) {
		$company = node_load($company_id);
		foreach ($company->field_owner[LANGUAGE_NONE] as $owner) {
			$loaded = user_load($owner['target_id']);
			if ($loaded != null) {
				$organizer_opts[$loaded->uid] = tm_events_username($loaded) . " (" . $loaded->uid . ")";
			}
		}
		$form['field_event_payment_payout_to'][LANGUAGE_NONE]['#options'] = $organizer_opts;
	}

	// Remove duplicates
	$organizer_opts = array_unique($organizer_opts, SORT_REGULAR);

	// Add "- Please select a recipient -" at top
	unset($organizer_opts["_none"]);
	$organizer_opts = array("_none" => "- Please select a recipient -") +  $organizer_opts;

	// Set options
	$form['field_event_payment_payout_to'][LANGUAGE_NONE]['#options'] = $organizer_opts;

	// Set default selected payout
	if (!$is_new_event) {
		if (isset($event->field_event_payment_payout_to[LANGUAGE_NONE][0]['target_id'])) {
		  $payout_uid = $event->field_event_payment_payout_to[LANGUAGE_NONE][0]['target_id'];
		  $form['field_event_payment_payout_to'][LANGUAGE_NONE]['#default_value'] = $payout_uid;
		}
	}

	// Step 7. Display Commission
	/////////

	// Fetch default commision
	// order of priority:
	// chapter_event: event override > chapter override > chapter default (conf)
	// company_event: event override > chapter override > company default (conf)
	// member_event:  event override > member default (conf)

	if ($typeof_event == "chapter_event") {
		$default_commission = _tm_payments_get_commission("chapter_event", $chapter_id);
	}
	if ($typeof_event == "company_event") {
		$default_commission = _tm_payments_get_commission("company_event", $company_id);
	}
	if ($typeof_event == "member_event") {
		$default_commission = _tm_payments_get_commission("member_event", $event_owner_uid);
	}

	$form['field_event_payment_commission'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = ($default_commission + 0); // remove .00

	// If commission set, display it
	if (!$is_new_event) {
		if (isset($event->field_event_payment_commission[LANGUAGE_NONE][0]['value'])) {
			$form['field_event_payment_commission'][LANGUAGE_NONE][0]['value']['#default_value'] = ($event->field_event_payment_commission[LANGUAGE_NONE][0]['value'] + 0); // remove .00
		}
	}

	// Disable field if not moderator or admin
	if (!$perm_is_moderator_or_admin) {
		$form['field_event_payment_commission']['#disabled'] = true;
	}

	// Hide deprecated fields
	unset($form['field_event_payment_enabled']);
	unset($form['field_event_payment_ask_address']);

}

/**
 * Hook - modify event before saving
 */
function tm_payments_node_presave($node) {

global $user;

  // make sure we are working on an event node
  if ($node->type != "event") { return; }

  // Todo: only allow moderators to save commission
  
  // If commission is not set, store default commission
  // This way modifying default commission in future won't affect past events
  if (!isset($node->field_event_payment_commission[LANGUAGE_NONE][0]['value'])) {
  	$default_commission = _tm_payments_get_event_commission($node);
  	$node->field_event_payment_commission[LANGUAGE_NONE][0]['value'] = $default_commission;
  }

}

/**
 * Validate subscriber discount field
 */
function tm_payments_sub_discount_validate($element, &$form_state, $form) {

	// get value
	$submitted_value = trim($form_state['values']['field_event_payment_sub_discount'][LANGUAGE_NONE][0]['value']);

	// check empty
	if ($submitted_value == "") {
    	return;
    }

    // remove %
    $filtered = abs(intval(preg_replace("/[^0-9.]/", "", $submitted_value)));

    // check range
    if ($filtered < 0) {
    	$filtered = 0;
    }
    if ($filtered > 100) {
    	$filtered = 100;
    }

    // set value
  	$form_state['values']['field_event_payment_sub_discount'][LANGUAGE_NONE][0]['value'] = $filtered;
}


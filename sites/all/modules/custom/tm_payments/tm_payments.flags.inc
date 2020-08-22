<?php

// tm_payments.flags.inc - flag methods

/**
 * Implementation hook tm_payments_preprocess_flag
 */
function tm_payments_preprocess_flag(&$variables) {

	$flag = &$variables['flag'];
	$action = $variables['action'];

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
		$variables['link_text'] = 'Registered';
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

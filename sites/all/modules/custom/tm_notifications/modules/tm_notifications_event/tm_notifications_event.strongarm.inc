<?php
/**
 * @file
 * tm_notifications_event.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_event_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_event_register';
  $strongarm->value = array(
    'user_label' => 'Confirmation when registering for an event',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'You registered for [node:title]',
      'message' => 'Hi [current-user:field_user_first_name],

This is to let you know that you succesfully registered for [node:title]. For more information about this event, please see: [node:url]

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__user_event_register'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_event_unregister';
  $strongarm->value = array(
    'user_label' => 'Confirmation when unregistering from an event',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'You unregistered from event [node:title]',
      'message' => 'Hi [current-user:field_user_first_name],

This is a notification to let you know that you successfully unregistered for the Travel Massive event: [node:title].

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__user_event_unregister'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_event_join_waitlist';
  $strongarm->value = array(
    'user_label' => 'Confirmation when joining waitlist for an event',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'You joined the waiting list for for [node:title]',
      'message' => 'Hi [current-user:field_user_first_name],

This is to let you know that you are on the waiting list for [node:title]. For more information about this event, please see: [node:url]

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__user_event_join_waitlist'] = $strongarm;

  return $export;
}

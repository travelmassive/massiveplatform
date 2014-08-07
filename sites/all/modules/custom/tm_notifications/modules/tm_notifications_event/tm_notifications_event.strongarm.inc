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
  $strongarm->name = 'tm_notifications__user_event_join';
  $strongarm->value = array(
    'user_label' => 'Confirmation when joining an event',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'You signed up for [node:title]',
      'message' => 'Hi [current-user:field_user_first_name],

This is to let you know that you succesfully signed up for [node:title]. For more information about this event, please see: [node:url]

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__user_event_join'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_event_unjoin';
  $strongarm->value = array(
    'user_label' => 'Confirmation when unjoining an event',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'You unjoined [node:title]',
      'message' => 'Hi [current-user:field_user_first_name],

This is a notification to let you know that you successfully unjoined the Travel Massive event: [node:title].

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__user_event_unjoin'] = $strongarm;

  return $export;
}

<?php
/**
 * @file
 * tm_notifications_upcoming_event.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_upcoming_event_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__event_upcoming_2_days';
  $strongarm->value = array(
    'user_label' => 'Reminder two days ahead of event date',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '[node:title] in 2 days',
      'message' => 'Hi [current-user:field_user_first_name],
<br>
<br>This is a friendly reminder that the event [node:title] will take place in two days.
<br>
<br>__EMAIL_SIGNOFF__',
    ),
  );
  $export['tm_notifications__event_upcoming_2_days'] = $strongarm;

  return $export;
}

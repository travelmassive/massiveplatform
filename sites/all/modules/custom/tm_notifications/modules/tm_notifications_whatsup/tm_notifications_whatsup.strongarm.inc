<?php
/**
 * @file
 * tm_notifications_whatsup.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_whatsup_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__weekly_whatsup';
  $strongarm->value = array(
    'user_label' => 'Weekly whats up notification',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'Your weekly what\'s up',
      'message' => 'Hi [current-user:field_user_first_name],
<br>
<br>Here\'s what\'s been happening.
<br>
<br>__EMAIL_SIGNOFF__',
    ),
  );
  $export['tm_notifications__event_upcoming_2_days'] = $strongarm;

  return $export;
}

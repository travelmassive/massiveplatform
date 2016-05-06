<?php
/**
 * @file
 * tm_notifications_newsfeed.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_newsfeed_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__weekly_newsfeed';
  $strongarm->value = array(
    'user_label' => 'Weekly newsfeed notification',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'Your weekly newsfeed what\'s up',
      'message' => 'Hi [current-user:field_user_first_name],
<br>
<br>Here\'s what\'s been happening.
<br>
<br>__EMAIL_SIGNOFF__',
    ),
  );
  $export['tm_notifications__weekly_newsfeed'] = $strongarm;

  return $export;
}

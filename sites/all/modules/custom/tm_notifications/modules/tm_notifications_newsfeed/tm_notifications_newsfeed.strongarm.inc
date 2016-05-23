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
  $strongarm->name = 'tm_notifications__daily_newsfeed';
  $strongarm->value = array(
    'user_label' => 'Daily News Feed',
    'default_state' => '0',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '__NEWSFEED_SUBJECT__',
      'message' => '__NEWSFEED_BODY__',
    ),
  );
  $export['tm_notifications__daily_newsfeed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__weekly_newsfeed';
  $strongarm->value = array(
    'user_label' => 'Weekly News Feed',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '__NEWSFEED_SUBJECT__',
      'message' => '__NEWSFEED_BODY__',
    ),
  );
  $export['tm_notifications__weekly_newsfeed'] = $strongarm;

  return $export;
}

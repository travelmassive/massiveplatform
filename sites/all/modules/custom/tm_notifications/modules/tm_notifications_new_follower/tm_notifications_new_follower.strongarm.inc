<?php
/**
 * @file
 * tm_notifications_chapter.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_new_follower_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__new_follower';
  $strongarm->value = array(
    'user_label' => 'Notify when someone follows me',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '[current-user:field_user_first_name] [current-user:field_user_last_name] just followed you on Travel Massive',
      'message' => 'Hi [user:field_user_first_name],

[current-user:field_user_first_name] [current-user:field_user_last_name] just followed you on Travel Massive.

You can view their profile here:

__SERVER_NAME__/__PROFILE_URL__

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__new_follower'] = $strongarm;


  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__new_organization_follower';
  $strongarm->value = array(
    'user_label' => 'Notify when someone follows my company profile',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '[current-user:field_user_first_name] [current-user:field_user_last_name] just followed [node:title] on Travel Massive',
      'message' => 'Hi [user:field_user_first_name],

[current-user:field_user_first_name] [current-user:field_user_last_name] just followed your company profile [node:title] on Travel Massive.

You can view their profile here:

__SERVER_NAME__/__PROFILE_URL__

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__new_organization_follower'] = $strongarm;

  return $export;
}

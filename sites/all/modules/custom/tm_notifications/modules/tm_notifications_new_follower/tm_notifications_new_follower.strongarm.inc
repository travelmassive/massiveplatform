<?php
/**
 * @file
 * tm_notifications_new_follower.strongarm.inc
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
      'subject' => '__USER_FIRST_NAME__ __USER_LAST_NAME__ just followed you on Travel Massive',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__USER_FIRST_NAME__ __USER_LAST_NAME__ just followed you on __SITE_NAME__.
<br>
<br>You can view their profile here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__PROFILE_URL__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
__MESSAGING_INSTRUCTIONS__
<br>__EMAIL_SIGNOFF__
',
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
<br>
<br>[current-user:field_user_first_name] [current-user:field_user_last_name] just followed your company profile [node:title] on __SITE_NAME__.
<br>
<br>You can view their profile here:
<br>
<a href="__SERVER_HTTP__://__SERVER_NAME__/__PROFILE_URL__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__new_organization_follower'] = $strongarm;

  return $export;
}

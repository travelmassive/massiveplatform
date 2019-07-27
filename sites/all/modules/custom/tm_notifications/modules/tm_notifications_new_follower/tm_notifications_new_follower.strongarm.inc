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
      'subject' => '__USER_FIRST_NAME__ __USER_LAST_NAME__ just followed you on __SITE_NAME__',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__USER_FIRST_NAME__ __USER_LAST_NAME__ just followed you on __SITE_NAME__.
__MEMBER_WIDGET__
__FOLLOWER_MESSAGE__
__MESSAGING_INSTRUCTIONS__
__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__new_follower'] = $strongarm;


  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__new_organization_follower';
  $strongarm->value = array(
    'user_label' => 'Notify when someone follows my company page',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '__USER_FIRST_NAME__ __USER_LAST_NAME__ just __ACTION__ [node:title] on __SITE_NAME__',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__USER_FIRST_NAME__ __USER_LAST_NAME__ just __ACTION__ <a href="__SERVER_HTTP__://__SERVER_NAME__/node/__PROFILE_NID__">[node:title]</a> on __SITE_NAME__.
__MEMBER_WIDGET__
__FOLLOWER_MESSAGE__
__FOLLOWER_COUNT_MESSAGE__
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__new_organization_follower'] = $strongarm;

  return $export;
}

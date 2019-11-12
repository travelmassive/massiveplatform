<?php
/**
 * @file
 * tm_notifications_lists.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_lists_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__added_to_list';
  $strongarm->value = array(
    'user_label' => 'Notify when my profile is featured &mdash; <a href="/lists">learn more</a>',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '__LIST_ITEM_NAME_SUBJECT__ just got featured on __SITE_NAME__',
      'message' => 'Hi __USER_FIRST_NAME__,
<br>
<br>__LIST_ITEM_NAME_INTRO__ was featured in <a href="__SERVER_HTTP__://__SERVER_NAME__/lists/__LIST_ID__">__LIST_TITLE__</a> on __SITE_NAME__.
__ADD_LIST_MESSAGE__
<br>
<br>You were added to the list by __MODERATOR_NAME__.
<br>
<br>
__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__added_to_list_creator'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__added_to_list_creator';
  $strongarm->value = array(
    'user_label' => 'Notify when a profile is added to your list',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => '__LIST_ITEM_NAME_SUBJECT__ was added to your list',
      'message' => 'Hi __USER_FIRST_NAME__,
<br>
<br>__LIST_ITEM_NAME_INTRO__ was added to <a href="__SERVER_HTTP__://__SERVER_NAME__/lists/__LIST_ID__">__LIST_TITLE__</a> by __MODERATOR_NAME__.
<br>
<br>
__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__added_to_list_creator'] = $strongarm;

  return $export;
}

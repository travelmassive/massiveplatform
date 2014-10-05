<?php
/**
 * @file
 * tm_notifications_chapter.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_chapter_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__event_added_member_chapter';
  $strongarm->value = array(
    'user_label' => 'New event in one of your chapters',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'New event in [node:field_chapter]',
      'message' => 'Hi [current-user:field_user_first_name],

One of the organizers of the [node:field_chapter] chapter added an event to Travel Massive: [node:title]. Sign up today to make sure you can participate.

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__event_added_member_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_chapter_join';
  $strongarm->value = array(
    'user_label' => 'Confirmation when joining a chapter',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'You joined [node:title]',
      'message' => 'Hi [current-user:field_user_first_name],

This is a notification to let you know that you successfully joined the Travel Massive chapter: [node:title].

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__user_chapter_join'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_chapter_unjoin';
  $strongarm->value = array(
    'user_label' => 'Confirmation when leaving a chapter',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => 'You left [node:title]',
      'message' => 'Hi [current-user:field_user_first_name],

This is a notification to let you know that you successfully left the Travel Massive chapter: [node:title].

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__user_chapter_unjoin'] = $strongarm;

  return $export;
}

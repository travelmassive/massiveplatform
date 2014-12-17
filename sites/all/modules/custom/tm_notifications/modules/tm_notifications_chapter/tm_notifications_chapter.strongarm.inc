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
  $strongarm->name = 'tm_notifications__user_chapter_join_notify_leaders';
  $strongarm->value = array(
    'user_label' => 'Notify chapter leaders when new member joins chapter',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'New member: [current-user:field_user_first_name] joined [node:title]',
      'message' => 'Hi [user:field_user_first_name],

A new member [current-user:field_user_first_name] [current-user:field_user_last_name] just joined your chapter [node:title].

You can view their profile here:

__SERVER_NAME__/__PROFILE_URL__

If they are a new member to Travel Massive, they may also need approval. Please take the time to review their profile and ensure it\'s valid before approving them.

__EMAIL_NOTIFICATION_FOOTER__
',
    ),
  );
  $export['tm_notifications__user_chapter_join_notify_leaders'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_chapter_join_welcome_member';
  $strongarm->value = array(
    'user_label' => 'Welcome message when joining a chapter',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'Welcome to Travel Massive [node:title]',
      'message' => 'Hi [current-user:field_user_first_name],

You\'re in!

Thanks for joining the Travel Massive [node:title] Chapter.

We hope you can learn, share, and connect with other like-minded travel insiders through local chapter events in [node:title].

If you are a new member, please ensure that you\'ve filled out your profile so the Chapter Leaders can approve your account. 

__EMAIL_NOTIFICATION_FOOTER__
',
    ),
  );
  $export['tm_notifications__user_chapter_join_welcome_member'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_chapter_announcements';
  $strongarm->value = array(
    'user_label' => 'Chapter and Event Announcements',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '',
      'message' => '',
    ),
  );
  $export['tm_notifications__user_chapter_announcements'] = $strongarm;

  return $export;
}

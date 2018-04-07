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
    'user_label' => '[Chapter Leaders] Notify when new member joins your chapter',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array("chapter leader"),
    'email' => array(
      'subject' => 'New member: __USER_FIRST_NAME__ joined [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>A new member __USER_FIRST_NAME__ __USER_LAST_NAME__ just joined your chapter [node:title].
<br>
<br>You can view their profile here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>__APPROVED_BY_MESSAGE__
<br>
<br>__EMAIL_SIGNOFF__
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
      'subject' => 'Welcome to __CHAPTER_NAME__',
      'message' => 'Hi __USER_FIRST_NAME__,
<br>
<br>You\'re in!
<br>
<br>Thanks for joining the __CHAPTER_NAME__ Chapter.
<br>
__WELCOME_MESSAGE__
<br>We hope you can learn, share, and connect with other like-minded __MEMBERS_LABEL__ through local chapter events in [node:title].
<br>
<br>PS: If you are a new member, please ensure that you\'ve completed <a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">your profile</a> so the Chapter Leaders can approve your account.
<br>
<br>__EMAIL_SIGNOFF__
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

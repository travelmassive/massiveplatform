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
      'subject' => 'New member: __USER_FIRST_NAME__ __JOIN_ACTION__ __CHAPTER_NAME__',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__USER_FIRST_NAME__ __USER_LAST_NAME__ just joined your chapter [node:title].
<br>
__MEMBER_WIDGET__
Check out their profile and welcome them to the community 🎉
<br>
__ADDED_BY_MESSAGE__
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
      'subject' => '__JOIN_ACTION__ __CHAPTER_NAME__',
      'message' => 'Hi __USER_FIRST_NAME__,
<br>
__INTRO_MESSAGE__
<br>
__WELCOME_MESSAGE__
<br>Learn, share, and connect with other like-minded __MEMBERS_LABEL__ through local chapter events in __CHAPTER_NAME__.
__UNAPPROVED_MESSAGE__
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

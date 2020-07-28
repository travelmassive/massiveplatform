<?php

/**
 * @file
 * tm_notifications_approval.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_approval_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_approval_allow_reply_to';
  $strongarm->value = array(
    'user_label' => '[Moderation] Allow members that I flag to reply to me',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array(
      0 => 'chapter leader',
      1 => 'moderator',
    ),
    'email' => array(
      'subject' => '',
      'message' => '',
    ),
  );
  $export['tm_notifications__user_approval_allow_reply_to'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_approval_is_company_or_brand';
  $strongarm->value = array(
    'user_label' => 'Notification that account is company or brand related',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'Action required for verification: Your __SITE_NAME__ account must be an individual profile',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>We can\'t verify your account because it looks like you\'ve created an account for a company or brand.
<br>
__MODERATOR_MESSAGE__
<br><strong>All accounts on __SITE_NAME__ are for individuals (people).</strong>
<br>
<br>Please personalize <a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">your profile</a></strong> &mdash; <i>it only takes a few minutes.</i>
__MEMBER_WIDGET__
__MODERATION_TIPS__
<br>
<br>__EMAIL_SIGNOFF__
<p><small>(Moderator: __MODERATOR_FIRST_NAME__)</small></p>
',
    ),
  );
  $export['tm_notifications__user_approval_is_company_or_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_approval_need_more_info';
  $strongarm->value = array(
    'user_label' => 'Notification that user needs to provide more information',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'Action required for verification: Your __SITE_NAME__ profile needs more information',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>Your profile needs to be completed before we can verify it.
<br>
__MODERATOR_MESSAGE__
<br>Please complete <a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">your profile</a></strong> &mdash; <i>it only takes a few minutes.</i>
__MEMBER_WIDGET__
__MODERATION_TIPS__
<br>
<br>__EMAIL_SIGNOFF__
<p><small>(Moderator: __MODERATOR_FIRST_NAME__)</small></p>
',
    ),
  );
  $export['tm_notifications__user_approval_need_more_info'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_approval_non_community';
  $strongarm->value = array(
    'user_label' => 'Notification that user account is does not meet membership guidelines',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'Action required: Your __SITE_NAME__ profile doesn\'t meet our membership guidelines',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>Thanks for signing up, however it looks like your account doesn\'t meet our <a href="__COMMUNITY_VALUES_URL__">membership criteria</a>.
<br>
__MODERATOR_MESSAGE__
<br>__MODERATION_MEMBERSHIP_CRITERIA__
<br>
<br><strong>I meet the criteria &mdash; what do I need to do?</strong>
<br>Please update your profile to show how you meet the criteria:
__MEMBER_WIDGET__
<strong>I\'ve updated my account, what next?</strong>
<br>Tap "Request verification" from your account menu. Our team will review your profile and you\'ll be notified when you are verified.
<br>
<br><strong>Important:</strong>
<br>We may remove your account without further notice if you are unable to respond to this request or meet our community guidelines.
<br>
<br>If you need any assistance please reply to this email.
<br>
<br>__EMAIL_SIGNOFF__
<p><small>(Moderator: __MODERATOR_FIRST_NAME__)</small></p>
',
    ),
  );
  $export['tm_notifications__user_approval_non_community'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_approved';
  $strongarm->value = array(
    'user_label' => 'Notification that user has been verified',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => '__SUBJECT_LINE__',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__APPROVAL_INTRO_TEXT__
__MEMBER_WIDGET__
<br>You can view your profile here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>__APPROVAL_WELCOME_MESSAGE__
<br>
<br>Thanks for joining and welcome to __SITE_NAME__!
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__user_approved'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_requested_approval';
  $strongarm->value = array(
    'user_label' => '[Chapter Leaders] Notify when new member requests verification',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array(
      0 => 'chapter leader',
    ),
    'email' => array(
      'subject' => 'Verification request: __USER_FIRST_NAME__ __USER_LAST_NAME__ requested verification on __SITE_NAME__',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__USER_FIRST_NAME__ __USER_LAST_NAME__ has requested verification of their account.
__MEMBER_WIDGET__
Reason for joining: <i>__REASON_FOR_JOINING__</i>
<br>
<br><strong>What you need to do:</strong>
<br>Please review their <a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">profile page</a> and decide if you will verify their membership to our community.
<br>
<br><strong>How to verify (or moderate) their account:</strong>
<br>To verify the member select the "Verify member" option from their profile page &mdash; or flag their account as incomplete, a company, or if they do not meet our membership criteria.
<br>
<br>Thanks for helping to moderate our growing community.
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__user_requested_approval'] = $strongarm;

  return $export;
}

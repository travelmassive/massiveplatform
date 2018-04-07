<?php
/**
 * @file
 * tm_notifications_moderation.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_moderation_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_report_member';
  $strongarm->value = array(
    'user_label' => '[Moderation] Notify when a member is reported',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array(
      0 => 'moderator',
    ),
    'email' => array(
      'subject' => 'Member reported: __REPORTED_FULL_NAME__',
      'message' => 'Hi,
<br>
<br>The following account was reported by <a href="__SERVER_HTTP__://__SERVER_NAME__/__REPORTING_PROFILE_URL__">__REPORTING_FULL_NAME__</a>:
<br>
<br>Name: __REPORTED_FULL_NAME__
<br>Profile: <a href="__SERVER_HTTP__://__SERVER_NAME__/__REPORTED_PROFILE_URL__">__SERVER_NAME__/__REPORTED_PROFILE_URL__</a>
<br>ID: __REPORTED_PROFILE_ID__
<br>Reason: __REPORT_REASON__
__REPORT_MESSAGE__
<br>
<br><strong>Please review the account and address the reported issue. You can respond to the member who reported this issue by replying to this email.</strong>
<br>
<br>
<br>Community guidelines: __COMMUNITY_VALUES_URL__
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__user_report_member'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_report_organization';
  $strongarm->value = array(
    'user_label' => '[Moderation] Notify when an organization is reported',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array(
      0 => 'moderator',
    ),
    'email' => array(
      'subject' => 'Organization reported: __REPORTED_ORGANIZATION_NAME__',
      'message' => 'Hi,
<br>
<br>The following organization profile was reported by <a href="__SERVER_HTTP__://__SERVER_NAME__/__REPORTING_PROFILE_URL__">__REPORTING_FULL_NAME__</a>:
<br>
<br>Name: __REPORTED_ORGANIZATION_NAME__
<br>Profile: <a href="__SERVER_HTTP__://__SERVER_NAME__/__REPORTED_PROFILE_URL__">__SERVER_NAME__/__REPORTED_PROFILE_URL__</a>
<br>ID: __REPORTED_PROFILE_ID__
<br>Reason: __REPORT_REASON__
__REPORT_MESSAGE__
<br>
<br><strong>Please review the account and address the reported issue. You can respond to the member who reported this issue by replying to this email.</strong>
<br>
<br>
<br>Community guidelines: __COMMUNITY_VALUES_URL__
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__user_report_organization'] = $strongarm;

  return $export;
}

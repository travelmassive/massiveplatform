<?php
/**
 * @file
 * tm_notifications_subscriptions_user.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_subscriptions_user_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__subscriptions_user_notify_chapter_leaders';
  $strongarm->value = array(
    'user_label' => '[Chapter Leaders] Notify when a member purchases a subscription',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array("chapter leader"),
    'email' => array(
      'subject' => 'New subscription: __USER_FIRST_NAME__ __USER_LAST_NAME__ __PURCHASE_LABEL_SUBJECT__ __SUBSCRIPTION_LABEL__',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>Congratulations, __USER_FIRST_NAME__ __USER_LAST_NAME__ just __PURCHASE_LABEL__ __SUBSCRIPTION_LABEL__ to support <a href="__CHAPTER_URL__">your chapter</a>.
<br>
<br>You can <a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">view their profile</a> &mdash; connect with them and say hello!
<br>
__CHAPTER_LEADER_MESSAGE__
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__subscriptions_user_notify_chapter_leaders'] = $strongarm;

  return $export;
}

<?php
/**
 * @file
 * tm_notifications_events.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_events_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__events_register_flag';
  $strongarm->value = array(
    'user_label' => '[Chapter Leaders] Notify when someone registers for my event',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array("chapter leader"),
    'email' => array(
      'subject' => '__PROFILE_FULL_NAME__ __REGISTER_ACTION_SUBJECT__ [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__PROFILE_FULL_NAME__ __REGISTER_ACTION_BODY__ <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">[node:title]</a>.
__MEMBER_WIDGET__
__REGISTER_PAYMENT_NOTE__
__REGISTERED_BY_MESSAGE__
__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__events_register_flag'] = $strongarm;


    $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__events_register_unflag';
  $strongarm->value = array(
    'user_label' => '[Chapter Leaders] Notify when someone unregisters for my event',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array("chapter leader"),
    'email' => array(
      'subject' => '__PROFILE_FULL_NAME__ just un-registered for [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__PROFILE_FULL_NAME__ just un-registered from <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">[node:title]</a>.
__MEMBER_WIDGET__
__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__events_register_unflag'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__events_waitlist_flag';
  $strongarm->value = array(
    'user_label' => '[Chapter Leaders] Notify when someone registers for waitlist to my event',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array("chapter leader"),
    'email' => array(
      'subject' => '__PROFILE_FULL_NAME__ just joined the waitlist for [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__PROFILE_FULL_NAME__ just joined the waiting list for <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">[node:title]</a>.
__MEMBER_WIDGET__
__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__events_waitlist_flag'] = $strongarm;


    $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__events_waitlist_unflag';
  $strongarm->value = array(
    'user_label' => '[Chapter Leaders] Notify when someone unregisters from the waitlist to my event',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array("chapter leader"),
    'email' => array(
      'subject' => '__PROFILE_FULL_NAME__ just un-registered from the waitlist for [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__PROFILE_FULL_NAME__ just left the waiting list for <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">[node:title]</a>.
__MEMBER_WIDGET__
__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__events_waitlist_unflag'] = $strongarm;


  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__events_register_flag_user';
  $strongarm->value = array(
    'user_label' => 'Notify when you register for an event',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => '__REGISTER_SUBJECT__ [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__REGISTER_INTRO__
<br>
<br><b><a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">[node:title]</a></b>.
<br>
__MODERATOR_MESSAGE__
<br>
Here\'s the event details to keep handy &mdash;
<br>__EVENT_TIME__
<br>🌍 [node:field_event_venue_name]
<br>📍 [node:field_location]
<br>🗓 <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_CALENDAR_LINK__">Add to calendar</a>
__ONLINE_INSTRUCTIONS__
<br>
__REGISTER_EVENT_MESSAGE__
<br>See who else is attending, or message the event organizers by visiting the <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">event page</a>.
<br>
<br>__EMAIL_SIGNOFF__
<br>__REGISTER_PAYMENT_NOTE__
',
    ),
  );
  $export['tm_notifications__events_register_flag_user'] = $strongarm;


  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__events_register_unflag_user';
  $strongarm->value = array(
    'user_label' => 'Notify when your unregister from an event',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'You\'ve been un-registered for [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>This is a confirmation that you\'ve been un-registered from <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">[node:title]</a>.
<br>
<br>If you\'d like to register again in the future, visit the event page.
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__events_register_unflag_user'] = $strongarm;


  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__events_waitlist_flag_user';
  $strongarm->value = array(
    'user_label' => 'Notify when you join the waitlist to an event',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'You\'re on the waitlist to [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>This is a confirmation that you\'re on the waiting list for <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">[node:title]</a>.
<br>
__WAITLIST_EVENT_MESSAGE__
<br><strong>How does the waitlist work?</strong>
<br>We\'ll let you know if we\'re able to get you on the attendee list. Because of limited venue or event capacity we might not be able to fit everyone in.
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__events_waitlist_flag_user'] = $strongarm;


  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__events_waitlist_unflag_user';
  $strongarm->value = array(
    'user_label' => 'Notify you when you un-register from the waitlist to an event',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'You\'ve been removed from the waitlist for [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>This is a confirmation that you have been removed from the waiting list to <a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">[node:title]</a>.
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__events_waitlist_unflag_user'] = $strongarm;

  return $export;
}

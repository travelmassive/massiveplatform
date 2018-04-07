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
      'subject' => '[current-user:field_user_first_name] [current-user:field_user_last_name] __REGISTER_ACTION_SUBJECT__ [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>[current-user:field_user_first_name] [current-user:field_user_last_name] __REGISTER_ACTION_BODY__ [node:title].
<br>__REGISTER_PAYMENT_NOTE__
<br>You can view their profile here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>Here\'s the link to your event:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">__SERVER_NAME__/__EVENT_URL__</a>
<br>
<br>
<br>__EMAIL_SIGNOFF__
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
      'subject' => '[current-user:field_user_first_name] [current-user:field_user_last_name] just un-registered for [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>[current-user:field_user_first_name] [current-user:field_user_last_name] just removed their registration for your event [node:title].
<br>
<br>You can view their profile here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>Here\'s the link to your event:
<br>
<a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">__SERVER_NAME__/__EVENT_URL__</a>
<br>
<br>__EMAIL_SIGNOFF__
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
      'subject' => '[current-user:field_user_first_name] [current-user:field_user_last_name] just registered for the waitlist to [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>[current-user:field_user_first_name] [current-user:field_user_last_name] just signed up to the waiting list for your event [node:title].
<br>
<br>You can view their profile here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>Here\'s the link to your event:
<br>
<a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">__SERVER_NAME__/__EVENT_URL__</a>
<br>
<br>__EMAIL_SIGNOFF__
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
      'subject' => '[current-user:field_user_first_name] [current-user:field_user_last_name] just un-registered from the waitlist for [node:title]',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>[current-user:field_user_first_name] [current-user:field_user_last_name] just removed their waiting list registration for your event [node:title].
<br>
<br>You can view their profile here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/user/__PROFILE_UID__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>Here\'s the link to your event:
<br>
<a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">__SERVER_NAME__/__EVENT_URL__</a>
<br>
<br>__EMAIL_SIGNOFF__
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
<br>__REGISTER_INTRO__ [node:title].
<br>
<br>Here\'s the event details to keep handy:
<br>
<br>[node:title]
<br>Date: __EVENT_DATE__
<br>Start: __EVENT_TIME__
<br>Venue: [node:field_event_venue_name]
<br>Location: [node:field_location]
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_CALENDAR_LINK__">Add to calendar</a>
<br>
__REGISTER_EVENT_MESSAGE__
<br>See who else is attending by visiting the event page:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">__SERVER_NAME__/__EVENT_URL__</a>
<br>
<br>See you there!
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
<br>This is a confirmation that you\'ve been un-registered for the event [node:title].
<br>
<br>If you\'d like to register again in the future, please visit the event page:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">__SERVER_NAME__/__EVENT_URL__</a>
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
<br>This is a confirmation that you\'re on the waiting list for the event [node:title].
<br>
__WAITLIST_EVENT_MESSAGE__
<br>We\'ll let you know if we\'re able to get you on the attendee list. Because of limited venue or event capacity we might not be able to fit everyone in.
<br>
<br>You can view the event details here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">__SERVER_NAME__/__EVENT_URL__</a>
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
<br>This is a confirmation that we\'ve removed your waiting list registration for the event [node:title].
<br>
<br>If you\'d like to register again in the future, please visit the event page:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__EVENT_URL__">__SERVER_NAME__/__EVENT_URL__</a>
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__events_waitlist_unflag_user'] = $strongarm;

  return $export;
}

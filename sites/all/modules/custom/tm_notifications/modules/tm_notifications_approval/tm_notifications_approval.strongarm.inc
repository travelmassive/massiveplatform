<?php
/**
 * @file
 * tm_notifications_chapter.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_approval_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_approved';
  $strongarm->value = array(
    'user_label' => 'Notification that user has been approved',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'Your __SITE_NAME__ account has been approved',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__APPROVAL_INTRO_TEXT__
<br>
<br>You can view your profile here:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__PROFILE_URL__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br><strong>ARE YOU READY TO CHANGE TRAVEL?</strong>
<br>
<br>Our mission is to connect travel insiders in every city in the world in order to empower change in travel.
<br>
<br>Travel Massive has always been an open and collaborative community founded on three simple principles – to be open to everyone in the travel industry, to be free to participate, and to connect and share globally.
<br>
<br>In addition to our core values we have the following community values:
<br>
<br>* Building Meaningful Partnerships
<br>* Fostering Innovation And Big Ideas
<br>* Supporting Diversity And New Talent
<br>* Empowering Change In Travel
<br>* Promoting Responsible Travel
<br>
<br>You can read more on our public plan at <a href="http://www.travelmassive.org">www.travelmassive.org</a>
<br>
<br>If there\'s a chapter in your city, then please join it and attend our events to connect with like-minded travel industry professionals. If you can\'t find a chapter, why don\'t you consider starting a Travel Massive chapter? Get in touch with us to find out more.
<br>
<br>Thanks for joining and welcome to __SITE_NAME__!
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__user_approved'] = $strongarm;

  return $export;
}

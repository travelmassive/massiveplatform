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
      'subject' => 'Your Travel Massive account has been approved',
      'message' => 'Hi [user:field_user_first_name],

Your account has been approved by [current-user:field_user_first_name].

You can view your profile here:

__SERVER_NAME__/__PROFILE_URL__

ARE YOU READY TO CHANGE TRAVEL?

Our mission is to connect travel insiders in every city in the world in order to empower change in travel.

Travel Massive has always been an open and collaborative community founded on three simple principles – to be open to everyone in the travel industry, to be free to participate, and to connect and share globally.

In addition to our core values we have the following community values:

* Building Meaningful Partnerships
* Fostering Innovation And Big Ideas
* Supporting Diversity And New Talent
* Empowering Change In Travel
* Promoting Responsible Travel

You can read more on our public plan at www.travelmassive.org

If there\'s a chapter in your city, then please join it and attend our events to connect with like-minded travel industry professionals. If you can\'t find a chapter, why don\'t you consider starting a Travel Massive chapter? Get in touch with us to find out more.

Why is my account pending approval?
Your account is currently pending approval by one of our local chapter leaders. We manually approve every account to ensure the Travel Massive community is genuine. 

Thanks for joining and welcome to Travel Massive!

cheers,
The Travel Massive team',
    ),
  );
  $export['tm_notifications__user_approved'] = $strongarm;

  return $export;
}

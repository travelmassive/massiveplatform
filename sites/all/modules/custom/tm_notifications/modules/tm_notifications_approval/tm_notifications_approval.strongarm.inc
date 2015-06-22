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
  $strongarm->name = 'tm_notifications__user_approval_is_company_or_brand';
  $strongarm->value = array(
    'user_label' => 'Notification that account is company or brand related',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'Action required for approval: Your __SITE_NAME__ account must be an individual profile',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>We can\'t approve your account because it looks like you\'ve created an account for a company or brand. See below for how to get your account approved and how to add a company profile.
<br>
__MODERATOR_MESSAGE__
<br><strong>Please read carefully:</strong>
<br>All accounts on __SITE_NAME__ are for individual use. After your account is approved, you can then add and manage your company listing which will be displayed on our <a href="__SERVER_HTTP__://__SERVER_NAME__/companies">company listings page</a>.
<br>
<br><strong>How to add your company profile:</strong>
<br>First, sign in and update your profile information so it\'s about yourself (not your brand) to get approved. Once your individual account is approved, you can add a company profile by visiting the "Add company" link from your profile menu.
<br>
<br><strong>What you need to do:</strong>
<br>Please sign in and personalize your profile information. Here\'s a link to you public profile:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__PROFILE_URL__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br><strong>Once you\'ve personalized your account, we\'ll be able to approve you and you can add your company profile.</strong>
<br>
<br>Here\'s some tips to get approved:
<br>* Provide your real First and Last name if you haven\'t already
<br>* Tell us about yourself (not your company) in the "About me" section
<br>* Upload a photo of yourself so members can recognize you
<br>* Tell us what city you are based in
<br>* Fill out your profile questions
<br>* Provide links to your website, Twitter, Facebook or LinkedIn accounts
<br>
<br><strong>I\'ve updated my account, what next?</strong>
<br>When you\'re ready for us to review your account, select "Approve my account" from your account menu.
<br>Our team will review your profile and you\'ll be notified when you are approved.
<br>If you need any assistance please reply to this email.
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
      'subject' => 'Action required for approval: Your __SITE_NAME__ profile needs more information',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>Your profile needs to be completed before we can approve your account.
<br>
__MODERATOR_MESSAGE__
<br><strong>What you need to do:</strong>
<br>Please sign in and complete your profile information. Here\'s a link to you public profile:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__PROFILE_URL__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>Here\'s some tips to get approved:
<br>* Tell us about yourself in the "About me" section
<br>* Upload a profile picture so members can recognize you
<br>* Tell us what city you are based in
<br>* Fill out your Job Role and Organization Name (if you work for a company)
<br>* Update your Industry Segment
<br>* Fill out your profile questions
<br>* Provide links to your website, Twitter, Facebook or LinkedIn accounts
<br>
<br><strong>I\'ve updated my account, what next?</strong>
<br>When you\'re ready for us to review your account, select "Approve my account" from your account menu.
<br>Our team will review your profile and you\'ll be notified when you are approved.
<br>If you need any assistance please reply to this email.
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

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_requested_approval';
  $strongarm->value = array(
    'user_label' => '[Chapter Leaders] Notify when new member requests approval',
    'default_state' => '1',
    'change_allowed' => 1,
    'change_allowed_roles' => array(
      0 => 'chapter leader',
    ),
    'email' => array(
      'subject' => 'Approval request: __USER_FIRST_NAME__ __USER_LAST_NAME__ requested approval on __SITE_NAME__',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>__USER_FIRST_NAME__ __USER_LAST_NAME__ has requested approval of their account:
<br>
<br>Please review their profile here:
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__PROFILE_URL__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br>Reason for joining: __REASON_FOR_JOINING__
<br>
<br>If you are satisfied with their profile you may approve the account. To approve the account select the "Approve member" option on their profile page menu.
<br>
<br>__EMAIL_SIGNOFF__
',
    ),
  );
  $export['tm_notifications__user_requested_approval'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_approval_community_guidelines';
  $strongarm->value = array(
    'user_label' => 'Notification that user account is non community',
    'default_state' => '1',
    'change_allowed' => 0,
    'email' => array(
      'subject' => 'Action required: Your __SITE_NAME__ profile doesn\'t meet our community guidelines',
      'message' => 'Hi [user:field_user_first_name],
<br>
<br>Thanks for signing up, however it looks like your account doesn\'t meet our <a href="__COMMUNITY_GUIDELINES_URL__">community guidelines</a>:
<br>
__MODERATOR_MESSAGE__
<br>
<br>To be a member of the __SITE_NAME__ community, you must demonstrate one of the following:
<br>* You are active in the travel industry (example: tour operator, travel brand, DMO)
<br>* You are active in a field closely related to the travel industry (example: Tourism PR)
<br>* You are are a travel content creator (example: blogger, vlogger, presenter)
<br>* You are studying tourism or a closely related course
<br>* Any other criteria that reasonably demonstrates a professional connection to the tourism or travel industry.
<br>* You can read our community guidelines at __COMMUNITY_GUIDELINES_URL__
<br>
<br>
<br><strong>But I love travel, can I join still?</strong>
<br>Our community provides professional networking and skills development for members of the travel and tourism industry. If you are an expert or avid traveler, then thanks for stopping by and we wish you all the best on your journeys!
<br>
<br>
<br><strong>What you need to do:</strong>
<br>If you meet the community guidelines, please update your profile information to demonstrate how you meet the criteria. Here\'s a link to you public profile:
<br>
<br><a href="__SERVER_HTTP__://__SERVER_NAME__/__PROFILE_URL__">__SERVER_NAME__/__PROFILE_URL__</a>
<br>
<br><strong>I\'ve updated my account, what next?</strong>
<br>When you\'re ready for us to review your account, select "Approve my account" from your account menu so we can check it out.
<br>Our team will review your profile and you\'ll be notified when you are approved.
<br>If you need any assistance please reply to this email.
<br>
<br><strong>Important:</strong>
<br>We may remove your account without further notice if you are unable to respond to this request or meet our community guidelines.
<br>__EMAIL_SIGNOFF__
<p><small>(Moderator: __MODERATOR_FIRST_NAME__)</small></p>
',
    ),
  );
  $export['tm_notifications__user_approval_community_guidelines'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__user_approval_allow_reply_to';
  $strongarm->value = array(
    'user_label' => '[Moderation] Allow members that I flag to reply to me',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '',
      'message' => '',
    ),
  );
  $export['tm_notifications__user_approval_allow_reply_to'] = $strongarm;

  return $export;
}

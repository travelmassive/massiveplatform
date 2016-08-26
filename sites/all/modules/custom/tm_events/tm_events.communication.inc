<?php

// tm_events.announcement.inc - annoucement methods

/**
 * Form for communicating with attendees
 */
function tm_events_communication_form($event) {

  $event = node_load(arg(2));
  $form = array();
  global $user;
  global $conf;

  // Add javascript for form
  drupal_add_js(drupal_get_path('module', 'tm_events') . '/js/tm_events_communication_v4.js');

  // Add page title and page url to javascript
  $event_url = drupal_get_path_alias('node/'.$event->nid);
  $event_title = drupal_get_path_alias($event->title);
  drupal_add_js(array('tm_events' => array('event_url' => $event_url, 'event_title' => $event_title)), array('type' => 'setting'));

  // Get chapter associated with event if there is one
  $chapter = null;
  if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
    $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
    $chapter = node_load($chapter_id);
  }

  // Get company associated with event if there is one
  $company = null;
  if (isset($event->field_event_company[LANGUAGE_NONE])) {
    if (sizeof($event->field_event_company[LANGUAGE_NONE][0] > 0)) {
      $company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
      $company = node_load($company_id);
    }
  }

  // Set event id in form
  $form['eventid'] = array(
    '#type' => 'token',
    '#id' => 'eventid',
    '#value' => $event->nid,
  );

  // Add intro help text
  if (isset($conf["tm_event_send_announcements_intro"])) {
    drupal_get_messages('help-tip'); // supress other help tips
    drupal_set_message($conf["tm_event_send_announcements_intro"], 'help-tip');
  }

  // Separator
  $form['step_1_text'] = array(
    '#markup' => '<div class="tm-event-send-announcement-step">Step 1. Create announcement</div>'
  );

  // Create subject
  $subject = t($conf['tm_site_name'] . ' - ').$event->title;
  if ($chapter != null) {
    $subject = t($chapter->title . ' ' . $conf['tm_site_name'] . ' - ').$event->title;
  }

  // Subject field
  $form['subject'] = array(
    '#title' => t('Subject: '),
    '#type' =>'textfield',
    '#required' => TRUE,
    '#value' => $subject,
    );

  // Reply to address
  $form['reply_to'] = array(
    '#title' => t('Reply-to:'),
    '#type' =>'textfield',
    '#id' => 'edit-reply-to',
    '#disabled' => true,
    '#default_value' => $user->mail,
    '#description' => 'Any replies to this announcement will be sent to your email address.',
    '#rules' => array('email')
    );

  // Allow chapter leaders, moderators, administrator to set reply-to
  if ((in_array('administrator', array_values($user->roles)))
      or (in_array('moderator', array_values($user->roles)))
      or (in_array('chapter leader', array_values($user->roles)))) {

    $form['reply_to']['#disabled'] = false;
    $form['reply_to']['#description'] = 'Any replies to this announcement will be sent here. Leave blank for no reply-to address.';
  }

  // Prefill announcement body text

  // Allow chapter leaders, moderators, administrator to use html email
  $send_html_email = false;
  if ((in_array('administrator', array_values($user->roles)))
      or (in_array('moderator', array_values($user->roles)))
      or (in_array('chapter leader', array_values($user->roles)))) {
    $send_html_email = true;
  }

  $prefill = tm_events_create_prefill_text($event, $chapter, $company, $send_html_email);
  $form['body'] = array(
    '#type' =>'text_format',
    '#title' => t('Custom Message'),
    '#rows' => 18,
    '#value' => $prefill,
    '#required' => TRUE,
    '#id' => 'edit-body',
    '#wysiwyg' => TRUE,
    '#format' => 'plain_text',
  );

  // Allow chapter leaders, moderators, administrator to use html email
  if ($send_html_email) {
    $form['body']['#format'] = 'html_email';
  }

  // Separator
  $form['include_cover_image_before'] = array(
    '#markup' => '<br>'
  );

  // Include event image
  $form['include_cover_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include event image'),
    '#default_value' => true,
    '#hidden' => true
  );

  // Show preview of event image
  if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
    $image_url = image_style_url("banner", $event->field_image[LANGUAGE_NONE][0]['uri']);
  } else {
    $default_image_uri = _tm_events_get_default_image($conf["tm_images_default_field_image"], $event->nid);
    $image_url = image_style_url("banner", $default_image_uri);
  }
  $form['include_cover_image']['#description'] = '<div style="margin-top: 1rem; margin-left: -1rem;"><img style="width: 150px; border: 1px solid #eee;" src="' . $image_url . '"/><br>Grab the attention of your members by including your event image.<br>It will be displayed above your custom message.</div>';
  
  // Check tm_event_announcement_enable_cover_image is enabled
  if ($conf["tm_event_announcement_enable_cover_image"] == false) {
      $form['include_cover_image']['#type'] = 'hidden';
  }

  // Separator
  $form['step_2_text'] = array(
    '#markup' => '<div style="border-top: 2px dashed #eee; margin-top: 2rem;"><div class="tm-event-send-announcement-step">Step 2. Test your announcement</div></div>'
  );

  // Test email address
  $form['test_email'] = array(
    '#title' => t('Send test email to:'),
    '#type' =>'textfield',
    '#id' => 'edit-testemail',
    '#value' => $user->mail,
  );

  // Test email
  // Note: disabled (hidden)
  $full_user = user_load($user->uid);
  $form['test_email_name'] = array(
    '#title' => t('Replace [first_name] with:'),
    '#type' =>'hidden',
    '#id' => 'edit-testemail-name',
    '#default_value' => $full_user->field_user_first_name[LANGUAGE_NONE][0]['value'],
    '#description' => "[first_name] will be replaced with the member's first name when sent to all recipients."
    );

  // Test email button
  $form['test_button'] = array(
    '#type' => 'submit',
    '#value' => t('Send test email'),
    '#id' => 'event-test-email-submit',
  );

  // Step 3
  $form['step_3_text'] = array(
    '#markup' => '<div style="border-top: 2px dashed #eee; margin-top: 2rem;"><div class="tm-event-send-announcement-step">Step 3. Send your announcement</div></div>'
  );

  // Registered count
  $flag_register = flag_get_flag('event_register');
  $flag_waitlist = flag_get_flag('event_waitlist');
  $num_registered = $flag_register->get_count($event->nid);
  $num_waitlisted = $flag_waitlist->get_count($event->nid);

  // Permission variables
  $perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));

  // Add event chapters
  $chapter_nids = tm_events_get_published_chapters($event);
  if ($chapter != null) {
    array_unshift($chapter_nids, $chapter->nid);
  }
  $chapter_nids = array_unique($chapter_nids);

  // Add each chapter
  foreach ($chapter_nids as $chapter_id) {

    // load chapter
    $published_chapter = node_load($chapter_id);

    // check if user is chapter leader
    if ((tm_users_is_chapter_leader($user->uid, $chapter_id)) or ($perm_is_moderator_or_admin)) {

      $num_members = _tm_chapters_get_num_members($published_chapter->nid);
      $options['chapter-' . $chapter_id] = tm_chapters_formal_name($published_chapter) . ' - All members (' . tm_events_pluralize_members($num_members) . ')';

      $members_not_registered = tm_events_get_chapter_members_not_registered($event->nid, $chapter_id);

      $num_members_not_registered = sizeof($members_not_registered);
      $options['chapter_and_not_rsvp_or_waitlist-' . $chapter_id] = tm_chapters_formal_name($published_chapter) . t(' - Not registered (' . tm_events_pluralize_members($num_members_not_registered) . ')');
    } 
  }

  // Only show company option if the event has a company
  if ($company != null) {
    $num_followers = tm_organizations_get_num_followers($company->nid);
    $options['company_followers'] = t('Company followers (' . tm_events_pluralize_members($num_followers) . ')');
  }

  // Member event - select followers
  if (($chapter == null) && ($company == null) && (tm_events_check_published_chapter_leader($event, $user->uid) == null)) {
    $num_followers = tm_users_get_num_followers($user->uid);
    $options['member_followers'] = t('My followers (' . tm_events_pluralize_members($num_followers) . ')');
  }

  // Recipient options
  if ($num_registered > 0) {
    $options['rsvp'] = t('Members who are registered for the event (' . tm_events_pluralize_members($num_registered) . ')');  
  }
  if ($num_waitlisted > 0) {
    $options['waitlist'] = t('Members on the waitlist (' . tm_events_pluralize_members($num_waitlisted) . ')');
  }
  if (($num_waitlisted + $num_registered) > 0) {
    $options['all'] = t('Both registered or waitlist (' . tm_events_pluralize_members($num_registered + $num_waitlisted). ')');
  }

  // Recipients
  $form['recipients'] = array(
    '#type' =>'select',
    '#options' => $options,
    '#title' => t('Send announcement to:'),
  );
  
  // Approved members
  $form['approved_members'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send to approved members only'),
    '#default_value' => true,
  );

  if ($chapter == null) {
    $form['approved_members']['#type'] = 'hidden';
  }

  // Confirm
  $form['confirm_send'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>I\'m ready.</strong> Have you tested your email? There\'s no undo or turning back from here!'),
    '#id' => 'event-email-attendees-confirm',
  );

  // send test email to this address (box, button, ajax)
  // and confirm popup when you hit the final submit
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Send Email To All Recipients'),
    '#id' => 'event-email-attendees-submit',
    '#attributes' => array('class' => array(t('bttn-success'))),
  );

  $help_text = '<div class="tm-event-send-announcement-tips">
  <b>How to send a great announcement</b>
  <ul style="list-style: none; padding-left: 1.5rem;">
  <li><span style="color: green;">&#10004;</span> Write a meaningful subject line that captures people\'s attention</li>
  <li><span style="color: green;">&#10004;</span> The best time to send an announcement is on a weekday between 10.00 and 11.00</li>
  <li><span style="color: green;">&#10004;</span> Your event is going to rock, tell your recipients why they should come!</li>
  <li><span style="color: red;">&#10008;</span> Don\'t WRITE IN ALL CAPITALS (that\'s shouting)</li>
  <li><span style="color: red;">&#10008;</span> Don\'t use many exclamation points!!!!!</li>
  <li><span style="color: red;">&#10008;</span> Don\'t use spam trigger words like "free" or "click here"</li>
  </ul>
  <b>Other things to know</b><ul>';

  // Warning for non chapter leaders
  if ((!in_array('administrator', array_values($user->roles)))
      and (!in_array('moderator', array_values($user->roles)))
      and (!in_array('chapter leader', array_values($user->roles)))) {
    $help_text .= "<li><b>Use of this service is covered by our <a target='_blank' href='" . $conf["tm_community_values_url"] . "'>community values</a>. Please use this service responsibly.</b></li>";
  }

  $help_text .= '<li>Members can opt-out by unchecking <i>Chapter and Event Announcements</i> in their notification settings.</li>
  <li>Members must have a verified email address to receive your announcement.</li>
  <li>Emails to recipients are sent out in a queue and may take a number of minutes to be delivered.</li>
  </ul>
  </div>';

  $form['help_text'] = array('#markup' => $help_text);

  return $form;
}

/**
 * Test email callback 
 */
function tm_events_test_email_ajax(){

  global $conf;

  if (!isset($_POST['subject'])) return FALSE;
  if (!isset($_POST['message'])) return FALSE;
  if (!isset($_POST['address'])) return FALSE;
  if (!isset($_POST['eventid'])) return FALSE;
  if (!isset($_POST['first_name'])) return FALSE;
  if (!isset($_POST['include_cover_image'])) return FALSE;
  if (!isset($_POST['message_format'])) return FALSE;

  $subject = $_POST['subject'];
  $message = $_POST['message'];
  $message .= "<p><small>(Event ID: " . $_POST['eventid'] . ")</small></p>";
  $email = $_POST['address'];
  $eventid = $_POST['eventid'];
  $include_cover_image = ($_POST['include_cover_image'] == "1");
  $first_name = $_POST['first_name'];
  $replyto = "";
  if (isset($_POST['replyto'])) { 
    $replyto = trim($_POST['replyto']);
  }
  $message_format = $_POST['message_format'];

  // Replace [first_name] token
  $message = str_replace("[first_name]", $first_name, $message);
  
  // If message format is text, add <br>'s
  if ($message_format == "text") {
    $message = str_replace("\n", "<br>", $message);
  }

  // Insert cover image
  // Check if tm_event_announcement_enable_cover_image is enabled
  $event_image_src = null;
  $event_image_link = null;
  if ($conf['tm_event_announcement_enable_cover_image'] == true) {
    if ($include_cover_image) {
      
      // load event
      $event = node_load($eventid);

      // get image url
      if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
        $event_image_src = image_style_url("banner", $event->field_image[LANGUAGE_NONE][0]['uri']);
      } else {
        $default_image_uri = _tm_events_get_default_image($conf["tm_images_default_field_image"], $event->nid);
        $event_image_src = image_style_url("banner", $default_image_uri);
      }

      // get event url
      $event_image_link = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . drupal_get_path_alias('node/' . $event->nid);

    }
  }
  
  $sendcount = tm_events_send_email(null, $email, $subject, $message, $event_image_src, $event_image_link, $eventid, $replyto);

  // indicate success to the js ajax handler
  print(json_encode(array("sent" => $sendcount)));
}

/**
 * Submit function of communication ajax request
 */
function tm_events_send_emails($recipients, $subject, $message, $event_image_src, $event_image_link, $eventid, $chapterid, $replyto = "", $approved_members = true){

  global $user;

  // permission check
  $event = node_load($eventid);
  if (!tm_events_check_permission_announce_event($user, $event)) {
    print(json_encode(array("sent" => $sendcount)));
  }

  // all these user lists are just flags to event or chapter
  $rsvpflag = flag_get_flag('event_register', NULL);
  $waitflag = flag_get_flag('event_waitlist', NULL);
  $chapterflag = flag_get_flag('signup', NULL);
  $follow_members = flag_get_flag('follow_members', NULL);
  $follow_organizations = flag_get_flag('follow_organizations', NULL);
  $sendcount = 0;

  // RSVP list
  if ($recipients == 'rsvp'){
    $results = db_select('flagging', 'f')
      ->fields('f')
      ->condition('fid', $rsvpflag->fid ,'=')
      ->condition('entity_id', $eventid ,'=')
      ->execute();

    // create user_ids list from results
    while ($result = $results->fetchAssoc()){
      $user_ids[] = $result['uid'];
    }
  }

  // Waitlist
  if ($recipients == 'waitlist'){
    $results = db_select('flagging', 'f')
      ->fields('f')
      ->condition('fid', $waitflag->fid ,'=')
      ->condition('entity_id', $eventid ,'=')
      ->execute();

    // create user_ids list from results
    while ($result = $results->fetchAssoc()){
      $user_ids[] = $result['uid'];
    }
  }

  // RSVP and Waitlist
  if ($recipients == 'all') {

    $results = db_select('flagging', 'f')
      ->fields('f')
      ->condition('fid', array($waitflag->fid, $rsvpflag->fid) ,'in')
      ->condition('entity_id', $eventid ,'=')
      ->execute();

    // create user_ids list from results
    while ($result = $results->fetchAssoc()){
      $user_ids[] = $result['uid'];
    }
  }

  // All chapter members
  if ($recipients == 'chapter') {

    $results = db_select('flagging', 'f')
      ->fields('f')
      ->condition('fid', $chapterflag->fid ,'=')
      ->condition('entity_id', $chapterid ,'=')
      ->execute();

    // create user_ids list from results
    while ($result = $results->fetchAssoc()){
      $user_ids[] = $result['uid'];
    }
  }

  // All chapter members who are not registered or on waitlist
  if ($recipients == 'chapter_and_not_rsvp_or_waitlist') {

    $members_in_chapter = array();
    $members_who_have_rsvp = array();
    $members_who_have_waitlist = array();

    $results = db_select('flagging', 'f')
      ->fields('f')
      ->condition('fid', $chapterflag->fid ,'=')
      ->condition('entity_id', $chapterid ,'=')
      ->execute();
    while ($result = $results->fetchAssoc()){
      $members_in_chapter[] = $result['uid'];
    }

    $results = db_select('flagging', 'f')
      ->fields('f')
      ->condition('fid', $rsvpflag->fid ,'=')
      ->condition('entity_id', $eventid ,'=')
      ->execute();
    while ($result = $results->fetchAssoc()){
      $members_who_have_rsvp[] = $result['uid'];
    }

    $results = db_select('flagging', 'f')
      ->fields('f')
      ->condition('fid', $waitflag->fid ,'=')
      ->condition('entity_id', $eventid ,'=')
      ->execute();

    while ($result = $results->fetchAssoc()){
      $members_who_have_waitlist[] = $result['uid'];
    }

    // we only want people who are in the chapter who have not registered or waitlist
    $user_ids = array_diff($members_in_chapter, $members_who_have_rsvp, $members_who_have_waitlist);
  }

  // Member Followers
  if ($recipients == 'member_followers') {
   
    $results = db_select('flagging', 'f')
      ->fields('f')
      ->condition('fid', $follow_members->fid ,'=')
      ->condition('entity_id', $user->uid ,'=')
      ->execute();

    // create user_ids list from results
    while ($result = $results->fetchAssoc()){
      $user_ids[] = $result['uid'];
    }
  }

  // Company Followers
  if ($recipients == 'company_followers') {
   
    $load_event = node_load($eventid);
    $company_id = null;
    if (isset($load_event->field_event_company[LANGUAGE_NONE])) {
      if (sizeof($load_event->field_event_company[LANGUAGE_NONE][0] > 0)) {
        $company_id = $load_event->field_event_company[LANGUAGE_NONE][0]['target_id'];

        $results = db_select('flagging', 'f')
          ->fields('f')
          ->condition('fid', $follow_organizations->fid ,'=')
          ->condition('entity_id', $company_id ,'=')
          ->execute();

        // create user_ids list from results
        while ($result = $results->fetchAssoc()){
          $user_ids[] = $result['uid'];
        }

      }
    }
  }

  // Remove any duplicates
  $user_ids = array_unique($user_ids);

  // send the emails
  // We use a cron queue tm_event_announcements to send emails in the background.
  $original_message = $message;
  foreach ($user_ids as $user_id) {

    $user_account = user_load($user_id);

    // if approved_members then check approval status of user
    if ($approved_members) {
      if (!in_array('approved user', $user_account->roles)) {
        continue; // skip this member, they are not approved
      }
    }

    // replace [first_name] token
    $first_name = $user_account->field_user_first_name[LANGUAGE_NONE][0]['value'];
    $message = str_replace("[first_name]", $first_name, $original_message);

    // Check user notification is checked: user_chapter_announcements
    $queue = DrupalQueue::get('tm_event_announcements');
    $notif = new TMNotification("user_chapter_announcements");
    if ($notif->isEnabled($user_account->uid)) {

      $sendcount++;

      // add to user_chapter_announcements cron queue
      $queue->createItem(["uid" => $user_account->uid,
                        "email" => $user_account->mail, 
                        "subject" => $subject,
                        "message" => $message,
                        "event_image_src" => $event_image_src,
                        "event_image_link" => $event_image_link,
                        "replyto" => $replyto,
                        "eventid" => $eventid]);
      
      // below is the non queue method
      //tm_events_send_email($user_account->uid, $user_account->mail, $subject, $message, $event_image_src, $event_image_link, $eventid);
    }

  }

  // return sendcount
  print(json_encode(array("sent" => $sendcount)));

}

/**
 * Implements hook_cron_queue_info().
 */
function tm_events_cron_queue_info() {
  $queues = array();
  $queues['tm_event_announcements'] = array(
    'worker callback' => 'tm_events_send_email_cron', //function to call for each item
    'time' => 120, //seconds to spend working on the queue
  );
  return $queues;
}

/**
 * Cron call back to send announcement emails
 *
 */
function tm_events_send_email_cron($item) {
  tm_events_send_email($item["uid"], $item["email"], $item["subject"], $item["message"], $item["event_image_src"], $item["event_image_link"], $item["eventid"], $item["replyto"]);
}


/**
 * Sends out a single email
 */
function tm_events_send_email($uid, $email, $subject, $message, $event_image_src, $event_image_link, $eventid, $replyto="") {

	global $conf;

	$module = 'tm_events';
	$key = 'key';
	$language = language_default();
	$params = array();
	$send = FALSE;


	// STEP 1. LOAD HTML TEMPLATE
	$template_file = drupal_get_path('module', 'tm_notifications') . '/templates/tm_notifications-email_template.tpl.php';
	$template = file_get_contents($template_file);


	// STEP 2. SET MAIL HTML HEADERS
	$email_message = drupal_mail($module, $key, $email, $language, $params, NULL, $send);
	$email_message['headers']['MIME-Version'] = '1.0';
	$email_message['headers']['Content-Type'] = 'multipart/mixed;';
	$email_message['headers']['Content-Type'] = 'text/html;';


	// STEP 3. SET SUBJECT
	global $conf;
	$subject = str_replace("__SERVER_NAME__", $_SERVER["SERVER_NAME"], $subject);
	$subject = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $subject);  
	$subject = str_replace("__SITE_NAME__", $conf["tm_site_name"], $subject);  
	$email_message['subject'] = check_plain($subject);


	// STEP 4. REPLACE BODY AND FOOTER IN HTML TEMPLATE
	$template = str_replace("__EMAIL_BODY__", $message, $template);
	global $conf;
	$footer = $conf["tm_email_default_footer"];
	if (isset ($params['footer'])) {
	  $footer = $params['footer'];
	}
	$template = str_replace("__EMAIL_FOOTER__", $footer, $template);

  // STEP 5. ADD UNSUBSCRIBE LINK
  if ($uid != null) {
     $secret_hash = tm_notifications_generate_secret_hash($uid, "user_chapter_announcements");
     $unsubscribe_url = "user/" . $uid . "/unsubscribe/" . "user_chapter_announcements" . "/" . $secret_hash;
     $unsubscribe_link = "<a href='__SERVER_HTTP__://__SERVER_NAME__/" . $unsubscribe_url . "'>Unsubscribe from event announcements</a>";
  } else {
    $unsubscribe_link = "Update Your Notification Settings at <a href='__SERVER_HTTP__://__SERVER_NAME__'>__SERVER_NAME__</a>"; 
  }
  $template = str_replace("__UNSUBSCRIBE_LINK__", $unsubscribe_link, $template);


  $secret_hash = tm_notifications_generate_secret_hash($account->uid, $notification_name); 
  $tokens["UNSUBSCRIBE_NOTIFICATION_LINK"] = "user/" . $account->uid . "/unsubscribe/" . $notification_name . "/" . $secret_hash;
 
  $unsubscribe_link = "<a href='__SERVER_HTTP__://__SERVER_NAME__/" . $params['tokens']['UNSUBSCRIBE_NOTIFICATION_LINK'] . "'>Unsubscribe from this notification</a>";
  $template = str_replace("__UNSUBSCRIBE_LINK__", $unsubscribe_link, $template);

	// Other vars
  $template = str_replace("__SERVER_NAME__", $conf["tm_email_server_url_domain"], $template); 
	$template = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $template);  
	$template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $template);  
	$template = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $template);  
  $template = str_replace("__EMAIL_BRAND_MESSAGE__", "", $template); // disable brand message

  // Event image
  // Render with tm_events_render_hero_image_html()
  if ($event_image_src != null) {
    $event_image_html = tm_notifications_render_hero_image_html($event_image_src, $event_image_link, "Event image");
    $template = str_replace("__HERO_IMAGE__", $event_image_html, $template);
  } else {
    $template = str_replace("__HERO_IMAGE__", "", $template); // disable brand message
  }

	// STEP 5. LOAD HTML TEMPLATE INTO EMAIL BODY
	$email_message['body'] = $template;


	// STEP 6. SET REPLY TO
	// If blank, use tm_announcement_reply_email
	if ($replyto == "") {
	  $replyto = $conf["tm_announcement_reply_email"];
	}
	$email_message['headers']['Reply-To'] = $replyto;


	// STEP 6. SEND EMAIL
	// Retrieve the responsible implementation for this message.
	$system = drupal_mail_system($module, $key);

	// Format the message body.
	$format_message = $system->format($email_message);

	// Send e-mail.
	$result = $system->mail($format_message);

	// Send 1 email
	return 1;
}

/**
 * Send emails callback
 */
function tm_events_send_emails_ajax() {

  global $conf;
  global $user;

  if (!isset($_POST['subject'])) return FALSE;
  if (!isset($_POST['message'])) return FALSE;
  if (!isset($_POST['recipients'])) return FALSE;
  if (!isset($_POST['approved_members'])) return FALSE;
  if (!isset($_POST['eventid'])) return FALSE;
  if (!isset($_POST['chapterid'])) return FALSE;
  if (!isset($_POST['include_cover_image'])) return FALSE;
  if (!isset($_POST['message_format'])) return FALSE;

  $subject = $_POST['subject'];
  $message = $_POST['message'];
  $message .= "<p><small>(Event ID: " . $_POST['eventid'] . ")</small></p>";
  $recipients = $_POST['recipients'];
  $approved_members = ($_POST['approved_members'] == "1");
  $eventid = $_POST['eventid'];
  $chapterid = $_POST['chapterid'];
  $include_cover_image = ($_POST['include_cover_image'] == "1");
  $replyto = "";
  if (isset($_POST['replyto'])) { 
    $replyto = trim($_POST['replyto']);
  }
  $message_format = $_POST['message_format'];

  // If message format is text, add <br>'s
  if ($message_format == "text") {
    $message = str_replace("\n", "<br>", $message);
  }

  // Insert cover image
  // Check if tm_event_announcement_enable_cover_image is enabled
  $event_image_src = null;
  $event_image_link = null;
  if ($conf['tm_event_announcement_enable_cover_image'] == true) {
    if ($include_cover_image) {
      
      // load event
      $event = node_load($eventid);

      // get image url
      if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
        $event_image_src = image_style_url("banner", $event->field_image[LANGUAGE_NONE][0]['uri']);
      } else {
        $default_image_uri = _tm_events_get_default_image($conf["tm_images_default_field_image"], $event->nid);
        $event_image_src = image_style_url("banner", $default_image_uri);
      }

      // get event url
      $event_image_link = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . drupal_get_path_alias('node/' . $event->nid);

    }
  }

  // copy announcement email to a specific address
  if (isset($conf["tm_announcement_copy_email"])) {
      tm_events_send_email(null, $conf["tm_announcement_copy_email"], "[Copy] " . $subject, $message, $event_image_src, $event_image_link, $eventid, $replyto);
  }

  $sendcount = tm_events_send_emails($recipients, $subject, $message, $event_image_src, $event_image_link, $eventid, $chapterid, $replyto, $approved_members);

  // indicate success to the js ajax handler
  // todo: return a nicer json response and parse it in the js
  print($sendcount);
}

/**
 * Create event prefill text
 */
function tm_events_create_prefill_text($event, $chapter, $company, $use_html = true, $override_intro = "") {

  global $conf;
  global $user;

  // event url
  $event_url = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . drupal_get_path_alias('node/' . $event->nid);

  // event date
  $view = field_view_field('node',$event,'field_event_date',
    array('label'=>'hidden',));
  $date = render($view);

  $intro_text = "We're pleased to announce a new event, please see details below:";
  if (($chapter == null) && ($company == null) && (tm_events_check_published_chapter_leader($event, $user->uid) == null)) {
    $intro_text = "I'd like to invite you to my upcoming event:";
  }

  $prefill = "Hi [first_name],<br><br>";
  $prefill .= $intro_text . "<br>";
  $prefill .= "<ul>";
  $prefill .= "<li>" . t('Event').': '.$event->title . "</li>";
  $prefill .= "<li>" . t('Date').': '.strip_tags($date) . "</li>";
  if ($event->field_event_venue_name != null) {
    $prefill .= "<li>" . t('Venue').': '.$event->field_event_venue_name[LANGUAGE_NONE][0]['value'];
  }
  if ($event->field_location != null) {
    $location_text = $event->field_location[LANGUAGE_NONE][0]['value'];
    $location_text = str_replace("\r", ", ", $location_text);
    $prefill .= "<li>" . t('Location').': '. $location_text . "</li>";
  }

  // List sponsors
  if (isset($event->field_event_sponsors[LANGUAGE_NONE][0])) {
      $sponsor_links = array();
      foreach ($event->field_event_sponsors[LANGUAGE_NONE] as $arr) {
        $sponsor = node_load($arr['target_id']);
        $sponsor_profile_url = drupal_get_path_alias("node/" . $arr['target_id']);
        $external_url = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . $sponsor_profile_url;
        $sponsor_links[] = l(t($sponsor->title), $external_url);
      }
      $prefill .= "<li>Sponsors: " . implode(", ", $sponsor_links) . "</li>";
  }

  $prefill .= '<li>Register here: <a href="' . $event_url . '">' . $event_url .'</a></li>';

  // add contact link if html email
  if ($use_html) {
    $contact_url = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . drupal_get_path_alias('events/message/' . $event->nid);
    $prefill .= '<li>Questions about this event? <a href="' . $contact_url . '">Send a message</a></li>';
  }


  $prefill .= "</ul>";
  if ($event->body != null) {
    // add line breaks
    $event_message = str_replace("\n", "<br>", $event->body[LANGUAGE_NONE][0]['value']); 
    $prefill .=  $event_message . "<br>";
  }
  $prefill .= "<br>Cheers";

  // chapter signoff
  if (tm_events_check_published_chapter_leader($event, $user->uid) != null) {
    $chapter_id = tm_events_check_published_chapter_leader($event, $user->uid);
    $published_chapter = node_load($chapter_id);
    $prefill .= ",<br>- The " . $published_chapter->title . " Team";
  } elseif ($chapter != null) {
    $prefill .= ",<br>- The " . $chapter->title  . " Team";
  }

  // individual signup
  if (($chapter == null) && ($company == null) && (tm_events_check_published_chapter_leader($event, $user->uid) == null)) {
    $loaded = user_load($user->uid);
    $first_name = strip_tags($loaded->field_user_first_name[LANGUAGE_NONE][0]['value']);
    $last_name = strip_tags($loaded->field_user_last_name[LANGUAGE_NONE][0]['value']);
    $prefill .= ",<br>- " . $first_name . " " . $last_name;
  }
  
  if (!$use_html) {
    $prefill = str_replace("<br>", "\n", $prefill);
    $prefill = str_replace("<li>", "\n* ", $prefill);
    $prefill = str_replace("</ul>", "\n\n", $prefill);
    $prefill = strip_tags($prefill);

  }

  return $prefill;
}

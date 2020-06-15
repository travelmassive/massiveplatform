<?php

// tm_chapters.communication.inc - annoucement methods

/**
 * Callback wrapper for tm_chapters_communication_form()
 */
function tm_chapters_check_announcement_callback() {

  global $conf;
  global $user;
  $chapter_id = arg(2); // /chapters/action/1234
  $chapter = node_load($chapter_id);
  return tm_chapters_check_permission_send_announcement($user, $chapter);
}

/**
 * Check if user can send annoucement to chapter
 */
function tm_chapters_check_permission_send_announcement($account, $chapter) {

  global $conf;

  // check tm_chapters_announcements_enabled
  if (isset($conf['tm_chapters_announcements_enabled'])) {
    if ($conf['tm_chapters_announcements_enabled'] == false) {
      return false;
    }
  }

  // check if is moderator or admin
  $perm_is_moderator_or_admin = ((in_array("moderator", $account->roles)) || (in_array("administrator", $account->roles)));
  if ($perm_is_moderator_or_admin) {
    return true;
  }

  // check user is chapter leader of this chpter
  if (tm_users_is_chapter_leader($account->uid, $chapter->nid)) {
    return true;
  }

  // not allowed
  return false;
}


/**
 * Form for communicating with attendees
 */
function tm_chapters_communication_form($chapter) {

  $chapter = node_load(arg(2));
  $form = array();
  global $user;
  global $conf;

  // Check tm_chapters_announcements_enabled
  // If set, don't allow announcing to chapter
  if (isset($conf['tm_chapters_announcements_enabled'])) {
    if ($conf['tm_chapters_announcements_enabled'] == false) {
        drupal_set_message("Chapter announcements are not enabled", "warning");
        drupal_access_denied();
    }
  }

  // Permission variables
  $perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));

  // Add javascript for form
  drupal_add_js(drupal_get_path('module', 'tm_chapters') . '/js/tm_chapters_communication.js');

  // Add page title and page url to javascript
  $chapter_url = drupal_get_path_alias('node/'.$chapter->nid);
  $chapter_title = tm_chapters_formal_name($chapter);
  drupal_add_js(array('tm_chapters' => array('chapter_url' => $chapter_url, 'chapter_title' => $chapter_title)), array('type' => 'setting'));

  // Set chapter id in form
  $form['chapterid'] = array(
    '#type' => 'token',
    '#id' => 'chapterid',
    '#value' => $chapter->nid,
  );

  // Add intro help text
  if (isset($conf["tm_chapters_announcements_intro"])) {
    drupal_get_messages('help-tip'); // supress other help tips
    drupal_set_message($conf["tm_chapters_announcements_intro"], 'help-tip');
  }

  // Separator
  $form['step_1_text'] = array(
    '#markup' => '<span class="tm-chapter-send-announcement-step">Step 1. Create announcement</span>'
  );

  // Create subject
  $subject = t($conf['tm_chapters_announcements_subject_prefix']) . tm_chapters_formal_name($chapter);

  // Subject field
  $form['subject'] = array(
    '#title' => t('Subject '),
    '#type' =>'textfield',
    '#required' => TRUE,
    '#value' => $subject,
    '#description' => 'Write a short and creative subject line &mdash; 7 words or less will increase your open rate'
    );

  // Prefill announcement body text

  // Use html email
  $send_html_email = false;
  if (isset($conf['tm_chapters_announcements_allow_html'])) {
  	$send_html_email = $conf['tm_chapters_announcements_allow_html'];
  }

  $prefill = tm_chapter_announcement_create_prefill_text($chapter, $send_html_email);
  $form['body'] = array(
    '#type' =>'text_format',
    '#title' => t('Announcement message'),
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

  // Include chapter image
  $form['include_cover_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include chapter image'),
    '#default_value' => true
  );

  // Show preview of chapter image
  if (isset($chapter->field_image[LANGUAGE_NONE][0]['uri'])) {
    $image_url = image_style_url("banner", $chapter->field_image[LANGUAGE_NONE][0]['uri']);
  } else {
    $default_image_uri = _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $chapter->nid);
    $image_url = image_style_url("banner", $default_image_uri);
  }
  $form['include_cover_image']['#description'] = '<div style="margin-top: 1rem; margin-left: -1rem;"><img style="width: 150px; border: 1px solid #eee;" src="' . $image_url . '"/><br>Grab the attention of your members by including your chapter image.<br>It will be displayed above your announcement message.</div>';
  
  // Check tm_chapters_announcement_enable_cover_image is enabled
  if ($conf["tm_chapters_announcements_enable_cover_image"] == false) {
      $form['include_cover_image']['#type'] = 'hidden';
  }

  // If members's email on same domain, set reply to
  // Otherwise, leave blank
  $default_reply_to = "";
  if (stripos($user->mail, $conf["tm_email_server_url_domain"]) > 0) {
    $default_reply_to = $user->mail;
  }

  // Reply to address
  $form['reply_to'] = array(
    '#title' => t('Reply-to (Moderators only)'),
    '#type' =>'hidden',
    '#attributes' => array('id' => 'edit-reply-to', 'placeholder' => $conf["tm_announcement_reply_email"]),
    // '#id' => 'edit-reply-to',
    '#default_value' => $default_reply_to,
    '#description' => 'Any replies to this announcement will be sent here. Leave blank for default reply-to address.<br><strong>Important:</strong> Must be a <i>' . $conf["tm_email_server_url_domain"] . '</i> email to comply with Gmail\'s new sender rules.',
    '#rules' => array('email')
    );

  // Allow moderators, administrator to set reply-to
  if ($perm_is_moderator_or_admin) {
    $form['reply_to']['#type'] = 'textfield';
  }

  // Separator
  $form['step_2_text'] = array(
    '#markup' => '<div style="margin-top: 2rem; padding-top: 1rem;"><span class="tm-chapter-send-announcement-step">Step 2. Test your announcement</span></div>'
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
    '#id' => 'chapter-test-email-submit',
  );

  // Step 3
  $form['step_3_text'] = array(
    '#markup' => '<div style="margin-top: 2rem; padding-top: 1rem;"><span class="tm-chapter-send-announcement-step">Step 3. Send your announcement</span></div>'
  );

  // Get membership counts
  $membership_counts = _tm_chapters_get_membership_counts($chapter->nid);

  $options['chapter-all'] = tm_chapters_formal_name($chapter) . ' - All members (' . tm_events_pluralize_members($membership_counts["members_total"]) . ')';
  $options['chapter-approved'] = tm_chapters_formal_name($chapter) . ' - ' . tm_users_get_approved_member_label("plural") . ' (' . tm_events_pluralize_members($membership_counts["members_approved_total"]) . ')';
  $options['chapter-unapproved'] = tm_chapters_formal_name($chapter) . ' - ' . tm_users_get_unapproved_member_label("plural") . ' (' . tm_events_pluralize_members($membership_counts["members_unapproved_total"]) . ')';

  // Recipients
  $form['recipients'] = array(
    '#type' =>'select',
    '#options' => $options,
    '#title' => t('Send announcement to:'),
  );
  
  // Approved members
  $form['approved_members'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send to ' . tm_users_get_approved_member_label("plural") . ' only'),
    '#default_value' => false,
    '#type' => 'hidden'
  );

  // Confirm
  $form['confirm_send'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>I\'m ready.</strong> Have you tested your email? There\'s no undo or turning back from here!'),
    '#id' => 'chapter-email-members-confirm',
  );

  // send test email to this address (box, button, ajax)
  // and confirm popup when you hit the final submit
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Send Email To All Recipients'),
    '#id' => 'chapter-email-members-submit',
    '#attributes' => array('class' => array(t('bttn-success'))),
  );

  $help_text = '<div class="tm-chapters-send-announcement-tips">
  <h2>How to send a great announcement</h2>
  <ul style="list-style: none; padding-left: 0.5rem;">
  <li><span style="color: green; margin-right: 4px;">&#10004;</span> Write a meaningful subject line that captures people\'s attention</li>
  <li><span style="color: green; margin-right: 4px;">&#10004;</span> The best time to send an announcement is on a weekday between 10.00 and 11.00</li>
  <li><span style="color: red; margin-right: 4px;">&#10008;</span> Don\'t WRITE IN ALL CAPITALS (that\'s shouting)</li>
  <li><span style="color: red; margin-right: 4px;">&#10008;</span> Don\'t use many exclamation points!!!!!</li>
  <li><span style="color: red; margin-right: 4px;">&#10008;</span> Don\'t use spam trigger words like "free" or "click here"</li>
  </ul>
  <h2>Other things to know</h2><ul style="padding-left: 1.5rem;">';

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
function tm_chapters_test_email_ajax(){

  global $conf;
  global $user;

  if (!isset($_POST['subject'])) return FALSE;
  if (!isset($_POST['message'])) return FALSE;
  if (!isset($_POST['address'])) return FALSE;
  if (!isset($_POST['chapterid'])) return FALSE;
  if (!isset($_POST['first_name'])) return FALSE;
  if (!isset($_POST['include_cover_image'])) return FALSE;
  if (!isset($_POST['message_format'])) return FALSE;

  $subject = '[Test] ' . $_POST['subject'];
  $message = $_POST['message'];
  $message .= "<p><small>(Chapter ID: " . $_POST['chapterid'] . ")</small></p>";
  $email = $_POST['address'];
  $chapterid = $_POST['chapterid'];
  $include_cover_image = ($_POST['include_cover_image'] == "1");
  $first_name = $_POST['first_name'];
  $replyto = "";
  if (isset($_POST['replyto'])) { 
    $replyto = trim($_POST['replyto']);
  }
  $message_format = $_POST['message_format'];

  // Remove blank lines created in editor
  $message = str_replace("<p>&nbsp;</p>", "", $message);

  // Replace [first_name] token
  $message = str_replace("[first_name]", $first_name, $message);
  $subject = str_replace("[first_name]", $first_name, $subject);
  
  // If message format is text, add <br>'s
  if ($message_format == "text") {
    $message = str_replace("\n", "<br>", $message);
  }

  // Insert cover image
  // Check if tm_chapters_announcement_enable_cover_image is enabled
  $chapter_image_src = null;
  $chapter_image_link = null;
  if ($conf['tm_chapters_announcements_enable_cover_image'] == true) {
    if ($include_cover_image) {
      
      // load chapter
      $chapter = node_load($chapterid);

      // get image url
      if (isset($chapter->field_image[LANGUAGE_NONE][0]['uri'])) {
        $chapter_image_src = image_style_url("banner", $chapter->field_image[LANGUAGE_NONE][0]['uri']);
      } else {
        $default_image_uri = _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $chapter->nid);
        $chapter_image_src = image_style_url("banner", $default_image_uri);
      }

      // get chapter url
      $chapter_image_link = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $chapter->nid);

    }
  }
  
  $sendcount = tm_chapters_send_email(null, $email, $subject, $message, $chapter_image_src, $chapter_image_link, $chapterid, $replyto);

  // indicate success to the js ajax handler
  print(json_encode(array("sent" => $sendcount)));
}

/**
 * Submit function of communication ajax request
 */
function tm_chapters_send_emails($recipients, $subject, $message, $chapter_image_src, $chapter_image_link, $chapterid, $replyto = "", $approved_members = true){

  global $user;

  // permission check
  $chapter = node_load($chapterid);
  if (!tm_chapters_check_permission_send_announcement($user, $chapter)) {
    print(json_encode(array("sent" => 0)));
  }

  // Get recipients
  $user_ids = array();
  $flag = flag_get_flag('signup', NULL);
  $approved_role = user_role_load_by_name("approved user");

  // All chapter members
  if ($recipients == "chapter-all") {

    $query_sql = "SELECT uid FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id = :chapter_id";
    $results = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_id' => $chapterid));
    while ($result = $results->fetchAssoc()){
      $user_ids[] = $result['uid'];
    }
  }

  // All approved members
  if ($recipients == "chapter-approved") {

    $query_sql = "SELECT DISTINCT users_roles.uid AS uid FROM {flagging} f RIGHT JOIN users_roles ON users_roles.uid = f.uid WHERE users_roles.rid = :role_id AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id = :chapter_id";
    $results = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_id' => $chapterid, ':role_id' => $approved_role->rid));

    while ($result = $results->fetchAssoc()){
      $user_ids[] = $result['uid'];
    }
  }

  // All unapproved members
  if ($recipients == "chapter-unapproved") {

    // Get all members
    $all_user_ids = array();
    $query_sql = "SELECT uid FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id = :chapter_id";
    $results = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_id' => $chapterid));
    while ($result = $results->fetchAssoc()){
      $all_user_ids[] = $result['uid'];
    }

    // Get approved members
    $approved_user_ids = array();
    $query_sql = "SELECT DISTINCT users_roles.uid AS uid FROM {flagging} f RIGHT JOIN users_roles ON users_roles.uid = f.uid WHERE users_roles.rid = :role_id AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id = :chapter_id";
    $results = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_id' => $chapterid, ':role_id' => $approved_role->rid));

    while ($result = $results->fetchAssoc()){
      $approved_user_ids[] = $result['uid'];
    }

    // array diff all user ids an approved ids
    $user_ids = array_diff($all_user_ids, $approved_user_ids);

  }

  // Remove any duplicates
  $user_ids = array_unique($user_ids);

  // Send the emails
  // We use a cron queue tm_event_announcements_process to send emails in the background.
  $queue = DrupalQueue::get('tm_chapter_announcements_process');  
  $queue->createItem(["user_ids" => $user_ids,
                    "subject" => $subject,
                    "message" => $message,
                    "chapter_image_src" => $chapter_image_src,
                    "chapter_image_link" => $chapter_image_link,
                    "replyto" => $replyto,
                    "chapterid" => $chapterid]);

  // return number of recipients
  // note: we can't return how many people we sent to as this is now handled in a queue
  return sizeof($user_ids);

}

/**
 * Implements hook_cron_queue_info().
 */
function tm_chapters_cron_queue_info() {
  $queues = array();
  $queues['tm_chapter_announcements_process'] = array(
    'worker callback' => 'tm_chapters_process_emails_cron', //function to call for each item
    'time' => 600, //seconds to spend working on the queue (Ian - should be ok for up to 10,000 recipients)
  );
  $queues['tm_chapter_announcements'] = array(
    'worker callback' => 'tm_chapters_send_email_cron', //function to call for each item
    'time' => 120, //seconds to spend working on the queue
  );
  return $queues;
}

/**
 * Cron call back to send announcement emails
 * Wrapper to tm_chapters_process_emails_cron_run($item) with exception handling
 * 
 */
function tm_chapters_process_emails_cron($item) {

  try {
    tm_chapters_process_emails_cron_run($item);
  }
  catch (Exception $e) {
    // If the process fails, log and the queue handler will remove the item.
    try {
      watchdog_exception("tm_chapters", $e, "tm_chapters_process_emails_cron_run(): Queue item @item_id failed with message @message", array(
          '@item_id' => $item->item_id,
          '@message' => (string) $e,
        ), WATCHDOG_ERROR);
    } catch (Exception $e) {
      // do nothing
    }
  }
}

/**
 * Cron call back to send announcement emails
 *
 */
function tm_chapters_process_emails_cron_run($item) {

  global $conf;
  
  // Get data
  $user_ids = $item["user_ids"];
  $subject = $item["subject"];
  $message = $item["message"];
  $chapter_image_src = $item["chapter_image_src"];
  $chapter_image_link = $item["chapter_image_link"];
  $replyto = $item["replyto"];
  $chapterid = $item["chapterid"];

  // Process emails
  $original_message = $message;
  $original_subject = $subject;
  $sendcount = 0;
  foreach ($user_ids as $user_id) {

    $user_account = user_load($user_id);

    // check that account exists
    if ($user_account == false) {
      continue; 
    }

    // if approved_members then check approval status of user
    if ($approved_members) {
      if (!in_array('approved user', $user_account->roles)) {
        continue; // skip this member, they are not approved
      }
    }

    // replace [first_name] token
    $first_name = $user_account->field_user_first_name[LANGUAGE_NONE][0]['value'];
    $message = str_replace("[first_name]", $first_name, $original_message);
    $subject = str_replace("[first_name]", $first_name, $original_subject);

    // Check user notification is checked: user_chapter_announcements
    $queue = DrupalQueue::get('tm_chapter_announcements');
    $notif = new TMNotification("user_chapter_announcements");
    if ($notif->isEnabled($user_account->uid)) {

      $sendcount++;

      // add to user_chapter_announcements cron queue
      $queue->createItem(["uid" => $user_account->uid,
                        "email" => $user_account->mail, 
                        "subject" => $subject,
                        "message" => $message,
                        "chapter_image_src" => $chapter_image_src,
                        "chapter_image_link" => $chapter_image_link,
                        "replyto" => $replyto,
                        "chapterid" => $chapterid]);
    }

  }

  // copy announcement email to a specific address
  // include email sent count
  if (isset($conf["tm_announcement_copy_email"])) {
      tm_chapters_send_email(null, $conf["tm_announcement_copy_email"], "[Copy] " . $original_subject . " (" . $sendcount  ." sent)", $original_message, $chapter_image_src, $chapter_image_link, $chapterid, $replyto);
  }

}

/**
 * Cron call back to send announcement emails
 * Wrapper to tm_chapters_send_email_cron_run($item) with exception handling
 * 
 */
function tm_chapters_send_email_cron($item) {

  try {
    tm_chapters_send_email_cron_run($item);
  }
  catch (Exception $e) {
    // If the process fails, log and the queue handler will remove the item.
    try {
      watchdog_exception("tm_chapters", $e, "tm_chapters_send_email_cron_run(): Queue item @item_id failed with message @message", array(
          '@item_id' => $item->item_id,
          '@message' => (string) $e,
        ), WATCHDOG_ERROR);
    } catch (Exception $e) {
      // do nothing
    }
  }
}

/**
 * Cron call back to send announcement emails
 *
 */
function tm_chapters_send_email_cron_run($item) {
  tm_chapters_send_email($item["uid"], $item["email"], $item["subject"], $item["message"], $item["chapter_image_src"], $item["chapter_image_link"], $item["chapterid"], $item["replyto"]);
}


/**
 * Sends out a single email
 */
function tm_chapters_send_email($uid, $email, $subject, $message, $chapter_image_src, $chapter_image_link, $chapterid, $replyto="") {

	global $conf;

	$module = 'tm_chapters';
	$key = 'key';
	$language = language_default();
	$params = array();
	$send = FALSE;


	// STEP 1. LOAD HTML TEMPLATE
	$template_file_name = 'tm_notifications-email_template.tpl.php';
  if (isset($conf["tm_email_custom_template"])) {
    $template_file_name = $conf["tm_email_custom_template"];
  }
  $template_file = drupal_get_path('module', 'tm_notifications') . '/templates/' . $template_file_name;
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
     $unsubscribe_link = "<a style='style='text-decoration: underline; color: #9B9B9B;' href='__SERVER_HTTP__://__SERVER_NAME__/" . $unsubscribe_url . "'>Unsubscribe from chapter announcements</a>";
  } else {
    $unsubscribe_link = "Update Your Notification Settings at <a style='style='text-decoration: underline; color: #9B9B9B;' href='__SERVER_HTTP__://__SERVER_NAME__'>__SERVER_NAME__</a></span>"; 
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
  $template = str_replace("__HERO_HEADLINE__", "", $template); // disable hero headline
  $template = str_replace("__PREHEADER_TEXT__", "", $template); // disable preheader text

  // Chapter image
  // Render with tm_chapters_render_hero_image_html()
  if ($chapter_image_src != null) {
    $chapter_image_html = tm_notifications_render_hero_image_html($chapter_image_src, $chapter_image_link, "Chapter image");
    $template = str_replace("__HERO_IMAGE__", $chapter_image_html, $template);
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
function tm_chapters_send_emails_ajax() {

  global $conf;
  global $user;

  if (!isset($_POST['subject'])) return FALSE;
  if (!isset($_POST['message'])) return FALSE;
  if (!isset($_POST['recipients'])) return FALSE;
  if (!isset($_POST['approved_members'])) return FALSE;
  if (!isset($_POST['chapterid'])) return FALSE;
  if (!isset($_POST['include_cover_image'])) return FALSE;
  if (!isset($_POST['message_format'])) return FALSE;

  $subject = $_POST['subject'];
  $message = $_POST['message'];
  $message .= "<p><small>(Chapter ID: " . $_POST['chapterid'] . ")</small></p>";
  $recipients = $_POST['recipients'];
  $approved_members = ($_POST['approved_members'] == "1");
  $chapterid = $_POST['chapterid'];
  $include_cover_image = ($_POST['include_cover_image'] == "1");
  $replyto = "";
  if (isset($_POST['replyto'])) { 
    $replyto = trim($_POST['replyto']);
  }
  $message_format = $_POST['message_format'];

  // Remove blank lines created in editor
  $message = str_replace("<p>&nbsp;</p>", "", $message);

  // If message format is text, add <br>'s
  if ($message_format == "text") {
    $message = str_replace("\n", "<br>", $message);
  }

  // Insert cover image
  // Check if tm_chapters_announcement_enable_cover_image is enabled
  $chapter_image_src = null;
  $chapter_image_link = null;
  if ($conf['tm_chapters_announcements_enable_cover_image'] == true) {
    if ($include_cover_image) {
      
      // load chapter
      $chapter = node_load($chapterid);

      // get image url
      if (isset($chapter->field_image[LANGUAGE_NONE][0]['uri'])) {
        $chapter_image_src = image_style_url("banner", $chapter->field_image[LANGUAGE_NONE][0]['uri']);
      } else {
        $default_image_uri = _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $chapter->nid);
        $chapter_image_src = image_style_url("banner", $default_image_uri);
      }

      // get chapter url
      $chapter_image_link = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $chapter->nid);

    }
  }

  $sendcount = tm_chapters_send_emails($recipients, $subject, $message, $chapter_image_src, $chapter_image_link, $chapterid, $replyto, $approved_members);

  // flag announcement last sent
  flag('unflag', 'announcement_last_sent', $chapterid, $user);
  flag('flag', 'announcement_last_sent', $chapterid, $user);

  // indicate success to the js ajax handler
  // todo: return a nicer json response and parse it in the js
  print(json_encode(array("sent" => $sendcount)));

}

/**
 * Create chapter prefill text
 */
function tm_chapter_announcement_create_prefill_text($chapter, $use_html = true, $override_intro = "") {

  global $conf;
  global $user;

  $prefill = $conf["tm_chapters_announcements_prefill"];

  // Sign off
  $prefill .= $conf["tm_chapters_announcements_signoff"];
  $prefill .= "<i>&mdash; The " . tm_chapters_formal_name($chapter) . " Team";

  // Show chapter leader names
  $leader_uids = array();
  if (isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
    foreach ($chapter->field_chapter_leaders[LANGUAGE_NONE] as $chapter_leader) {
      $leader_uids[] = $chapter_leader['target_id'];
    }
  }

  $chapter_leaders_names = array();
  foreach ($leader_uids as $leader_uid) {
    $chapter_leader = user_load($leader_uid);
    $chapter_leaders_names[] = strip_tags($chapter_leader->field_user_first_name[LANGUAGE_NONE][0]['value']); 
  }
  if (sizeof($chapter_leaders_names) > 0) {
    $last = array_pop($chapter_leaders_names);
    $prefill .= " (" . (count($chapter_leaders_names) ? implode(", ", $chapter_leaders_names) . " and " . $last : $last) . ")";
  }
  $prefill .= "</i>";

  // chapter url
  $chapter_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $chapter->nid);

  // Fetch the current date  and make sure it's UTC and user-defined timezone.
  $current_date = new DateTime('now', new DateTimeZone('UTC'));

  // Fetch the nearest upcoming event that references a particular chapter
  if ($use_html) {

    $previous_event = null;
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'event')
      ->propertyCondition('status', 1)
      ->fieldCondition('field_chapter', 'target_id', $chapter->nid)
      ->fieldCondition('field_event_date', 'value2', $current_date->format('Y-m-d H:i:s'), '>=')
      ->fieldOrderBy('field_event_date', 'value2', 'ASC');
    $results = $query->execute();

    foreach ($results as $result) {
     foreach($result as $nid => $obj) {
      $event = node_load($nid);

      // ignore preview mode
      if ($event->field_event_is_preview_mode[LANGUAGE_NONE][0]['value'] == 1) {
        continue;
      }

      if ($event != null) {

        if ($previous_event == null) {
          $prefill .= "<br><br>PS: Join our upcoming events<ul>";
        }

        // event date
        $event_date = strtotime($event->field_event_date[LANGUAGE_NONE][0]['value']);
        $event_date_str = date('l j F Y', $event_date);

        // event url
        $event_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $event->nid);

        // text
        $prefill .= '<li><a href="' . $event_url . '">' . $event->title . "</a>, " . $event_date_str . "</li>";

        $previous_event = $event;
        
      }
     }
    }
    if ($previous_event != null) {
      $prefill .= "\n</ul>";
    }
  }

  if (!$use_html) {
    $prefill = str_replace("<p>", "", $prefill);
    $prefill = str_replace("</p>", "\n\n", $prefill);
    $prefill = str_replace("<br>", "\n", $prefill);
    $prefill = str_replace("<li>", "\n* ", $prefill);
    $prefill = str_replace("</ul>", "\n\n", $prefill);
    $prefill = strip_tags($prefill);

  }

  return $prefill;
}

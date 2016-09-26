<?php

// tm_users.mail.inc - mail methods

/** 
 * Implement hook_mail_alter()
 * Override some system emails
 * We send an html email with the reply-to address set
 * We also stop the default contact email from being sent.
 */
function tm_users_mail_alter(&$mail_form) {

  // Verify email an dd reset password
  if (($mail_form['id'] == 'user_register_no_approval_required') or ($mail_form['id'] == 'user_password_reset')) {
    _tm_users_mail_alter_reset_password($mail_form);
  }

  // Site contact page
  if ($mail_form['id'] == 'contact_page_mail') {
    _tm_users_mail_alter_contact_page_mail($mail_form);
  }

}

/**
 * Sent reset password mail
 */
function _tm_users_mail_alter_reset_password(&$mail_form) {
    
  global $user;
  global $conf;

  $module = 'tm_users';
  $key = $mail_form['key'];
  $language = language_default();
  $params = $mail_form['params'];
  $to = $mail_form['to'];
  $email = $mail_form['to'];
  $account = $mail_form['params']['account'];
  $first_name = $account->field_user_first_name[LANGUAGE_NONE][0]['value'];
  $last_name = $account->field_user_last_name[LANGUAGE_NONE][0]['value'];

  // SET SUBJECT
  //$subject = $mail_form['subject'];
  if ($mail_form['id'] == 'user_register_no_approval_required') {
    $subject = "Confirm your " . $conf["tm_site_name"] . " account, " . $first_name . " " . $last_name;

  } elseif ($mail_form['id'] == 'user_password_reset') {
    $subject = "Recover your " . $conf["tm_site_name"] . " login";
  }

  $message = $mail_form['body']['0'];

  // SEND EMAIL
  tm_users_send_html_mail($module, $key, $email, $language, $params, $subject, $message, NULL);

  // Don't send via drupal mail
  $mail_form['send'] = false;
}


/**
 * Sent site contact page email
 */
function _tm_users_mail_alter_contact_page_mail(&$mail_form) {
    
  global $user;
  global $conf;

  $module = 'tm_users';
  $key = 'key';
  $language = language_default();
  $params = array();
  $reply = $mail_form['params']['mail'];
  $email = $conf['tm_contact_page_email'];

  // ANTI SPAM
  if (isset($mail_form['params']['fullname'])) {
    if ($mail_form['params']['fullname'] != "") {
      drupal_set_message("There was a problem sending your message. Please email us at " . $conf["tm_contact_page_email"], "warning");
      drupal_goto("contact");
    }
  }

  // DETERMINE CONTACT

  // Get contact id
  $contact_id = "";
  $contact_re = "";
  if (isset($mail_form['params']['contact_list'])) {
    $contact_id = $mail_form['params']['contact_list'];
  }

  // Default contact
  $email = $conf['tm_contact_page_email'];

  // Specific contact
  if ($contact_id != "") {

    // send to chapter leaders
    if (is_numeric($contact_id)) {
      $chapter = node_load($contact_id);
      if ($chapter) {
        $contact_re = tm_chapters_formal_name($chapter);
      }
      if ($chapter and isset($chapter->field_chapter_leaders[$chapter->language])) {

          // get chapter leaders emails
          $chapter_leader_emails = array();
          foreach($chapter->field_chapter_leaders[$chapter->language] as $leader){
              $chapterleader = user_load($leader['target_id']);
              $chapter_leader_emails[] = $chapterleader->mail;
          }
          $chapter_leader_emails = array_unique($chapter_leader_emails);

          // send to chapter leaders
          $email = implode(",", $chapter_leader_emails);

          // cc default contact address
          $params["headers"] = array("Cc" => $conf['tm_contact_page_email']);
      }

    } else {

      // send to contact address
      if (isset($conf["tm_contact_page_addresses"][$contact_id])) {
        $email = $conf["tm_contact_page_addresses"][$contact_id];
        $params["headers"] = array("Bcc" => $conf['tm_contact_page_email']); // bcc
        if (isset($conf["tm_contact_page_contacts"][$contact_id])) {
          $contact_re = $conf["tm_contact_page_contacts"][$contact_id];
        }
      }
    }
  }

  // CONSTRUCT MESSAGE
  $member_url = url('user/'.$user->uid, array('absolute'=>TRUE));
  $message = "Hi, a member just sent a message via the site contact form:";
  $message .= "<br>";
  if ($contact_re != "") {
    $message .= "<br>Attention: " . $contact_re;
  }
  $message .= "<br>Name: " . $mail_form['params']['name'];
  $message .= "<br>Subject: " . check_plain($mail_form['params']['subject']);
  $message .= "<p><strong>Message:</strong><br>";
  $custom_message = strip_tags(check_plain($mail_form['params']['message'])); // remove html
  $message .= str_replace("\n", "<br>", $custom_message); // add line breaks
  $message .= "</p>";
  if (user_is_logged_in()) {
      $message .= "Member's profile: <a href='" . $member_url . "'>" . $member_url . "</a><br>";
  } 
  $message .= "Reply email: " . check_plain($mail_form['params']['mail']);
  $message .= "<br>";
  $message .= "<br>To reply directly, just reply to this email.";
  $message .= "<br>";
  $message .= "<br>__EMAIL_SIGNOFF__";

  // SUBJECT
  $subject = "Contact Form: " . check_plain($mail_form['params']['subject']);

  // SEND EMAIL
  tm_users_send_html_mail($module, $key, $email, $language, $params, $subject, $message, $reply);

  // Don't send via drupal mail
  $mail_form['send'] = false;
  
}


/* Helper method
 * Send html email
 * Note for system messages you need to set $mail_form['send'] = false;
 */
function tm_users_send_html_mail($module, $key, $email, $language, $params, $subject, $message, $reply = NULL) {

  global $conf;

  // STEP 1. LOAD HTML TEMPLATE
  $template_file = drupal_get_path('module', 'tm_notifications') . '/templates/tm_notifications-email_template.tpl.php';
  $template = file_get_contents($template_file);

  // STEP 2. SET MAIL HTML HEADERS
  $email_message = drupal_mail($module, $key, $email, $language, $params, NULL, FALSE);
  $email_message['headers']['MIME-Version'] = '1.0';
  $email_message['headers']['Content-Type'] = 'multipart/mixed;';
  $email_message['headers']['Content-Type'] = 'text/html;';
  if ($reply != NULL) {
    $email_message['headers']['Reply-to'] = $reply; // set reply-to to the user
  }

  // STEP 3. SET SUBJECT 
  $email_message['subject'] = $subject;

  // STEP 4. REPLACE BODY AND FOOTER IN HTML TEMPLATE
  $template = str_replace("__EMAIL_BODY__", $message, $template);
  $footer = $conf["tm_email_default_footer"];
  $template = str_replace("__EMAIL_FOOTER__", $footer, $template);

  // Other vars
  $unsubscribe_link = "Update Your Notification Settings at <a href='__SERVER_HTTP__://__SERVER_NAME__'>__SERVER_NAME__</a>"; 
  $template = str_replace("__UNSUBSCRIBE_LINK__", $unsubscribe_link, $template);
  $template = str_replace("__SERVER_NAME__", $_SERVER["SERVER_NAME"], $template); 
  $template = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $template);  
  $template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $template);  
  $template = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $template); 
  $template = str_replace("__EMAIL_BRAND_MESSAGE__", "", $template); // disable brand message
  $template = str_replace("__HERO_IMAGE__", "", $template); // remove hero image

  // STEP 5. LOAD HTML TEMPLATE INTO EMAIL BODY
  $email_message['body'] = $template;

  // STEP 6. SEND EMAIL
  // Retrieve the responsible implementation for this message.
  $system = drupal_mail_system($module, $key);

  // Format the message body.
  $format_message = $system->format($email_message);

  // Send e-mail.
  $result = $system->mail($format_message);

  // Don't send via drupal mail
  $mail_form['send'] = false;

  // Suppress help tip
  drupal_get_messages("help-tip");

}

<?php

// tm_chapters.contact.inc - contact page methods

/**
 * Form for chapter contact form
 */
function tm_chapters_contact_form(){

  $form['subject'] = array(
    '#title' => t('Subject'),
    '#type' =>'textfield',
    '#required' => TRUE,
    );
  $form['body'] = array(
    '#type' =>'textarea',
    '#title' => t('Message'),
    '#rows' => 10,
    '#required' => TRUE,
    );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Send message'),
    '#id' => 'edit-submit'  
  );

  global $conf;
  $contact_form_message = "Get in touch with your chapter leaders to provide feedback, ideas or questions.<br>Please follow our <a target='_blank' href='" . $conf["tm_community_values_url"] ."'>community guidelines</a> and use this contact page responsibly.";
  drupal_set_message($contact_form_message, "help-tip");

  return $form;
}

/**
 * Contact chapter leaders form submit
 */
function tm_chapters_contact_form_submit($form, &$form_state) {

  global $user;
  global $conf;
  $nid = _orig_nid();

  if ($nid == 0) {
    print drupal_not_found();
    return;
  }

  $chapter = node_load($nid);
  $account = user_load($user->uid);

  if ($chapter) {
    foreach($chapter->field_chapter_leaders[$chapter->language] as $leader){
      $chapterleader = user_load($leader['target_id']);
      tm_chapters_send_email($chapterleader->mail, $form_state['values']['subject'], $form_state['values']['body'], $account, $chapter);
    }

    drupal_set_message(t('Your message has been sent.'));
    drupal_goto('node/'.$chapter->nid);
  }
}

/**
 * Email sending function
 */
function tm_chapters_send_email($email, $subject, $body, $user, $chapter){

    global $conf;

    // Get Users Name
    if (empty($user->language)) $language = LANGUAGE_NONE;
    else $language = $user->language;
    $name = $user->name;
    if (isset($user->field_user_first_name[$language])){
      $name = $user->field_user_first_name[$language][0]['value'];
      if (isset($user->field_user_first_name[$language])){
        $name .= ' '.$user->field_user_last_name[$language][0]['value'];
      }
    }

    // SEND EMAIL
    $module = 'tm_chapters';
    $key = 'key';
    $language = language_default();
    $params = array();
    $from = NULL;
    $reply = $user->mail;
    $send = FALSE;
    

    // CONSTRUCT MESSAGE
    $member_url = url('user/'.$user->uid, array('absolute'=>TRUE));
    $chapter_url = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . drupal_get_path_alias('node/' . $chapter->nid);

    $message = "Hi, a member just sent a message via your Chapter contact form:";
    $message .= "<br>";
    $message .= "<br><i>Chapter: <a href='" . $chapter_url . "'>" . $chapter->title . "</a></i>";
    $message .= "<br>";
    $message .= "<br>Name: " . $name;
    $message .= "<br>Subject: " . check_plain($subject);
    $message .= "<br>Message: " . check_plain($body);
    $message .= "<br>Member's profile: <a href='" . $member_url . "'>" . $member_url . "</a>";
    $message .= "<br>Member's email: " . $reply;
    $message .= "<br>";
    $message .= "<br>To respond, just reply to this email.";
    $message .= "<br>";
    $message .= "<br>__EMAIL_SIGNOFF__";

    // STEP 1. LOAD HTML TEMPLATE
    $template_file = drupal_get_path('module', 'tm_notifications') . '/templates/tm_notifications-email_template.tpl.php';
    $template = file_get_contents($template_file);


    // STEP 2. SET MAIL HTML HEADERS
    $email_message = drupal_mail($module, $key, $email, $language, $params, NULL, $send);
    $email_message['headers']['MIME-Version'] = '1.0';
    $email_message['headers']['Content-Type'] = 'multipart/mixed;';
    $email_message['headers']['Content-Type'] = 'text/html;';
    $email_message['headers']['Reply-to'] = $reply; // set reply-to to the user

    // STEP 3. SET SUBJECT 
    $email_message['subject'] = "Contact Form: " . check_plain($subject);

    // STEP 4. REPLACE BODY AND FOOTER IN HTML TEMPLATE
    $template = str_replace("__EMAIL_BODY__", $message, $template);
    $footer = $conf["tm_email_default_footer"];
    if (isset ($params['footer'])) {
      $footer = $params['footer'];
    }
    $template = str_replace("__EMAIL_FOOTER__", $footer, $template);

    // Other vars
    $template = str_replace("__SERVER_NAME__", $_SERVER["SERVER_NAME"], $template); 
    $template = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $template);  
    $template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $template);  
    $template = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $template); 

    // STEP 5. LOAD HTML TEMPLATE INTO EMAIL BODY
    $email_message['body'] = $template;

    // STEP 6. SEND EMAIL
    // Retrieve the responsible implementation for this message.
    $system = drupal_mail_system($module, $key);

    // Format the message body.
    $format_message = $system->format($email_message);

    // Send e-mail.
    $result = $system->mail($format_message);
}

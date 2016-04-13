<?php

// tm_events.contact.inc - contact page methods

/** 
 * Form for event contact form 
 */
function tm_events_contact_form(){


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
  $contact_form_message = "Get in touch with the event organizers to ask a question or provide feedback.";
  drupal_set_message($contact_form_message, "help-tip");

  return $form;
}

/** 
 * Contact chapter leaders form submit 
 */
function tm_events_contact_form_submit($form, &$form_state) {

  global $user;
  global $conf;
  $nid = arg(2); // ie: /events/contact/13784

  $event = node_load($nid);
  if ($event->type != 'event') {
    print drupal_not_found();
    return;
  }

  $event = node_load($nid);
  $account = user_load($user->uid);

  if ($event) {

    $notify_ids = [];

    // case 1: event with organizers -> organizers
    if (sizeof($event->field_organizer) > 0) {
      foreach ($event->field_organizer['und'] as $organizer) {
        $notify_ids[] = $organizer['target_id'];
      }
    }

    // case 2: event with no organizers and a chapter -> chapter leaders
    if ((sizeof($event->field_organizer) == 0) and (sizeof($event->field_chapter) > 0)) {

      if (isset($event->field_chapter['und'])) {
        $chapter_id = $event->field_chapter['und'][0]['target_id'];
        $chapter = node_load($chapter_id);
      
        foreach($chapter->field_chapter_leaders['und'] as $chapter_leader) {
          $notify_ids[] = $chapter_leader['target_id'];
        }
      }
    }

    // case 3: event with no organizers and no chapter -> event creator
    if ((sizeof($event->field_organizer) == 0) and (sizeof($event->field_chapter) == 0)) {
        $notify_ids[] = $event->uid;
    }

    // case 4: event with no organizers and a company -> company managers
    if ((sizeof($event->field_organizer) == 0) and (sizeof($event->field_event_company) > 0)) {

      if (isset($event->field_event_company['und'])) {
        $company_id = $event->field_event_company['und'][0]['target_id'];
        $company = node_load($company_id);
      
        foreach ($company->field_owner[LANGUAGE_NONE] as $profile_owner) {
          $notify_ids[] = $profile_owner['target_id'];
        }
      }
    }

    // in case duplicate organiser are set - remove duplicate notify_ids
    $notify_ids = array_unique($notify_ids);

    foreach($notify_ids as $notify_id){
      $event_organizer = user_load($notify_id);
      tm_events_send_contact_email($event_organizer->mail, $form_state['values']['subject'], $form_state['values']['body'], $account, $event);
    }

    drupal_set_message(t('Your message has been sent.'));
    drupal_goto('node/'.$event->nid);
  }
}

/** 
 * Email sending function 
 */
function tm_events_send_contact_email($email, $subject, $body, $user, $event){

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
  $module = 'tm_events';
  $key = 'key';
  $language = language_default();
  $params = array();
  $from = NULL;
  $reply = $user->mail;
  $send = FALSE;
  

  // CONSTRUCT MESSAGE
  $member_url = url('user/'.$user->uid, array('absolute'=>TRUE));
  $event_url = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . drupal_get_path_alias('node/' . $event->nid);

  $message = "Hi, a member just sent a message via your Event contact form:";
  $message .= "<br>";
  $message .= "<br><i>Event: <a href='" . $event_url . "'>" . $event->title . "</a></i>";
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

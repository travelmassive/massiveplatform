<?php
/**
 * @file tm_notifications.admin.pages.inc
 * 
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */
 
/**
 * Admin form to setup the notifications.
 */
function tm_notifications_messages_admin_form($form, &$form_state) {
  $form['introduction'] = array(
    '#type' => 'item',
    '#title' => t('Notification Messages'),
    '#description' => t('On this page you can edit the custom Travel Massive notifications and reminders.') . '<br/>' . 
      t('If you are searching for the standard Drupal messages (email on user creation for example), !link', array('!link' => l(t('click here'), 'admin/config/people/accounts'))),
  );
  
  $notifications = module_invoke_all('tm_notif_info');
  
  if (empty($notifications)) {
    $form['no-notifications'] = array(
      '#markup' => t('No notifications found.')
    );
  }
  else {
    
    $form['notifications'] = array(
      '#type' => 'vertical_tabs',
    );
    
    foreach ($notifications as $name => $value) {
      
      $notif = new TMNotification($name);
      $key = $notif->getVarName();

      $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => $value['title'],
        '#collapsible' => TRUE,
        '#description' => $value['description'],
        '#group' => 'notifications',
        '#tree' => TRUE
      );
      
      $form[$key]['user_label'] = array(
        '#type' => 'textfield',
        '#title' => t('User label'),
        '#description' => t('The label to show the user for this notification.'),
        '#default_value' => $notif->getLabel($value['title']),
      );
      
      $form[$key]['change_allowed'] = array(
        '#type' => 'checkbox',
        '#title' => t('Is change allowed?'),
        '#description' => t('Whether the users can subscribe/unsubscribe to this notification.'),
        '#default_value' => $notif->isChangeAllowed(),
      );
      
      $form[$key]['default_state'] = array(
        '#type' => 'radios',
        '#title' => t('Default state'),
        '#description' => t('The default state of the notification.'),
        '#options' => array(
          0 => t('Unsubscribed'),
          1 => t('Subscribed')
        ),
        '#default_value' => $notif->getDefaultState(),
      );
      
       $form[$key]['email'] = array(
         '#type' => 'fieldset',
         '#collapsible' => FALSE,
         '#title' => t('Email settings'),
       );
      
      $form[$key]['email']['subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#description' => t('The email subject.'),
        '#default_value' => $notif->getEmailSubject(),
      );
      
      $form[$key]['email']['message'] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#description' => t('The email body.'),
        '#default_value' => $notif->getEmailMessage(),
      );
      
      $form[$key]['email']['use_cron'] = array(
        '#type' => 'checkbox',
        '#title' => t('Send through cron'),
        '#description' => t('If this notification is likely to be sent to multiple users simultaneously thick the checkbox. The emails will be sent during cron run preventing timeouts.'),
        '#default_value' => $notif->isCronEmail(),
      );
      
      if (isset($value['tokens'])) {
        $form[$key]['email']['subject']['#element_validate'] = array('token_element_validate');
        $form[$key]['email']['subject']['#token_types'] = $value['tokens']['token_types'];
        $form[$key]['email']['message']['#element_validate'] = array('token_element_validate');
        $form[$key]['email']['message']['#token_types'] = $value['tokens']['token_types'];
        
        $form[$key]['email']['tokens'] = array(
          '#markup' => theme('token_tree_link', $value['tokens'])
        );
      }
    }

    $form['#submit'][] = 'tm_notifications_messages_admin_form_submit';
  }

  return system_settings_form($form);
}

/**
 * Additional submit hook tm_notifications_messages_admin_form.
 * Responsible for updating the queue_mail_keys when the use cron checkbox
 * is checked.
 */
function tm_notifications_messages_admin_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $mail_keys = variable_get('queue_mail_keys', '');
  
  // If the wildcard for the module is set, all notifications will
  // be sent through cron regardless of the checkbox.
  // Users should not add this to queue_mail but if they do it's covered.
  if (!preg_match('/tm_notifications_\*/', $mail_keys)) {
    // Add the the notifications with send through cron to queue_mail
    foreach ($form_state['values'] as $key => $value) {
      if (is_array($value)) {
        // The key used by the module is modulename__mailkey.
        // this needs to be translated to modulename_mailkey
        $m_key = preg_replace('/tm_notifications__/', 'tm_notifications_', $key);
        
        if ($value['email']['use_cron']) {
          // Add to queue_mail.
          // Check if not already there.
          if (!drupal_match_path($m_key, $mail_keys)) {
            $mail_keys .= "\n" . $m_key;
          }
        }
        else {
          // Remove from queue_mail.
          $mail_keys = preg_replace('/' . $m_key . '(\r\n?|\n)?/', '', $mail_keys);
        }
      }
    }
    variable_set('queue_mail_keys', $mail_keys);
  }
}

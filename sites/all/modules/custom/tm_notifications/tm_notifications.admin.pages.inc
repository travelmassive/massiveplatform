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
      t('If you are searching for the standard Drupal messages (email on user creation for example), !link.', array('!link' => l(t('click here'), 'admin/config/people/accounts'))),
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
        '#title' => t('Notification label'),
        '#description' => t('This label will be shown to the users on their notification preferences page.'),
        '#default_value' => $notif->getLabel($value['title']),
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
      
      $form[$key]['change_allowed'] = array(
        '#type' => 'checkbox',
        '#title' => t('Can users change the state?'),
        '#description' => t('Whether the users can subscribe/unsubscribe to this notification.'),
        '#default_value' => $notif->isChangeAllowed(),
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
  }

  return system_settings_form($form);
}
<?php
/**
 * @file tm_notifications.pages
 * 
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */
 
/**
 * Admin form.
 * By default shows a message stating that there are no notifications.
 * Every notification should hide this message, to ensue that it is not present:
 * $form['no-notifications']['#access'] = FALSE;
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
      '#markup' => t('No notifications were created.')
    );
  }
  else {
    
    $form['notifications'] = array(
      '#type' => 'vertical_tabs',
    );
    
    foreach ($notifications as $key => $value) {
      $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => $value['title'],
        '#collapsible' => TRUE,
        '#description' => $value['description'],
        '#group' => 'notifications',
      );
      
      $form[$key]['tm_notifications-' . $key . '-subject'] = array(
        '#type' => 'textfield',
        '#title' => isset($value['subject']['title']) ? $value['subject']['title'] : t('Subject'),
        '#description' => isset($value['subject']['description']) ? $value['subject']['description'] : t('The email subject.'),
        '#default_value' => variable_get('tm_notifications-' . $key . '-subject', ''),
      );
      
      $form[$key]['tm_notifications-' . $key . '-message'] = array(
        '#type' => 'textarea',
        '#title' => isset($value['body']['title']) ? $value['subject']['title'] : t('Body'),
        '#description' => isset($value['body']['description']) ? $value['body']['description'] : t('The email body.'),
        '#default_value' => variable_get('tm_notifications-' . $key . '-message', ''),
      );
      
      if (isset($value['tokens'])) {        
        $form[$key]['tm_notifications-' . $key . '-subject']['#element_validate'] = array('token_element_validate');
        $form[$key]['tm_notifications-' . $key . '-subject']['#token_types'] = $value['tokens']['token_types'];
        $form[$key]['tm_notifications-' . $key . '-message']['#element_validate'] = array('token_element_validate');
        $form[$key]['tm_notifications-' . $key . '-message']['#token_types'] = $value['tokens']['token_types'];
        
        $form[$key]['tokens'] = array(
          '#markup' => theme('token_tree_link', $value['tokens'])
        );
      }
      
    }
  }

  return system_settings_form($form);
}

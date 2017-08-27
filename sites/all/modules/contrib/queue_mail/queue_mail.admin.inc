<?php

/**
 * @file
 * Admin interface for Queue Mail.
 */

/**
 * The admin settings form for Queue Mail.
 */
function queue_mail_settings_form() {
  $queue_length = _queue_mail_get_queue()->numberOfItems();

  $form['queue_status'] = array(
    '#type' => 'item',
    '#title' => t('Queue status'),
    '#markup' => $queue_length > 0 ? format_plural($queue_length,
      '1 mail currently queued for sending.',
      '@count mails currently queued for sending.') : t('Mail queue is empty'),
  );

  $form['queue_mail_send_on_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send queued mail on cron run'),
    '#description' => t('If checked, sending of queued mail will happen on cron runs. You can <a href="@cron_link">run cron manually</a>. Uncheck this if you want to process the mail queue outside of cron (e.g. via Drush).', array('@cron_link' => url('admin/reports/status/run-cron', array('query' => drupal_get_destination())))),
    '#default_value' => variable_get('queue_mail_send_on_cron', TRUE),
  );

  $form['queue_mail_keys'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail IDs to queue'),
    '#description' => t('Enter each mail ID to queue on a separate line. Use <strong>*</strong> to do a wildcard match.') . '<br/>' . t('Mail IDs are a combination of the first and second arguments sent to <em>drupal_mail</em> when a module sends an email. E.g. <em>user_mail</em>, <em>register_pending_approval_admin</em>') . '<br />' . t('For example, to queue all mails sent by the User module, enter: <em>user_*</em> above, to just queue password recovery emails enter: <em>user_password_reset</em>'),
    '#default_value' => variable_get('queue_mail_keys', ''),
  );

  // Get a list of modules that implement hook_mail.
  $mail_modules = module_implements('mail');
  $all_modules = system_list('module_enabled');
  $rows = array();
  foreach ($mail_modules as $module) {
    $row = array();
    $row[] = check_plain(isset($all_modules[$module]->info['name']) ? $all_modules[$module]->info['name'] : $module);
    $row[] = check_plain($module . '_*');
    $rows[] = $row;
  }
  $headers = array(
    t('Module name'),
    t('Mail ID prefix'),
  );
  $form['queue_mail_keys_help'] = array(
    '#type' => 'item',
    '#title' => t('Modules that send emails'),
    '#markup' => t('The following modules send emails. The contents of the second column can be used in the options above to queue the sending of those emails.') . theme('table', array('header' => $headers, 'rows' => $rows)),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );

  $options = array();
  for ($i = 5; $i <= 240; $i += 5) {
    $options[$i] = format_plural($i, '1 second', '@count seconds');
  }

  $form['advanced']['queue_mail_queue_time'] = array(
    '#type' => 'select',
    '#title' => t('Queue processing time (max)'),
    '#description' => t('How much time in seconds to allow queue mail to send emails for on cron. Warning if you set a very high limit your cron run could timeout and never complete.'),
    '#options' => $options,
    '#default_value' => variable_get('queue_mail_queue_time', 15),
  );

  return system_settings_form($form);
}

<?php

/**
 * @file
 * Drush integration for the Queue Mail module.
 */

/**
 * Implements hook_drush_command().
 */
function queue_mail_drush_command() {
  $items = array();

  $items['queue-mail-send-all'] = array(
    'description' => "Send all remaining queued emails.",
    'options' => array(
      'timeout' => array(
        'description' => 'The maximum amount of time to spend sending emails, defaults to 0, which means: keep going until all queued emails are sent.',
        'example-value' => 60,
        'value' => 'optional',
      ),
    ),
    'drupal dependencies' => array(
      'queue_mail',
    ),
  );

  return $items;
}

/**
 * Drush command to dequeue and send all emails.
 */
function drush_queue_mail_send_all() {
  $sent_mails_count = 0;

  // Prevent session information from being saved while queue is running.
  drupal_save_session(FALSE);

  // Force the current user to anonymous to ensure consistent permissions on
  // queue runs.
  $original_user = $GLOBALS['user'];
  $GLOBALS['user'] = drupal_anonymous_user();

  // Grab the defined cron queues.
  $queues = module_invoke_all('cron_queue_info');
  drupal_alter('cron_queue_info', $queues);

  // And fetch and process our queue.
  $queue_name = QUEUE_MAIL_QUEUE_NAME;
  if (isset($queues[$queue_name])) {
    $info = $queues[$queue_name];
    $function = $info['worker callback'];
    $timeout = drush_get_option('timeout', 0);
    $end = time() + $timeout;
    $queue = _queue_mail_get_queue();
    while ((($timeout == 0) || time() < $end) && ($item = $queue->claimItem())) {
      try {
        $function($item->data);
        $queue->deleteItem($item);
        $sent_mails_count++;
      }
      catch (Exception $e) {
        // In case of exception log it and leave the item in the queue
        // to be processed again later.
        watchdog_exception('queue_mail', $e);
      }
    }
  }
  // Restore the user.
  $GLOBALS['user'] = $original_user;
  drupal_save_session(TRUE);

  drush_log(dt('Processed @count queued items.', array('@count' => $sent_mails_count)), 'ok');
}

<?php
/**
 * @file
 * tm_base.ultimate_cron.inc
 */

/**
 * Implements hook_default_ultimate_cron_job().
 */
function tm_base_default_ultimate_cron_job() {
  $export = array();

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'queue_queue_mail';
  $job->title = 'Queue: queue_mail';
  $job->settings = array(
    'settings' => array(
      'queue' => array(
        'throttle' => 0,
      ),
      'poorman' => array(),
      'general' => array(),
    ),
    'scheduler' => array(
      'name' => 'crontab',
      'crontab' => array(
        'rules' => array(
          0 => '*/5 * * * *',
        ),
      ),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
  );
  $export['queue_queue_mail'] = $job;

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'update_cron';
  $job->title = 'Check system for updates';
  $job->settings = array(
    'scheduler' => array(
      'name' => '',
      'simple' => array(
        'rules' => array(
          0 => '0+@ 0 * * 0',
        ),
      ),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
    'settings' => array(
      'queue' => array(),
      'poorman' => array(),
      'general' => array(),
    ),
  );
  $export['update_cron'] = $job;

  return $export;
}

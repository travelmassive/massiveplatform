<?php

/**
 * Implements hook_migrate_api().
 */
function tm_sample_content_migrate_api() {
  $api = array(
    'api' => 2,
    'migrations' => array(
      'Chapters' => array('class_name' => 'TMChaptersMigration'),
    ),
  );
  return $api;
}
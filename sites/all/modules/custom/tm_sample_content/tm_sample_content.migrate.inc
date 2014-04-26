<?php

/**
 * Implements hook_migrate_api().
 */
function tm_sample_content_migrate_api() {
  $api = array(
    'api' => 2,
    'migrations' => array(
      'Users' => array('class_name' => 'TMUserMigration'),
      'Chapters' => array('class_name' => 'TMChaptersMigration'),
      'Companies' => array('class_name' => 'TMCompaniesMigration'),
      'Events' => array('class_name' => 'TMEventsMigration'),
      'PartnerEvents' => array('class_name' => 'TMPartnerEventsMigration'),
    ),
  );
  return $api;
}
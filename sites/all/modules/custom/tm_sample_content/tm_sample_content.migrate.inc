<?php

/**
 * Implements hook_migrate_api().
 */
function tm_sample_content_migrate_api() {
  $api = array(
    'api' => 2,
    'migrations' => array(
      'TMUser' => array('class_name' => 'TMUserMigration'),
      'TMChapter' => array('class_name' => 'TMChapterMigration'),
      'TMCompany' => array('class_name' => 'TMCompanyMigration'),
      'TMEvent' => array('class_name' => 'TMEventMigration'),
      'TMPartnerEvent' => array('class_name' => 'TMPartnerEventMigration'),
    ),
  );
  return $api;
}

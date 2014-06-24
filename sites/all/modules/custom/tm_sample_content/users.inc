<?php

class TMUserMigration extends TMBaseMigration {

  public function __construct() {
    parent::__construct();

    $this->description = t('Import users.');

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV(
      drupal_get_path('module', 'tm_sample_content') . '/import/users.csv',
      array(),
      $this->sourceFileOptions()
    );

    // Destination
    $this->destination = new MigrateDestinationUser();


    // Field Mappings

    // Name
    $this->addFieldMapping('name', 'name');

    // Pass
    $this->addFieldMapping('pass', 'pass');

    // Mail
    $this->addFieldMapping('mail', 'mail');

    // Avatar
    $this->addFieldMapping('field_avatar', 'avatar');
    $this->addFieldMapping('field_avatar:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_avatar:source_dir')
      ->defaultValue(drupal_get_path('module', 'tm_sample_content') . '/import/files');
    
    // Banner image
    $this->addFieldMapping('field_image', 'banner');
    $this->addFieldMapping('field_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_image:source_dir')
      ->defaultValue(drupal_get_path('module', 'tm_sample_content') . '/import/files');

    // Role ID
    $this->addFieldMapping('roles', 'role');

    // Status
    $this->addFieldMapping('status','status');

    // Friendly URL
    $this->addFieldMapping('field_friendly_url','url');

    // The profile questions
    $this->addFieldMapping('field_user_question_destination','q1');
    $this->addFieldMapping('field_user_question_dream_travel','q2');
    $this->addFieldMapping('field_user_question_learn','q3');
    $this->addFieldMapping('field_user_question_job','q4');
    $this->addFieldMapping('field_user_question_why_travel','q5');

  }

  public function prepareRow($row) {
    // Make sure that any empty fields, are imported as null and not empty strings.
    if (empty($row->limit)) {
      $row->limit = array();
    }
  }

}

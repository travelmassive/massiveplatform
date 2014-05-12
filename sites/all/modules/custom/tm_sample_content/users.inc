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

  }

  public function prepareRow($row) {
  }

}

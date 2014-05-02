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

    // Role ID
    $this->addFieldMapping('roles', 'role');

    // Status
    $this->addFieldMapping('status','status');

  }

  public function prepareRow($row) {
  }

}

<?php

class TMPartnerEventMigration extends TMBaseMigration {

  public function __construct() {
    parent::__construct();

    $this->description = t('Import partner events.');

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
      drupal_get_path('module', 'tm_sample_content') . '/import/partner_events.csv',
      array(),
      $this->sourceFileOptions()
    );

    // Destination
    $this->destination = new MigrateDestinationNode('partner_event');


    // Field Mappings

    // Title
    $this->addFieldMapping('title', 'title');

    // Body
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:format')
      ->defaultValue('filtered_html');

    // Author, default to admin
    $this->addFieldMapping('uid')
      ->defaultValue(1);

  }

  public function prepareRow($row) {
  }

}

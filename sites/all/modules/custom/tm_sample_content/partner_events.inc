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
    $this->destination = new MigrateDestinationNode('event');


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

    // Date field
    $this->addFieldMapping('field_event_date', 'date');
    
    // RSVP link
    $this->addFieldMapping('field_rsvp_link', 'rsvp_link');
  
    // Venue name, street address and map link
    $this->addFieldMapping('field_event_venue_name', 'venue');
    $this->addFieldMapping('field_location', 'address');
    $this->addFieldMapping('field_map_link', 'map_link');
  }

  public function prepareRow($row) {
  }
}

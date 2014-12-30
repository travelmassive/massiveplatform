<?php

class TMEventMigration extends TMBaseMigration {

  public function __construct() {
    parent::__construct();

    $this->description = t('Import events.');

    // Dependencies, so we migrate in order.
    $this->dependencies = array('TMUser', 'TMChapter');

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
      //drupal_get_path('module', 'tm_sample_content') . '/import/events.csv',
      drupal_get_path('module', 'tm_sample_content') . '/import/events_travelmassive_historical.csv',

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

    // Link with chapters, based on the ID
    $this->addFieldMapping('field_chapter', 'chapter')
      ->sourceMigration('TMChapter');

    // Event organizer, only one user ref
    $this->addFieldMapping('field_organizer', 'organizer')
    ->separator('|')
    ->sourceMigration('TMUser');

    // Temporary field mapping to use on flags
    $this->addFieldMapping('event_register', 'event_register')
      ->separator('|')
      ->sourceMigration('TMUser');
    
    // Banner image
    $this->addFieldMapping('field_image', 'banner');
    $this->addFieldMapping('field_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_image:source_dir')
      ->defaultValue(drupal_get_path('module', 'tm_sample_content') . '/import/files');

    // Date field
    $this->addFieldMapping('field_event_date', 'date');

    // Signup limit
    $this->addFieldMapping('field_signup_limit', 'limit');

    // External RSVP
    $this->addFieldMapping('field_external_rsvp', 'external');
    
    // RSVP link
    $this->addFieldMapping('field_rsvp_link', 'rsvp_link');
  
    // Venue name, street address and map link
    $this->addFieldMapping('field_event_venue_name', 'venue');
    $this->addFieldMapping('field_location', 'address');
    $this->addFieldMapping('field_map_link', 'map_link');

    // Partner Event
    $this->addFieldMapping('field_is_partner_event', 'is_partner_event');

  }

  public function prepareRow($row) {
    // Make sure that any 'empty' signup limits, are imported as null and not empty strings.
    if (empty($row->limit) or ($row->limit == "0")) {
      $row->limit = array();
    }
  }

  public function complete($entity, $row) {
    // simulate "event register" flag activation
    if ($entity->event_register) {
      // prepare variables
      $flag = flag_get_flag('event_register');

      // perform flagging
      foreach ($entity->event_register as $uid) {
        $participant = user_load($uid);
        $flag->flag('flag', $entity->nid, $participant, TRUE);
      }
    }
  }

}

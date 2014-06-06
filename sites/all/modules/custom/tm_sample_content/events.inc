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
      drupal_get_path('module', 'tm_sample_content') . '/import/events.csv',
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
      ->sourceMigration('TMUser');

    // Temporary field mapping to use on flags
    $this->addFieldMapping('signed_up', 'signed_up')
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
  }

  public function prepareRow($row) {
    if (empty($row->limit)) {
      $row->limit = array();
    }
  }

  public function complete($entity, $row) {
    // simulate "signed up" flag activation
    if ($entity->signed_up) {
      // prepare variables
      $flag = flag_get_flag('signup');

      // perform flagging
      foreach ($entity->signed_up as $uid) {
        $participant = user_load($uid);
        $flag->flag('flag', $entity->nid, $participant, TRUE);
      }
    }
  }

}

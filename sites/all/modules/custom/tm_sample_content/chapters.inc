<?php

class TMChapterMigration extends TMBaseMigration {

  public function __construct() {
    parent::__construct();

    $this->description = t('Import chapters.');

    // Dependencies, so we migrate in order.
    $this->dependencies = array('TMUser');

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
      drupal_get_path('module', 'tm_sample_content') . '/import/chapters.csv',
      array(),
      $this->sourceFileOptions()
    );

    // Destination
    $this->destination = new MigrateDestinationNode('chapter');


    // Field Mappings

    // Title
    $this->addFieldMapping('title', 'title');

    // Body
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:format')->defaultValue('filtered_html');

    // Chapter leaders
    $this->addFieldMapping('field_chapter_leaders', 'leaders')
      ->separator(' ')
      ->sourceMigration('TMUser');

    // Author, default to admin
    $this->addFieldMapping('uid')->defaultValue(1);

  }

  public function prepareRow($row) {
  }

}

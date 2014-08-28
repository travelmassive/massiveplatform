<?php

class TMCompanyMigration extends TMBaseMigration {

  public function __construct() {
    parent::__construct();

    $this->description = t('Import companies.');

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
      drupal_get_path('module', 'tm_sample_content') . '/import/companies.csv',
      array(),
      $this->sourceFileOptions()
    );

    // Destination
    $this->destination = new MigrateDestinationNode('organization');


    // Field Mappings

    // Title
    $this->addFieldMapping('title', 'title');

    // Body
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:format')->defaultValue('filtered_html');

    // Owner (user manager by)
    $this->addFieldMapping('field_owner', 'owner')
      ->separator(' ')
      ->sourceMigration('TMUser');

    // Author, default to admin
    $this->addFieldMapping('uid')
      ->defaultValue(1);

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

    // Friendly URL
    $this->addFieldMapping('field_friendly_url','url');

    // The social link fields
    $this->addFieldMapping('field_link_twitter', 'tw_link');
    $this->addFieldMapping('field_link_facebook', 'fb_link');
    $this->addFieldMapping('field_link_linkedin', 'li_link');
    $this->addFieldMapping('field_link_website', 'web_link');

    // City + country fields
    $this->addFieldMapping('field_location_city', 'city');
    $this->addFieldMapping('field_country', 'country');

  }

  public function prepareRow($row) {
  }

}

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

    // First and last name
    $this->addFieldMapping('field_user_first_name', 'first');
    $this->addFieldMapping('field_user_last_name', 'last');

    // About me
    $this->addFieldMapping('field_about_me', 'about');

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

    // The social link fields
    $this->addFieldMapping('field_link_twitter', 'tw_link');
    $this->addFieldMapping('field_link_twitter:title', 'tw_link_title');
    $this->addFieldMapping('field_link_facebook', 'fb_link');
    $this->addFieldMapping('field_link_facebook:title', 'fb_link_title');
    $this->addFieldMapping('field_link_linkedin', 'li_link');
    $this->addFieldMapping('field_link_linkedin:title', 'li_link_title');
    $this->addFieldMapping('field_link_website', 'web_link');
    $this->addFieldMapping('field_link_website:title', 'web_link_title');

    // City + country fields
    $this->addFieldMapping('field_location_city', 'city');
    $this->addFieldMapping('field_location_country', 'country');
  }

  public function prepareRow($row) {
    // Make sure that any empty fields, are imported as null and not empty strings.
    if (empty($row->limit)) {
      $row->limit = array();
    }
  }

}

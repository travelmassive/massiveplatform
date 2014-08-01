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

    // Banner image
    $this->addFieldMapping('field_image', 'banner');
    $this->addFieldMapping('field_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_image:source_dir')
      ->defaultValue(drupal_get_path('module', 'tm_sample_content') . '/import/files');
    
    // City shortcode
    $this->addFieldMapping('field_chapter_shortcode', 'code');

    // City color
    $this->addFieldMapping('field_chapter_color', 'color');

    // Temporary field mapping to use on chapters
    $this->addFieldMapping('member', 'member')
      ->separator(' ')
      ->sourceMigration('TMUser');

    // The social link fields
    $this->addFieldMapping('field_link_twitter', 'tw_link');
    $this->addFieldMapping('field_link_twitter:title', 'tw_link_title');
    $this->addFieldMapping('field_link_facebook', 'fb_link');
    $this->addFieldMapping('field_link_facebook:title', 'fb_link_title');
    $this->addFieldMapping('field_link_linkedin', 'li_link');
    $this->addFieldMapping('field_link_linkedin:title', 'li_link_title');
    $this->addFieldMapping('field_link_website', 'web_link');
    $this->addFieldMapping('field_link_website:title', 'web_link_title');
  }

  public function complete($entity, $row) {
    // simulate "member"
    if ($entity->member) {
      // set the chapter on the user
      foreach ($entity->member as $uid) {
        $participant = user_load($uid);
        $participant->field_chapter[LANGUAGE_NONE][0]['target_id'] = $entity->nid;
        user_save($participant);
      }
    }
  }

}

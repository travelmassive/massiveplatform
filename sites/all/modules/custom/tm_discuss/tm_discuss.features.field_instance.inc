<?php
/**
 * @file
 * tm_discuss.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function tm_discuss_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-chapter-field_discuss_category_id'.
  $field_instances['node-chapter-field_discuss_category_id'] = array(
    'bundle' => 'chapter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Link recent discussions to a category. Leave blank for default behavior.
<br>Enter the Vanilla Forums Category ID or ClassName to display.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_discuss_category_id',
    'label' => 'Link to Discussions',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-event-field_event_discuss_category_id'.
  $field_instances['node-event-field_event_discuss_category_id'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Link recent discussions to a category.
<br>Enter the Vanilla Forums Category ID or ClassName to display.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 54,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_discuss_category_id',
    'label' => 'Link to Discussions',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 9,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Link recent discussions to a category.
<br>Enter the Vanilla Forums Category ID or ClassName to display.');
  t('Link recent discussions to a category. Leave blank for default behavior.
<br>Enter the Vanilla Forums Category ID or ClassName to display.');
  t('Link to Discussions');

  return $field_instances;
}

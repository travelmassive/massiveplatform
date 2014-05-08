<?php
/**
 * @file
 * tm_events.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function tm_events_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|event|full';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'event';
  $ds_fieldsetting->view_mode = 'full';
  $ds_fieldsetting->settings = array(
    'field_image' => array(
      'formatter_settings' => array(
        'ft' => array(),
      ),
    ),
  );
  $export['node|event|full'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|event|grid';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'event';
  $ds_fieldsetting->view_mode = 'grid';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '0',
        'wrapper' => 'h1',
        'class' => 'prime-title',
        'ft' => array(),
      ),
    ),
    'field_image' => array(
      'formatter_settings' => array(
        'ft' => array(),
      ),
    ),
    'field_location' => array(
      'formatter_settings' => array(
        'ft' => array(
          'ow' => TRUE,
          'ow-el' => 'p',
          'ow-cl' => 'meta',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
  );
  $export['node|event|grid'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function tm_events_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|event|full';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'event';
  $ds_layout->view_mode = 'full';
  $ds_layout->layout = 'trilithon';
  $ds_layout->settings = array(
    'regions' => array(
      'first_column' => array(
        0 => 'body',
        1 => 'field_date',
        2 => 'field_map_link',
        3 => 'field_signup_limit',
        6 => 'field_location',
        7 => 'field_rsvp_link',
        8 => 'field_organizer',
      ),
      'banner' => array(
        4 => 'field_chapter',
        5 => 'field_image',
      ),
    ),
    'fields' => array(
      'body' => 'first_column',
      'field_date' => 'first_column',
      'field_map_link' => 'first_column',
      'field_signup_limit' => 'first_column',
      'field_chapter' => 'banner',
      'field_image' => 'banner',
      'field_location' => 'first_column',
      'field_rsvp_link' => 'first_column',
      'field_organizer' => 'first_column',
    ),
    'classes' => array(),
    'wrappers' => array(
      'banner' => 'header',
      'first_column' => 'div',
      'second_column' => 'div',
    ),
    'layout_wrapper' => 'article',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'hide_page_title' => '0',
    'page_option_title' => '',
  );
  $export['node|event|full'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|event|grid';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'event';
  $ds_layout->view_mode = 'grid';
  $ds_layout->layout = 'card';
  $ds_layout->settings = array(
    'regions' => array(
      'teaser' => array(
        0 => 'title',
        4 => 'field_location',
      ),
      'media' => array(
        1 => 'field_image',
        2 => 'field_date',
      ),
      'extra' => array(
        3 => 'field_chapter',
      ),
    ),
    'fields' => array(
      'title' => 'teaser',
      'field_image' => 'media',
      'field_date' => 'media',
      'field_chapter' => 'extra',
      'field_location' => 'teaser',
    ),
    'classes' => array(),
    'wrappers' => array(
      'media' => 'div',
      'teaser' => 'div',
      'extra' => 'div',
    ),
    'layout_wrapper' => 'article',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => 'content',
    'layout_link_custom' => '',
  );
  $export['node|event|grid'] = $ds_layout;

  return $export;
}

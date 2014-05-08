<?php
/**
 * @file
 * tm_chapters.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function tm_chapters_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|chapter|full';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'chapter';
  $ds_fieldsetting->view_mode = 'full';
  $ds_fieldsetting->settings = array(
    'field_image' => array(
      'formatter_settings' => array(
        'ft' => array(),
      ),
    ),
  );
  $export['node|chapter|full'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|chapter|grid';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'chapter';
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
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'field_image' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
  );
  $export['node|chapter|grid'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function tm_chapters_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|chapter|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'chapter';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'trilithon';
  $ds_layout->settings = array(
    'regions' => array(
      'first_column' => array(
        0 => 'body',
      ),
      'second_column' => array(
        1 => 'field_chapter_leaders',
      ),
      'banner' => array(
        2 => 'field_image',
      ),
    ),
    'fields' => array(
      'body' => 'first_column',
      'field_chapter_leaders' => 'second_column',
      'field_image' => 'banner',
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
  );
  $export['node|chapter|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|chapter|full';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'chapter';
  $ds_layout->view_mode = 'full';
  $ds_layout->layout = 'trilithon';
  $ds_layout->settings = array(
    'regions' => array(
      'first_column' => array(
        0 => 'body',
      ),
      'second_column' => array(
        1 => 'field_chapter_leaders',
      ),
      'banner' => array(
        2 => 'field_image',
      ),
    ),
    'fields' => array(
      'body' => 'first_column',
      'field_chapter_leaders' => 'second_column',
      'field_image' => 'banner',
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
  $export['node|chapter|full'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|chapter|grid';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'chapter';
  $ds_layout->view_mode = 'grid';
  $ds_layout->layout = 'card';
  $ds_layout->settings = array(
    'regions' => array(
      'teaser' => array(
        0 => 'title',
      ),
      'media' => array(
        1 => 'field_image',
      ),
    ),
    'fields' => array(
      'title' => 'teaser',
      'field_image' => 'media',
    ),
    'classes' => array(),
    'wrappers' => array(
      'media' => 'div',
      'teaser' => 'div',
      'extra' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|chapter|grid'] = $ds_layout;

  return $export;
}

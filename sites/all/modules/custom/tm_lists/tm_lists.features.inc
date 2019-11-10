<?php

/**
 * @file
 * tm_lists.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_lists_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function tm_lists_image_default_styles() {
  $styles = array();

  // Exported image style: list_newsletter.
  $styles['list_newsletter'] = array(
    'label' => 'List Newsletter (1040 x 800)',
    'effects' => array(
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 1040,
          'height' => 800,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function tm_lists_node_info() {
  $items = array(
    'list' => array(
      'name' => t('List'),
      'base' => 'node_content',
      'description' => t('A list of things'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

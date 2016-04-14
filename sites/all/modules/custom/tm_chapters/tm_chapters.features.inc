<?php
/**
 * @file
 * tm_chapters.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_chapters_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "field_validation" && $api == "default_field_validation_rules") {
    return array("version" => "2");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tm_chapters_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function tm_chapters_node_info() {
  $items = array(
    'chapter' => array(
      'name' => t('Chapter'),
      'base' => 'node_content',
      'description' => t('A community chapter'),
      'has_title' => '1',
      'title_label' => t('Chapter'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

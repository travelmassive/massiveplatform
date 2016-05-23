<?php
/**
 * @file
 * tm_branding.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_branding_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tm_branding_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function tm_branding_node_info() {
  $items = array(
    'site_brand' => array(
      'name' => t('Site Brand'),
      'base' => 'node_content',
      'description' => t('Site branding'),
      'has_title' => '1',
      'title_label' => t('Brand label'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

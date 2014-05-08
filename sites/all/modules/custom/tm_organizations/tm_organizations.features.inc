<?php
/**
 * @file
 * tm_organizations.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_organizations_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tm_organizations_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function tm_organizations_node_info() {
  $items = array(
    'organization' => array(
      'name' => t('Company'),
      'base' => 'node_content',
      'description' => t('Companies can have a presence on Travel Massive and are managed by members.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}

<?php
/**
 * @file
 * tm_base.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_base_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function tm_base_node_info() {
  $items = array(
    'chapter' => array(
      'name' => t('City'),
      'base' => 'node_content',
      'description' => t('A Travel Massive city'),
      'has_title' => '1',
      'title_label' => t('City'),
      'help' => '',
    ),
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('A Travel Massive event organized by one of the chapters.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'organization' => array(
      'name' => t('Company'),
      'base' => 'node_content',
      'description' => t('Companies can have a presence on Travel Massive and are managed by members.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'partner_event' => array(
      'name' => t('Partner event'),
      'base' => 'node_content',
      'description' => t('Travel related events organized by TM partners.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

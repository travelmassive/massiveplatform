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
function tm_organizations_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function tm_organizations_flag_default_flags() {
  $flags = array();
  // Exported flag: "TM Sponsor".
  $flags['tm_sponsor'] = array(
    'entity_type' => 'node',
    'title' => 'TM Sponsor',
    'global' => 1,
    'types' => array(
      0 => 'organization',
    ),
    'flag_short' => 'Set as sponsor',
    'flag_long' => 'Set as TM sponsor',
    'flag_message' => '[node:title] was set as a Travel Massive sponsor',
    'unflag_short' => 'Unset as sponsor',
    'unflag_long' => 'Remove [node:title] from the sponsor list',
    'unflag_message' => '[node:title] was removed as a Travel Massive sponsor',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
      'search_index' => 0,
      'search_result' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'grid' => 0,
      'list' => 0,
      'revision' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'tm_organizations',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

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
  drupal_alter('node_info', $items);
  return $items;
}

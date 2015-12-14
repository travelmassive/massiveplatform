<?php
/**
 * @file
 * tm_payments.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_payments_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function tm_payments_flag_default_flags() {
  $flags = array();
  // Exported flag: "Event Paid".
  $flags['event_paid'] = array(
    'entity_type' => 'node',
    'title' => 'Event Paid',
    'global' => 0,
    'types' => array(
      0 => 'event',
    ),
    'flag_short' => 'Pay',
    'flag_long' => 'Pay for [node:title]',
    'flag_message' => 'You successfully paid for [node:title]',
    'unflag_short' => 'Unpay',
    'unflag_long' => 'Unpay for [node:title]',
    'unflag_message' => 'You successfully unpaid for [node:title]',
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
    'module' => 'tm_payments',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

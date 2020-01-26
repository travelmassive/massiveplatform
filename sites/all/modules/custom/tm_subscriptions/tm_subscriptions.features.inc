<?php

/**
 * @file
 * tm_subscriptions.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_subscriptions_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function tm_subscriptions_flag_default_flags() {
  $flags = array();
  // Exported flag: "Organization Subscription Enabled".
  $flags['organization_subscription_enable'] = array(
    'entity_type' => 'node',
    'title' => 'Organization Subscription Enabled',
    'global' => 1,
    'types' => array(
      0 => 'organization',
    ),
    'flag_short' => 'Enable Subscription',
    'flag_long' => 'Enable Subscription',
    'flag_message' => '[node:title] Subscription enabled',
    'unflag_short' => 'Disable Subscription',
    'unflag_long' => 'Disable Subscription',
    'unflag_message' => '[node:title] Subscription disabled',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
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
    'module' => 'tm_subscriptions',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Organization Subscription Paid".
  $flags['organization_subscription_paid'] = array(
    'entity_type' => 'node',
    'title' => 'Organization Subscription Paid',
    'global' => 1,
    'types' => array(
      0 => 'organization',
    ),
    'flag_short' => 'Flag this item',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unflag this item',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
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
    'module' => 'tm_subscriptions',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

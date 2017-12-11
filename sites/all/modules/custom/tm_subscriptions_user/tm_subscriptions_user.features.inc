<?php
/**
 * @file
 * tm_subscriptions_user.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_subscriptions_user_ctools_plugin_api($module = NULL, $api = NULL) {
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
function tm_subscriptions_user_flag_default_flags() {
  $flags = array();
  // Exported flag: "User Subscription Enabled".
  $flags['user_subscription_enable'] = array(
    'entity_type' => 'user',
    'title' => 'User Subscription Enabled',
    'global' => 1,
    'types' => array(),
    'flag_short' => 'Enable Subscription',
    'flag_long' => 'Enable Subscription',
    'flag_message' => '[user:name] Subscription enabled',
    'unflag_short' => 'Disable Subscription',
    'unflag_long' => 'Disable Subscription',
    'unflag_message' => '[user:name] Subscription disabled',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'grid' => 0,
      'list' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'show_on_profile' => 0,
    'access_uid' => '',
    'module' => 'tm_subscriptions_user',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "User Subscription Paid".
  $flags['user_subscription_paid'] = array(
    'entity_type' => 'user',
    'title' => 'User Subscription Paid',
    'global' => 1,
    'types' => array(),
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
      'diff_standard' => 0,
      'token' => 0,
      'grid' => 0,
      'list' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'show_on_profile' => 0,
    'access_uid' => '',
    'module' => 'tm_subscriptions_user',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

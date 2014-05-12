<?php
/**
 * @file
 * tm_users.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_users_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tm_users_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

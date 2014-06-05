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
function tm_users_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

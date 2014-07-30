<?php
/**
 * @file
 * tm_notifications.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_notifications_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}
<?php
/**
 * @file
 * tm_notifications_whatsup.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_notifications_whatsup_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

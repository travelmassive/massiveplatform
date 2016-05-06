<?php
/**
 * @file
 * tm_notifications_newsfeed.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_notifications_newsfeed_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

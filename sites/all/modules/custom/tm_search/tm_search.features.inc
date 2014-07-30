<?php
/**
 * @file
 * tm_search.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_search_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

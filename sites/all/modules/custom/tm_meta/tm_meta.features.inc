<?php

/**
 * @file
 * tm_meta.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_meta_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

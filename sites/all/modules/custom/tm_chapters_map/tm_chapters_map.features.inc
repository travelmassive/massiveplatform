<?php
/**
 * @file
 * tm_chapters_map.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_chapters_map_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "openlayers" && $api == "openlayers_layers") {
    return array("version" => "1");
  }
  if ($module == "openlayers" && $api == "openlayers_maps") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tm_chapters_map_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

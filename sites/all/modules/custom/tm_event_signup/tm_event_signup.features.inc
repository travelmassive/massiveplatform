<?php
/**
 * @file
 * tm_event_signup.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_event_signup_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tm_event_signup_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

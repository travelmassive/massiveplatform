<?php
/**
 * @file
 * tm_users_external_signin.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_users_external_signin_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

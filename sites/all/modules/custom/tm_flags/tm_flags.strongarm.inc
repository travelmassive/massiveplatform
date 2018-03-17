<?php
/**
 * @file
 * tm_flags.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_flags_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__announcement_last_sent';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__announcement_last_sent'] = $strongarm;

  return $export;
}

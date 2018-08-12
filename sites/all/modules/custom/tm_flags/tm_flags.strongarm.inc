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
  $strongarm->name = 'field_bundle_settings_flagging__add_member_to_chapter';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__add_member_to_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__announcement_last_sent';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__announcement_last_sent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__remove_member_from_chapter';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__remove_member_from_chapter'] = $strongarm;

  return $export;
}

<?php
/**
 * @file
 * tm_status_updates.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_status_updates_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__status_updated_by_chapter';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__status_updated_by_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__status_updated_by_event';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__status_updated_by_event'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__status_updated_by_member';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__status_updated_by_member'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__status_updated_by_organization';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__status_updated_by_organization'] = $strongarm;

  return $export;
}

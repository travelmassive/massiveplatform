<?php
/**
 * @file
 * tm_flags.ds.inc
 */

/**
 * Implements hook_ds_custom_fields_info().
 */
function tm_flags_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'members_following';
  $ds_field->label = 'Members following';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = 'organization|*';
  $ds_field->properties = array(
    'block' => 'views|4c6c0d281ab03d2ab669c932644fc515',
    'block_render' => '3',
  );
  $export['members_following'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'members_following_members';
  $ds_field->label = 'Following';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'user' => 'user',
  );
  $ds_field->ui_limit = 'user|*';
  $ds_field->properties = array(
    'block' => 'views|member_listings-block_following',
    'block_render' => '3',
  );
  $export['members_following_members'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'members_signed_up';
  $ds_field->label = 'Members signed up';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = 'event|*
chapter|*';
  $ds_field->properties = array(
    'block' => 'views|844877411cb8e789aba478ebfd20174d',
    'block_render' => '3',
  );
  $export['members_signed_up'] = $ds_field;

  return $export;
}

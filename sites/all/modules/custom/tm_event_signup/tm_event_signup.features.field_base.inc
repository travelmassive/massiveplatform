<?php

/**
 * @file
 * tm_event_signup.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function tm_event_signup_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_external_rsvp'.
  $field_bases['field_external_rsvp'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_external_rsvp',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => '(Default) Allow members to RSVP instantly',
        1 => 'Link to an external RSVP service',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_signup_limit'.
  $field_bases['field_signup_limit'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_signup_limit',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  return $field_bases;
}

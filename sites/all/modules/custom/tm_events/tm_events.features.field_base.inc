<?php
/**
 * @file
 * tm_events.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function tm_events_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_event_date'
  $field_bases['field_event_date'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_event_date',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'timezone_db' => 'America/Los_Angeles',
      'todate' => 'optional',
      'tz_handling' => 'date',
    ),
    'translatable' => 0,
    'type' => 'datetime',
  );

  return $field_bases;
}

<?php
/**
 * @file
 * tm_events.default_field_validation_rules.inc
 */

/**
 * Implements hook_default_field_validation_rule().
 */
function tm_events_default_field_validation_rule() {
  $export = array();

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'validate_event_external_rsvp_url';
  $rule->name = 'validate_event_external_rsvp_url';
  $rule->field_name = 'field_rsvp_link';
  $rule->col = 'url';
  $rule->entity_type = 'node';
  $rule->bundle = 'event';
  $rule->validator = 'field_validation_url_validator';
  $rule->settings = array(
    'external' => 1,
    'internal' => 0,
    'bypass' => 0,
    'roles' => array(
      1 => 0,
      2 => 0,
      4 => 0,
      6 => 0,
      7 => 0,
      3 => 0,
      5 => 0,
    ),
    'errors' => 0,
  );
  $rule->error_message = 'Please provide a valid RSVP URL (ie: http://www.eventbrite.com/e/your-event-url)';
  $export['validate_event_external_rsvp_url'] = $rule;

  return $export;
}

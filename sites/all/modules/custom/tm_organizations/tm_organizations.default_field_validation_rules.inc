<?php
/**
 * @file
 * tm_organizations.default_field_validation_rules.inc
 */

/**
 * Implements hook_default_field_validation_rule().
 */
function tm_organizations_default_field_validation_rule() {
  $export = array();

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'Friendly URL characters';
  $rule->name = 'friendly_url_org_characters';
  $rule->field_name = 'field_friendly_url';
  $rule->col = 'value';
  $rule->entity_type = 'node';
  $rule->bundle = 'organization';
  $rule->validator = 'field_validation_regex_validator';
  $rule->settings = array(
    'data' => '^[-_a-zA-Z]+$',
    'bypass' => 0,
    'roles' => array(
      1 => 0,
      2 => 0,
      3 => 0,
    ),
    'errors' => 0,
  );
  $rule->error_message = 'The friendly url may only contain letters, numbers, hyphens and underscores.';
  $export['friendly_url_org_characters'] = $rule;

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'Friendly URL unique';
  $rule->name = 'friendly_url_org_unique';
  $rule->field_name = 'field_friendly_url';
  $rule->col = 'value';
  $rule->entity_type = 'node';
  $rule->bundle = 'organization';
  $rule->validator = 'tm_fields_unique_validator';
  $rule->settings = array(
    'data' => 'global',
    'per_user' => 0,
    'skip_empty' => 1,
    'bypass' => 0,
    'roles' => array(
      1 => 0,
      2 => 0,
      3 => 0,
      4 => 0,
    ),
    'errors' => 0,
  );
  $rule->error_message = 'This friendly url is already taken by [existing-entity-link].';
  $export['friendly_url_org_unique'] = $rule;

  return $export;
}

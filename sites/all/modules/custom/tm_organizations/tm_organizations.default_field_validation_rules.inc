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
  $rule->rulename = 'Friendly URL blacklist';
  $rule->name = 'friendly_url_org_blacklist';
  $rule->field_name = 'field_friendly_url';
  $rule->col = 'value';
  $rule->entity_type = 'node';
  $rule->bundle = 'organization';
  $rule->validator = 'field_validation_blacklist_validator';
  $rule->settings = array(
    'data' => 'admin, administrator',
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
  $rule->error_message = 'The friendly url you picked is reserved. Please pick another one.';
  $export['friendly_url_org_blacklist'] = $rule;

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
    'data' => '^[_\\-\\sa-zA-Z0-9]+$',
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
  $rule->error_message = 'The friendly url may only contain letters, numbers, hyphens and underscores. (example: "your-name")';
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

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'validate_company_facebook_url';
  $rule->name = 'validate_company_facebook_url';
  $rule->field_name = 'field_link_facebook';
  $rule->col = 'url';
  $rule->entity_type = 'node';
  $rule->bundle = 'organization';
  $rule->validator = 'field_validation_url_validator';
  $rule->settings = array(
    'external' => 0,
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
  $rule->error_message = 'Please provide a valid Facebook URL (ie: https://facebook.com/mycompany)';
  $export['validate_company_facebook_url'] = $rule;

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'validate_company_linkedin_url';
  $rule->name = 'validate_company_linkedin_url';
  $rule->field_name = 'field_link_linkedin';
  $rule->col = 'url';
  $rule->entity_type = 'node';
  $rule->bundle = 'organization';
  $rule->validator = 'field_validation_url_validator';
  $rule->settings = array(
    'external' => 0,
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
  $rule->error_message = 'Please provide a valid LinkedIn URL (ie: https://linkedin.com/company/yourcompany)';
  $export['validate_company_linkedin_url'] = $rule;

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'validate_company_twitter_url';
  $rule->name = 'validate_company_twitter_url';
  $rule->field_name = 'field_link_twitter';
  $rule->col = 'url';
  $rule->entity_type = 'node';
  $rule->bundle = 'organization';
  $rule->validator = 'field_validation_url_validator';
  $rule->settings = array(
    'external' => 0,
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
  $rule->error_message = 'Please provide a valid Twitter URL (ie: https://twitter.com/yourcompany)';
  $export['validate_company_twitter_url'] = $rule;

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'validate_company_website_url';
  $rule->name = 'validate_company_website_url';
  $rule->field_name = 'field_link_website';
  $rule->col = 'url';
  $rule->entity_type = 'node';
  $rule->bundle = 'organization';
  $rule->validator = 'field_validation_url_validator';
  $rule->settings = array(
    'external' => 0,
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
  $rule->error_message = 'Please provide a valid Website URL (ie: http://www.mycompany.com)';
  $export['validate_company_website_url'] = $rule;

  return $export;
}

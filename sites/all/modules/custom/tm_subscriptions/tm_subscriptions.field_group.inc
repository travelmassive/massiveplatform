<?php

/**
 * @file
 * tm_subscriptions.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_subscriptions_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_company_account_details|node|organization|form';
  $field_group->group_name = 'group_company_account_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Account Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_company_account_contact',
      1 => 'field_company_account_email',
      2 => 'field_company_account_phone',
      3 => 'field_company_account_receipt',
      4 => 'field_company_testimonial',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Account Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-company-account-details field-group-fieldset',
        'description' => 'This information will be kept <i>private</i>.',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_company_account_details|node|organization|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Account Details');

  return $field_groups;
}

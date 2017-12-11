<?php
/**
 * @file
 * tm_subscriptions_user.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_subscriptions_user_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_account_details|user|user|form';
  $field_group->group_name = 'group_user_account_details';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_account';
  $field_group->data = array(
    'label' => 'Account Details',
    'weight' => '27',
    'children' => array(
      0 => 'field_user_account_phone',
      1 => 'field_user_account_receipt',
      2 => 'field_user_subscription_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Account Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-user-account-details field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_user_account_details|user|user|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Account Details');

  return $field_groups;
}

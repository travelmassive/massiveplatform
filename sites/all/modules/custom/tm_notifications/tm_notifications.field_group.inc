<?php
/**
 * @file
 * tm_notifications.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_notifications_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_notifications|user|user|form';
  $field_group->group_name = 'group_user_notifications';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_edit';
  $field_group->data = array(
    'label' => 'Notifications',
    'weight' => '35',
    'children' => array(),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Notifications',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => 'user-notifications-options',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_user_notifications|user|user|form'] = $field_group;

  return $export;
}
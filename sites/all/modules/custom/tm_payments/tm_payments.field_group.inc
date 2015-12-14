<?php
/**
 * @file
 * tm_payments.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_payments_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_event_payment|node|event|form';
  $field_group->group_name = 'group_event_payment';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Sell Tickets',
    'weight' => '10',
    'children' => array(
      0 => 'field_event_payment_amount',
      1 => 'field_event_payment_enabled',
      2 => 'field_event_payment_refund_text',
      3 => 'field_event_payment_currency',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Sell Tickets',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-event-payment field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_event_payment|node|event|form'] = $field_group;

  return $export;
}

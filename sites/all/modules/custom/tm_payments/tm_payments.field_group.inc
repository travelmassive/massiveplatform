<?php

/**
 * @file
 * tm_payments.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_payments_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_company_payments|node|organization|form';
  $field_group->group_name = 'group_company_payments';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Payments',
    'weight' => '57',
    'children' => array(
      0 => 'field_company_payment_commission',
      1 => 'field_company_payments_enabled',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Payments',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-company-payments field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_company_payments|node|organization|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_event_early_bird|node|event|form';
  $field_group->group_name = 'group_event_early_bird';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Early Bird Pricing',
    'weight' => '15',
    'children' => array(
      0 => 'field_event_payment_early_amount',
      1 => 'field_event_payment_early_enable',
      2 => 'field_event_payment_early_label',
      3 => 'field_event_payment_early_seats',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Early Bird Pricing',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-event-early-bird field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_event_early_bird|node|event|form'] = $field_group;

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
    'weight' => '14',
    'children' => array(
      0 => 'field_event_payment_amount',
      1 => 'field_event_payment_currency',
      2 => 'field_event_payment_enabled',
      3 => 'field_event_payment_refund_text',
      4 => 'field_event_payment_ask_address',
      5 => 'field_event_payment_tiers',
      6 => 'field_event_payment_type',
      7 => 'field_event_payment_message_text',
      8 => 'field_event_payment_post_event',
      9 => 'field_event_payment_sub_discount',
      10 => 'field_event_payment_partner_code',
      11 => 'field_event_payments_show_total',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Sell Tickets',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-event-payment field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_event_payment|node|event|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Early Bird Pricing');
  t('Payments');
  t('Sell Tickets');

  return $field_groups;
}

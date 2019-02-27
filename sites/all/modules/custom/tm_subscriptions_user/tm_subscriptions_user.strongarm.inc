<?php

/**
 * @file
 * tm_subscriptions_user.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_subscriptions_user_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__user_subscription_enable';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__user_subscription_enable'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_flagging__user_subscription_paid';
  $strongarm->value = array();
  $export['field_bundle_settings_flagging__user_subscription_paid'] = $strongarm;

  return $export;
}

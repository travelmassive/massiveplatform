<?php

/**
 * @file
 * tm_subscriptions.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_subscriptions_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'flag organization_subscription_enable'.
  $permissions['flag organization_subscription_enable'] = array(
    'name' => 'flag organization_subscription_enable',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag organization_subscription_paid'.
  $permissions['flag organization_subscription_paid'] = array(
    'name' => 'flag organization_subscription_paid',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag organization_subscription_enable'.
  $permissions['unflag organization_subscription_enable'] = array(
    'name' => 'unflag organization_subscription_enable',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag organization_subscription_paid'.
  $permissions['unflag organization_subscription_paid'] = array(
    'name' => 'unflag organization_subscription_paid',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  return $permissions;
}

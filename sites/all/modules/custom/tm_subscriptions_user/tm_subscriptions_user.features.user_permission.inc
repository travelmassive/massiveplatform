<?php
/**
 * @file
 * tm_subscriptions_user.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_subscriptions_user_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'flag user_subscription_enable'.
  $permissions['flag user_subscription_enable'] = array(
    'name' => 'flag user_subscription_enable',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag user_subscription_paid'.
  $permissions['flag user_subscription_paid'] = array(
    'name' => 'flag user_subscription_paid',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag user_subscription_enable'.
  $permissions['unflag user_subscription_enable'] = array(
    'name' => 'unflag user_subscription_enable',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag user_subscription_paid'.
  $permissions['unflag user_subscription_paid'] = array(
    'name' => 'unflag user_subscription_paid',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  return $permissions;
}

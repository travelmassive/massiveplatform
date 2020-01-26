<?php

/**
 * @file
 * tm_payments.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_payments_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'flag event_paid'.
  $permissions['flag event_paid'] = array(
    'name' => 'flag event_paid',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag event_paid'.
  $permissions['unflag event_paid'] = array(
    'name' => 'unflag event_paid',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  return $permissions;
}

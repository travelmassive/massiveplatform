<?php
/**
 * @file
 * tm_notifications.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_notifications_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'manage tm notifications'.
  $permissions['manage tm notifications'] = array(
    'name' => 'manage tm notifications',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'tm_notifications',
  );

  return $permissions;
}

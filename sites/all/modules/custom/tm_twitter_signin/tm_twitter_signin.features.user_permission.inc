<?php
/**
 * @file
 * tm_twitter_signin.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_twitter_signin_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'manage tm twitter settings'.
  $permissions['manage tm twitter settings'] = array(
    'name' => 'manage tm twitter settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'tm_twitter_signin',
  );

  return $permissions;
}

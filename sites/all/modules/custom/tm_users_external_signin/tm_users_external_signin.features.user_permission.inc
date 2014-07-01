<?php
/**
 * @file
 * tm_users_external_signin.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_users_external_signin_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer consumers'.
  $permissions['administer consumers'] = array(
    'name' => 'administer consumers',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'oauth_common',
  );

  // Exported permission: 'administer oauth'.
  $permissions['administer oauth'] = array(
    'name' => 'administer oauth',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'oauth_common',
  );

  // Exported permission: 'manage tm twitter settings'.
  $permissions['manage tm twitter settings'] = array(
    'name' => 'manage tm twitter settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'tm_users_external_signin',
  );

  // Exported permission: 'oauth authorize any consumers'.
  $permissions['oauth authorize any consumers'] = array(
    'name' => 'oauth authorize any consumers',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'oauth_common',
  );

  // Exported permission: 'oauth register any consumers'.
  $permissions['oauth register any consumers'] = array(
    'name' => 'oauth register any consumers',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'oauth_common',
  );

  return $permissions;
}

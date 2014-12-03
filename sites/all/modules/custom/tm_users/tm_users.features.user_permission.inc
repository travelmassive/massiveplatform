<?php
/**
 * @file
 * tm_users.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_users_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access user profiles'.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'user',
  );

  // Exported permission: 'approve users'.
  $permissions['approve users'] = array(
    'name' => 'approve users',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'tm_users',
  );

  // Exported permission: 'flag follow_members'.
  $permissions['flag follow_members'] = array(
    'name' => 'flag follow_members',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unapprove users'.
  $permissions['unapprove users'] = array(
    'name' => 'unapprove users',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'tm_users',
  );

  // Exported permission: 'unflag follow_members'.
  $permissions['unflag follow_members'] = array(
    'name' => 'unflag follow_members',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
  );

  return $permissions;
}

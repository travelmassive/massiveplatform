<?php

/**
 * @file
 * tm_organizations.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_organizations_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create organization content'.
  $permissions['create organization content'] = array(
    'name' => 'create organization content',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any organization content'.
  $permissions['delete any organization content'] = array(
    'name' => 'delete any organization content',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own organization content'.
  $permissions['delete own organization content'] = array(
    'name' => 'delete own organization content',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any organization content'.
  $permissions['edit any organization content'] = array(
    'name' => 'edit any organization content',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own organization content'.
  $permissions['edit own organization content'] = array(
    'name' => 'edit own organization content',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'flag follow_organizations'.
  $permissions['flag follow_organizations'] = array(
    'name' => 'flag follow_organizations',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag follow_organizations'.
  $permissions['unflag follow_organizations'] = array(
    'name' => 'unflag follow_organizations',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  return $permissions;
}

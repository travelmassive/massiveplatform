<?php

/**
 * @file
 * tm_lists.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_lists_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create list content'.
  $permissions['create list content'] = array(
    'name' => 'create list content',
    'roles' => array(
      'list-editor' => 'list-editor',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any list content'.
  $permissions['delete any list content'] = array(
    'name' => 'delete any list content',
    'roles' => array(
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own list content'.
  $permissions['delete own list content'] = array(
    'name' => 'delete own list content',
    'roles' => array(
      'list-editor' => 'list-editor',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any list content'.
  $permissions['edit any list content'] = array(
    'name' => 'edit any list content',
    'roles' => array(
      'list-editor' => 'list-editor',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own list content'.
  $permissions['edit own list content'] = array(
    'name' => 'edit own list content',
    'roles' => array(
      'list-editor' => 'list-editor',
      'moderator' => 'moderator',
    ),
    'module' => 'node',
  );

  return $permissions;
}

<?php
/**
 * @file
 * tm_events.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_events_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create event content'.
  $permissions['create event content'] = array(
    'name' => 'create event content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create partner_event content'.
  $permissions['create partner_event content'] = array(
    'name' => 'create partner_event content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any event content'.
  $permissions['delete any event content'] = array(
    'name' => 'delete any event content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any partner_event content'.
  $permissions['delete any partner_event content'] = array(
    'name' => 'delete any partner_event content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own event content'.
  $permissions['delete own event content'] = array(
    'name' => 'delete own event content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own partner_event content'.
  $permissions['delete own partner_event content'] = array(
    'name' => 'delete own partner_event content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any event content'.
  $permissions['edit any event content'] = array(
    'name' => 'edit any event content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any partner_event content'.
  $permissions['edit any partner_event content'] = array(
    'name' => 'edit any partner_event content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own event content'.
  $permissions['edit own event content'] = array(
    'name' => 'edit own event content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own partner_event content'.
  $permissions['edit own partner_event content'] = array(
    'name' => 'edit own partner_event content',
    'roles' => array(),
    'module' => 'node',
  );

  return $permissions;
}

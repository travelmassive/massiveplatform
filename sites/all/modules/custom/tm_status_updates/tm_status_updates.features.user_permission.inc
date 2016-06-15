<?php
/**
 * @file
 * tm_status_updates.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_status_updates_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'flag status_updated_by_chapter'.
  $permissions['flag status_updated_by_chapter'] = array(
    'name' => 'flag status_updated_by_chapter',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: 'flag status_updated_by_event'.
  $permissions['flag status_updated_by_event'] = array(
    'name' => 'flag status_updated_by_event',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: 'flag status_updated_by_member'.
  $permissions['flag status_updated_by_member'] = array(
    'name' => 'flag status_updated_by_member',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: 'flag status_updated_by_organization'.
  $permissions['flag status_updated_by_organization'] = array(
    'name' => 'flag status_updated_by_organization',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: 'unflag status_updated_by_chapter'.
  $permissions['unflag status_updated_by_chapter'] = array(
    'name' => 'unflag status_updated_by_chapter',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: 'unflag status_updated_by_event'.
  $permissions['unflag status_updated_by_event'] = array(
    'name' => 'unflag status_updated_by_event',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: 'unflag status_updated_by_member'.
  $permissions['unflag status_updated_by_member'] = array(
    'name' => 'unflag status_updated_by_member',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: 'unflag status_updated_by_organization'.
  $permissions['unflag status_updated_by_organization'] = array(
    'name' => 'unflag status_updated_by_organization',
    'roles' => array(),
    'module' => 'flag',
  );

  return $permissions;
}

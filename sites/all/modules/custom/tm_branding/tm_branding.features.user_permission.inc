<?php
/**
 * @file
 * tm_branding.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_branding_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create site_brand content'.
  $permissions['create site_brand content'] = array(
    'name' => 'create site_brand content',
    'roles' => array(
      'administrator' => 'administrator',
      'brand-editor' => 'brand-editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any site_brand content'.
  $permissions['delete any site_brand content'] = array(
    'name' => 'delete any site_brand content',
    'roles' => array(
      'administrator' => 'administrator',
      'brand-editor' => 'brand-editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own site_brand content'.
  $permissions['delete own site_brand content'] = array(
    'name' => 'delete own site_brand content',
    'roles' => array(
      'administrator' => 'administrator',
      'brand-editor' => 'brand-editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any site_brand content'.
  $permissions['edit any site_brand content'] = array(
    'name' => 'edit any site_brand content',
    'roles' => array(
      'administrator' => 'administrator',
      'brand-editor' => 'brand-editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own site_brand content'.
  $permissions['edit own site_brand content'] = array(
    'name' => 'edit own site_brand content',
    'roles' => array(
      'administrator' => 'administrator',
      'brand-editor' => 'brand-editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}

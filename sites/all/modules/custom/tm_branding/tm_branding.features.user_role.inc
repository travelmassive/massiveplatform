<?php
/**
 * @file
 * tm_branding.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tm_branding_user_default_roles() {
  $roles = array();

  // Exported role: brand-editor.
  $roles['brand-editor'] = array(
    'name' => 'brand-editor',
    'weight' => 10,
  );

  return $roles;
}

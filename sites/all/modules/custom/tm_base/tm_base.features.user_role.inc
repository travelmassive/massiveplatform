<?php
/**
 * @file
 * tm_base.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tm_base_user_default_roles() {
  $roles = array();

  // Exported role: chapter leader.
  $roles['chapter leader'] = array(
    'name' => 'chapter leader',
    'weight' => 3,
  );

  return $roles;
}

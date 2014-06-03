<?php
/**
 * @file
 * tm_users.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tm_users_user_default_roles() {
  $roles = array();

  // Exported role: non-validated.
  $roles['non-validated'] = array(
    'name' => 'non-validated',
    'weight' => 3,
  );

  return $roles;
}

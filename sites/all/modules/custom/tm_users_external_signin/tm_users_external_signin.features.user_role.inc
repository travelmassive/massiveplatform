<?php
/**
 * @file
 * tm_users_external_signin.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tm_users_external_signin_user_default_roles() {
  $roles = array();

  // Exported role: non-validated.
  $roles['non-validated'] = array(
    'name' => 'non-validated',
    'weight' => 4,
  );

  return $roles;
}

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

  // Exported role: moderator.
  $roles['moderator'] = array(
    'name' => 'moderator',
    'weight' => 2,
  );

  return $roles;
}

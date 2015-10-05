<?php
/**
 * @file
 * tm_discuss.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tm_discuss_user_default_roles() {
  $roles = array();

  // Exported role: discuss-administrator.
  $roles['discuss-administrator'] = array(
    'name' => 'discuss-administrator',
    'weight' => 7,
  );

  // Exported role: discuss-moderator.
  $roles['discuss-moderator'] = array(
    'name' => 'discuss-moderator',
    'weight' => 6,
  );

  return $roles;
}

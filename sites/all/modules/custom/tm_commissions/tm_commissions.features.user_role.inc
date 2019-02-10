<?php

/**
 * @file
 * tm_commissions.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tm_commissions_user_default_roles() {
  $roles = array();

  // Exported role: pro chapter leader.
  $roles['pro chapter leader'] = array(
    'name' => 'pro chapter leader',
    'weight' => 12,
  );

  return $roles;
}

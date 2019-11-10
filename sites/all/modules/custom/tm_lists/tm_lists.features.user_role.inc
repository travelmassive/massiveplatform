<?php

/**
 * @file
 * tm_lists.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tm_lists_user_default_roles() {
  $roles = array();

  // Exported role: list-editor.
  $roles['list-editor'] = array(
    'name' => 'list-editor',
    'weight' => 13,
  );

  return $roles;
}

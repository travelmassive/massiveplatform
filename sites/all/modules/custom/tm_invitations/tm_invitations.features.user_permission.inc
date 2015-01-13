<?php
/**
 * @file
 * tm_invitations.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_invitations_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'tm invite all chapters'.
  $permissions['tm invite all chapters'] = array(
    'name' => 'tm invite all chapters',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'tm_invitations',
  );

  // Exported permission: 'tm invite own chapters'.
  $permissions['tm invite own chapters'] = array(
    'name' => 'tm invite own chapters',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'tm_invitations',
  );

  // Exported permission: 'tm invite unlimited'.
  $permissions['tm invite unlimited'] = array(
    'name' => 'tm invite unlimited',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'tm_invitations',
  );

  // Exported permission: 'tm invite users'.
  $permissions['tm invite users'] = array(
    'name' => 'tm invite users',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'tm_invitations',
  );

  return $permissions;
}

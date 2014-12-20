<?php
/**
 * @file
 * tm_flags.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_flags_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'flag event_register'.
  $permissions['flag event_register'] = array(
    'name' => 'flag event_register',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag event_waitlist'.
  $permissions['flag event_waitlist'] = array(
    'name' => 'flag event_waitlist',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag signup'.
  $permissions['flag signup'] = array(
    'name' => 'flag signup',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag event_register'.
  $permissions['unflag event_register'] = array(
    'name' => 'unflag event_register',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag event_waitlist'.
  $permissions['unflag event_waitlist'] = array(
    'name' => 'unflag event_waitlist',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag signup'.
  $permissions['unflag signup'] = array(
    'name' => 'unflag signup',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  return $permissions;
}

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

  // Exported role: access-google-analytics-report.
  $roles['access-google-analytics-report'] = array(
    'name' => 'access-google-analytics-report',
    'weight' => 9,
  );

  // Exported role: approved user.
  $roles['approved user'] = array(
    'name' => 'approved user',
    'weight' => 2,
  );

  // Exported role: chapter leader.
  $roles['chapter leader'] = array(
    'name' => 'chapter leader',
    'weight' => 3,
  );

  // Exported role: export-global-newsletter-csv.
  $roles['export-global-newsletter-csv'] = array(
    'name' => 'export-global-newsletter-csv',
    'weight' => 8,
  );

  // Exported role: moderator.
  $roles['moderator'] = array(
    'name' => 'moderator',
    'weight' => 5,
  );

  return $roles;
}

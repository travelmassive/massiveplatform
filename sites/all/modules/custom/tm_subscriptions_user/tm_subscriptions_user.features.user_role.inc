<?php
/**
 * @file
 * tm_subscriptions_user.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tm_subscriptions_user_user_default_roles() {
  $roles = array();

  // Exported role: subscriber.
  $roles['subscriber'] = array(
    'name' => 'subscriber',
    'weight' => 11,
  );

  return $roles;
}

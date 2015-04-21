<?php
/**
 * @file
 * tm_messaging.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tm_messaging_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'flag message_sent'.
  $permissions['flag message_sent'] = array(
    'name' => 'flag message_sent',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag message_sent'.
  $permissions['unflag message_sent'] = array(
    'name' => 'unflag message_sent',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  return $permissions;
}

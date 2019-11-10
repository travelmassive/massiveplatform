<?php

// tm_chapters.grants.inc
// Allow views to display events
// nodeaccess_nodereference, nodeaccess_userreference don't provide 'view' grants for performance
// Reference: deeson.co.uk/labs/drupal-7-node-access-grants

/**
 * Implements hook_node_access_records().
 */
function tm_chapters_node_access_records($node) {

  $grants = array();

  if ($node->type == 'chapter' && $node->status) {
    $grants[] = array(
      'realm' => 'tm_chapters',
      'gid' => 888,
      'grant_view' => 1,
      'grant_update' => 0,
      'grant_delete' => 0,
      'priority' => 0,
    );
  }

  return $grants;
}

/**
 * Implements hook_node_grants().
 */
function tm_chapters_node_grants($account, $op) {
  $grants = array();

  if ($op == 'view') {
    // Within our realm users with the correct
    // permission have a gid of 888.
    $grants['tm_chapters'][] = array(888);
  }

  return $grants;
}

<?php

// tm_organizations.helpers.inc - organization helper methods

/**
 * Get count of how many followers a company has
 */
function tm_organizations_get_num_followers($nid) {

  $flag = flag_get_flag("follow_organizations");
  $query = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_id = :nid AND f.entity_type = 'node'";
  $result = db_query($query, array(':fid' => $flag->fid, ':nid' => $nid))->fetch();
  return $result->total;
}

/**
 * Checks if a user a manager of a company profile
 */
function tm_organizations_is_company_profile_manager($uid, $company_nid) {
  
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'organization')
    ->propertyCondition('nid', $company_nid)
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_owner', 'target_id', $uid, '=');

  $result = $query->execute();

  if (isset($result['node'])) {
    return TRUE;
  }
  return FALSE;
}
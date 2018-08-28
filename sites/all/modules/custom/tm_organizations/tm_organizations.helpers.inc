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

/** 
 * Return an array of organization profiles the member is managing
 */
function tm_organizations_get_users_companies($uid) {

  // Faster query thatn EntityFieldQuery()
  $query = "SELECT entity_id FROM field_data_field_owner WHERE bundle = 'organization' AND deleted = 0 AND field_owner_target_id = :uid";
  $rows = db_query($query, array(':uid' => $uid))->fetchAll();

  // return result
  $companies_nids = array();
  foreach ($rows as $row) {
    $companies_nids[] = $row->entity_id;
  }

  // return companies
  return $companies_nids;
}

/**
 * Get taxononomy tid of an organization
 * Return tid or null if not found
 */
function tm_organizations_get_taxonomy_tid($nid) {

  $query = "SELECT field_segment_tid FROM field_data_field_segment WHERE entity_type = 'node' AND bundle = 'organization' AND entity_id = :nid LIMIT 1";
  $rows = db_query($query, array(':nid' => $nid))->fetchAll();

  // return result
  foreach ($rows as $row) {
    return $row->field_segment_tid;
  }

  // none
  return null;
}


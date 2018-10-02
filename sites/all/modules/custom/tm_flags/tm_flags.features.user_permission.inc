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

  // Exported permission: 'flag add_member_to_chapter'.
  $permissions['flag add_member_to_chapter'] = array(
    'name' => 'flag add_member_to_chapter',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag announcement_last_sent'.
  $permissions['flag announcement_last_sent'] = array(
    'name' => 'flag announcement_last_sent',
    'roles' => array(
      'approved user' => 'approved user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag approval_approved_by'.
  $permissions['flag approval_approved_by'] = array(
    'name' => 'flag approval_approved_by',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'chapter leader' => 'chapter leader',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag approval_is_company_or_brand'.
  $permissions['flag approval_is_company_or_brand'] = array(
    'name' => 'flag approval_is_company_or_brand',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag approval_need_more_info'.
  $permissions['flag approval_need_more_info'] = array(
    'name' => 'flag approval_need_more_info',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag approval_requested_by_user'.
  $permissions['flag approval_requested_by_user'] = array(
    'name' => 'flag approval_requested_by_user',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag download_member_csv'.
  $permissions['flag download_member_csv'] = array(
    'name' => 'flag download_member_csv',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

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

  // Exported permission: 'flag featured_member'.
  $permissions['flag featured_member'] = array(
    'name' => 'flag featured_member',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag follow_members_log'.
  $permissions['flag follow_members_log'] = array(
    'name' => 'flag follow_members_log',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'authenticated user' => 'authenticated user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag follow_organizations_log'.
  $permissions['flag follow_organizations_log'] = array(
    'name' => 'flag follow_organizations_log',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'authenticated user' => 'authenticated user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag moderate_non_community_profile'.
  $permissions['flag moderate_non_community_profile'] = array(
    'name' => 'flag moderate_non_community_profile',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag moderate_report_member'.
  $permissions['flag moderate_report_member'] = array(
    'name' => 'flag moderate_report_member',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag moderate_report_organization'.
  $permissions['flag moderate_report_organization'] = array(
    'name' => 'flag moderate_report_organization',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag remove_member_from_chapter'.
  $permissions['flag remove_member_from_chapter'] = array(
    'name' => 'flag remove_member_from_chapter',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
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

  // Exported permission: 'flag tm_sponsor'.
  $permissions['flag tm_sponsor'] = array(
    'name' => 'flag tm_sponsor',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag add_member_to_chapter'.
  $permissions['unflag add_member_to_chapter'] = array(
    'name' => 'unflag add_member_to_chapter',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag announcement_last_sent'.
  $permissions['unflag announcement_last_sent'] = array(
    'name' => 'unflag announcement_last_sent',
    'roles' => array(
      'approved user' => 'approved user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag approval_approved_by'.
  $permissions['unflag approval_approved_by'] = array(
    'name' => 'unflag approval_approved_by',
    'roles' => array(
      'administrator' => 'administrator',
      'approved user' => 'approved user',
      'chapter leader' => 'chapter leader',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag approval_is_company_or_brand'.
  $permissions['unflag approval_is_company_or_brand'] = array(
    'name' => 'unflag approval_is_company_or_brand',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag approval_need_more_info'.
  $permissions['unflag approval_need_more_info'] = array(
    'name' => 'unflag approval_need_more_info',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag approval_requested_by_user'.
  $permissions['unflag approval_requested_by_user'] = array(
    'name' => 'unflag approval_requested_by_user',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'non-validated' => 'non-validated',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag download_member_csv'.
  $permissions['unflag download_member_csv'] = array(
    'name' => 'unflag download_member_csv',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
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

  // Exported permission: 'unflag featured_member'.
  $permissions['unflag featured_member'] = array(
    'name' => 'unflag featured_member',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag follow_members_log'.
  $permissions['unflag follow_members_log'] = array(
    'name' => 'unflag follow_members_log',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag follow_organizations_log'.
  $permissions['unflag follow_organizations_log'] = array(
    'name' => 'unflag follow_organizations_log',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag moderate_non_community_profile'.
  $permissions['unflag moderate_non_community_profile'] = array(
    'name' => 'unflag moderate_non_community_profile',
    'roles' => array(
      'administrator' => 'administrator',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag moderate_report_member'.
  $permissions['unflag moderate_report_member'] = array(
    'name' => 'unflag moderate_report_member',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag moderate_report_organization'.
  $permissions['unflag moderate_report_organization'] = array(
    'name' => 'unflag moderate_report_organization',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag remove_member_from_chapter'.
  $permissions['unflag remove_member_from_chapter'] = array(
    'name' => 'unflag remove_member_from_chapter',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'chapter leader' => 'chapter leader',
      'moderator' => 'moderator',
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

  // Exported permission: 'unflag tm_sponsor'.
  $permissions['unflag tm_sponsor'] = array(
    'name' => 'unflag tm_sponsor',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'flag',
  );

  return $permissions;
}

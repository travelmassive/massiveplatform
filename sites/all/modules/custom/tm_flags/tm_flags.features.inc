<?php
/**
 * @file
 * tm_flags.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_flags_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tm_flags_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function tm_flags_flag_default_flags() {
  $flags = array();
  // Exported flag: "Approved by".
  $flags['approval_approved_by'] = array(
    'entity_type' => 'user',
    'title' => 'Approved by',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Flag this item',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unflag this item',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'search_result' => 0,
      'grid' => 0,
      'list' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'show_on_profile' => 0,
    'access_uid' => 'others',
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Approval is Company or Brand".
  $flags['approval_is_company_or_brand'] = array(
    'entity_type' => 'user',
    'title' => 'Approval is Company or Brand',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Flag this item',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unflag this item',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'search_result' => 0,
      'grid' => 0,
      'list' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'show_on_profile' => 0,
    'access_uid' => 'others',
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Approval Need More Info".
  $flags['approval_need_more_info'] = array(
    'entity_type' => 'user',
    'title' => 'Approval Need More Info',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Flag this item',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unflag this item',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'search_result' => 0,
      'grid' => 0,
      'list' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'show_on_profile' => 0,
    'access_uid' => 'others',
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Approval Requested by User".
  $flags['approval_requested_by_user'] = array(
    'entity_type' => 'user',
    'title' => 'Approval Requested by User',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Flag this item',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unflag this item',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'search_result' => 0,
      'grid' => 0,
      'list' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'show_on_profile' => 0,
    'access_uid' => '',
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Event Register".
  $flags['event_register'] = array(
    'entity_type' => 'node',
    'title' => 'Event Register',
    'global' => 0,
    'types' => array(
      0 => 'event',
    ),
    'flag_short' => 'Register',
    'flag_long' => 'Register for [node:title]',
    'flag_message' => 'You successfully registered for [node:title]',
    'unflag_short' => 'Registered',
    'unflag_long' => 'Unregister for [node:title]',
    'unflag_message' => 'You successfuly unregistered for [node:title]',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
      'search_index' => 0,
      'search_result' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'grid' => 0,
      'list' => 0,
      'revision' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Event Waitlist".
  $flags['event_waitlist'] = array(
    'entity_type' => 'node',
    'title' => 'Event Waitlist',
    'global' => 0,
    'types' => array(
      0 => 'event',
    ),
    'flag_short' => 'Join Waitlist',
    'flag_long' => 'Join Waitlist for [node:title]',
    'flag_message' => 'You successfully joined the waitlist for [node:title]',
    'unflag_short' => 'Leave Waitlist',
    'unflag_long' => 'Leave Waitlist for [node:title]',
    'unflag_message' => 'You successfuly left the Waitlist for [node:title]',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
      'search_index' => 0,
      'search_result' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'grid' => 0,
      'list' => 0,
      'revision' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Follow members".
  $flags['follow_members'] = array(
    'entity_type' => 'user',
    'title' => 'Follow members',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Follow',
    'flag_long' => 'Follow [user:name]',
    'flag_message' => 'You successfully followed [user:name]',
    'unflag_short' => 'Following',
    'unflag_long' => 'Unfollow [user:name]',
    'unflag_message' => 'You successfuly unfollowed [user:name]',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'search_result' => 0,
      'grid' => 0,
      'list' => 0,
    ),
    'show_as_field' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'show_on_profile' => 0,
    'access_uid' => 'others',
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Follow organizations".
  $flags['follow_organizations'] = array(
    'entity_type' => 'node',
    'title' => 'Follow organizations',
    'global' => 0,
    'types' => array(
      0 => 'organization',
    ),
    'flag_short' => 'Follow',
    'flag_long' => 'Follow [node:title]',
    'flag_message' => 'You successfully followed [node:title]',
    'unflag_short' => 'Following',
    'unflag_long' => 'Unfollow [node:title]',
    'unflag_message' => 'You successfuly unfollowed [node:title]',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'grid' => 0,
      'list' => 0,
      'revision' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Join Chapter".
  $flags['signup'] = array(
    'entity_type' => 'node',
    'title' => 'Join Chapter',
    'global' => 0,
    'types' => array(
      0 => 'chapter',
    ),
    'flag_short' => 'Join',
    'flag_long' => 'Join [node:title]',
    'flag_message' => 'You successfully joined [node:title]',
    'unflag_short' => 'Joined',
    'unflag_long' => 'Unjoin [node:title]',
    'unflag_message' => 'You successfuly unjoined [node:title]',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
      'search_index' => 0,
      'search_result' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'grid' => 0,
      'list' => 0,
      'revision' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Waitlist".
  $flags['waitlist'] = array(
    'entity_type' => 'node',
    'title' => 'Waitlist',
    'global' => 0,
    'types' => array(
      0 => 'event',
    ),
    'flag_short' => 'Join Waitlist',
    'flag_long' => 'Join Waitlist for [node:title]',
    'flag_message' => 'You successfully joined the waitlist for [node:title]',
    'unflag_short' => 'Leave Waitlist',
    'unflag_long' => 'Leave Waitlist for [node:title]',
    'unflag_message' => 'You successfuly left the Waitlist for [node:title]',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'teaser' => 0,
      'rss' => 0,
      'search_index' => 0,
      'search_result' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'grid' => 0,
      'list' => 0,
      'revision' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'i18n' => 0,
    'api_version' => 3,
    'module' => 'tm_flags',
    'locked' => array(
      0 => 'name',
    ),
    'status' => FALSE,
  );
  return $flags;

}

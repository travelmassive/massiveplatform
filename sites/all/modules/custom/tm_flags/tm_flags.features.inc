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
  // Exported flag: "Follow members".
  $flags['follow_members'] = array(
    'entity_type' => 'user',
    'title' => 'Follow members',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Follow',
    'flag_long' => 'Follow [user:name]',
    'flag_message' => 'You successfully followed [user:name]',
    'unflag_short' => 'Unfollow',
    'unflag_long' => 'Unfollow [user:name]',
    'unflag_message' => 'You successfuly unfollowed [user:name]',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 0,
      'diff_standard' => 0,
      'token' => 0,
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
    'unflag_short' => 'Unfollow',
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
  // Exported flag: "Signup".
  $flags['signup'] = array(
    'entity_type' => 'node',
    'title' => 'Signup',
    'global' => 0,
    'types' => array(
      0 => 'chapter',
      1 => 'event',
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
  return $flags;

}

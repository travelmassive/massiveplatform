<?php
/**
 * @file
 * tm_event_signup.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_event_signup_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tm_event_signup_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function tm_event_signup_flag_default_flags() {
  $flags = array();
  // Exported flag: "Signup".
  $flags['signup'] = array(
    'entity_type' => 'node',
    'title' => 'Signup',
    'global' => 0,
    'types' => array(
      0 => 'event',
    ),
    'flag_short' => 'Join',
    'flag_long' => 'Join this event',
    'flag_message' => 'You successfully signed up for this event.',
    'unflag_short' => 'Joined',
    'unflag_long' => 'Unjoin from this event',
    'unflag_message' => 'You successfuly unjoined.',
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
    'module' => 'tm_event_signup',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

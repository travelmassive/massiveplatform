<?php
/**
 * @file
 * tm_messaging.features.inc
 */

/**
 * Implements hook_flag_default_flags().
 */
function tm_messaging_flag_default_flags() {
  $flags = array();
  // Exported flag: "Message sent to member".
  $flags['message_sent'] = array(
    'entity_type' => 'user',
    'title' => 'Message sent to member',
    'global' => 0,
    'types' => array(),
    'flag_short' => 'Messaged',
    'flag_long' => 'Messaged [user:name]',
    'flag_message' => 'You messaged [user:name]',
    'unflag_short' => 'Unflag messaged',
    'unflag_long' => 'Unflag messaged [user:name]',
    'unflag_message' => 'You unflagged messaged [user:name]',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
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
    'api_version' => 3,
    'module' => 'tm_messaging',
    'locked' => array(
      0 => 'name',
    ),
  );
  return $flags;

}

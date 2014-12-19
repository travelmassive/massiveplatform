<?php
/**
 * @file
 * tm_notifications_newsletters.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_notifications_newsletters_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__chapter_newsletters';
  $strongarm->value = array(
    'user_label' => 'Subscribe to Local Chapter Newsletters',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '',
      'message' => '',
    ),
  );
  $export['tm_notifications__chapter_newsletters'] = $strongarm;

   $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_notifications__global_newsletter';
  $strongarm->value = array(
    'user_label' => 'Subscribe to Global Newsletter',
    'default_state' => '1',
    'change_allowed' => 1,
    'email' => array(
      'subject' => '',
      'message' => '',
    ),
  );
  $export['tm_notifications__global_newsletter'] = $strongarm;

  return $export;
}

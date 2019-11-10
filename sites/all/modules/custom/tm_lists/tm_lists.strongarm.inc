<?php

/**
 * @file
 * tm_list.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_lists_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_tm_list';
  $strongarm->value = array();
  $export['menu_options_tm_list'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_tm_list';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_tm_list'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_tm_list';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_tm_list'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_tm_list';
  $strongarm->value = 0;
  $export['node_submitted_tm_list'] = $strongarm;

  return $export;
}

<?php
/**
 * @file
 * tm_base.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_base_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeaccess_nodereference';
  $strongarm->value = array(
    'event' => array(
      'field_chapter' => array(
        'referenced' => array(
          'update' => array(
            'update' => 'update',
          ),
          'view' => array(
            'view' => 'view',
          ),
        ),
        'field_type' => 'entityreference',
      ),
    ),
  );
  $export['nodeaccess_nodereference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeaccess_nodereference_cross_access';
  $strongarm->value = array(
    'view' => FALSE,
    'update' => FALSE,
    'delete' => FALSE,
  );
  $export['nodeaccess_nodereference_cross_access'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeaccess_userreference';
  $strongarm->value = array(
    'chapter' => array(
      'field_chapter_leaders' => array(
        'referenced' => array(
          'update' => 'update',
        ),
        'create' => array(
          'event' => 'event',
        ),
        'author' => array(
          'update' => 'update',
        ),
        'all' => array(
          'view' => 'view',
        ),
      ),
    ),
  );
  $export['nodeaccess_userreference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_pattern';
  $strongarm->value = 'content/[node:title]';
  $export['pathauto_node_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_punctuation_slash';
  $strongarm->value = '2';
  $export['pathauto_punctuation_slash'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_frontpage';
  $strongarm->value = 'frontpage';
  $export['site_frontpage'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_base_copright';
  $strongarm->value = '&copy; All rights reserved.';
  $export['tm_base_copright'] = $strongarm;

  return $export;
}

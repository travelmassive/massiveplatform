<?php
/**
 * @file
 * tm_fields.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_fields_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeaccess_nodereference';
  $strongarm->value = array(
    'event' => array(
      'field_chapter_publish' => array(
        'author' => array(
          'view' => 'view',
        ),
        'referenced' => array(
          'view' => array(
            'view' => 'view',
          ),
        ),
        'unused' => 1,
        'field_type' => 'entityreference',
      ),
      'field_event_company' => array(
        'all' => array(
          'view' => 'view',
        ),
        'referenced' => array(
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
      'field_community_managers' => array(
        'all' => array(
          'view' => 'view',
        ),
        'author' => array(
          'update' => 'update',
        ),
        'create' => array(
          'event' => 'event',
        ),
        'referenced' => array(
          'update' => 'update',
        ),
      ),
    ),
    'organization' => array(
      'field_owner' => array(
        'all' => array(
          'view' => 'view',
        ),
        'author' => array(
          'delete' => 'delete',
          'update' => 'update',
          'view' => 'view',
        ),
        'referenced' => array(
          'delete' => 'delete',
          'update' => 'update',
          'view' => 'view',
        ),
      ),
    ),
    'event' => array(
      'field_organizer' => array(
        'referenced' => array(
          'delete' => 'delete',
          'update' => 'update',
          'view' => 'view',
        ),
        'author' => array(
          'delete' => 'delete',
          'update' => 'update',
          'view' => 'view',
        ),
      ),
      'field_event_featured_members' => array(
        'referenced' => array(
          'view' => 'view',
        ),
        'all' => array(
          'view' => 'view',
        ),
      ),
    ),
  );
  $export['nodeaccess_userreference'] = $strongarm;

  return $export;
}

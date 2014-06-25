<?php
/**
 * @file
 * tm_fields.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function tm_fields_taxonomy_default_vocabularies() {
  return array(
    'tm_segments' => array(
      'name' => 'Industry segment',
      'machine_name' => 'tm_segments',
      'description' => 'A list with industry segments',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}

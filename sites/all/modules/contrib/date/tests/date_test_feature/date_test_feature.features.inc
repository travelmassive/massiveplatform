<?php

/**
 * @file
 * date_test_feature.features.inc
 */

/**
 * Implements hook_node_info().
 */
function date_test_feature_node_info() {
  $items = array(
    'date_test_feature' => array(
      'name' => t('Date Example'),
      'base' => 'node_content',
      'description' => t('This content type is used for demonstrating and testing the Date module.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

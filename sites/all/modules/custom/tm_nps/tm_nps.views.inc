<?php

/**
 * @file
 * Views integration for the tm_nps module
 */

/**
 * Implements hook_views_data()
 *
 */
function tm_nps_views_data() {
  // Views Group
  $data['tm_nps']['table']['group'] = t('TM NPS');

  // Base Views
  $data['tm_nps']['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('Net Promoter Score Views'),
    'help' => t('TM NPS Views database table.'),
    'weight' => -10,
  );
  
  // ID table field.
  $data['tm_nps']['id'] = array(
    'title' => t('ID'),
    'help' => t('The primary identifier for a review.'),
  );

  $data['tm_nps']['table']['join'] = array(
      'users' => array(
          'left_field' => 'uid',
          'field' => 'uid',
      ),
  );
  
  // Uid field.
  $data['tm_nps']['uid'] = array(
    'title' => t('Uid'),
    'help' => t('The user uid who submitted review.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    // Add argument.
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'numeric' => TRUE,
      'validate type' => 'uid',
    ),
    // Add relationship.    
    'relationship' => array(
      'base' => 'users', // The name of the table to join with.
      'base field' => 'uid', // The name of the field on the joined table.
      'handler' => 'views_handler_relationship',
      'label' => t('user_uid'),
      'title' => t('User uid'),
      'help' => t('Relationship for the user profile'),
    ),
  );
  
  // Score field.
  $data['tm_nps']['score'] = array(
    'title' => t('Score'),
    'help' => t('The Net Promoter Score by the member submitting feedback.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    // Add argument.
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
  );
  
  // Comment field.
  $data['tm_nps']['comment'] = array(
    'title' => t('Comment'),
    'help' => t('Comment by the member submitting feedback.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE, // This is use by the table display plugin.
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // Chapter Nid field.
  $data['tm_nps']['chapter_id'] = array(
    'title' => t('Chapter id'),
    'help' => t('The home chapter nid of the reviewer.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    // Add argument.
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    // Add relationship.    
    'relationship' => array(
      'base' => 'node', // The name of the table to join with.
      'base field' => 'nid', // The name of the field on the joined table.
      'handler' => 'views_handler_relationship',
      'label' => t('chapter_id'),
      'title' => t('Chapter id'),
      'help' => t('Relationship for the chapter id'),
    ),
  );

  // Url field.
  $data['tm_nps']['url'] = array(
    'title' => t('URL'),
    'help' => t('URL when reviewing.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE, // This is use by the table display plugin.
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
     
  // Timestamp field.
  $data['tm_nps']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('When this feedback was submitted.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  
  return $data;
}

<?php

/**
 * @file
 * Views integration for the tm_invitations module
 */

/**
 * Implements hook_views_data()
 *
 */
function tm_invitations_views_data() {
  // Views Group
  $data['tm_invitations']['table']['group'] = t('TM invitations');

  // Base Views
  $data['tm_invitations']['table']['base'] = array(
    'field' => 'id', // This is the identifier field for the view.
    'title' => t('Invitations Views'),
    'help' => t('TM invitations Views database table.'),
    'weight' => -10,
  );
  
  // ID table field.
  $data['tm_invitations']['id'] = array(
    'title' => t('ID'),
    'help' => t('The primary identifier for an invitation.'),
  );

  $data['tm_invitations']['table']['join'] = array(
      'users' => array(
          'left_field' => 'email',
          'field' => 'user_email',
      ),
  );
  
  // Uid field.
  $data['tm_invitations']['uid'] = array(
    'title' => t('Uid'),
    'help' => t('The user uid who sent this invitation.'),
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
  );
  
  // Nid field.
  $data['tm_invitations']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node nid to which the user was invited.'),
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
  
  // Email field.
  $data['tm_invitations']['email'] = array(
    'title' => t('Email'),
    'help' => t('The email address of the person being invited.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Add relationship.    
    'relationship' => array(
      'base' => 'users', // The name of the table to join with.
      'base field' => 'mail', // The name of the field on the joined table.
      'handler' => 'views_handler_relationship',
      'label' => t('user_email'),
      'title' => t('User email'),
      'help' => t('Relationship for the user profile'),
    ),
  );
     
  // Created field.
  $data['tm_invitations']['created'] = array(
    'title' => t('Created'),
    'help' => t('When this invitation was generated.'),
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

  // Accepted field.
  $data['tm_invitations']['accepted'] = array(
    'title' => t('Accepted'),
    'help' => t('When this invitation was accepted.'),
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
  
  // Inviter UID field.
  $data['tm_invitations']['uid'] = array(
    'title' => t('Inviter UID'),
    'help' => t('The inviter UID.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Add relationship.    
    'relationship' => array(
      'base' => 'users', // The name of the table to join with.
      'base field' => 'uid', // The name of the field on the joined table.
      'handler' => 'views_handler_relationship',
      'label' => t('inviter_uid'),
      'title' => t('Inviter UID'),
      'help' => t('Relationship for the inviter uid'),
    ),
  );
  
  return $data;
}

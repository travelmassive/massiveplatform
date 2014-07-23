<?php
/**
 * @file
 * tm_users.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_users_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_action_menu_wrapper|user|user|default';
  $field_group->group_name = 'group_action_menu_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Action menu wrapper',
    'weight' => '4',
    'children' => array(
      0 => 'group_action_menu',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Action menu wrapper',
      'instance_settings' => array(
        'classes' => 'actions',
        'element' => 'div',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
        'id' => '',
      ),
    ),
  );
  $export['group_action_menu_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_action_menu|user|user|default';
  $field_group->group_name = 'group_action_menu';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_action_menu_wrapper';
  $field_group->data = array(
    'label' => 'Action menu',
    'weight' => '8',
    'children' => array(
      0 => 'flag_follow_members',
      1 => 'tm_ds_more_actions',
      2 => 'tm_ds_anon_flag_placeholder',
      3 => 'tm_ds_edit_own_profile',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Action menu',
      'instance_settings' => array(
        'classes' => 'actions-menu',
        'element' => 'ul',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
        'id' => '',
      ),
    ),
  );
  $export['group_action_menu|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_city_list_wrapper|user|user|default';
  $field_group->group_name = 'group_city_list_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Cities joined',
    'weight' => '8',
    'children' => array(
      0 => 'joined_city_list',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Cities joined',
      'formatter' => 'open',
    ),
  );
  $export['group_city_list_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_community_wrapper|user|user|default';
  $field_group->group_name = 'group_community_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Followers',
    'weight' => '7',
    'children' => array(
      0 => 'members_following_members',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Followers',
      'formatter' => 'open',
    ),
  );
  $export['group_community_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details_wrapper|user|user|default';
  $field_group->group_name = 'group_details_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'About',
    'weight' => '6',
    'children' => array(
      0 => 'field_about_me',
      1 => 'field_link_facebook',
      2 => 'field_link_linkedin',
      3 => 'field_link_twitter',
      4 => 'field_link_website',
      5 => 'field_segment',
      6 => 'group_location_wrapper',
      7 => 'group_user_qa_wrapper',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'About',
      'formatter' => 'open',
    ),
  );
  $export['group_details_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_event_list_wrapper|user|user|default';
  $field_group->group_name = 'group_event_list_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Events joined',
    'weight' => '10',
    'children' => array(
      0 => 'joined_event_list',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Events joined',
      'formatter' => 'open',
    ),
  );
  $export['group_event_list_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_job_wrapper|user|user|default';
  $field_group->group_name = 'group_job_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Job wrapper',
    'weight' => '3',
    'children' => array(
      0 => 'field_job_role',
      1 => 'field_job_organization',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Job wrapper',
      'instance_settings' => array(
        'classes' => 'meta',
        'element' => 'p',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
      ),
    ),
  );
  $export['group_job_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_job_wrapper|user|user|grid';
  $field_group->group_name = 'group_job_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'grid';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Job wrapper',
    'weight' => '1',
    'children' => array(
      0 => 'field_job_role',
      1 => 'field_job_organization',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Job wrapper',
      'instance_settings' => array(
        'classes' => 'meta',
        'element' => 'p',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
      ),
    ),
  );
  $export['group_job_wrapper|user|user|grid'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location_wrapper|user|user|default';
  $field_group->group_name = 'group_location_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_details_wrapper';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '21',
    'children' => array(
      0 => 'field_location_city',
      1 => 'field_location_country',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Location',
      'instance_settings' => array(
        'classes' => 'detail-item field-location',
        'element' => 'div',
        'show_label' => '1',
        'label_element' => 'h2',
        'attributes' => '',
      ),
    ),
  );
  $export['group_location_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_qa_wrapper|user|user|default';
  $field_group->group_name = 'group_user_qa_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_details_wrapper';
  $field_group->data = array(
    'label' => 'QA',
    'weight' => '20',
    'children' => array(
      0 => 'group_user_qa',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'QA',
      'instance_settings' => array(
        'classes' => 'detail-item qa',
        'element' => 'div',
        'show_label' => '1',
        'label_element' => 'h2',
        'attributes' => '',
      ),
    ),
  );
  $export['group_user_qa_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_qa|user|user|default';
  $field_group->group_name = 'group_user_qa';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_user_qa_wrapper';
  $field_group->data = array(
    'label' => 'QA',
    'weight' => '17',
    'children' => array(
      0 => 'field_user_question_destination',
      1 => 'field_user_question_dream_travel',
      2 => 'field_user_question_job',
      3 => 'field_user_question_learn',
      4 => 'field_user_question_why_travel',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'QA',
      'instance_settings' => array(
        'classes' => '',
        'element' => 'dl',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
      ),
    ),
  );
  $export['group_user_qa|user|user|default'] = $field_group;

  return $export;
}

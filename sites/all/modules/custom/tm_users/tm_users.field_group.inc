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
    'weight' => '5',
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
  $field_group->identifier = 'group_chapter_list_wrapper|user|user|default';
  $field_group->group_name = 'group_chapter_list_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Chapters joined',
    'weight' => '8',
    'children' => array(
      0 => 'joined_chapter_list',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Chapters joined',
      'formatter' => 'open',
    ),
  );
  $export['group_chapter_list_wrapper|user|user|default'] = $field_group;

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
    'weight' => '9',
    'children' => array(
      0 => 'members_followed_by_members',
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
    'weight' => '7',
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
    'weight' => '12',
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
  $field_group->identifier = 'group_form_about|user|user|form';
  $field_group->group_name = 'group_form_about';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_profile';
  $field_group->data = array(
    'label' => 'About',
    'weight' => '31',
    'children' => array(
      0 => 'field_about_me',
      1 => 'field_avatar',
      2 => 'field_chapter',
      3 => 'field_image',
      4 => 'field_job_organization',
      5 => 'field_job_role',
      6 => 'field_location_city',
      7 => 'field_segment',
      8 => 'field_user_country',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'About',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'group-form-about field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_form_about|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_basic_settings|user|user|form';
  $field_group->group_name = 'group_form_basic_settings';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_account';
  $field_group->data = array(
    'label' => 'Basic settings',
    'weight' => '25',
    'children' => array(
      0 => 'field_friendly_url',
      1 => 'field_user_first_name',
      2 => 'field_user_last_name',
      3 => 'masquerade',
      4 => 'account',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Basic settings',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'group-form-basic-settings field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_form_basic_settings|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_elsewhere|user|user|form';
  $field_group->group_name = 'group_form_elsewhere';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_profile';
  $field_group->data = array(
    'label' => 'Elsewhere',
    'weight' => '33',
    'children' => array(
      0 => 'field_link_facebook',
      1 => 'field_link_linkedin',
      2 => 'field_link_twitter',
      3 => 'field_link_website',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Elsewhere',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'group-form-elsewhere field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_form_elsewhere|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_qa|user|user|form';
  $field_group->group_name = 'group_form_qa';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_profile';
  $field_group->data = array(
    'label' => 'A little more about you',
    'weight' => '32',
    'children' => array(
      0 => 'field_user_question_destination',
      1 => 'field_user_question_dream_travel',
      2 => 'field_user_question_job',
      3 => 'field_user_question_learn',
      4 => 'field_user_question_why_travel',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'A little more about you',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'group-form-qa field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_form_qa|user|user|form'] = $field_group;

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
      0 => 'field_job_organization',
      1 => 'field_job_role',
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
  $field_group->identifier = 'group_summary_wrapper|user|user|search_result';
  $field_group->group_name = 'group_summary_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'search_result';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Summary',
    'weight' => '2',
    'children' => array(
      0 => 'field_about_me',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Summary',
      'instance_settings' => array(
        'classes' => 'summary',
        'element' => 'div',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
      ),
    ),
  );
  $export['group_summary_wrapper|user|user|search_result'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_account|user|user|form';
  $field_group->group_name = 'group_user_account';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_edit';
  $field_group->data = array(
    'label' => 'Account',
    'weight' => '34',
    'children' => array(
      0 => 'timezone',
      1 => 'group_form_basic_settings',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Account',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => 'user-account-options',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_user_account|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_edit|user|user|form';
  $field_group->group_name = 'group_user_edit';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'User edit',
    'weight' => '0',
    'children' => array(
      0 => 'group_user_account',
      1 => 'group_user_notifications',
      2 => 'group_user_twitter_settings',
      3 => 'group_user_profile',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $export['group_user_edit|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_notifications|user|user|form';
  $field_group->group_name = 'group_user_notifications';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_edit';
  $field_group->data = array(
    'label' => 'Notifications',
    'weight' => '36',
    'children' => array(
      0 => 'tm_notifications',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Notifications',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => 'user-notifications-options',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_user_notifications|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_profile|user|user|form';
  $field_group->group_name = 'group_user_profile';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_edit';
  $field_group->data = array(
    'label' => 'Profile',
    'weight' => '35',
    'children' => array(
      0 => 'group_form_about',
      1 => 'group_form_elsewhere',
      2 => 'group_form_qa',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Profile',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => 'user-profile-options',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_user_profile|user|user|form'] = $field_group;

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

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_twitter_settings|user|user|form';
  $field_group->group_name = 'group_user_twitter_settings';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_edit';
  $field_group->data = array(
    'label' => 'Twitter',
    'weight' => '37',
    'children' => array(
      0 => 'tm_twitter_settings',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Twitter',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => 'user-twitter-options',
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_user_twitter_settings|user|user|form'] = $field_group;

  return $export;
}

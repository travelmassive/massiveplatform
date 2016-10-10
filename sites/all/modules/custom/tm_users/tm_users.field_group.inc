<?php
/**
 * @file
 * tm_users.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_users_field_group_info() {
  $field_groups = array();

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
  $field_groups['group_action_menu_wrapper|user|user|default'] = $field_group;

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
      2 => 'tm_ds_communication_actions',
      3 => 'tm_ds_anon_flag_placeholder',
      4 => 'tm_ds_edit_own_profile',
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
  $field_groups['group_action_menu|user|user|default'] = $field_group;

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
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $field_groups['group_chapter_list_wrapper|user|user|default'] = $field_group;

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
    'weight' => '10',
    'children' => array(
      0 => 'members_followed_by_members',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Followers',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_community_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_companies_list_wrapper|user|user|default';
  $field_group->group_name = 'group_companies_list_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Company Profiles',
    'weight' => '9',
    'children' => array(
      0 => 'list_users_companies',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Company Profiles',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_companies_list_wrapper|user|user|default'] = $field_group;

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
      6 => 'field_link_instagram',
      7 => 'field_link_snapchat',
      8 => 'field_link_vimeo',
      9 => 'field_link_youtube',
      10 => 'field_link_strava',
      11 => 'tm_ds_user_stats',
      12 => 'tm_ds_user_featured',
      13 => 'tm_ds_connection_actions',
      14 => 'group_location_wrapper',
      15 => 'group_user_qa_wrapper',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'About',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_details_wrapper|user|user|default'] = $field_group;

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
    'weight' => '13',
    'children' => array(
      0 => 'joined_event_list',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Events joined',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_event_list_wrapper|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_following_organizations|user|user|default';
  $field_group->group_name = 'group_following_organizations';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Following Organizations',
    'weight' => '12',
    'children' => array(
      0 => 'following_organizations',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => 'group-following-organizations field-group-contained_block',
      ),
    ),
  );
  $field_groups['group_following_organizations|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_following_wrapper|user|user|default';
  $field_group->group_name = 'group_following_wrapper';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Following',
    'weight' => '11',
    'children' => array(
      0 => 'members_following_members',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => 'group-following-wrapper field-group-contained_block',
      ),
    ),
  );
  $field_groups['group_following_wrapper|user|user|default'] = $field_group;

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
      2 => 'field_image',
      3 => 'field_job_organization',
      4 => 'field_job_role',
      5 => 'field_location_city',
      6 => 'field_segment',
      7 => 'field_user_country',
      8 => 'field_home_chapter',
      9 => 'field_user_cover_video',
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
  $field_groups['group_form_about|user|user|form'] = $field_group;

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
      3 => 'account',
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
  $field_groups['group_form_basic_settings|user|user|form'] = $field_group;

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
      4 => 'field_link_instagram',
      5 => 'field_link_snapchat',
      6 => 'field_link_vimeo',
      7 => 'field_link_youtube',
      8 => 'field_link_strava',
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
  $field_groups['group_form_elsewhere|user|user|form'] = $field_group;

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
      0 => 'field_user_question_1',
      1 => 'field_user_question_2',
      2 => 'field_user_question_3',
      3 => 'field_user_question_4',
      4 => 'field_user_question_5',
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
  $field_groups['group_form_qa|user|user|form'] = $field_group;

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
  $field_groups['group_job_wrapper|user|user|default'] = $field_group;

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
  $field_groups['group_job_wrapper|user|user|grid'] = $field_group;

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
    'weight' => '37',
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
  $field_groups['group_location_wrapper|user|user|default'] = $field_group;

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
  $field_groups['group_summary_wrapper|user|user|search_result'] = $field_group;

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
      0 => 'field_facebook_signin_id',
      1 => 'timezone',
      2 => 'group_form_basic_settings',
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
  $field_groups['group_user_account|user|user|form'] = $field_group;

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
      2 => 'group_user_privacy',
      3 => 'group_user_profile',
      4 => 'group_user_twitter_settings',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $field_groups['group_user_edit|user|user|form'] = $field_group;

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
  $field_groups['group_user_notifications|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_privacy|user|user|form';
  $field_group->group_name = 'group_user_privacy';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_user_edit';
  $field_group->data = array(
    'label' => 'Privacy',
    'weight' => '38',
    'children' => array(
      0 => 'field_user_disable_public_index',
      1 => 'field_user_has_private_profile',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-user-privacy field-group-fieldset',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_user_privacy|user|user|form'] = $field_group;

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
  $field_groups['group_user_profile|user|user|form'] = $field_group;

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
    'weight' => '36',
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
  $field_groups['group_user_qa_wrapper|user|user|default'] = $field_group;

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
      0 => 'field_user_question_1',
      1 => 'field_user_question_2',
      2 => 'field_user_question_3',
      3 => 'field_user_question_4',
      4 => 'field_user_question_5',
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
  $field_groups['group_user_qa|user|user|default'] = $field_group;

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
    'label' => 'Connect',
    'weight' => '37',
    'children' => array(
      0 => 'tm_connect_settings',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Connect',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $field_groups['group_user_twitter_settings|user|user|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('A little more about you');
  t('About');
  t('Account');
  t('Action menu');
  t('Action menu wrapper');
  t('Basic settings');
  t('Chapters joined');
  t('Company Profiles');
  t('Connect');
  t('Elsewhere');
  t('Events joined');
  t('Followers');
  t('Following');
  t('Following Organizations');
  t('Job wrapper');
  t('Location');
  t('Notifications');
  t('Privacy');
  t('Profile');
  t('QA');
  t('Summary');
  t('User edit');

  return $field_groups;
}

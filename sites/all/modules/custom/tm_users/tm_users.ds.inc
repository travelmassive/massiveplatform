<?php

/**
 * @file
 * tm_users.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function tm_users_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'user|user|default';
  $ds_fieldsetting->entity_type = 'user';
  $ds_fieldsetting->bundle = 'user';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'joined_chapter_list' => array(
      'weight' => '11',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'joined_event_list' => array(
      'weight' => '12',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'members_following_members' => array(
      'weight' => '13',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'following_organizations' => array(
      'weight' => '14',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'list_users_companies' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'members_followed_by_members' => array(
      'weight' => '12',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'name' => array(
      'weight' => '2',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '0',
        'wrapper' => 'h1',
        'class' => 'prime-title',
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_user_stats' => array(
      'weight' => '23',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
        ),
      ),
    ),
    'tm_ds_user_featured' => array(
      'weight' => '26',
      'label' => 'above',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
        ),
      ),
    ),
    'tm_ds_subscription_user_cta_ds' => array(
      'weight' => '24',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'tm_ds_subscription_user_badge_ds' => array(
      'weight' => '25',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'tm_ds_chapter_sticker' => array(
      'weight' => '6',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_approved' => array(
      'weight' => '4',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'tm_ds_more_actions' => array(
      'weight' => '4',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'li',
          'ow-cl' => '',
          'ow-def-cl' => FALSE,
          'ow-at' => 'data-dropd-wrapper',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_communication_actions' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'li',
          'ow-cl' => '',
          'ow-def-cl' => FALSE,
          'ow-at' => 'data-dropd-wrapper',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_anon_flag_placeholder' => array(
      'weight' => '2',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'li',
          'ow-cl' => '',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_edit_own_profile' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'Extra classes' => 'bttn bttn-secondary bttn-m edit',
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'li',
          'ow-cl' => '',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_connection_actions' => array(
      'weight' => '22',
      'label' => 'above',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
        ),
      ),
    ),
    'tm_ds_track_viewers' => array(
      'weight' => '27',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
        ),
      ),
    ),
    'field_about_me' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default_markdown',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_avatar' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'avatar',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
          'fi' => TRUE,
          'fi-el' => 'span',
          'fi-cl' => 'badge-user zoomable',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
          'fi-first-last' => FALSE,
        ),
      ),
    ),
    'field_image' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'cover',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'field_job_organization' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'span',
          'ow-cl' => 'organization',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'field_job_role' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'span',
          'ow-cl' => 'role',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'field_link_facebook' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_linkedin' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_twitter' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_website' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_location_city' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'span',
          'ow-cl' => 'city',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'field_segment' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_1' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_2' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_3' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_4' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_5' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_reason_for_joining' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_instagram' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_snapchat' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_vimeo' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_youtube' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_strava' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_testimonial' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_link_tiktok' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_covid19_message' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb' => 'How has COVID-19 impacted you?',
          'lb-col' => TRUE,
        ),
      ),
    ),
  );
  $export['user|user|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'user|user|grid';
  $ds_fieldsetting->entity_type = 'user';
  $ds_fieldsetting->bundle = 'user';
  $ds_fieldsetting->view_mode = 'grid';
  $ds_fieldsetting->settings = array(
    'name' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '0',
        'wrapper' => 'h1',
        'class' => 'prime-title',
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_chapter_sticker' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'field_avatar' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'avatar',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
          'fi' => TRUE,
          'fi-el' => 'span',
          'fi-cl' => 'badge-user',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
          'fi-first-last' => FALSE,
        ),
      ),
    ),
    'field_image' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'cover',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'field_job_organization' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'span',
          'ow-cl' => 'organization',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'field_job_role' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'span',
          'ow-cl' => 'role',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
  );
  $export['user|user|grid'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'user|user|list';
  $ds_fieldsetting->entity_type = 'user';
  $ds_fieldsetting->bundle = 'user';
  $ds_fieldsetting->view_mode = 'list';
  $ds_fieldsetting->settings = array(
    'field_avatar' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
  );
  $export['user|user|list'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'user|user|search_result';
  $ds_fieldsetting->entity_type = 'user';
  $ds_fieldsetting->bundle = 'user';
  $ds_fieldsetting->view_mode = 'search_result';
  $ds_fieldsetting->settings = array(
    'name' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '0',
        'wrapper' => 'h1',
        'class' => 'prime-title',
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_content_type' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'p',
          'ow-cl' => 'meta',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'field_about_me' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'p',
          'ow-cl' => '',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
  );
  $export['user|user|search_result'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function tm_users_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'following_organizations';
  $ds_field->label = 'Following Organizations';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'user' => 'user',
  );
  $ds_field->ui_limit = 'user|*';
  $ds_field->properties = array(
    'block' => 'views|556ada30b55719eb99cf9afde6fc2f56',
    'block_render' => '3',
  );
  $export['following_organizations'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'list_users_companies';
  $ds_field->label = 'List Users Companies';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'user' => 'user',
  );
  $ds_field->ui_limit = 'user|*';
  $ds_field->properties = array(
    'block' => 'views|cb8676855b7262194e5050809bd7e81b',
    'block_render' => '3',
  );
  $export['list_users_companies'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'members_followed_by_members';
  $ds_field->label = 'Followers';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'user' => 'user',
  );
  $ds_field->ui_limit = 'user|*';
  $ds_field->properties = array(
    'block' => 'views|member_listings-block_followers',
    'block_render' => '3',
  );
  $export['members_followed_by_members'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function tm_users_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'user|user|default';
  $ds_layout->entity_type = 'user';
  $ds_layout->bundle = 'user';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'trilithon';
  $ds_layout->settings = array(
    'regions' => array(
      'header_body' => array(
        0 => 'tm_ds_edit_own_profile',
        3 => 'field_job_role',
        4 => 'field_avatar',
        5 => 'flag_follow_members',
        6 => 'field_job_organization',
        7 => 'tm_ds_anon_flag_placeholder',
        8 => 'name',
        9 => 'tm_ds_communication_actions',
        10 => 'group_job_wrapper',
        11 => 'tm_ds_more_actions',
        12 => 'tm_ds_approved',
        13 => 'group_action_menu_wrapper',
        17 => 'group_action_menu',
      ),
      'header_media' => array(
        1 => 'field_image',
      ),
      'second_column' => array(
        2 => 'list_users_companies',
        16 => 'group_chapter_list_wrapper',
        18 => 'group_companies_list_wrapper',
        19 => 'group_community_wrapper',
        20 => 'joined_chapter_list',
        21 => 'group_following_wrapper',
        22 => 'joined_event_list',
        23 => 'group_following_organizations',
        24 => 'members_followed_by_members',
        25 => 'members_following_members',
        26 => 'group_event_list_wrapper',
        27 => 'following_organizations',
      ),
      'header_extra' => array(
        14 => 'tm_ds_chapter_sticker',
      ),
      'first_column' => array(
        15 => 'group_details_wrapper',
        28 => 'tm_ds_connection_actions',
        29 => 'field_location_city',
        30 => 'tm_ds_user_stats',
        31 => 'tm_ds_subscription_user_cta_ds',
        32 => 'field_reason_for_joining',
        33 => 'tm_ds_subscription_user_badge_ds',
        34 => 'tm_ds_user_featured',
        35 => 'group_user_qa',
        36 => 'field_user_question_1',
        37 => 'field_user_question_2',
        38 => 'tm_ds_track_viewers',
        39 => 'field_about_me',
        40 => 'field_user_question_3',
        41 => 'field_user_covid19_message',
        42 => 'field_user_question_4',
        43 => 'field_user_question_5',
        44 => 'field_segment',
        45 => 'field_link_website',
        46 => 'field_link_twitter',
        47 => 'field_link_facebook',
        48 => 'field_link_linkedin',
        49 => 'field_link_instagram',
        50 => 'field_link_youtube',
        51 => 'field_link_vimeo',
        52 => 'field_link_snapchat',
        53 => 'field_link_strava',
        54 => 'field_link_tiktok',
        55 => 'field_user_testimonial',
        56 => 'group_user_qa_wrapper',
        57 => 'group_location_wrapper',
      ),
    ),
    'fields' => array(
      'tm_ds_edit_own_profile' => 'header_body',
      'field_image' => 'header_media',
      'list_users_companies' => 'second_column',
      'field_job_role' => 'header_body',
      'field_avatar' => 'header_body',
      'flag_follow_members' => 'header_body',
      'field_job_organization' => 'header_body',
      'tm_ds_anon_flag_placeholder' => 'header_body',
      'name' => 'header_body',
      'tm_ds_communication_actions' => 'header_body',
      'group_job_wrapper' => 'header_body',
      'tm_ds_more_actions' => 'header_body',
      'tm_ds_approved' => 'header_body',
      'group_action_menu_wrapper' => 'header_body',
      'tm_ds_chapter_sticker' => 'header_extra',
      'group_details_wrapper' => 'first_column',
      'group_chapter_list_wrapper' => 'second_column',
      'group_action_menu' => 'header_body',
      'group_companies_list_wrapper' => 'second_column',
      'group_community_wrapper' => 'second_column',
      'joined_chapter_list' => 'second_column',
      'group_following_wrapper' => 'second_column',
      'joined_event_list' => 'second_column',
      'group_following_organizations' => 'second_column',
      'members_followed_by_members' => 'second_column',
      'members_following_members' => 'second_column',
      'group_event_list_wrapper' => 'second_column',
      'following_organizations' => 'second_column',
      'tm_ds_connection_actions' => 'first_column',
      'field_location_city' => 'first_column',
      'tm_ds_user_stats' => 'first_column',
      'tm_ds_subscription_user_cta_ds' => 'first_column',
      'field_reason_for_joining' => 'first_column',
      'tm_ds_subscription_user_badge_ds' => 'first_column',
      'tm_ds_user_featured' => 'first_column',
      'group_user_qa' => 'first_column',
      'field_user_question_1' => 'first_column',
      'field_user_question_2' => 'first_column',
      'tm_ds_track_viewers' => 'first_column',
      'field_about_me' => 'first_column',
      'field_user_question_3' => 'first_column',
      'field_user_covid19_message' => 'first_column',
      'field_user_question_4' => 'first_column',
      'field_user_question_5' => 'first_column',
      'field_segment' => 'first_column',
      'field_link_website' => 'first_column',
      'field_link_twitter' => 'first_column',
      'field_link_facebook' => 'first_column',
      'field_link_linkedin' => 'first_column',
      'field_link_instagram' => 'first_column',
      'field_link_youtube' => 'first_column',
      'field_link_vimeo' => 'first_column',
      'field_link_snapchat' => 'first_column',
      'field_link_strava' => 'first_column',
      'field_link_tiktok' => 'first_column',
      'field_user_testimonial' => 'first_column',
      'group_user_qa_wrapper' => 'first_column',
      'group_location_wrapper' => 'first_column',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header_media' => 'div',
      'header_body' => 'div',
      'header_extra' => 'div',
      'below_header' => 'div',
      'first_column' => 'div',
      'second_column' => 'div',
    ),
    'layout_wrapper' => 'article',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => FALSE,
  );
  $export['user|user|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'user|user|grid';
  $ds_layout->entity_type = 'user';
  $ds_layout->bundle = 'user';
  $ds_layout->view_mode = 'grid';
  $ds_layout->layout = 'card';
  $ds_layout->settings = array(
    'regions' => array(
      'teaser' => array(
        0 => 'name',
        3 => 'group_job_wrapper',
        4 => 'field_job_role',
        5 => 'field_job_organization',
      ),
      'media' => array(
        1 => 'field_image',
        2 => 'field_avatar',
      ),
      'extra' => array(
        6 => 'tm_ds_chapter_sticker',
      ),
    ),
    'fields' => array(
      'name' => 'teaser',
      'field_image' => 'media',
      'field_avatar' => 'media',
      'group_job_wrapper' => 'teaser',
      'field_job_role' => 'teaser',
      'field_job_organization' => 'teaser',
      'tm_ds_chapter_sticker' => 'extra',
    ),
    'classes' => array(),
    'wrappers' => array(
      'media' => 'div',
      'teaser' => 'div',
      'extra' => 'div',
    ),
    'layout_wrapper' => 'article',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
    'layout_disable_css' => FALSE,
  );
  $export['user|user|grid'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'user|user|list';
  $ds_layout->entity_type = 'user';
  $ds_layout->bundle = 'user';
  $ds_layout->view_mode = 'list';
  $ds_layout->layout = 'ds_reset';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_avatar',
      ),
    ),
    'fields' => array(
      'field_avatar' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['user|user|list'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'user|user|search_result';
  $ds_layout->entity_type = 'user';
  $ds_layout->bundle = 'user';
  $ds_layout->view_mode = 'search_result';
  $ds_layout->layout = 'list_item';
  $ds_layout->settings = array(
    'regions' => array(
      'li_content' => array(
        0 => 'field_about_me',
        1 => 'name',
        2 => 'tm_ds_content_type',
        3 => 'group_summary_wrapper',
      ),
    ),
    'fields' => array(
      'field_about_me' => 'li_content',
      'name' => 'li_content',
      'tm_ds_content_type' => 'li_content',
      'group_summary_wrapper' => 'li_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'li_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['user|user|search_result'] = $ds_layout;

  return $export;
}

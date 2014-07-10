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
    'joined_event_list' => array(
      'weight' => '8',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'members_following_members' => array(
      'weight' => '7',
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
    'tm_ds_chapter_sticker' => array(
      'weight' => '4',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_more_actions' => array(
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
    'field_about_me' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
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
          'fi-cl' => 'badge-user',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
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
    'field_location_country' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'span',
          'ow-cl' => 'country',
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
    'field_user_question_destination' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_dream_travel' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_job' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_learn' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'field_user_question_why_travel' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_dl',
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
      'header_media' => array(
        0 => 'field_image',
      ),
      'header_body' => array(
        1 => 'tm_ds_edit_own_profile',
        2 => 'flag_follow_members',
        3 => 'field_avatar',
        4 => 'name',
        5 => 'tm_ds_anon_flag_placeholder',
        6 => 'tm_ds_more_actions',
        7 => 'group_action_menu_wrapper',
        12 => 'group_action_menu',
      ),
      'header_extra' => array(
        8 => 'tm_ds_chapter_sticker',
      ),
      'first_column' => array(
        9 => 'group_details_wrapper',
        14 => 'field_about_me',
        15 => 'field_segment',
        16 => 'field_link_facebook',
        17 => 'field_user_question_destination',
        18 => 'field_user_question_dream_travel',
        19 => 'field_link_linkedin',
        20 => 'group_user_qa',
        21 => 'field_link_twitter',
        22 => 'field_user_question_learn',
        23 => 'field_user_question_job',
        24 => 'field_link_website',
        25 => 'group_user_qa_wrapper',
        26 => 'field_user_question_why_travel',
        27 => 'group_location_wrapper',
        28 => 'field_location_city',
        29 => 'field_location_country',
      ),
      'second_column' => array(
        10 => 'group_community_wrapper',
        11 => 'members_following_members',
        13 => 'joined_event_list',
      ),
    ),
    'fields' => array(
      'field_image' => 'header_media',
      'tm_ds_edit_own_profile' => 'header_body',
      'flag_follow_members' => 'header_body',
      'field_avatar' => 'header_body',
      'name' => 'header_body',
      'tm_ds_anon_flag_placeholder' => 'header_body',
      'tm_ds_more_actions' => 'header_body',
      'group_action_menu_wrapper' => 'header_body',
      'tm_ds_chapter_sticker' => 'header_extra',
      'group_details_wrapper' => 'first_column',
      'group_community_wrapper' => 'second_column',
      'members_following_members' => 'second_column',
      'group_action_menu' => 'header_body',
      'joined_event_list' => 'second_column',
      'field_about_me' => 'first_column',
      'field_segment' => 'first_column',
      'field_link_facebook' => 'first_column',
      'field_user_question_destination' => 'first_column',
      'field_user_question_dream_travel' => 'first_column',
      'field_link_linkedin' => 'first_column',
      'group_user_qa' => 'first_column',
      'field_link_twitter' => 'first_column',
      'field_user_question_learn' => 'first_column',
      'field_user_question_job' => 'first_column',
      'field_link_website' => 'first_column',
      'group_user_qa_wrapper' => 'first_column',
      'field_user_question_why_travel' => 'first_column',
      'group_location_wrapper' => 'first_column',
      'field_location_city' => 'first_column',
      'field_location_country' => 'first_column',
    ),
    'classes' => array(),
    'wrappers' => array(
      'header_media' => 'div',
      'header_body' => 'div',
      'header_extra' => 'div',
      'first_column' => 'div',
      'second_column' => 'div',
    ),
    'layout_wrapper' => 'article',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
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
      ),
      'media' => array(
        1 => 'field_image',
        2 => 'field_avatar',
      ),
      'extra' => array(
        3 => 'tm_ds_chapter_sticker',
      ),
    ),
    'fields' => array(
      'name' => 'teaser',
      'field_image' => 'media',
      'field_avatar' => 'media',
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

  return $export;
}

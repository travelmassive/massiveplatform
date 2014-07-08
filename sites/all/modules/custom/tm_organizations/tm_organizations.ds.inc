<?php
/**
 * @file
 * tm_organizations.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function tm_organizations_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|organization|full';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'organization';
  $ds_fieldsetting->view_mode = 'full';
  $ds_fieldsetting->settings = array(
    'ds_flag_follow_organizations' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'members_following' => array(
      'weight' => '6',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'title' => array(
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
    'tm_ds_more_actions' => array(
      'weight' => '1',
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
    'body' => array(
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
          'fi-cl' => 'badge-organization',
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
    'field_owner' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
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
    'field_link_linkedin' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
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
  );
  $export['node|organization|full'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|organization|grid';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'organization';
  $ds_fieldsetting->view_mode = 'grid';
  $ds_fieldsetting->settings = array(
    'title' => array(
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
          'fi-cl' => 'badge-organization',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
        ),
      ),
    ),
  );
  $export['node|organization|grid'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function tm_organizations_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|organization|full';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'organization';
  $ds_layout->view_mode = 'full';
  $ds_layout->layout = 'trilithon';
  $ds_layout->settings = array(
    'regions' => array(
      'header_body' => array(
        0 => 'ds_flag_follow_organizations',
        2 => 'field_avatar',
        3 => 'tm_ds_more_actions',
        4 => 'title',
        5 => 'group_action_menu_wrapper',
        16 => 'group_action_menu',
      ),
      'header_media' => array(
        1 => 'field_image',
      ),
      'first_column' => array(
        6 => 'field_owner',
        7 => 'group_details_wrapper',
        9 => 'body',
        11 => 'field_segment',
        12 => 'field_link_twitter',
        13 => 'field_link_facebook',
        14 => 'field_link_linkedin',
        15 => 'field_link_website',
      ),
      'second_column' => array(
        8 => 'group_community_wrapper',
        10 => 'members_following',
      ),
    ),
    'fields' => array(
      'ds_flag_follow_organizations' => 'header_body',
      'field_image' => 'header_media',
      'field_avatar' => 'header_body',
      'tm_ds_more_actions' => 'header_body',
      'title' => 'header_body',
      'group_action_menu_wrapper' => 'header_body',
      'field_owner' => 'first_column',
      'group_details_wrapper' => 'first_column',
      'group_community_wrapper' => 'second_column',
      'body' => 'first_column',
      'members_following' => 'second_column',
      'field_segment' => 'first_column',
      'field_link_twitter' => 'first_column',
      'field_link_facebook' => 'first_column',
      'field_link_linkedin' => 'first_column',
      'field_link_website' => 'first_column',
      'group_action_menu' => 'header_body',
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
    'hide_page_title' => '1',
    'page_option_title' => '',
  );
  $export['node|organization|full'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|organization|grid';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'organization';
  $ds_layout->view_mode = 'grid';
  $ds_layout->layout = 'card';
  $ds_layout->settings = array(
    'regions' => array(
      'teaser' => array(
        0 => 'title',
      ),
      'media' => array(
        1 => 'field_image',
        2 => 'field_avatar',
      ),
    ),
    'fields' => array(
      'title' => 'teaser',
      'field_image' => 'media',
      'field_avatar' => 'media',
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
  $export['node|organization|grid'] = $ds_layout;

  return $export;
}

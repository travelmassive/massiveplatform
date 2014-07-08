<?php
/**
 * @file
 * tm_chapters.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function tm_chapters_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|chapter|full';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'chapter';
  $ds_fieldsetting->view_mode = 'full';
  $ds_fieldsetting->settings = array(
    'ds_flag_signup' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'chapter_avatar' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'event_list' => array(
      'weight' => '6',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'members_signed_up' => array(
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
    'tm_ds_add_event' => array(
      'weight' => '4',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'Extra classes' => '',
        'ft' => array(),
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
    'field_chapter_leaders' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'fis' => TRUE,
          'fis-el' => 'ul',
          'fis-cl' => 'user-list related-list',
          'fis-at' => '',
          'fis-def-at' => FALSE,
          'fi' => TRUE,
          'fi-el' => 'li',
          'fi-cl' => '',
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
    'field_link_twitter' => array(
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
    'field_link_linkedin' => array(
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
  $export['node|chapter|full'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|chapter|grid';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'chapter';
  $ds_fieldsetting->view_mode = 'grid';
  $ds_fieldsetting->settings = array(
    'chapter_avatar' => array(
      'weight' => '1',
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
    'tm_ds_next_event' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'span',
          'ow-cl' => 'meta event',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
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
  $export['node|chapter|grid'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function tm_chapters_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'chapter_avatar';
  $ds_field->label = 'City avatar';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = 'chapter|*';
  $ds_field->properties = array(
    'code' => array(
      'value' => '<div class="avatar">
<span class="badge-chapter" style="background-color:[node:field_chapter_color];">[node:field_chapter_shortcode]</span>
</div>',
      'format' => 'full_html',
    ),
    'use_token' => 1,
  );
  $export['chapter_avatar'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function tm_chapters_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|chapter|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'chapter';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'trilithon';
  $ds_layout->settings = array(
    'regions' => array(
      'first_column' => array(
        0 => 'body',
      ),
      'second_column' => array(
        1 => 'field_chapter_leaders',
      ),
      'banner' => array(
        2 => 'field_image',
      ),
    ),
    'fields' => array(
      'body' => 'first_column',
      'field_chapter_leaders' => 'second_column',
      'field_image' => 'banner',
    ),
    'classes' => array(),
    'wrappers' => array(
      'banner' => 'header',
      'first_column' => 'div',
      'second_column' => 'div',
    ),
    'layout_wrapper' => 'article',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|chapter|default'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|chapter|full';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'chapter';
  $ds_layout->view_mode = 'full';
  $ds_layout->layout = 'trilithon';
  $ds_layout->settings = array(
    'regions' => array(
      'header_body' => array(
        0 => 'ds_flag_signup',
        2 => 'chapter_avatar',
        3 => 'tm_ds_more_actions',
        4 => 'title',
        5 => 'group_action_menu_wrapper',
        19 => 'group_action_menu',
      ),
      'header_media' => array(
        1 => 'field_image',
      ),
      'first_column' => array(
        6 => 'tm_ds_add_event',
        7 => 'group_details_wrapper',
        8 => 'body',
        9 => 'event_list',
        11 => 'field_link_website',
        12 => 'group_events_wrapper',
        13 => 'field_link_twitter',
        15 => 'field_link_facebook',
        17 => 'field_link_linkedin',
      ),
      'second_column' => array(
        10 => 'members_signed_up',
        14 => 'group_team_wrapper',
        16 => 'group_community_wrapper',
        18 => 'field_chapter_leaders',
      ),
    ),
    'fields' => array(
      'ds_flag_signup' => 'header_body',
      'field_image' => 'header_media',
      'chapter_avatar' => 'header_body',
      'tm_ds_more_actions' => 'header_body',
      'title' => 'header_body',
      'group_action_menu_wrapper' => 'header_body',
      'tm_ds_add_event' => 'first_column',
      'group_details_wrapper' => 'first_column',
      'body' => 'first_column',
      'event_list' => 'first_column',
      'members_signed_up' => 'second_column',
      'field_link_website' => 'first_column',
      'group_events_wrapper' => 'first_column',
      'field_link_twitter' => 'first_column',
      'group_team_wrapper' => 'second_column',
      'field_link_facebook' => 'first_column',
      'group_community_wrapper' => 'second_column',
      'field_link_linkedin' => 'first_column',
      'field_chapter_leaders' => 'second_column',
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
  $export['node|chapter|full'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|chapter|grid';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'chapter';
  $ds_layout->view_mode = 'grid';
  $ds_layout->layout = 'card';
  $ds_layout->settings = array(
    'regions' => array(
      'media' => array(
        0 => 'field_image',
        1 => 'chapter_avatar',
      ),
      'teaser' => array(
        2 => 'title',
        3 => 'tm_ds_next_event',
      ),
    ),
    'fields' => array(
      'field_image' => 'media',
      'chapter_avatar' => 'media',
      'title' => 'teaser',
      'tm_ds_next_event' => 'teaser',
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
  $export['node|chapter|grid'] = $ds_layout;

  return $export;
}

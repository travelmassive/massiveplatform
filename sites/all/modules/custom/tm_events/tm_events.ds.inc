<?php

/**
 * @file
 * tm_events.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function tm_events_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|event|full';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'event';
  $ds_fieldsetting->view_mode = 'full';
  $ds_fieldsetting->settings = array(
    'ds_flag_event_register' => array(
      'weight' => '2',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'ds_flag_event_waitlist' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'members_signed_up' => array(
      'weight' => '11',
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
    'tm_ds_event_price' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'detail-item price',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_event_refund_policy' => array(
      'weight' => '8',
      'label' => 'above',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb-col' => TRUE,
        ),
      ),
    ),
    'tm_ds_event_payments_show_total' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'detail-item payments',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_chapter_sticker' => array(
      'weight' => '5',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_more_actions' => array(
      'weight' => '6',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'li',
          'ow-cl' => 'tm-more-actions',
          'ow-def-cl' => FALSE,
          'ow-at' => 'data-dropd-wrapper',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_communication_actions' => array(
      'weight' => '5',
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
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_event_seats_count' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'fi' => TRUE,
          'fi-el' => 'p',
          'fi-cl' => 'js-tm-seats-count meta',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
          'fi-first-last' => FALSE,
        ),
      ),
    ),
    'tm_ds_date_avatar' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_fulldate_event' => array(
      'weight' => '2',
      'label' => 'above',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'lb-el' => 'h2',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'detail-item field-date-event',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_event_message' => array(
      'weight' => '5',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
        ),
      ),
    ),
    'tm_ds_event_location' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'detail-item location',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'tm_ds_event_chatbox' => array(
      'weight' => '7',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_event_livestream' => array(
      'weight' => '6',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_event_online_instructions' => array(
      'weight' => '4',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'ow' => TRUE,
          'ow-el' => 'div',
          'ow-cl' => 'detail-item online-instructions',
          'ow-def-cl' => FALSE,
          'ow-at' => '',
          'ow-def-at' => FALSE,
        ),
      ),
    ),
    'body' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default_markdown',
          'lb-col' => TRUE,
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
    'field_organizer' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'fis' => TRUE,
          'fis-el' => 'ul',
          'fis-cl' => 'team-list',
          'fis-at' => '',
          'fis-def-at' => FALSE,
          'fi' => TRUE,
          'fi-el' => 'li',
          'fi-cl' => '',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
          'fi-first-last' => FALSE,
        ),
      ),
    ),
    'field_rsvp_link' => array(
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
    'field_event_sponsors' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'fis' => TRUE,
          'fis-el' => 'ul',
          'fis-cl' => 'related-list',
          'fis-at' => '',
          'fis-def-at' => FALSE,
          'fi' => TRUE,
          'fi-el' => 'li',
          'fi-cl' => '',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
          'fi-first-last' => FALSE,
        ),
      ),
    ),
    'field_event_featured_members' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_expert',
          'fis' => TRUE,
          'fis-el' => 'ul',
          'fis-cl' => 'team-list',
          'fis-at' => '',
          'fis-def-at' => FALSE,
          'fi' => TRUE,
          'fi-el' => 'li',
          'fi-cl' => '',
          'fi-at' => '',
          'fi-def-at' => FALSE,
          'fi-odd-even' => FALSE,
          'fi-first-last' => FALSE,
        ),
      ),
    ),
    'field_event_covid19_message' => array(
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_tm_default',
          'lb' => 'Health and Safety',
          'lb-col' => TRUE,
        ),
      ),
    ),
  );
  $export['node|event|full'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|event|grid';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'event';
  $ds_fieldsetting->view_mode = 'grid';
  $ds_fieldsetting->settings = array(
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
    'tm_ds_date_avatar' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
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
  );
  $export['node|event|grid'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|event|list';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'event';
  $ds_fieldsetting->view_mode = 'list';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '20',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '1',
        'wrapper' => 'h1',
        'class' => 'prime-title',
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
    'tm_ds_date_avatar' => array(
      'weight' => '12',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'ft' => array(
          'func' => 'theme_ds_field_reset',
        ),
      ),
    ),
  );
  $export['node|event|list'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|event|search_result';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'event';
  $ds_fieldsetting->view_mode = 'search_result';
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
    'body' => array(
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
  $export['node|event|search_result'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function tm_events_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'event_list';
  $ds_field->label = 'Event list';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'node' => 'node',
  );
  $ds_field->ui_limit = 'chapter|full
organization|full';
  $ds_field->properties = array(
    'block' => 'views|related_events-event_list',
    'block_render' => '3',
  );
  $export['event_list'] = $ds_field;

  $ds_field = new stdClass();
  $ds_field->api_version = 1;
  $ds_field->field = 'joined_event_list';
  $ds_field->label = 'Joined event list';
  $ds_field->field_type = 6;
  $ds_field->entities = array(
    'user' => 'user',
  );
  $ds_field->ui_limit = 'user|default';
  $ds_field->properties = array(
    'block' => 'views|signups-block_joined_events',
    'block_render' => '3',
  );
  $export['joined_event_list'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function tm_events_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|event|full';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'event';
  $ds_layout->view_mode = 'full';
  $ds_layout->layout = 'trilithon';
  $ds_layout->settings = array(
    'regions' => array(
      'first_column' => array(
        0 => 'tm_ds_event_price',
        3 => 'tm_ds_event_payments_show_total',
        5 => 'tm_ds_fulldate_event',
        9 => 'tm_ds_event_location',
        11 => 'tm_ds_event_online_instructions',
        14 => 'tm_ds_event_message',
        18 => 'body',
        21 => 'field_event_covid19_message',
        22 => 'tm_ds_event_refund_policy',
        23 => 'group_details_wrapper',
      ),
      'header_media' => array(
        1 => 'field_image',
      ),
      'header_body' => array(
        2 => 'field_rsvp_link',
        4 => 'tm_ds_date_avatar',
        6 => 'title',
        7 => 'ds_flag_event_register',
        8 => 'ds_flag_event_waitlist',
        10 => 'tm_ds_event_seats_count',
        12 => 'group_action_menu_wrapper',
        13 => 'tm_ds_anon_flag_placeholder',
        15 => 'tm_ds_communication_actions',
        19 => 'tm_ds_more_actions',
        31 => 'group_action_menu',
      ),
      'header_extra' => array(
        16 => 'tm_ds_chapter_sticker',
      ),
      'below_header' => array(
        17 => 'tm_ds_event_livestream',
        20 => 'tm_ds_event_chatbox',
      ),
      'second_column' => array(
        24 => 'group_sponsors',
        25 => 'group_featured_wrapper',
        26 => 'field_event_sponsors',
        27 => 'members_signed_up',
        28 => 'group_organizers_wrapper',
        29 => 'group_community_wrapper',
        30 => 'field_organizer',
        32 => 'field_event_featured_members',
      ),
    ),
    'fields' => array(
      'tm_ds_event_price' => 'first_column',
      'field_image' => 'header_media',
      'field_rsvp_link' => 'header_body',
      'tm_ds_event_payments_show_total' => 'first_column',
      'tm_ds_date_avatar' => 'header_body',
      'tm_ds_fulldate_event' => 'first_column',
      'title' => 'header_body',
      'ds_flag_event_register' => 'header_body',
      'ds_flag_event_waitlist' => 'header_body',
      'tm_ds_event_location' => 'first_column',
      'tm_ds_event_seats_count' => 'header_body',
      'tm_ds_event_online_instructions' => 'first_column',
      'group_action_menu_wrapper' => 'header_body',
      'tm_ds_anon_flag_placeholder' => 'header_body',
      'tm_ds_event_message' => 'first_column',
      'tm_ds_communication_actions' => 'header_body',
      'tm_ds_chapter_sticker' => 'header_extra',
      'tm_ds_event_livestream' => 'below_header',
      'body' => 'first_column',
      'tm_ds_more_actions' => 'header_body',
      'tm_ds_event_chatbox' => 'below_header',
      'field_event_covid19_message' => 'first_column',
      'tm_ds_event_refund_policy' => 'first_column',
      'group_details_wrapper' => 'first_column',
      'group_sponsors' => 'second_column',
      'group_featured_wrapper' => 'second_column',
      'field_event_sponsors' => 'second_column',
      'members_signed_up' => 'second_column',
      'group_organizers_wrapper' => 'second_column',
      'group_community_wrapper' => 'second_column',
      'field_organizer' => 'second_column',
      'group_action_menu' => 'header_body',
      'field_event_featured_members' => 'second_column',
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
    'hide_page_title' => '1',
    'page_option_title' => '',
  );
  $export['node|event|full'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|event|grid';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'event';
  $ds_layout->view_mode = 'grid';
  $ds_layout->layout = 'card';
  $ds_layout->settings = array(
    'regions' => array(
      'media' => array(
        0 => 'field_image',
        1 => 'tm_ds_date_avatar',
      ),
      'teaser' => array(
        2 => 'title',
      ),
      'extra' => array(
        3 => 'tm_ds_chapter_sticker',
      ),
    ),
    'fields' => array(
      'field_image' => 'media',
      'tm_ds_date_avatar' => 'media',
      'title' => 'teaser',
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
  $export['node|event|grid'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|event|list';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'event';
  $ds_layout->view_mode = 'list';
  $ds_layout->layout = 'ds_reset';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'tm_ds_date_avatar',
        1 => 'title',
      ),
    ),
    'fields' => array(
      'tm_ds_date_avatar' => 'ds_content',
      'title' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => TRUE,
    'layout_link_attribute' => FALSE,
    'layout_link_custom' => '',
  );
  $export['node|event|list'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|event|search_result';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'event';
  $ds_layout->view_mode = 'search_result';
  $ds_layout->layout = 'list_item';
  $ds_layout->settings = array(
    'regions' => array(
      'li_content' => array(
        0 => 'title',
        1 => 'tm_ds_content_type',
        2 => 'body',
        3 => 'group_summary_wrapper',
      ),
    ),
    'fields' => array(
      'title' => 'li_content',
      'tm_ds_content_type' => 'li_content',
      'body' => 'li_content',
      'group_summary_wrapper' => 'li_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'li_content' => 'div',
    ),
    'layout_wrapper' => 'article',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
    'layout_link_attribute' => '',
    'layout_link_custom' => '',
  );
  $export['node|event|search_result'] = $ds_layout;

  return $export;
}

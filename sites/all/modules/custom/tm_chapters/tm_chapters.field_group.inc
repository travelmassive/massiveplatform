<?php
/**
 * @file
 * tm_chapters.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_chapters_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_action_menu_wrapper|node|chapter|full';
  $field_group->group_name = 'group_action_menu_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'full';
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
  $export['group_action_menu_wrapper|node|chapter|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_action_menu|node|chapter|full';
  $field_group->group_name = 'group_action_menu';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_action_menu_wrapper';
  $field_group->data = array(
    'label' => 'Action menu',
    'weight' => '16',
    'children' => array(
      0 => 'ds_flag_signup',
      1 => 'tm_ds_more_actions',
      2 => 'tm_ds_anon_flag_placeholder',
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
  $export['group_action_menu|node|chapter|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_community_wrapper|node|chapter|full';
  $field_group->group_name = 'group_community_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Community',
    'weight' => '8',
    'children' => array(
      0 => 'members_signed_up',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Community',
      'formatter' => 'open',
    ),
  );
  $export['group_community_wrapper|node|chapter|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details_wrapper|node|chapter|full';
  $field_group->group_name = 'group_details_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'About',
    'weight' => '5',
    'children' => array(
      0 => 'body',
      1 => 'field_link_facebook',
      2 => 'field_link_linkedin',
      3 => 'field_link_twitter',
      4 => 'field_link_website',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'About',
      'formatter' => 'open',
    ),
  );
  $export['group_details_wrapper|node|chapter|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_about|node|chapter|form';
  $field_group->group_name = 'group_form_about';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'About',
    'weight' => '0',
    'children' => array(
      0 => 'body',
      1 => 'field_chapter_color',
      2 => 'field_chapter_leaders',
      3 => 'field_chapter_shortcode',
      4 => 'field_image',
      5 => 'field_country',
      6 => 'title',
      7 => 'path',
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
  $export['group_form_about|node|chapter|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_elsewhere|node|chapter|form';
  $field_group->group_name = 'group_form_elsewhere';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Elsewhere',
    'weight' => '3',
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
  $export['group_form_elsewhere|node|chapter|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_summary_wrapper|node|chapter|search_result';
  $field_group->group_name = 'group_summary_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'search_result';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Summary',
    'weight' => '2',
    'children' => array(
      0 => 'body',
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
  $export['group_summary_wrapper|node|chapter|search_result'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_team_wrapper|node|chapter|full';
  $field_group->group_name = 'group_team_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Team',
    'weight' => '7',
    'children' => array(
      0 => 'field_chapter_leaders',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Team',
      'formatter' => 'open',
    ),
  );
  $export['group_team_wrapper|node|chapter|full'] = $field_group;

  return $export;
}

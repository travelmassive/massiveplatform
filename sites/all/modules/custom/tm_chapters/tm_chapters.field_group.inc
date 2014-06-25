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
    'weight' => '3',
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
    'weight' => '6',
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
    'label' => 'Details',
    'weight' => '4',
    'children' => array(
      0 => 'group_details',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Details',
      'formatter' => 'open',
    ),
  );
  $export['group_details_wrapper|node|chapter|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|chapter|full';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_details_wrapper';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '5',
    'children' => array(
      0 => 'body',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Details',
      'instance_settings' => array(
        'classes' => 'details-list',
        'element' => 'dl',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
      ),
    ),
  );
  $export['group_details|node|chapter|full'] = $field_group;

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
    'weight' => '5',
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

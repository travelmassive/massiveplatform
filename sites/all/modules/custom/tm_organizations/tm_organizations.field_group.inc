<?php
/**
 * @file
 * tm_organizations.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_organizations_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_action_menu_wrapper|node|organization|full';
  $field_group->group_name = 'group_action_menu_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
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
  $field_groups['group_action_menu_wrapper|node|organization|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_action_menu|node|organization|full';
  $field_group->group_name = 'group_action_menu';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_action_menu_wrapper';
  $field_group->data = array(
    'label' => 'Action menu',
    'weight' => '15',
    'children' => array(
      0 => 'ds_flag_follow_organizations',
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
  $field_groups['group_action_menu|node|organization|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_community_wrapper|node|organization|full';
  $field_group->group_name = 'group_community_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Followers',
    'weight' => '9',
    'children' => array(
      0 => 'members_following',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Followers',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_community_wrapper|node|organization|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_company_account_details|node|organization|form';
  $field_group->group_name = 'group_company_account_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Account Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_company_account_receipt',
      1 => 'field_company_account_contact',
      2 => 'field_company_account_email',
      3 => 'field_company_account_phone',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Account Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-company-account-details field-group-fieldset',
        'description' => 'This information will be kept <i>private</i>.',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_company_account_details|node|organization|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_company_moderator_settings|node|organization|form';
  $field_group->group_name = 'group_company_moderator_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Moderator Settings',
    'weight' => '4',
    'children' => array(
      0 => 'field_company_enable_events',
      1 => 'field_company_newsfeed_location',
      2 => 'field_company_newsfeed_tags',
      3 => 'field_company_newsfeed_url',
      4 => 'field_company_subscription_type',
      5 => 'field_company_subscription_on',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Moderator Settings',
      'instance_settings' => array(
        'required_fields' => 0,
        'id' => '',
        'classes' => 'group-company-moderator-settings field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_company_moderator_settings|node|organization|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details_wrapper|node|organization|full';
  $field_group->group_name = 'group_details_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
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
      5 => 'field_link_instagram',
      6 => 'field_link_snapchat',
      7 => 'field_link_vimeo',
      8 => 'field_link_youtube',
      9 => 'tm_ds_subscription_organization_cta_ds',
      10 => 'tm_ds_subscription_organization_badge_ds',
      11 => 'tm_ds_tm_sponsor',
      12 => 'tm_ds_organization_follow_message',
      13 => 'tm_ds_organization_stats',
      14 => 'group_location_wrapper',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'About',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_details_wrapper|node|organization|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_events_wrapper|node|organization|full';
  $field_group->group_name = 'group_events_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Events',
    'weight' => '6',
    'children' => array(
      0 => 'event_list',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Events',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_events_wrapper|node|organization|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_about|node|organization|form';
  $field_group->group_name = 'group_form_about';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'About',
    'weight' => '0',
    'children' => array(
      0 => 'body',
      1 => 'field_avatar',
      2 => 'field_friendly_url',
      3 => 'field_image',
      4 => 'field_location_city',
      5 => 'field_owner',
      6 => 'field_segment',
      7 => 'field_country',
      8 => 'field_company_cover_video',
      9 => 'title',
      10 => 'path',
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
  $field_groups['group_form_about|node|organization|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_elsewhere|node|organization|form';
  $field_group->group_name = 'group_form_elsewhere';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Elsewhere',
    'weight' => '1',
    'children' => array(
      0 => 'field_link_facebook',
      1 => 'field_link_linkedin',
      2 => 'field_link_twitter',
      3 => 'field_link_website',
      4 => 'field_follower_message',
      5 => 'field_link_instagram',
      6 => 'field_link_snapchat',
      7 => 'field_link_vimeo',
      8 => 'field_link_youtube',
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
  $field_groups['group_form_elsewhere|node|organization|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location_wrapper|node|organization|full';
  $field_group->group_name = 'group_location_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_details_wrapper';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '14',
    'children' => array(
      0 => 'field_location_city',
      1 => 'field_country',
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
  $field_groups['group_location_wrapper|node|organization|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_managed_by_wrapper|node|organization|full';
  $field_group->group_name = 'group_managed_by_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Profile managed by',
    'weight' => '8',
    'children' => array(
      0 => 'field_owner',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Profile managed by',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_managed_by_wrapper|node|organization|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_summary_wrapper|node|organization|search_result';
  $field_group->group_name = 'group_summary_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'organization';
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
  $field_groups['group_summary_wrapper|node|organization|search_result'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('About');
  t('Account Details');
  t('Action menu');
  t('Action menu wrapper');
  t('Elsewhere');
  t('Events');
  t('Followers');
  t('Location');
  t('Moderator Settings');
  t('Profile managed by');
  t('Summary');

  return $field_groups;
}

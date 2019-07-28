<?php

/**
 * @file
 * tm_branding.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_branding_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_band_matching|node|site_brand|form';
  $field_group->group_name = 'group_band_matching';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'site_brand';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Match Criteria',
    'weight' => '1',
    'children' => array(
      0 => 'field_brand_match_continent',
      1 => 'field_brand_match_country',
      2 => 'field_brand_match_method',
      3 => 'field_brand_match_probability',
      4 => 'field_brand_match_user_chapter',
      5 => 'field_brand_match_user_industry',
      6 => 'field_brand_process_order',
      7 => 'field_match_cookie_expiry',
      8 => 'field_match_partner_cookie',
      9 => 'field_match_referer_domain',
      10 => 'field_match_stop_rule',
      11 => 'field_match_url_path',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Match Criteria',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-band-matching field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_band_matching|node|site_brand|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_brand_display_period|node|site_brand|form';
  $field_group->group_name = 'group_brand_display_period';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'site_brand';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Brand display period',
    'weight' => '2',
    'children' => array(
      0 => 'field_brand_enabled_timespan',
      1 => 'field_brand_enabled_timespan_on',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Brand display period',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-brand-display-period field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_brand_display_period|node|site_brand|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_brand_elements|node|site_brand|form';
  $field_group->group_name = 'group_brand_elements';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'site_brand';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Brand Elements',
    'weight' => '5',
    'children' => array(
      0 => 'field_brand_asset_files',
      1 => 'field_brand_email_footer_html',
      2 => 'field_brand_favicon',
      3 => 'field_brand_footer_html',
      4 => 'field_brand_footer_level1_html',
      5 => 'field_brand_footer_level2_html',
      6 => 'field_brand_footer_logo',
      7 => 'field_brand_frontpage_block_html',
      8 => 'field_brand_header_logo',
      9 => 'field_brand_include_css',
      10 => 'field_brand_include_js',
      11 => 'field_brand_menu_html',
      12 => 'field_brand_search_page_banner',
      13 => 'field_brand_search_page_link',
      14 => 'field_brand_navbar_html',
      15 => 'field_brand_fp_block_anon_html',
      16 => 'field_brand_fp_block_user_html',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Brand Elements',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-brand-elements field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_brand_elements|node|site_brand|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_brand_frontpage_video|node|site_brand|form';
  $field_group->group_name = 'group_brand_frontpage_video';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'site_brand';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Brand frontpage video',
    'weight' => '4',
    'children' => array(
      0 => 'field_brand_frontpage_image',
      1 => 'field_brand_frontpage_video_link',
      2 => 'field_brand_frontpage_video_text',
      3 => 'field_brand_frontpage_video_url',
      4 => 'field_brand_frontpage_opacity',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Brand frontpage video',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-brand-frontpage-video field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_brand_frontpage_video|node|site_brand|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_brand_settings|node|site_brand|form';
  $field_group->group_name = 'group_brand_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'site_brand';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Brand Settings',
    'weight' => '0',
    'children' => array(
      0 => 'field_brand_description',
      1 => 'field_brand_enabled',
      2 => 'title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Brand Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-brand-settings field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_brand_settings|node|site_brand|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_brand_testing|node|site_brand|form';
  $field_group->group_name = 'group_brand_testing';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'site_brand';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Testing',
    'weight' => '3',
    'children' => array(
      0 => 'field_brand_test_enabled',
      1 => 'field_brand_test_ips',
      2 => 'field_brand_test_users',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Testing',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-brand-testing field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_brand_testing|node|site_brand|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Brand Elements');
  t('Brand Settings');
  t('Brand display period');
  t('Brand frontpage video');
  t('Match Criteria');
  t('Testing');

  return $field_groups;
}

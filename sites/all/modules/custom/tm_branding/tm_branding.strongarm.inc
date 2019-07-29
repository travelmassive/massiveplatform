<?php

/**
 * @file
 * tm_branding.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_branding_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_enable_revisions_page_node_site_brand';
  $strongarm->value = 1;
  $export['diff_enable_revisions_page_node_site_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_show_preview_changes_node_site_brand';
  $strongarm->value = 0;
  $export['diff_show_preview_changes_node_site_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_view_mode_preview_node_site_brand';
  $strongarm->value = 'full';
  $export['diff_view_mode_preview_node_site_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__site_brand';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'metatags' => array(
          'weight' => '6',
        ),
        'title' => array(
          'weight' => '8',
        ),
        'path' => array(
          'weight' => '7',
        ),
        'redirect' => array(
          'weight' => '6',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__site_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'hs_config_taxonomy-field_brand_match_user_industry';
  $strongarm->value = array(
    'save_lineage' => 0,
    'enforce_deepest' => 0,
    'resizable' => 1,
    'level_labels' => array(
      'status' => 0,
      'labels' => array(
        0 => '',
        1 => '',
      ),
    ),
    'dropbox' => array(
      'status' => 1,
      'title' => 'All selections',
      'limit' => 0,
      'reset_hs' => 1,
      'sort' => 1,
    ),
    'editability' => array(
      'status' => 0,
      'item_types' => array(
        0 => '',
        1 => '',
      ),
      'allowed_levels' => array(
        0 => 1,
        1 => 1,
      ),
      'allow_new_levels' => 0,
      'max_levels' => 1,
    ),
    'entity_count' => array(
      'enabled' => 0,
      'require_entity' => 0,
      'settings' => array(
        'entity_types' => array(
          'country' => array(
            'count_country' => array(
              'country' => 0,
            ),
          ),
          'flagging' => array(
            'count_flagging' => array(
              'follow_members' => 0,
              'follow_organizations' => 0,
              'signup' => 0,
              'tm_sponsor' => 0,
              'event_waitlist' => 0,
              'event_register' => 0,
              'approval_is_company_or_brand' => 0,
              'approval_need_more_info' => 0,
              'approval_requested_by_user' => 0,
              'approval_approved_by' => 0,
              'message_sent' => 0,
              'moderate_non_community_profile' => 0,
              'featured_member' => 0,
              'follow_members_log' => 0,
              'follow_organizations_log' => 0,
              'event_paid' => 0,
              'moderate_report_member' => 0,
              'moderate_report_organization' => 0,
              'status_updated_by_chapter' => 0,
              'status_updated_by_event' => 0,
              'status_updated_by_member' => 0,
              'status_updated_by_organization' => 0,
            ),
          ),
          'node' => array(
            'count_node' => array(
              'chapter' => 'chapter',
              'organization' => 'organization',
              'event' => 'event',
              'page' => 'page',
              'site_brand' => 'site_brand',
            ),
          ),
          'taxonomy_term' => array(
            'count_taxonomy_term' => array(
              'tm_segments' => 0,
            ),
          ),
          'user' => array(
            'count_user' => array(
              'user' => 0,
            ),
          ),
        ),
      ),
    ),
    'animation_delay' => 400,
    'special_items' => array(),
    'render_flat_select' => 0,
    'config_id' => 'taxonomy-field_brand_match_user_industry',
  );
  $export['hs_config_taxonomy-field_brand_match_user_industry'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_site_brand';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_site_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_site_brand';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_site_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_site_brand';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_site_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_site_brand';
  $strongarm->value = '0';
  $export['node_preview_site_brand'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_site_brand';
  $strongarm->value = 1;
  $export['node_submitted_site_brand'] = $strongarm;

  return $export;
}

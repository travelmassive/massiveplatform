<?php

/**
 * @file
 * tm_branding.features.hierarchical_select.inc
 */

/**
 * Implements hook_hierarchical_select_default_configs().
 */
function tm_branding_hierarchical_select_default_configs() {
$configs = array();
$config = array(
  'save_lineage' => 0,
  'enforce_deepest' => 0,
  'resizable' => 1,
  'level_labels' => array(
    'status' => 0,
    'labels' => array(
      '0' => '',
      '1' => '',
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
      '0' => '',
      '1' => '',
    ),
    'allowed_levels' => array(
      '0' => 1,
      '1' => 1,
    ),
    'allow_new_levels' => 0,
    'max_levels' => 1,
  ),
  'entity_count' => array(
    'enabled' => 0,
    'require_entity' => 0,
    'settings' => array(
      'count_children' => 0,
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
            'moderate_report_member' => 0,
            'moderate_report_organization' => 0,
            'event_paid' => 0,
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

$configs['taxonomy-field_brand_match_user_industry'] = $config;
return $configs;
}

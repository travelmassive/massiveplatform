<?php
/**
 * @file
 * tm_branding.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tm_branding_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'branding_moderation';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Branding Moderation';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'site_brand' => 'site_brand',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'branding_moderation_list');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Site branding';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
    10 => '10',
  );
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_brand_enabled' => 'field_brand_enabled',
    'field_brand_process_order' => 'field_brand_process_order',
    'title' => 'title',
    'field_brand_description' => 'field_brand_description',
    'field_brand_enabled_timespan_on' => 'field_brand_enabled_timespan_on',
    'field_brand_enabled_timespan' => 'field_brand_enabled_timespan',
    'field_brand_test_enabled' => 'field_brand_test_enabled',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'field_brand_process_order';
  $handler->display->display_options['style_options']['info'] = array(
    'field_brand_enabled' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_brand_process_order' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_brand_description' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_brand_enabled_timespan_on' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_brand_enabled_timespan' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_brand_test_enabled' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Messages */
  $handler->display->display_options['header']['messages']['id'] = 'messages';
  $handler->display->display_options['header']['messages']['table'] = 'views';
  $handler->display->display_options['header']['messages']['field'] = 'messages';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<a href="/node/add/site-brand">Add site brand</a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Activated */
  $handler->display->display_options['fields']['field_brand_enabled']['id'] = 'field_brand_enabled';
  $handler->display->display_options['fields']['field_brand_enabled']['table'] = 'field_data_field_brand_enabled';
  $handler->display->display_options['fields']['field_brand_enabled']['field'] = 'field_brand_enabled';
  $handler->display->display_options['fields']['field_brand_enabled']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_brand_enabled']['type'] = 'list_key';
  /* Field: Content: Processing Order */
  $handler->display->display_options['fields']['field_brand_process_order']['id'] = 'field_brand_process_order';
  $handler->display->display_options['fields']['field_brand_process_order']['table'] = 'field_data_field_brand_process_order';
  $handler->display->display_options['fields']['field_brand_process_order']['field'] = 'field_brand_process_order';
  $handler->display->display_options['fields']['field_brand_process_order']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Brand Description */
  $handler->display->display_options['fields']['field_brand_description']['id'] = 'field_brand_description';
  $handler->display->display_options['fields']['field_brand_description']['table'] = 'field_data_field_brand_description';
  $handler->display->display_options['fields']['field_brand_description']['field'] = 'field_brand_description';
  /* Field: Content: Enable branding for period */
  $handler->display->display_options['fields']['field_brand_enabled_timespan_on']['id'] = 'field_brand_enabled_timespan_on';
  $handler->display->display_options['fields']['field_brand_enabled_timespan_on']['table'] = 'field_data_field_brand_enabled_timespan_on';
  $handler->display->display_options['fields']['field_brand_enabled_timespan_on']['field'] = 'field_brand_enabled_timespan_on';
  $handler->display->display_options['fields']['field_brand_enabled_timespan_on']['type'] = 'list_key';
  /* Field: Content: Start and end date */
  $handler->display->display_options['fields']['field_brand_enabled_timespan']['id'] = 'field_brand_enabled_timespan';
  $handler->display->display_options['fields']['field_brand_enabled_timespan']['table'] = 'field_data_field_brand_enabled_timespan';
  $handler->display->display_options['fields']['field_brand_enabled_timespan']['field'] = 'field_brand_enabled_timespan';
  $handler->display->display_options['fields']['field_brand_enabled_timespan']['type'] = 'date_plain';
  $handler->display->display_options['fields']['field_brand_enabled_timespan']['settings'] = array(
    'format_type' => 'medium',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Stop rules (don't process any more rules) */
  $handler->display->display_options['fields']['field_match_stop_rule']['id'] = 'field_match_stop_rule';
  $handler->display->display_options['fields']['field_match_stop_rule']['table'] = 'field_data_field_match_stop_rule';
  $handler->display->display_options['fields']['field_match_stop_rule']['field'] = 'field_match_stop_rule';
  $handler->display->display_options['fields']['field_match_stop_rule']['label'] = 'Stop rules';
  $handler->display->display_options['fields']['field_match_stop_rule']['type'] = 'list_key';
  /* Field: Content: Enable testing */
  $handler->display->display_options['fields']['field_brand_test_enabled']['id'] = 'field_brand_test_enabled';
  $handler->display->display_options['fields']['field_brand_test_enabled']['table'] = 'field_data_field_brand_test_enabled';
  $handler->display->display_options['fields']['field_brand_test_enabled']['field'] = 'field_brand_test_enabled';
  $handler->display->display_options['fields']['field_brand_test_enabled']['type'] = 'list_key';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Activated (field_brand_enabled) */
  $handler->display->display_options['sorts']['field_brand_enabled_value']['id'] = 'field_brand_enabled_value';
  $handler->display->display_options['sorts']['field_brand_enabled_value']['table'] = 'field_data_field_brand_enabled';
  $handler->display->display_options['sorts']['field_brand_enabled_value']['field'] = 'field_brand_enabled_value';
  $handler->display->display_options['sorts']['field_brand_enabled_value']['order'] = 'DESC';
  /* Sort criterion: Content: Processing Order (field_brand_process_order) */
  $handler->display->display_options['sorts']['field_brand_process_order_value']['id'] = 'field_brand_process_order_value';
  $handler->display->display_options['sorts']['field_brand_process_order_value']['table'] = 'field_data_field_brand_process_order';
  $handler->display->display_options['sorts']['field_brand_process_order_value']['field'] = 'field_brand_process_order_value';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['path'] = 'admin/branding';
  $export['branding_moderation'] = $view;

  return $export;
}

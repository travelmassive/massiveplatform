<?php
/**
 * @file
 * tm_organizations.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tm_organizations_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'companies';
  $view->description = 'A list with companies';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Companies';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Companies';
  $handler->display->display_options['css_class'] = 'grid';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['view_mode'] = 'grid';
  $handler->display->display_options['row_options']['links'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'We could not find any company. Please try to change your search criteria.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Field: Country (field_country) */
  $handler->display->display_options['relationships']['field_country_iso2']['id'] = 'field_country_iso2';
  $handler->display->display_options['relationships']['field_country_iso2']['table'] = 'field_data_field_country';
  $handler->display->display_options['relationships']['field_country_iso2']['field'] = 'field_country_iso2';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['expose']['label'] = 'Title';
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
    'organization' => 'organization',
  );
  /* Filter criterion: Countries: Name - list */
  $handler->display->display_options['filters']['name_list']['id'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['table'] = 'countries_country';
  $handler->display->display_options['filters']['name_list']['field'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['relationship'] = 'field_country_iso2';
  $handler->display->display_options['filters']['name_list']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_list']['expose']['operator_id'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['operator'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['identifier'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
  );
  /* Filter criterion: Field: Segment (field_segment) */
  $handler->display->display_options['filters']['field_segment_tid']['id'] = 'field_segment_tid';
  $handler->display->display_options['filters']['field_segment_tid']['table'] = 'field_data_field_segment';
  $handler->display->display_options['filters']['field_segment_tid']['field'] = 'field_segment_tid';
  $handler->display->display_options['filters']['field_segment_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_segment_tid']['expose']['operator_id'] = 'field_segment_tid_op';
  $handler->display->display_options['filters']['field_segment_tid']['expose']['operator'] = 'field_segment_tid_op';
  $handler->display->display_options['filters']['field_segment_tid']['expose']['identifier'] = 'segment';
  $handler->display->display_options['filters']['field_segment_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['field_segment_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_segment_tid']['vocabulary'] = 'tm_segments';
  $handler->display->display_options['filters']['field_segment_tid']['hierarchy'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['path'] = 'companies';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Companies';
  $handler->display->display_options['menu']['weight'] = '4';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['companies'] = $view;

  return $export;
}

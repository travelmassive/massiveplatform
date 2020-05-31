<?php

/**
 * @file
 * tm_users.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tm_users_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'chapter_leaders';
  $view->description = 'A list with TM chaper leaders';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Chapter Leaders';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Chapter Leaders';
  $handler->display->display_options['css_class'] = 'grid';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'user';
  $handler->display->display_options['row_options']['view_mode'] = 'grid';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'We can\'t find any results that match your criteria.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: User: Country (field_user_country) */
  $handler->display->display_options['relationships']['field_user_country_iso2']['id'] = 'field_user_country_iso2';
  $handler->display->display_options['relationships']['field_user_country_iso2']['table'] = 'field_data_field_user_country';
  $handler->display->display_options['relationships']['field_user_country_iso2']['field'] = 'field_user_country_iso2';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: User: First name (field_user_first_name) */
  $handler->display->display_options['sorts']['field_user_first_name_value']['id'] = 'field_user_first_name_value';
  $handler->display->display_options['sorts']['field_user_first_name_value']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['sorts']['field_user_first_name_value']['field'] = 'field_user_first_name_value';
  /* Contextual filter: Countries: ISO2 */
  $handler->display->display_options['arguments']['iso2']['id'] = 'iso2';
  $handler->display->display_options['arguments']['iso2']['table'] = 'countries_country';
  $handler->display->display_options['arguments']['iso2']['field'] = 'iso2';
  $handler->display->display_options['arguments']['iso2']['relationship'] = 'field_user_country_iso2';
  $handler->display->display_options['arguments']['iso2']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['iso2']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['iso2']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['iso2']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['iso2']['limit'] = '0';
  /* Filter criterion: User: Active status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    7 => '7',
  );
  /* Filter criterion: Countries: Name - list */
  $handler->display->display_options['filters']['name_list']['id'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['table'] = 'countries_country';
  $handler->display->display_options['filters']['name_list']['field'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['relationship'] = 'field_user_country_iso2';
  $handler->display->display_options['filters']['name_list']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_list']['expose']['operator_id'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['label'] = 'Name - list';
  $handler->display->display_options['filters']['name_list']['expose']['operator'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['identifier'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    3 => 0,
    5 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'load_more';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['waypoint']['infinite'] = 1;
  $handler->display->display_options['pager']['options']['effects']['speed'] = 'slow';
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: User: Country (field_user_country) */
  $handler->display->display_options['relationships']['field_user_country_iso2']['id'] = 'field_user_country_iso2';
  $handler->display->display_options['relationships']['field_user_country_iso2']['table'] = 'field_data_field_user_country';
  $handler->display->display_options['relationships']['field_user_country_iso2']['field'] = 'field_user_country_iso2';
  /* Relationship: Flags: follow_members counter */
  $handler->display->display_options['relationships']['flag_count_rel']['id'] = 'flag_count_rel';
  $handler->display->display_options['relationships']['flag_count_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_count_rel']['field'] = 'flag_count_rel';
  $handler->display->display_options['relationships']['flag_count_rel']['label'] = 'number of followers';
  $handler->display->display_options['relationships']['flag_count_rel']['flag'] = 'follow_members';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flag counter */
  $handler->display->display_options['sorts']['count']['id'] = 'count';
  $handler->display->display_options['sorts']['count']['table'] = 'flag_counts';
  $handler->display->display_options['sorts']['count']['field'] = 'count';
  $handler->display->display_options['sorts']['count']['relationship'] = 'flag_count_rel';
  $handler->display->display_options['sorts']['count']['order'] = 'DESC';
  /* Sort criterion: User: First name (field_user_first_name) */
  $handler->display->display_options['sorts']['field_user_first_name_value']['id'] = 'field_user_first_name_value';
  $handler->display->display_options['sorts']['field_user_first_name_value']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['sorts']['field_user_first_name_value']['field'] = 'field_user_first_name_value';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Countries: ISO2 */
  $handler->display->display_options['arguments']['iso2']['id'] = 'iso2';
  $handler->display->display_options['arguments']['iso2']['table'] = 'countries_country';
  $handler->display->display_options['arguments']['iso2']['field'] = 'iso2';
  $handler->display->display_options['arguments']['iso2']['relationship'] = 'field_user_country_iso2';
  $handler->display->display_options['arguments']['iso2']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['iso2']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['iso2']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['iso2']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['iso2']['limit'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    7 => '7',
  );
  /* Filter criterion: Countries: Name - list */
  $handler->display->display_options['filters']['name_list']['id'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['table'] = 'countries_country';
  $handler->display->display_options['filters']['name_list']['field'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['relationship'] = 'field_user_country_iso2';
  $handler->display->display_options['filters']['name_list']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_list']['expose']['operator_id'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['operator'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['identifier'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    3 => 0,
    5 => 0,
  );
  $handler->display->display_options['path'] = 'leaders';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Leaders';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['chapter_leaders'] = $view;

  $view = new view();
  $view->name = 'community';
  $view->description = 'A list with TM members';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Community';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Community';
  $handler->display->display_options['css_class'] = 'grid';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'user';
  $handler->display->display_options['row_options']['view_mode'] = 'grid';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'We can\'t find any results that match your criteria.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: User: Country (field_user_country) */
  $handler->display->display_options['relationships']['field_user_country_iso2']['id'] = 'field_user_country_iso2';
  $handler->display->display_options['relationships']['field_user_country_iso2']['table'] = 'field_data_field_user_country';
  $handler->display->display_options['relationships']['field_user_country_iso2']['field'] = 'field_user_country_iso2';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Countries: ISO2 */
  $handler->display->display_options['arguments']['iso2']['id'] = 'iso2';
  $handler->display->display_options['arguments']['iso2']['table'] = 'countries_country';
  $handler->display->display_options['arguments']['iso2']['field'] = 'iso2';
  $handler->display->display_options['arguments']['iso2']['relationship'] = 'field_user_country_iso2';
  $handler->display->display_options['arguments']['iso2']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['iso2']['default_argument_options']['argument'] = 'ZA';
  $handler->display->display_options['arguments']['iso2']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['iso2']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['iso2']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['iso2']['limit'] = '0';
  /* Filter criterion: User: Active status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Countries: Name - list */
  $handler->display->display_options['filters']['name_list']['id'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['table'] = 'countries_country';
  $handler->display->display_options['filters']['name_list']['field'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['relationship'] = 'field_user_country_iso2';
  $handler->display->display_options['filters']['name_list']['group'] = 1;
  $handler->display->display_options['filters']['name_list']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_list']['expose']['operator_id'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['operator'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['identifier'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    3 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['name_list']['configuration'] = '1';
  $handler->display->display_options['filters']['name_list']['filter'] = array(
    'enabled' => '0',
    'continents' => array(),
  );
  /* Filter criterion: Taxonomy Entity Index: Has taxonomy terms on User  (with depth and indexed in taxonomy_entity_index) */
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['id'] = 'taxonomy_entity_index_tid_depth';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['table'] = 'users';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['field'] = 'taxonomy_entity_index_tid_depth';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['exposed'] = TRUE;
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['expose']['operator_id'] = 'taxonomy_entity_index_tid_depth_op';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['expose']['operator'] = 'taxonomy_entity_index_tid_depth_op';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['expose']['identifier'] = 'taxonomy_entity_index_tid_depth';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    3 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['type'] = 'select';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['vocabulary'] = 'tm_segments';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['hierarchy'] = 1;
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['depth'] = '2';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'load_more';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['waypoint']['infinite'] = 1;
  $handler->display->display_options['pager']['options']['effects']['speed'] = 'slow';
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: User: Country (field_user_country) */
  $handler->display->display_options['relationships']['field_user_country_iso2']['id'] = 'field_user_country_iso2';
  $handler->display->display_options['relationships']['field_user_country_iso2']['table'] = 'field_data_field_user_country';
  $handler->display->display_options['relationships']['field_user_country_iso2']['field'] = 'field_user_country_iso2';
  /* Relationship: Flags: moderate_non_community_profile */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'flagged_non_community';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'moderate_non_community_profile';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: approval_need_more_info */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'flagged_approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  /* Relationship: Flags: approval_is_company_or_brand */
  $handler->display->display_options['relationships']['flag_content_rel_2']['id'] = 'flag_content_rel_2';
  $handler->display->display_options['relationships']['flag_content_rel_2']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_2']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_2']['label'] = 'flagged_approval_company_profile';
  $handler->display->display_options['relationships']['flag_content_rel_2']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_2']['flag'] = 'approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_2']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Countries: Name - list */
  $handler->display->display_options['filters']['name_list']['id'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['table'] = 'countries_country';
  $handler->display->display_options['filters']['name_list']['field'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['relationship'] = 'field_user_country_iso2';
  $handler->display->display_options['filters']['name_list']['group'] = 1;
  $handler->display->display_options['filters']['name_list']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_list']['expose']['operator_id'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['operator'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['identifier'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    3 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['name_list']['configuration'] = '1';
  $handler->display->display_options['filters']['name_list']['filter'] = array(
    'enabled' => '0',
    'continents' => array(),
  );
  /* Filter criterion: Taxonomy Entity Index: Has taxonomy terms on User  (with depth and indexed in taxonomy_entity_index) */
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['id'] = 'taxonomy_entity_index_tid_depth';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['table'] = 'users';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['field'] = 'taxonomy_entity_index_tid_depth';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['exposed'] = TRUE;
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['expose']['operator_id'] = 'taxonomy_entity_index_tid_depth_op';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['expose']['operator'] = 'taxonomy_entity_index_tid_depth_op';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['expose']['identifier'] = 'taxonomy_entity_index_tid_depth';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    6 => 0,
    7 => 0,
    3 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['type'] = 'select';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['vocabulary'] = 'tm_segments';
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['hierarchy'] = 1;
  $handler->display->display_options['filters']['taxonomy_entity_index_tid_depth']['depth'] = '2';
  /* Filter criterion: Flags: Flagged */
  $handler->display->display_options['filters']['flagged']['id'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['filters']['flagged']['value'] = '0';
  /* Filter criterion: Flags: Flagged */
  $handler->display->display_options['filters']['flagged_1']['id'] = 'flagged_1';
  $handler->display->display_options['filters']['flagged_1']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged_1']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged_1']['relationship'] = 'flag_content_rel_2';
  $handler->display->display_options['filters']['flagged_1']['value'] = '0';
  /* Filter criterion: Flags: Flagged */
  $handler->display->display_options['filters']['flagged_2']['id'] = 'flagged_2';
  $handler->display->display_options['filters']['flagged_2']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged_2']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged_2']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['filters']['flagged_2']['value'] = '0';
  $handler->display->display_options['path'] = 'community';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Community';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['community'] = $view;

  $view = new view();
  $view->name = 'following_organizations';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Following Organizations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Following Organizations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Flags: follow_organizations */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'follow_organizations';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Contextual filter: Flags: User uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'flagging';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published status */
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

  /* Display: Block - Following Organizations */
  $handler = $view->new_display('block', 'Block - Following Organizations', 'block_following_organizations');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'user-list related-list';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'ds';
  $handler->display->display_options['row_options']['view_mode'] = 'grid';
  $handler->display->display_options['row_options']['alternating'] = 0;
  $handler->display->display_options['row_options']['grouping'] = 0;
  $handler->display->display_options['row_options']['advanced'] = 0;
  $handler->display->display_options['row_options']['delta_fieldset']['delta_fields'] = array();
  $handler->display->display_options['row_options']['grouping_fieldset']['group_field'] = 'node|created';
  $handler->display->display_options['row_options']['default_fieldset']['view_mode'] = 'grid';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['block_description'] = 'block_following_organizations';

  /* Display: Page - Following Organizations */
  $handler = $view->new_display('page', 'Page - Following Organizations', 'page_1');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'grid';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'load_more';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['waypoint']['infinite'] = 1;
  $handler->display->display_options['pager']['options']['effects']['speed'] = 'slow';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'ds';
  $handler->display->display_options['row_options']['view_mode'] = 'grid';
  $handler->display->display_options['row_options']['alternating'] = 0;
  $handler->display->display_options['row_options']['grouping'] = 0;
  $handler->display->display_options['row_options']['advanced'] = 0;
  $handler->display->display_options['row_options']['delta_fieldset']['delta_fields'] = array();
  $handler->display->display_options['row_options']['grouping_fieldset']['group_field'] = 'node|created';
  $handler->display->display_options['row_options']['default_fieldset']['view_mode'] = 'grid';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'user/%/following-organizations';
  $export['following_organizations'] = $view;

  $view = new view();
  $view->name = 'signups';
  $view->description = 'Overview of the chapters and events a user signed up for.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Signups';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Joined events';
  $handler->display->display_options['css_class'] = 'event-list related-list';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'ds';
  $handler->display->display_options['row_options']['view_mode'] = 'grid';
  $handler->display->display_options['row_options']['alternating'] = 0;
  $handler->display->display_options['row_options']['grouping'] = 0;
  $handler->display->display_options['row_options']['advanced'] = 0;
  $handler->display->display_options['row_options']['delta_fieldset']['delta_fields'] = array();
  $handler->display->display_options['row_options']['grouping_fieldset']['group_field'] = 'node|created';
  $handler->display->display_options['row_options']['default_fieldset']['view_mode'] = 'grid';
  /* Relationship: Flags: signup */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'signup';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['sorts']['timestamp']['group_type'] = 'min';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Contextual filter: Flags: User uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'flagging';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published status */
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
    'event' => 'event',
  );

  /* Display: Block - joined events */
  $handler = $view->new_display('block', 'Block - joined events', 'block_joined_events');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = 'block_joined_events';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: event_register */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'event_register';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: User uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'flagging';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published status */
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
    'event' => 'event',
  );
  /* Filter criterion: Content: Preview mode (field_event_is_preview_mode) */
  $handler->display->display_options['filters']['field_event_is_preview_mode_value']['id'] = 'field_event_is_preview_mode_value';
  $handler->display->display_options['filters']['field_event_is_preview_mode_value']['table'] = 'field_data_field_event_is_preview_mode';
  $handler->display->display_options['filters']['field_event_is_preview_mode_value']['field'] = 'field_event_is_preview_mode_value';
  $handler->display->display_options['filters']['field_event_is_preview_mode_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_event_is_preview_mode_value']['value'] = array(
    1 => '1',
  );

  /* Display: Block - joined chapters */
  $handler = $view->new_display('block', 'Block - joined chapters', 'block_joined_chapters');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Joined chapters';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'chapter-list related-list';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published status */
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
    'chapter' => 'chapter',
  );

  /* Display: Block - list companies */
  $handler = $view->new_display('block', 'Block - list companies', 'block_list_users_companies');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'List users companies';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'companies-list related-list';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_owner_target_id']['id'] = 'field_owner_target_id';
  $handler->display->display_options['relationships']['field_owner_target_id']['table'] = 'field_data_field_owner';
  $handler->display->display_options['relationships']['field_owner_target_id']['field'] = 'field_owner_target_id';
  $handler->display->display_options['relationships']['field_owner_target_id']['label'] = 'field_owner';
  $handler->display->display_options['relationships']['field_owner_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Managed by (field_owner) */
  $handler->display->display_options['arguments']['field_owner_target_id']['id'] = 'field_owner_target_id';
  $handler->display->display_options['arguments']['field_owner_target_id']['table'] = 'field_data_field_owner';
  $handler->display->display_options['arguments']['field_owner_target_id']['field'] = 'field_owner_target_id';
  $handler->display->display_options['arguments']['field_owner_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_owner_target_id']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['field_owner_target_id']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['field_owner_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_owner_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_owner_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published status */
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
  $export['signups'] = $view;

  $view = new view();
  $view->name = 'unapproved_users';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Unapproved users';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Moderate members';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
    5 => '5',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_user_first_name' => 'field_user_first_name',
    'field_user_last_name' => 'field_user_last_name',
    'mail' => 'mail',
    'name_1' => 'name_1',
    'field_home_chapter' => 'field_home_chapter',
    'created' => 'created',
    'flagged' => 'flagged',
    'flagged_1' => 'flagged_1',
    'flagged_2' => 'flagged_2',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_user_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_user_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_home_chapter' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'List of active users who are not approved.';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Field: User: First name */
  $handler->display->display_options['fields']['field_user_first_name']['id'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['field'] = 'field_user_first_name';
  /* Field: User: Last name */
  $handler->display->display_options['fields']['field_user_last_name']['id'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['table'] = 'field_data_field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['field'] = 'field_user_last_name';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'Username';
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Joined';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['operator'] = 'not';
  $handler->display->display_options['filters']['rid']['value'] = array(
    6 => '6',
  );
  $handler->display->display_options['filters']['rid']['reduce_duplicates'] = TRUE;
  /* Filter criterion: User: Share a Testimonial (field_user_testimonial) */
  $handler->display->display_options['filters']['field_user_testimonial_value']['id'] = 'field_user_testimonial_value';
  $handler->display->display_options['filters']['field_user_testimonial_value']['table'] = 'field_data_field_user_testimonial';
  $handler->display->display_options['filters']['field_user_testimonial_value']['field'] = 'field_user_testimonial_value';

  /* Display: All unapproved members */
  $handler = $view->new_display('page', 'All unapproved members', 'all_unapproved_members');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All unapproved users';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_user_first_name' => 'field_user_first_name',
    'field_user_last_name' => 'field_user_last_name',
    'mail' => 'mail',
    'name_1' => 'name_1',
    'field_home_chapter' => 'field_home_chapter',
    'created' => 'created',
    'timestamp' => 'timestamp',
    'flagged' => 'flagged',
    'flagged_1' => 'flagged_1',
    'flagged_3' => 'flagged_3',
    'flagged_2' => 'flagged_2',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_user_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_user_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_home_chapter' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: approval_need_more_info */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'flag_approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: approval_is_company_or_brand */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'flag_approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  /* Relationship: Flags: approval_requested_by_user */
  $handler->display->display_options['relationships']['flag_content_rel_2']['id'] = 'flag_content_rel_2';
  $handler->display->display_options['relationships']['flag_content_rel_2']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_2']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_2']['label'] = 'flag_user_requested_approval';
  $handler->display->display_options['relationships']['flag_content_rel_2']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_2']['flag'] = 'approval_requested_by_user';
  $handler->display->display_options['relationships']['flag_content_rel_2']['user_scope'] = 'any';
  /* Relationship: Flags: moderate_non_community_profile */
  $handler->display->display_options['relationships']['flag_content_rel_3']['id'] = 'flag_content_rel_3';
  $handler->display->display_options['relationships']['flag_content_rel_3']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_3']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_3']['label'] = 'flag_moderate_non_community';
  $handler->display->display_options['relationships']['flag_content_rel_3']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_3']['flag'] = 'moderate_non_community_profile';
  $handler->display->display_options['relationships']['flag_content_rel_3']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: First name */
  $handler->display->display_options['fields']['field_user_first_name']['id'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['field'] = 'field_user_first_name';
  /* Field: User: Last name */
  $handler->display->display_options['fields']['field_user_last_name']['id'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['table'] = 'field_data_field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['field'] = 'field_user_last_name';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'Username';
  /* Field: User: Home Chapter */
  $handler->display->display_options['fields']['field_home_chapter']['id'] = 'field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['table'] = 'field_data_field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['field'] = 'field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_home_chapter']['settings'] = array(
    'link' => 0,
  );
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Joined';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Flags: Flagged time */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['relationship'] = 'flag_content_rel_2';
  $handler->display->display_options['fields']['timestamp']['label'] = 'When Requested Verification';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
  /* Field: Flags: Flagged */
  $handler->display->display_options['fields']['flagged']['id'] = 'flagged';
  $handler->display->display_options['fields']['flagged']['table'] = 'flagging';
  $handler->display->display_options['fields']['flagged']['field'] = 'flagged';
  $handler->display->display_options['fields']['flagged']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['flagged']['label'] = 'Flagged: Profile Incomplete';
  $handler->display->display_options['fields']['flagged']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['flagged']['not'] = 0;
  /* Field: Flags: Flagged */
  $handler->display->display_options['fields']['flagged_1']['id'] = 'flagged_1';
  $handler->display->display_options['fields']['flagged_1']['table'] = 'flagging';
  $handler->display->display_options['fields']['flagged_1']['field'] = 'flagged';
  $handler->display->display_options['fields']['flagged_1']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['fields']['flagged_1']['label'] = 'Flagged: Company Account';
  $handler->display->display_options['fields']['flagged_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['flagged_1']['not'] = 0;
  /* Field: Flags: Flagged */
  $handler->display->display_options['fields']['flagged_3']['id'] = 'flagged_3';
  $handler->display->display_options['fields']['flagged_3']['table'] = 'flagging';
  $handler->display->display_options['fields']['flagged_3']['field'] = 'flagged';
  $handler->display->display_options['fields']['flagged_3']['relationship'] = 'flag_content_rel_3';
  $handler->display->display_options['fields']['flagged_3']['label'] = 'Flagged: Non Community';
  $handler->display->display_options['fields']['flagged_3']['not'] = 0;
  /* Field: Flags: Flagged */
  $handler->display->display_options['fields']['flagged_2']['id'] = 'flagged_2';
  $handler->display->display_options['fields']['flagged_2']['table'] = 'flagging';
  $handler->display->display_options['fields']['flagged_2']['field'] = 'flagged';
  $handler->display->display_options['fields']['flagged_2']['relationship'] = 'flag_content_rel_2';
  $handler->display->display_options['fields']['flagged_2']['label'] = 'User requested verification?';
  $handler->display->display_options['fields']['flagged_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['flagged_2']['not'] = 0;
  $handler->display->display_options['path'] = 'admin/unapproved-members';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'All members';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Unapproved Chapter members */
  $handler = $view->new_display('page', 'Unapproved Chapter members', 'unapproved_members_in_chapter');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Unapproved Chapter Members';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_user_first_name' => 'field_user_first_name',
    'field_user_last_name' => 'field_user_last_name',
    'mail' => 'mail',
    'name_1' => 'name_1',
    'field_home_chapter' => 'field_home_chapter',
    'created' => 'created',
    'timestamp' => 'timestamp',
    'flagged' => 'flagged',
    'flagged_1' => 'flagged_1',
    'flagged_3' => 'flagged_3',
    'flagged_2' => 'flagged_2',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_user_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_user_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_home_chapter' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: approval_need_more_info */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'flag_approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: approval_is_company_or_brand */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'flag_approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  /* Relationship: Flags: approval_requested_by_user */
  $handler->display->display_options['relationships']['flag_content_rel_2']['id'] = 'flag_content_rel_2';
  $handler->display->display_options['relationships']['flag_content_rel_2']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_2']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_2']['label'] = 'flag_user_requested_approval';
  $handler->display->display_options['relationships']['flag_content_rel_2']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_2']['flag'] = 'approval_requested_by_user';
  $handler->display->display_options['relationships']['flag_content_rel_2']['user_scope'] = 'any';
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['label'] = 'user_joined_chapter';
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'signup';
  /* Relationship: Flags: moderate_non_community_profile */
  $handler->display->display_options['relationships']['flag_content_rel_3']['id'] = 'flag_content_rel_3';
  $handler->display->display_options['relationships']['flag_content_rel_3']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_3']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_3']['label'] = 'flag_moderate_non_community';
  $handler->display->display_options['relationships']['flag_content_rel_3']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_3']['flag'] = 'moderate_non_community_profile';
  $handler->display->display_options['relationships']['flag_content_rel_3']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: First name */
  $handler->display->display_options['fields']['field_user_first_name']['id'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['field'] = 'field_user_first_name';
  /* Field: User: Last name */
  $handler->display->display_options['fields']['field_user_last_name']['id'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['table'] = 'field_data_field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['field'] = 'field_user_last_name';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = 'View Profile';
  /* Field: User: Home Chapter */
  $handler->display->display_options['fields']['field_home_chapter']['id'] = 'field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['table'] = 'field_data_field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['field'] = 'field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_home_chapter']['settings'] = array(
    'link' => 0,
  );
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Joined';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Flags: Flagged time */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['relationship'] = 'flag_content_rel_2';
  $handler->display->display_options['fields']['timestamp']['label'] = 'When Requested Verification';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
  /* Field: Flags: Flagged */
  $handler->display->display_options['fields']['flagged']['id'] = 'flagged';
  $handler->display->display_options['fields']['flagged']['table'] = 'flagging';
  $handler->display->display_options['fields']['flagged']['field'] = 'flagged';
  $handler->display->display_options['fields']['flagged']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['flagged']['label'] = 'Flagged: Profile Incomplete';
  $handler->display->display_options['fields']['flagged']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['flagged']['not'] = 0;
  /* Field: Flags: Flagged */
  $handler->display->display_options['fields']['flagged_1']['id'] = 'flagged_1';
  $handler->display->display_options['fields']['flagged_1']['table'] = 'flagging';
  $handler->display->display_options['fields']['flagged_1']['field'] = 'flagged';
  $handler->display->display_options['fields']['flagged_1']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['fields']['flagged_1']['label'] = 'Flagged: Company Account';
  $handler->display->display_options['fields']['flagged_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['flagged_1']['not'] = 0;
  /* Field: Flags: Flagged */
  $handler->display->display_options['fields']['flagged_3']['id'] = 'flagged_3';
  $handler->display->display_options['fields']['flagged_3']['table'] = 'flagging';
  $handler->display->display_options['fields']['flagged_3']['field'] = 'flagged';
  $handler->display->display_options['fields']['flagged_3']['relationship'] = 'flag_content_rel_3';
  $handler->display->display_options['fields']['flagged_3']['label'] = 'Flagged: Non Community';
  $handler->display->display_options['fields']['flagged_3']['not'] = 0;
  /* Field: Flags: Flagged */
  $handler->display->display_options['fields']['flagged_2']['id'] = 'flagged_2';
  $handler->display->display_options['fields']['flagged_2']['table'] = 'flagging';
  $handler->display->display_options['fields']['flagged_2']['field'] = 'flagged';
  $handler->display->display_options['fields']['flagged_2']['relationship'] = 'flag_content_rel_2';
  $handler->display->display_options['fields']['flagged_2']['label'] = 'User requested verification?';
  $handler->display->display_options['fields']['flagged_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['flagged_2']['not'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Entity ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'chapters/%/unapproved-members';
  $handler->display->display_options['menu']['title'] = 'Unapproved users';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Members with testimonials */
  $handler = $view->new_display('page', 'Members with testimonials', 'members_with_testimonials');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Member testimonials';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_user_first_name' => 'field_user_first_name',
    'field_user_last_name' => 'field_user_last_name',
    'mail' => 'mail',
    'name_1' => 'name_1',
    'field_home_chapter' => 'field_home_chapter',
    'created' => 'created',
    'timestamp' => 'timestamp',
    'flagged' => 'flagged',
    'flagged_1' => 'flagged_1',
    'flagged_3' => 'flagged_3',
    'flagged_2' => 'flagged_2',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_user_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_user_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_home_chapter' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: approval_need_more_info */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'flag_approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: approval_is_company_or_brand */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'flag_approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  /* Relationship: Flags: approval_requested_by_user */
  $handler->display->display_options['relationships']['flag_content_rel_2']['id'] = 'flag_content_rel_2';
  $handler->display->display_options['relationships']['flag_content_rel_2']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_2']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_2']['label'] = 'flag_user_requested_approval';
  $handler->display->display_options['relationships']['flag_content_rel_2']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_2']['flag'] = 'approval_requested_by_user';
  $handler->display->display_options['relationships']['flag_content_rel_2']['user_scope'] = 'any';
  /* Relationship: Flags: moderate_non_community_profile */
  $handler->display->display_options['relationships']['flag_content_rel_3']['id'] = 'flag_content_rel_3';
  $handler->display->display_options['relationships']['flag_content_rel_3']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_3']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_3']['label'] = 'flag_moderate_non_community';
  $handler->display->display_options['relationships']['flag_content_rel_3']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_3']['flag'] = 'moderate_non_community_profile';
  $handler->display->display_options['relationships']['flag_content_rel_3']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: First name */
  $handler->display->display_options['fields']['field_user_first_name']['id'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['field'] = 'field_user_first_name';
  /* Field: User: Last name */
  $handler->display->display_options['fields']['field_user_last_name']['id'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['table'] = 'field_data_field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['field'] = 'field_user_last_name';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: User: Home Chapter */
  $handler->display->display_options['fields']['field_home_chapter']['id'] = 'field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['table'] = 'field_data_field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['field'] = 'field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_home_chapter']['settings'] = array(
    'link' => 0,
  );
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Joined';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: User: Share a Testimonial */
  $handler->display->display_options['fields']['field_user_testimonial']['id'] = 'field_user_testimonial';
  $handler->display->display_options['fields']['field_user_testimonial']['table'] = 'field_data_field_user_testimonial';
  $handler->display->display_options['fields']['field_user_testimonial']['field'] = 'field_user_testimonial';
  $handler->display->display_options['fields']['field_user_testimonial']['label'] = 'Testimonial';
  $handler->display->display_options['fields']['field_user_testimonial']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['field_user_testimonial']['settings'] = array(
    'trim_length' => '600',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['operator'] = 'not';
  $handler->display->display_options['filters']['rid']['value'] = array(
    6 => '6',
  );
  $handler->display->display_options['filters']['rid']['reduce_duplicates'] = TRUE;
  /* Filter criterion: User: Share a Testimonial (field_user_testimonial) */
  $handler->display->display_options['filters']['field_user_testimonial_value']['id'] = 'field_user_testimonial_value';
  $handler->display->display_options['filters']['field_user_testimonial_value']['table'] = 'field_data_field_user_testimonial';
  $handler->display->display_options['filters']['field_user_testimonial_value']['field'] = 'field_user_testimonial_value';
  $handler->display->display_options['filters']['field_user_testimonial_value']['operator'] = 'not empty';
  $handler->display->display_options['path'] = 'admin/member-testimonials';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'All members';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Members with COVID-19 Message */
  $handler = $view->new_display('page', 'Members with COVID-19 Message', 'members_with_covid19_message');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Member COVID-19 messages';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_user_first_name' => 'field_user_first_name',
    'field_user_last_name' => 'field_user_last_name',
    'mail' => 'mail',
    'name_1' => 'name_1',
    'field_home_chapter' => 'field_home_chapter',
    'created' => 'created',
    'timestamp' => 'timestamp',
    'flagged' => 'flagged',
    'flagged_1' => 'flagged_1',
    'flagged_3' => 'flagged_3',
    'flagged_2' => 'flagged_2',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_user_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_user_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_home_chapter' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'flagged_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: approval_need_more_info */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'flag_approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'approval_need_more_info';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: approval_is_company_or_brand */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'flag_approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'approval_is_company_or_brand';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  /* Relationship: Flags: approval_requested_by_user */
  $handler->display->display_options['relationships']['flag_content_rel_2']['id'] = 'flag_content_rel_2';
  $handler->display->display_options['relationships']['flag_content_rel_2']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_2']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_2']['label'] = 'flag_user_requested_approval';
  $handler->display->display_options['relationships']['flag_content_rel_2']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_2']['flag'] = 'approval_requested_by_user';
  $handler->display->display_options['relationships']['flag_content_rel_2']['user_scope'] = 'any';
  /* Relationship: Flags: moderate_non_community_profile */
  $handler->display->display_options['relationships']['flag_content_rel_3']['id'] = 'flag_content_rel_3';
  $handler->display->display_options['relationships']['flag_content_rel_3']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel_3']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_3']['label'] = 'flag_moderate_non_community';
  $handler->display->display_options['relationships']['flag_content_rel_3']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_3']['flag'] = 'moderate_non_community_profile';
  $handler->display->display_options['relationships']['flag_content_rel_3']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: First name */
  $handler->display->display_options['fields']['field_user_first_name']['id'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['field'] = 'field_user_first_name';
  /* Field: User: Last name */
  $handler->display->display_options['fields']['field_user_last_name']['id'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['table'] = 'field_data_field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['field'] = 'field_user_last_name';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  /* Field: User: Home Chapter */
  $handler->display->display_options['fields']['field_home_chapter']['id'] = 'field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['table'] = 'field_data_field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['field'] = 'field_home_chapter';
  $handler->display->display_options['fields']['field_home_chapter']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_home_chapter']['settings'] = array(
    'link' => 0,
  );
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Joined';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: User: Share your response to COVID-19 */
  $handler->display->display_options['fields']['field_user_covid19_message']['id'] = 'field_user_covid19_message';
  $handler->display->display_options['fields']['field_user_covid19_message']['table'] = 'field_data_field_user_covid19_message';
  $handler->display->display_options['fields']['field_user_covid19_message']['field'] = 'field_user_covid19_message';
  $handler->display->display_options['fields']['field_user_covid19_message']['label'] = 'Response to COVID-19';
  $handler->display->display_options['fields']['field_user_covid19_message']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['field_user_covid19_message']['settings'] = array(
    'trim_length' => '600',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['operator'] = 'not';
  $handler->display->display_options['filters']['rid']['value'] = array(
    6 => '6',
  );
  $handler->display->display_options['filters']['rid']['reduce_duplicates'] = TRUE;
  /* Filter criterion: User: Share a Testimonial (field_user_testimonial) */
  $handler->display->display_options['filters']['field_user_testimonial_value']['id'] = 'field_user_testimonial_value';
  $handler->display->display_options['filters']['field_user_testimonial_value']['table'] = 'field_data_field_user_testimonial';
  $handler->display->display_options['filters']['field_user_testimonial_value']['field'] = 'field_user_testimonial_value';
  $handler->display->display_options['filters']['field_user_testimonial_value']['operator'] = 'not empty';
  $handler->display->display_options['path'] = 'admin/member-covid19-messages';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'All members';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['unapproved_users'] = $view;

  return $export;
}

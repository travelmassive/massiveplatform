<?php
/**
 * @file
 * Provides default views.
 */

/**
 * Implements hook_views_default_views().
 */
function ip_views_default_views() {

  $view = new view();
  $view->name = 'ip_tracker';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'ip_tracker';
  $view->human_name = 'IP addresses';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'User IP addresses';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'manage ip addresses';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'visits' => 'visits',
    'first_visit' => 'first_visit',
    'last_visit' => 'last_visit',
    'ip' => 'ip',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'visits' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'first_visit' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_visit' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'ip' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: User IP: User id */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'ip_tracker';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'USER';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  /* Field: User IP: Visits */
  $handler->display->display_options['fields']['visits']['id'] = 'visits';
  $handler->display->display_options['fields']['visits']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['visits']['field'] = 'visits';
  /* Field: User IP: First visit */
  $handler->display->display_options['fields']['first_visit']['id'] = 'first_visit';
  $handler->display->display_options['fields']['first_visit']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['first_visit']['field'] = 'first_visit';
  $handler->display->display_options['fields']['first_visit']['date_format'] = 'long';
  /* Field: User IP: Last visit */
  $handler->display->display_options['fields']['last_visit']['id'] = 'last_visit';
  $handler->display->display_options['fields']['last_visit']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['last_visit']['field'] = 'last_visit';
  $handler->display->display_options['fields']['last_visit']['date_format'] = 'long';
  /* Field: User IP: IP Address */
  $handler->display->display_options['fields']['ip']['id'] = 'ip';
  $handler->display->display_options['fields']['ip']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['ip']['field'] = 'ip';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: User IP: IP Address */
  $handler->display->display_options['filters']['ip']['id'] = 'ip';
  $handler->display->display_options['filters']['ip']['table'] = 'ip_tracker';
  $handler->display->display_options['filters']['ip']['field'] = 'ip';
  $handler->display->display_options['filters']['ip']['exposed'] = TRUE;
  $handler->display->display_options['filters']['ip']['expose']['operator_id'] = 'ip_op';
  $handler->display->display_options['filters']['ip']['expose']['label'] = 'IP Address';
  $handler->display->display_options['filters']['ip']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['ip']['expose']['operator'] = 'ip_op';
  $handler->display->display_options['filters']['ip']['expose']['identifier'] = 'ip';
  $handler->display->display_options['filters']['ip']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/people/ip';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'IP';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'ip_tracker_user';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'ip_tracker';
  $view->human_name = 'User IP addresses';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'User IP addresses';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'manage ip addresses';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'ip' => 'ip',
    'visits' => 'visits',
    'first_visit' => 'first_visit',
    'last_visit' => 'last_visit',
    'user_count' => 'user_count',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'ip' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'visits' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'first_visit' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_visit' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_count' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: User IP: User id */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'ip_tracker';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'USER';
  /* Field: User IP: IP Address */
  $handler->display->display_options['fields']['ip']['id'] = 'ip';
  $handler->display->display_options['fields']['ip']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['ip']['field'] = 'ip';
  /* Field: User IP: Visits */
  $handler->display->display_options['fields']['visits']['id'] = 'visits';
  $handler->display->display_options['fields']['visits']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['visits']['field'] = 'visits';
  /* Field: User IP: First visit */
  $handler->display->display_options['fields']['first_visit']['id'] = 'first_visit';
  $handler->display->display_options['fields']['first_visit']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['first_visit']['field'] = 'first_visit';
  $handler->display->display_options['fields']['first_visit']['date_format'] = 'long';
  /* Field: User IP: Last visit */
  $handler->display->display_options['fields']['last_visit']['id'] = 'last_visit';
  $handler->display->display_options['fields']['last_visit']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['last_visit']['field'] = 'last_visit';
  $handler->display->display_options['fields']['last_visit']['date_format'] = 'long';
  /* Field: User IP: User count */
  $handler->display->display_options['fields']['user_count']['id'] = 'user_count';
  $handler->display->display_options['fields']['user_count']['table'] = 'ip_tracker';
  $handler->display->display_options['fields']['user_count']['field'] = 'user_count';
  $handler->display->display_options['fields']['user_count']['element_label_colon'] = FALSE;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/ip';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'IP';
  $handler->display->display_options['menu']['description'] = 'User IP addresses';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views[$view->name] = $view;

  return $views;
}

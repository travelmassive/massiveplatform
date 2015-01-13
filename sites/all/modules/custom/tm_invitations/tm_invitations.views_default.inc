<?php
/**
 * @file
 * tm_invitations.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tm_invitations_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'invitations';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'tm_invitations';
  $view->human_name = 'Invitations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Invitation';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'email' => 'email',
    'created' => 'created',
    'accepted' => 'accepted',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'email' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'accepted' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: TM invitations: User email */
  $handler->display->display_options['relationships']['email']['id'] = 'email';
  $handler->display->display_options['relationships']['email']['table'] = 'tm_invitations';
  $handler->display->display_options['relationships']['email']['field'] = 'email';
  /* Relationship: TM invitations: Inviter UID */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'tm_invitations';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name_1']['label'] = 'Inviter';
  /* Field: TM invitations: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'tm_invitations';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: TM invitations: Accepted */
  $handler->display->display_options['fields']['accepted']['id'] = 'accepted';
  $handler->display->display_options['fields']['accepted']['table'] = 'tm_invitations';
  $handler->display->display_options['fields']['accepted']['field'] = 'accepted';
  $handler->display->display_options['fields']['accepted']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['accepted']['second_date_format'] = 'long';
  /* Field: TM invitations: Email */
  $handler->display->display_options['fields']['email']['id'] = 'email';
  $handler->display->display_options['fields']['email']['table'] = 'tm_invitations';
  $handler->display->display_options['fields']['email']['field'] = 'email';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'email';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['name']['empty_zero'] = TRUE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'email';
  $handler->display->display_options['fields']['uid']['label'] = 'Name';
  $handler->display->display_options['fields']['uid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['uid']['alter']['text'] = '[name]';
  $handler->display->display_options['fields']['uid']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['uid']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Sort criterion: TM invitations: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'tm_invitations';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Created';
  /* Contextual filter: TM invitations: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'tm_invitations';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'php';
  $handler->display->display_options['arguments']['nid']['default_argument_options']['code'] = 'return _orig_nid();';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $export['invitations'] = $view;

  return $export;
}

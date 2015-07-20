<?php
/**
 * @file
 * tm_reports.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tm_reports_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'reports';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    5 => '5',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['pure_distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_country_1' => 'field_country_1',
    'field_country' => 'field_country',
    'title' => 'title',
    'field_user_first_name' => 'field_user_first_name',
    'field_user_last_name' => 'field_user_last_name',
    'uid' => 'uid',
    'mail' => 'mail',
    'field_link_twitter' => 'field_link_twitter',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_country_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_country' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
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
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_link_twitter' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['id'] = 'reverse_field_chapter_node';
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['field'] = 'reverse_field_chapter_node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date']['id'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date']['field'] = 'field_event_date';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Organizers Contact */
  $handler = $view->new_display('page', 'Organizers Contact', 'organizers_contact');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Organizers Contact';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'continent' => 'continent',
    'field_country' => 'field_country',
    'title' => 'title',
    'field_user_first_name' => 'field_user_first_name',
    'field_user_last_name' => 'field_user_last_name',
    'uid' => 'uid',
    'mail' => 'mail',
    'field_link_website' => 'field_link_website',
    'field_link_facebook' => 'field_link_facebook',
    'field_link_twitter' => 'field_link_twitter',
    'field_link_linkedin' => 'field_link_linkedin',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'continent' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_country' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
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
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_link_website' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_link_facebook' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_link_twitter' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_link_linkedin' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
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
  $handler->display->display_options['header']['area']['content'] = '<p><a href="/admin/community_stats/chapter_leaders/export">Export as CSV</a></p>
';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['id'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['table'] = 'field_data_field_chapter_leaders';
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['field'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['label'] = 'chapter_leaders';
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['required'] = TRUE;
  /* Relationship: Content: Country (field_country) */
  $handler->display->display_options['relationships']['field_country_iso2']['id'] = 'field_country_iso2';
  $handler->display->display_options['relationships']['field_country_iso2']['table'] = 'field_data_field_country';
  $handler->display->display_options['relationships']['field_country_iso2']['field'] = 'field_country_iso2';
  $handler->display->display_options['relationships']['field_country_iso2']['label'] = 'country';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Countries: Continent */
  $handler->display->display_options['fields']['continent']['id'] = 'continent';
  $handler->display->display_options['fields']['continent']['table'] = 'countries_country';
  $handler->display->display_options['fields']['continent']['field'] = 'continent';
  $handler->display->display_options['fields']['continent']['relationship'] = 'field_country_iso2';
  $handler->display->display_options['fields']['continent']['label'] = 'Region';
  $handler->display->display_options['fields']['continent']['continent_code'] = 0;
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_country']['id'] = 'field_country';
  $handler->display->display_options['fields']['field_country']['table'] = 'field_data_field_country';
  $handler->display->display_options['fields']['field_country']['field'] = 'field_country';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Chapter';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: User: First name */
  $handler->display->display_options['fields']['field_user_first_name']['id'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['field'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['relationship'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['fields']['field_user_first_name']['exclude'] = TRUE;
  /* Field: User: Last name */
  $handler->display->display_options['fields']['field_user_last_name']['id'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['table'] = 'field_data_field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['field'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['relationship'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['fields']['field_user_last_name']['exclude'] = TRUE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['fields']['uid']['label'] = 'Name';
  $handler->display->display_options['fields']['uid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['uid']['alter']['text'] = '<a href="/user/[uid]" target="_blank">[field_user_first_name]  [field_user_last_name]</a>';
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'field_chapter_leaders_target_id';
  /* Field: Field: Website */
  $handler->display->display_options['fields']['field_link_website']['id'] = 'field_link_website';
  $handler->display->display_options['fields']['field_link_website']['table'] = 'field_data_field_link_website';
  $handler->display->display_options['fields']['field_link_website']['field'] = 'field_link_website';
  $handler->display->display_options['fields']['field_link_website']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_link_website']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['field_link_website']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_link_website']['alter']['path'] = '[field_link_website-url]';
  $handler->display->display_options['fields']['field_link_website']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_link_website']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_website']['type'] = 'link_plain';
  /* Field: Field: Facebook */
  $handler->display->display_options['fields']['field_link_facebook']['id'] = 'field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['table'] = 'field_data_field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['field'] = 'field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_link_facebook']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['field_link_facebook']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_link_facebook']['alter']['path'] = '[field_link_facebook-url]';
  $handler->display->display_options['fields']['field_link_facebook']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_link_facebook']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_facebook']['type'] = 'link_plain';
  /* Field: Field: Twitter */
  $handler->display->display_options['fields']['field_link_twitter']['id'] = 'field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['table'] = 'field_data_field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['field'] = 'field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_link_twitter']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['field_link_twitter']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_link_twitter']['alter']['path'] = '[field_link_twitter-url]';
  $handler->display->display_options['fields']['field_link_twitter']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_link_twitter']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_twitter']['type'] = 'link_plain';
  /* Field: Field: Linkedin */
  $handler->display->display_options['fields']['field_link_linkedin']['id'] = 'field_link_linkedin';
  $handler->display->display_options['fields']['field_link_linkedin']['table'] = 'field_data_field_link_linkedin';
  $handler->display->display_options['fields']['field_link_linkedin']['field'] = 'field_link_linkedin';
  $handler->display->display_options['fields']['field_link_linkedin']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_link_linkedin']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['field_link_linkedin']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_link_linkedin']['alter']['path'] = '[field_link_linkedin-url]';
  $handler->display->display_options['fields']['field_link_linkedin']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_link_linkedin']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_linkedin']['type'] = 'link_plain';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'chapter' => 'chapter',
  );
  $handler->display->display_options['path'] = 'admin/community_stats/chapter_leaders';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Organizers Contact';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Organizers Contact';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'user-menu';

  /* Display: Organizers Contact export */
  $handler = $view->new_display('views_data_export', 'Organizers Contact export', 'organizers_contact_export');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%display-%timestamp-dd-%timestamp-mm-%timestamp-yyyy.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['separator'] = ';';
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['id'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['table'] = 'field_data_field_chapter_leaders';
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['field'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['label'] = 'chapter_leaders';
  $handler->display->display_options['relationships']['field_chapter_leaders_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_country_1']['id'] = 'field_country_1';
  $handler->display->display_options['fields']['field_country_1']['table'] = 'field_data_field_country';
  $handler->display->display_options['fields']['field_country_1']['field'] = 'field_country';
  $handler->display->display_options['fields']['field_country_1']['label'] = 'Region';
  $handler->display->display_options['fields']['field_country_1']['type'] = 'country_continent';
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_country']['id'] = 'field_country';
  $handler->display->display_options['fields']['field_country']['table'] = 'field_data_field_country';
  $handler->display->display_options['fields']['field_country']['field'] = 'field_country';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Chapter';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: User: First name */
  $handler->display->display_options['fields']['field_user_first_name']['id'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['table'] = 'field_data_field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['field'] = 'field_user_first_name';
  $handler->display->display_options['fields']['field_user_first_name']['relationship'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['fields']['field_user_first_name']['exclude'] = TRUE;
  /* Field: User: Last name */
  $handler->display->display_options['fields']['field_user_last_name']['id'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['table'] = 'field_data_field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['field'] = 'field_user_last_name';
  $handler->display->display_options['fields']['field_user_last_name']['relationship'] = 'field_chapter_leaders_target_id';
  $handler->display->display_options['fields']['field_user_last_name']['label'] = 'Name';
  $handler->display->display_options['fields']['field_user_last_name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_user_last_name']['alter']['text'] = '[field_user_first_name] [field_user_last_name]';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'field_chapter_leaders_target_id';
  /* Field: Field: Website */
  $handler->display->display_options['fields']['field_link_website']['id'] = 'field_link_website';
  $handler->display->display_options['fields']['field_link_website']['table'] = 'field_data_field_link_website';
  $handler->display->display_options['fields']['field_link_website']['field'] = 'field_link_website';
  $handler->display->display_options['fields']['field_link_website']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_website']['type'] = 'link_plain';
  /* Field: Field: Facebook */
  $handler->display->display_options['fields']['field_link_facebook']['id'] = 'field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['table'] = 'field_data_field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['field'] = 'field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_facebook']['type'] = 'link_plain';
  /* Field: Field: Twitter */
  $handler->display->display_options['fields']['field_link_twitter']['id'] = 'field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['table'] = 'field_data_field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['field'] = 'field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_twitter']['type'] = 'link_title_plain';
  /* Field: Field: Linkedin */
  $handler->display->display_options['fields']['field_link_linkedin']['id'] = 'field_link_linkedin';
  $handler->display->display_options['fields']['field_link_linkedin']['table'] = 'field_data_field_link_linkedin';
  $handler->display->display_options['fields']['field_link_linkedin']['field'] = 'field_link_linkedin';
  $handler->display->display_options['fields']['field_link_linkedin']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_linkedin']['type'] = 'link_plain';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'chapter' => 'chapter',
  );
  $handler->display->display_options['path'] = 'admin/community_stats/chapter_leaders/export';

  /* Display: Chapter Stats */
  $handler = $view->new_display('page', 'Chapter Stats', 'chapter_stats');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Chapter Stats';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'continent' => 'continent',
    'field_country' => 'field_country',
    'title_1' => 'title_1',
    'field_link_website' => 'field_link_website',
    'field_link_facebook' => 'field_link_facebook',
    'field_link_twitter' => 'field_link_twitter',
    'field_event_date' => 'field_event_date',
    'field_event_date_1' => 'field_event_date_1',
    'field_event_date_2' => 'field_event_date_2',
    'field_event_date_3' => 'field_event_date_3',
    'count' => 'count',
    'field_community_managers' => 'field_community_managers',
    'field_segment' => 'field_segment',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'continent' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_country' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_link_website' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_link_facebook' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_link_twitter' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_event_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_event_date_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_event_date_2' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_event_date_3' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'count' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_community_managers' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_segment' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
  $handler->display->display_options['header']['area']['content'] = '<p><a href="/admin/community_stats/chapter_stats/export">Export as CSV</a></p>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['id'] = 'reverse_field_chapter_node';
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['field'] = 'reverse_field_chapter_node';
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['label'] = 'field_chapter';
  /* Relationship: Flags: signup counter */
  $handler->display->display_options['relationships']['flag_count_rel']['id'] = 'flag_count_rel';
  $handler->display->display_options['relationships']['flag_count_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_count_rel']['field'] = 'flag_count_rel';
  $handler->display->display_options['relationships']['flag_count_rel']['label'] = 'join_chapter';
  $handler->display->display_options['relationships']['flag_count_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_count_rel']['flag'] = 'signup';
  /* Relationship: Content: Country (field_country) */
  $handler->display->display_options['relationships']['field_country_iso2']['id'] = 'field_country_iso2';
  $handler->display->display_options['relationships']['field_country_iso2']['table'] = 'field_data_field_country';
  $handler->display->display_options['relationships']['field_country_iso2']['field'] = 'field_country_iso2';
  $handler->display->display_options['relationships']['field_country_iso2']['label'] = 'country';
  /* Relationship: Flags: signup */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'signup';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'flagging';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['uid']['label'] = 'flag_user';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Countries: Continent */
  $handler->display->display_options['fields']['continent']['id'] = 'continent';
  $handler->display->display_options['fields']['continent']['table'] = 'countries_country';
  $handler->display->display_options['fields']['continent']['field'] = 'continent';
  $handler->display->display_options['fields']['continent']['relationship'] = 'field_country_iso2';
  $handler->display->display_options['fields']['continent']['label'] = 'Region';
  $handler->display->display_options['fields']['continent']['continent_code'] = 0;
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_country']['id'] = 'field_country';
  $handler->display->display_options['fields']['field_country']['table'] = 'field_data_field_country';
  $handler->display->display_options['fields']['field_country']['field'] = 'field_country';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = 'Chapter';
  /* Field: Field: Website */
  $handler->display->display_options['fields']['field_link_website']['id'] = 'field_link_website';
  $handler->display->display_options['fields']['field_link_website']['table'] = 'field_data_field_link_website';
  $handler->display->display_options['fields']['field_link_website']['field'] = 'field_link_website';
  $handler->display->display_options['fields']['field_link_website']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_link_website']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['field_link_website']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_link_website']['alter']['path'] = '[field_link_website-url]';
  $handler->display->display_options['fields']['field_link_website']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_link_website']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_website']['type'] = 'link_plain';
  $handler->display->display_options['fields']['field_link_website']['group_column'] = 'entity_id';
  /* Field: Field: Facebook */
  $handler->display->display_options['fields']['field_link_facebook']['id'] = 'field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['table'] = 'field_data_field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['field'] = 'field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_link_facebook']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['field_link_facebook']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_link_facebook']['alter']['path'] = '[field_link_facebook-url]';
  $handler->display->display_options['fields']['field_link_facebook']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_link_facebook']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_facebook']['type'] = 'link_plain';
  $handler->display->display_options['fields']['field_link_facebook']['group_column'] = 'entity_id';
  /* Field: Field: Twitter */
  $handler->display->display_options['fields']['field_link_twitter']['id'] = 'field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['table'] = 'field_data_field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['field'] = 'field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_link_twitter']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['field_link_twitter']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_link_twitter']['alter']['path'] = '[field_link_twitter-url]';
  $handler->display->display_options['fields']['field_link_twitter']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['field_link_twitter']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_twitter']['type'] = 'link_plain';
  $handler->display->display_options['fields']['field_link_twitter']['group_column'] = 'entity_id';
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date']['id'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date']['field'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date']['relationship'] = 'reverse_field_chapter_node';
  $handler->display->display_options['fields']['field_event_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_event_date']['label'] = 'Date of Last Event';
  $handler->display->display_options['fields']['field_event_date']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_event_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_event_date']['group_columns'] = array(
    'value' => 'value',
  );
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date_1']['id'] = 'field_event_date_1';
  $handler->display->display_options['fields']['field_event_date_1']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date_1']['field'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date_1']['relationship'] = 'reverse_field_chapter_node';
  $handler->display->display_options['fields']['field_event_date_1']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_event_date_1']['label'] = 'Time Ago';
  $handler->display->display_options['fields']['field_event_date_1']['type'] = 'format_interval';
  $handler->display->display_options['fields']['field_event_date_1']['settings'] = array(
    'interval' => '2',
    'interval_display' => 'time ago',
  );
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date_2']['id'] = 'field_event_date_2';
  $handler->display->display_options['fields']['field_event_date_2']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date_2']['field'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date_2']['relationship'] = 'reverse_field_chapter_node';
  $handler->display->display_options['fields']['field_event_date_2']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['field_event_date_2']['label'] = '# Events/12 months';
  $handler->display->display_options['fields']['field_event_date_2']['separator'] = ', ';
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date_3']['id'] = 'field_event_date_3';
  $handler->display->display_options['fields']['field_event_date_3']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date_3']['field'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date_3']['relationship'] = 'reverse_field_chapter_node';
  $handler->display->display_options['fields']['field_event_date_3']['group_type'] = 'min';
  $handler->display->display_options['fields']['field_event_date_3']['label'] = 'Next Event';
  $handler->display->display_options['fields']['field_event_date_3']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Flags: Flag counter */
  $handler->display->display_options['fields']['count']['id'] = 'count';
  $handler->display->display_options['fields']['count']['table'] = 'flag_counts';
  $handler->display->display_options['fields']['count']['field'] = 'count';
  $handler->display->display_options['fields']['count']['relationship'] = 'flag_count_rel';
  $handler->display->display_options['fields']['count']['label'] = '# Members';
  /* Field: Content: Community Managers */
  $handler->display->display_options['fields']['field_community_managers']['id'] = 'field_community_managers';
  $handler->display->display_options['fields']['field_community_managers']['table'] = 'field_data_field_community_managers';
  $handler->display->display_options['fields']['field_community_managers']['field'] = 'field_community_managers';
  $handler->display->display_options['fields']['field_community_managers']['group_type'] = 'stddev_pop';
  $handler->display->display_options['fields']['field_community_managers']['label'] = 'Managers';
  $handler->display->display_options['fields']['field_community_managers']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['field_community_managers']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['field_community_managers']['delta_offset'] = '0';
  /* Field: Field: Industry Segment */
  $handler->display->display_options['fields']['field_segment']['id'] = 'field_segment';
  $handler->display->display_options['fields']['field_segment']['table'] = 'field_data_field_segment';
  $handler->display->display_options['fields']['field_segment']['field'] = 'field_segment';
  $handler->display->display_options['fields']['field_segment']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_segment']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_segment']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_segment']['separator'] = ', ';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'chapter' => 'chapter',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['path'] = 'admin/community_stats/chapter_stats';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Chapter Stats';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Chapter Stats export */
  $handler = $view->new_display('views_data_export', 'Chapter Stats export', 'chapter_stats_export');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%display-%timestamp-dd-%timestamp-mm-%timestamp-yyyy.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['separator'] = ';';
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['id'] = 'reverse_field_chapter_node';
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['field'] = 'reverse_field_chapter_node';
  $handler->display->display_options['relationships']['reverse_field_chapter_node']['label'] = 'field_chapter';
  /* Relationship: Flags: signup counter */
  $handler->display->display_options['relationships']['flag_count_rel']['id'] = 'flag_count_rel';
  $handler->display->display_options['relationships']['flag_count_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_count_rel']['field'] = 'flag_count_rel';
  $handler->display->display_options['relationships']['flag_count_rel']['label'] = 'join_chapter';
  $handler->display->display_options['relationships']['flag_count_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_count_rel']['flag'] = 'signup';
  /* Relationship: Flags: signup */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'signup';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'flagging';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['uid']['label'] = 'flag_user';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_country_1']['id'] = 'field_country_1';
  $handler->display->display_options['fields']['field_country_1']['table'] = 'field_data_field_country';
  $handler->display->display_options['fields']['field_country_1']['field'] = 'field_country';
  $handler->display->display_options['fields']['field_country_1']['label'] = 'Region';
  $handler->display->display_options['fields']['field_country_1']['type'] = 'country_continent';
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_country']['id'] = 'field_country';
  $handler->display->display_options['fields']['field_country']['table'] = 'field_data_field_country';
  $handler->display->display_options['fields']['field_country']['field'] = 'field_country';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = 'Chapter';
  /* Field: Field: Website */
  $handler->display->display_options['fields']['field_link_website']['id'] = 'field_link_website';
  $handler->display->display_options['fields']['field_link_website']['table'] = 'field_data_field_link_website';
  $handler->display->display_options['fields']['field_link_website']['field'] = 'field_link_website';
  $handler->display->display_options['fields']['field_link_website']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_website']['type'] = 'link_plain';
  $handler->display->display_options['fields']['field_link_website']['group_column'] = 'entity_id';
  /* Field: Field: Facebook */
  $handler->display->display_options['fields']['field_link_facebook']['id'] = 'field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['table'] = 'field_data_field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['field'] = 'field_link_facebook';
  $handler->display->display_options['fields']['field_link_facebook']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_facebook']['group_column'] = 'entity_id';
  /* Field: Field: Twitter */
  $handler->display->display_options['fields']['field_link_twitter']['id'] = 'field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['table'] = 'field_data_field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['field'] = 'field_link_twitter';
  $handler->display->display_options['fields']['field_link_twitter']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_link_twitter']['group_column'] = 'entity_id';
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date']['id'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date']['field'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date']['relationship'] = 'reverse_field_chapter_node';
  $handler->display->display_options['fields']['field_event_date']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_event_date']['label'] = 'Date of Last Event';
  $handler->display->display_options['fields']['field_event_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_event_date']['group_columns'] = array(
    'value' => 'value',
  );
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date_1']['id'] = 'field_event_date_1';
  $handler->display->display_options['fields']['field_event_date_1']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date_1']['field'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date_1']['relationship'] = 'reverse_field_chapter_node';
  $handler->display->display_options['fields']['field_event_date_1']['group_type'] = 'max';
  $handler->display->display_options['fields']['field_event_date_1']['label'] = 'Time Ago';
  $handler->display->display_options['fields']['field_event_date_1']['type'] = 'format_interval';
  $handler->display->display_options['fields']['field_event_date_1']['settings'] = array(
    'interval' => '2',
    'interval_display' => 'time ago',
  );
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date_2']['id'] = 'field_event_date_2';
  $handler->display->display_options['fields']['field_event_date_2']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date_2']['field'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date_2']['relationship'] = 'reverse_field_chapter_node';
  $handler->display->display_options['fields']['field_event_date_2']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['field_event_date_2']['label'] = '# Events/12 months';
  $handler->display->display_options['fields']['field_event_date_2']['separator'] = ', ';
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_event_date_3']['id'] = 'field_event_date_3';
  $handler->display->display_options['fields']['field_event_date_3']['table'] = 'field_data_field_event_date';
  $handler->display->display_options['fields']['field_event_date_3']['field'] = 'field_event_date';
  $handler->display->display_options['fields']['field_event_date_3']['relationship'] = 'reverse_field_chapter_node';
  $handler->display->display_options['fields']['field_event_date_3']['group_type'] = 'min';
  $handler->display->display_options['fields']['field_event_date_3']['label'] = 'Next Event';
  $handler->display->display_options['fields']['field_event_date_3']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Flags: Flag counter */
  $handler->display->display_options['fields']['count']['id'] = 'count';
  $handler->display->display_options['fields']['count']['table'] = 'flag_counts';
  $handler->display->display_options['fields']['count']['field'] = 'count';
  $handler->display->display_options['fields']['count']['relationship'] = 'flag_count_rel';
  $handler->display->display_options['fields']['count']['label'] = '# Members';
  /* Field: Content: Community Managers */
  $handler->display->display_options['fields']['field_community_managers']['id'] = 'field_community_managers';
  $handler->display->display_options['fields']['field_community_managers']['table'] = 'field_data_field_community_managers';
  $handler->display->display_options['fields']['field_community_managers']['field'] = 'field_community_managers';
  $handler->display->display_options['fields']['field_community_managers']['group_type'] = 'stddev_pop';
  $handler->display->display_options['fields']['field_community_managers']['label'] = 'Managers';
  $handler->display->display_options['fields']['field_community_managers']['settings'] = array(
    'link' => 1,
  );
  $handler->display->display_options['fields']['field_community_managers']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['field_community_managers']['delta_offset'] = '0';
  /* Field: Field: Industry Segment */
  $handler->display->display_options['fields']['field_segment']['id'] = 'field_segment';
  $handler->display->display_options['fields']['field_segment']['table'] = 'field_data_field_segment';
  $handler->display->display_options['fields']['field_segment']['field'] = 'field_segment';
  $handler->display->display_options['fields']['field_segment']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_segment']['group_type'] = 'count';
  $handler->display->display_options['fields']['field_segment']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_segment']['separator'] = ', ';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'chapter' => 'chapter',
  );
  $handler->display->display_options['path'] = 'admin/community_stats/chapter_stats/export';

  /* Display: Attachment */
  $handler = $view->new_display('attachment', 'Attachment', 'attachment_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $export['reports'] = $view;

  return $export;
}

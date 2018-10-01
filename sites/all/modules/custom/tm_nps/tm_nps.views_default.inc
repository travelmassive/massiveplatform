<?php
/**
 * @file
 * tm_nps.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tm_nps_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'tm_nps';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'tm_nps';
  $view->human_name = 'Net Promoter Scores';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Net Promoter Score';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'load_more';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['waypoint']['infinite'] = 0;
  $handler->display->display_options['pager']['options']['more_button_text'] = 'Show more reviews';
  $handler->display->display_options['pager']['options']['effects']['speed'] = 'slow';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'realname' => 'realname',
    'title' => 'title',
    'score' => 'score',
    'comment' => 'comment',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'realname' => array(
      'sortable' => 0,
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
    'score' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'comment' => array(
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
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>Member Reviews</h2>

👉 Share our <a target="_blank" href="/review">Review Page</a> with members to get feedback.
';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '<br><br>
<h3>What is Net Promoter Score (NPS)?</h3>

NPS, or Net Promoter Score, is a popular way to measure customer satisfaction for brands.
Members rate from 10-0 based on how likely they are to recommend us to their friends of colleagues (10 = very likely, 0 = unlikely).

<h3>How is NPS calculated?</h3>

😒 Score of 6 or below are "Detractors"
🤔 Score of 7 or 8 are "Passives"
❤️ Score of 9 or 10 are "Promoters"

<strong>Net Promoter Score = (% of Promoters - % of Detractors).</strong>

<h3>What is a good NPS score?</h3>

A score above 0 is good. A score above 50 is excellent, and above 70 is exceptional.';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Relationship: TM NPS: User uid */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'tm_nps';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Realname: Real name */
  $handler->display->display_options['fields']['realname']['id'] = 'realname';
  $handler->display->display_options['fields']['realname']['table'] = 'realname';
  $handler->display->display_options['fields']['realname']['field'] = 'realname';
  $handler->display->display_options['fields']['realname']['relationship'] = 'uid';
  $handler->display->display_options['fields']['realname']['label'] = 'Member Name';
  /* Field: TM NPS: Score */
  $handler->display->display_options['fields']['score']['id'] = 'score';
  $handler->display->display_options['fields']['score']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['score']['field'] = 'score';
  $handler->display->display_options['fields']['score']['label'] = 'Net Promoter Score (0-10)';
  /* Field: TM NPS: Comment */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['comment']['field'] = 'comment';
  $handler->display->display_options['fields']['comment']['label'] = 'Feedback';
  $handler->display->display_options['fields']['comment']['element_label_colon'] = FALSE;
  /* Field: TM NPS: Timestamp */
  $handler->display->display_options['fields']['created']['id'] = 'timestamp';
  $handler->display->display_options['fields']['created']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['created']['field'] = 'timestamp';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';

  /* Display: Net Promoter Score */
  $handler = $view->new_display('page', 'Net Promoter Score', 'tm_nps_report_chapter');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
    7 => '7',
    5 => '5',
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>Member Feedback</h2>

👉 Share our <a target="_blank" href="/feedback">Feedback Page</a> with members to get feedback.
';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: TM NPS: Chapter id */
  $handler->display->display_options['relationships']['chapter_id']['id'] = 'chapter_id';
  $handler->display->display_options['relationships']['chapter_id']['table'] = 'tm_nps';
  $handler->display->display_options['relationships']['chapter_id']['field'] = 'chapter_id';
  $handler->display->display_options['relationships']['chapter_id']['required'] = TRUE;
  /* Relationship: TM NPS: User uid */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'tm_nps';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Realname: Real name */
  $handler->display->display_options['fields']['realname']['id'] = 'realname';
  $handler->display->display_options['fields']['realname']['table'] = 'realname';
  $handler->display->display_options['fields']['realname']['field'] = 'realname';
  $handler->display->display_options['fields']['realname']['relationship'] = 'uid';
  $handler->display->display_options['fields']['realname']['label'] = 'Member Name';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'chapter_id';
  $handler->display->display_options['fields']['title']['label'] = 'Home Chapter';
  /* Field: TM NPS: Score */
  $handler->display->display_options['fields']['score']['id'] = 'score';
  $handler->display->display_options['fields']['score']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['score']['field'] = 'score';
  $handler->display->display_options['fields']['score']['label'] = 'Rating (10-0)';
  /* Field: TM NPS: Comment */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['comment']['field'] = 'comment';
  $handler->display->display_options['fields']['comment']['label'] = 'Feedback';
  $handler->display->display_options['fields']['comment']['element_label_colon'] = FALSE;
  /* Field: TM NPS: Timestamp */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['created']['field'] = 'timestamp';
  $handler->display->display_options['fields']['created']['label'] = 'When';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: TM NPS: Chapter id */
  $handler->display->display_options['arguments']['chapter_id']['id'] = 'chapter_id';
  $handler->display->display_options['arguments']['chapter_id']['table'] = 'tm_nps';
  $handler->display->display_options['arguments']['chapter_id']['field'] = 'chapter_id';
  $handler->display->display_options['arguments']['chapter_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['chapter_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['chapter_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['chapter_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'feedback/results/%';

  /* Display: Net Promoter Score (Global) */
  $handler = $view->new_display('page', 'Net Promoter Score (Global)', 'tm_nps_report_global');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
    7 => '7',
    5 => '5',
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>Member Feedback &mdash; All Chapters</h2>

👉 Share our <a target="_blank" href="/feedback">Feedback Page</a> with members to get feedback.
';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: TM NPS: User uid */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'tm_nps';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: TM NPS: Chapter id */
  $handler->display->display_options['relationships']['chapter_id']['id'] = 'chapter_id';
  $handler->display->display_options['relationships']['chapter_id']['table'] = 'tm_nps';
  $handler->display->display_options['relationships']['chapter_id']['field'] = 'chapter_id';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Realname: Real name */
  $handler->display->display_options['fields']['realname']['id'] = 'realname';
  $handler->display->display_options['fields']['realname']['table'] = 'realname';
  $handler->display->display_options['fields']['realname']['field'] = 'realname';
  $handler->display->display_options['fields']['realname']['relationship'] = 'uid';
  $handler->display->display_options['fields']['realname']['label'] = 'Member Name';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'chapter_id';
  $handler->display->display_options['fields']['title']['label'] = 'Home Chapter';
  /* Field: TM NPS: Score */
  $handler->display->display_options['fields']['score']['id'] = 'score';
  $handler->display->display_options['fields']['score']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['score']['field'] = 'score';
  $handler->display->display_options['fields']['score']['label'] = 'Rating (10-0)';
  /* Field: TM NPS: Comment */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['comment']['field'] = 'comment';
  $handler->display->display_options['fields']['comment']['label'] = 'Feedback';
  $handler->display->display_options['fields']['comment']['element_label_colon'] = FALSE;
  /* Field: TM NPS: Timestamp */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'tm_nps';
  $handler->display->display_options['fields']['created']['field'] = 'timestamp';
  $handler->display->display_options['fields']['created']['label'] = 'When';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  $handler->display->display_options['path'] = 'feedback/results/all';
  $export['tm_nps'] = $view;

  return $export;
}

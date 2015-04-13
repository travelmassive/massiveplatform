<?php
/**
 * @file
 * tm_flags.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tm_flags_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'member_listings';
  $view->description = 'A series of member listings';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Member listings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Member Listing';
  $handler->display->display_options['css_class'] = 'user-list related-list';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View all members';
  $handler->display->display_options['link_display'] = 'members_signedup_page';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'ds';
  $handler->display->display_options['row_options']['view_mode'] = 'grid';
  $handler->display->display_options['row_options']['alternating'] = 0;
  $handler->display->display_options['row_options']['grouping'] = 0;
  $handler->display->display_options['row_options']['advanced'] = 0;
  $handler->display->display_options['row_options']['delta_fieldset']['delta_fields'] = array();
  $handler->display->display_options['row_options']['grouping_fieldset']['group_field'] = 'flagging|timestamp';
  $handler->display->display_options['row_options']['default_fieldset']['view_mode'] = 'grid';
  $handler->display->display_options['row_options']['alternating_fieldset']['item_0'] = 'full';
  $handler->display->display_options['row_options']['alternating_fieldset']['item_1'] = 'full';
  $handler->display->display_options['row_options']['alternating_fieldset']['item_2'] = 'full';
  $handler->display->display_options['row_options']['alternating_fieldset']['item_3'] = 'full';
  $handler->display->display_options['row_options']['alternating_fieldset']['item_4'] = 'full';
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'signup';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';

  /* Display: Block - registered to event */
  $handler = $view->new_display('block', 'Block - registered to event', 'members_signedup_block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Registered to event';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View all attendees';
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['link_url'] = 'node/!1/attendees';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'event_register';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['block_description'] = 'Registered to event';

  /* Display: Page - registered to event */
  $handler = $view->new_display('page', 'Page - registered to event', 'members_signedup_page');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'user-list grid';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View more';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No members signed up yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'event_register';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['entity_id']['title'] = 'Who\'s going?';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'node/%/attendees';

  /* Display: Block - following org */
  $handler = $view->new_display('block', 'Block - following org', 'block_following_org');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Following Organisations';
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'members_following_org_page';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'follow_organizations';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';

  /* Display: Block - following */
  $handler = $view->new_display('block', 'Block - following', 'block_following');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Following';
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_1';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: follow_members */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'follow_members';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
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

  /* Display: Page - following org */
  $handler = $view->new_display('page', 'Page - following org', 'members_following_org_page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Following Organizations';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'user-list grid';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View more';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'follow_organizations';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['entity_id']['title'] = 'Members following %1';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'node/%/followers';

  /* Display: Page - followers */
  $handler = $view->new_display('page', 'Page - followers', 'members_following_members_page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Followers';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'user-list grid';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View more';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'follow_members';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'user/%/followers';

  /* Display: Block - followers */
  $handler = $view->new_display('block', 'Block - followers', 'block_followers');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Followers';
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'members_following_members_page';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'follow_members';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['entity_id']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page - following */
  $handler = $view->new_display('page', 'Page - following', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Following';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'user-list grid';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View more';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '18';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: follow_members */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'follow_members';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
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
  $handler->display->display_options['path'] = 'user/%/following';

  /* Display: Block - Chapter Members */
  $handler = $view->new_display('block', 'Block - Chapter Members', 'block_chapter_members_list');
  $handler->display->display_options['defaults']['link_display'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_chapter_members';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: User's flaggings */
  $handler->display->display_options['relationships']['flag_user_content_rel']['id'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['table'] = 'users';
  $handler->display->display_options['relationships']['flag_user_content_rel']['field'] = 'flag_user_content_rel';
  $handler->display->display_options['relationships']['flag_user_content_rel']['flag'] = 'signup';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    6 => '6',
  );
  $handler->display->display_options['block_description'] = 'Chapter Members';

  /* Display: Page - Chapter Members */
  $handler = $view->new_display('page', 'Page - Chapter Members', 'page_chapter_members');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Approved Chapter Members';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'user-list grid';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'View all members';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flagging';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Flags: Content ID */
  $handler->display->display_options['arguments']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['table'] = 'flagging';
  $handler->display->display_options['arguments']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['arguments']['entity_id']['relationship'] = 'flag_user_content_rel';
  $handler->display->display_options['arguments']['entity_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['entity_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    6 => '6',
  );
  $handler->display->display_options['path'] = 'node/%/members';
  $export['member_listings'] = $view;

  return $export;
}

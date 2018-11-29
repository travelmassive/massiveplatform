<?php

/**
 * @file
 * tm_users.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function tm_users_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'community_page';
  $context->description = 'Context for the community page';
  $context->tag = 'community';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'community:page' => 'community:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-community-page' => array(
          'module' => 'views',
          'delta' => '-exp-community-page',
          'region' => 'header',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for the community page');
  t('community');
  $export['community_page'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'leaders_page';
  $context->description = 'Context for the leaders page';
  $context->tag = 'community';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'chapter_leaders:page' => 'chapter_leaders:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-chapter_leaders-page' => array(
          'module' => 'views',
          'delta' => '-exp-chapter_leaders-page',
          'region' => 'header',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for the leaders page');
  t('community');
  $export['leaders_page'] = $context;

  return $export;
}

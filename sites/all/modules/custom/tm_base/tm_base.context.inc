<?php
/**
 * @file
 * tm_base.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function tm_base_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'frontpage';
  $context->description = 'The main frontpage context';
  $context->tag = 'front';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-events-front_block' => array(
          'module' => 'views',
          'delta' => 'events-front_block',
          'region' => 'content',
          'weight' => '1',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('The main frontpage context');
  t('front');
  $export['frontpage'] = $context;

  return $export;
}

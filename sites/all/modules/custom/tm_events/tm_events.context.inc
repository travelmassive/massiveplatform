<?php

/**
 * @file
 * tm_events.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function tm_events_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'event_page';
  $context->description = 'Context for the event page';
  $context->tag = 'event';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'events:page' => 'events:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-events-page' => array(
          'module' => 'views',
          'delta' => '-exp-events-page',
          'region' => 'header',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for the event page');
  t('event');
  $export['event_page'] = $context;

  return $export;
}

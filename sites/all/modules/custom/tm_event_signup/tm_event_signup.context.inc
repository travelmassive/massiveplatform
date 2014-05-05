<?php
/**
 * @file
 * tm_event_signup.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function tm_event_signup_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'signup_event_page';
  $context->description = 'The signup of an event page';
  $context->tag = 'signup';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'event' => 'event',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-attendees-block_1' => array(
          'module' => 'views',
          'delta' => 'attendees-block_1',
          'region' => 'content',
          'weight' => '1',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('The signup of an event page');
  t('signup');
  $export['signup_event_page'] = $context;

  return $export;
}

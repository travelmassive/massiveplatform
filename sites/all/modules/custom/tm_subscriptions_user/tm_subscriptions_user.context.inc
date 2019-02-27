<?php

/**
 * @file
 * tm_subscriptions_user.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function tm_subscriptions_user_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'subscribers_page';
  $context->description = 'Context for the subscribers page';
  $context->tag = 'community';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'subscribed_members' => 'subscribed_members',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-6d6a29fe2ae53702bdd9ecaa0c690cc6' => array(
          'module' => 'views',
          'delta' => '6d6a29fe2ae53702bdd9ecaa0c690cc6',
          'region' => 'header',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for the subscribers page');
  t('community');
  $export['subscribers_page'] = $context;

  return $export;
}

<?php

/**
 * @file
 * tm_organizations.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function tm_organizations_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'company_page';
  $context->description = 'Context for the company listing';
  $context->tag = 'companies';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'companies:page' => 'companies:page',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-companies-page' => array(
          'module' => 'views',
          'delta' => '-exp-companies-page',
          'region' => 'header',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context for the company listing');
  t('companies');
  $export['company_page'] = $context;

  return $export;
}

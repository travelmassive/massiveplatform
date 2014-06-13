<?php
/**
 * @file
 * tm_twitter_signin.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_twitter_signin_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_twitter_api';
  $strongarm->value = 'https://api.twitter.com';
  $export['tm_twitter_api'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_twitter_host';
  $strongarm->value = 'http://twitter.com';
  $export['tm_twitter_host'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_twitter_search';
  $strongarm->value = 'http://search.twitter.com';
  $export['tm_twitter_search'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_twitter_tinyurl';
  $strongarm->value = 'http://tinyurl.com';
  $export['tm_twitter_tinyurl'] = $strongarm;

  return $export;
}

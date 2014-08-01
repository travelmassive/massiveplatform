<?php
/**
 * @file
 * tm_users_external_signin.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function tm_users_external_signin_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'oauth_common_enable_provider';
  $strongarm->value = 0;
  $export['oauth_common_enable_provider'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'oauth_common_login_path';
  $strongarm->value = 'user/login';
  $export['oauth_common_login_path'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'oauth_common_request_token_lifetime';
  $strongarm->value = '7200';
  $export['oauth_common_request_token_lifetime'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_twitter_api';
  $strongarm->value = 'https://api.twitter.com';
  $export['tm_twitter_api'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_twitter_consumer_key';
  $strongarm->value = '7uSkI5QYRTgx3Rb0O319hZ0vO';
  $export['tm_twitter_consumer_key'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tm_twitter_consumer_secret';
  $strongarm->value = 'bVtNzb00VFOIEBuE2MsyOHVblWOikL7YVkTBXhNBDd1fetBzpR';
  $export['tm_twitter_consumer_secret'] = $strongarm;

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

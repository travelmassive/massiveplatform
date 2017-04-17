<?php

/**
 * @file
 * Rules hook implementations for Simple FB Connect module.
 */

/**
 * Implements hook_rules_event_info().
 *
 * Defines a rules event to react on Facebook login and user creation.
 *
 * @ingroup rules
 */
function simple_fb_connect_rules_event_info() {
  return array(
    'simple_fb_connect_login' => array(
      'label' => t('User logs in to their existing Drupal account with Facebook'),
      'module' => 'simple_fb_connect',
      'group' => t('Social Media'),
      'variables' => array(
        'user' => array('type' => 'user', 'label' => t('The user logging in with Facebook')),
      ),
    ),
    'simple_fb_connect_registration' => array(
      'label' => t('User registers a new account connected to Facebook'),
      'module' => 'simple_fb_connect',
      'group' => t('Social Media'),
      'variables' => array(
        'user' => array('type' => 'user', 'label' => t('The user registering the account')),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 *
 * @ingroup rules
 */
function simple_fb_connect_rules_condition_info() {
  return array(
    'simple_fb_connect_user_has_facebook_session' => array(
      'label' => t('User has Facebook session'),
      'module' => 'simple_fb_connect',
      'group' => t('Social Media'),
    ),
  );
}

/**
 * Rule condition: Check if user has a valid Facebook session.
 *
 * @return bool
 *   True, if use has a valid Facebook session.
 *   False otherwise.
 */
function simple_fb_connect_user_has_facebook_session() {
  return simple_fb_connect_get_session() ? TRUE : FALSE;
}

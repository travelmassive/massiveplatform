<?php
/**
 * @file tm_twitter_signin.pages
 * Page callbacks for tm_twitter_signin module.
 * This is heavily based on the twitter_signin module (http://drupal.org/project/twitter)
 * 
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */
 
/**
 * Twitter settings form.
 */
function tm_twitter_signin_admin_form($form, &$form_state) {

  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => t('OAuth Settings'),
    '#access' => module_exists('oauth_common'),
    '#description' => t('To enable OAuth based access for twitter, you must <a href="@url">register your application</a> with Twitter and add the provided keys here.', array('@url' => 'https://dev.twitter.com/apps/new')),
  );
  $form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#markup' => url('tm_twitter/oauth/callback', array('absolute' => TRUE)),
  );
  $form['oauth']['tm_twitter_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer key'),
    '#default_value' => variable_get('tm_twitter_consumer_key', NULL),
  );
  $form['oauth']['tm_twitter_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Consumer secret'),
    '#default_value' => variable_get('tm_twitter_consumer_secret', NULL),
  );

  // Twitter external APIs settings.
  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter Settings'),
    '#description' => t('The following settings connect Twitter module with external APIs. ' .
      'Change them if, for example, you want to use Identi.ca.'),
  );
  $form['twitter']['tm_twitter_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter host'),
    '#default_value' => variable_get('tm_twitter_host', TWITTER_HOST),
  );
  $form['twitter']['tm_twitter_api'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter API'),
    '#default_value' => variable_get('tm_twitter_api', TWITTER_API),
  );
  $form['twitter']['tm_twitter_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter search'),
    '#default_value' => variable_get('tm_twitter_search', TWITTER_SEARCH),
  );
  $form['twitter']['tm_twitter_tinyurl'] = array(
    '#type' => 'textfield',
    '#title' => t('TinyURL'),
    '#default_value' => variable_get('tm_twitter_tinyurl', TWITTER_TINYURL),
  );

  return system_settings_form($form);
}

<?php
/**
 * @file
 * tm_base.admin.inc
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */

/**
* Implements hook_variable_info().
*/
function tm_base_variable_info($options) {
  $variable['tm_base_copyright'] = array(
    'title' => t('Copyright notice'),
    'description' => t('The copyright notice for the site.'),
    'type' => 'string',
    'access' => 'administer content',
  );
  $variable['tm_base_homepage_title_anon'] = array(
    'title' => t('Homepage title - anonymous'),
    'description' => t('The homepage title for the anonymous users.'),
    'type' => 'string',
    'access' => 'administer content',
  );
  $variable['tm_base_homepage_description_anon'] = array(
    'title' => t('Homepage description - anonymous'),
    'description' => t('The homepage description for the anonymous users.'),
    'type' => 'string',
    'access' => 'administer content',
  );
  $variable['tm_base_homepage_title_members'] = array(
    'title' => t('Homepage title - members'),
    'description' => t('The homepage title for logged in members.'),
    'type' => 'string',
    'access' => 'administer content',
  );
  $variable['tm_base_homepage_description_members'] = array(
    'title' => t('Homepage description - members'),
    'description' => t('The homepage description for logged in members.'),
    'type' => 'string',
    'access' => 'administer content',
  );
  return $variable;
}

/**
 * Implements hook_form_FORM_ID_alter()
 */
function tm_base_form_system_site_information_settings_alter(&$form, &$form_state) {
  $form['site_information']['tm_base_copyright'] = array(
    '#type' => 'textfield',
    '#title' => 'Copyright notice',
    '#default_value' => variable_get('tm_base_copyright', '')
  );
  $form['front_page']['tm_base_homepage_title_anon'] = array(
    '#type' => 'textfield',
    '#title' => 'Homepage title - anonmyous users',
    '#default_value' => variable_get('tm_base_homepage_title_anon', ''),
    '#description' => t('This field supports tokens. See available tokens below.'),
    '#element_validate' => array('token_element_validate'),
    '#token_types' => array(),
  );
  $form['front_page']['tm_base_homepage_description_anon'] = array(
    '#type' => 'textarea',
    '#title' => 'Homepage description - anonmyous users',
    '#default_value' => variable_get('tm_base_homepage_description_anon', ''),
    '#description' => t('This field supports tokens. See available tokens below.'),
    '#element_validate' => array('token_element_validate'),
    '#token_types' => array(),
  );
  $form['front_page']['tm_base_homepage_title_members'] = array(
    '#type' => 'textfield',
    '#title' => 'Homepage title - members',
    '#default_value' => variable_get('tm_base_homepage_title_members', ''),
    '#description' => t('This field supports tokens. See available tokens below.'),
    '#element_validate' => array('token_element_validate'),
    '#token_types' => array(),
  );
  $form['front_page']['tm_base_homepage_description_members'] = array(
    '#type' => 'textarea',
    '#title' => 'Homepage description - members',
    '#default_value' => variable_get('tm_base_homepage_description_members', ''),
    '#description' => t('This field supports tokens. See available tokens below.'),
    '#element_validate' => array('token_element_validate'),
    '#token_types' => array(),
  );
  
  $form['front_page']['tokens'] = array(
    '#markup' => theme('token_tree_link', array(
      // The token types that have specific context. Can be multiple token types like 'term' and/or 'user'
      'token_types' => array('node'),
      // A boolean TRUE or FALSE whether to include 'global' context tokens like [current-user:*] or [site:*]. Defaults to TRUE.
      'global_types' => TRUE,
      // A boolean whether to include the 'Click this token to insert in into the the focused textfield' JavaScript functionality. Defaults to TRUE.
      'click_insert' => TRUE
    ))
  );
}

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
    '#default_value' => variable_get('tm_base_homepage_title_anon', '')
  );
  $form['front_page']['tm_base_homepage_description_anon'] = array(
    '#type' => 'textarea',
    '#title' => 'Homepage description - anonmyous users',
    '#default_value' => variable_get('tm_base_homepage_description_anon', '')
  );
  $form['front_page']['tm_base_homepage_title_members'] = array(
    '#type' => 'textfield',
    '#title' => 'Homepage title - members',
    '#default_value' => variable_get('tm_base_homepage_title_members', '')
  );
  $form['front_page']['tm_base_homepage_description_members'] = array(
    '#type' => 'textarea',
    '#title' => 'Homepage description - members',
    '#default_value' => variable_get('tm_base_homepage_description_members', '')
  );
}

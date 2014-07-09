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
  $variable['tm_base_copright'] = array(
    'title' => t('Copright notice'),
    'description' => t('The copright notice for the site.'),
    'type' => 'string',
    'access' => 'administer content',
  );
  $variable['tm_base_homepage_title'] = array(
    'title' => t('Homepage title'),
    'description' => t('The title for the homepage.'),
    'type' => 'string',
    'access' => 'administer content',
  );
  $variable['tm_base_homepage_description'] = array(
    'title' => t('Homepage description'),
    'description' => t('The description for the homepage.'),
    'type' => 'string',
    'access' => 'administer content',
  );
  return $variable;
}

/**
 * Implements hook_form_FORM_ID_alter()
 */
function tm_base_form_system_site_information_settings_alter(&$form, &$form_state) {
  $form['site_information']['tm_base_copright'] = array(
    '#type' => 'textfield',
    '#title' => 'Copright notice',
    '#default_value' => variable_get('tm_base_copright', '')
  );
  $form['front_page']['tm_base_homepage_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Homepage title',
    '#default_value' => variable_get('tm_base_homepage_title', '')
  );
  $form['front_page']['tm_base_homepage_description'] = array(
    '#type' => 'textarea',
    '#title' => 'Homepage description',
    '#default_value' => variable_get('tm_base_homepage_description', '')
  );
}

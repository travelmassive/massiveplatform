<?php

/**
 * @file
 *   Administrative pages for the token variables.
 */

/**
 * Build token_var_settings_form form.
 *
 * @param array $form_state
 * @return array The created form.
 */
function token_var_settings_form($form, &$form_state) {
  $form = array();
  _token_var_get_variables($form);
  $form = system_settings_form($form);
  $form['#submit'][] .= 'token_var_settings_form_submit';
  $form['#theme'] = 'token_var_token_variables_form';

  return $form;
}

/**
 * Clear cache on  token_var_settings_form submit.
 *
 * @param array $form_state
 *
 */
function token_var_settings_form_submit($form_state) {
  token_clear_cache();
}

/**
 * Add checkboxes to token_var_settings_form form.
 *
 * @param array $form
 *
 */
function _token_var_get_variables(&$form) {
  global $conf;
  $options = array();

  $header = array(
    'name' => t("Variable Name"),
    'value' => t("Value"),
  );

  foreach ($conf as $key => $value) {
    if (!is_array($value) && !is_object($value)) {
      $options[$key] = array(
        'name' => $key,
        'value' => $key,
      );
    }
    elseif (is_array($value) && ($key != TOKENIZE_DRUPAL_VARIABLES_OPTIONS)) {
      foreach ($value as $var_key => $var_value) {
        if (!is_array($var_value) && !is_object($var_value)) {
          $options[$key . "|" . $var_key] = array(
            'name' => $key . "|" . $var_key,
            'value' => $var_key,
          );
        }
      }
    }
  }

  $form[TOKENIZE_DRUPAL_VARIABLES_OPTIONS] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#default_value' => variable_get(TOKENIZE_DRUPAL_VARIABLES_OPTIONS, array()),
  );
}

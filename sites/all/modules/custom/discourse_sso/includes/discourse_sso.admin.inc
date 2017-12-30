<?php

/**
 * @file
 * Discource SSO administration functionality.
 */

/**
 * Discourse SSO administration page form/content.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the values of the form.
 */
function discourse_sso_admin_form(array $form, array &$form_state) {
  $form[DISCOURSE_SSO_URL] = array(
    '#type' => 'textfield',
    '#title' => t('Discourse site URL'),
    '#description' => t('URL of discourse site.'),
    '#default_value' => discourse_sso_get_url(),
    '#size' => 60,
    '#required' => TRUE,
  );
  $form[DISCOURSE_SSO_SECRET] = array(
    '#type' => 'textfield',
    '#title' => t('SSO secret'),
    '#description' => t('Secret string used to encrypt/decrypt SSO information, be sure it is 10 chars or longer.'),
    '#size' => 60,
    '#default_value' => discourse_sso_get_secret(),
  );

  // Register custom submit callback before the system submit callback is
  // registered.
  $form['#submit'][] = 'discourse_sso_admin_form_submit';

  return system_settings_form($form);
}

/**
 * Admin form submit callback.
 *
 * In case of wmpty sso secret field set its value by already existing value.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   An associative array containing the values of the form.
 */
function discourse_sso_admin_form_submit(array $form, array &$form_state) {
  if (empty($form_state['values'][DISCOURSE_SSO_SECRET])) {
    $form_state['values'][DISCOURSE_SSO_SECRET] = discourse_sso_get_secret();
  }
}

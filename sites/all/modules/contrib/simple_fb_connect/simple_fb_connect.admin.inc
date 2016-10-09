<?php

/**
 * @file
 * Administration page callbacks for the simple fb connect module.
 */
function simple_fb_connect_api_keys_settings($form, &$form_state) {
  // Check that Facebook PHP SDK is properly installed and that the version is 4.0.x
  $sdk = libraries_detect('facebook-php-sdk-v4');
  if (!is_array($sdk) || !$sdk['installed'] || $sdk['version'] < '4.0' || $sdk['version'] >= '4.1') {
    drupal_set_message(t('Facebook PHP SDK v4 not properly installed. Check README.txt and <a href="@url">Drupal status report</a> for further details!', array('@url' => url('admin/reports/status'))), 'error');
  }

  // show instructions to README.txt and Drupal account settings
  drupal_set_message(t('Installation and configuration instructions can be found from the README.txt'));
  drupal_set_message(t('Also check <a href="@url">Drupal account settings</a>', array('@url' => url('admin/config/people/accounts'))));

  $form['fb_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook App settings'),
    '#description' => t('You need to first create a Facebook App at <a href="https://developers.facebook.com/apps">https://developers.facebook.com/apps</a>')
  );

  $form['fb_settings']['simple_fb_connect_appid'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('App ID'),
    '#default_value' => variable_get('simple_fb_connect_appid', NULL),
    '#description' => t('Copy the App ID of your Facebook App here'),
  );

  $form['fb_settings']['simple_fb_connect_skey'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('App Secret'),
    '#default_value' => variable_get('simple_fb_connect_skey', NULL),
    '#description' => t('Copy the App Secret of your Facebook App here'),
  );

  $form['fb_settings']['simple_fb_connect_connect_url'] = array(
    '#type' => 'textfield',
    '#disabled' => TRUE,
    '#title' => t('Connect url'),
    '#description' => t('Copy this value to <em>Site URL</em> and <em>Mobile Site URL</em> of your Facebook App settings.'),
    '#default_value' => $GLOBALS['base_url'],
  );

  $form['module_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simple FB Connect configurations'),
    '#description' => t('These settings allow you to configure how Simple FB Connect module behaves on your Drupal site')
  );

  $form['module_settings']['simple_fb_connect_post_login_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Post login path'),
    '#description' => t('Drupal path (without trailing slash) where the user should be redirected after successful login. Use <em>&lt;front&gt;</em> to redirect user to your front page. You can override this with ?destination URL parameter. Refer to README.txt for more information.'),
    '#default_value' => variable_get('simple_fb_connect_post_login_url', 'user'),
  );

  $form['module_settings']['simple_fb_connect_redirect_user_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect new users to Drupal user form'),
    '#description' => t('If you check this, new users are redirected to Drupal user form after the user is created. This is useful if you want to encourage users to fill in additional user fields.'),
    '#default_value' => variable_get('simple_fb_connect_redirect_user_form', 0),
  );

  $form['module_settings']['simple_fb_connect_login_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Login Only (No Registration)'),
    '#description' => t('Allow only existing users to login with FB. New users can not register using FB login.'),
    '#default_value' => variable_get('simple_fb_connect_login_only', 0),
  );

  $form['module_settings']['simple_fb_connect_disable_admin_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable FB login for administrator'),
    '#description' => t('Disabling FB login for administrator (<em>user 1</em>) can help protect your site if a security vulnerability is ever discovered in Facebook PHP SDK or this module.'),
    '#default_value' => variable_get('simple_fb_connect_disable_admin_login', 1),
  );

  // Option to disable FB login for specific roles.
  // - roles 1 & 2 (anonymous & authenticated users) are not sensible options
  // - D7 has a default role 'administrator'. Block this role by default if
  //   simple_fb_connect_disabled_roles has not been set.
  // - we save this setting by role name and not id so that the configuration
  //   is transferable with Features: https://www.drupal.org/node/2653060
  $roles = user_roles();
  $options = array();
  $defaults = variable_get('simple_fb_connect_disabled_roles', array());
  foreach ($roles as $rid => $label) {
    if ($rid > 2) {
      $options[$label] = check_plain($label);
    }
    if (empty($defaults) && $label == 'administrator') {
      $defaults[$label] = $label;
    }
  }
  $form['module_settings']['simple_fb_connect_disabled_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Disable FB login for the following roles'),
    '#options' => $options,
    '#default_value' => $defaults,
  );
  if (empty($options)) {
    $form['module_settings']['simple_fb_connect_disabled_roles']['#description'] = t('No roles found.');
  }

  return system_settings_form($form);
}

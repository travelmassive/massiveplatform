<?php
/**
* Implements hook_form_FORM_ID_alter().
*/
function tm_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name
 	$form['site_information']['site_name']['#default_value'] = 'Travel Massive';
 	
 	// Set default country to US
 	$form['server_settings']['site_default_country']['#default_value'] = 'US';
  
 	// Set default time-zone to UTC. Source: https://drupal.org/comment/6897960#comment-6897960
  $form['server_settings']['date_default_timezone']['#default_value'] = 'UTC';
  unset($form['server_settings']['date_default_timezone']['#attributes']);
}
?>
<?php
/**
 * @file
 * tm_base.admin.inc
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */

/**
 * Implements hook_form_FORM_ID_alter()
 */
function tm_base_form_system_site_information_settings_alter(&$form, &$form_state) {
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

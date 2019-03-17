<?php

/**
 * @file
 * Hooks for the shs module.
 *
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard Drupal
 * manner.
 */

/**
 * Alter information provided by shs_js() for the js module.
 *
 * @param array $settings
 *   Settings for js module.
 *
 * @see hook_js()
 */
function hook_shs_js_info_alter(array &$settings) {
  // Add pathauto as dependency for the default json callback.
  $settings['json']['dependencies'][] = 'pathauto';
  // Change the access callback.
  $settings['json']['access callback'] = 'my_custom_access';
}

/**
 * Alter permissions to create new terms using shs on a per-term level.
 *
 * @param int $vid
 *   ID of vocabulary to create the term in.
 * @param int $parent
 *   ID of parent term (0 for top level).
 * @param string $field
 *   Name of field requesting the term list (DOM element name).
 * @param object $account
 *   The user to check access for.
 *
 * @return bool
 *   <code>FALSE</code> if no new terms should be created below the given
 *   parent, otherwise <code>TRUE</code>.
 */
function hook_shs_add_term_access($vid, $parent, $field, $account) {
  // Deny creating new terms on tid 0 and 3.
  if ($parent == 0 || $parent == 3) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Alter the list of JSON callbacks supported by shs.
 *
 * JSON callback definitions are used to validate incoming data and function
 * requests (i.e. adding a new term).
 *
 * @param array $callbacks
 *   List of callback definitions.
 */
function hook_shs_json_callbacks_alter(array &$callbacks) {
  // Use custom callback for adding new terms.
  $callbacks['shs_json_term_add']['callback'] = 'my_custom_shs_json_term_add';
}

/**
 * Alter the list of terms within a level of the term hierarchy.
 *
 * @param array $terms
 *   List of terms displayed to the user (single hierarchy level).
 * @param array $alter_options
 *   - vid: ID of vocabulary or field name
 *   - parent: ID of parent term
 *   - settings: Additional settings (for example "language", etc.,)
 */
function hook_shs_term_get_children_alter(array &$terms, array &$alter_options) {

}

/**
 * Alter Javascript settings of shs widgets in entity forms and views.
 *
 * @param array $settings_js
 *   Javascript settings for shs widgets.
 * @param string $field_name
 *   Name of field the provided settings are used for.
 * @param int|string $vocabulary_identifier
 *   ID or machine_name of vocabulary the settings are used for.
 */
function hook_shs_js_settings_alter(array &$settings_js, $field_name, $vocabulary_identifier) {
  if ($field_name == 'field_article_terms') {
    foreach ($settings_js['shs'] as $field => $container) {
      foreach ($container as $identifier => $settings) {
        $settings_js['shs'][$field][$identifier]['any_label'] = t('- Select an item -');
      }
    }
  }
}

/**
 * Alter Javascript settings for a single shs widget.
 *
 * @param array $settings_js
 *   Javascript settings for the specified field.
 * @param string $field_name
 *   Name of field the provided settings are used for.
 * @param int|string $vocabulary_identifier
 *   ID or machine_name of vocabulary the settings are used for.
 */
function hook_shs_FIELDNAME_js_settings_alter(array &$settings_js, $field_name, $vocabulary_identifier) {
  foreach ($settings_js['shs'] as $field => &$container) {
    foreach ($container as $identifier => &$settings) {
      // Define labels for each level.
      $settings['labels'] = array(
        // No label for first level.
        FALSE,
        t('Country'),
        t('City'),
      );
      // Small speed-up for anmiations (defaults to 400ms).
      $settings['display']['animationSpeed'] = 100;
    }
  }
}

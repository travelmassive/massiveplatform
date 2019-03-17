<?php

/**
 * @file
 * Definition of shs_handler_filter_numeric.
 */

/**
 * Extends simple numeric filter with hierarchical selection.
 *
 * @ingroup views_filter_handlers
 */
class shs_handler_filter_entityreference extends views_handler_filter_numeric {

  /**
   * Define filter options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['shs_enabled'] = array('default' => FALSE);
    $options['use_chosen'] = array('default' => 'never');

    return $options;
  }

  /**
   * Build the options form.
   *
   * @param array $form
   *   Current form structure.
   * @param array $form_state
   *   Current form state.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['shs_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Output filter as hierarchical select'),
      '#default_value' => $this->options['shs_enabled'],
    );

    if (module_exists('chosen')) {
      $form['use_chosen'] = array(
        '#type' => 'select',
        '#title' => t('Output this field with !chosen', array('!chosen' => l(t('Chosen'), 'http://drupal.org/project/chosen'))),
        '#description' => t('Select in which cases the element will use the !chosen module for the term selection of each level.', array('!chosen' => l(t('Chosen'), 'http://drupal.org/project/chosen'))),
        '#default_value' => empty($this->options['use_chosen']) ? 'never' : $this->options['use_chosen'],
        '#dependency' => array(
          'edit-options-shs-enabled' => array(1),
        ),
        '#options' => array(
          'chosen' => t('let chosen decide'),
          'always' => t('always'),
          'never' => t('never'),
        ),
      );
    }
  }

  /**
   * Provide a simple textfield for equality.
   */
  public function value_form(&$form, &$form_state) {
    global $language;

    parent::value_form($form, $form_state);
    if (empty($form_state['exposed']) || empty($this->options['shs_enabled'])) {
      return;
    }

    $identifier = $this->options['expose']['identifier'];
    $multiple = $this->options['expose']['multiple'];

    $field = field_info_field($this->definition['field_name']);
    $handler = entityreference_get_selection_handler($field);
    $referencable_entities = $handler->getReferencableEntities();
    $vocabulary_names = array();
    if ('views' === $field['settings']['handler']) {
      $view_settings = $field['settings']['handler_settings']['view'];
      // Try to load vocabularies from view filter.
      $vocabulary_names = _shs_entityreference_views_get_vocabularies($view_settings['view_name'], $view_settings['display_name']);
    }
    else {
      $vocabulary_names = empty($field['settings']['handler_settings']['target_bundles']) ? array() : $field['settings']['handler_settings']['target_bundles'];
    }
    $vocabularies = array();
    foreach ($vocabulary_names as $vocabulary_name) {
      if (($vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name)) !== FALSE) {
        $vocabularies[] = $vocabulary;
      }
    }

    if (empty($form_state['input'][$identifier])) {
      $form_state['input'][$identifier] = 'All';
    }
    $default_value = check_plain($form_state['input'][$identifier]);
    if (!is_array($default_value) && $multiple) {
      $default_value = explode('+', $default_value);
      foreach ($default_value as $delta => $value) {
        $default_value[$delta] = explode(',', $value);
      }
    }

    $parents = array();
    if (!empty($default_value) && $default_value != 'All') {
      if (is_array($default_value) && $default_value[0] != 'All') {
        foreach ($default_value as $delta => $level) {
          $parents[$delta]['tid'] = $level;
        }
      }
      elseif (is_string($default_value)) {
        $term_parents = taxonomy_get_parents_all($default_value);
        // Remove selected term from list.
        array_shift($term_parents);
        foreach (array_reverse($term_parents) as $term) {
          $parents[] = array('tid' => $term->tid);
        }
        // Add current term (name not needed).
        $parents[] = array('tid' => $default_value);
      }
    }

    // Add fake item for next level.
    $parents[] = array('tid' => 0);

    $element_settings = array(
      'create_new_terms' => FALSE,
      'create_new_levels' => FALSE,
      'required' => FALSE,
      'language' => $language,
    );
    if (module_exists('chosen') && !empty($this->options['use_chosen'])) {
      $element_settings['use_chosen'] = $this->options['use_chosen'];
    }
    if (!empty($this->options['exposed']) && !empty($this->options['expose']['required'])) {
      $element_settings['required'] = TRUE;
    }
    // Allowing other modules to add settings for shs.
    if (!empty($this->options['expose']['shs'])) {
      $element_settings = array_merge($element_settings, $this->options['expose']['shs']);
    }

    $vocabulary_identifier = array(
      'field_name' => $field['field_name'],
    );
    if (count($vocabularies) == 1) {
      // Get ID from first (and only) vocabulary.
      $vocabulary_identifier = $vocabularies[0]->vid;
    }

    // Generate a random hash to avoid merging of settings by drupal_add_js.
    // This is necessary until http://drupal.org/node/208611 lands for D7.
    $js_hash = _shs_create_hash();
    // Create settings needed for our js magic.
    $settings_js = array(
      'shs' => array(
        $identifier => array(
          $js_hash => array(
            'vid' => $vocabulary_identifier,
            'settings' => $element_settings,
            'default_value' => $default_value,
            'parents' => $parents,
            'multiple' => $multiple,
            'any_label' => variable_get('views_exposed_filter_any_label', 'new_any') == 'old_any' ? t('<Any>') : t('- Any -'),
            'any_value' => 'All',
          ),
        ),
      ),
    );
    // Add settings.
    drupal_add_js($settings_js, 'setting');
    // Add behavior.
    drupal_add_js(drupal_get_path('module', 'shs') . '/js/shs.js');

    if (is_array($default_value) && $multiple && $default_value[0] != 'All') {
      foreach ($default_value as $delta => $level) {
        if ($delta == 0) {
          $default_value_str = implode(',', $level);
        }
        else {
          $default_value_str .= '+' . implode(',', $level);
        }
      }
    }

    $options = array();
    foreach ($referencable_entities as $terms) {
      $options += $terms;
    }
    // Change type of value element.
    $form['value']['#type'] = 'select';
    $form['value']['#options'] = $options;
    $form['value']['#size'] = 1;
    $form['value']['#default_value'] = empty($default_value) ? 'All' : $default_value;
    $form['value']['#attributes'] = array(
      'class' => array('element-invisible', 'shs-enabled'),
    );

    if (!empty($form_state['exposed']) && isset($identifier) && empty($form_state['input'][$identifier]) || $multiple && isset($form_state['input'][$identifier][0])) {
      $form_state['input'][$identifier] = empty($default_value_str) ? 'All' : $default_value_str;
    }
  }

  /**
   * Function to build the admin summary.
   *
   * @return string
   *   Short summary explaining the settings.
   */
  public function admin_summary() {
    $summary = parent::admin_summary();

    // @todo: add "shs".
    return $summary;
  }

}

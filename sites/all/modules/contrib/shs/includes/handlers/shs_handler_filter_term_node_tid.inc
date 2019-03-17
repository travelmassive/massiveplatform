<?php

/**
 * @file
 * Definition of shs_handler_filter_term_node_tid.
 */

/**
 * Filter by term id (including selection by simple hierarchical select).
 *
 * @ingroup views_filter_handlers
 */
class shs_handler_filter_term_node_tid extends views_handler_filter_term_node_tid {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'shs');
    $options['use_chosen'] = array('default' => 'never');

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $form['type']['#options'] += array(
      'shs' => t('Simple hierarchical select'),
    );

    if (module_exists('chosen')) {
      $form['use_chosen'] = array(
        '#type' => 'select',
        '#title' => t('Output this field with !chosen', array('!chosen' => l(t('Chosen'), 'http://drupal.org/project/chosen'))),
        '#description' => t('Select in which cases the element will use the !chosen module for the term selection of each level.', array('!chosen' => l(t('Chosen'), 'http://drupal.org/project/chosen'))),
        '#default_value' => empty($this->options['use_chosen']) ? 'never' : $this->options['use_chosen'],
        '#dependency' => array('radio:options[type]' => array('shs')),
        '#options' => array(
          'chosen' => t('let chosen decide'),
          'always' => t('always'),
          'never' => t('never'),
        ),
      );
    }
  }

  /**
   * {@inheritdoc}
   */
  public function value_form(&$form, &$form_state) {
    global $language;

    parent::value_form($form, $form_state);

    $multiple = $this->options['expose']['multiple'];
    $vocabulary = taxonomy_vocabulary_machine_name_load($this->options['vocabulary']);
    if (empty($vocabulary) && $this->options['limit']) {
      $form['markup'] = array(
        '#markup' => '<div class="form-item">' . t('An invalid vocabulary is selected. Please change it in the options.') . '</div>',
      );
      return;
    }

    if ($this->options['type'] == 'shs' && $this->options['exposed']) {
      $identifier = $this->options['expose']['identifier'];

      // Get parents of selected term.
      $default_value = empty($this->value) ? 0 : $this->value;

      if (!empty($form_state['input'][$identifier])) {
        $default_value = check_plain($form_state['input'][$identifier]);
        if ($multiple && !is_array($default_value)) {
          $default_value = array($default_value);
        }
      }

      $parents = array();
      if (!empty($default_value) && $default_value != 'All') {
        if (is_array($default_value) && $default_value[0] != 'All') {
          if (strpos($default_value[0], ',') !== FALSE || strpos($default_value[0], '+') !== FALSE) {
            $values = array();
            foreach ($default_value as $default) {
              $values += preg_split('/[,\+]+/', $default, -1, PREG_SPLIT_NO_EMPTY);
            }
            $form_state['input'][$identifier] = empty($values) ? 'All' : $values;
            $default_value = $values;
          }
          $parents_multiple = array();
          foreach ($default_value as $tid) {
            $parents_multiple[] = $tid;
          }
          $parents[] = array('tid' => $parents_multiple);
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

      // Generate a random hash to avoid merging of settings by drupal_add_js.
      // This is necessary until http://drupal.org/node/208611 lands for D7.
      $js_hash = _shs_create_hash();
      // Create settings needed for our js magic.
      $settings_js = array(
        'shs' => array(
          $identifier . ($multiple ? '[]' : '') => array(
            $js_hash => array(
              'vid' => $vocabulary->vid,
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
      drupal_alter(array('shs_js_settings', "shs_{$identifier}_js_settings"), $settings_js, $identifier, $vocabulary->vid);
      // Add settings.
      drupal_add_js($settings_js, 'setting');
      // Add behavior.
      drupal_add_js(drupal_get_path('module', 'shs') . '/js/shs.js');

      // Rewrite default select element.
      $form['value'] += array(
        '#attributes' => array(
          'class' => array('element-invisible', 'shs-enabled'),
        ),
      );
    }

    if (empty($form_state['exposed'])) {
      // Retain the helper option.
      $this->helper->options_form($form, $form_state);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function admin_summary() {
    // Set up $this->value_options for the parent summary.
    $this->value_options = array();

    if ($this->value == 'All') {
      $this->value = NULL;
    }

    return parent::admin_summary();
  }

}

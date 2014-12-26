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

  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'shs');
    $options['use_chosen'] = array('default' => 'never');

    return $options;
  }

  function extra_options_form(&$form, &$form_state) {
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

  function value_form(&$form, &$form_state) {
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
        $default_value = $form_state['input'][$identifier];
        if (!is_array($default_value) && $multiple) {
          $default_value = explode('+', $default_value);
          foreach ($default_value as $delta => $value) {
            $default_value[$delta] = explode(',', $value);
          }
        }
      }

      $parents = array();
      if (!empty($default_value) && $default_value != 'All') {
        if (is_array($default_value) && $default_value[0] != 'All') {
          foreach ($default_value as $delta => $level) {
            foreach ($level as $tid) {
              $parents[$delta]['tid'] = $level;
            }
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
      );
      if (module_exists('chosen') && !empty($this->options['use_chosen'])) {
        $element_settings['use_chosen'] = $this->options['use_chosen'];
      }
      if (!empty($this->options['exposed']) && !empty($this->options['expose']['required'])) {
        $element_settings['required'] = TRUE;
      }

      // Generate a random hash to avoid merging of settings by drupal_add_js.
      // This is necessary until http://drupal.org/node/208611 lands for D7.
      $js_hash = _shs_create_hash();
      // Create settings needed for our js magic.
      $settings_js = array(
        'shs' => array(
          $identifier => array(
            $js_hash => array(
              'vid' => $vocabulary->vid,
              'settings' => $element_settings,
              'default_value' => $default_value,
              'parents' => $parents,
              'multiple' => $multiple,
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

      // Rewrite default select to textfield.
      $form['value'] = array(
        '#type' => 'textfield',
        '#default_value' => empty($default_value_str) ? 'All' : $default_value_str,
        '#attributes' => array(
          'class' => array('element-invisible', 'shs-enabled'),
        ),
      );

      if (!empty($form_state['exposed']) && isset($identifier) && empty($form_state['input'][$identifier]) || $multiple && isset($form_state['input'][$identifier][0])) {
        $form_state['input'][$identifier] = empty($default_value_str) ? 'All' : $default_value_str;
      }
    }

    if (empty($form_state['exposed'])) {
      // Retain the helper option.
      $this->helper->options_form($form, $form_state);
    }
  }

  function admin_summary() {
    // Set up $this->value_options for the parent summary.
    $this->value_options = array();

    if ($this->value == 'All') {
      $this->value = NULL;
    }

    return parent::admin_summary();
  }

}

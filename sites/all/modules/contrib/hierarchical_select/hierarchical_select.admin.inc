<?php


/**
 * @file
 * Module settings and configuration administration UI.
 */


/**
 * Form definition; admin settings.
 */
function hierarchical_select_admin_settings($form, &$form_state) {
  $form['description'] = array(
    '#markup' => t('All settings below will be used as site-wide defaults.'),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );
  $form['hierarchical_select_animation_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation delay'),
    '#description' => t(
      'The delay that will be used for the "drop in/out" effect when a
      hierarchical select is being updated (in milliseconds).'
    ),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => variable_get('hierarchical_select_animation_delay', 400),
  );
  $form['hierarchical_select_level_labels_style'] = array(
    '#type' => 'select',
    '#title' => t('Level labels style'),
    '#description' => t(
      'The style that will be used for level labels. This is not supported by
      all browsers! If you want a consistent interface, choose to use no
      style.'
    ),
    '#options' => array(
      'none' => t('No style'),
      'bold' => t('Bold'),
      'inversed' => t('Inversed'),
      'underlined' => t('Underlined'),
    ),
    '#default_value' => variable_get('hierarchical_select_level_labels_style', 'none'),
  );
  // TODO: port the HS client-side cache system to Drupal 7.
  /*
  $form['hierarchical_select_js_cache_system'] = array(
    '#type' => 'radios',
    '#title' => t('Cache in a HTML 5 client-side database'),
    '#description' => t(
      'This feature only works in browsers that support the
      <a href="!spec-url">HTML 5 client-side database storage specification
      </a>.</br>
      After enabling this, you will notice (in supporting browsers) that
      refreshing the hierarchical select will not require a request to the
      server when a part is being requested that has been requested before.',
      array('!spec-url' => url('http://www.whatwg.org/specs/web-apps/current-work/multipage/section-sql.html'))
    ),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('hierarchical_select_js_cache_system', 0),
  );
  */

  return system_settings_form($form);
}

/**
 * Menu callback; a table that lists all Hierarchical Select configs.
 */
function hierarchical_select_admin_configs() {
  $header = array(t('Hierarchy type'), t('Hierarchy'), t('Entity type'), t('Bundle'), t('Context type'), t('Context'), t('Actions'));

  // Retrieve all information items
  $info_items = array();
  foreach (module_implements('hierarchical_select_config_info') as $module) {
    $info_items = array_merge_recursive($info_items, module_invoke($module, 'hierarchical_select_config_info'));
  }

  // Process the retrieved information into rows.
  $rows = array();
  foreach ($info_items as $id => $item) {
    $config_id = $item['config_id'];

    $rows[$id] = array(
      $item['hierarchy type'],
      $item['hierarchy'],
      $item['entity type'],
      $item['bundle'],
      $item['context type'],
      $item['context'],
      theme('links', array('links' => array(
          array(
            'title' => t('Edit'),
            'href' => $item['edit link'],
            'fragment' => "hierarchical-select-config-form-$config_id",
          ),
          array(
            'title' => t('Export'),
            'href' => "admin/config/content/hierarchical_select/export/$config_id",
          ),
          array(
            'title' => t('Import'),
            'href' => "admin/config/content/hierarchical_select/import/$config_id",
          ),
        ))),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array(), 'caption' => t('Overview of all Hierarchical Select configurations.')));
}

/**
 * Menu callback; a table that lists all Hierarchical Select implementations
 * and the features they support.
 */
function hierarchical_select_admin_implementations() {
  $output = '';
  $header = array(t('Implementation (module)'), t('Hierarchy type'), t('Entity type'), t('Create new items'), t('Entity count'));

  // Retrieve all information items
  $rows = array();
  foreach (module_implements('hierarchical_select_root_level') as $module) {
    $filename = db_query("SELECT filename FROM {system} WHERE type = :type AND name = :name", array(':type' => 'module', ':name' => $module))->fetchField();
    $module_info = drupal_parse_info_file(dirname($filename) . "/$module.info");
    // Try to extract the hierarchy type from the optional hook_hierarchical_select_config_info().
    $hierarchy_type = $entity_type = t('unknown');
    if (module_hook($module, 'hierarchical_select_implementation_info')) {
      $implementation = module_invoke($module, 'hierarchical_select_implementation_info');
      $hierarchy_type = $implementation['hierarchy type'];
      $entity_type    = $implementation['entity type'];
    }

    $rows[] = array(
      $module_info['name'],
      $hierarchy_type,
      $entity_type,
      (module_hook($module, 'hierarchical_select_create_item')) ? t('Yes') : t('No'),
      (module_hook($module, 'hierarchical_select_entity_count'))  ? t('Yes') : t('No'),
    );
  }

  $output .= '<p>';
  $output .= t('
    The table below allows you to find out <strong>which Hierarchical Select
    features are supported</strong> by the implementations of the Hierarchical
    Select API.<br />
    It is <strong>not a reflection of some settings</strong>.
  ');
  $output .= '</p>';

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array(), 'caption' => t('Overview of all installed Hierarchical Select implementations.')));

  return $output;
}

/**
 * Form definition; config export form.
 */
function hierarchical_select_admin_export($form, &$form_state, $config_id) {
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'hierarchical_select') . '/includes/common.inc';

  $config = hierarchical_select_common_config_get($config_id);
  $code = _hierarchical_select_create_export_code($config);

  drupal_add_css(drupal_get_path('module', 'hierarchical_select') . '/hierarchical_select.css');
  drupal_add_js('$(document).ready(function() { $(".hierarchical-select-code").focus(); });', array('type' => 'inline', 'scope' => JS_DEFAULT));

  $lines = substr_count($code, "\n") + 1;
  $form['config'] = array(
    '#type' => 'textarea',
    '#title' => t('Hierarchical Select configuration %config_id', array('%config_id' => $config_id)),
    '#default_value' => $code,
    '#rows' => $lines,
    '#attributes' => array('class' => array('hierarchical-select-config-code')),
  );

  return $form;
}

/**
 * Form definition; config import form.
 */
function hierarchical_select_admin_import($form, &$form_state, $config_id) {
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'hierarchical_select') . '/includes/common.inc';

  drupal_add_css(drupal_get_path('module', 'hierarchical_select') . '/hierarchical_select.css');
  drupal_add_js('$(document).ready(function() { $(".hierarchical-select-code").focus(); });', array('type' => 'inline', 'scope' => JS_DEFAULT));

  $form['config'] = array(
    '#type' => 'textarea',
    '#title' => t('Import Hierarchical Select configuration code'),
    '#cols' => 60,
    '#rows' => 15,
    '#description' => t('Copy and paste the results of an exported
      Hierarchical Select configuration here.<br />This will override the
      current Hierarchical Select configuration for %config_id.',
      array('%config_id' => $config_id)
    ),
    '#attributes' => array('class' => array('hierarchical-select-config-code')),
  );
  $form['interpreted_config'] = array('#type' => 'value', '#value' => NULL);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Import"),
  );
  $form_state['#redirect'] = NULL;
  return $form;
}

/**
 * Validate callback; config import form.
 */
function hierarchical_select_admin_import_validate($form, &$form_state) {
  ob_start();
  eval($form_state['values']['config']);
  ob_end_clean();

  form_set_value($form['interpreted_config'], serialize($config), $form_state);

  if (empty($form_state['values']['config'])) {
    form_error($form['config'], t('You did not enter anything.'));
  }
  elseif ($config == NULL) {
    form_error($form['config'], t('There is a syntax error in the Hierarchical Select configuration you entered.'));
  }
  elseif (!isset($config['config_id']) || empty($config['config_id'])) {
    form_error($form['config'], t('Unable to import this configuration, because no Hierarchical Select <em>config id</em> is set.'));
  }
}

/**
 * Submit callback; config import form.
 */
function hierarchical_select_admin_import_submit($form, &$form_state) {
  $config = unserialize($form_state['values']['interpreted_config']);
  $config_id = $config['config_id'];
  hierarchical_select_common_config_set($config_id, $config);
  drupal_set_message(t('Hierarchical Select configuration for %config_id imported!', array('%config_id' => $config_id)));
}


//----------------------------------------------------------------------------
// Private functions.

/**
 * Given a config array, create the export code for it.
 *
 * @param array $config
 *   A Hierarchical Select config array, as described in API.txt
 * @return string
 *   The code as it would appear in an editor.
 */
function _hierarchical_select_create_export_code($config) {
  $output = _hierarchical_select_create_code_from_array($config);
  $output = '$config = ' . $output . ";\n";
  return $output;
}

/**
 * Given a array, create the export code for it.
 *
 * This functions is a refactoring of features_var_export() to use with the
 * hierarchical select module.
 *
 * @param mixed $config
 *   A value to export as code.
 * @param string $prefix
 *   Padding for nested array.
 * @param boolean $init
 *   Indicator of the first level of the export.
 * @return string
 *   The code as it would appear in an editor.
 */
function _hierarchical_select_create_code_from_array($var, $prefix = '', $init = TRUE) {
  $output = "";
  $type = gettype($var);
  switch ($type) {
    case 'array':
      if (empty($var)) {
        $output = "array()";
      }
      else {
        $output = "array(\n";
        foreach ($var as $key => $value) {
          $value = _hierarchical_select_create_code_from_array($value, '  ', FALSE);
          $output .= "  '$key' => " . $value . ",\n";
        }
        $output .= ')';
      }
      break;
    case 'string':
      $var = str_replace("\n", "***BREAK***", $var);
      $output = var_export($var, TRUE);
      break;
    case 'boolean':
      $var = empty($var) ? 'FALSE' : 'TRUE';
      $output = var_export($var, TRUE);
      break;
    default:
      $output = var_export($var, TRUE);
  }
  if ($prefix) {
    $output = str_replace("\n", "\n$prefix", $output);
  }
  if ($init) {
    $output = str_replace("***BREAK***", "\n", $output);
  }
  return $output;
}

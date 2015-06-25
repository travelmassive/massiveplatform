<?php

// Run: drush -d scr --uri=massiveplatform.com massiveplatform_setup_fies/rename_questions_fields.php

// From: https://www.drupal.org/node/1130386#comment-7055614

/**
 * Rename an array of fields.
 *
 * Adapted from code in forum_update_7003(). See also
 * zgadzaj.com/how-to-change-the-machine-name-of-a-content-field-in-
 * drupal-7.
 *
 * @param $fields
 *   Array of field names, with old names as keys and new names as values.
 * @param $drop_first
 *   Boolean: whether to drop any existing tables for the renamed fields. If
 *   you have created the renamed field already - for example, by renaming
 *   a field that was exported to Features and then reverting the feature -
 *   you may wish to set this argument to TRUE so that your data will be
 *   copied.
 */
function tm_site_rename_fields($fields, $drop_first = FALSE) {
  foreach ($fields as $old_field_name => $new_field_name) {
    // Read field data.
    $old_field = field_read_field($old_field_name);

    // Update {field_config}.
    db_update('field_config')->fields(array('field_name' => $new_field_name))->condition('id', $old_field['id'])->execute();

    // Update {field_config_instance}.
    db_update('field_config_instance')->fields(array('field_name' => $new_field_name))->condition('field_id', $old_field['id'])->execute();

    // The tables that need updating in the form 'old_name' => 'new_name'.
    $tables = array(
      'field_data_' . $old_field_name => 'field_data_' . $new_field_name,
      'field_revision_' . $old_field_name => 'field_revision_' . $new_field_name,
    );
    // Iterate through tables to be redefined and renamed.
    foreach ($tables as $old_table => $new_table) {

      // Iterate through the field's columns. For example, a 'text' field will
      // have columns 'value' and 'format'.
      foreach ($old_field['columns'] as $column_name => $column_definition) {
        // Column names are in the format {field_name}_{column_name}.
        $old_column_name = $old_field_name . '_' . $column_name;
        $new_column_name = $new_field_name . '_' . $column_name;

        // If there is an index for the field, drop and then re-add it.
        $has_index = isset($old_field['indexes'][$column_name]) && ($old_field['indexes'][$column_name] == array($column_name));
        if ($has_index) {
          db_drop_index($old_table, $old_column_name);
        }

        // Rename the column.
        db_change_field($old_table, $old_column_name, $new_column_name, $column_definition);
        if ($has_index) {
          db_drop_index($old_table, $new_column_name);
          db_add_index($old_table, $new_column_name, array($new_column_name));
        }

        drush_print('Renamed field ' . $old_field_name . ' to ' . $new_field_name . '.');
      }

      // The new table may exist e.g. due to having been included in a feature
      // that was reverted prior to this update being run. If so, we need to
      // drop the new table so that the old one can be renamed.
      if ($drop_first && db_table_exists($new_table)) {
        db_drop_table($new_table);
      }

      // Rename the table.
      db_rename_table($old_table, $new_table);
    }
  }
}

$fields = array(
	'field_user_question_destination' => 'field_user_question_1',
	'field_user_question_dream_travel' => 'field_user_question_2',
	'field_user_question_job' => 'field_user_question_3',
	'field_user_question_learn' => 'field_user_question_4',
	'field_user_question_why_travel' => 'field_user_question_5'
    // 'field_my_old_field_name' => 'field_my_new_field_name',
    // 'field_my_other_old_field_name' => 'field_my_other_new_field_name',
);

drush_print("renaming fields...");
tm_site_rename_fields($fields);


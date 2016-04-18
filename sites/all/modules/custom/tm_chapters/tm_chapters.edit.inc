<?php

// tm_chapters.edit.inc - edit methods

/** 
 * Alters a node on saving
 */
function tm_chapters_node_presave($node){
  $role_name = 'chapter leader';
  
  // make sure we are working on a chapter node
  if ($node->type != "chapter") { return; }

  // Check chapter leaders
  // If removing chapter leader, remove roles
  if ($role = user_role_load_by_name($role_name)) {
    foreach ($node->field_chapter_leaders[LANGUAGE_NONE] as $target){
      $user = user_load($target['target_id']);
      // make sure all chapter leaders have the right role set
      user_multiple_role_edit(array($user->uid), 'add_role', $role->rid);
    }
    // should also check $node->original for users being *removed*
    if (isset($node->original->field_chapter_leaders[LANGUAGE_NONE])){
      $original = flatten_chapter_leaders($node->original->field_chapter_leaders[LANGUAGE_NONE]);
      $new = flatten_chapter_leaders($node->field_chapter_leaders[LANGUAGE_NONE]);
      $removed = array_diff($original, $new);
      foreach ($removed as $rem){
        // check first that leader is not in other chapters
          $chapters = db_select('field_data_field_chapter_leaders', 'd')
          ->fields('d')
          ->condition('field_chapter_leaders_target_id',$rem,'=')
          ->condition('deleted',0,'=')
          ->execute();
        if ($chapters->rowCount() == 1){
          user_multiple_role_edit(array($rem), 'remove_role', $role->rid);
        }
      }
    }
  }

  // If sponsors are set, flag them
  // IC note:
  // Flag module (7.x-3.5) sets uid 0 for global flags.
  // So we use this patch from https://www.drupal.org/node/2087797 to set a uid
  // Patch file (flag_record_uid_2087797_28.patch) is in flags module directory
  if (isset($node->field_chapter_sponsors[LANGUAGE_NONE][0])) {
    $flag = flag_get_flag('tm_sponsor');
    foreach ($node->field_chapter_sponsors[LANGUAGE_NONE] as $arr) {
      $flag->flag("flag", $arr['target_id']);        
    }
  }

}

/**
 * Helper method for tm_chapters_node_presave
 * array diff didn't like 2 dimensional arrays
 */
function flatten_chapter_leaders($array){
  $newarray = array();
  foreach ($array as $item){
    $newarray[] = $item['target_id'];
  }
  return $newarray;
}

/**
 * Chapters form alter hook
 */
function tm_chapters_form_alter(&$form, &$form_state, $form_id) {
  //print_r($form_state);

  global $conf;
  global $user;

  if ($form_id != "chapter_node_form") {
    return;
  }

  // Prevent moderators to delete chapters that have members
  $chapter_id = $form['nid']['#value'];
  if (!_tm_chapters_check_moderator_delete($chapter_id)) {
    unset($form['actions']['delete']);
  }

  // Permission var
  $perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) or (in_array("administrator", $user->roles)));

  // Only allow moderators and administrators to edit chapter leaders
  // setting: tm_chapters_allow_edit_chapter_leaders
  if ((!$perm_is_moderator_or_admin)) {
    if (isset($conf["tm_chapters_allow_edit_chapter_leaders"])) {
      if (!$conf["tm_chapters_allow_edit_chapter_leaders"]) {
        unset($form['field_chapter_leaders']);
      }
    }
  }

  // Only allow moderators and administrators to edit group setting
  if ((!$perm_is_moderator_or_admin)) {
    
    // if value is set, show and disable the field
    if (isset($form['#entity']->field_chapter_is_group[LANGUAGE_NONE][0]['value'])) {
      $form['field_chapter_is_group']['#disabled'] = true;
    } else {
      // hide the field
      unset($form['field_chapter_is_group']);
    }
    
  }

  // Check tm_chapters_groups_enabled
  if ($conf["tm_chapters_groups_enabled"] != true) {
    unset($form['field_chapter_is_group']);
  }
    
}


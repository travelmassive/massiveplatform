<?php

// tm_chapters.edit.inc - edit methods

/** 
 * Alters a node on saving
 */
function tm_chapters_node_presave($node) {
    
  // Only operate on chapters
  if ($node->type != "chapter") { return; }

  // Handle chapter leader changes
  tm_chapters_leaders_chapter_save($node);
  $node->field_chapter_management_reason[LANGUAGE_NONE] = array(); // unset reason

  // Flag chapter sponsors
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

  // check for null array
  if ($array == null) { 
    return $newarray;
  }

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
  $perm_is_moderator_or_admin = (tm_users_is_community_manager($user->uid, $chapter_id) or (in_array("moderator", $user->roles)) or (in_array("administrator", $user->roles)));


  // Only allow moderators and administrators to edit chapter leaders
  // setting: tm_chapters_allow_edit_chapter_leaders
  if ((!$perm_is_moderator_or_admin)) {
    if (isset($conf["tm_chapters_allow_edit_chapter_leaders"])) {
      if (!$conf["tm_chapters_allow_edit_chapter_leaders"]) {
        unset($form['field_chapter_leaders']);
        unset($form['field_chapter_management_reason']);
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

  // Hide discussion category (deprecated)
  unset($form['field_discuss_category_id']);

  // hide cover video field if not enabled
  if (!$conf["tm_cover_videos_enabled"]) {
    unset($form['field_chapter_cover_video']);
  }

  // check if payments not enabled
  if (!module_exists("tm_payments")) {
    field_group_hide_field_groups($form, array('group_chapter_payments'));
    unset($form['field_chapter_payments_enabled']);
  }

  // only moderators can edit JSON data
  if (!(in_array("moderator", $user->roles)) or (in_array("administrator", $user->roles))) {
    field_group_hide_field_groups($form, array('group_chapter_api'));
    unset($form['field_chapter_api_public_json']);
  }
    
}


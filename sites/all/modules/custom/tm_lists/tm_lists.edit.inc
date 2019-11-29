<?php

// tm_lists.edit.inc

/** 
 * Alters a node on saving
 */
function tm_lists_node_presave($node) {
    
  // Only operate on lists
  if ($node->type != "list") { return; }

  // trim node title
  $node->title = trim($node->title);

  // list slug
  $list_slug = "";
  if (isset($node->field_list_slug_url[LANGUAGE_NONE][0]['value'])) {
  	 $list_slug = $node->field_list_slug_url[LANGUAGE_NONE][0]['value'];
  }
 
  // use node title is no slug
  if ($list_slug == "") {
  	$list_slug = $node->title;
  }

  $parts = explode("/", $list_slug);
  $list_slug = $parts[sizeof($parts) - 1];

  // list count
  $list_slug = str_replace('##', '__LIST_COUNT__', $list_slug);

  // remove url chars
  $list_slug = str_replace('#', "", $list_slug);
  $list_slug = str_replace('&', "and", $list_slug);
  $list_slug = str_replace('?', "", $list_slug);
  $list_slug = str_replace(':', "", $list_slug);
  $list_slug = str_replace('!', "", $list_slug);
  $list_slug = str_replace('__LIST_COUNT__', '##', $list_slug);

  // replace space
  $list_slug = trim($list_slug);
  $list_slug = str_replace(" ", "-", $list_slug);

  // remove --
  $list_slug = str_replace("----", "-", $list_slug);
  $list_slug = str_replace("---", "-", $list_slug);
  $list_slug = str_replace("--", "-", $list_slug);

  // clean up
  $list_slug = strtolower($list_slug);
  $list_slug = strip_tags($list_slug);

  // save list-slug
  $node->field_list_slug_url[LANGUAGE_NONE][0]['value'] = $list_slug;

  // list creators can always add and remove
  $node->field_list_roles_add[LANGUAGE_NONE][] = array('value' => 'list_creators');
  $node->field_list_roles_remove[LANGUAGE_NONE][] = array('value' => 'list_creators');

  // always show profile
  $node->field_list_display_links[LANGUAGE_NONE][] = array('value' => 'profile');

}

/**
 * Lists form alter hook
 */
function tm_lists_form_alter(&$form, &$form_state, $form_id) {

  global $conf;
  global $user;

  if ($form_id != "list_node_form") {
    return;
  }

  // placeholder text for message
  $form['field_list_message_added'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = t($conf["tm_lists_notification_message"]);

  // Permission var
  $perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) or (in_array("administrator", $user->roles)));

  // Only moderators can turn off nofollow
  if (!$perm_is_moderator_or_admin) {
    $form['field_list_nofollow_links']['#disabled'] = true;
  }
 
  // Disable with js
  $js = "
//  List Creators can always add to list
document.getElementById('edit-field-list-roles-add-und-list-creators').disabled = true;
true;
document.getElementById('edit-field-list-roles-add-und-list-creators').checked = true;
true;

// List Creators can always remove from list
document.getElementById('edit-field-list-roles-remove-und-list-creators').disabled = true;
true;
document.getElementById('edit-field-list-roles-remove-und-list-creators').checked = true;
true;

// Profile always displayed
document.getElementById('edit-field-list-display-links-und-profile').disabled = true;
true;
document.getElementById('edit-field-list-display-links-und-profile').checked = true;
true;
";
  drupal_add_js($js, array('type' => 'inline', 'scope' => 'footer'));

}


<?php

// tm_fields.menu_list.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_lists_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;

  // ensure tm_lists enabled
  if (!module_exists("tm_lists")) {
    return;
  }
  
  $base = $base_root . request_uri();

  // START add member to list
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Check if user can add to list
    if (tm_lists_user_can_add_to_lists($user)) {

      // Get user name
      $first_name = strip_tags($targetuser->field_user_first_name[LANGUAGE_NONE][0]['value']);
      $last_name = strip_tags($targetuser->field_user_last_name[LANGUAGE_NONE][0]['value']);
      $full_name = $first_name . " " . $last_name;

      $links['user'][] = array(
        'wrapper_class' => array('add_member_to_list'),
        'content' => l(t('Add to list'), 'javascript:tm_add_to_list("user", ' . $targetuser->uid . ', "' . htmlentities($full_name) . '")',
          array(
          'fragment' => '',
          'external'=> true,
          'attributes' => array(
            'title' => t('Add to list')
          )
        ))
      );
    }
  }
  // END add member to list


  // START add node to list
  if (isset($menu_item['page_arguments'][0]->nid)) {

    // note: require moderator role to show link for chapter
    if (($field['entity']->type == "organization")
      or ($field['entity']->type == "event")
      or (($field['entity']->type == "chapter") and (in_array("moderator", $user->roles)))) {

        // Check if user can add to list
        if (tm_lists_user_can_add_to_lists($user)) {

        // Get entity title
        $title = $field['entity']->title;

        $links[$field['entity']->type][] = array(
        'wrapper_class' => array('add_node_to_list'),
        'content' => l(t('Add to list'), 'javascript:tm_add_to_list("node", ' . $nid . ', "' . htmlentities($title) . '")',
          array(
          'fragment' => '',
          'external'=> true,
          'attributes' => array(
            'title' => t('Add to list')
          )
        ))
        );

      }
    }
  }
  // END remove node from list

   // START remove member from list
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Check if user can add to list
    if (tm_lists_user_can_remove_from_existing_list($user, 1, $targetuser->uid)) {

      // Get user name
      $first_name = strip_tags($targetuser->field_user_first_name[LANGUAGE_NONE][0]['value']);
      $last_name = strip_tags($targetuser->field_user_last_name[LANGUAGE_NONE][0]['value']);
      $full_name = $first_name . " " . $last_name;

      $links['user'][] = array(
        'wrapper_class' => array('add_member_to_list'),
        'content' => l(t('Remove from list'), 'javascript:tm_remove_from_list("user", ' . $targetuser->uid . ', "' . htmlentities($full_name) . '")',
          array(
          'fragment' => '',
          'external'=> true,
          'attributes' => array(
            'title' => t('Remove from list')
          )
        ))
      );
    }
  }
  // END remove member from list


  // START remove node from list
  if (isset($menu_item['page_arguments'][0]->nid)) {

      // note: require moderator role to show link for event or chapter
      if (($field['entity']->type == "organization")
      or (($field['entity']->type == "event") and (in_array("moderator", $user->roles)))
      or (($field['entity']->type == "chapter") and (in_array("moderator", $user->roles)))) {

        // Check if user can add to list
        if (tm_lists_user_can_remove_from_existing_list($user, 2, $field['entity']->nid)) {

        // Get entity title
        $title = $field['entity']->title;

        $links[$field['entity']->type][] = array(
        'wrapper_class' => array('add_node_to_list'),
        'content' => l(t('Remove from list'), 'javascript:tm_remove_from_list("node", ' . $nid . ', "' . htmlentities($title) . '")',
          array(
          'fragment' => '',
          'external'=> true,
          'attributes' => array(
            'title' => t('Remove from list')
          )
        ))
        );

      }
    }
  }
  // END remove node from list

}

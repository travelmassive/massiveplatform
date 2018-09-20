<?php

// tm_fields.menu_edit.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_edit_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;

  $base = $base_root . request_uri();

  // Is it a user
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {
    if (isset($menu_item['page_arguments'][0]->uid)) {
      $edit_menu = menu_get_item('user/' . $targetuser->uid . '/edit');
      if ($edit_menu['access']) {
        $links['user'][] = array(
          'wrapper_class' => array('user_edit'),
          'content' => l(t('Edit'), 'user/' . $targetuser->uid . '/edit', array(
            'attributes' => array(
              'title' => t('Edit user profile')
            )
          ))
        );
      }
    }
  }
  // END Edit link.

  // START Edit node link.
  if (isset($menu_item['page_arguments'][0]->nid)) {

    $allowed_to_edit = false;
    if (isset($field['entity']->type) && $field['entity']->type == 'event') {
      // use our own permission method for events
      $allowed_to_edit = tm_events_check_permission_edit_event($user, $field['entity']);
    } else {
      $edit_menu = menu_get_item('node/' . $nid . '/edit');
      $allowed_to_edit = $edit_menu['access'];
    }

    if ($allowed_to_edit) {
      $node_type = $field['entity']->type;
      $links[$node_type][] = array(
        'wrapper_class' => array('edit'),
        'content' => l(t('Edit'), 'node/' . $nid . '/edit', array(
          'attributes' => array(
            'title' => t('Edit')
          )
        ))
      );
    }
  }
  // End Edit node link.

}

<?php

// tm_fields.menu_user.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_user_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {
  
  global $conf;
  global $user;
  global $base_root;
  
  $base = $base_root . request_uri();

  // START requested approval note
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Only show link for unapproved users
    if (!in_array("approved user", $targetuser->roles)){

      // Show requested approval or date of signup if:
      // 1. if user is a moderator or administrator
      // 2. or if they are chapter leader and the member is signed up their chapter
      if ((in_array("moderator", $user->roles) or user_access('administer'))
        or ((in_array("chapter leader", $user->roles))) and (tm_users_member_in_users_chapter($targetuser->uid, $user->uid, true))) {

        // show if user request approval
        $flag = flag_get_flag('approval_requested_by_user');

        // get list of people who flagged
        $self_flagged = flag_get_entity_flags("user", $targetuser->uid, "approval_requested_by_user");

        // There should only be one flag (by the user)
        $menu_text = "";
        foreach ($self_flagged as $flagger) {
          $difference = time() - $flagger->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Requested " . $flagged_time . "</span><br>";
        }

        if ($menu_text != "") {
          $links['user'][] = array(
            'wrapper_class' => array('user_approve'),
            'content' => t($menu_text),
          );
        }
      }
    }
  }
  // END requested approval note


}

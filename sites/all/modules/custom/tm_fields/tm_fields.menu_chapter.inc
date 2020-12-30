<?php

// tm_fields.menu_chapter.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_chapter_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;

  $base = $base_root . request_uri();

  // START download chapter members.
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'chapter') {
      global $user;
      $show_menu_item = false;

      // show to chapter leaders
      if (tm_users_is_chapter_leader($currentuser->uid, $node->nid)) {
        $show_menu_item = true;
      }

      // if is admin or moderator always show
      if ((in_array('administrator', array_values($user->roles))) or (in_array('moderator', array_values($user->roles)))) {
        $show_menu_item = true;
      }

      if ($show_menu_item) {

        $current_uri = request_uri();
        $current_uri = substr($current_uri, 1); // delete first slash

        // export data
        $link_path = $current_uri.'/export';
        $links['chapter'][] = array(
          'wrapper_class' => array('chapter_download_csv'),
          'content' => '<a href="/' . $link_path . '">Download members list</a>'
        );

        // Display when last downloaded
        $download_last_flagged = flag_get_entity_flags("node", $nid, "download_member_csv");
        if (sizeof($download_last_flagged) > 0) {
          $flagged_last = array_shift($download_last_flagged);
          $difference = time() - $flagged_last->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $who_sent = user_load($flagged_last->uid);
          $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>" . $flagged_time . " by " . l($who_sent->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $who_sent->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . "</span><br>";
          $links['chapter'][] = array(
            'wrapper_class' => array('last_downloaded_members'),
            'content' => t($last_sent_text),
          );
        }

      }
    }
  }
  // END download chapter members

  // START chapter invitations and unapproved members
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'chapter') {
      global $user;
      $show_menu_item = false;

      // show to chapter leaders
      if (tm_users_is_chapter_leader($user->uid, $node->nid) || user_access('tm invite all chapters')) {
        $show_menu_item = true;
      }

      // if is admin or moderator always show
      if ((in_array('administrator', array_values($user->roles))) or (in_array('moderator', array_values($user->roles)))) {
        $show_menu_item = true;
      }

      if ($show_menu_item) {
        $current_uri = request_uri();
        $current_uri = substr($current_uri, 1); // delete first slash

        // chapter invitations
        $link_path = $current_uri.'/invitations';
        $links['chapter'][] = array(
          'wrapper_class' => array('chapter_invitations'),
           'content' => '<a href="/' . $link_path . '">Chapter invitations</a>'
        );

        // unapproved members
        $unapproved_label = "Review " . tm_users_get_unapproved_member_label("plural");
        $link_path = $current_uri.'/unapproved-members';
        $links['chapter'][] = array(
          'wrapper_class' => array('chapter_unapproved_members'),
          'content' => '<a href="/' . $link_path . '">' . $unapproved_label . '</a>'
        );

        // show unapproved member count
        $membership_counts = _tm_chapters_get_membership_counts($node->nid);
        $members_unapproved_total = $membership_counts["members_unapproved_total"];
        if ($membership_counts["members_unapproved_total"] > 0) {

          $unapproved_members_html = "1 Guest Member";
          if ($membership_counts["members_unapproved_total"] > 1) {
            $unapproved_members_html = $members_unapproved_total . " Guest Members";
          }

          $links['chapter'][] = array(
            'wrapper_class' => array('chapter_unapproved_members'),
            'content' => "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>" . $unapproved_members_html . "</span>" 
          );

        }

        // chapter announcement
        if ($conf['tm_chapters_announcements_enabled']) {
          $link_path = '/chapters/send-announcement/' . $node->nid;
          $links['chapter'][] = array(
            'wrapper_class' => array('chapter_sent_announcement'),
            'content' => l(t('Chapter announcement'), $link_path, array(
              'attributes' => array( 
              'title' => t('Chapter announcement')
              )
            ))
          );

          // Display when last sent announcement
          $announcement_last_sent_flagged = flag_get_entity_flags("node", $node->nid, "announcement_last_sent");
          if (sizeof($announcement_last_sent_flagged) > 0) {
            $flagged_last = array_shift($announcement_last_sent_flagged);
            $difference = time() - $flagged_last->timestamp;
            $flagged_time = format_interval($difference, 1) . " ago";
            $who_sent = user_load($flagged_last->uid);
            $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Sent " . $flagged_time . " by " . l($who_sent->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $who_sent->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . "</span><br>";
            $links['chapter'][] = array(
              'wrapper_class' => array('chapter_announcement'),
              'content' => t($last_sent_text),
            );
          }

        }

        // view stats
        $link_path = $current_uri.'/insights';
        $links['chapter'][] = array(
          'wrapper_class' => array('chapter_insights'),
          'content' => '<a href="/' . $link_path . '">Chapter insights</a>'
        );

        // view nps results
        $link_path = '/feedback/results/' . $node->nid;
        $links['chapter'][] = array(
          'wrapper_class' => array('member_feedback_results'),
          'content' => l(t('Member feedback'), $link_path, array(
            'attributes' => array( 
            'title' => t('Member feedback')
            )
          ))
        );

        if (module_exists("tm_payments")) {
          // view chapter payments
          $link_path = '/checkout/reports/chapter-payments/' . $node->nid . '/365';
          $links['chapter'][] = array(
            'wrapper_class' => array('chapter_payments'),
            'content' => '<a href="' . $link_path . '">Payments</a>'
          );
        }

        // profile wall
        $link_path = '/chapters/image_wall?chapter_id=' . $node->nid; 
        $links['chapter'][] = array(
          'wrapper_class' => array('chapter_images'),
          'content' => '<a target="_blank" href="' . $link_path . '">Profile wall</a>'
        );
        
      }
    }
  }
  // END chapter invitiations and unapproved members 

  // START add member to chapter
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Only show to chapter leader or moderator
    if (((in_array("chapter leader", $user->roles) or (in_array("moderator", $user->roles)))) 
      and ($targetuser->uid != $user->uid)) {

      // Only show if user is not in the members chapter, or is a moderator
      $leading_chapters = tm_users_get_chapters_leading($user);
      $members_chapters = tm_users_get_chapters($targetuser);
      $chapters_available_to_add = array_diff($leading_chapters, $members_chapters);
    
      if ((sizeof($chapters_available_to_add) > 0) or (in_array("moderator", $user->roles))) {

        // Get user name
        $first_name = strip_tags($targetuser->field_user_first_name[LANGUAGE_NONE][0]['value']);
        $last_name = strip_tags($targetuser->field_user_last_name[LANGUAGE_NONE][0]['value']);
        $full_name = $first_name . " " . $last_name;

        $links['user'][] = array(
          'wrapper_class' => array('chapter_add_to_chapter'),
          'content' => l(t('Add to chapter'), 'javascript:jq_add_member_to_chapter(' . $targetuser->uid . ', "' . htmlentities($full_name) . '")',
            array(
            'fragment' => '',
            'external'=> true,
            'attributes' => array(
              'title' => t('Add to chapter')
            )
          ))
        );
      }

    }
  }
  // END add member to chapter

  // START remove member from chapter
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Only show to chapter leader or moderator
    if ((in_array("chapter leader", $user->roles) or (in_array("moderator", $user->roles))) 
      and ($targetuser->uid != $user->uid)) {

      // Dont allow removal of chapter leader or moderator
      if (!in_array("chapter leader", $targetuser->roles) and (!in_array("moderator", $targetuser->roles))) {

        // Only show if user is in the members chapter, or is a moderator
        if (tm_users_member_in_users_chapter($targetuser->uid, $user->uid) or (in_array("moderator", $user->roles))) {

          // Display if member has joined at least one chapter
          $members_chapters = tm_users_get_chapters($targetuser);
          if (sizeof($members_chapters) > 0) {

            // Get user name
            $first_name = strip_tags($targetuser->field_user_first_name[LANGUAGE_NONE][0]['value']);
            $last_name = strip_tags($targetuser->field_user_last_name[LANGUAGE_NONE][0]['value']);
            $full_name = $first_name . " " . $last_name;

            $links['user'][] = array(
              'wrapper_class' => array('chapter_remove_from_chapter'),
              'content' => l(t('Remove from chapter'), 'javascript:jq_remove_member_to_chapter(' . $targetuser->uid . ', "' . htmlentities($full_name) . '")',
                array(
                'fragment' => '',
                'external'=> true,
                'attributes' => array(
                  'title' => t('Remove from chapter')
                )
              ))
            );

          }
        }
      }
    }
  }
  // END remove member from chapter

}

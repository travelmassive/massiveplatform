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

  // START approve link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {
    if (isset($menu_item['page_arguments'][0]->uid)){
      if (!in_array("approved user", $targetuser->roles)){

        // Need approve users access
        if (user_access("approve users")){

          // Show approval tool if:
          // 1. if user is a moderator or administrator
          // 2. or if they are chapter leader and the member is signed up their chapter
          // 3. or if they are a community manager and the member is a chapter they community manage
          if ((in_array("moderator", $user->roles) or user_access('administer')) 
          or ((in_array("chapter leader", $user->roles))) and (tm_users_member_in_users_chapter($targetuser->uid, $user->uid, true))) {

            // Check if user can be approved
            // Note: users can request if score > 50
            // We allow approval if score >= 50 to give a bit of flexibility
            $user_score = tm_users_signup_score($targetuser->uid);

            $score_threshold = 50; // default
            if (isset($conf["tm_users_approval_minimum_score"])) {
              $score_threshold = $conf["tm_users_approval_minimum_score"];
            }
            
            if (($user_score >= $score_threshold) or (in_array("moderator", $user->roles) or user_access('administer'))) {

              // Show approve user link
              $community_values_url = $conf["tm_community_values_url"];
              $links['user'][] = array(
                'moderator' => array('moderator_approve'),
                'content' => l(t('Approve member'), "javascript:jq_confirm_approve_member(" . $targetuser->uid . ", '" . $community_values_url . "')", array(
                  'fragment' => '',
                  'external'=>true,
                  'attributes' => array(
                    'title' => t('Approve member')
                  )
                ))
              );

            } else {

              // Incomplete profile
              $links['user'][] = array(
                'wrapper_class' => array('moderator_approve_message'),
                'content' => l(t('Approve member'), "javascript:jq_alert('Incomplete Profile', 'Sorry, you can\'t approve this member as their profile is only " . $user_score . "% complete.')", array(
                  'fragment' => '',
                  'external'=>true,
                  'attributes' => array(
                    'title' => t('Approve member')
                  )
                ))
              );

            }

          }
        }
      }
    }
  }
  // END approve link
  
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

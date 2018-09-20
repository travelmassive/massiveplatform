<?php

// tm_fields.menu_moderator.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_moderator_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;
  
  $base = $base_root . request_uri();

  // START unapprove link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {
    if (isset($menu_item['page_arguments'][0]->uid)){
      if (in_array("approved user", $targetuser->roles)){

        if (user_access("unapprove users")){

          // Don't show unapproval of moderators and admins
          if ((!in_array("moderator", $targetuser->roles)) and (!in_array("chapter leader", $targetuser->roles)) and (!in_array("administrator", $targetuser->roles))) {

            $links['user'][] = array(
              'wrapper_class' => array('moderator_unapprove'),
              'content' => l(t('Un-Approve'), 'javascript:jq_confirm_unapprove_user(' . $targetuser->uid . ')', array(
                'fragment' => '',
                'external'=>true,
                'attributes' => array(
                  'title' => t('Un-Approve user')
                )
              ))
            );
            
          }   
        }
      }
    }
  }
  // END unapprove link


  // START moderator info
  // Signup date
  // Reason for joining
  // Who approved
  // Email
  // Geoip
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // show who approved user
    if ((in_array("moderator", $user->roles) or user_access('administer')) 
    or ((in_array("chapter leader", $user->roles)))) {

      // 1. SIGNUP AND MODIFIED DATE
      $menu_text = "";
      $show_user = user_load($targetuser->uid);
      $difference = time() - $show_user->created;
      $flagged_time = format_interval($difference, 1) . " ago";
      $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Signed up " . $flagged_time . "</span><br>";
      
      $links['user_info'][] = array(
        'wrapper_class' => array('moderator_signed_up'),
        'content' => t($menu_text),
      );

      // 2. WHO APPROVED
      $flag = flag_get_flag('approval_approved_by');

      // get list of people who flagged
      $who_flagged = flag_get_entity_flags("user", $targetuser->uid, "approval_approved_by");

      // sort by flag time
      usort($who_flagged, function($a, $b){
        return ($a->timestamp > $b->timestamp) ? -1 : 1;
      });

      $menu_text = "";
      $approved_difference = null;
      foreach ($who_flagged as $flagger) {
        $approved_difference = time() - $flagger->timestamp;
        $flagged_time = format_interval($approved_difference, 1) . " ago";
        $flagged_user = user_load($flagger->uid);
        if ($flagged_user->uid > 0) {
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Approved by " . l($flagged_user->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $flagged_user->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . " " . $flagged_time . "</span><br>";
        } else {
          // Approved by a user who has removed their account
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Approved " . $flagged_time . "</span><br>";
        }
      }

      // if empty menu text, then user was approved before we started flagging
      if (($menu_text == "") and (in_array("approved user", $targetuser->roles))) {
        $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Approved by administrator</span>";
      }

      $links['user_info'][] = array(
        'wrapper_class' => array('moderator_approved_by'),
        'content' => t($menu_text),
      );

      // 3. SHOW MODIFIED TIME
      if (isset($show_user->field_last_modified_timestamp[LANGUAGE_NONE][0]['value'])) {
        $updated_difference = time() - $show_user->field_last_modified_timestamp[LANGUAGE_NONE][0]['value'];
        // show if updated different to approval time
        if ($updated_difference <> $approved_difference) {
          $flagged_time = format_interval($updated_difference, 1) . " ago";
          $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Profile updated " . $flagged_time . "</span><br>";
          $links['user_info'][] = array(
            'wrapper_class' => array('profile_updated'),
            'content' => t($menu_text),
          );
        }
      }

      // 4. WHO HAS VIEWED PROFILE
      if (module_exists("tm_track_views")) {
        if (in_array("moderator", $user->roles) or in_array("administrator", $user->roles)) {
        
          // get number of viewers  
          $max_view_days = $conf["tm_track_views_store_days"];
          $num_views = tm_track_views_how_many_viewed($targetuser->uid, "user", $targetuser->uid, $max_view_days);
          
          if ($num_views > 0) {

            $num_views_plural = "members";
            if ($num_views == 1) {
              $num_views_plural = "member";
            }

            $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'><a style='display: inline; padding: 0rem;' href='/user/" . $targetuser->uid . "/viewers'>Viewed by " . $num_views .  " " . $num_views_plural . "</a></span>";

            $links['user_info'][] = array(
              'wrapper_class' => array('moderator_viewed_by'),
              'content' => t($menu_text),
            );
          }
          
        }
      }

      // 5. REASON FOR JOINING
      if (isset($targetuser->field_reason_for_joining[LANGUAGE_NONE][0]['value'])) {
        $reason_for_joining = strip_tags($targetuser->field_reason_for_joining[LANGUAGE_NONE][0]['value']);
        if (trim($reason_for_joining) != "") {
          $menu_text = "<div style='font-size: smaller; font-style: italic; padding-left: 2em; background-color: #efefef;'>Reason for joining:<br><span style='color: #888; font-style: normal;'>" . $reason_for_joining . "</span></div>";

          $links['user_info'][] = array(
            'wrapper_class' => array('moderator_reason_for_joining'),
            'content' => t($menu_text),
          );
        }
      }

      // 6. Show Facebook signin
      if (isset($targetuser->field_facebook_signin_link[LANGUAGE_NONE][0]['value'])) {
        $facebook_signin_link = strip_tags($targetuser->field_facebook_signin_link[LANGUAGE_NONE][0]['value']);
        if (trim($facebook_signin_link) != "") {
          $menu_text = "<div style='font-size: smaller; font-style: italic; padding-left: 2em; background-color: #efefef;'>Signed in with Facebook:<br><span style='color: #888; font-style: normal;'><a style='display: inline; padding: 0rem;' target='_blank' href='" . $facebook_signin_link . "'>view Facebook profile</a></span></div>";

          $links['user_info'][] = array(
            'wrapper_class' => array('moderator_facebook_login'),
            'content' => t($menu_text),
          );
        }
      }

      // 7. Show any possible duplicate logins
      if (isset($conf["tm_users_check_duplicate_accounts"])) {
        if ($conf["tm_users_check_duplicate_accounts"] == true) {

          // Check duplicates
          $possible_duplicates = tm_users_get_duplicate_accounts($targetuser->uid);
          if (sizeof($possible_duplicates) > 0) {

            // Show duplicate accounts
            $duplicate_message = "Possible duplicate";
            if (sizeof($possible_duplicates) > 1) {
              $duplicate_message = "Possible duplicates";
            }
            $menu_text = "<div style='font-size: smaller; font-style: italic; padding-left: 2em; background-color: #efefef;'>" . $duplicate_message . ":<br>";
            foreach ($possible_duplicates as $duplicate_uid) {

              $duplicate_account = user_load($duplicate_uid);
              $duplicate_first_name = strip_tags($duplicate_account->field_user_first_name[LANGUAGE_NONE][0]['value']);
              $duplicate_last_name = strip_tags($duplicate_account->field_user_last_name[LANGUAGE_NONE][0]['value']);

              $duplicate_user_url = drupal_get_path_alias("user/" . $duplicate_uid);
              $menu_text .= "<span style='color: #888; font-style: normal;'><a style='display: inline; padding: 0rem;' target='_blank' href='/" . $duplicate_user_url . "'>" . $duplicate_first_name . " " . $duplicate_last_name . " (id: " . $duplicate_uid . ")</a><br></span>";
            }
            $menu_text .= "</div>";

            $links['user_info'][] = array(
              'wrapper_class' => array('moderator_duplicate_account'),
              'content' => t($menu_text),
            );

          }

        }
      }
      
      // 8. Show email address to chapter leader if unapproved
      // Always show to moderators
      if ((!in_array("approved user", $targetuser->roles) and (tm_users_member_in_users_chapter($targetuser->uid, $user->uid, true))) or (in_array("moderator", $user->roles))) {

        $verified_text = "not-verified";
        if (!in_array('non-validated', $targetuser->roles)) {
          $verified_text = "verified";
        }

        $menu_text = "<div style='font-size: smaller; font-style: italic; padding-left: 2em; background-color: #efefef;'>Signup email:<br><span style='color: #888; font-style: normal;'><a style='display: inline; padding: 0rem;' href='mailto:" . $targetuser->mail . "'>" . $targetuser->mail . "</a> (" . $verified_text . ")</span></div>";

        $links['user_info'][] = array(
          'wrapper_class' => array('moderator_contact_details'),
          'content' => t($menu_text),
        );
      }

      // 9. Show last activity time if in your chapter and IP to moderator
      if (tm_users_member_in_users_chapter($targetuser->uid, $user->uid, true) or (in_array("moderator", $user->roles))) {

        // Get last active time    
        $last_visit_timestamp = $targetuser->access;
        if (($last_visit_timestamp == null) or ($last_visit_timestamp == 0)) {
          $last_visit_time = "never";
        } else {
          $last_visit_difference = time() - $last_visit_timestamp;
          if ($last_visit_difference < (60 * 60 * 24)) {
            // $last_visit_time = "today";
            $last_visit_time = format_interval($last_visit_difference, 1) . " ago";
          } else {
            $last_visit_time = format_interval($last_visit_difference, 1) . " ago";
          }
        }

        // Get geoip info
        $last_ip = tm_geoip_get_users_last_ip($targetuser->uid);
        $geoip_details = "";
        if ($last_ip == null) {
          $last_ip = "never signed in";
        } else {
          $geoip_info = tm_geoip_lookup($last_ip);
          if ($geoip_info != null) {
            $geoip_labels = array();
            if (isset($geoip_info["city"]["names"]["en"])) {
              $geoip_labels[] = htmlentities($geoip_info["city"]["names"]["en"]);
            }
            if (isset($geoip_info["country"]["names"]["en"])) {
              $geoip_labels[] = htmlentities($geoip_info["country"]["names"]["en"]);
            }
            $geoip_details .= implode(", ", $geoip_labels);
          }
        }

        // Show IP to moderator
        if (in_array("moderator", $user->roles)) {
          if ($geoip_details != "") {
            $geoip_details .= "<br>";
          }
          // link to /admin/geoip_text
          $geoip_details .= "IP: <a style='display: inline; padding: 0rem;' target='_blank' href='/admin/geoip_info?ip=" . $last_ip . "'>" . $last_ip . "</a>";
        }
       
        $menu_text = "<div style='font-size: smaller; font-style: italic; padding-left: 2em; background-color: #efefef;'>Last active " . $last_visit_time . "<br><span style='color: #888; font-style: normal;'>" . $geoip_details . "</span></div>";

        $links['user_info'][] = array(
          'wrapper_class' => array('last_active'),
          'content' => t($menu_text),
        );
      }

      // 10. Show if user has set profile to private
      if (in_array("moderator", $user->roles) or in_array("administrator", $user->roles)) {
        if (isset($targetuser->field_user_has_private_profile[LANGUAGE_NONE][0]['value'])) {
          if ($targetuser->field_user_has_private_profile[LANGUAGE_NONE][0]['value']) {

            $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Profile set to private</span>";

            $links['user_info'][] = array(
              'wrapper_class' => array('profile_private'),
              'content' => t($menu_text),
            );
          }
        }
      }

    }
  }
  // END moderator info

  // START profile incomplete link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Only show link for unapproved users
    if (!in_array("approved user", $targetuser->roles)){

      // Show flagging tool if:
      // 1. if user is a moderator or administrator
      // 2. or if they are chapter leader and the member is signed up their chapter
      if ((in_array("moderator", $user->roles) or user_access('administer'))
        or ((in_array("chapter leader", $user->roles))) and (tm_users_member_in_users_chapter($targetuser->uid, $user->uid, true))) {

        $links['user'][] = array(
          'wrapper_class' => array('moderator_flag_incomplete'),
          'content' => l(t('Flag: Incomplete profile'), 'javascript:jq_confirm_incomplete_profile(' . $targetuser->uid . ')',
            array(
            'fragment' => '',
            'external'=> true,
            'attributes' => array(
              'title' => t('Profile incomplete')
            )
          ))
        );
        
        // show last time request info was flagged
        $flag = flag_get_flag('approval_need_more_info');

        // get list of people who flagged
        $who_flagged = flag_get_entity_flags("user", $targetuser->uid, "approval_need_more_info");

        // sort by flag time
        usort($who_flagged, function($a, $b){
          return ($a->timestamp > $b->timestamp) ? -1 : 1;
        });

        $menu_text = "";
        foreach ($who_flagged as $flagger) {
          $difference = time() - $flagger->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $flagged_user = user_load($flagger->uid);
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Flagged by " . l($flagged_user->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $flagged_user->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . " " . $flagged_time . "</span><br>";
        }
        
        $links['user'][] = array(
          'wrapper_class' => array('moderator_flagged_by'),
          'content' => t($menu_text),
        );
      }
    }
  }
  // END profile incomplete link

  // START is company or brand link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Only show link for unapproved users
    if (!in_array("approved user", $targetuser->roles)){

      // Show flagging tool if:
      // 1. if user is a moderator or administrator
      // 2. or if they are chapter leader and the member is signed up their chapter
      if ((in_array("moderator", $user->roles) or user_access('administer')) 
        or ((in_array("chapter leader", $user->roles))) and (tm_users_member_in_users_chapter($targetuser->uid, $user->uid, true))) {

        $links['user'][] = array(
          'wrapper_class' => array('moderator_flag_is_company_or_brand'),
          'content' => l(t('Flag: Is company or brand'), 'javascript:jq_confirm_company_profile(' . $targetuser->uid . ')',
            array(
            'fragment' => '',
            'external'=> true,
            'attributes' => array(
              'title' => t('Is company or brand')
            )
          ))
        );
        
        // show last time request info was flagged
        $flag = flag_get_flag('approval_is_company_or_brand');

        // get list of people who flagged
        $who_flagged = flag_get_entity_flags("user", $targetuser->uid, "approval_is_company_or_brand");

        // sort by flag time
        usort($who_flagged, function($a, $b){
          return ($a->timestamp > $b->timestamp) ? -1 : 1;
        });

        $menu_text = "";
        foreach ($who_flagged as $flagger) {
          $difference = time() - $flagger->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $flagged_user = user_load($flagger->uid);
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Flagged by " . $flagged_user->field_user_first_name['und'][0]['value'] . " " . $flagged_time . "</span><br>";
        }
        
        $links['user'][] = array(
          'wrapper_class' => array('moderator_flagged_by'),
          'content' => t($menu_text),
        );

      }
    }
  }
  // END is company or brand link

  // START moderate community values link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Don't show flag for chapter leaders or moderators
    if ((!in_array("moderator", $targetuser->roles)) and (!in_array("chapter leader", $targetuser->roles)) and (!in_array("administrator", $targetuser->roles))) {

      // Show flagging tool if:
      // 1. if user is a moderator or administrator
      // 2. or if they are chapter leader and the member is signed up their chapter
      if ((in_array("moderator", $user->roles) or user_access('administer'))
        or ((in_array("chapter leader", $user->roles))) and (tm_users_member_in_users_chapter($targetuser->uid, $user->uid, true))) {

        $community_values_url = $conf["tm_community_values_url"];

        $links['user'][] = array(
          'wrapper_class' => array('moderator_flag_non_community'),
          'content' => l(t('Flag: Non-community'), 'javascript:jq_confirm_non_community_profile(' . $targetuser->uid . ', \'' . $community_values_url . '\')',
            array(
            'fragment' => '',
            'external'=> true,
            'attributes' => array(
              'title' => t('Non-community profile')
            )
          ))
        );
        
        // show last time request info was flagged
        $flag = flag_get_flag('moderate_non_community_profile');

        // get list of people who flagged
        $who_flagged = flag_get_entity_flags("user", $targetuser->uid, "moderate_non_community_profile");

        // sort by flag time
        usort($who_flagged, function($a, $b){
          return ($a->timestamp > $b->timestamp) ? -1 : 1;
        });

        $menu_text = "";
        foreach ($who_flagged as $flagger) {
          $difference = time() - $flagger->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $flagged_user = user_load($flagger->uid);
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Flagged by " . $flagged_user->field_user_first_name['und'][0]['value'] . " " . $flagged_time . "</span><br>";
        }
        
        $links['user'][] = array(
          'wrapper_class' => array('moderator_flagged_by'),
          'content' => t($menu_text),
        );
      }
    }
  }
  // END moderate community values link

   // START report member profile link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Don't show flag for administrators or own acccount
    if (!in_array("administrator", $targetuser->roles) and ($targetuser->uid != $user->uid)) {

      // Step 1. Show report flag to approved members
      if (tm_users_moderate_report_check($user)) {

        $community_values_url = $conf["tm_community_values_url"];

        $links['user'][] = array(
          'wrapper_class' => array('user_report'),
          'content' => l(t('Report'), 'javascript:void(0);',
            array(
            'fragment' => '',
            'external'=> true,
            'attributes' => array(
              'title' => t('Report'),
              'onClick' => 'javascript:jq_confirm_report_member(' . $targetuser->uid . ', \'' . $community_values_url . '\')'
            )
          ))
        );
      }

      // Step 2. Show who flagged this member to moderators and chapter leaders
      $who_flagged = array();
      if ((in_array("moderator", $user->roles) or user_access('administer'))
      or (in_array("chapter leader", $user->roles))) {

        // show last time request info was flagged
        $flag = flag_get_flag('moderate_report_member');

        // get list of people who flagged
        $who_flagged = flag_get_entity_flags("user", $targetuser->uid, "moderate_report_member");

        // sort by flag time
        usort($who_flagged, function($a, $b){
          return ($a->timestamp > $b->timestamp) ? -1 : 1;
        });

        $menu_text = "";
        foreach ($who_flagged as $flagger) {
          $difference = time() - $flagger->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $flagged_user = user_load($flagger->uid);
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Reported by " . l($flagged_user->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $flagged_user->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . " " . $flagged_time . "</span><br>";
        }
        
        $links['user'][] = array(
          'wrapper_class' => array('user_reported_by'),
          'content' => t($menu_text),
        );
      }

      // Step 3. Show unflag link to moderators
      if (sizeof($who_flagged) > 0) {

        if (in_array("moderator", $user->roles) or user_access('administer')) {

          $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>" . l("Issue resolved", 'javascript:void(0);',
              array('fragment' => '','external'=>true, 'attributes' => array('onClick' => 'javascript:jq_confirm_report_member_resolved(' . $targetuser->uid . ');', 'class' => array('report-flagged-link')))) . "</span><br>";

           $links['user'][] = array(
            'wrapper_class' => array('user_report_resolved'),
            'content' => t($menu_text),
          );
        }
      }

    }
  }
  // END report member profile link

  // START cancel member profile link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Only show to administrator or moderator
    if (((in_array("administrator", $user->roles) or (in_array("moderator", $user->roles)))) 
      and ($targetuser->uid != $user->uid)) {

      $links['user'][] = array(
        'wrapper_class' => array('user_cancel_account'),
        'content' => l(t('Cancel account'), '/user/' . $targetuser->uid . '/cancel',
          array(
          'attributes' => array(
            'title' => t('Cancel account'),
          )
        ))
      );
    }
  }
  // END cancel account profile link

  // START Set as sponsor link
  if (isset($field['entity']->nid) && $flag_link = flag_create_link('tm_sponsor', $field['entity']->nid)) {

      // 1. Show Flag / Unflag link
      // only show for admin and moderator
      // chapter leaders flag sponsors via the event page but not directly
      if ((in_array('administrator', array_values($user->roles))) or (in_array('moderator', array_values($user->roles)))) {

      $links['organization'][] = array(
        'wrapper_class' => array('moderator_set_sponsor'),
        'content' => $flag_link
      );

      // 2. WHO FLAGGED
      $flag = flag_get_flag('tm_sponsor');

      // get list of people who flagged
      $who_flagged = flag_get_entity_flags("node", $field['entity']->nid, "tm_sponsor");

      // sort by flag time
      usort($who_flagged, function($a, $b){
        return ($a->timestamp > $b->timestamp) ? -1 : 1;
      });

      $menu_text = "";
      foreach ($who_flagged as $flagger) {
        $difference = time() - $flagger->timestamp;
        $flagged_time = format_interval($difference, 1) . " ago";
        if ($flagger->uid == 0) { 
          $flagger_name = "Administrator"; // global flags used to record 0
        }
        else {
          $flagged_user = user_load($flagger->uid);
          $flagger_name = $flagged_user->field_user_first_name['und'][0]['value'];
        }
        $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Set by " . $flagger_name . " " . $flagged_time . "</span><br>";
      }

      $links['organization'][] = array(
        'wrapper_class' => array('moderator_sponsor_set_by'),
        'content' => t($menu_text),
      );

    }
  }
  // END Set as sponsor link

  // START preview notification link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Only show to administrator or moderator
    if (in_array("administrator", $user->roles) or in_array("moderator", $user->roles)) {

      if (module_exists("tm_newsfeed") and ($conf["tm_newsfeed_moderator_preview"])) {
        $links['moderator'][] = array(
          'wrapper_class' => array('moderator_preview_news_update'),
          'content' => l(t('Preview news update'), '/admin/newsfeed/notification_preview?uid=' . $targetuser->uid,
            array(
            'external' => true,
            'attributes' => array(
              'target' => '_blank',
            )
          ))
        );
      }
      
    }
  }
  // END preview notification link

  // BEGIN google analytics report link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'node/%')) {

    global $user;
    
    if (in_array("access-google-analytics-report", $user->roles)) {

      // get report path
      if ($menu_item['path'] == 'user/%') {
        $report_path = drupal_get_path_alias("user/" . $field['entity']->uid);
      } else {
        $report_path = drupal_get_path_alias("node/" . $field['entity']->nid);
      }      
      $google_analytics_report_link = tm_fields_google_analytics_report_link($report_path);

      // create link
      $links['moderator'][] = array(
        'wrapper_class' => array('moderator_analytics_link'),
        'content' => l(t('View analytics'), $google_analytics_report_link,
          array(
          'fragment' => '',
          'external'=> true,
          'attributes' => array(
            'title' => t('View analytics'),
            'target' => '_blank'
          )
        ))
      );

    }
  }
  // END google analytics report link

}
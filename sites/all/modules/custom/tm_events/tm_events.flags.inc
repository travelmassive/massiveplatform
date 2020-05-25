<?php

// tm_events.flags.inc - flag methods

/**
 * Hook - Automatically join chapter if you sign up to an event for that chapter
 */
function tm_events_flag_flag($flag, $entity_id, $account, $flagging) {
  
  // only listen for event register or event waitlist
  if (($flag->name != 'event_register') and ($flag->name != 'event_waitlist')) {
    return;
  }

  global $conf;

  // get user who flagged
  $user = user_load($account->uid);

  // get the chapter
  $event = node_load($entity_id);

  // get type of event
  $typeof_event = null;
  if (isset($event->field_chapter[LANGUAGE_NONE][0])) {
    $typeof_event = "chapter_event";
  } elseif (isset($event->field_event_company[LANGUAGE_NONE][0])){
    $typeof_event = "company_event";
  } else {
    $typeof_event = "member_event";
  }

  // Option 1. Chapter event
  if ($typeof_event == "chapter_event") {

    // Automatically join chapter if any of the following:
    // Case 1. Event is not online event (ie: in person)
    // Case 2. Event chapter is a group (ie: online group)
    // Case 3. Member country is same as event country (more relevant)
    // Note: New users will still join the event chapter (via chapter_referral_id)

    $join_chapter = false;
    $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
    $event_chapter = node_load($chapter_id);

    // Case 1. Not online event
    if (!tm_events_is_online_event($event)) {
       $join_chapter = true;
    }

    // Case 2. Event chapter is a group
    if (tm_chapters_chapter_is_group($event_chapter)) {
      $join_chapter = true;
    }

    // Case 3. Member country is same as event country

    // Get user's country
    $users_country = null;
    if (isset($user->field_user_country[LANGUAGE_NONE][0]['iso2'])) {
      $users_country = $user->field_user_country[LANGUAGE_NONE][0]['iso2'];
    }

    // Get event country
    $country_code = null;
    if (isset($event_chapter->field_country[LANGUAGE_NONE][0]['iso2'])) {
      $country_code = $event_chapter->field_country[LANGUAGE_NONE][0]['iso2'];
    }

    // In same country
    if ($users_country == $country_code) {
      $join_chapter = true;
    }

    // Show help tip to view chapter if not automatically joining
    if (!$join_chapter) {
      if (!tm_chapters_is_member_of_chapter($user, $event_chapter)) {
        $chapter_name = tm_chapters_formal_name($event_chapter);
        $chapter_url = drupal_get_path_alias("node/" . $chapter_id);
        drupal_set_message("Learn more about the <a href='/" . $chapter_url . "'>" . $chapter_name . "</a> community and how to get involved.", "help-tip");
      }
    }

    // Join chapter
    if ($join_chapter) {
      $signup_flag = flag_get_flag('signup');
      if (!$signup_flag->is_flagged($chapter_id, $account->uid)) {
        flag('flag', 'signup', $chapter_id, $user);
      } 
    }

  }

  // Option 2. Company event
  if ($typeof_event == "company_event") {

    // Check tm_event_follow_company
    if (isset($conf["tm_event_follow_company"])) {
      if ($conf["tm_event_follow_company"]) {

        // follow organization
        $company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
        $follow_organizations_flag = flag_get_flag('follow_organizations');
        if (!$follow_organizations_flag->is_flagged($company_id, $account->uid)) {
          flag('flag', 'follow_organizations', $company_id, $user);
        }
      }
    }
  }

  // Option 3. Member event
  if ($typeof_event == "member_event") {

    // Check tm_event_follow_member
    if (isset($conf["tm_event_follow_member"])) {
      if ($conf["tm_event_follow_member"]) {

        // follow member
        $event_owner_uid = $event->uid;
        $follow_members_flag = flag_get_flag('follow_members');
        if (!$follow_members_flag->is_flagged($event_owner_uid, $account->uid)) {
          flag('flag', 'follow_members', $event_owner_uid, $user);
        } 
      }
    }
  }

}

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

    // (Note: automatically join chapter)

    // join chapter
    $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
    $signup_flag = flag_get_flag('signup');
    if (!$signup_flag->is_flagged($chapter_id, $account->uid)) {
      flag('flag', 'signup', $chapter_id, $user);
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

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

  // get user who flagged
  $user = user_load($account->uid);

  // get the chapter
  $event = node_load($entity_id);

  // if event doesn't have a chapter, return
  if (!isset($event->field_chapter[LANGUAGE_NONE])) {
    return;
  }

  $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
  $chapter = node_load($chapter_id);

  // get signup flag
  $signup_flag = flag_get_flag('signup');
  if ($signup_flag) {
    if (!$signup_flag->is_flagged($chapter->nid, $account->uid)) {
      flag('flag', 'signup', $chapter->nid, $user);
    } 
  }
}
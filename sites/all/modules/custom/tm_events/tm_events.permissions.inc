<?php

// tm_events.permissions.inc - access permission methods

/**
 * Implements hook_permission()
 */
function tm_events_permission() {
  return array(
    'add event to any chapter' => array(
      'title' => t('Add event to any chapter'),
      'description' => t('Add events to a chapter, even without being a chapter leader.'),
    ),
  );
}

/**
 * Implement hook_node_access
 * Use our own method tm_events_check_permission_edit_event to determinine access
 */
function tm_events_node_access($node, $op, $account) {

    // create event
    if (is_string($node) && ($node == "event") && ($op == 'create')) {

      $allowed = false;

      if (isset($_GET["field_chapter"])) {
        $chapter_id = $_GET["field_chapter"];
        $allowed = _tm_events_check_create_chapter_event($account, $chapter_id);
      } else if (isset($_GET["field_event_company"])) {
        $company_id = $_GET["field_event_company"];
        $allowed = _tm_events_check_create_company_event($account, $company_id);
      } else {
        // member event
        $allowed = _tm_events_check_create_member_event($account);
      }

      if ($allowed) {
        return NODE_ACCESS_ALLOW;
      } else {
        return NODE_ACCESS_DENY;
      }  
    }

    // node is not event
    if (is_string($node)) {
       return NODE_ACCESS_IGNORE;
    }

    // update event
    if (($node->type == 'event') && ($op == 'update')) {

        if (tm_events_check_permission_edit_event($account, $node)) {
          return NODE_ACCESS_ALLOW;
        } else {
          return NODE_ACCESS_DENY;
        }  
    }

    // delete event event
    if (($node->type == 'event') && ($op == 'delete')) {

      // Check permissions first
      if (tm_events_check_permission_edit_event($account, $node)) {
        
        // Check that no members are already registered
        $flag_register = flag_get_flag('event_register');
        $flag_waitlist = flag_get_flag('event_waitlist');
        $num_registered = $flag_register->get_count($node->nid);
        $num_waitlisted = $flag_waitlist->get_count($node->nid);

        if (($num_registered == 0) && ($num_waitlisted == 0)) {
          return NODE_ACCESS_ALLOW;
        } else {
          return NODE_ACCESS_DENY;
        } 

      } else {
        return NODE_ACCESS_DENY;
      }
    }

    // we can always view published events
    if (($node->type == 'event') && ($op == 'view')) {
      if ($node->status == 1) {
        return NODE_ACCESS_ALLOW;
      }
    }

    return NODE_ACCESS_IGNORE;
}

/**
 * Callback wrapper for tm_events_check_permission_manage_event()
 */
function tm_events_check_permission_callback() {

	global $user;
	$event_id = arg(2); // /events/action/1234
	$event = node_load($event_id);

  // check permissions
  return tm_events_check_permission_edit_event($user, $event);
}

/**
 * Check if a user has ability to edit existing event
 * This will allow access to edit event, manage rsvps, and send announcements
 * Will also check configuration settings
 */
function tm_events_check_permission_edit_event($check_user, $event) {

  global $conf;

  // need to be logged in
  if (!user_is_logged_in()) {
    return false;
  }

  // invalid node id
  if ($event == null) {
    return false;
  }

  // invalid event type
  if ($event->type != "event") {
    return false;
  }

  // must have validated email
  if (in_array('non-validated', $check_user->roles)) {
    return false;
  }

  // must be approved user
  if (!in_array("approved user", $check_user->roles)) {
    return false;
  }

  // if is admin or moderator always allow
  if ((in_array('administrator', array_values($check_user->roles))) or (in_array('moderator', array_values($check_user->roles)))) {
    return true;
  }

  // if is chapter leader
  if ($conf["tm_event_chapter_events_enabled"] == true) {
    if (isset($event->field_chapter[LANGUAGE_NONE][0])) {
      if (tm_users_is_chapter_leader($check_user->uid, $event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
        return true;
      }
    }
  }

  // if managing company page 
  if ($conf["tm_event_company_events_enabled"] == true) {
    if (isset($event->field_event_company[LANGUAGE_NONE][0])) {
      if (tm_organizations_is_company_profile_manager($check_user->uid, $event->field_event_company[LANGUAGE_NONE][0]['target_id'])) { 
        return true;
      }
    }
  }

  // for events without a chapter, if the user is an organizer of or owner
  if ($conf["tm_event_member_events_enabled"] == true) {
    if (isset($event->field_organizer[LANGUAGE_NONE])) {
      foreach ($event->field_organizer[LANGUAGE_NONE] as $field_organizer) {  
        if ($field_organizer["target_id"] == $check_user->uid) {
          return true;
        }
      }
    }
    // is owner
    if ($check_user->uid == $event->uid) {
      return true;
    }
  }

  // not allowed
  return false;
}

/**
 * Check if user can create a chapter event
 */
function _tm_events_check_create_chapter_event($check_user, $chapter_id) {
  
  global $conf;

  // need to be logged in
  if (!user_is_logged_in()) {
    return false;
  }

  // check tm_event_chapter_event_enabled is enabled
  if ($conf["tm_event_chapter_events_enabled"] != true) {
    return false;
  }

  // check valid chapter
  $chapter = node_load($chapter_id);
  if ($chapter == null) {
    return false;
  }

  // must have validated email
  if (in_array('non-validated', $check_user->roles)) {
    return false;
  }

  // must be approved user
  if (!in_array("approved user", $check_user->roles)) {
    return false;
  }

  // if is admin or moderator always allow
  if ((in_array('administrator', array_values($check_user->roles))) or (in_array('moderator', array_values($check_user->roles)))) {
    return true;
  }

  // check if user has 'add event to any chapter' permission
  if (user_access('add event to any chapter')) {
    return true;
  }

  // check if user is chapter leader of this chapter
  if (tm_users_is_chapter_leader($check_user->uid, $chapter_id)) {
    return true;
  }

  // no access
  return false;
}

/**
 * Check if user can create a company event
 */
function _tm_events_check_create_company_event($check_user, $company_id) {

  global $conf;

  // need to be logged in
  if (!user_is_logged_in()) {
    return false;
  }

  // check tm_event_chapter_event_enabled is enabled
  if ($conf["tm_event_company_events_enabled"] != true) {
    return false;
  }

  // must have validated email
  if (in_array('non-validated', $check_user->roles)) {
    return false;
  }

  // must be approved user
  if (!in_array("approved user", $check_user->roles)) {
    return false;
  }

  // if is admin or moderator always allow
  if ((in_array('administrator', array_values($check_user->roles))) or (in_array('moderator', array_values($check_user->roles)))) {
    return true;
  }

  // check user is a manager of the company page 
  if (!tm_organizations_is_company_profile_manager($check_user->uid, $company_id)) {
    return false;
  }

  // if field_company_enable_events enabled, allow company event
  $company = node_load($company_id);
  if (isset($company->field_company_enable_events[LANGUAGE_NONE][0]['value'])) {
    if ($company->field_company_enable_events[LANGUAGE_NONE][0]['value']) {
      return true;
    }
  }

  // allow approved members with minimum number of company followers to create an event
  $min_followers = $conf["tm_event_company_event_min_followers"];
  $num_followers = tm_organizations_get_num_followers($company_id);
  if ($num_followers >= $min_followers) {
    return true;
  }

  // check for subscription account (ie: premium member)
  if (module_exists("tm_subscriptions")) {
    if (tm_subscriptions_is_organization_subscription_enabled($company_id)) {
      return true;
    }
  }

	return false;
}

/**
 * Check if user can create a member event
 */
function _tm_events_check_create_member_event($check_user) {

  global $conf;

  // need to be logged in
  if (!user_is_logged_in()) {
    return false;
  }

  // allow moderators and administrators to create member event
  if (in_array("moderator", $check_user->roles) || in_array("administrator", $check_user->roles) ) { 
    return true;
  }

  // check tm_event_chapter_event_enabled is enabled
  if ($conf["tm_event_member_events_enabled"] != true) {
    return false;
  }

  // must have validated email
  if (in_array('non-validated', $check_user->roles)) {
    return false;
  }

  // must be approved user
  if (!in_array("approved user", $check_user->roles)) {
    return false;
  }

  // allow chapter leaders to create member events
  if (in_array("chapter leader", $check_user->roles)) { 
    return true;
  }

  // allow approved members with minimum number of followers to create an event
  $min_followers = $conf["tm_event_member_event_min_followers"];
  $num_followers = tm_users_get_num_followers($check_user->uid);
  if ($num_followers >= $min_followers) {
    return true;
  }

  // check for subscription account (ie: premium member)
  if (module_exists("tm_subscriptions_user")) {
    if (tm_subscriptions_is_user_subscription_enabled($check_user->uid)) {
      return true;
    }
  }

  // no access 
  return false;
}

/**
 * Check if user can create a member event
 * Return a message explaining the requirements
 * Return empty teaser message if action shouldn't be displayed to user
 */
function _tm_events_check_create_member_event_message($check_user) {

  global $conf;

  // not enabled
  if ($conf["tm_event_member_events_enabled"] != true) {
    return "";
  }

  // not approved member
  if (!in_array("approved user", $check_user->roles)) {
    return "";
  }

  // must have validated email
  if (in_array('non-validated', $check_user->roles)) {
    return "Please verify your email address.";
  }

  // need minimum number of followers to vouch for you
  $min_followers = $conf["tm_event_member_event_min_followers"];
  $num_followers = tm_users_get_num_followers($check_user->uid);
  if ($num_followers < $min_followers) {
    $message = "<b>You need " . ($min_followers - $num_followers) . " more members to vouch for you.</b>";
    $message .= "<br><br>To add a member event in our community you need at least " . $conf["tm_event_member_event_min_followers"] . " followers.";
    return $message;
  }

  // No reason
  return "";

}

/**
 * Check if user can create a company event
 * Return a message explaining the requirements
 * Return empty teaser message if action shouldn't be displayed to user
 */
function _tm_events_check_create_company_event_message($check_user, $company_id) {

  global $conf;

  // not enabled
  if ($conf["tm_event_member_events_enabled"] != true) {
    return "";
  }

  // not approved member
  if (!in_array("approved user", $check_user->roles)) {
    return "";
  }

  // must be company that you manage
  if (!tm_organizations_is_company_profile_manager($check_user->uid, $company_id)) {
    return "";
  }

  // must have validated email
  if (in_array('non-validated', $check_user->roles)) {
    return "Please verify your email address.";
  }

  // need minimum number of company followers to vouch for you
  $min_followers = $conf["tm_event_company_event_min_followers"];
  $num_followers = tm_organizations_get_num_followers($company_id);
  if ($num_followers < $min_followers) {
    $message = "<b>You need " . ($min_followers - $num_followers) . " more members to vouch for you.</b>";
    $message .= "<br><br>To add an event in our community your company needs at least " . $conf["tm_event_company_event_min_followers"] . " followers.";
    return $message;
  }

  // No reason
  return "";

}

/**
 * Special check to see if featured member of event can download the registration list
 */
function tm_events_check_featured_member_download_csv($check_user, $event) {

  // for events with featured members and allowed to download rsvp list
  // we check field_event_featured_access_rsvp is enabled
  if (isset($event->field_event_featured_access_rsvp[LANGUAGE_NONE][0]['value'])) {
    if ($event->field_event_featured_access_rsvp[LANGUAGE_NONE][0]['value'] == true) {
      if (isset($event->field_event_featured_members[LANGUAGE_NONE])) {
        foreach ($event->field_event_featured_members[LANGUAGE_NONE] as $field_featured_member) {  
          if ($field_featured_member["target_id"] == $check_user->uid) {
            return true;
          }
        }
      }
    }
  }

  // not allowed
  return false;
}

/**
 * Callback wrapper for tm_events_check_permission_announce_event()
 */
function tm_events_check_announcement_callback() {

  global $user;
  $event_id = arg(2); // /events/action/1234
  $event = node_load($event_id);

  // check permissions
  return tm_events_check_permission_announce_event($user, $event);
}

/**
 * Check if a user has ability to announce existing event
 */
function tm_events_check_permission_announce_event($check_user, $event) {

  // Step 1. Check edit permissions
  $allowed = tm_events_check_permission_edit_event($check_user, $event);
  if ($allowed) {
    return true;
  }

  // Step 2. Check "Enable Chapter announcements" setting
  if (isset($event->field_chapter_can_announce[LANGUAGE_NONE][0]['value'])) {
    if ($event->field_chapter_can_announce[LANGUAGE_NONE][0]['value']) {

      // check if user is chapter leader of published chapter
      $published_chapters = tm_events_get_published_chapters($event);
      foreach ($published_chapters as $chapter_id) {
        if (tm_users_is_chapter_leader($check_user->uid, $chapter_id)) {
          return true;
        }
      }
    }
  }

  // no permission
  return false;
}

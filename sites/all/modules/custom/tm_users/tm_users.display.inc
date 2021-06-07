<?php

// tm_users.display.inc - user display methods

/**
 * Modify users account content before being displayed
 */
function tm_users_user_view($account, $view_mode, $langcode) {

  global $conf;
  global $user;

  // redirect from /user to full alias
  if (($view_mode == "full") and (request_path() == "user")) {
    drupal_goto(drupal_get_path_alias("user/" . $account->uid));
    return;
  }

  // Show additional link fields based on $conf['tm_users_link_fields']
  $all_fields = array('website', 'twitter', 'facebook', 'linkedin', 'instagram', 'youtube', 'vimeo', 'snapchat', 'strava', 'tiktok');
  if (isset($conf["tm_users_link_fields"])) {
    $show_link_fields = $conf["tm_users_link_fields"];
    foreach ($all_fields as $field) {
      if (!in_array($field, $show_link_fields)) {
        unset($account->content['field_link_' . $field]); // hide field
      }
    }
  }

  // Add javascript to show edit photo buttons
  if ($view_mode == "full") {
    tm_users_add_photo_edit_js($account);
  }

  // If viewing own account, set create member event message js
  if (($view_mode == "full") && ($account->uid == $user->uid)) {
    // Set create member event message as js var
    // We use this in jq_create_member_event_message()
    $member_event_message = _tm_events_check_create_member_event_message($account);
    drupal_add_js(array('tm_events' => array('create_member_event_message' => $member_event_message)), array('type' => 'setting'));
  }

  // Member privacy - add noindex metatag if field_user_disable_public_index enabled
  if ($view_mode == "full") {
    if (isset($account->field_user_disable_public_index[LANGUAGE_NONE][0]['value'])) {
      if ($account->field_user_disable_public_index[LANGUAGE_NONE][0]['value']) {

        $noindex_meta_tag = array(
          '#tag' => 'meta',
          '#attributes' => array(
          'name' => 'robots',
          'content' => 'noindex, nofollow',
          ),
        );
        drupal_add_html_head($noindex_meta_tag, 'user_profile_meta_robots');
      }
    }
  }

  // Member privacy - don't allow anonymous or non-approved member to view
  if ($view_mode == "full") {
    if (isset($account->field_user_has_private_profile[LANGUAGE_NONE][0]['value'])) {
      if ($account->field_user_has_private_profile[LANGUAGE_NONE][0]['value']) {

        $private_profile = false;

        // check anonymous user
        if (user_is_anonymous()) {
          $private_profile = true;
        } else {
          // check user is approved
          if ((!in_array('approved user', $user->roles)) and (!in_array('administrator', $user->roles))) {
            $private_profile = true;
          }
        }

        // allow non-approved member to view their own profile
        if ($account->uid == $user->uid) {
          $private_profile = false;
        }

        // if profile private, display message
        if ($private_profile) {

          // also add no-index
          $noindex_meta_tag = array(
            '#tag' => 'meta',
            '#attributes' => array(
            'name' => 'robots',
            'content' => 'noindex, nofollow',
            ),
          );
          drupal_add_html_head($noindex_meta_tag, 'user_profile_meta_robots');

          // markup the access denied message with js
          $js = "document.title = 'Private profile'; document.getElementById('page-title').innerHTML = 'Private profile'; document.getElementById('content').innerHTML = document.getElementById('content').innerHTML.replace('You are not authorized to access this page.', '');";
          drupal_add_js($js, "inline");
          drupal_set_message("This member's profile is private. Only approved members can view this page.", "help-tip");
          drupal_access_denied();
          module_invoke_all('exit');
          exit();
        }

      }
    }
  }

  // Unapproved members - add noindex metatag if member is not approved
  if ($view_mode == "full") {

    if (!in_array('approved user', $account->roles)) {

      // also add no-index
      $noindex_meta_tag = array(
        '#tag' => 'meta',
        '#attributes' => array(
        'name' => 'robots',
        'content' => 'noindex, nofollow',
        ),
      );
      drupal_add_html_head($noindex_meta_tag, 'user_profile_meta_robots');

    }
  }

}

/** 
 * Add display suite field for follower stats and featured user 
 */
function tm_users_ds_fields_info($entity_type) {
  if ($entity_type != 'user') {
    return;
  }

  // stats
  $fields = array();
  $fields['tm_ds_user_stats'] = array(
    'title' => t('Member Stats'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('user|*'),
    'function' => '_tm_ds_user_stats'
  );

  // featured user badge
  $fields['tm_ds_user_featured'] = array(
    'title' => t('Featured Member Badge'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('user|*'),
    'function' => '_tm_ds_user_featured'
  );
  return array($entity_type => $fields);
}

/**
 * Render if user is feature
 */
function _tm_ds_user_featured($field, $title = NULL) {
  global $conf;
  $flag = flag_get_flag('featured_member');
  $loaded = user_load($field['entity']->uid);

  // show if chapter leader
  if (in_array("chapter leader", $loaded->roles)) {
    return t($conf['tm_site_name'] . " Chapter Leader");
  }
  
  // show custom badge if featured
  if (tm_user_is_featured_member($field['entity']->uid)) {
    return t($conf["tm_featured_members_badge_text"]);
  }
}

/**
 * Render user's follower and following stats
 */
function _tm_ds_user_stats($field, $title = NULL) {

  // check if user is flagged
  $user = $field['entity'];
  $uid = $user->uid;

  $followers_text = "Follower";
  $following_text = "Following";
  $events_text = "Event";

  $num_followers = tm_users_get_num_followers($uid);
  $num_following = tm_users_get_num_following($uid);
  $num_events = tm_users_get_num_registered_events($uid);

  if (($num_followers == 0) and ($num_following == 0) and ($num_events == 0)) {
    return; 
  }

  $html = "<div class='tm_stats_container'>";
  if ($num_followers > 0) {
    if ($num_followers > 1) { $followers_text = "Followers"; }
    $html .= "<a href='/user/" . $uid . "/followers'><span class='tm_stats tm_stats_followers'>" . $num_followers . " <span class='tm_stats_followers_divider'>|</span> " . $followers_text . " </span></a>";
  }
  
  if ($num_following > 0) {
    $html .= "<a href='/user/" . $uid . "/following'><span class='tm_stats tm_stats_following'>" . $num_following . " <span class='tm_stats_following_divider'>|</span> " . $following_text . " </span></a>";
  }
  
  if ($num_events > 0) {
    if ($num_events > 1) { $events_text = "Events"; }
      $html .= "<span class='tm_stats tm_stats_events'>" . $num_events . " <span class='tm_stats_events_divider'>|</span> " . $events_text . "</span>";
  }
  $html .= "</div>";

  return $html;
}

/* 
 * Link by title to any chapters this user has joined
 * inserts this markup into the user menu tpl file
 */
function tm_users_menu_chapters($uid){
 
  global $conf;
  $markup = "";
  $MAX_ITEMS = 4;

  // Step 1. Fetch list of chapters the user has joined
  $flags = flag_get_user_flags("node");
  $chapters_nids = array();
  foreach ($flags as $flag_name => $flag) {
    if ($flag_name == "signup") {
      foreach($flag as $f) {
        $chapters_nids[] = $f->entity_id;
      }
    }
  }

  // Step 2. Fetch list of chapters the user is a leader of and is community manager of

  // Chapter leader
  $chapters_leader_nids = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'chapter')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_chapter_leaders', 'target_id', $uid, '=');

  $result = $query->execute();
  if (isset($result['node'])) {
    $chapters_leader_nids = array_keys($result['node']);
  }

  // Community manager
  $community_manager_nids = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'chapter')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_community_managers', 'target_id', $uid, '=');

  $result = $query->execute();
  if (isset($result['node'])) {
    $community_manager_nids = array_keys($result['node']);
  }

  // Step 3. Join the two arrays of chapter leader nids and joined chapters
  $all_chapter_nids = $chapters_leader_nids;
  foreach ($chapters_nids as $nid) {
    $all_chapter_nids[] = $nid;
  }

  // Step 4. Create HTML
  $chapter_html = array();
  $chapter_leader_html = array();
  $num_items = 0;
  $count_chapters = 0;
  foreach ($all_chapter_nids as $nid) {

    $count_chapters++;
    $chapter = node_load($nid);

    // add to chapter html
    if (!array_key_exists($chapter->title, $chapter_html)) {

      // Create html for menu entry
      $chapter_leader_text = " (Member)";
      if (in_array($nid, $chapters_leader_nids)) { 
        $chapter_leader_text = " (Chapter Leader)";
      }
      if (in_array($nid, $community_manager_nids)) { 
        $chapter_leader_text = " (Community Manager)";
      }

      // get image
      $img_url = $conf["tm_images_default_field_image"];
      if (isset($chapter->field_image[LANGUAGE_NONE][0]['uri'])) {
        $img_url = $chapter->field_image[LANGUAGE_NONE][0]['uri'];
      }
      $img_url = _tm_chapters_get_default_image($img_url, $nid);
      $style = 'thumbnail';

      $html = "<li style='margin-bottom: 4px;'><div style='width: 36px; max-height: 24px; overflow: hidden; float: left; margin-left: 1rem; margin-right: 0.5rem; margin-top: 2px;'>
      <img align='left' class='tm_menu_chapter_image' src='" . image_style_url($style, $img_url) . "'></div>" . l($chapter->title . $chapter_leader_text,'node/'.$chapter->nid) . '</li>'; 

      // Put into either a chapter or chapter_leader array
      if (in_array($nid, $chapters_leader_nids)) { 
        $chapter_leader_html[$chapter->title] = $html;
      } else {
        $chapter_html[$chapter->title] = $html;
      }
    }
  }

  // Step 5. Join HTML
  $markup = "";

  if ($count_chapters > 0) {

    // Sort the chapters by title
    ksort($chapter_html);
    ksort($chapter_leader_html);

    // Show chapters you are a chapter leader for first
    foreach ($chapter_leader_html as $html) {
      // only print max items
      $num_items++;
      if ($num_items > $MAX_ITEMS) {
        continue;
      }
      $markup .= $html;
    }

    // Show other chapters second
    foreach ($chapter_html as $html) {
      // only print max items
      $num_items++;
      if ($num_items > $MAX_ITEMS) {
        continue;
      }

      $markup .= $html;
    }
    if ($num_items > $MAX_ITEMS) {
      $markup .= "<li>" . l(t('(View ') . ($num_items - $MAX_ITEMS) . " more)", 'chapters') . "</li>";
    }
  }
  else {
    $markup .= "<li>" . l(t('Join a chapter'), 'chapters') . "</li>";
  }

  return $markup;
}

/**
 * Add javascript to show edit photo buttons
 */
function tm_users_add_photo_edit_js($account) {

  global $user;
  global $conf;

  // check we are viewing user account
  if (!(arg(0) == 'user' and is_numeric(arg(1)) and arg(2) == FALSE)) {
    return;
  }

  // Check we are looking at own account
  if ($user->uid != $account->uid) {
    return;
  }

  // check if user has profile and cover images
  $has_profile_image = false;
  if (isset($account->field_avatar[LANGUAGE_NONE][0]['uri'])) {
    $has_profile_image = ($account->field_avatar[LANGUAGE_NONE][0]['uri'] != $conf["tm_images_default_field_avatar"]);
  }

  $has_cover_image = false;
  if (isset($account->field_image[LANGUAGE_NONE][0]['uri'])) {
    $has_cover_image = ($account->field_image[LANGUAGE_NONE][0]['uri'] != $conf["tm_images_default_field_image"]);
  }

  // add js variables
  drupal_add_js(array('tm_users' => array('current_user_uid' => $user->uid)), array('type' => 'setting'));
  drupal_add_js(array('tm_users' => array('has_profile_image' => $has_profile_image)), array('type' => 'setting'));
  drupal_add_js(array('tm_users' => array('has_cover_image' => $has_cover_image)), array('type' => 'setting'));

  // add edit photos script to page
  drupal_add_js(drupal_get_path('module', 'tm_users') . '/js/user-edit-photos.js');

}

/* 
 * Link by title to any companies this user is managing 
 * inserts this markup into the user menu tpl file
 */
function tm_users_menu_companies($uid){

  global $conf;

  // get list of company pages managed by user
  $companies_nids = tm_organizations_get_users_companies($uid);

  $markup = "";
  foreach ($companies_nids as $nid) {
    $company = node_load($nid);

    // get image
    $img_url = $conf["tm_images_default_field_image"];
    if (isset($company->field_image[LANGUAGE_NONE][0]['uri'])) {
      $img_url = $company->field_image[LANGUAGE_NONE][0]['uri'];
    }
    $img_url = _tm_organizations_get_default_image($img_url, $nid);
    $style = 'thumbnail';

    // check if subscription
    $subscription_html = "";
    if (module_exists("tm_subscriptions")) {
      if (tm_subscriptions_is_organization_subscription_enabled($nid)) {
        $subscription_html = "<style></style>";
        $subscription_html .= " <span class='tm-user-menu-subscription-badge'>" . strtoupper($conf["tm_subscriptions_organization_label_short"]) . "</span>";
      }
    }
    $company_url = drupal_get_path_alias("node/" . $company->nid);
    $markup .= "<li style='margin-bottom: 4px;'><div style='width: 36px; max-height: 24px; overflow: hidden; float: left; margin-left: 1rem; margin-right: 0.5rem; margin-top: 2px;'>
    <img align='left' class='tm_menu_organization_image' src='" . image_style_url($style, $img_url) . "'></div><a href='/" . $company_url . "'>" . strip_tags($company->title) . $subscription_html . '</a></li>'; 
  }
  return $markup;
}

/**
 * Return default avatar image
 * If tm_images_default_path is set then return a random image based in node id
 * URL must be in public:// format
 */
function _tm_users_get_default_avatar($default_image_uri, $uid) {

  global $conf;

  if (isset($conf["tm_images_default_path"])) {
    if (($default_image_uri == $conf["tm_images_default_field_avatar"]) or ($default_image_uri == "")) {
      $avatar_files = $conf["tm_images_default_avatar"];
      $image_index = $uid % sizeof($avatar_files);
      $avatar_file = $conf["tm_images_default_path"] . $avatar_files[$image_index];
      return $avatar_file;
    }
  }

  // no random image, return what we got
  return $default_image_uri;
}

/**
 * Helper method to return user's avatar uri
 * If user has default image, use _tm_users_get_default_avatar() to return random avatar
 * Otherwise, return user image
 */
function _tm_users_get_avatar_uri($account) {

  global $conf;

  // Return avatar
  if (isset($account->field_avatar[LANGUAGE_NONE][0]['uri'])) {
    if ($account->field_avatar[LANGUAGE_NONE][0]['uri'] != $conf["tm_images_default_field_avatar"]) {
      return $account->field_avatar[LANGUAGE_NONE][0]['uri'];
    }
  }

  // Return random avatar
  return _tm_users_get_default_avatar($conf["tm_images_default_field_avatar"], $account->uid);
}

/* 
 * Link by title to any events this user had registered for or is organizer of
 * inserts this markup into the user menu tpl file
 */
function tm_users_menu_events($uid, $include_chapter_events = false, $limit = 5, $is_moderator = false){

  global $conf;

  // get list of events the user has registered for or is organizer of
  $event_nids = tm_events_get_users_future_events($uid, $include_chapter_events);
  $event_nids = array_slice($event_nids, 0, $limit);

  $markup = "";
  foreach ($event_nids as $nid) {
    $event = node_load($nid);

    // check valid event
    if ($event == null) {
      continue;
    }

    // get image
    $img_url = $conf["tm_images_default_field_image"];
    if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
      $img_url = $event->field_image[LANGUAGE_NONE][0]['uri'];
    }
    $img_url = _tm_events_get_default_image($img_url, $nid);

    // get preview mode
    $preview_mode_text = "";
    if (tm_events_is_preview_mode($nid)) {
      $preview_mode_text = " (Preview mode)";
    }

    // get waitlisted
    $waitlisted_text = "";
    if (tm_events_user_is_waitlisted($nid, $uid)) {
      $waitlisted_text = " (Waitlist)";
    }

    // how long until event
    $time_until_text = "";
    $event_date = field_get_items('node',$event,'field_event_date');
    $current_date = new DateTime('now', new DateTimeZone('UTC'));
    $event_timezone = _tm_get_event_timezone($event);
    $event_datetime = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
    if (($event_datetime->format('U') - $current_date->format('U')) > 0) {
      $time_until_text =  "<br>in " . _tm_event_time_elapsed($event_datetime->format('U') - $current_date->format('U')) . $waitlisted_text;
    }

    $style = 'thumbnail';
    $event_menu_text = t($event->title) . "<span style='font-style: italic; font-size: smaller;'>" . $time_until_text . $preview_mode_text . "</span>";
    $event_menu_link = drupal_get_path_alias("node/" . $event->nid);

    $markup .= "<li>
    <a href='/" . $event_menu_link . "'>
    <img style='width: 36px; display:block; float:left' class='tm_menu_event_image' src='" . image_style_url($style, $img_url) . "'><span class='tm_menu_event_link' style='width: 100%; display:block; padding-left: 44px;'>" . $event_menu_text . '</span></a></li>'; 
  }

  return $markup;
}

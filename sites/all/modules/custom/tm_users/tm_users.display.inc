<?php

// tm_users.display.inc - user display methods

/**
 * Modify users account content before being displayed
 */
function tm_users_user_view($account, $view_mode, $langcode) {

  global $conf;

  // Show additional link fields based on $conf['tm_users_link_fields']
  $all_fields = array('website', 'twitter', 'facebook', 'linkedin', 'instagram', 'youtube', 'vimeo', 'snapchat');
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
  if ($flag->is_flagged($field['entity']->uid)) {
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

  // Step 2. Fetch list of chapters the user is a leader of
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

  // get list of company profiles managed by user
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
    $markup .= "<li style='margin-bottom: 4px;'><div style='width: 36px; max-height: 24px; overflow: hidden; float: left; margin-left: 1rem; margin-right: 0.5rem; margin-top: 2px;'>
    <img align='left' class='tm_menu_organization_image' src='" . image_style_url($style, $img_url) . "'></div>" . l($company->title,'node/'.$company->nid) . '</li>'; 
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

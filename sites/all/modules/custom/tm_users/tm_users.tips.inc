<?php

// tm_users.tips.inc - user tips


/* Give new members some signup tips to help */
function tm_users_signup_tips($return_score = false, $uid = null, $show_threshold = 80) {

  global $user;
  global $conf;

  // don't calculate to non-logged in users
  if (!user_is_logged_in()) {
    return 0;
  }

  // load current user if no uid specified
  if ($uid == null) {
    $uid = $user->uid;
  }

  $account = user_load($uid);

  $user_tip = "";
  $user_points = 10; // some points for signing up
  $total_points = 10; // start at 10

   // user edit page
  $edit_url = "user/" . $uid . "/edit";

  // about me (10 points)
  $total_points += 10;
  if (sizeof($account->field_about_me) == 0) {
    if ($user_tip == "") {
      $user_tip = "Share a little bit " . l(t('about yourself'), $edit_url, array('fragment' => 'edit-field-about-me')) . " and your story with the community.";
    }
  } else {
    $user_points += 10;
  }

  // location (city and country) (10 points)
  $total_points += 10;
  if ((sizeof($account->field_location_city) == 0) or (sizeof($account->field_user_country) == 0)) {
    if ($user_tip == "") {
      $user_tip = "Tell us " . l(t('where you are located'), $edit_url, array('fragment' => 'edit-field-location-city')) . " so other members can discover you.";
    }
  } else {
    $user_points += 10;
  }

  // job role (10 points)
  $disable_job_role = false;
  if (isset($conf["tm_users_disable_job_role"])) {
    $disable_job_role = $conf["tm_users_disable_job_role"];
  }
  if (!$disable_job_role) {
    $total_points += 10;
    if (sizeof($account->field_job_role) == 0) {
      if ($user_tip == "") {
        $user_tip = "Share your " . l(t('job role'), $edit_url, array('fragment' => 'edit-field-job-role')) . " so like-minded members can connect with you.";
      }
    } else {
      $user_points += 10;
    }
  }
 
  // organization (10 points)
  $disable_job_organization = false;
  if (isset($conf["tm_users_disable_job_organization"])) {
    $disable_job_organization = $conf["tm_users_disable_job_organization"];
  }
  if (!$disable_job_organization) {
    $total_points += 10;
    if (sizeof($account->field_job_organization) == 0) {
      if ($user_tip == "") {
        $user_tip = "Share the organization " . l(t('where you work'), $edit_url, array('fragment' => 'edit-field-job-organization')) . ", except if you are a top secret agent.";
      }
    } else {
      $user_points += 10;
    }
  }

  // user photo (10 points)
  $total_points += 10;
  $default_image = false;
  if (isset($account->field_avatar['und'][0]['is_default'])) {
    $default_image = (isset($account->field_avatar['und'][0]['is_default']));
  }
  if ((sizeof($account->field_avatar) == 0) or ($default_image)) {
    if ($user_tip == "") {
      $user_tip = "Upload a " . l(t('profile photo'), $edit_url, array('fragment' => 'edit-field-avatar')) . " so members can recognize you.";
    }
  } else {
    $user_points += 10;
  }

  // cover photo (10 points)
  $total_points += 10;
  $default_image = false;
  if (isset($account->field_image['und'][0]['is_default'])) {
    $default_image = (isset($account->field_image['und'][0]['is_default']));
  }
  if ((sizeof($account->field_image) == 0) or ($default_image)) {
    if ($user_tip == "") {
      $user_tip = "Upload a " . l(t('cover image'), $edit_url, array('fragment' => 'edit-field-image')) . " to make your profile stand out.";
    }
  } else {
    $user_points += 10;
  }

  // industry segment (10 points)
  // organization (10 points)
  $disable_industry_segment = false;
  if (isset($conf["tm_users_disable_industry_segment"])) {
    $disable_industry_segment = $conf["tm_users_disable_industry_segment"];
  }
  if (!$disable_industry_segment) {
    $total_points += 10;
    if (sizeof($account->field_segment) == 0) {
      if ($user_tip == "") {
        $user_tip = "Select an " . l(t('industy segment'), $edit_url, array('fragment' => 'edit-field-segment')) . " to help us connect you with other members.";
      }
    } else {
      $user_points += 10;
    }
  }

  // profile questions (10 points)
  $total_points += 10;
  // get question fields
  // we search the object array rather than hardcoding in case the question fields are changed
  $object_vars = get_object_vars($account);
  $has_answered_question = false;
  foreach ($object_vars as $name => $value) {
    if (strpos($name, "field_user_question_") === 0) {
      if (sizeof($value) > 0) {
        $has_answered_question = true;
      }
    }
  }
  if (!$has_answered_question) {
    if ($user_tip == "") {
      $user_tip = "Answer some " . l(t('profile questions'), $edit_url, array('fragment' => 'edit-field-user-question-destination')) . " so other members can learn about you.";
    }
  } else {
    $user_points += 10;
  }

  // website fields (10 points)
  $total_points += 10;
  // get website fields
  // we search the object array rather than hardcoding in case more links are added
  $object_vars = get_object_vars($account);
  $has_linked_website = false;
  foreach ($object_vars as $name => $value) {
    if (strpos($name, "field_link_") === 0) {
      if (sizeof($value) > 0) {
        $has_linked_website = true;
      }
    }
  }
  if (!$has_linked_website) {
    if ($user_tip == "") {
      $user_tip = "List your " . l(t('website, blog, or social accounts'), $edit_url, array('fragment' => 'edit-field-link-website')) . " so other members can find you.";
    }
  } else {
    $user_points += 10;
  }

  // calculate percentage complete
  $percentage = round((($user_points / $total_points) * 100), 0);
  // score is in 0 to 100 in increments of 5
  $user_score = round($percentage*2,-1)/2;

  // Option 1. Return score
  // if we only want the score
  // score wil be 0 to 100
  if ($return_score) {
      return $user_score;
  }

  // Option 2. Display the tips
  // Only show on the following pages
  // Only show on up to two levels deep (ie: /chapters and /chapters/london)
  $target_pages = array("events", "community", "leaders", "search");
  $url_parts = explode('/', request_path());
  if ((!in_array($url_parts[0], $target_pages)) or (sizeof($url_parts) > 2)) {
    return;
  }
  // Only show on the /community page and not on users profiles ie: /community/username
  if (($url_parts[0] == "community") && (sizeof($url_parts) > 1)) {
    return;
  }
  
  // Display the user tip
  if (($user_tip != "") and ($show_threshold > $user_score)) {
      $user_tip = "Your profile is " . $user_score . "% complete. " . $user_tip;
      drupal_set_message($user_tip, 'page_notice');
  }

  return;
}

/* Show user tips on chapter page */
function tm_users_in_chapter_tips() {

  global $conf;
  global $user;
  $user_tip = "";

  // make sure we are logged in
  if (!user_is_logged_in()) {
    return;
  }

  // get chapter
  // /chapters/berlin
  // Show chapter page tips
  $url_parts = explode("/", request_path());
  if (($url_parts[0] == "chapters") and (sizeof($url_parts) == 2)) {
    if ($url_parts[1] == "map") {
      return; // don't display for map
    }
    $node = menu_get_object();
    $nodeid = 0;

    // make sure we have a valid chapter
    if ($node == null) {
      return;
    }

    if ($node->type == "chapter") {
      $nodeid = $node->nid;
    }
    
  }

  // /node/12833/members
  if (($url_parts[0] == "node") and ($url_parts[2] == "members") and (sizeof($url_parts) == 3)) {
    $nodeid = $url_parts[1];
    $node = node_load($nodeid);
    if ($node->type == "chapter") {
      $nodeid = $node->nid;
    }
  }

  // get chapter title
  $chapter_name = $node->title;

  // get users chapters
  $chapters_nids = array();

  // Fetch list of chapters the user has joined
  $in_chapter = false;
  $flags = flag_get_user_flags("node");
  foreach ($flags as $flag_name => $flag) {
    if ($flag_name == "signup") {
      foreach($flag as $f) {
        $chapters_nids[] = $f->entity_id;
        if ($nodeid == $f->entity_id) {
          $in_chapter = true;
        }
      }
    }
  }

  // if you are a member of the chapter and non-approved
  // warn user they will not be displayed
  if ($in_chapter) {
    if (!in_array("approved user", $user->roles)) {
        $your_account_url = drupal_get_path_alias('user/' . $user->uid);
        $user_tip = "Your membership will be displayed in the " . $chapter_name . " community when <a href='/" . $your_account_url . "'>your account</a> has been verified.";
    }
  }

  // Shower user tip
  if ($user_tip != "") {
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  }
  
}

/* Show user tips on chapter page */
function tm_users_chapter_tips() {

  global $conf;
  global $user;
  $user_tip = "";

  $chapters_nids = array();
  if (user_is_logged_in()) {
    // Fetch list of chapters the user has joined
    $flags = flag_get_user_flags("node");
    foreach ($flags as $flag_name => $flag) {
      if ($flag_name == "signup") {
        foreach($flag as $f) {
          $chapters_nids[] = $f->entity_id;
        }
      }
    }
  }

  // If no chapters, show apply for chapter link
  $account = user_load($user->uid);
  if ((sizeof($chapters_nids) == 0)) {
    $user_tip = "Join a " . $conf["tm_site_name"] . " chapter near you or apply to " . l(t('start a chapter'), $conf["tm_tips_start_chapter_link"]) . " in your city.";
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  } 
  elseif (sizeof($account->field_home_chapter) > 0) {
      $chapter = node_load($account->field_home_chapter['und'][0]['target_id']);
      $chapter_url = url('node/' . $chapter->nid, array("absolute" => true));
      $user_tip = "Explore the community and discover chapters world-wide. Your home chapter is " . l(t( $conf["tm_site_name"] . " " . $chapter->title), $chapter_url) . ".";
  }

  // Shower user tip
  if ($user_tip != "") {
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  }
  
}

/* Show user tips on groups page */
function tm_users_groups_tips() {

  global $conf;
  global $user;
  $user_tip = "";

  $chapters_nids = array();
  if (user_is_logged_in()) {
    // Fetch list of chapters the user has joined
    $flags = flag_get_user_flags("node");
    foreach ($flags as $flag_name => $flag) {
      if ($flag_name == "signup") {
        foreach($flag as $f) {
          $chapters_nids[] = $f->entity_id;
        }
      }
    }
  }

  // If no chapters, show apply for chapter link
  $account = user_load($user->uid);
  if ((sizeof($chapters_nids) == 0)) {
    $user_tip = "Join a " . $conf["tm_site_name"] . " group near you or apply to " . l(t('start a chapter'), $conf["tm_tips_start_chapter_link"]) . " for your industry group.";
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  } 

  // Shower user tip
  if ($user_tip != "") {
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  }
  
}

/* Show user tips on leaders page */
function tm_users_leaders_tips() {

  global $conf;
  global $user;
  $user_tip = "";

  if (in_array('chapter leader', $user->roles)) {
    // Fetch list of chapters the user is a leader of
    $chapters_leader_nids = array();
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'chapter')
      ->propertyCondition('status', NODE_PUBLISHED)
      ->fieldCondition('field_chapter_leaders', 'target_id', $user->uid, '=');
    $result = $query->execute();
    if (isset($result['node'])) {
      $chapters_leader_nids = array_keys($result['node']);
    }
    if (sizeof($chapters_leader_nids) > 0) {
      $first_chapter = node_load($chapters_leader_nids[0]);
      $chapter_url = url('node/' . $first_chapter->nid, array("absolute" => true));
      $user_tip = "Connect with other chapter leaders world-wide. You are a chapter leader of  " . l(t($conf["tm_site_name"] . " " . $first_chapter->title), $chapter_url) . ".";
    }
  } else {
    $user_tip = "Follow chapter leaders in the " . $conf["tm_site_name"] . " community, or apply to " . l(t('start a chapter'), $conf["tm_tips_start_chapter_link"]) . " in your city.";
  }

  // Shower user tip
  if ($user_tip != "") {
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  }

}

/* Show user tips on communities page */
function tm_users_community_tips() {

  global $conf;
  global $user;
  $user_tip = "Discover and follow other members in the " . $conf["tm_site_name"] . " community. Looking for an organization? Check out our " . l(t("company listings"), "companies") . ".";

  if (module_exists("tm_search_api")) {
    if (isset($conf["tm_tips_community_list"])) {
      $user_tip = $conf["tm_tips_community_list"];
    } else {
      $user_tip = "Discover and follow other members and companies in our community with <a href='/search'>" . $conf["tm_site_name"] . " Search</a>.";
    }
  }

  // Shower user tip
  if ($user_tip != "") {
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  } 

}

/* Show user tips on events page */
function tm_users_event_tips() {

  global $conf;
  global $user;
  $user_tip = "";

  $chapters_nids = array();
  if (user_is_logged_in()) {
    if (isset($account->field_home_chapter['und'][0]['target_id'])) {
      $account = user_load($user->uid);
      $chapter = node_load();
      $chapter_url = url('node/' . $chapter->nid, array("absolute" => true));
      $user_tip = "Visit your home chapter " . l(t( $conf["tm_site_name"] . " " . $chapter->title), $chapter_url) . " for events near you.";
    }
  } 
  
  if ($user_tip == "") {
    $user_tip = "Find and register for upcoming events in the " . $conf["tm_site_name"] . " community around the world to learn, share and connect with other members.";
  }

  // Shower user tip
  if ($user_tip != "") {
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  }
  
}

/* Show user tips on sponsors page */
function tm_users_sponsors_tips() {

  global $conf;
  global $user;
  $user_tip = "Our events are sponsored by leading companies and brands around the world. Learn more about " . l(t('sponsoring ' . $conf["tm_site_name"] . ' or a local chapter'), $conf["tm_tips_sponsor_page_link"]) . ".";

  // Shower user tip
  if ($user_tip != "") {
    drupal_get_messages('page_notice');
    drupal_set_message($user_tip, 'page_notice');
  }
  
}

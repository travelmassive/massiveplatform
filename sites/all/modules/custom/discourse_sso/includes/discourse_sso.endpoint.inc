<?php

/**
 * @file
 * Discource SSO endpoint functionality.
 * Modified by Ian C for Massive Platform.
 */

/**
 * Menu callback function used for discourse/sso.
 */
function discourse_sso_endpoint() {

  // Only allow approved members
  global $user;
  if (!in_array('approved user', array_values($user->roles))) {
    global $conf;
    drupal_set_message($conf["tm_discourse_sso_unaproved_message"], "help-tip");
    drupal_goto("user/" . $user->uid);
    return;
  }

  // Get SSO input parameters.
  // They have been already validated in access callback.
  $payload = discourse_sso_get_payload();
  $nonce = discourse_sso_get_nonce($payload);

  discourse_sso_process_response($nonce);
}

/**
 * Process response to discourse site.
 *
 * @param string $nonce
 *   In security engineering, a nonce is an arbitrary number used only once
 *   in a cryptographic communication. It ensures that old communications
 *   cannot be reused in replay attacks.
 */
function discourse_sso_process_response($nonce) {
  // Prepare response data.
  $user = discourse_sso_authenticate_user();
  $payload = discourse_sso_create_payload($user, $nonce);
  $sig = discourse_sso_sign_payload($payload);

  // Response to the discourse forum.
  discourse_sso_response($payload, $sig);
}

/**
 * Authenticate user.
 *
 * @global object $user
 *
 * @return object
 *   User object with user information.
 */
function discourse_sso_authenticate_user() {
  global $user;

  // User authentication.
  if (!user_is_logged_in()) {
    // Redirect user for login page and set redirection/destination back
    // to here after the user is logged in.
    drupal_goto('user', array('query' => drupal_get_destination()));
  }

  // load account
  $account = user_load($user->uid);
  return $account;
}

/**
 * Create payload.
 *
 * Payload contains nonce + user information.
 *
 * @param object $user
 *   User object with user information.
 * @param string $nonce
 *   In security engineering, a nonce is an arbitrary number used only once
 *   in a cryptographic communication. It ensures that old communications
 *   cannot be reused in replay attacks.
 *
 * @return string
 *   A payload. It's a base64 encoded query of parameters.
 */
function discourse_sso_create_payload($user, $nonce) {

  global $conf;

  // get user params
  $params = discourse_sso_get_user_params($user, $nonce);

  // Create payload from user params.
  $query = drupal_http_build_query($params);
  $payload = base64_encode($query);

  return $payload;

}

/**
 * Get user params
 */
function discourse_sso_get_user_params($user, $nonce = "") {

  global $conf;

  // use TM avatar
  if (isset($user->field_avatar[LANGUAGE_NONE][0]['uri'])) {
    $avatar_url = image_style_url("avatar", $user->field_avatar[LANGUAGE_NONE][0]['uri']);
  } else {
    $avatar_url = image_style_url("avatar", "public://default_images/avatar-default.png");
  }

  // bio
  $bio = $user->field_about_me[LANGUAGE_NONE][0]['value'];

  // use profile_url as bio
  $profile_url = "__SERVER_HTTP__://__SERVER_NAME__/" . drupal_get_path_alias("user/" . $user->uid);
  $profile_url = str_replace("__SERVER_NAME__", $conf["tm_email_server_url_domain"], $profile_url); 
  $profile_url = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $profile_url);  
  $bio = "Find me on " . $conf["site_name"] . " at " . $profile_url;

  // admins
  $discourse_admin_uids = $conf["tm_discourse_admin_uids"]; // array(123);
  $is_admin = 'false';
  if (in_array('administrator', array_values($user->roles))) {
    $is_admin = 'true';
  }
  if (in_array($user->uid, $discourse_admin_uids)) {
    $is_admin = 'true';
  }

  // moderators
  $discourse_moderator_uids = $conf["tm_discourse_moderator_uids"]; // array(123);
  $is_moderator = 'false';
  if (in_array('moderator', array_values($user->roles))) {
    $is_moderator = 'true';
  }
  if (in_array($user->uid, $discourse_moderator_uids)) {
    $is_moderator = 'true';
  }

  // add or remove from groups
  $add_groups = array();
  $remove_groups = array();

  // approved member
  if (in_array('approved user', array_values($user->roles))) {
    $add_groups[] = 'approved-members';
  } else {
    $remove_groups[] = 'approved-members';
  }

  // chapter leaders groups
  if ((in_array('chapter leader', array_values($user->roles)))
    and (!in_array($user->uid, $conf["tm_discourse_sso_ignore_users"]))) {
    $add_groups[] = 'chapter-leaders';
  } else {
    $remove_groups[] = 'chapter-leaders';
  }

  // subscriber role
  if (in_array('subscriber', array_values($user->roles))) {
    $add_groups[] = 'subscribers';
  } else {
    $remove_groups[] = 'subscribers';
  }

  // company subscriber role
  if (module_exists("tm_subscriptions")) {
    // is user is associated with a company that has a subscription
    $subscribed_companies = tm_subscriptions_get_users_subscribed_companies($uid);
    if (sizeof($subscribed_companies) > 0) {
      $add_groups[] = 'company-subscribers';
    } else {
      $remove_groups[] = 'company-subscribers';
    }
  }

  // custom groups
  // ie: 
  // $conf["tm_discourse_sso_custom_groups"] = array("vips" => array(123));
  foreach($conf["tm_discourse_sso_custom_groups"] as $group_name => $uids) {

    // subscriber role
    if (in_array($user->uid, $uids)) {
      $add_groups[] = $group_name;
    } else {
      $remove_groups[] = $group_name;
    }

  }
  
  $add_groups_text = implode(",", $add_groups);
  $remove_groups_text = implode(",", $remove_groups);

  // supress welcome message
  $suppress_welcome_message = 'false';

  // set user parameters
  $user_params = array(
    "external_id" => $user->uid,
    "email" => $user->mail,
    "username" => $user->name, 
    "avatar_url" => $avatar_url,
    "bio" => $bio,
    "admin" => $is_admin,
    "moderator" => $is_moderator,
    "avatar_force_update" => 'true',
    "suppress_welcome_message" => $suppress_welcome_message,
    "add_groups" => $add_groups_text,
    "remove_groups" => $remove_groups_text
  );

  // add nonce
  if ($nonce != "") {
    $user_params["nonce"] = $nonce;
  }

  $user_params += discourse_sso_get_optional_user_params($user);

  return $user_params;
}

/**
 * Get optional user parameters.
 *
 * @param object $user
 *   User object with user information.
 *
 * @return array
 *   An array of optional user parameters.
 */
function discourse_sso_get_optional_user_params($user) {
  $user_params = array();

  // Load all user information and create its entity metadata wrapper.
  $user_wrapper = entity_metadata_wrapper('user', user_load($user->uid));

  // Massive Platform name
  $acccount = user_load($user->uid);
  $user_params['name'] = $acccount->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $acccount->field_user_last_name[LANGUAGE_NONE][0]['value'];

  return $user_params;
}

/**
 * Sign payload.
 *
 * @param string $payload
 *   Base64 encoded data.
 *
 * @return string
 *   Returns a payload signature.
 */
function discourse_sso_sign_payload($payload) {
  $sso_secret = discourse_sso_get_secret();

  return hash_hmac("sha256", $payload, $sso_secret);
}

/**
 * Response to the discourse site.
 *
 * @param string $payload
 *   Base64 encoded data.
 * @param string $sig
 *   Payload signature.
 */
function discourse_sso_response($payload, $sig) {
  $query = array("sso" => $payload, "sig" => $sig);
  $sso_login_url = discourse_sso_get_url() . '/session/sso_login';

  drupal_goto($sso_login_url, array('query' => $query, 'external' => TRUE));
}

<?php 

// tm_events.chatbox.inc

/* Display Suite - Render chatbox */
function _tm_event_chatbox($field, $title = NULL) {

  global $conf;
  global $user;

  // Check if we display chatbox
  $node = $field['entity'];
  if (!tm_event_chatbox_check_display($user, $node)) {
    if (isset($node->field_event_chatbox_enabled[LANGUAGE_NONE][0]['value'])) {
      if ($node->field_event_chatbox_enabled[LANGUAGE_NONE][0]['value'] == 1) {
        if (!user_is_logged_in()) {
          return tm_events_chatbox_sign_in_message($node);
        }
      }
    }
    // not displaying
    return;
  }

  // chatbox must be enabled
  if (!isset($conf["tm_chatbox_enabled"])) {
    return;
  }

  // chatbox must be enabled
  if (!($conf["tm_chatbox_enabled"])) {
    return;
  }

  // event title
  $event_title = $node->title;

  // check if event has payments
  $event_has_payments = false;
  if (module_exists("tm_payments")) {
    $event_has_payments = _tm_payments_is_payment_enabled($node);
  }

  // check for external rsvp
  $external_rsvp = false;
  if (isset($node->field_external_rsvp[LANGUAGE_NONE][0]['value'])) {
    if ($node->field_external_rsvp[LANGUAGE_NONE][0]['value'] != 0) {
      $external_rsvp = true;
    }
  }

  // if user is not registered
  $message = "";
  $message_js = "";

  if ((!tm_events_user_is_registered($node->nid, $user->uid)) and (!$external_rsvp)) {
    
    if ($event_has_payments) {
      $message = "You must be registered for the event to join the live discussion.";
    } else if (tm_events_user_is_waitlisted($node->nid, $user->uid)) {
      $message = "You must be registered for the event to join the live discussion.";
    } else if (tm_events_is_past_event($node)) {
      $message = "Only registered attendees can join the live discussion.";
    } else {
      $message = "You must be <a id='tm_chatbox_register_link' href='#'>registered for the event</a> to join the live discussion.";
      $message_js .= "<script>
      jQuery(document).ready(function() {
        
        // register
        var register_link = jQuery('.flag-event-register a.flag').attr('href');
        if (register_link != null) {
          jQuery('#tm_chatbox_register_link').attr('href', register_link);
        }

        // waitlist
        var waitlist_link = jQuery('.flag-event-waitlist a.flag').attr('href');
        if (waitlist_link != null) {
          jQuery('#tm_chatbox_register_link').attr('href', waitlist_link);
        }

      });
      </script>";
    }
  }

  // if past event, disable messages
  if (tm_events_is_past_event($node, 60)) {
    $message = "Comments are turned off";
  }

  // event moderators can always post
  if (tm_events_check_permission_edit_event($user, $node)) {
    $message = "";
  }

  // render message
  if ($message != "") {
    $message = "<div class='event_chatbox_message'>" . $message . "</div>" . "\n" . $message_js;
  }

  // live chat title
  $live_chat_title = "LIVE CHAT";
  if (tm_event_livestream_check_display($node)) {
    $live_chat_title .= " &mdash; " . strip_tags($event_title); // show event title when live streaming
  }

  // Show the chatbox
  return "<div id='event_chatbox_container' style=''><div class='chatbox_title'><h1 class='prime-title'>" . $live_chat_title . "</h1></div><div id='chatbro' style='height: 300px;'></div>" . $message . "</div>";

}


/* Render moderator chatbox page */
function tm_event_chatbox_moderator_page($nid) {

  global $user;

  // Logged in
  if (!user_is_logged_in()) {
    drupal_access_denied();
  }

  // Check valid node
  $node = node_load($nid);
  if ($node == null) {
    drupal_access_denied();
  }

  // Check if is featured member
  $is_featured_member = false;
  if (isset($node->field_event_featured_members[LANGUAGE_NONE])) {
    foreach ($node->field_event_featured_members[LANGUAGE_NONE] as $field_featured_member) {  
      if ($field_featured_member["target_id"] == $user->uid) {
        $is_featured_member = true;
      }
    }
  }

  // Must be featured member, or editor of event
  if (!($is_featured_member or tm_events_check_permission_edit_event($user, $node))) {
    drupal_access_denied();
  }

  // include chatbox js
  $account = user_load($user->uid);
  $widget_js = tm_event_chatbox_widget_js($account, $node);
  drupal_add_js($widget_js, array('type' => 'inline', 'scope' => 'footer'));
  drupal_add_css(drupal_get_path('module', 'tm_events') . '/css/tm_events_chatbox.css');
  drupal_add_css("#page-title { display: none; } ", 'inline');

  // event title
  $event_title = $node->title;

   // live chat title
  $live_chat_title = "LIVE CHAT";
  $live_chat_title .= " &mdash; <a href='/node/" . $node->nid . "'>" . strip_tags($event_title) . "</a>"; // show event title when live streaming

  // Show the chatbox
  return "<div id='event_chatbox_container'><div class='chatbox_title'><h1 class='prime-title'>" . $live_chat_title . "</h1></div><div id='chatbro' style='height: 600px;'></div></div>";
}


/* Check if we display the chatbox */
function tm_event_chatbox_check_display($account, $node) {

  global $conf;

  // check node is event
  if ($node->type != "event") {
    return false;
  }

  // check $conf["tm_chatbox_enabled"]
  if (!isset($conf["tm_chatbox_enabled"])) {
    return false;
  }

  // check $conf["tm_chatbox_enabled"]
  if (!$conf["tm_chatbox_enabled"]) {
    return false;
  }

  // Must be logged in to see chat
  if (!user_is_logged_in()) {
    return false;
  }

  // check setting
  if (!isset($node->field_event_chatbox_enabled[LANGUAGE_NONE][0]['value'])) {
    return false;
  }

  // check if not enabled
  if (!$node->field_event_chatbox_enabled[LANGUAGE_NONE][0]['value'] == 1) {
    return false;
  }

  // member is reported
  if (tm_users_is_member_reported($account->uid)) {
    return false;
  }

  return true;

}

/* Check if user a chat moderator */
function tm_event_chatbox_check_moderator($account, $node) {
  return tm_events_check_permission_edit_event($account, $node);
}

/* Include the JS */
function tm_event_chatbox_widget_js($account, $node) {

  global $conf;

  // widget
  $widget_js = "/* Chatbro Widget Embed Code Start */function ChatbroLoader(chats,async){async=!1!==async;var params={embedChatsParameters:chats instanceof Array?chats:[chats],lang:navigator.language||navigator.userLanguage,needLoadCode:'undefined'==typeof Chatbro,embedParamsVersion:localStorage.embedParamsVersion,chatbroScriptVersion:localStorage.chatbroScriptVersion},xhr=new XMLHttpRequest;xhr.withCredentials=!0,xhr.onload=function(){eval(xhr.responseText)},xhr.onerror=function(){console.error('Chatbro loading error')},xhr.open('GET','//www.chatbro.com/embed.js?'+btoa(unescape(encodeURIComponent(JSON.stringify(params)))),async),xhr.send()}/* Chatbro Widget Embed Code End */";

  // get user information
  $secret_key = $conf["tm_chatbox_secret_key"];
  $parent_chat_id = $conf["tm_chatbox_id"];
  $site_domain = $conf["tm_chatbox_site_domain"];
  $chat_title = "Event ID: " . $node->nid;
  $external_id = $account->uid;
  $name_color = $conf["tm_chatbox_name_color"];

  // name
  $full_name = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']) . " " . strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);

  // avatar
  if (isset($account->field_avatar[LANGUAGE_NONE][0]['uri'])) {
    $avatar_url = image_style_url("avatar", $account->field_avatar[LANGUAGE_NONE][0]['uri']);
  } else {
    $avatar_url = image_style_url("avatar", "public://default_images/avatar-default.png");
  }

  // profile url
  $profile_url = "__SERVER_HTTP__://__SERVER_NAME__/" . drupal_get_path_alias("user/" . $account->uid);
  $profile_url = str_replace("__SERVER_NAME__", $conf["tm_email_server_url_domain"], $profile_url); 
  $profile_url = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $profile_url);  

  // allow event editors to moderate chat
  $permissions = array();
  $user_is_event_moderator = false;
  if (tm_events_check_permission_edit_event($account, $node)) {
    $user_is_event_moderator = true;
    $permissions = array("delete", "ban");
  }

  // user must be registered for event to chat
  $allow_send_message = "false";
  $is_registered = tm_events_user_is_registered($node->nid, $account->uid);
  if (($is_registered) or ($user_is_event_moderator)) {
    $allow_send_message = "true";
  }

  // external rsvp allow all members to chat
  if (isset($node->field_external_rsvp[LANGUAGE_NONE][0]['value'])) {
    if ($node->field_external_rsvp[LANGUAGE_NONE][0]['value'] != 0) {
      $external_rsvp = true;
      $allow_send_message = "true";
    }
  }

  // disable comments after event ends, grace period 60 minutes
  if (tm_events_is_past_event($node, 60) and (!$user_is_event_moderator)) {
    $allow_send_message = "false";
  }

  // calculate signature
  // see: http://sso.chatbro.com/
  $signature = md5($site_domain . $external_id . $full_name . $avatar_url . $profile_url . $name_color . implode("", $permissions) . $secret_key);

  // configure widget
  $widget_js .= "
ChatbroLoader({
parentEncodedChatId: '" . $parent_chat_id . "',
chatTitle: '" . $chat_title . "',
siteDomain: '" . $site_domain . "',
siteUserExternalId: '" . $external_id . "',
siteUserFullName: '" . str_replace("'", "\'", $full_name) . "',
siteUserAvatarUrl: '" . str_replace("'", "\'", $avatar_url) . "',
siteUserProfileUrl: '" . str_replace("'", "\'", $profile_url) . "',
showChatMenu: false,
allowUploadFile: false,
avatarBorderRadius: '16px',
allowSendMessages: " . $allow_send_message . ",
siteUserFullNameColor: '" . $name_color . "',
permissions: " . json_encode($permissions) . ",
signature: '" . $signature . "'
});";

  return $widget_js;
}

/* Render a sign in message for logged out users */
function tm_events_chatbox_sign_in_message($node) {

  $event_title = $node->title;

  // live chat title
  $live_chat_title = "LIVE CHAT";
  if (tm_event_livestream_check_display($node)) {
    $live_chat_title .= " &mdash; " . strip_tags($event_title); // show event title when live streaming
  }

  $html = "<div id='event_chatbox_container'><div class='chatbox_title'><h1 class='prime-title'>" . $live_chat_title . "</h1></div><div class='event_chatbox_message'>You must be <a href='/user/login?destination=/node/" . $node->nid . "'>signed in</a> to view live discussion.</div></div>";

  return $html;
}

<?php

// tm_chapters.moderation.inc- chapter leaders moderation methods

/**
 * Modify users account content before being displayed
 */
function tm_chapters_user_view($account, $view_mode, $langcode) {

  global $user;

  // Add moderation tools for moderator and chapter leader 
  if ($view_mode == "full") {
  	$account = user_load($user->uid);
	if ((in_array("chapter leader", $account->roles) or (in_array("moderator", $account->roles)))) {
		drupal_add_js(drupal_get_path('module', 'tm_chapters') . '/js/tm_chapters_moderation.js');
	}
  }
}

/**
 * Add member to a chapter callback
 * examples:
 * chapters/moderation-chapter-add-member/10507/13737?message=welcome
 */
function tm_chapters_moderation_chapters_add_member_callback() {

	global $user;
	$moderator = user_load($user->uid);

	// user to moderate
	$user_to_moderate = user_load(arg(2));

	// chapter to add
	$add_to_chapter = node_load(arg(3));

	tm_chapters_moderation_chapters_add_member($user_to_moderate, $add_to_chapter, $moderator);
}

/**
 * Remove member to a chapter
 * examples:
 * chapters/moderation-chapter-remove-member/10507/13737
 */
function tm_chapters_moderation_chapters_remove_member_callback() {

	global $user;
	$moderator = user_load($user->uid);

	// user to moderate
	$user_to_moderate = user_load(arg(2));

	// chapter to remove
	$remove_from_chapter = node_load(arg(3));

	tm_chapters_moderation_chapters_remove_member($user_to_moderate, $remove_from_chapter, $moderator);
}

/**
 * Add member to a chapter
 */
function tm_chapters_moderation_chapters_add_member($user_to_moderate, $add_to_chapter, $moderator, $is_console = false) {

	// check permissions
	if ((!in_array("chapter leader", $moderator->roles) and (!in_array("moderator", $moderator->roles)))) {
		if ($is_console) { return false; }
		drupal_set_message(t('You don\'t have permission to peform this action.'), 'error');
		drupal_goto("/");
	}

	// check user and chapter
	if (($user_to_moderate == null) or ($add_to_chapter == null)) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (not a valid selection).'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// check user not in chapter already
	// get list of chapters user is leading
	$members_chapters = tm_users_get_chapters($user_to_moderate);
	if (in_array($add_to_chapter->nid, $members_chapters)) {
		if ($is_console) { return false; }
		drupal_set_message(t('This member is already a member of ' . tm_chapters_formal_name($add_to_chapter) . '.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// user must be chapter leader, community manager, or moderator of the chapter
	if ((!tm_users_is_chapter_leader($moderator->uid, $add_to_chapter->nid, $check_community_manager = true)) and (!in_array("moderator", $moderator->roles))) {
		if ($is_console) { return false; }
		drupal_set_message(t('You don\'t have permission to moderate this member.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// add message into member welcome email
	// set static variables for tm_notifications_chapter_flag_flag():
	// tm_add_member_moderator_first_name
	// tm_add_member_moderator_last_name
	// tm_add_moderator_member_message

	// moderator first name
	$moderator_first_name = strip_tags($moderator->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$static_add_member_moderator_first_name = &drupal_static("tm_add_member_moderator_first_name");
	$static_add_member_moderator_first_name = $moderator_first_name;

	// moderator last name
	$moderator_last_name = strip_tags($moderator->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$static_add_member_moderator_last_name = &drupal_static("tm_add_member_moderator_last_name");
	$static_add_member_moderator_last_name = $moderator_last_name;

	if (isset($_GET["message"])) {
		$message = strip_tags(trim($_GET["message"]));
		if ($message != "") {
			// moderator message
			$message_html = "Message from " . $moderator_first_name . ":<p>";
			$message_html .= $message;
			$message_html .= "</p>";

			$static_add_member_moderator_message = &drupal_static("tm_add_member_moderator_message");
      		$static_add_member_moderator_message = $message_html;
		}
	}

	// get full name
	$first_name = strip_tags($user_to_moderate->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$last_name = strip_tags($user_to_moderate->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$full_name = $first_name . " " . $last_name;

	// flag member
	flag('unflag', 'remove_member_from_chapter', $add_to_chapter->nid, $user_to_moderate);
	flag('unflag', 'add_member_to_chapter', $add_to_chapter->nid, $user_to_moderate); 
	flag('flag', 'add_member_to_chapter', $add_to_chapter->nid, $user_to_moderate); 

	// log in watchdog
	watchdog('tm_chapters', 'Member ' . $full_name . ' (uid: ' . $user_to_moderate->uid . ') added to chapter ' . $add_to_chapter->title . ' (nid: ' . $add_to_chapter->nid . ') by ' . $moderator_first_name . ' (uid: ' . $moderator->uid . ')');

	// join chapter
    $signup_flag = flag_get_flag('signup');
    if (!$signup_flag->is_flagged($add_to_chapter->nid, $user_to_moderate->uid)) {
      flag('flag', 'signup', $add_to_chapter->nid, $user_to_moderate);
    }

    // console return
    if ($is_console) { return true; }
    
	// notify moderator and redirect to members 
	$chapter_url = drupal_get_path_alias("node/" . $add_to_chapter->nid);
    drupal_set_message('You added ' . $full_name . ' to <a href="/' . $chapter_url . '">' .tm_chapters_formal_name($add_to_chapter) . '</a>.');
    drupal_goto("user/" . $user_to_moderate->uid);
}

/**
 * Remove member to a chapter
 */
function tm_chapters_moderation_chapters_remove_member($user_to_moderate, $remove_from_chapter, $moderator, $is_console = false) {

	// check permissions
	if ((!in_array("chapter leader", $moderator->roles) and (!in_array("moderator", $moderator->roles)))) {
		if ($is_console) { return false; }
		drupal_set_message(t('You don\'t have permission to peform this action.'), 'error');
		drupal_goto("/");
	}

	// check user and chapter
	if (($user_to_moderate == null) or ($remove_from_chapter == null)) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (not a valid selection).'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// check user not chapter leader or moderator
	if ((in_array("chapter leader", $user_to_moderate->roles) or (in_array("moderator", $user_to_moderate->roles)))) {
		if ($is_console) { return false; }
		drupal_set_message(t('You can\'t remove a chapter leader or moderator from a chapter.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// check user not in chapter
	// get list of chapters user is leading
	$members_chapters = tm_users_get_chapters($user_to_moderate);
	if (!in_array($remove_from_chapter->nid, $members_chapters)) {
		if ($is_console) { return false; }
		drupal_set_message(t('This member is not a member of ' . tm_chapters_formal_name($remove_from_chapter) . '.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// user must be chapter leader, community manager, or moderator of the chapter
	if ((!tm_users_is_chapter_leader($moderator->uid, $remove_from_chapter->nid, $check_community_manager = true)) and (!in_array("moderator", $moderator->roles))) {
		if ($is_console) { return false; }
		drupal_set_message(t('You don\'t have permission to moderate this member.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// moderator first name
	$moderator_first_name = strip_tags($moderator->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$static_remove_member_moderator_first_name = &drupal_static("tm_remove_member_moderator_first_name");
	$static_remove_member_moderator_first_name = $moderator_first_name;

	// moderator last name
	$moderator_last_name = strip_tags($moderator->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$static_remove_member_moderator_last_name = &drupal_static("tm_remove_member_moderator_last_name");
	$static_remove_member_moderator_last_name = $moderator_last_name;

	// get full name
	$first_name = strip_tags($user_to_moderate->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$last_name = strip_tags($user_to_moderate->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$full_name = $first_name . " " . $last_name;

	// flag member
	flag('unflag', 'remove_member_from_chapter', $remove_from_chapter->nid, $user_to_moderate);
	flag('unflag', 'add_member_to_chapter', $remove_from_chapter->nid, $user_to_moderate); 
	flag('flag', 'remove_member_from_chapter', $remove_from_chapter->nid, $user_to_moderate); 

	// log in watchdog
	watchdog('tm_chapters', 'Member ' . $full_name . ' (uid: ' . $user_to_moderate->uid . ') removed from chapter ' . $remove_from_chapter->title . ' (nid: ' . $remove_from_chapter->nid . ') by ' . $moderator_first_name . ' (uid: ' . $moderator->uid . ')');

	// remove from chapter
    $signup_flag = flag_get_flag('signup');
    if ($signup_flag->is_flagged($remove_from_chapter->nid, $user_to_moderate->uid)) {
      flag('unflag', 'signup', $remove_from_chapter->nid, $user_to_moderate);
    }
    
    // console return
    if ($is_console) { return true; }

	// notify moderator and redirect to members 
	$chapter_url = drupal_get_path_alias("node/" . $remove_from_chapter->nid);
    drupal_set_message('You removed ' . $full_name . ' from <a href="/' . $chapter_url . '">' .tm_chapters_formal_name($remove_from_chapter) . '</a>.');
    drupal_goto("user/" . $user_to_moderate->uid);
}

/**
 * Implements hook_flag_validate()
 * Check if user can join chapter
 */
function tm_chapters_flag_validate($action, $flag, $entity_id, $account, $skip_permission_check, $flagging) {

	// skip if permission check set
	if ($skip_permission_check) {
		return;
	}

	// check flag signup
	if (($action == 'flag') and ($flag->name == 'signup')) {

		// load flag
		$remove_member_from_chapter_flag = flag_get_flag('remove_member_from_chapter');

		// check if member is removed from chapter
		if ($remove_member_from_chapter_flag->is_flagged($entity_id, $account->uid)) {
			drupal_set_message("Sorry, you are unable to join this chapter.", "warning");
			return(array('access-denied' => t('Sorry, you are unable to join this chapter.')));
		}
	}

	// check event register
	if (($action == 'flag') and (($flag->name == 'event_register') or ($flag->name == 'event_waitlist'))) {

		// load flag
		$remove_member_from_chapter_flag = flag_get_flag('remove_member_from_chapter');

		// get chapter of event
		$entity = node_load($entity_id);
		$chapter_id = _tm_event_get_chapter_id($entity);
	
		if ($chapter_id != null) {
			if ($remove_member_from_chapter_flag->is_flagged($chapter_id, $account->uid)) {
				drupal_set_message("Sorry, you are unable to register for this event.", "warning");
				drupal_goto("node/" . $entity_id);
			}
		}
	}
}

/**
 * Implementation hook tm_flags_preprocess_flag
 * Prevent joining chapter if member has been removed from it
 */
function tm_chapters_preprocess_flag(&$variables) {

	$flag = &$variables['flag'];
	$action = $variables['action'];
	$entity_id = $variables['entity_id'];
	$flag_css_name = str_replace('_', '-', $flag->name);
	$uid = $variables['user']->uid;

	// check for signup flag
	if (($flag->name == "signup") && ($action == "flag")) {

		// load removed from chapter flag
		$remove_member_from_chapter_flag = flag_get_flag('remove_member_from_chapter');

		// check if member is removed the chapter
		if ($remove_member_from_chapter_flag->is_flagged($entity_id, $uid)) {

			global $conf;
			$variables['flag_button_class'] = 'flag-limit follow bttn bttn-secondary bttn-m';
			drupal_add_js('
			jQuery(document).ready(function($) {
				$( ".flag" ).unbind("click");
				$( ".flag" ).bind("click.confirm", function(event) {
					jq_alert(null, "Sorry, you are unable to join this chapter at this time.<br>Please visit our <a href=\'' . $conf["tm_community_values_url"] . '\'>Community Guidelines</a> for assistance.");
					event.preventDefault();
				});
			});
			', 'inline');
			return true;
		}
	}
}

/**
 * Render html options list for chapter moderation
 * examples:
 * /chapters/moderation-chapter-list-ajax/10507/add
 * /chapters/moderation-chapter-list-ajax/10507/remove
 */
function tm_chapters_moderation_chapters_list_ajax() {

	global $user;
	$account = user_load($user->uid);

	// user to moderate
	$user_to_moderate = user_load(arg(2));

	// moderation_mode - add or remove
	$moderation_mode = arg(3);

	// get list of chapters user is leading
	$chapters = tm_users_get_chapters_leading($user);

	// if user is moderator
	if (in_array("moderator", $account->roles)) {
		$chapters = tm_chapters_get_all_chapters();
	}
	
	// get list of chapters user has joined
	$joined_chapters = array();
	if ($user_to_moderate != null) {
		$joined_chapters = tm_users_get_chapters($user_to_moderate);
	}
	
	// calculate chapters to display
	$display_chapters = array();
	$disabled_chapters = array();
	foreach ($chapters as $nid) {

		// show chapters that member has not joined
		if ($moderation_mode == "add") {
			if (!in_array($nid, $joined_chapters)) {
				$display_chapters[] = $nid;
			} else {
				$disabled_chapters[] = $nid;
			}
		}

		// show chapters that member has joined
		if ($moderation_mode == "remove") {
			if (in_array($nid, $joined_chapters)) {
				$display_chapters[] = $nid;
			} else {
				$disabled_chapters[] = $nid;
			}
		}

	}

	// no results
	if (count($display_chapters) == 0) {
		$html = "No available chapters";
		print $html;
		return;
	}

	// generate html
	$html = "<select id='moderation_chapter_ids'>";
	foreach ($display_chapters as $nid) {
		$chapter = node_load($nid);
		$html .= "<option value='" . $nid . "'>" . strip_tags($chapter->title) . "</option>\n";
	}

	// show all available chapters when adding	
	if ($moderation_mode == "add") {
		$disabled_label = "(already a member)";
		foreach ($disabled_chapters as $nid) {
			$chapter = node_load($nid);
			$html .= "<option value='" . $nid . "' disabled>" . strip_tags($chapter->title) . " " . $disabled_label . "</option>\n";
		}
	}
	
	$html .= "</select>";
	print $html;
	
}

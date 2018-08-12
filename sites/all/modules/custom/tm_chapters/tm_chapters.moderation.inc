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
 * Add member to a chapter
 * examples:
 * chapters/moderation-chapter-add-member/10507/13737?message=welcome
 */
function tm_chapters_moderation_chapters_add_member() {

	global $user;
	$moderator = user_load($user->uid);

	// check permissions
	if ((!in_array("chapter leader", $moderator->roles) and (!in_array("moderator", $moderator->roles)))) {
		drupal_set_message(t('You don\'t have permission to peform this action.'), 'error');
		drupal_goto("/");
	}

	// user to moderate
	$user_to_moderate = user_load(arg(2));

	// chapter to add
	$add_to_chapter = node_load(arg(3));

	// check user and chapter
	if (($user_to_moderate == null) or ($add_to_chapter == null)) {
		drupal_set_message(t('Oops, an error occurred performing the action (invalid parameters).'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
		return;
	}

	// check user not in chapter already
	// get list of chapters user is leading
	$members_chapters = tm_users_get_chapters_leading($user_to_moderate);
	if (in_array($add_to_chapter->nid, $members_chapters)) {
		drupal_set_message(t('This member is already a member of ' . tm_chapters_formal_name($add_to_chapter) . '.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
		return;
	}

	// user must be chapter leader, community manager, or moderator of the chapter
	if ((!tm_users_is_chapter_leader($moderator->uid, $add_to_chapter->nid, $check_community_manager = true)) and (!in_array("moderator", $moderator->roles))) {
		drupal_set_message(t('You don\'t have permission to moderate this member.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
		return;
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
			$message_html = "Message from " . $moderator_first_name . ":<br>";
			$message_html .= $message;
			$message_html .= "<br><br>";

			$static_add_member_moderator_message = &drupal_static("tm_add_member_moderator_message");
      		$static_add_member_moderator_message = $message_html;
		}
	}

	// get full name
	$first_name = strip_tags($user_to_moderate->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$last_name = strip_tags($user_to_moderate->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$full_name = $first_name . " " . $last_name;

	// flag member
	flag('unflag', 'remove_member_to_chapter', $add_to_chapter->nid, $user_to_moderate);
	flag('unflag', 'add_member_to_chapter', $add_to_chapter->nid, $user_to_moderate); 
	flag('flag', 'add_member_to_chapter', $add_to_chapter->nid, $user_to_moderate); 

	// log in watchdog
	watchdog('tm_chapters', 'Member ' . $full_name . ' (uid: ' . $user_to_moderate->uid . ') added to chapter ' . $add_to_chapter->title . ' (nid: ' . $add_to_chapter->nid . ') by ' . $moderator_first_name . ' (uid: ' . $moderator->uid . ')');

	// join chapter
    $signup_flag = flag_get_flag('signup');
    if (!$signup_flag->is_flagged($add_to_chapter->nid, $user_to_moderate->uid)) {
      flag('flag', 'signup', $add_to_chapter->nid, $user_to_moderate);
    }

    
	// notify moderator and redirect to members 
	$chapter_url = drupal_get_path_alias("node/" . $add_to_chapter->nid);
    drupal_set_message('You added ' . $full_name . ' to <a href="/' . $chapter_url . '">' .tm_chapters_formal_name($add_to_chapter) . '</a>.');
    drupal_goto("user/" . $user_to_moderate->uid);
	return;
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
	$disabled_label = "(already a member)";
	if ($moderation_mode == "remove") {
		$disabled_label = "(not a member)";
	}
	foreach ($disabled_chapters as $nid) {
		$chapter = node_load($nid);
		$html .= "<option value='" . $nid . "' disabled>" . strip_tags($chapter->title) . " " . $disabled_label . "</option>\n";
	}
	$html .= "</select>";
	print $html;
	
}

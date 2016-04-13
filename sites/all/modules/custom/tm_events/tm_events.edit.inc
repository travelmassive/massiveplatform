<?php

// tm_events.edit.inc - edit and create form methods

/**
 * Hook - modify event edit form
 * Note: create and update permission granted by tm_events_node_access
 */
function tm_events_form_event_node_form_alter(&$form, &$form_state) {

  global $user;
  global $conf;

  $typeof_event = null; // member_event, chapter_event, or company_event
  $is_new_event = false; // if this is a new node or not

  // Permission variables
  $perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));
  $perm_is_chapter_leader = (in_array("chapter leader", $user->roles));

  // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_events') . '/js/tm_events_edit_event.js');

  // STEP 1. Determine if event is new or editing existing
  //////////
  $event = $form_state['node'];
  $is_new_event = (!isset($event->nid) || isset($event->is_new));

  // STEP 2. Fetch event_type and set chapter_id, company_id
  //////////

  if ($is_new_event) {

    if (isset($_GET["field_chapter"])) {
      $typeof_event = "chapter_event";
      $chapter_id = $_GET["field_chapter"];
    } elseif (isset($_GET["field_event_company"])) {
      $typeof_event = "company_event";
      $company_id = $_GET["field_event_company"];
    } else {
      $typeof_event = "member_event";
    }
  }

  // Existing event
  if (!$is_new_event) {

    if (isset($event->field_chapter[LANGUAGE_NONE][0])) {
      $typeof_event = "chapter_event";
      $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
    } elseif (isset($event->field_event_company[LANGUAGE_NONE][0])){
      $typeof_event = "company_event";
      $company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
    } else {
      $typeof_event = "member_event";
    }
  } 

  // Step 3. Modify form
  //////////

  if ($is_new_event) {
  	drupal_add_js(array('tm_events' => array('form_subtitle' => 'New Event')), array('type' => 'setting'));
  }

  // CHAPTER EVENT
  if ($typeof_event == "chapter_event") {

  	// Set heading
		if ($is_new_event) {
			drupal_add_js(array('tm_events' => array('form_heading' => 'Chapter Event')), array('type' => 'setting'));
		}

		// Set tips
		if (isset($conf['tm_event_types_edit_tips']['community'])) {
			drupal_get_messages("help-tip");
			drupal_set_message($conf['tm_event_types_edit_tips']['community'], "help-tip");
		}

  	// Set organizers
		$opts = array('_none' => t('- None -'))  + _load_organizers_per_chapter($chapter_id);
		foreach ($opts as $uid => $name) {
			if ($uid != "_none") {
				$loaded = user_load($uid);
				$option_label = $loaded->field_user_first_name[LANGUAGE_NONE][0]['safe_value'] . " " . $loaded->field_user_last_name[LANGUAGE_NONE][0]['safe_value'] . " (" . $uid . ")";
				$opts[$uid] = $option_label;
			}
		}
		$form['field_organizer'][LANGUAGE_NONE]['#options'] = $opts;

		// Disable fields
  	$form['field_chapter']['#disabled'] = true;
  	$form['field_event_company']['#disabled'] = true;

  	// Hide fields
  	$form['field_event_company']['#access'] = false;

    // Event timezone
    if (!$perm_is_moderator_or_admin) {
      $form['field_event_timezone']['#access'] = FALSE;
    }

  } 

  // COMPANY EVENT
  if ($typeof_event == "company_event") {

  	// Set heading
		if ($is_new_event) {
			drupal_add_js(array('tm_events' => array('form_heading' => 'Company Event')), array('type' => 'setting'));
		}

		// Set tips
		if (isset($conf['tm_event_types_edit_tips']['company'])) {
			drupal_get_messages("help-tip");
			drupal_set_message($conf['tm_event_types_edit_tips']['company'], "help-tip");
		}

		// Set organizers
    $company = node_load($company_id);
    $organizer_opts =array('_none' => t('- None -'));
    foreach ($company->field_owner[LANGUAGE_NONE] as $owner) {
      $loaded = user_load($owner['target_id']);
      $organizer_opts[$loaded->uid] = tm_events_username($loaded) . " (" . $loaded->uid . ")";
    }
    $form['field_organizer'][LANGUAGE_NONE]['#options'] = $organizer_opts;

    // Set timezone
		if (($is_new_event) && ($user->timezone != null)) {
			$form['field_event_timezone'][LANGUAGE_NONE]['#default_value'] = $user->timezone;
		}
		$form['field_event_timezone'][LANGUAGE_NONE]['#description'] = 'Set the timezone where your event is happening.';

		// Disable fields
  	$form['field_chapter']['#disabled'] = true;
  	$form['field_event_company']['#disabled'] = true;

		// Hide fields
    $form['field_event_hide_organizers']['#access'] = false;
    $form['field_chapter']['#access'] = false;
    $form['field_event_hide_location']['#access'] = false;

		// Hide fields for non-moderators
    if (!$perm_is_moderator_or_admin) {
      field_group_hide_field_groups($form, array('group_sponsors'));
      field_group_hide_field_groups($form, array('group_featured_members'));
      field_group_hide_field_groups($form, array('group_extra_stuff'));
    }

  }

  // MEMBER EVENT
  if ($typeof_event == "member_event") {

  	// Set heading
		if ($is_new_event) {
			drupal_add_js(array('tm_events' => array('form_heading' => 'Member Event')), array('type' => 'setting'));
		}

		// Set tips
		if (isset($conf['tm_event_types_edit_tips']['member'])) {
			drupal_get_messages("help-tip");
			drupal_set_message($conf['tm_event_types_edit_tips']['member'], "help-tip");
		}

		// Set timezone
		if (($is_new_event) && ($user->timezone != null)) {
			$form['field_event_timezone'][LANGUAGE_NONE]['#default_value'] = $user->timezone;
		}
		$form['field_event_timezone'][LANGUAGE_NONE]['#description'] = 'Set the timezone where your event is happening.';

		// Disable fields
  	$form['field_chapter']['#disabled'] = true;
  	$form['field_event_company']['#disabled'] = true;

		// Hide fields
		$form['field_chapter']['#access'] = false;
    $form['field_event_company']['#access'] = false;
    $form['field_organizer']['#access'] = false;
    $form['field_event_hide_organizers']['#access'] = false;
    $form['field_event_hide_location']['#access'] = false;

    // Hide fields for non-moderators
    if (!$perm_is_moderator_or_admin) { 
      field_group_hide_field_groups($form, array('group_sponsors'));
      field_group_hide_field_groups($form, array('group_featured_members'));
      field_group_hide_field_groups($form, array('group_extra_stuff'));
    }

  }

  // Restrict these fields to moderator and administrator
  if (!$perm_is_moderator_or_admin) {

    // Event type
    $form['field_event_type']['#access'] = false;

    // Change owner
    $form['field_event_reassign_owner']['#access'] = false;
    
    // Publish to event chapters
    field_group_hide_field_groups($form, array('group_event_chapters'));

    // Notify featured members of event registrations
    $form['field_notify_featured_members']['#access'] = false;

    // Allow featured members to download rsvp list
    $form['field_event_featured_access_rsvp']['#access'] = false;

    // Mark as partner event
    $form['field_is_partner_event']['#access'] = false;

    // Hide organizers
    $form['field_event_hide_organizers']['#access'] = false;

    // Hide moderator settings
    field_group_hide_field_groups($form, array('group_moderator_fields'));

  }

  // Always show these fields to moderator or administrator
  if ($perm_is_moderator_or_admin) {

  	// Allow move to chapter or event
  	// Note: admin must transfer ownership for member events
  	$form['field_chapter']['#access'] = true;
  	$form['field_chapter']['#disabled'] = false;
  	$form['#group_children']['field_chapter'] = 'group_moderator_fields';
  	
    $form['field_event_company']['#access'] = true;
    $form['field_event_company']['#disabled'] = false;
    $form['#group_children']['field_event_company'] = 'group_moderator_fields';

    drupal_add_js('jQuery(document).ready(function () { jQuery("#tm-event-edit-company-description").html("The company listing this event.<br><b>Listing a sponsor? See the <i>Event Sponsors</i> section."); });', 'inline');

  }

	// TOGGLE SIGNUP LIMIT OR RSVP LINK FIELD DISPLAY
	$form['field_signup_limit']['#states'] = array(
	'visible' => array(
	 ':input[name="field_external_rsvp[und]"]' => array('value' => '0'),
	),
	);
	$form['field_rsvp_link']['#states'] = array(
	'visible' => array(
	 ':input[name="field_external_rsvp[und]"]' => array('value' => '1'),
	),
	);

	// POPULATE AND SET EVENT TYPES
	$event_types = array();
	$default_event_type = "";

	if (isset($conf["tm_event_types"])) {
		$event_types = $conf["tm_event_types"];
	}

	if (isset($conf["tm_event_types_default"])) {
		$default_event_type = $conf["tm_event_types_default"];
	}

	if ($typeof_event == "chapter_event") {
		$default_event_type = "community";
	}
	if ($typeof_event == "company_event") {
		$default_event_type = "company";
	}
	if ($typeof_event == "member_event") {
		$default_event_type = "member";
	}

  $form['field_event_type'][LANGUAGE_NONE]['#options'] = $event_types;

  // if event type saved, select it in the dropdown
  $event = $form['field_event_type'][LANGUAGE_NONE]['#entity'];
  if (isset($event->field_event_type[LANGUAGE_NONE][0]['value'])) {
    $default_event_type = $event->field_event_type[LANGUAGE_NONE][0]['value'];
  }
  $form['field_event_type'][LANGUAGE_NONE]['#default_value'] = $default_event_type;
  
  // If event has attendees, show remove event message
  if (!in_array('administrator', array_values($user->roles))) {
		if (!$is_new_event) {
			$flag_register = flag_get_flag('event_register');
			$flag_waitlist = flag_get_flag('event_waitlist');
			$num_registered = $flag_register->get_count($event->nid);
			$num_waitlisted = $flag_waitlist->get_count($event->nid);

			if (($num_registered + $num_waitlisted) > 0) {
				$form['cancel_event_link'] = array('#markup' => '<div style="font-size:smaller; color: #79828c; margin-top: 1rem; text-align: center;">Note: You cannot remove this event as it has registered attendees.</div>', '#weight' => 100);
			}
  	}
  }
  

  // STEP 6. Modify payment fields if tm_payments enabled
  //////////
  /*if (module_exists("tm_payments")) {
    _tm_payments_form_event_node_form_alter($form, $form_state);
  }*/

}

/**
 * Hook - modify event before saving
 */
function tm_events_node_presave($node) {

	global $user;

  // make sure we are working on an event node
  if ($node->type != "event") { return; }

  // Moderators can reassign the owner of the event
  // This will change the owner of the node
  $perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));
	if ($perm_is_moderator_or_admin) { 
		if (isset($node->field_event_reassign_owner[LANGUAGE_NONE][0])) {
			$new_owner_uid = $node->field_event_reassign_owner[LANGUAGE_NONE][0]['target_id'];
			if ($new_owner_uid != $user->uid) {
				$node->uid = $new_owner_uid;
			}
		}
	}

	// Set the field_event_reassign_owner with the node uid
	$node->field_event_reassign_owner[LANGUAGE_NONE][0]['target_id'] = $node->uid;


  // If field_chapter is set, unset field_event_company
	if (isset($node->field_chapter[LANGUAGE_NONE][0])) {
  	$node->field_event_company[LANGUAGE_NONE] = array();
  }

  // If member event (no chapter or company) then set organizer as owner
  if (!isset($node->field_chapter[LANGUAGE_NONE][0]) && !isset($node->field_event_company[LANGUAGE_NONE][0])) {
  	$node->field_organizer[LANGUAGE_NONE][0]['target_id'] = $node->uid;
  }

  // Remove signup limit if field_external_rsvp is set to external
  if (isset($node->field_external_rsvp[LANGUAGE_NONE][0]['value'])) {
    $external_rsvp = $node->field_external_rsvp[LANGUAGE_NONE][0]['value'];
    if ($external_rsvp !== '0') {
      $node->field_signup_limit[LANGUAGE_NONE] = array();
    }
  }

  // Remove rsvp link if field_external_rsvp is set to manage rsvps
  if (isset($node->field_external_rsvp[LANGUAGE_NONE][0]['value'])) {
    $external_rsvp = $node->field_external_rsvp[LANGUAGE_NONE][0]['value'];
    if ($external_rsvp !== '1') {
      $node->field_rsvp_link[LANGUAGE_NONE] = array();
    }
  }

  // If sponsors are set, flag them
  // IC note:
  // Flag module (7.x-3.5) sets uid 0 for global flags.
  // So we use this patch from https://www.drupal.org/node/2087797 to set a uid
  // Patch file (flag_record_uid_2087797_28.patch) is in flags module directory
  if (isset($node->field_event_sponsors[LANGUAGE_NONE][0])) {
    $flag = flag_get_flag('tm_sponsor');
    foreach ($node->field_event_sponsors[LANGUAGE_NONE] as $arr) {
      $flag->flag("flag", $arr['target_id']);        
    }
  }

  // If featured members are set, flag them
  // Note global flag comment above
  if (isset($node->field_event_featured_members[LANGUAGE_NONE][0])) {
    $flag = flag_get_flag('featured_member');
    foreach ($node->field_event_featured_members[LANGUAGE_NONE] as $arr) {
      $flag->flag("flag", $arr['target_id']);        
    }
  }

  // Always enable pathautho for events
  // This is so that when moderator saves event, the path is updated
  // http://drupal.stackexchange.com/questions/22983/how-can-i-enable-automatic-path-creating-for-all-nodes
  $node->path['pathauto']=1;

}
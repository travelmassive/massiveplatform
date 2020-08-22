<?php

// tm_events.edit.inc - edit and create form methods

/**
 * Hook - modify event edit form
 * Note: create and update permission granted by tm_events_node_access
 */
function tm_events_form_event_node_form_alter(&$form, &$form_state, $form_id) {
  
  global $user;
  global $conf;

  $typeof_event = null; // member_event, chapter_event, or company_event
  $is_new_event = false; // if this is a new node or not

  // Permission variables
  $perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));
  $perm_is_chapter_leader = (in_array("chapter leader", $user->roles));

  // Allow to hide moderator options
  if (isset($_GET["hide_moderator_options"])) {
    $perm_is_moderator_or_admin = false;
  }

  // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_events') . '/js/tm_events_edit_event.js');
  drupal_add_js(drupal_get_path('module', 'tm_events') . '/js/tm_events_sync_end_date.js');

  // Hide "show row weights"
  drupal_add_css(".tabledrag-toggle-weight { display: none; }", "inline");

  // Detect ajax request (show sponsors and speakers)
  $ajax_request = false;
  if (strpos(current_path(), 'ajax') !== false) {
      $ajax_request = true;
  }

  // STEP 1. Determine if event is new or editing existing
  //////////
  $event = $form_state['node'];
  $is_new_event = (!isset($event->nid) || isset($event->is_new));

  // STEP 2. Fetch event_type and set chapter_id, company_id
  //////////

  // new event from form submit
  if ($is_new_event && !$ajax_request) {

    if (isset($_GET["field_chapter"])) {
      $typeof_event = "chapter_event";
      $chapter_id = $_GET["field_chapter"];
    } elseif (isset($_GET["field_event_company"])) {
      $typeof_event = "company_event";
      $company_id = $_GET["field_event_company"];
    } else {
      $typeof_event = "member_event";
    }
    $event_owner_uid = $user->uid;
  }


  // new event via ajax handler (ie: user uploads image)
  if ($is_new_event && $ajax_request) {

    // parse referer string into an array similar to _GET
    $ajax_query_get = array();
    if (isset($_SERVER["HTTP_REFERER"])) {
      $ajax_request_parts = explode("?", $_SERVER["HTTP_REFERER"]);
      if (sizeof($ajax_request_parts) > 1) {
        $ajax_query_string = $ajax_request_parts[1]; // field_event_company=13739&opt=2
        $ajax_query_parts = explode("&", $ajax_query_string);
        foreach ($ajax_query_parts as $ajax_query_part) {
          $ajax_query_key_value = explode("=", $ajax_query_part);
          if (sizeof($ajax_query_key_value) > 1) {
            $ajax_query_get[$ajax_query_key_value[0]] = $ajax_query_key_value[1];
          } else {
            $ajax_query_get[$ajax_query_key_value[0]] = null;
          }
        }
      }
    }

    if (isset($ajax_query_get["field_chapter"])) {
      $typeof_event = "chapter_event";
      $chapter_id = $ajax_query_get["field_chapter"];
    } elseif (isset($ajax_query_get["field_event_company"])) {
      $typeof_event = "company_event";
      $company_id = $ajax_query_get["field_event_company"];
    } else {
      $typeof_event = "member_event";
    }
    $event_owner_uid = $user->uid;
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
    $event_owner_uid = $event->uid;
  }

  // Title placeholder text
  if (isset($conf["tm_event_title_placeholder_text"])) {
    $form['title']['#attributes']['placeholder'] = $conf["tm_event_title_placeholder_text"];
  }

  // Description placeholder text
  if (isset($conf["tm_event_description_placeholder_text"])) {
    $form['body'][LANGUAGE_NONE]['0']['value']['#attributes']['placeholder'] = $conf["tm_event_description_placeholder_text"];
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
    if (!$ajax_request) {
      if (isset($conf['tm_event_types_edit_tips']['community'])) {
        drupal_get_messages("help-tip");
        drupal_set_message($conf['tm_event_types_edit_tips']['community'], "help-tip");
      }
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
    $form['field_organizer'][LANGUAGE_NONE]['#multiple'] = true; // multiple select

    // Select existing organizers if existing event
    if (!$is_new_event) {
      $existing_organizers = array();
      if (sizeof($event->field_organizer) > 0) {
        foreach ($event->field_organizer['und'] as $organizer) {
          $existing_organizers[] = $organizer['target_id'];
        }
      }
      $form['field_organizer'][LANGUAGE_NONE]['#default_value'] = $existing_organizers;
    }

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
    if (!$ajax_request) {
  		if (isset($conf['tm_event_types_edit_tips']['company'])) {
  			drupal_get_messages("help-tip");
  			drupal_set_message($conf['tm_event_types_edit_tips']['company'], "help-tip");
  		}
    }

		// Set organizers
    $company = node_load($company_id);
    $organizer_opts =array('_none' => t('- None -'));
    foreach ($company->field_owner[LANGUAGE_NONE] as $owner) {
      $loaded = user_load($owner['target_id']);
      if ($loaded != null) {
        $organizer_opts[$loaded->uid] = tm_events_username($loaded) . " (" . $loaded->uid . ")";
      }
    }
    $form['field_organizer'][LANGUAGE_NONE]['#options'] = $organizer_opts;

    // Select existing organizers if existing event
    if (!$is_new_event) {
      $existing_organizers = array();
      if (sizeof($event->field_organizer) > 0) {
        foreach ($event->field_organizer['und'] as $organizer) {
          $existing_organizers[] = $organizer['target_id'];
        }
      }
      $form['field_organizer'][LANGUAGE_NONE]['#default_value'] = $existing_organizers;
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
    //$form['field_event_hide_organizers']['#access'] = false;
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
    if (!$ajax_request) {
  		if (isset($conf['tm_event_types_edit_tips']['member'])) {
  			drupal_get_messages("help-tip");
  			drupal_set_message($conf['tm_event_types_edit_tips']['member'], "help-tip");
  		}
    }

		// Set timezone
		if (($is_new_event) && ($user->timezone != null)) {
			$form['field_event_timezone'][LANGUAGE_NONE]['#default_value'] = $user->timezone;
		}
		$form['field_event_timezone'][LANGUAGE_NONE]['#description'] = 'Set the timezone where your event is happening.<br><br>';

		// Disable fields
  	$form['field_chapter']['#disabled'] = true;
  	$form['field_event_company']['#disabled'] = true;

		// Hide fields
		$form['field_chapter']['#access'] = false;
    $form['field_event_company']['#access'] = false;
    $form['field_organizer']['#access'] = false;
    //$form['field_event_hide_organizers']['#access'] = false;
    $form['field_event_hide_location']['#access'] = false;

    // Hide fields for non-moderators
    if (!$perm_is_moderator_or_admin) { 
      if (!$ajax_request) { field_group_hide_field_groups($form, array('group_sponsors')); }
      if (!$ajax_request) { field_group_hide_field_groups($form, array('group_featured_members')); }
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

    // Custom banner
    $form['field_event_custom_banner']['#access'] = false;

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
    $form['field_event_company']['#weight'] = 7;
    $form['#group_children']['field_event_company'] = 'group_moderator_fields';

    $form['field_event_custom_banner']['#weight'] = 8;

    drupal_add_js('jQuery(document).ready(function () { jQuery("#tm-event-edit-company-description").html("The company listing this event.<br><b>Listing a sponsor? See the <i>Event Sponsors</i> section."); });', array('type' => 'inline', 'scope' => 'footer'));

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
  $form['field_rsvp_link_title']['#states'] = array(
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

  // hide cover video field if not enabled
  if (!$conf["tm_cover_videos_enabled"]) {
    unset($form['field_event_cover_video']);
  }

  // customize the approved_members setting
  if ($is_new_event) {
    if (isset($conf['tm_event_approved_members_default'])) {
      $form['field_approved_members_only'][LANGUAGE_NONE]['#default_value'] = $conf['tm_event_approved_members_default'];
    }
  }

  // STEP 6. Modify payment fields if tm_payments enabled
  //////////
  if (module_exists("tm_payments")) {
    _tm_payments_form_alter_event_form($form, $form_state, $form_id);
  } else {

    // hide fields manually
    // note: because if module is disabled, field groups don't exist
    $disable_payment_fields = array(
      'field_event_payment_enabled',
      'field_event_payment_early_date',
      'field_event_payment_early_label',
      'field_event_payment_currency',
      'field_event_payment_amount',
      'field_event_payment_refund_text',
      'field_event_payment_ask_address',
      'field_event_payment_early_enable',
      'field_event_payment_early_amount',
      'field_event_payment_early_seats',
      'field_event_is_preview_mode',
      'field_event_payment_payout_to',
      'field_event_payment_commission',
      'field_event_payment_type',
      'field_event_payment_tiers',
      'field_event_payment_partner_code',
      'field_event_payment_sub_discount',
      'field_event_payments_show_total',
      'field_event_payment_post_event',
      'field_event_payment_message_text'
    );

    foreach ($disable_payment_fields as $payment_field_name) {
      if (isset($form[$payment_field_name])) {
        $form[$payment_field_name]['#access'] = false;
      }
    }

  }

  // STEP 7. Hide any help tips for ajax requests
  // Hide any help tips for ajax requests
  // Determine if this call is from an ajax request
  $is_ajax_request = false;
  if (isset($form_state['triggering_element']['#name'])) {
    if (($form_state['triggering_element']['#name'] == 'field_image_und_0_remove_button') or
      ($form_state['triggering_element']['#name'] == 'field_image_und_0_upload_button')) {
      $is_ajax_request = true;
    }
  }
  if ($is_ajax_request) {
    drupal_get_messages("help-tip");
  }

  // STEP 8. SET BUTTON TEXT
  //////////
  if ($is_new_event) { 
    $form['actions']['submit']['#value'] = t('Add event');
  }

  // STEP 9. RSVP LINK TEXT
  ////////////
  $form['field_rsvp_link_title'][LANGUAGE_NONE][0]['value']['#attributes']['placeholder'] = "RSVP";
  if (!$perm_is_moderator_or_admin) {
    unset($form['field_rsvp_link_title']);
  }

  // STEP 10. Add js prompt if event is new and user returned via back button
  //////////
  if ($is_new_event) {
    drupal_add_js(drupal_get_path('module', 'tm_events') . '/js/tm_events_add_event.js');
  }

  // STEP 11. ONLINE EVENT SETTINGS
  if (isset($conf["tm_chatbox_enabled"])) {
    if (!$conf["tm_chatbox_enabled"]) {
      unset($form['field_event_chatbox_enabled']);
    }
  } else {
    unset($form['field_event_chatbox_enabled']);
  }
  // must select a livestream mode
  unset($form['field_event_livestream_mode'][LANGUAGE_NONE]['#options']["_none"]);

  // show online event help
  if (isset($conf["tm_event_online_event_tips"])) {
    $online_events_help_text = "<div class='tm-online-events-edit-form-message' style='border: 5px solid red; font-size: 13.6px !important; line-height: 20px !important; margin-top: 16px !important;'>" . $conf["tm_event_online_event_tips"] . "</div>";
    $form['#fieldgroups']['group_events_online_settings']->format_settings['instance_settings']['description'] = $online_events_help_text;
  }

  // STEP 12. GEOBLOCK SETTINGS
  // must select a geoblock mode
  // event chapter

  // get event chapter country
  $event_chapter = null;
  $event_chapter_country = "";
  if (isset($chapter_id)) {
    $event_chapter = node_load($chapter_id);
    if (isset($event_chapter->field_country[LANGUAGE_NONE][0]['iso2'])) {
      $country_code = $event_chapter->field_country[LANGUAGE_NONE][0]['iso2'];
      $country_names = tm_base_get_all_country_names();
      $event_chapter_country = $country_names[$country_code];
    }
  }
  
  // grammar for USA
  if ($event_chapter_country == "United States") {
    $event_chapter_country = "the United States";
  }

  // published chapters
  $published_chapters_text = "";
  if (sizeof(tm_events_get_published_chapters($event)) > 0) {
     $published_chapters_text = " and published chapters";
  }

  $geoblock_options = array();
  $geoblock_options["all_members"] = "All members (default)";

  if ($event_chapter != null) {
    $geoblock_options["same_chapter_as_event"] = "Only members of " . tm_chapters_formal_name($event_chapter) . $published_chapters_text;
  }
  if ($event_chapter_country != "") {
    $geoblock_options["same_country_as_event"] = "Only members in " . $event_chapter_country;
  }
  $form['field_event_geoblock_signups'][LANGUAGE_NONE]['#options'] = $geoblock_options;

  // STEP 13. Hide livestream on checkbox (we put a button in the options dropdown)
  unset($form['field_event_livestream_on']);

  // STEP 14. Reminders

  // Update description if sending to followers
  // Members: your followers
  // Companies: your followers, or your upvoters
  if ($typeof_event != "chapter_event") {
    $followers_label = "followers";
    if ($typeof_event == "company_event") {
      $followers_label = "company followers";
      if (isset($conf["tm_organizations_upvote"])) {
        if ($conf["tm_organizations_upvote"]) {
          $followers_label = "upvoters";
        }
      }
    }

    // set description
    $send_reminders_description_followers = "Your " . $followers_label . " are invited <b>24 hours</b> before the event starts to register.<br>
Registered attendees are reminded <b>1.5 hours</b> and <b>10 minutes</b> before you go live.";
    $form['field_event_send_reminders'][LANGUAGE_NONE]['#description'] = $send_reminders_description_followers;
  }

  // Hide reminders if they aren't enabled in $conf["tm_event_online_reminders_enabled"]
  $reminders_enabled = false;
  if (isset($conf["tm_event_online_reminders_enabled"])) {
    $reminders_enabled = $conf["tm_event_online_reminders_enabled"];
  }
  if (!$reminders_enabled) {
     unset($form['field_event_send_reminders']);
  }

  // Hide discuss category id (deprecated)
  unset($form['field_event_discuss_category_id']);

}

/**
 * Hook - modify event before saving
 */
function tm_events_node_presave($node) {

	global $user;

  // make sure we are working on an event node
  if ($node->type != "event") { return; }

  // Don't interfere with tm_users_user_cancel()
  // This method prevents reassignment of nodes to anonymous user when $user is moderator or admin
  $menu_path = menu_get_item();
  if (isset($menu_path['path'])) {
    if ($menu_path['path'] == "user/%/cancel") {
      return;
    }
  }

  // Moderators can reassign the owner of the event
  // This will change the owner of the node
  // Only perform this if node is not owned by anonymous
  $perm_is_moderator_or_admin = ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles)));
	if ($perm_is_moderator_or_admin) { 
		if (isset($node->field_event_reassign_owner[LANGUAGE_NONE][0])) {
			$new_owner_uid = $node->field_event_reassign_owner[LANGUAGE_NONE][0]['target_id'];
			$node->uid = $new_owner_uid;
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

  // If external rsvp is set but no url provided, switch back to managed rsvp
  $external_rsvp = $node->field_external_rsvp[LANGUAGE_NONE][0]['value'];
  if ($external_rsvp === '1') {
    if (!isset($node->field_rsvp_link[LANGUAGE_NONE][0]['url'])) {
      $node->field_external_rsvp[LANGUAGE_NONE][0]['value'] = 0; // turn off
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

  // Update the Non-Registered guests field
  if (isset($node->field_event_total_attendance[LANGUAGE_NONE][0]['value'])) {
    
    // Get Total Attendance
    $total_attendance = intval($node->field_event_total_attendance[LANGUAGE_NONE][0]['value']);

    // If Total Attendance is set, update Non-Registered guests field
    if ($total_attendance > 0) {

      // Count number of registrations
      $flag = flag_get_flag('event_register');
      $registered_count = $flag->get_count($node->nid);

      $non_registered_guests = $total_attendance - $registered_count;
      if ($non_registered_guests > 0) {
        $node->field_event_extra_guests[LANGUAGE_NONE][0]['value'] = $non_registered_guests;
      }
    }
  }

  // Trim COVID-19 Message
  if (isset($node->field_event_covid19_message[LANGUAGE_NONE][0]['value'])) {
    $trimmed = trim($node->field_event_covid19_message[LANGUAGE_NONE][0]['value']);
    if ($trimmed != "") {
      // save trimmed value
      $node->field_event_covid19_message[LANGUAGE_NONE][0]['value'] = $trimmed;
    } else {
      // remove empty value
      $node->field_event_covid19_message[LANGUAGE_NONE][0]['value'] = null;
    }
  }

}

/**
 * Implement hook_node_insert
 * Set cookie for event nid on save
 */
function tm_events_node_insert($node) {
  if ($node->type == "event") {
    user_cookie_save(array("last_edited_event" => $node->nid));
  }
}

/**
 * Handler /node/new/event
 * Removes last_edited_event cookie and redirects to /node/add/event
 */
function tm_events_new_event() {

  // remove last_edited_event cookie
  user_cookie_delete("last_edited_event");

  // set new url
  $request_uri = ($_SERVER['REQUEST_URI']);
  $request_uri = str_replace("/node/new/event", "/node/add/event", $request_uri);
  $options['absolute'] = TRUE;
  $url = url($request_uri, $options);
  header('Location: ' . $request_uri, TRUE, 302);
  drupal_exit($url);
}

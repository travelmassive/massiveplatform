<?php

// tm_fields.menu_event.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_event_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;
  
  $base = $base_root . request_uri();

  // quick lookup tm_events_check_permission_edit_event()
  $check_permission_edit_event = false;
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'event') {
      $check_permission_edit_event = tm_events_check_permission_edit_event($user, $field['entity']);
    }
  }

  // START Add chapter event Link
  if (isset($field['entity']->type) && $field['entity']->type == 'chapter') {
    if (_tm_events_check_create_chapter_event($user, $field['entity']->nid)) {
      $links['chapter'][] = array(
      'wrapper_class' => array('event_add'),
      'content' => l(t('Add chapter event'), '/node/new/event', array(
        'query' => array(
          'field_chapter' => $field['entity']->nid
        ),
        'attributes' => array(
          'title' => t('Add event for @chapter', array('@chapter' => $field['entity']->title))
        )
      )));
    }
  }
  // END Add chapter event Link

  // START Add company event Link
  if (isset($field['entity']->type) && $field['entity']->type == 'organization') {

    // Check if can create company event
    if (_tm_events_check_create_company_event($user, $field['entity']->nid)) {
      $links['organization'][] = array(
      'wrapper_class' => array('event_add'),
      'content' => l(t('Add event'), '/node/new/event', array(
        'query' => array(
          'field_event_company' => $field['entity']->nid
        ),
        'attributes' => array(
          'title' => t('Add event for @organization', array('@organization' => $field['entity']->title))
        )
      )));

    } else {

      // If events enabled, show user a message
      if ($conf['tm_event_company_events_enabled'] == true) {

        // Set create member event message as js var
        // We use this in jq_create_company_event_message()
        $company_event_message = _tm_events_check_create_company_event_message($user, $field['entity']->nid);
        drupal_add_js(array('tm_events' => array('create_company_event_message' => $company_event_message)), array('type' => 'setting'));

        if ($company_event_message != "") {
          $links['organization'][] = array(
            'wrapper_class' => array('event_add'),
            'content' => l(t('Add event'), 'javascript:jq_create_company_event_message();',
            array('fragment' => '','external'=>true)
          ));
        }
      }
    }
  }
  // END Add company event Link

  // START Add member event link
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    if ($conf['tm_event_member_events_enabled'] == true) {
      if ($user->uid == $field['entity']->uid) {

        // member can create event
        if (_tm_events_check_create_member_event($user)) {

          // chapter leader
          if (in_array("chapter leader", $user->roles)) {
            // don't display a link for chapter leaders to create a member event
          } else {
            $links['user'][] = array(
              'wrapper_class' => array('add'),
              'content' => l(t('Add event'), '/node/new/event',
                array('attributes' => array('title' => t('Add your own event')))
              )
            );
          }
         
        } else {
          // member not able to create event
          $member_event_message = _tm_events_check_create_member_event_message($user);
          if ($member_event_message != "") {
            $links['user'][] = array(
              'wrapper_class' => array('event_add'),
              'content' => l(t('Add event'), 'javascript:jq_create_member_event_message();',
                array(
                  'fragment' => '',
                  'external' => true,
                  'attributes' => array('title' => t('Add your own event'))
                )
              )
            );
          }

        } // end else
      }
    }
  }
  // END Add member event link

  // START cancel ticket
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'event') {

      // if donation button showing, show unregister link
      $event_display_flag = _tm_event_display_flag($node->nid, $user);
      if (($event_display_flag == "show_donate") and (!_tm_event_is_past($node))) {

        // generate un-register link
        $rsvpflag = flag_get_flag('event_register');
        $link_type = $rsvpflag->get_link_type();
        $flag_link = module_invoke($link_type['module'], 'flag_link', $rsvpflag, "unflag", $node->nid);
        $unregister_link = "/" . $flag_link['href'] . "?destination=node/" . $node->nid . "&token=" . $flag_link['query']['token'];

        $links['event'][] = array(
          'wrapper_class' => array('event_manage_registrations'),
          'content' => l(t('Cancel my ticket'), $unregister_link, array(
            'external' => true,
            'attributes' => array( 
            'title' => t('Cancel ticket')
            )
          ))
        );

      }
    }
  }
  // END cancel ticket

  // START manage RSVPs
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'event') {
      
      $show_menu_item = false;

      // check permission
      $show_menu_item = $check_permission_edit_event;
    
      // if event is external rsvp, don't show
      if ($node->field_external_rsvp['und'][0]['value']) {
        $show_menu_item = false;
      }

      if ($show_menu_item) {
        $links['event'][] = array(
          'wrapper_class' => array('event_manage_registrations'),
          'content' => l(t('Manage registrations'), 'events/manage-registrations/' . $nid, array(
            'attributes' => array( 
            'title' => t('Manage registrations')
            )
          ))
        );
      }
    }
  }
  // END manage RSVPs

  // START download RSVP list
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'event') {
      
      // check permission
      $show_menu_item = $check_permission_edit_event;
    
      // also see if user is featured and has access to download csv
      if (!$show_menu_item) {
        $show_menu_item = tm_events_check_featured_member_download_csv($user, $field['entity']);
      }

      // if event is external rsvp, don't show
      if ($node->field_external_rsvp['und'][0]['value']) {
        $show_menu_item = false;
      }

      if ($show_menu_item) {
        $links['event'][] = array(
          'wrapper_class' => array('event_download_attendee_list'),
          'content' => l(t('Download attendee list'), 'events/download-csv/' . $nid, array(
            'attributes' => array( 
            'title' => t('Download attendee list')
            )
          ))
        );

        // Display when last downloaded
        $download_last_flagged = flag_get_entity_flags("node", $nid, "download_member_csv");
        if (sizeof($download_last_flagged) > 0) {
          $flagged_last = array_shift($download_last_flagged);
          $difference = time() - $flagged_last->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $who_sent = user_load($flagged_last->uid);
          $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>" . $flagged_time . " by " . l($who_sent->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $who_sent->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . "</span><br>";
          $links['event'][] = array(
            'wrapper_class' => array('last_downloaded_attendees'),
            'content' => t($last_sent_text),
          );
        }

      }
    }
  }
  // END download RSVP list

  // START contact attendees
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'event') {

      // check permission
      $show_menu_item = tm_events_check_permission_announce_event($user, $field['entity']);

      // if event is external rsvp, don't show
      if (isset($conf['tm_event_restrict_announcement_rsvp'])) {
        if ($conf['tm_event_restrict_announcement_rsvp'] == true) {
      
          if ($node->field_external_rsvp['und'][0]['value']) {
            $show_menu_item = false;
            $links['event'][] = array(
              'wrapper_class' => array('event_send_announcement_message'),
              'content' => l(t('Send announcement'), "javascript:jq_alert('Enable event management features', 'Make it easier for members to register and get access to event management features by enabling <i>Allow members to RSVP instantly</i> in your event settings.')", array(
                                'fragment' => '',
                                'external'=>true,
                                'attributes' => array('title' => t('Send member'))
                            ))
              );
          }
        }
      }

      if ($show_menu_item) {
        $links['event'][] = array(
          'wrapper_class' => array('event_send_announcement'),
          'content' => l(t('Send announcement'), 'events/send-announcement/' . $nid, array(
            'attributes' => array( 
            'title' => t('Send announcement')
            )
          ))
        );

        // Display when last sent announcement
        $announcement_last_sent_flagged = flag_get_entity_flags("node", $nid, "announcement_last_sent");
        if (sizeof($announcement_last_sent_flagged) > 0) {
          $flagged_last = array_shift($announcement_last_sent_flagged);
          $difference = time() - $flagged_last->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $who_sent = user_load($flagged_last->uid);
          $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Sent " . $flagged_time . " by " . l($who_sent->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $who_sent->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . "</span><br>";
          $links['event'][] = array(
            'wrapper_class' => array('event_last_sent'),
            'content' => t($last_sent_text),
          );
        }

        // Check if reminders are enabled
        $reminders_enabled = false;
        if (isset($conf["tm_event_online_reminders_enabled"])) {
          $reminders_enabled = $conf["tm_event_online_reminders_enabled"];
        }

        // Event is online
        $is_online_event = tm_events_is_online_event($node);

        // Event has reminders
        $event_send_reminders = false;
        if (isset($node->field_event_send_reminders[LANGUAGE_NONE][0]['value'])) {
          $event_send_reminders = $node->field_event_send_reminders[LANGUAGE_NONE][0]['value'];
        }

        // Event has external rsvp
        $has_external_rsvp = false;
        if (isset($node->field_external_rsvp[LANGUAGE_NONE][0]['value'])) {
           $has_external_rsvp = $node->field_external_rsvp[LANGUAGE_NONE][0]['value'];
        }

        // Event reminders turned off
        if ($reminders_enabled and $is_online_event and !$event_send_reminders and !$has_external_rsvp and !tm_events_is_past_event($node, 60)) {
          $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Event reminders are " . l("disabled", "/node/" . $nid . "/edit", array('attributes' => array('class' => array('report-flagged-link')))) . "</span><br>";
          $links['event'][] = array(
            'wrapper_class' => array('event_last_sent'),
            'content' => t($last_sent_text),
          );
        }

        // Preview test emails
        // Don't show for past events
        if ($reminders_enabled and $is_online_event and $event_send_reminders and !tm_events_is_past_event($node, 60)) {
          
          // Preview 1hr30min reminder
          if (!tm_events_is_preview_mode($node->nid)) {
            $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Preview " . l("1 day", "/events/send-test-reminder/" . $nid . "/24hr", array('attributes' => array('class' => array('report-flagged-link')))) . " reminder</span><br>";
            $links['event'][] = array(
              'wrapper_class' => array('event_last_sent'),
              'content' => t($last_sent_text),
            );
          }
        
          // Check not external rsvp
          if (!$has_external_rsvp) {

            // Preview 1hr30min reminder
            $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Preview " . l("1 hour 30 min", "/events/send-test-reminder/" . $nid . "/1hr30min", array('attributes' => array('class' => array('report-flagged-link')))) . " reminder</span><br>";
            $links['event'][] = array(
              'wrapper_class' => array('event_last_sent'),
              'content' => t($last_sent_text),
            );

            // Preview 10min reminder
            $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Preview " . l("10 minute", "/" . "events/send-test-reminder/" . $nid . "/10min", array('attributes' => array('class' => array('report-flagged-link')))) . " reminder</span><br>";
            $links['event'][] = array(
              'wrapper_class' => array('event_last_sent'),
              'content' => t($last_sent_text),
            );

          }          
        }

      }
    }
  }
  // END contact attendees

  // START event insights
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    
    if ($node->type == 'event') {

      // check permission
      $show_menu_item = tm_events_check_permission_announce_event($user, $field['entity']);
      if ($show_menu_item) {

        if ($node->field_external_rsvp[LANGUAGE_NONE][0]['value']) {

          $links['event'][] = array(
            'wrapper_class' => array('event_insights_message'),
            'content' => l(t('Event insights'), "javascript:jq_alert('Get detailed reporting', 'Manage your attendee list and view detailed insights by enabling <i>Allow members to RSVP instantly</i> in your event settings.')", array(
                              'fragment' => '',
                              'external'=>true,
                              'attributes' => array('title' => t('Event insights'))
                          ))
            );

        } else {

          $links['event'][] = array(
            'wrapper_class' => array('event_insights'),
            'content' => l(t('Event insights'), 'events/insights/' . $nid, array(
              'attributes' => array( 
              'title' => t('Event insights')
              )
            ))
          );
        
        }
      }
    }
  }
  // END event insights

  // START View event viewers
  if (module_exists("tm_track_views")) {
    if (isset($field['entity']->type) && $field['entity']->type == 'event') {

      // Check if is moderator or administrator
      if ($check_permission_edit_event) {

        // get number of viewers
        $max_view_days = $conf["tm_track_views_store_days"];
        $num_views = tm_track_views_how_many_viewed($field['entity']->nid, "node", $user->uid, $max_view_days);

        if ($num_views > 0) {

          $num_views_plural = "members";
          if ($num_views == 1) {
            $num_views_plural = "member";
          }
        
          $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'><a style='display: inline; padding: 0rem;' href='/event/" . $field['entity']->nid . "/viewers'>Viewed by " . $num_views .  " " . $num_views_plural . "</a></span>";

          $links['event'][] = array(
            'wrapper_class' => array('event_viewers'),
            'content' => t($menu_text),
          );

        }
      }
    }
  }
  // END View event viewers

  // START ticket sales and checkout lnk
  if (module_exists("tm_payments")) {
    if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
      $node = menu_get_object();
      if ($node->type == 'event') {

        // check permission
        $show_menu_item = false;
        $allowed_manage_event = $check_permission_edit_event;
        if ($allowed_manage_event) {

          // check payments are enabled
          if (_tm_payments_is_payment_enabled($field['entity'])) {
             $show_menu_item = true;
          }

          // check if any payments have been made
          if (tm_checkout_event_has_payments($node->nid)) {
            $show_menu_item = true;
          }
        }

        // if event is external rsvp, don't show
        if ($node->field_external_rsvp[LANGUAGE_NONE][0]['value']) {
          $show_menu_item = false;
        }

        if ($show_menu_item) {

          // checkout page
          $links['event'][] = array(
            'wrapper_class' => array('event_ticket_sales'),
            'content' => l(t('Checkout page'), 'checkout/event/' . $nid, array(
              'attributes' => array( 
              'title' => t('Checkout page')
              )
            ))
          );

          // ticket sales
          $links['event'][] = array(
            'wrapper_class' => array('event_ticket_sales'),
            'content' => l(t('Ticket sales'), 'payment/reports/event-sales/' . $nid, array(
              'attributes' => array( 
              'title' => t('Ticket sales')
              )
            ))
          );
        }
      }
    }
  }
  // END ticket sales and checkout link

  // START show calendar link
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'event') {
      
      $show_menu_item = false;

      // check permission
      $show_menu_item = $check_permission_edit_event;
    
      if ($show_menu_item) {

        $calendar_link = '/events/download-ical/' . $nid;
        $links['event'][] = array(
          'wrapper_class' => array('event_download_calendar'),
          'content' => l(t('Download calendar'), $calendar_link, array(
            'attributes' => array(
              'title' => t('Download calendar')
            )
          ))
        );
      }
    }
  }
  // END show calendar link

  // START register member to event
  if (($menu_item['path'] == 'user/%') or ($menu_item['path'] == 'user/%/view')) {

    // Only show to chapter leader or moderator
    if (((in_array("chapter leader", $user->roles) or (in_array("moderator", $user->roles)))) 
      and ($targetuser->uid != $user->uid)) {

      // get list of chapters user is leading or is community manager of
      $leading_chapters = tm_users_get_chapters_leading($user, true);

      // get list of chapter
      $members_chapters = tm_users_get_chapters($targetuser);

      // get list of leading chapters that member is in
      if (sizeof($members_chapters) > 0) {
        $leading_members_chapters = array();
        foreach($members_chapters as $chapter_id) {
          if (in_array($chapter_id, $leading_chapters)) {
            $leading_members_chapters[] = $chapter_id;
          }
        }
      } else {
        // member is not in any chapter
        $leading_members_chapters = $leading_chapters;
      }
      
      // get list of upcoming events in chapters
      $upcoming_events = array();
      if (sizeof($leading_members_chapters) > 0) {
        $upcoming_events = tm_events_get_future_events_for_chapters($leading_members_chapters);
      }
    
      // if chapter has upcoming events, or user is moderator
      if ((sizeof($upcoming_events) > 0) or (in_array("moderator", $user->roles))) {

        // Get user name
        $first_name = strip_tags($targetuser->field_user_first_name[LANGUAGE_NONE][0]['value']);
        $last_name = strip_tags($targetuser->field_user_last_name[LANGUAGE_NONE][0]['value']);
        $full_name = $first_name . " " . $last_name;

        $links['user'][] = array(
          'wrapper_class' => array('event_register_member'),
          'content' => l(t('Register to event'), 'javascript:jq_register_member_to_event(' . $targetuser->uid . ', "' . htmlentities($full_name) . '")',
            array(
            'fragment' => '',
            'external'=> true,
            'attributes' => array(
              'title' => t('Register to event')
            )
          ))
        );
      }

    }
  }
  // END register member to event

  // START show chatbox moderation link
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'event') {
      
      $show_menu_item = false;

      // check permission
      if ($check_permission_edit_event) {

        // check if chat enabled
        if (isset($node->field_event_chatbox_enabled[LANGUAGE_NONE][0]['value'])) {
          if ($node->field_event_chatbox_enabled[LANGUAGE_NONE][0]['value'] == 1) {
            $show_menu_item = true;
          }
        }

      }

      if ($show_menu_item) {

        $calendar_link = '/events/moderation-chatbox/' . $nid;
        $links['event'][] = array(
          'wrapper_class' => array('event_moderation_chatbox'),
          'content' => l(t('Chat moderation'), $calendar_link, array(
            'attributes' => array(
              'title' => t('Chat moderation'),
              'target' => '_blank',
            )
          ))
        );
      }
    }
  }
  // END show chatbox moderation link

  // START show go live link
  if (isset($menu_item['page_arguments'][0]->nid) && $user->uid > 0) {
    $node = menu_get_object();
    if ($node->type == 'event') {
      
      $show_menu_item = false;

      // check permission
      if ($check_permission_edit_event) {

        // Get live stream URL
        $livestream_url = tm_event_get_livestream_url($node);

        // Get youtube video id
        $youtube_video_id = tm_event_get_youtube_video_id($livestream_url);

        // Show live stream button
        if ((!tm_event_livestream_is_on($node)) and ($youtube_video_id != null)) {

           $links['event'][] = array(
            'wrapper_class' => array('event_moderation_go_live'),
            'content' => l(t('Go Live'), "/events/display-live-stream-callback/" . $node->nid . "/on", array(
              'attributes' => array(
                'title' => t('Go Live'),
                'class' => 'tm-go-live-button',
              )
            ))
          );

          // Show scheduled time if automated schedule enabled
          if (isset($conf["tm_event_online_livestream_schedule_enabled"])) {
            if ($conf["tm_event_online_livestream_schedule_enabled"]) {

              // Scheduled
              // How long until scheduled to go live
              $scheduled_text = "";
              $event_date = field_get_items('node',$node,'field_event_date');
              $current_date = new DateTime('now', new DateTimeZone('UTC'));
              $event_timezone = _tm_get_event_timezone($node);
              $event_datetime = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
              if (($event_datetime->format('U') - $current_date->format('U')) > 0) {
                $scheduled_text = "Scheduled in " . _tm_event_time_elapsed($event_datetime->format('U') - $current_date->format('U'));
              }

              if ($scheduled_text != "") {
                $last_sent_text = "<span style='font-size: smaller; font-style: italic; padding-left: 1.5em;'>" . $scheduled_text . "</span><br>";
                $links['event'][] = array(
                  'wrapper_class' => array('event_moderation_go_live'),
                  'content' => t($last_sent_text),
                );
              }
             
            }
          }
        }

        // End live stream button
        if (tm_event_livestream_is_on($node)) {

           $links['event'][] = array(
            'wrapper_class' => array('event_moderation_chatbox'),
            'content' => l(t('End Live View'), "/events/display-live-stream-callback/" . $node->nid . "/off", array(
              'attributes' => array(
                'title' => t('End Live View'),
                'class' => 'tm-end-live-button',
              )
            ))
          );
        }

      }
    }
  }



}

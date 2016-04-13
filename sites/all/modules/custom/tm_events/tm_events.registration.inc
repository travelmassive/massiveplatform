<?php

// tm_events.rsvp.inc - manage rsvp methods

/** 
 * Form for managing RSVPs
 */
function tm_events_manage_registrations_form($event){

  global $user;
  $event = node_load(arg(2)); // /events/manage-registrations/1234
  $form = array();

  // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_events') . '/js/tm_events_manage_registrations.js');

  // Add page title and page url to javascript
  $event_url = drupal_get_path_alias('node/'.$event->nid);
  $event_title = drupal_get_path_alias($event->title);
  drupal_add_js(array('tm_events' => array('event_url' => $event_url, 'event_title' => $event_title)), array('type' => 'setting'));

  //$form['#prefix'] = l(t('Back to event'),'node/'.$event->nid);

  $options['rsvp'] = t('Registered (Attending)');  
  $options['waitlist'] = t('Waitlist');  
  $options['remove'] = t('Not attending');  

  $form['status'] = array(
    '#type' =>'select',
    '#options' => $options,
    '#title' => t('Change status of selected attendees'),
    );

  $form['eventid'] = array(
    '#type' =>'hidden',
    '#value' => $event->nid,
    );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Apply changes'),
    '#id' => 'edit-submit'
  );

  if ($event){

    // list of users, each with checkbox
    $query = db_select('flagging', 'f')
          ->fields('f')
          ->condition('entity_id',$event->nid,'=')
          ->condition('entity_type','node','=')
          ->orderBy('f.timestamp', 'ASC') //ORDER BY flagging time
          ->groupBy('uid');
    $query->addExpression('GROUP_CONCAT(DISTINCT fid)', 'flag_list');
    $users = $query->execute();

    $useroptions = array();
    while ($attendee_user = $users->fetchAssoc()){
      $account = user_load($attendee_user['uid']);
      // set profile url (either friendly url or community url)
      $profile_url = drupal_get_path_alias("user/" . $account->uid);

      $difference = time() - $attendee_user['timestamp'];
      $flagged_time = format_interval($difference, 1) . " ago";

      $flag_list = explode(",", $attendee_user['flag_list']); // get flag list
      $useroptions[$attendee_user['uid']] = 
        '<span class="tmeventuser">'.tm_events_username($account).'</span>'.
        '<span class="tmeventstatus">'.tm_events_status($flag_list).' <span class="tmeventstime">'.$flagged_time.'</span> | <span class="tmeventslink"><a target="_blank" href="/' . $profile_url . '">view profile</a></span></span>';
    }

    $form['userlist'] = array(
      '#type' =>'checkboxes',
      '#options' => $useroptions,
      '#title' => t('Attendees'),
      );

    $form['download'] = array(
      '#type' => 'markup',
      '#value' => t('Download attendee list'),
      '#markup' => '<p><a class="form-download" href="/events/download-csv/'.$event->nid.'">'.t('Download attendee list').'</a></p>',
      );

    // determine if we show the email addres or not
    $show_email = false;
    if ((in_array('administrator', array_values($user->roles)))
        or (in_array('moderator', array_values($user->roles)))
        or (in_array('chapter leader', array_values($user->roles)))) {
      $show_email = true;
    }

    $form_notes = '<div class="tm-event-manage-registrations-tips">
          <b>How to manage registrations</b>
          <ul>
          <li>Change the status of selected attendees by clicking <b>Apply changes</b>.</li>
          <li>Members will be notified immediately of their status change via email.</li>';
          
    if ($show_email) {
    	$form_notes .= '<li>Click <b>Download attendee list</b> to export attendee information.';
    	$form_notes .= '<br><i>Privacy Note - We care about privacy so please use CSV data in accordance with our members\' Privacy Terms.</i></li>';
    } else {
    	$form_notes .= '<li>Click <b>Download attendee list</b> to export attendee information.</li>';
    }

    $form_notes .= '</ul>
          <b>How to use the Waitlist</b>
          <ul>
          <li>The Waitlist will be enabled when the number of registrations reaches the number seats available.</li>
          <li>If you increase the number of seats, you need to update the status of waitlisted attendees.</li>
          <li>If a registered attendee cancels their registration, the first person on the Waitlist will be automatically registered.</li>
          </ul></div>';

    $form['notes'] = array(
      '#markup' => $form_notes
    );
  }

  return $form;
}

/** 
 * RSVP form submit function
 */
function tm_events_manage_registrations_form_submit($form, &$form_state) {

  $rsvpflag = flag_get_flag('event_register', NULL);
  $waitflag = flag_get_flag('event_waitlist', NULL);

  // tm_payments - paid flag
  if (module_exists("tm_payments")) {
    $paidflag = flag_get_flag('event_paid', NULL);
  }

  $action = $form_state['values']['status'];
  $event  = $form_state['values']['eventid'];

  foreach($form_state['values']['userlist'] as $uid){
    if (!empty($uid)){
      $account = user_load($uid);
      if ($action == 'remove'){
        $rsvpflag->flag('unflag', $event, $account, TRUE);
        $waitflag->flag('unflag', $event, $account, TRUE);
        // tm_payments - paid flag
        if (module_exists("tm_payments")) {
          $paidflag->flag('unflag', $event, $account, TRUE);
        }
      }
      else if ($action == 'waitlist'){
        $rsvpflag->flag('unflag', $event, $account, TRUE);
        $waitflag->flag('flag', $event, $account, TRUE);
      }
      else if ($action == 'rsvp'){
        // add to rsvp first, unflag waitlist second
        // this will trigger a single rsvp notification
        $rsvpflag->flag('flag', $event, $account, TRUE);
        $waitflag->flag('unflag', $event, $account, TRUE);
      }
    }
  }
}

/**
 * Hook for unflag 
 */
function tm_events_flag_unflag($flag, $entity_id, $account, $flagging) {
  
  // If member is un-registered from an event, add next in line for the waitlist
  // Note: if we want to only work when user peforms the action and not a moderator, add this:
  // if (($GLOBALS['user']->uid == $user->uid)) {
  if ($flag->name == 'event_register') {
    global $conf;
    if (isset($conf["tm_event_auto_register_waitlist"])) {
      if ($conf["tm_event_auto_register_waitlist"]) {
        $event = node_load($entity_id);
        _tm_events_auto_register_from_waitlist($event);
      }
    }
  }
}


/** 
 * Automatically register the next person on the waitlist
 * We only do this if spare seats are available
 */
function _tm_events_auto_register_from_waitlist($event) {

  // Step 1. Check if there are available seats
  $seats_left = _tm_events_seats_left($event);
  if ($seats_left <= 0) {
    return false;
  }

  // Step 2. Find next person on waitlist, and add them
  // Note: We don't handle the user's unregistration here. 
  $rsvpflag = flag_get_flag('event_register', NULL);
  $waitflag = flag_get_flag('event_waitlist', NULL);

  // find first person on waitlist for event
  $waitlisted_users = db_select('flagging', 'f')
        ->fields('f')
        ->condition('entity_id',$event->nid,'=')
        ->condition('entity_type','node','=')
        ->condition('fid',$waitflag->fid,'=') // waitlist flag condition
        ->orderBy('f.timestamp', 'ASC') //ORDER BY flagging time
        ->execute();
  
  $firstrow = $waitlisted_users->fetchAssoc();
  if ($firstrow == FALSE) {
    return false; // no-one on waitlist
  }

  $waitlist_user = user_load($firstrow['uid']);

  // Add to rsvp first, unflag waitlist second
  // this will trigger a single rsvp notification
  $rsvpflag->flag('flag', $event->nid, $waitlist_user, TRUE);
  $waitflag->flag('unflag', $event->nid, $waitlist_user, TRUE);

  // We added someone from the waitlist
  return true;
}

/**
 * Converts the flag ID to the flag name
 * Takes array of flag_ids
 */
function tm_events_status($flag_ids){

  $event_register = false;
  $event_waitlist = false;
  $event_paid = false;

  foreach ($flag_ids as $fid) {
    $flag = flag_get_flag(NULL, $fid);
    if ($flag->name == 'event_register') $event_register = true;
    if ($flag->name == 'event_waitlist') $event_waitlist = true;
    if ($flag->name == 'event_paid') $event_paid = true;
  }

  // If paid but not registered, user has cancelled
  if (module_exists("tm_payments")) {
    if ($event_paid) { 
      if ($event_register || $event_waitlist) {
        return 'Paid';
      } else {
        return 'Cancelled';
      }
    }
  }

  if ($event_register) {
    return 'Registered';
  }

  if ($event_waitlist) {
    return 'Waitlist';
  }

  return '';
  
}

<?php

// tm_events.csv.inc - export csv methods


/**
 * Check if user can download RSVP list
 */
function tm_events_download_csv_check() {

	global $user;
  $event_id = arg(2); // /events/download-csv/1234
  $event = node_load($event_id);

  // check permissions
  if (tm_events_check_permission_edit_event($user, $event)) {
  	return true;
  }

  if (tm_events_check_featured_member_download_csv($user, $event)) {
    return true;
  }

  // not valid
  return false;

}

/*
 * RSVP Download function 
 */
function tm_events_download_csv($nid){

	global $conf;
	global $user;

  $event = node_load($nid);
  $oldzone = date_default_timezone_get();
  date_default_timezone_set(drupal_get_user_timezone());
  $csvname = $event->title.'_'.date('M_d_Y_Hi',time()) . ".csv";
  $csvname = str_replace(array(","," "), "_", $csvname); // replace spaces and ,
  date_default_timezone_set($oldzone);

  // determine if we show the email addres or not
  $show_email = false;
  if ((in_array('administrator', array_values($user->roles)))
    	or (in_array('moderator', array_values($user->roles)))
    	or (in_array('chapter leader', array_values($user->roles)))) {
  	$show_email = true;
  }

  // csv header
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$csvname);

  $fh = fopen('php://output', 'w');

  // put information use reminder
  $message = "";
  if ($show_email) {
  	$message = "# IMPORTANT: Please care about our members' privacy.";
  	$message .= "\n# Under the " . $conf['tm_site_name'] . " Privacy Terms you cannot transfer personal data (name or email) to sponsors or 3rd parties.";
  	$message .= "\n# Please use the SubscribeEventAnnouncements field to manage any externally delivered notifications or announcements about the event.\n\n";
  }

  fputs($fh,$message,strlen($message));

  // puts a header row across the top of the csv
  if ($show_email) {
  	$item = array(t('First name'),t('Last name'),t('Email'),t('RSVP Type'),t('SubscribeEventAnnouncements'),t('Country'),t('City'),t('Organization'),t('Role'),t('Website'),t('Segment'),t('MemberSince'));
  } else {
  	$item = array(t('First name'),t('Last name'),t('RSVP Type'),t('Country'),t('City'),t('Organization'),t('Role'),t('Website'),t('Segment'),t('MemberSince'));
  }

  fputcsv($fh, $item);

  // list of users
  $query = db_select('flagging', 'f')
    ->fields('f')
    ->condition('entity_id',$nid,'=')
    ->condition('entity_type','node','=')
    ->groupBy('uid');
  $query->addExpression('GROUP_CONCAT(DISTINCT fid)', 'flag_list');
  $users = $query->execute();
  while ($user_row = $users->fetchAssoc()){

    $account = user_load($user_row['uid']);
    if (empty($account->language)) $language = LANGUAGE_NONE;
    else $language = $account->language;

    $item = array();
    // columns in same order as the header array above
    $item[] = $account->field_user_first_name[$language][0]['value'];
    $item[] = $account->field_user_last_name[$language][0]['value'];

    // if allowed to return email
    if ($show_email) {
    	 $item[] = $account->mail;
    }

    $flag_list = explode(",", $user_row['flag_list']); // get flag list 
    $item[] = tm_events_status($flag_list);
    $notif = new TMNotification("user_chapter_announcements");
    if ($show_email) {
    	$item[] = ($notif->isEnabled($account->uid) ? "yes" : "no");
   	}
    if (isset($account->field_user_country[$language][0])){
      $item[] = $account->field_user_country[$language][0]['iso2'];
    }
    else $item[] = t('none');
    if (isset($account->field_location_city[$language][0]['value'])){
      $item[] = $account->field_location_city[$language][0]['value'];
    }
    else $item[] = t('none');
    if (isset($account->field_job_organization[$language][0]['value'])){
      $item[] = $account->field_job_organization[$language][0]['value'];
    }
    else $item[] = t('none');
    if (isset($account->field_job_role[$language][0]['value'])){
      $item[] = $account->field_job_role[$language][0]['value'];
    }
    else $item[] = t('none');
    if (isset($account->field_link_website[$language][0]['url'])){
      $item[] = $account->field_link_website[$language][0]['url'];
    }
    else $item[] = t('none');
    if (isset($account->field_segment[$language][0]['tid'])){
      $term = taxonomy_term_load($account->field_segment[$language][0]['tid']);
      // load parent term
      $parent_terms = taxonomy_get_parents($term->tid);
      if (sizeof($parent_terms > 0)) {
        $parent_term = current($parent_terms);
        $item[] = $parent_term->name. " > " . $term->name;
      } else {
        $item[] = $term->name;
      }
    }
    else $item[] = t('none');
    $item[] = date('j M Y',$user_row['timestamp']);

    // remove commas from any input data
    $new_item = array();
    foreach($item as $col => $val) {
      $new_item[$col] = str_replace(",", "", $val);
    }

    fputcsv($fh, $new_item);
  } 

  fclose($fh);
}

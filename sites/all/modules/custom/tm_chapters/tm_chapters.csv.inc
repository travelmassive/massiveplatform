<?php

// tm_chapters.csv.inc - csv methods

/**
 * Download Chapter CSV function 
 */
function tm_chapters_download_csv(){

  global $user;
  global $conf;
  $nid = _orig_nid();

  if($nid == 0) {
    print drupal_not_found();
    return;
  }

  // Check that user is a chapter leader of this chapter
  if ((!tm_users_is_chapter_leader($user->uid, $nid)) 
    and (!(in_array("moderator", $user->roles))) 
    and (!in_array("administrator", $user->roles)))
 {
    $output = drupal_access_denied();
    print $output;
    return;
  }

  // Get chapter
  $chapter = node_load($nid);

  $oldzone = date_default_timezone_get();
  date_default_timezone_set(drupal_get_user_timezone());
  $csvname = $chapter->title.'_'.date('M_d_Y_Gi',time()) . ".csv";
  date_default_timezone_set($oldzone);

  // csv header
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$csvname);
  
  $fh = fopen('php://output', 'w');

  // put information use reminder
  global $conf;
  $message = "# IMPORTANT: Please care about our members' privacy.";
  $message .= "\n# Under the " . $conf['tm_site_name'] . " Privacy Terms you cannot transfer personal data (name or email) to sponsors or 3rd parties.";
  $message .= "\n# Please use the SubscribeChapterNewsletter field to manage external email newsletter subscriptions.\n\n";

  fputs($fh,$message,strlen($message));

  // puts a header row across the top of the csv
  $item = array(t('First name'),t('Last name'),t('Email'),t('SubscribeChapterNewsletter'),t('SubscribeGlobalNewsletter'),t('SubscribeChapterAnnouncements'),t('Country'),t('City'),t('Organization'),t('Role'),t('Website'),t('Segment'),t('MemberSince'),t('ApprovalStatus'));
  fputcsv($fh, $item);

  // list of users
  $users = db_select('flagging', 'f')
    ->fields('f')
    ->condition('entity_id',$nid,'=')
    ->condition('entity_type','node','=')
    ->execute();
  while ($load_user = $users->fetchAssoc()){
    $account = user_load($load_user['uid']);
    if (empty($account->language)) $language = LANGUAGE_NONE;
    else $language = $account->language;

    $item = array();
    // columns in same order as the header array above
    $item[] = $account->field_user_first_name[$language][0]['value'];
    $item[] = $account->field_user_last_name[$language][0]['value'];
    $item[] = $account->mail;
    $notif = new TMNotification("chapter_newsletters");
    $item[] = ($notif->isEnabled($account->uid) ? "yes" : "no");
    $notif = new TMNotification("global_newsletter");
    $item[] = ($notif->isEnabled($account->uid) ? "yes" : "no");
    $notif = new TMNotification("user_chapter_announcements");
    $item[] = ($notif->isEnabled($account->uid) ? "yes" : "no");
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
    $item[] = date('j M Y',$load_user['timestamp']);

    // approved or not
    if (!in_array("approved user", $account->roles)) { 
      $item[] = "not approved";
    } else {
      $item[] = "approved";
    }

    // remove commas from any input data
    $new_item = array();
    foreach($item as $col => $val) {
      $new_item[$col] = str_replace(",", "", $val);
    }

    fputcsv($fh, $new_item);
  } 
  fclose($fh);
}

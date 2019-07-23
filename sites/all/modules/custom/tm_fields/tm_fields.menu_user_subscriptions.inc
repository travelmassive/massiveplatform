<?php

// tm_fields.menu_user_subscriptions.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_user_subscriptions_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;
  
  $base = $base_root . request_uri();

  // START View user subscription details
  if (module_exists("tm_subscriptions_user") && (!isset($field['entity']->nid) && $flag_link = flag_create_link('user_subscription_enable', $field['entity']->uid))) {

    // 1. Show Flag / Unflag link
    // only show for admin and moderator
    // chapter leaders flag sponsors via the event page but not directly
    if ((in_array('administrator', array_values($user->roles))) or (in_array('moderator', array_values($user->roles)))) {

      $links['subscription_user'][] = array(
        'wrapper_class' => array('subscription_user_set_subscription'),
        'content' => $flag_link
      );

      // 2. WHO PURCHASED
      $flag = flag_get_flag('user_subscription_paid');

      // get list of people who flagged
      $who_purchased = flag_get_entity_flags("user", $field['entity']->uid, "user_subscription_paid");

      // sort by flag time
      usort($who_purchased, function($a, $b){
        return ($a->timestamp > $b->timestamp) ? -1 : 1;
      });

      $menu_text = "";
      $purchased_user = null;
      $purchased_user_uid = null;
      foreach ($who_purchased as $flagger) {
        $difference = time() - $flagger->timestamp;
        $flagged_time = format_interval($difference, 1) . " ago";
        if ($flagger->uid == 0) { 
          $flagger_name = "Adminstrator"; // global flags used to record 0
        }
        else {
          $purchased_user = user_load($flagger->uid);
          $purchased_user_uid = $purchased_user->uid;
          $flagger_name = $purchased_user->field_user_first_name['und'][0]['value'];
        }
        $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Purchased by " . l($purchased_user->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $purchased_user->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . " " . $flagged_time . "</span><br>";
      }

      $links['subscription_user'][] = array(
        'wrapper_class' => array('subscription_user_purchased_by'),
        'content' => t($menu_text),
      );

      // 3. WHO FLAGGED
      $flag = flag_get_flag('user_subscription_enable');

      // get list of people who flagged
      $who_flagged = flag_get_entity_flags("user", $field['entity']->uid, "user_subscription_enable");

      // sort by flag time
      usort($who_flagged, function($a, $b){
        return ($a->timestamp > $b->timestamp) ? -1 : 1;
      });

      $menu_text = "";
      foreach ($who_flagged as $flagger) {
        $difference = time() - $flagger->timestamp;
        $flagged_time = format_interval($difference, 1) . " ago";
        if ($flagger->uid == 0) { 
          $flagger_name = "Administrator"; // global flags used to record 0
        }
        else {
          $flagged_user = user_load($flagger->uid);
          $flagger_name = $flagged_user->field_user_first_name['und'][0]['value'];
        }
        if ($flagged_user->uid != $purchased_user_uid) {
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Set by " . l($flagged_user->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $flagged_user->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . " " . $flagged_time . "</span><br>";
        }
      }

      $links['subscription_user'][] = array(
        'wrapper_class' => array('subscription_user_set_by'),
        'content' => t($menu_text),
      );

      // 4. SUBSCRIPTION TYPE
      $subscription_type = tm_subscriptions_user_get_subscription_type($field['entity']);
      if ($subscription_type != null) {
        $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Subscription: " . stripslashes($subscription_type) . "</span><br>";
        $links['subscription_user'][] = array(
          'wrapper_class' => array('subscription_user_info'),
          'content' => t($menu_text),
        );
      }
     

      // 4. SUBSCRIPTION END DATE
      $subscription_date = tm_subscriptions_user_get_subscription_date($field['entity']);
      if ($subscription_date != null) {

        $subscription_date_text = date('Y-m-d', strtotime($subscription_date));
        $subscription_date_text .= " (" . tm_subscriptions_user_get_subscription_expires_text($field['entity']) . ")";

        $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Expires: " . $subscription_date_text . "</span><br>";
        $links['subscription_user'][] = array(
          'wrapper_class' => array('subscription_user_expires'),
          'content' => t($menu_text),
        );
      }

      // 5. SHOW SUBSCRIPTION CTA BANNER STATUS AND PREVIEW LINK 
      $show_subscription_cta_status = "hidden";
      if ((tm_subscriptions_check_show_user_cta($field['entity']->uid, $field['entity']->uid))) { 
	      $show_subscription_cta_status = "displayed";
      }
      $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'><a class='report-flagged-link' href='/" . drupal_get_path_alias("user/" . $field['entity']->uid) . "?preview_subscription_cta'>Preview Call To Action</a> (" . $show_subscription_cta_status . ")</span><br>";
        $links['subscription_user'][] = array(
          'wrapper_class' => array('subscription_user_cta'),
          'content' => t($menu_text),
        );

    }
  }
  // END View user subscription details

  // START View user subscription payments
  if (module_exists("tm_subscriptions_user")) {
    if (isset($field['entity']->uid) and (!isset($field['entity']->nid))) {

      // Check if is moderator or administrator
      $is_moderator_or_administrator = (in_array("administrator", $user->roles) or in_array("moderator", $user->roles));

      // Check if user has made payment
      if (tm_subscriptions_is_user_subscription_paid($field['entity']->uid)) {

        // If moderator or you are looking at your own profile
        if ($is_moderator_or_administrator or ($targetuser->uid == $user->uid)) {

          $links['subscription_user'][] = array(
          'wrapper_class' => array('subscription_user_view_payments'),
          'content' => l(t('View payments'), '/payment/reports/subscription-payments-member/' . $field['entity']->uid, array(
            'attributes' => array(
              'title' => t('View payments for @account', array('@account' => $field['entity']->name))
            )
          )));

        }
      }
    }
  }
  // END View user subscription payments

}

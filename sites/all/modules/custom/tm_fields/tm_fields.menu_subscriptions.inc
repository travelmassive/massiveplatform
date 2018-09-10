<?php

// tm_fields.menu_subscriptions.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_subscriptions_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;
  
  $base = $base_root . request_uri();

  // START View subscription details
  if (module_exists("tm_subscriptions") && (isset($field['entity']->nid) && $flag_link = flag_create_link('organization_subscription_enable', $field['entity']->nid))) {

      // 1. Show Flag / Unflag link
      // only show for admin and moderator
      // chapter leaders flag sponsors via the event page but not directly
    if ((in_array('administrator', array_values($user->roles))) or (in_array('moderator', array_values($user->roles)))) {

      $links['subscription'][] = array(
        'wrapper_class' => array('subscription_set_subscription'),
        'content' => $flag_link
      );

      // 2. WHO PURCHASED
      $flag = flag_get_flag('organization_subscription_paid');

      // get list of people who flagged
      $who_purchased = flag_get_entity_flags("node", $field['entity']->nid, "organization_subscription_paid");

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
          $flagger_name = "Administrator"; // global flags used to record 0
        }
        else {
          $purchased_user = user_load($flagger->uid);
          $purchased_user_uid = $purchased_user->uid;
          $flagger_name = $purchased_user->field_user_first_name['und'][0]['value'];
        }
        $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Purchased by " . l($purchased_user->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $purchased_user->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . " " . $flagged_time . "</span><br>";
      }

      $links['subscription'][] = array(
        'wrapper_class' => array('subscription_purchased_by'),
        'content' => t($menu_text),
      );

      // 3. WHO FLAGGED
      $flag = flag_get_flag('organization_subscription_enable');

      // get list of people who flagged
      $who_flagged = flag_get_entity_flags("node", $field['entity']->nid, "organization_subscription_enable");

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

      $links['subscription'][] = array(
        'wrapper_class' => array('subscription_set_by'),
        'content' => t($menu_text),
      );

      // 4. SUBSCRIPTION TYPE
      $subscription_type = tm_subscriptions_get_subscription_type($field['entity']);
      if ($subscription_type != null) {
        $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Subscription: " . stripslashes($subscription_type) . "</span><br>";
        $links['subscription'][] = array(
          'wrapper_class' => array('subscription_info'),
          'content' => t($menu_text),
        );
      }
     

      // 4. SUBSCRIPTION END DATE
      $subscription_date = tm_subscriptions_get_subscription_date($field['entity']);
      if ($subscription_date != null) {

        $subscription_date_text = date('Y-m-d', strtotime($subscription_date));
        $subscription_date_text .= " (" . tm_subscriptions_get_subscription_expires_text($field['entity']) . ")";

        $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Expires: " . $subscription_date_text . "</span><br>";
        $links['subscription'][] = array(
          'wrapper_class' => array('subscription_expires'),
          'content' => t($menu_text),
        );
      }


    }
  }
  // END View subscription details

  // START View organization subscription payments
  if (module_exists("tm_subscriptions")) {
    if (isset($field['entity']->type) && $field['entity']->type == 'organization') {

      // Check if is moderator or administrator
      $is_moderator_or_administrator = (in_array("administrator", $user->roles) or in_array("moderator", $user->roles));

      // Check if is company manager
      if ((tm_organizations_is_company_profile_manager($user->uid, $field['entity']->nid)) or $is_moderator_or_administrator) {

        // Check if company has made payment
        if (tm_subscriptions_is_organization_subscription_paid($field['entity']->nid)) {

            $links['subscription'][] = array(
            'wrapper_class' => array('subscription_view_payments'),
            'content' => l(t('View payments'), '/payment/reports/subscription-payments/' . $field['entity']->nid, array(
              'attributes' => array(
                'title' => t('View payments for @organization', array('@organization' => $field['entity']->title))
              )
            )));

        }
      }
    }
  }
  // END View organization subscription payments

}

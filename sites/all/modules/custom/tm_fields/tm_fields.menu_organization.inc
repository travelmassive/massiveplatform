<?php

// tm_fields.menu_organization.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_organization_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;
  
  $base = $base_root . request_uri();

  // START View organization viewers
  if (module_exists("tm_track_views")) {
    if (isset($field['entity']->type) && $field['entity']->type == 'organization') {

      // Check if is moderator or administrator
      if (in_array("administrator", $user->roles) or in_array("moderator", $user->roles)) {

        // get number of viewers
        $max_view_days = $conf["tm_track_views_store_days"];
        $num_views = tm_track_views_how_many_viewed($field['entity']->nid, "node", $user->uid, $max_view_days);

        if ($num_views > 0) {

          $num_views_plural = "members";
          if ($num_views == 1) {
            $num_views_plural = "member";
          }
        
          $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'><a style='display: inline; padding: 0rem;' href='/company/" . $field['entity']->nid . "/viewers'>Viewed by " . $num_views .  " " . $num_views_plural . "</a></span>";

          $links['organization'][] = array(
            'wrapper_class' => array('organization_edit'),
            'content' => t($menu_text),
          );
        }

      }

    }
  }
  // END View organization viewers

  // START report organization profile link
  if (isset($field['entity']->type) && $field['entity']->type == 'organization') {

     // Keep track of org nid
     $organization_nid = $field['entity']->nid;

    // Don't show flag for profiles that you manage
    if (!tm_organizations_is_company_profile_manager($user->uid, $organization_nid)) {

      // Step 1. Show report flag to approved members
      if (tm_organizations_moderate_report_check($user)) {

        $community_values_url = $conf["tm_community_values_url"];

        $links['organization'][] = array(
          'wrapper_class' => array('organization_report'),
          'content' => l(t('Report'), 'javascript:void(0);',
            array(
            'fragment' => '',
            'external'=> true,
            'attributes' => array(
              'title' => t('Report'),
              'onClick' => 'javascript:jq_confirm_report_organization(' . $organization_nid . ', \'' . $community_values_url . '\')'
            )
          ))
        );
      }

      // Step 2. Show who flagged this profile to moderators and chapter leaders
      $who_flagged = array();
      if ((in_array("moderator", $user->roles) or user_access('administer'))
      or (in_array("chapter leader", $user->roles))) {

        // show last time request info was flagged
        $flag = flag_get_flag('moderate_report_organization');

        // get list of people who flagged
        $who_flagged = flag_get_entity_flags("node", $organization_nid, "moderate_report_organization");

        // sort by flag time
        usort($who_flagged, function($a, $b){
          return ($a->timestamp > $b->timestamp) ? -1 : 1;
        });

        $menu_text = "";
        foreach ($who_flagged as $flagger) {
          $difference = time() - $flagger->timestamp;
          $flagged_time = format_interval($difference, 1) . " ago";
          $flagged_user = user_load($flagger->uid);
          $menu_text .= "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>Reported by " . l($flagged_user->field_user_first_name[LANGUAGE_NONE][0]['value'], "/" . drupal_get_path_alias("user/" . $flagged_user->uid), array('attributes' => array('target' => '_blank', 'class' => array('report-flagged-link')))) . " " . $flagged_time . "</span><br>";
        }
        
        $links['organization'][] = array(
          'wrapper_class' => array('organization_reported_by'),
          'content' => t($menu_text),
        );
      }

      // Step 3. Show unflag link to moderators
      if (sizeof($who_flagged) > 0) {

        if (in_array("moderator", $user->roles) or user_access('administer')) {

          $menu_text = "<span style='font-size: smaller; font-style: italic; padding-left: 2em;'>" . l("Issue resolved", '',
              array('fragment' => 'javascript:void(0);','external'=>true, 'attributes' => array('onClick' => 'javascript:jq_confirm_report_organization_resolved(' . $organization_nid . ');', 'class' => array('report-flagged-link')))) . "</span><br>";

           $links['organization'][] = array(
            'wrapper_class' => array('organization_report_resolved'),
            'content' => t($menu_text),
          );
        }
      }

    }
  }
  // END report organization profile link

}

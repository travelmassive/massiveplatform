<?php

// tm_users.global.inc - global site methods

/**
 * Modify forms
 */
function tm_users_form_alter(&$form, $form_state, $form_id) {

  global $conf;

  if ($form_id == 'user_login') {
    $form['cutomtext'] = array(
        '#type' => 'item',
        '#markup' => '<p style="margin-top: 0.8em;">Don\'t have an account? <a href="/user/register">Sign up</a></p>',
        '#weight' => 200, // Adjust so that you can place it whereever 
        );
  }

  // Modify registration form
  if (($form_id == 'user_register_form') or ($form_id == 'tm_users_external_signin_register_form')) {
    _tm_users_modify_register_form($form, $form_state, $form_id);
  }

  // Modify profile form
  if ($form_id == 'user_profile_form') {
    _tm_users_modify_profile_form($form, $form_state, $form_id);
  }

  // Hide revision information, authoring information from moderators
  // http://drupal.stackexchange.com/questions/45339/how-to-hide-authoring-information-revision-information-publishing-options-tabs-i
  global $user;
  if (!in_array('administrator', $user->roles)) {
    if (isset($form['revision_information'])) {
      $form['revision_information']['#access'] = FALSE;
      $form['author']['#access'] = FALSE;
      $form['options']['#access'] = FALSE;
      /*$form['padding_before_save'] = array(
        '#markup' => '<br>',
        '#weight' => 10);*/
    }
  }

  // Alter community view dropdowns
  // Cache for 1 hour
  if(isset($form_state['view'])) {
    if (($form_state['view']->name == 'community') && ($form_id == "views_exposed_form")) {

      // Filter community by chapter or by country
      $tm_chapters_community_search_filter = "country";
      if (isset($conf["tm_community_search_filter"])) {
        $tm_chapters_community_search_filter = $conf["tm_community_search_filter"];
      }

      if ($tm_chapters_community_search_filter == "chapter") {

        // Filter community by chapter - show available chapters
        $output = '';
        $result = db_query("SELECT DISTINCT nid, title from {node} n WHERE n.type = 'chapter' AND n.status = 1 ORDER BY title ASC");
        $defaultoptions = array('All'=>'Chapter');
        $output = $defaultoptions + $result->fetchAllKeyed();

        // Replace the name_list options with chapters
        $form['name_list']['#options'] = $output;

        // Set default option
        $form['name_list']['#options']['All'] = "Chapter";

      } else {

        // Filter community by country - limit countries dropdown to members' countries
        $output = '';
        $output = cache_get('views-exposed-form-community-countries', 'cache');
        if (empty($output)) {
          $result = db_query("SELECT DISTINCT iso2, name from {field_data_field_user_country} f LEFT JOIN {countries_country} c ON f.field_user_country_iso2 = c.iso2 WHERE f.bundle = 'user' AND c.iso2 IS NOT NULL ORDER BY name ASC");
          $defaultoptions = array('All'=>'- Any -');
          $output = $defaultoptions + $result->fetchAllKeyed();
          $form['name_list']['#options'] = $output;
          cache_set('views-exposed-form-community-countries', $output, 'cache', time() + 3600);
        } else{
          $form['name_list']['#options'] = $output->data;
        }

        // Set default option
        $form['name_list']['#options']['All'] = "Country";

      }

      // Alter the segment dropdown
      // Hide the 2nd level of taxonomy
      if ($conf["tm_community_show_segment_2nd_level"] == false) {
        foreach ($form['taxonomy_entity_index_tid_depth']['#options'] as $key => $obj) {
          if (isset($obj->option)) {
            $option = $obj->option;
            if (strpos(current($option), "-") === 0) {
              unset($form['taxonomy_entity_index_tid_depth']['#options'][$key]);
            }
          }
        }
      }

      // Industry label
      if (isset($conf['tm_segment_label'])) {
        $form['taxonomy_entity_index_tid_depth']['#options']['All'] = $conf['tm_segment_label'];
      } else {
        $form['taxonomy_entity_index_tid_depth']['#options']['All'] = "Industry";
      }

      // Remove form description
      unset($form['taxonomy_entity_index_tid_depth']['#description']);

    }
  }
  
  // Alter leaders view dropdowns
  if (isset($form_state['view'])) {

    if ($form_state['view']->name == 'chapter_leaders') {

      // Limit countries dropdown to users countries
      $output = '';
      $output = cache_get('views-exposed-form-leaders-countries', 'cache');
      if (empty($output)) {
        $approved_member_role = user_role_load_by_name("chapter leader")->rid;
        $result = db_query("SELECT * FROM (SELECT DISTINCT iso2, name, entity_id from {field_data_field_user_country} f LEFT JOIN {countries_country} c ON f.field_user_country_iso2 = c.iso2 WHERE f.bundle = 'user' AND c.iso2 IS NOT NULL) as countries_with_users INNER JOIN {users_roles} u ON u.uid = countries_with_users.entity_id AND u.rid = " . intval($approved_member_role) . " ORDER BY name ASC");
        $defaultoptions = array('All'=>'- Any -');
        $output = $defaultoptions + $result->fetchAllKeyed();
        $form['name_list']['#options'] = $output;
      cache_set('views-exposed-form-leaders-countries', $output, 'cache', time() + 3600);
      } else{
        $form['name_list']['#options'] = $output->data;
      }

      // Replace "All" option to more meaningful name
      $form['name_list']['#options']['All'] = "Country";

    }
  }

  // disable user from choosing own contact form
  // http://stackoverflow.com/questions/2540582/how-do-i-disable-the-personal-contact-form-option-in-the-user-edit-form
  if ('user_profile_form' === $form_id) {
     $form['contact']['#access'] = FALSE;
  }

  // adjustments to the organization add form
  // hide row weights for non administrator/moderator
  // hide manager on first edit
  if ($form_id == 'organization_node_form') {
    
    // Hide row weights unless administrator or moderator
    drupal_add_js("(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){ $('.tabledrag-toggle-weight-wrapper').hide(); });})(jQuery, Drupal, this, this.document);", array('type' => 'inline', 'scope' => 'footer'));

    // Hide managed by if its first edit
    if (!(isset($form['nid']['#value']))) {
      hide($form['field_owner']);
    }

    // Add placeholder text for follower message
    $display_upvotes = $conf["tm_organizations_upvote"];
    $follow_action = "following";
    if ($display_upvotes) {
      $follow_action = "upvoting";
    }
    drupal_add_js("(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){ $('#edit-field-follower-message-und-0-value').attr('placeholder', 'Thanks for " . $follow_action . " us...'); });})(jQuery, Drupal, this, this.document);", array('type' => 'inline', 'scope' => 'footer'));
  }

  // adjustments to the event add form
  // hide row weights for non administrator/moderator
  if ($form_id == 'event_node_form') {
    
    // Hide row weights unless administrator or moderator
    drupal_add_js("(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){ $('.tabledrag-toggle-weight-wrapper').hide(); });})(jQuery, Drupal, this, this.document);", array('type' => 'inline', 'scope' => 'footer'));
  }

  // Modify chapter edit form (can't use hook in tm_chapters)
  // For non-admin and non-moderator hide the chapter leaders and geomap location fields
  if ($form_id == 'chapter_node_form') {
    global $user;
    $loaded = user_load($user->uid);
    if (!in_array("moderator", $loaded->roles) and !in_array("administrator", $loaded->roles)) {
      field_group_hide_field_groups($form, array('group_chapter_location'));
      field_group_hide_field_groups($form, array('group_community_management'));
    }

    // Hide row weights unless administrator or moderator
    drupal_add_js("(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){ $('.tabledrag-toggle-weight-wrapper').hide(); });})(jQuery, Drupal, this, this.document);", array('type' => 'inline', 'scope' => 'footer'));
  }

}

/**
 * Alter site contact form and add user's email address
 */
function tm_users_form_contact_site_form_alter(&$form, &$form_state, $form_id) {

  global $user;
  global $conf;

  // If you are logged in, disable editing name and email
  if (user_is_logged_in()) {
      $form['name']['#disabled'] = TRUE;
      $form['mail']['#disabled'] = TRUE;
  }

  // Contact form tip
  $contact_form_message = "Get in touch with " . $conf["tm_site_name"] . " and send us your ideas, comments or feedback.";
  drupal_set_message($contact_form_message, "help-tip", $repeat = FALSE);
  
  // Remove "send a copy to yourself" option from form (if required).
  $form['copy']['#access'] = FALSE;

  // get a list of chapters
  $chapters = node_load_multiple(array(), array('type' => "chapter"));

  // add chapter dropdown list
  $chapter_list = array();
  foreach ($chapters as $chapter) { // See this line in your original code
    $chapter_list[$chapter->nid] = strip_tags($chapter->title);
  }
  asort($chapter_list);

  // add contact titles
  $contact_list = array();
  $default_value = "";
  if(isset($conf["tm_contact_page_contacts"])) {
    foreach($conf["tm_contact_page_contacts"] as $value => $title) {
      $contact_list[$value] = $title;
      if ($default_value == "") {
        $default_value = $conf["tm_contact_page_contacts"][$value];
      }
    }
  } else {
    $contact_list["defaut_contact"] = "Contact us";
    $default_value = "contact_us";
  }

  // set default chapter if set from cookie
  $default_chapter_nid = null;
  if (isset($_COOKIE['Drupal_visitor_chapter_referral_id'])) {
    $chapter_referral_id = $_COOKIE['Drupal_visitor_chapter_referral_id'];
    if (isset($chapter_list[$chapter_referral_id])) {
      $default_chapter_nid = $chapter_referral_id;
      $chapter = node_load($chapter_referral_id);
      $contact_list[$chapter_referral_id] = tm_chapters_formal_name($chapter);
    }
  }

  // allow approved members to choose a chapter
  // restrict if reported
  if (user_is_logged_in() and !(tm_users_is_member_reported($user->uid))) {
    if (in_array('approved user', $user->roles)) {
      $contact_list["select_chapter"] = " - " . $conf["tm_site_name"] . " Chapters - ";
      foreach($chapter_list as $nid => $title) {
        if ($nid != $default_chapter_nid) {
          $contact_list[$nid] = $title;
        }
      }
    }
  }
  
  // add chapter list to registraton form
  $form['contact_list']=array(
    '#type'=>'select',
    '#title' =>t('Who do you want to contact?'),
    '#options' => $contact_list,
    //'#description' => "Help us notify the best person to reply to you.",
    '#multiple' => false,
    '#weight'=>-100,
    //'#default_value' => $default_value,
    '#required' => true
  );

  // set default value from url
  if (isset($_GET["default"])) {
    if (array_key_exists($_GET["default"], $contact_list)) {
      $form['contact_list']['#default_value'] = $_GET["default"];
    }
  }

  // catch spam
  $form['fullname'] = array(
    '#title'    => t('Name'),
    '#type'     => 'textfield',
    '#required' => false,
  );

  // hide spam field
  $js = "document.getElementsByClassName('form-item-fullname')[0].style.display = 'none';";
  drupal_add_js($js, "inline");

}

<?php

// tm_users.global.inc - global site methods

/**
 * Modify forms
 */
function tm_users_form_alter(&$form, $form_state, $form_id) {

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
      $form['padding_before_save'] = array(
        '#markup' => '<br>',
        '#weight' => 10);
    }
  }

  // Alter companies view dropdowns
  // Note this should be in tm_organizations but it doesn't seem to fire there
  // Cache for 1 hour
  if(isset($form_state['view'])) {
    if (($form_state['view']->name == 'companies') && ($form_id == "views_exposed_form")) {

      // Limit countries dropdown to users countries
      $output = '';
      $output = cache_get('views-exposed-form-companies-countries', 'cache');
      if (empty($output)) {
        $result = db_query("SELECT DISTINCT iso2, name from {field_data_field_country} f LEFT JOIN {countries_country} c ON f.field_country_iso2 = c.iso2 WHERE f.bundle = 'organization' AND c.iso2 IS NOT NULL ORDER BY name ASC");
        $defaultoptions = array('All'=>'- Any -');
        $output = $defaultoptions + $result->fetchAllKeyed();
        $form['name_list']['#options'] = $output;
        cache_set('views-exposed-form-companies-countries', $output, 'cache', time() + 3600);
      } else{
        $form['name_list']['#options'] = $output->data;
      }

      // Alter the segment dropdown
      // Hide the 2nd level of taxonomy
      // In the future we might allow searching deeper
      foreach ($form['term_node_tid_depth']['#options'] as $key => $obj) {
        if (isset($obj->option)) {
          $option = $obj->option;
          if (strpos(current($option), "-") === 0) {
            unset($form['term_node_tid_depth']['#options'][$key]);
          }
        }
      }

      // Replace "All" option to more meaningful name
      $form['name_list']['#options']['All'] = "Country";
      $form['term_node_tid_depth']['#options']['All'] = "Industry";

      // Remove form description
      unset($form['term_node_tid_depth']['#description']);

    }
  }

  // Alter community view dropdowns
  // Cache for 1 hour
  if(isset($form_state['view'])) {
    if (($form_state['view']->name == 'community') && ($form_id == "views_exposed_form")) {

      // Limit countries dropdown to users countries
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

      // Alter the segment dropdown
      // Hide the 2nd level of taxonomy
      // In the future we might allow searching deeper
      foreach ($form['taxonomy_entity_index_tid_depth']['#options'] as $key => $obj) {
        if (isset($obj->option)) {
          $option = $obj->option;
          if (strpos(current($option), "-") === 0) {
            unset($form['taxonomy_entity_index_tid_depth']['#options'][$key]);
          }
        }
      }

      // Replace "All" option to more meaningful name
      $form['name_list']['#options']['All'] = "Country";
      $form['taxonomy_entity_index_tid_depth']['#options']['All'] = "Industry";

      // Remove form description
      unset($form['taxonomy_entity_index_tid_depth']['#description']);

    }
  }
  
  // Alter leaders view dropdowns
  if(isset($form_state['view'])) {

    if ($form_state['view']->name == 'chapter_leaders') {

      // Limit countries dropdown to users countries
      $output = '';
      $output = cache_get('views-exposed-form-leaders-countries', 'cache');
      if (empty($output)) {
        $approved_member_role = user_role_load_by_name("approved user")->rid;
        $result = db_query("SELECT * FROM (SELECT DISTINCT iso2, name, entity_id from {field_data_field_user_country} f LEFT JOIN {countries_country} c ON f.field_user_country_iso2 = c.iso2 WHERE f.bundle = 'user' AND c.iso2 IS NOT NULL) as countries_with_users INNER JOIN {users_roles} u ON u.uid = countries_with_users.entity_id AND u.rid = " . intval($approved_member_role) . " ORDER BY name ASC");
        $defaultoptions = array('All'=>'- Any -');
        $output = $defaultoptions + $result->fetchAllKeyed();
        $form['name_list']['#options'] = $output;
      cache_set('views-exposed-form-leaders-countries', $output, 'cache', time() + 300);
      } else{
        $form['name_list']['#options'] = $output->data;
      }

      // Replace "All" option to more meaningful name
      $form['name_list']['#options']['All'] = "Country";
      
    }
  }

  // Contact form tip
  if($form_id == "contact_site_form") {
    global $conf;
    $contact_form_message = "Get in touch with " . $conf["tm_site_name"] . " and send us your ideas, comments or feedback.";
    drupal_set_message($contact_form_message, "help-tip");
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
    global $user;
    $form['#attached']['js'][] = array(
      'data' => "(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){ $('.tabledrag-toggle-weight-wrapper').hide(); });})(jQuery, Drupal, this, this.document);",
      'type' => 'inline',
    );

    // Hide managed by if its first edit
    if (!(isset($form['nid']['#value']))) {
      hide($form['field_owner']);
    }

    // Add placeholder text for follower message
    $form['#attached']['js'][] = array(
      'data' => "(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){ $('#edit-field-follower-message-und-0-value').attr('placeholder', 'Thanks for following us...'); });})(jQuery, Drupal, this, this.document);",
      'type' => 'inline',
    );

  }

  // adjustments to the event add form
  // hide row weights for non administrator/moderator
  if ($form_id == 'event_node_form') {
    
    // Hide row weights unless administrator or moderator
    $form['#attached']['js'][] = array(
      'data' => "(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){ $('.tabledrag-toggle-weight-wrapper').hide(); });})(jQuery, Drupal, this, this.document);",
      'type' => 'inline',
    );
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
    $form['#attached']['js'][] = array(
      'data' => "(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){ $('.tabledrag-toggle-weight-wrapper').hide(); });})(jQuery, Drupal, this, this.document);",
      'type' => 'inline',
    );
  }

}

/**
 * Alter site contact form and add user's email address
 */
function tm_users_form_contact_site_form_alter(&$form, &$form_state, $form_id) {

  global $conf;

  // If you are logged in, disable editing name and email
  if (user_is_logged_in()) {
      $form['name']['#disabled'] = TRUE;
      $form['mail']['#disabled'] = TRUE;
  }

  // Remove "send a copy to yourself" option from form (if required).
  $form['copy']['#access'] = FALSE;

}


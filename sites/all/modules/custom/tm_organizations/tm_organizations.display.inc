<?php

// tm_organizations.display.inc - organization display methods

/**
 * Implement hook_ds_field_info()
 */
function tm_organizations_ds_fields_info($entity_type) {
  if ($entity_type != 'node') {
    return;
  }
  $fields = array();

  // sponsor badge
  $fields['tm_ds_tm_sponsor'] = array(
    'title' => t('TM Sponsor'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('organization|full'),
    'function' => '_tm_sponsor'
  );

  // follower message
  $fields['tm_ds_organization_follow_message'] = array(
    'title' => t('Message for Followers'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('organization|*'),
    'function' => 'tm_fields_organization_follow_message',
    'properties' => array(
      'settings' => array(
        'Extra classes' => array(
          'type' => 'textfield',
          'description' => t('Classes for the link')
        )
      )
    )
  );

  // organization stats (followers)
  $fields['tm_ds_organization_stats'] = array(
    'title' => t('Organization Stats'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('organization|*'),
    'function' => '_tm_ds_organization_stats'
  );

  return array($entity_type => $fields);
}

/**
 * Render organization's follower stats
 */
function _tm_ds_organization_stats($field, $title = NULL) {

  global $conf;
  $display_upvotes = $conf["tm_organizations_upvote"];

  // check if user is flagged
  $node = $field['entity'];
  $nid = $node->nid;

  $followers_text = "Follower";
  if ($display_upvotes) { $followers_text = "Upvote"; }
  $num_followers = tm_organizations_get_num_followers($nid);

  $html = "";
  if ($num_followers > 0) {
    if ($num_followers > 1) { 
      $followers_text = "Followers";
      if ($display_upvotes) { $followers_text = "Upvotes"; }
    }
    $html = "<div class='tm_stats_container'>";
    $html .= "<a href='/node/" . $nid . "/followers'><span class='tm_stats tm_stats_followers'>" . $num_followers . " <span class='tm_stats_followers_divider'>|</span> " . $followers_text . " </span></a>";
    $html .= "</div>";
  }

  return $html;
}

/**
 * Print an indication that a company is a TM sponsor
 */
function _tm_sponsor($field, $title = NULL) {
  global $conf;
  $flag = flag_get_flag('tm_sponsor');
  if ($flag->is_flagged($field['entity']->nid)) {
    return t('A ' . $conf['tm_site_name']) . ' ' . l('sponsor', 'sponsors') . '';
  }
}

/**
 * Add javascript to show edit photo buttons
 */
function tm_organizations_add_photo_edit_js($node) {

  global $user;
  global $conf;

  // If viewing company profile, 
  if (!(arg(0) == 'node' and is_numeric(arg(1)) and arg(2) == FALSE)) {
    return;
  }

  // Check node type
  if (!isset($node->type)) {
    return;
  }

  // Check node is organization
  if ($node->type != "organization") {
    return;
  }

  // Check if company is managed by user
  $users_companies = tm_users_get_companies($user->uid);
  if (!in_array($node->nid, $users_companies)) {
    return;
  }

  // check if organization has logo and cover images
  $has_profile_image = true; // mandatory field
  $has_cover_image = false;
  if (isset($node->field_image[LANGUAGE_NONE][0]['uri'])) {
    $has_cover_image = ($node->field_image[LANGUAGE_NONE][0]['uri'] != $conf["tm_images_default_field_image"]);
  }

  // add js vars
  drupal_add_js(array('tm_organizations' => array('nid' => $node->nid)), array('type' => 'setting'));
  drupal_add_js(array('tm_organizations' => array('has_profile_image' => $has_profile_image)), array('type' => 'setting'));
  drupal_add_js(array('tm_organizations' => array('has_cover_image' => $has_cover_image)), array('type' => 'setting'));

  // add edit photo js to page
  drupal_add_js(drupal_get_path('module', 'tm_organizations') . '/js/organization-edit-photos.js');

}

/**
 * Add a link to sponsors from the companies page
 */
function tm_organizations_views_post_execute(&$view) {

  if ($view->name == 'companies') {
    $view->build_info['title'] = "Companies &middot; <a style='color: red;' href='/sponsors'>Sponsors</a>";
  }

  if ($view->name == 'sponsors') {
    $view->build_info['title'] = "<a href='/companies'>Companies</a> &middot; Sponsors";
  }

}

/**
 * Set default images
 * Refer to settings.php configuration
 */
function tm_organizations_preprocess_field(&$variables) {

  global $conf;

  // Check element type
  $element_type = null;
  if (isset($variables['element']['#object']->type)) {
    $element_type = $variables['element']['#object']->type;
  }

  // Only operate on event
  if ($element_type != "organization") {
    return;
  }

  // Replace default cover image
  if ($variables['element']['#field_name'] == 'field_image') {
    $variables['items'][0]['#item']['uri'] = _tm_organizations_get_default_image($variables['items'][0]['#item']['uri'], $variables['element']['#object']->nid);
  }
}

/**
 * Return default cover image
 * If tm_images_default_path is set then return a random image based in node id
 * URL must be in public:// format
 */
function _tm_organizations_get_default_image($default_image_uri, $nid) {

  global $conf;

  if (isset($conf["tm_images_default_path"])) {
    if (($default_image_uri == $conf["tm_images_default_field_image"]) or ($default_image_uri == "")) {
      $cover_files = $conf["tm_images_default_cover_organization"];
      $image_index = $nid % sizeof($cover_files);
      $cover_file = $conf["tm_images_default_path"] . $cover_files[$image_index];
      return $cover_file;
    }
  }

  // no random image, return what we got
  return $default_image_uri;
}

/**
 * Helper method to return organization cover uri
 * If organization has default cover, use _tm_organizations_get_default_image() to return random image
 * Otherwise, return organization image
 */
function _tm_organizations_get_image_uri($organization) {

  global $conf;

  // Return avatar if it's not the default avatar
  if (isset($organization->field_image[LANGUAGE_NONE][0]['uri'])) {
    if ($organization->field_image[LANGUAGE_NONE][0]['uri'] != $conf["tm_images_default_field_image"]) {
      return $organization->field_image[LANGUAGE_NONE][0]['uri'];
    }
  }

  // Return random avatar
  return _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $organization->nid);
}

/**
 * Helper method to return avatar uri
 * note: organization will always have a logo as it's mandatory field
 */
function _tm_organizations_get_avatar_uri($organization) {

  global $conf;
  
  $image_uri = $conf["tm_images_default_field_avatar"];
  if (isset($organization->field_avatar[LANGUAGE_NONE][0]['uri'])) {
    $image_uri = $organization->field_avatar[LANGUAGE_NONE][0]['uri'];
  }

  return $image_uri;
}

/**
 * Implement hook_preprocess_views_view
 */
function tm_organizations_preprocess_views_view(&$variables) {

  global $conf;

  $display_upvotes = $conf["tm_organizations_upvote"];
  $view = $variables['view'];

  if ($view->name == 'member_listings' && $view->current_display == 'members_following_org_page') {
    $nid = arg(1);
    $node = node_load($nid);
    $view->set_title('Members who upvoted ' . htmlentities(strip_tags($node->title)));
  }
}

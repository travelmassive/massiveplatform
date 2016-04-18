<?php

// tm_chapters.display.inc - chapter display methods

/**
 * Implement hook_ds_field_info()
 */
function tm_chapters_ds_fields_info($entity_type) {
  if ($entity_type != 'node') {
    return;
  }
  $fields = array();

  $fields['tm_ds_members_chapter'] = array(
    'title' => t('Amount chapter members'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('chapter|full'),
    'function' => '_tm_members_chapter'
  );

  return array($entity_type => $fields);
}

/**
 * Custom function to print the amount of members belonging to a chapter
 */
function _tm_members_chapter($field, $title = NULL) {
  global $conf;
  $member_label = $conf["tm_member_label"];
  $members_label = $conf["tm_members_label"];

  $flag = flag_get_flag('signup');
  $amount_members = $flag->get_count($field['entity']->nid);
  return $amount_members ? format_plural($amount_members, '1 ' . $member_label,'@members '.$members_label, array('@members' => $amount_members)) : t('No members yet');
}

/**
 * Set default images
 * Refer to settings.php configuration
 */
function tm_chapters_preprocess_field(&$variables) {
  
  global $conf;

  // Check element type
  $element_type = null;
  if (isset($variables['element']['#object']->type)) {
    $element_type = $variables['element']['#object']->type;
  }

  // Only operate on event
  if ($element_type != "chapter") {
    return;
  }

  // Replace default cover image
  if ($variables['element']['#field_name'] == 'field_image') {
    $variables['items'][0]['#item']['uri'] = _tm_chapters_get_default_image($variables['items'][0]['#item']['uri'], $variables['element']['#object']->nid);
  }

}

/**
 * Return default cover image
 * If tm_images_default_path is set then return a random image based in node id
 * URL must be in public:// format
 */
function _tm_chapters_get_default_image($default_image_uri, $nid) {

  global $conf;

  if (isset($conf["tm_images_default_path"])) {
    if (($default_image_uri == $conf["tm_images_default_field_image"])or ($default_image_uri == "")) {
      $cover_files = $conf["tm_images_default_cover_chapter"];
      $image_index = $nid % sizeof($cover_files);
      $cover_file = $conf["tm_images_default_path"] . $cover_files[$image_index];
      return $cover_file;
    }
  }

  // no random image, return what we got
  return $default_image_uri;
}

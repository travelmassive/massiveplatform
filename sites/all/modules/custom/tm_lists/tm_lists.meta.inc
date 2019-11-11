<?php

// tm_lists.meta.inc

/**
 * Add html meta tags for /lists page
 * Called from tm/themes/template.php
 */
function tm_lists_preprocess_html(&$variables, $hook) {

  global $conf;

  if (current_path() != "lists") {
    return;
  }
  
  // og:title
  if (isset($conf['tm_lists_og_title'])) {
    $meta = array(
      '#tag' => 'meta', 
      '#attributes' => array(
        'name' => 'og:title', 
        'content' => $conf['tm_lists_og_title'],
      ),
    );
    drupal_add_html_head($meta, 'og:title');
  }

  // og:image
  if (isset($conf['tm_lists_og_image'])) {
    $meta = array(
      '#tag' => 'meta', 
      '#attributes' => array(
        'name' => 'og:image', 
        'content' => $conf['tm_lists_og_image'],
      ),
    );
    drupal_add_html_head($meta, 'og:image');
  }

  // description
  if (isset($conf['tm_lists_og_description'])) {
    $meta = array(
      '#tag' => 'meta', 
      '#attributes' => array(
        'name' => 'description',
        'property' => 'og:description',
        'content' => $conf['tm_lists_og_description'],
      ),
    );
    drupal_add_html_head($meta, 'description');
  }
}

/**
 * Add meta tags for list
 */
function tm_lists_add_list_meta_tags($node) {

  // date edited
  /* ie: 
  <meta property="article:modified_time" content="2016-01-19T10:38:35-08:00" />
  <meta property="og:updated_time" content="2016-01-19T10:38:35-08:00" />*/

  // validate we have a list
  if (!isset($node->type)) {
    return;
  }

  if ($node->type != "list") {
    return;
  }

  $time_format = "Y-m-d\TH:i:s-00:00";

  // published
  $published_dt = new Datetime(null, new DateTimeZone('UTC'));
  $published_dt->setTimestamp($node->created);
  $published_time = $published_dt->format($time_format); //"2016-01-19T10:38:35-08:00";

  // modified
  $most_recent = tm_lists_get_latest_timestamp($node->nid);
  $updated_dt = new Datetime(null, new DateTimeZone('UTC'));
  $updated_dt->setTimestamp($most_recent);
  $updated_time = $updated_dt->format($time_format); //"2016-01-19T10:38:35-08:00";

  // article:published_time
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'property' => 'article:published_time', 
      'content' => $published_time,
    ),
  );
  drupal_add_html_head($meta, 'article:published_time');

  // article:modified_time
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'property' => 'article:modified_time', 
      'content' => $updated_time,
    ),
  );
  drupal_add_html_head($meta, 'article:modified_time', true);

  // og:updated_time
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'property' => 'og:updated_time', 
      'content' => $updated_time,
    ),
  );
  drupal_add_html_head($meta, 'og:updated_time');

  // og:description
  $description = strip_tags(trim($node->field_list_description[LANGUAGE_NONE][0]['value']));
  
  // use short description if available
  if (isset($node->field_list_description[LANGUAGE_NONE][0]['value'])) {
    if (trim($node->field_list_description[LANGUAGE_NONE][0]['value']) != "") {
      $description = strip_tags(trim($node->field_list_description[LANGUAGE_NONE][0]['value']));
    }
  }

  // dynamic ## item count
  $list_count = tm_lists_get_num_items($node->nid, 1);
  $description = str_replace("##", $list_count, $description);

  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'property' => 'og:description', 
      'content' => $description,
    ),
  );
  drupal_add_html_head($meta, 'og:description');

  // image
  if (isset($node->field_list_social_image[LANGUAGE_NONE][0]['uri'])) {
    $image_url = file_create_url($node->field_list_social_image[LANGUAGE_NONE][0]['uri']);
    $meta = array(
      '#tag' => 'meta', 
      '#attributes' => array(
        'property' => 'og:image', 
        'content' => $image_url,
      ),
    );
    drupal_add_html_head($meta, 'og:image');
  }

}

/**
 * Modify metatags
 */
function tm_lists_metatag_metatags_view_alter(&$output, $instance) {

  global $conf;

  // remove following metatags as we create them in tm_preprocess_html
  if (current_path() == "lists") {
    unset($output['canonical']);
    unset($output['og:url']);
    unset($output['og:title']);
  }
}


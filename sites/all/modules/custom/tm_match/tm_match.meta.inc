<?php

// tm_match.meta.inc

/**
 * Add html meta tags for /match page
 */
function tm_match_preprocess_html(&$variables, $hook) {

  global $conf;

  if (current_path() != "match") {
    return;
  }
  
  // og:title
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'name' => 'og:title', 
      'content' => $conf['tm_match_og_title'],
    ),
  );
  drupal_add_html_head($meta, 'og:title');

  // og:image
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'name' => 'og:image', 
      'content' => $conf['tm_match_og_image'],
    ),
  );
  drupal_add_html_head($meta, 'og:image');

  // description
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
      'name' => 'description',
      'property' => 'og:description',
      'content' => $conf['tm_match_og_description'],
    ),
  );
  drupal_add_html_head($meta, 'description');

  // twitter:card
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
    'property' => 'twitter:card', 
    'content' => 'summary_large_image', // or summary, for smaller image
    ),
  );
  drupal_add_html_head($meta, 'twitter:card');

  // twitter:url
  $url = $conf["tm_email_server_http"] . "://" . $conf['tm_email_server_url_domain'] . "/match";
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
    'property' => 'twitter:url', 
    'content' => $url,
    ),
  );
  drupal_add_html_head($meta, 'twitter:url');

  // twitter:title
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
    'property' => 'twitter:title', 
    'content' => $conf['tm_match_og_title'],
    ),
  );
  drupal_add_html_head($meta, 'twitter:title');

  // twitter:description
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
    'property' => 'twitter:description', 
    'content' => $conf['tm_match_og_description'],
    ),
  );
  drupal_add_html_head($meta, 'twitter:description');

  // twitter:image
  $meta = array(
    '#tag' => 'meta', 
    '#attributes' => array(
    'property' => 'twitter:image', 
    'content' => $conf['tm_match_og_image'],
    ),
  );
  drupal_add_html_head($meta, 'twitter:image');

}
  
/**
 * Modify metatags
 */
function tm_match_metatag_metatags_view_alter(&$output, $instance) {

  global $conf;

  // remove following metatags as we create them in tm_preprocess_html
  if (current_path() == "match") {
    unset($output['canonical']);
    unset($output['og:url']);
    unset($output['og:title']);
  }
}


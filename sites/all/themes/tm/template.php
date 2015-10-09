<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728096
 */


/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function tm_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  tm_preprocess_html($variables, $hook);
  tm_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
function tm_preprocess_html(&$variables, $hook) {
  // The body tag's classes are controlled by the $classes_array variable.
  if ((module_exists("tm_discuss") and (current_path() == "discuss"))) {

    global $conf;

    // add tag
    $variables['classes_array'][] = "tm_discuss";
 
    // gererate canonical url 
    $canonical = url('/discuss', array('absolute' => TRUE)) . "/";
   
    // canonical url
    $meta = array(
      '#tag' => 'link', 
      '#attributes' => array(
        'rel' => 'canonical', 
        'href' => $canonical,
      ),
    );
    drupal_add_html_head($meta, 'canonical');

    // og:url
    $meta = array(
      '#tag' => 'meta', 
      '#attributes' => array(
        'name' => 'og:url', 
        'content' => $canonical,
      ),
    );
    drupal_add_html_head($meta, 'og:url');

    // og:title
    if (isset($conf['tm_discuss_meta_og_title'])) {
      $meta = array(
        '#tag' => 'meta', 
        '#attributes' => array(
          'name' => 'og:title', 
          'content' => $conf['tm_discuss_meta_og_title'],
        ),
      );
      drupal_add_html_head($meta, 'og:title');
    }

    // og:image
    if (isset($conf['tm_discuss_meta_og_image'])) {
      $meta = array(
        '#tag' => 'meta', 
        '#attributes' => array(
          'name' => 'og:image', 
          'content' => $conf['tm_discuss_meta_og_image'],
        ),
      );
      drupal_add_html_head($meta, 'og:image');
    }

    // description
    if (isset($conf['tm_discuss_meta_description'])) {
      $meta = array(
        '#tag' => 'meta', 
        '#attributes' => array(
          'name' => 'description',
          'property' => 'og:description',
          'content' => $conf['tm_discuss_meta_description'],
        ),
      );
      drupal_add_html_head($meta, 'description');
    }

  } // end if tm_discuss and /discuss page

}

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */

function tm_preprocess_page(&$variables, $hook) {

  global $user;
  // Only a loaded user has values for the fields.
  $loaded = user_load($user->uid);

  // Hide the page title from the user profiles
  if (isset($variables['page']['content']['system_main']['#entity_type']) && $variables['page']['content']['system_main']['#entity_type'] == 'user' && isset($variables['page']['content']['system_main']['#view_mode']) && $variables['page']['content']['system_main']['#view_mode'] == 'full') {
    drupal_set_title('');
  }

  // Pass the footer and social menu to the page template
  $variables['foot_menu'] = menu_load('menu-footer-menu');
  $variables['foot_menu']['links'] = menu_navigation_links('menu-footer-menu');

  $variables['social_menu'] = menu_load('menu-social-links');
  $variables['social_menu']['links'] = menu_navigation_links('menu-social-links');

  // On registration page put a twitter link
  if (current_path() == "user/register") {
      drupal_set_message('<a href="/tm_twitter/oauth" class="twitter-login" style="margin-left: -16px; width: 220px; text-decoration: none;">Sign Up With Twitter</a>', 'signup_notice');
  }

  // Nag user to verify email if there are no other messages
  if (in_array("non-validated", $loaded->roles)) {

      // Check we haven't already displaying the validation e-mail message
      $drupal_message = drupal_get_messages('status', false);
      $hide_message = false;
      if (isset($drupal_message['status'][0])) {
        $hide_message = (strpos($drupal_message['status'][0], "A validation e-mail has been sent to your e-mail address") !== FALSE);
      }
      if (!$hide_message) {
        drupal_set_message("Please verify your email address. We sent a verification email to " . $loaded->mail . ". Didn't get it? " . l(t('Re-send it'), 'toboggan/revalidate/' . $loaded->uid) . ".", "warning", FALSE);
      }
  }

  // customize account page titles
  if (!$user->uid) {

      global $conf;

      if (arg(0) == 'user' && arg(1) == 'login') {
          drupal_set_title(t('Sign in'));
      }

      if (arg(0) == 'user' && arg(1) == 'password') {
          drupal_set_title(t('Forgot password'));
      }

      if (arg(0) == 'user' && arg(1) == 'register') {
          drupal_set_title(t('Join ' . $conf["tm_site_name"] ));
      }
    }
  
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function tm_preprocess_node(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // tm_preprocess_node_page() or tm_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function tm_preprocess_comment(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
/* -- Delete this line if you want to use this function
function tm_preprocess_region(&$variables, $hook) {
  // Don't use Zen's region--sidebar.tpl.php template for sidebars.
  //if (strpos($variables['region'], 'sidebar_') === 0) {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__sidebar'));
  //}
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function tm_preprocess_block(&$variables, $hook) {
  // Add a count to all the blocks in the region.
  // $variables['classes_array'][] = 'count-' . $variables['block_id'];

  // By default, Zen will use the block--no-wrapper.tpl.php for the main
  // content. This optional bit of code undoes that:
  //if ($variables['block_html_id'] == 'block-system-main') {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('block__no_wrapper'));
  //}
}
// */

/**
 * Add a wrapper around the search results that are being taken care of by DS Search
 *
 */

function tm_preprocess_ds_search_page(&$build) {
  $build['#prefix'] = '<section class="contained contained-block">';
  $build['#suffix'] = '</section>';
  $build['search_results']['#prefix'] = '<ul class="search-list">';
  $build['search_results']['#suffix'] = '</ul>';
}

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

  global $conf;

  // Add meta tags for theme-color for mobile Android, iOS
  // see: http://stackoverflow.com/questions/26960703/how-to-change-the-color-of-header-bar-and-address-bar-in-newest-android-chrome-v
  if (isset($conf["tm_theme_meta_color"])) {

    // Android, Firefox OS
    $meta = array(
      '#tag' => 'meta', 
      '#attributes' => array(
        'name' => 'theme-color', 
        'content' => $conf['tm_theme_meta_color'],
      ),
    );
    drupal_add_html_head($meta, 'theme-color');

    // Microsoft
    $meta = array(
      '#tag' => 'meta', 
      '#attributes' => array(
        'name' => 'msapplication-navbutton-color', 
        'content' => $conf['tm_theme_meta_color'],
      ),
    );
    drupal_add_html_head($meta, 'msapplication-navbutton-color');

    // iOS
    $meta = array(
      '#tag' => 'meta', 
      '#attributes' => array(
        'name' => 'apple-mobile-web-app-status-bar-style', 
        'content' => $conf['tm_theme_meta_color'],
      ),
    );
    drupal_add_html_head($meta, 'apple-mobile-web-app-status-bar-style');

  }

  // Add .page-payment-reports class to commissions page (tm_commissions)
  if (arg(0) == 'chapters' && (arg(2) == 'commissions' || arg(2) == 'commissions-by-region')) {
    $variables['classes_array'][] = 'page-payment-reports';
  }

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
  
  global $conf;
  global $user;

  // Workaround for gmail image proxy
  // Gmail uses a + sign for spaces, which causes a a 404
  $header = drupal_get_http_header("status");
  if(($header == "403 Forbidden") or ($header == "404 Not Found")) {
    $request_uri = $_SERVER["REQUEST_URI"];
    if (strpos($request_uri, "/sites/default/files/styles/banner/public/images/events/") === 0) {
      if (strpos($request_uri, "+") !== false) {
        drupal_goto(str_replace("+", "%20", $request_uri), array(), '301');
        return;
      }
    }
  }

  // Only a loaded user has values for the fields.
  $loaded = user_load($user->uid);

  // Hide the page title from the user profiles
  if (isset($variables['page']['content']['system_main']['#entity_type']) && $variables['page']['content']['system_main']['#entity_type'] == 'user' && isset($variables['page']['content']['system_main']['#view_mode']) && $variables['page']['content']['system_main']['#view_mode'] == 'full') {
    drupal_set_title('');
  }

  // On registration page put a twitter link
  if (current_path() == "user/register") {

    // check not rendering custom register page
    if (isset($conf["tm_users_custom_signup_page"])) {
      if ($conf["tm_users_custom_signup_page"]) {
        return;
      }
    }

    $message = "";
    if (isset($conf["tm_signin_facebook"])) {
      if ($conf["tm_signin_facebook"]) {
        $message .= '<a href="/user/simple-fb-connect" class="facebook-login" style="margin-left: -16px; width: 220px; text-decoration: none; display: inline-block; margin-right: 1.5rem; margin-top: 0.25rem; margin-bottom: 0.25rem;">' . $conf['tm_signin_facebook_label'] . '</a>';
      }
    }
    if (isset($conf["tm_signin_twitter"])) {
      if ($conf["tm_signin_twitter"]) {
        $message .= ' <a href="/tm_twitter/oauth" class="twitter-login" style="margin-left: -16px; width: 220px; text-decoration: none; display: inline-block; margin-right: 1.5rem; margin-top: 0.25rem; margin-bottom: 0.25rem;">' . $conf['tm_signin_twitter_label'] . '</a>';
      }
    }
    if ($message != "") {
      drupal_set_message($message, 'signup_notice');
    }
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

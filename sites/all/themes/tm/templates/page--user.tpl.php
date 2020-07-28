<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 * 
 * CUSTOMIZATIONS:
 * - removed section, header and column inside the content div, since these will be handled by the DS layout
 */
?>

<?php

  global $conf;

  // Enable discussion feedme plugin
  // Show recent discussion posts
  if (@isset($conf["tm_discuss_enable_feedme"])) {
    if ($conf["tm_discuss_enable_feedme"]) {
      include './'. path_to_theme() .'/templates/page--discuss-feedme.tpl.php';
    }
  }
  
  // Enable wordpress feedme plugin
  // Show recent blog posts and more
  if (@isset($conf["tm_enable_wordpress_feedme"])) {
    if ($conf["tm_enable_wordpress_feedme"]) {
      include './'. path_to_theme() .'/templates/page--wordpress-feedme.tpl.php';
    }
  }

  // Enable lists feedme plugin
  // Show related lists
  if (module_exists("tm_lists")) {
    include './'. path_to_theme() .'/templates/page--lists-feedme.tpl.php';
  }

  // Configure navbar to position in the header
  $tm_branding_navbar_top = false;
  if (isset($conf["tm_branding_navbar_top"])) {
    $tm_branding_navbar_top = $conf["tm_branding_navbar_top"];
  }

  // Custom header template
  $custom_sidebar_template = "";
  if (isset($conf["tm_theme_custom_sidebar_template"])) {
    $custom_sidebar_template = $conf["tm_theme_custom_sidebar_template"];
  }

?>

<?php
// Redirect /user/uid paths to canonical URL
// Check canonical URL is not current URL
$request_uri_parts = explode("/", request_uri());
if (sizeof($request_uri_parts) == 3) {
  if (($request_uri_parts[1] == "user") and (is_numeric($request_uri_parts[2]))) {
    $profile_url = drupal_get_path_alias("user/" . $request_uri_parts[2]);
    if ("/" . $profile_url != request_uri()) {
      drupal_goto($profile_url); // Redirect to canonical URL
    }
  }
}
?>

<div id="page">

  <?php
  if ($custom_sidebar_template != "") { 
    // load custom header
    include './'. path_to_theme() . '/templates/' . $custom_sidebar_template;
  } else { 
  ?>
  <header class="header" id="header" role="banner">
    <div class="row">
      <h1 id="site-title">
        <a title="<?php print t('Home'); ?>" rel="home" href="<?php print $front_page; ?>">
          <img class="header-logo" src="<?php echo tm_branding_get_element("header_logo");?>" alt="<?php print t('Home'); ?>" width="104" height="48" />
          <span><?php print $site_name; ?></span>
        </a>
      </h1>
      <nav id="prime-nav" role="navigation">
        <h1><?= t('Primary navigation'); ?></h1>

        <?php if ($tm_branding_navbar_top) { ?>
        <div class="top-navbar top-navbar-header">
          <div class="row">
            <section>
              <?php echo tm_branding_get_element("navbar_html"); ?>
            </section>
          </div>
        </div>
        <?php } ?>

         <div class='topnav-search-container <?php if ($tm_branding_navbar_top) { ?>top-navbar-header<?php } ?>'>
          <div class='topnav-search-inner'>
            <i class='topnav-search-icon'></i>
            <form method="GET" action="/search"><input type='text' id='topnav-search-textfield' name='query' placeholder='<?php print $conf['tm_search_api_placeholder_text'];?>' value='' size='40' maxlength='255' class='form-text' autocomplete='off'></form>
          </div>
        </div>

        <ul class="prime-nav-wrappers">
          <li class="browse-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#topnav-links-dropdown" data-dropd-toggle><span class="hide"><?= t('Browse'); ?></span></a></h2>
            <div id="topnav-links-dropdown" class="inner dropd dropd-right" data-dropd>
              <?php echo tm_branding_get_element("menu_html"); ?>
            </div>
          </li>
          <?php if (module_exists("tm_status_updates") and ($conf["tm_status_updates_enabled"])) : ?>
          <li class="newsfeed-wrapper">
            <h2><a class="toggle" href="/newsfeed"><span class="hide"><?= t('Newsfeed'); ?></span></a></h2>
          </li>
          <?php endif; ?>
          <li class="account-wrapper" data-dropd-wrapper>
            <?php print($page['account_menu']); ?>
          </li>
        </ul>
      </nav>
    </div>
  </header>
  <?php } // end if custom template ?>

  <div class="top-navbar-divider"></div>

  <?php if (!$tm_branding_navbar_top) { ?>
  <div class="top-navbar">
    <div class="row">
      <section>
        <?php echo tm_branding_get_element("navbar_html"); ?>
      </section>
    </div>
  </div>
  <?php } ?>

  <?php

  // Custom Messages

  // TM_SUBSCRIPTIONS_USER CTA
  // check that module is enabled and user is logged in
  $subscription_cta = "";
  if (module_exists("tm_subscriptions_user")) {
    if ($user->uid > 0) {
      if (isset($tm_theme_user_id)) { 
        $subscription_cta = tm_subscriptions_user_cta_banner($tm_theme_user_id);
      }
    }
  }

  
  // Check to see if viewing own profile
  $is_viewing_own_profile = false;
  if ($user->uid > 0) {
    if (isset($tm_theme_user_id)) { 
      if ($user->uid == $tm_theme_user_id) {
        $is_viewing_own_profile = true;
      }
    }
  }

  // TOP_BLOCK BRANDING
  // only render if no subscription cta
  // also check for exclusion of url in $conf['tm_branding_hide_top_block_on_urls']
  $top_block_html = "";
  $show_top_block_html = !$is_viewing_own_profile;
  if ($subscription_cta == "") {

    // hide top block for specific URL path
    if (isset($conf['tm_branding_hide_top_block_on_urls'])) {
      $url_path = explode("?", $_SERVER["REQUEST_URI"])[0];
      foreach($conf['tm_branding_hide_top_block_on_urls'] as $check_url) {
        if (strpos($url_path, $check_url) !== false) {
          $show_top_block_html = false;
        }
      }
    }

    // render top block
    if ($show_top_block_html) {
      $top_block_html = tm_branding_get_element("top_block_html");
    }
    
  }

  // render top block
  print $top_block_html;

  ?>

  <main id="main" role="main">
    <div class="row">
      <div id="content" role="main">
        <div class="header-wrapper">
          <a id="main-content"></a>
          <?php print render($title_prefix); ?>
          <?php if ($title): ?>
            <h1 class="page__title title" id="page-title"><?php print $title; ?></h1>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php print render($page['header']); ?>
          <?php print $messages; ?>
          <?php print $subscription_cta; ?>
          <?php if(module_exists("tm_status_updates")) { print tm_status_updates_render_theme(); } ?>
          <?php //print render($tabs); ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?>
            <ul class="action-links"><?php print render($action_links); ?></ul>
          <?php endif; ?>
        </div>
        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>
      </div>
    </div>
  </main>

  <footer id="footer" role="contentinfo">
    <?php 
    // only show on view
    if ((arg(1) == "") or (is_numeric(arg(1)) and (arg(2) == ""))) {

      // show footer level 2 branding
      $show_footer_level2_html = true;

      // hide for specific URL path
      if (isset($conf['tm_branding_hide_footer_level2_on_urls'])) {
        $url_path = explode("?", $_SERVER["REQUEST_URI"])[0];
        foreach($conf['tm_branding_hide_footer_level2_on_urls'] as $check_url) {
          if (strpos($url_path, $check_url) !== false) {
            $show_footer_level2_html = false;
          }
        }
      }

      // display footer level 2
      if ($show_footer_level2_html) {
        echo tm_branding_get_element("footer_level2_html");
      }
      
    } 
    ?>
    <?php echo tm_branding_get_element("footer_html"); ?>
  </footer>
</div>
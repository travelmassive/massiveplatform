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

  // Enable discussion feedme plugin
  // Show recent discussion posts
  global $conf;
  if (@isset($conf["tm_discuss_enable_feedme"])) {
    if ($conf["tm_discuss_enable_feedme"]) {
      include './'. path_to_theme() .'/templates/page--discuss-feedme.tpl.php';
    }
  }

  // Enable wordpress feedme plugin
  // Show recent blog posts and more
  global $conf;
  if (@isset($conf["tm_enable_wordpress_feedme"])) {
    if ($conf["tm_enable_wordpress_feedme"]) {
      include './'. path_to_theme() .'/templates/page--wordpress-feedme.tpl.php';
    }
  }

  // Enable marketplace feedme plugin
  // Show recent marketplace posts
  global $conf;
  if (@isset($conf["tm_marketplace_enable_feedme"])) {
    if ($conf["tm_marketplace_enable_feedme"]) {
      include './'. path_to_theme() .'/templates/page--marketplace-feedme.tpl.php';
    }
  }

?>

<?php
// Redirect /node/nid paths to canonical URL
// Check canonical URL is not current URL
$request_uri_parts = explode("/", request_uri());
if (sizeof($request_uri_parts) == 3) {
  if (($request_uri_parts[1] == "node") and (is_numeric($request_uri_parts[2]))) {
    $clean_url = drupal_get_path_alias("node/" . $request_uri_parts[2]);
    if ("/" . $clean_url != request_uri()) {
      drupal_goto($clean_url); // Redirect to canonical URL
    }
  }
}
?>

<div id="page">

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

        <div class='topnav-search-container'>
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

  <div class="top-navbar">
    <div class="row">
      <section>
        <?php echo tm_branding_get_element("navbar_html"); ?>
      </section>
    </div>
  </div>

  <?php
    // render top block
    $top_block_html = tm_branding_get_element("top_block_html");
    if ($top_block_html != "") { echo $top_block_html; }
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
          <?php

          // custom call to action banner for events
          // display on view mode, and supresses any subscription CTAs
          $custom_banner = "";
          if ((arg(1) == "") or (is_numeric(arg(1)) and (arg(2) == ""))) {
            if (isset($tm_theme_node_id)) {
              if ($node->type == "event") {
                $custom_banner = tm_events_get_custom_banner($node);
              }
              print $custom_banner;
            }
          }

          // tm_subscriptions call to action banner
          // only shows on organization pages 
          // check that module is enabled and user is logged in
          $subscription_cta = "";
          if (module_exists("tm_subscriptions") and ($custom_banner == "")) {
            if ($user->uid > 0) {
              if (isset($tm_theme_node_id)) {
                if (tm_subscriptions_check_show_organization_cta($tm_theme_node_id, $user->uid)) {
                  $subscription_cta = tm_subscriptions_organization_cta_banner($tm_theme_node_id);
                  print $subscription_cta;
                }
              }
            }
          }

          // tm_subscriptions_user call to action banner
          // displays on events and chapter pages
          // check that module is enabled and user is logged in
          if (module_exists("tm_subscriptions_user") and ($custom_banner == "")) {
            if ($user->uid > 0) {
              if (isset($tm_theme_node_id)) {
                $subscription_cta = tm_subscriptions_user_cta_banner_node($user->uid, $tm_theme_node_id);
                print $subscription_cta;
              }
            }
          }
          
          ?> 
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

      // hide on events
      if ($node->type == "event") {
        if (isset($conf["tm_branding_hide_footer_level2_on_events"])) {
          if ($conf["tm_branding_hide_footer_level2_on_events"]) {
            $show_footer_level2_html = false;
          }
        }
      }

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
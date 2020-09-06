<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */
?>

<?php

  global $conf;

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

<div id="page">

  <?php
  if ($custom_sidebar_template != "") { 
    // load custom header
    include './'. path_to_theme() . '/templates/' . $conf["tm_theme_custom_sidebar_template"];
  } else { 
  ?>
  <header class="header" id="header" role="banner" <?php if ($is_front) {?>style="opacity: 0.9;"<?php } ?>>
    <div class="row">
      <h1 id="site-title">
        <a title="<?php print t('Home'); ?>" rel="home" href="<?php print $front_page; ?>">
          <img class='header-logo' src="<?php echo tm_branding_get_element("header_logo");?>" alt="<?php print t('Home'); ?>" width="104" height="48" />
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

        <div class='topnav-search-container <?php if ($tm_branding_navbar_top) { ?>top-navbar-header<?php } ?>' <?php if ((arg(0) == "search") or (arg(0) == "insights")) { echo "style='display: none;'"; } ?>>
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

  // TOP_BLOCK BRANDING
  // also check for exclusion of url in $conf['tm_branding_hide_top_block_on_urls']
  $top_block_html = "";
  $show_top_block_html = true;
  
  // hide top block for specific URL path
  if (isset($conf['tm_branding_hide_top_block_on_urls'])) {

    // special case check for front page
    if (drupal_is_front_page() and in_array("__FRONTPAGE__", $conf['tm_branding_hide_top_block_on_urls'])) {
      $show_top_block_html = false;
    } else {
      // check for url
      $url_path = explode("?", $_SERVER["REQUEST_URI"])[0];
      foreach($conf['tm_branding_hide_top_block_on_urls'] as $check_url) {
        if (strpos($url_path, $check_url) !== false) {
          $show_top_block_html = false;
        }
      }
    }
  }

  // render top block
  if ($show_top_block_html) {
    $top_block_html = tm_branding_get_element("top_block_html");
  }

  // display top block
  print $top_block_html;

  ?>

  <main id="main" role="main">
    <div class="row">
      <div id="content" role="main">
        <section>
          <header>
            <a id="main-content"></a>
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
              <h1 class="page__title title" id="page-title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php print render($page['header']); ?>
            <?php print render($tabs); ?>
            <?php print $messages; ?>
            <?php print render($page['help']); ?>
            <?php if ($action_links): ?>
              <ul class="action-links"><?php print render($action_links); ?></ul>
            <?php endif; ?>
          </header>

          <div class="column">

            <?php print render($page['content']); ?>

            <?php 
            // FRONT PAGE ITEMS
            if (drupal_is_front_page()) {
            ?>

            <div class="trilithon" id="frontpage_feed" style="margin-top: 64px;">

<?php

// Render front page feeds

// Check discussions feed enabled
$tm_frontpage_discussions_enabled = false;
if (@isset($conf["tm_discourse_frontpage_feed"])) {
  $tm_frontpage_discussions_enabled = $conf["tm_discourse_frontpage_feed"];
}

// Check marketplace feed enabled
$tm_frontpage_marketplace_enabled = false;
if (@isset($conf["tm_marketplace_frontpage_url"])) {
  $tm_frontpage_marketplace_enabled = true;
}

// Check wordpress feed enabled
$tm_frontpage_wordpress_enabled = false;
if (@isset($conf["tm_enable_wordpress_feedme"])) {
  $tm_frontpage_wordpress_enabled = $conf["tm_enable_wordpress_feedme"];
}

// Check status updates enabled
$tm_frontpage_status_updates_enabled = false;
if (@isset($conf["tm_status_updates_frontpage_feed"])) {
  $tm_frontpage_status_updates_enabled = $conf["tm_status_updates_frontpage_feed"];
}

// wordpress and status updates
$tm_frontpage_discussions_classes = "trilithon-header contained";
$tm_frontpage_marketplace_classes = "trilithon-header contained";
if ($tm_frontpage_discussions_enabled and $tm_frontpage_marketplace_enabled) {
  $tm_frontpage_discussions_classes = "column first";
  $tm_frontpage_marketplace_classes = "column second";
}

// wordpress and status updates
$tm_frontpage_wordpress_classes = "trilithon-header contained";
$tm_frontpage_status_updates_classes = "tm-frontpage-feed-fullwidth contained";
if ($tm_frontpage_wordpress_enabled and $tm_frontpage_status_updates_enabled) {
  $tm_frontpage_wordpress_classes = "column third";
  $tm_frontpage_status_updates_classes = "column second";
}

?>

            <?php if ($tm_frontpage_discussions_enabled) { ?>
              <div class="<?php echo($tm_frontpage_discussions_classes);?>">
                <div id="frontpage_discussions_feed">
                  <?php print(tm_newsfeed_discourse_render_frontpage_feed($conf["tm_discourse_frontpage_limit"]));?>
                </div>
              </div>
            <?php } // end if ?>

            <?php if ($tm_frontpage_marketplace_enabled) { ?>
              <div class="<?php echo($tm_frontpage_marketplace_classes);?>">
                <div id="frontpage_marketplace_feed">
                  <section id="feedme-placeholder" class="contained contained-block feedme-marketplace tm-preloading-background"></section>
                </div>
              </div>
              <?php include './'. path_to_theme() .'/templates/page--marketplace-frontpage.tpl.php';?></div>
            <?php } // end if ?>

            </div><div class="trilithon" id="frontpage_feed_row_2">
            
            <?php if ($tm_frontpage_wordpress_enabled) { ?>
              <div class="<?php echo($tm_frontpage_wordpress_classes);?>" id="frontpage_wordpress_feed">
                <section id="feedme-placeholder" class="contained contained-block feedme tm-preloading-background"></section>
              </div>
            <?php include './'. path_to_theme() .'/templates/page--wordpress-feedme.tpl.php'; ?>
            <?php } // end if ?>

            <?php if ($tm_frontpage_status_updates_enabled) { ?>
              <div class="<?php echo($tm_frontpage_status_updates_classes);?>" id="frontpage_flag_feed">
                <?php print(tm_status_updates_show_frontpage_updates()); ?>
              </div> <!-- close second column -->
            <?php } // end if ?>
              
            </div> <!-- close trilithon -->

            <?php } // end if front page ?>

          </div> <!-- close column -->

        </section>
      </div>
    </div>
  </main>

  <footer id="footer" role="contentinfo">
    <?php 

      // front and secondary footers
      if ($is_front) {
        echo tm_branding_get_element("footer_level1_html");
      } else {
        echo tm_branding_get_element("footer_level2_html");
      }

      // footer
      echo tm_branding_get_element("footer_html");
    ?>
  </footer>
</div>

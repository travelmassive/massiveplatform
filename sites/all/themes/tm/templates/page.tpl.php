<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */

 global $conf;
?>

<div id="page">

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

        <div class='topnav-search-container' <?php if (arg(0) == "search") { echo "style='display: none;'"; } ?>>
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

            <div class="trilithon" id="frontpage_feed" style="margin-top: 64px;">

            <?php
            // Enable wordpress feedme plugin
            // Show recent blog posts and more
            $tm_enable_wordpress_feedme = false;
            if (@isset($conf["tm_enable_wordpress_feedme"])) {
              if ($conf["tm_enable_wordpress_feedme"]) {
                $tm_enable_wordpress_feedme = true;
            ?>
              <div class="column first" id="frontpage_wordpress_feed" style="float: left;"></div>
            <?php
                include './'. path_to_theme() .'/templates/page--wordpress-feedme.tpl.php';
              } // end if
            } // end if
            ?>

            <?php
            // only show flag feeds if enabled and on front page
            $show_flag_feeds = false;
            if (@isset($conf["tm_status_updates_frontpage_feed"])) {
              if ($conf["tm_status_updates_frontpage_feed"] and drupal_is_front_page()) {
                $show_flag_feeds = true;
              }
            }
            
            if ($show_flag_feeds) {
            ?>
              <div class="<?php if($tm_enable_wordpress_feedme) { print("column second"); }?>" id="frontpage_flag_feed" style="float: right;">
                
            <?php print(tm_status_updates_show_frontpage_updates()); ?>
              </div> <!-- close second column -->
            <?php
              } // end if show flag feeds
            ?>

            </div> <!-- close triliton -->

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

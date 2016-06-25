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
        <ul class="prime-nav-wrappers">
          <?php if ($main_menu): ?>
          <li class="browse-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#browse-menu-blk" data-dropd-toggle><span class="hide"><?= t('Browse'); ?></span></a></h2>
            <div id="browse-menu-blk" class="inner dropd dropd-right" data-dropd>
              <?php echo tm_branding_get_element("menu_html"); ?>
            </div>
          </li>
          <?php endif; ?>
          <?php if (module_exists("tm_status_updates") and ($conf["tm_status_updates_enabled"])) : ?>
          <li class="newsfeed-wrapper">
            <h2><a class="toggle" href="/newsfeed"><span class="hide"><?= t('Newsfeed'); ?></span></a></h2>
          </li>
          <?php endif; ?>
          <li class="search-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#search-menu-blk" data-dropd-toggle><span class="hide"><?= t('Search'); ?></span></a></h2>
            <div id="search-menu-blk" class="inner dropd dropd-right" data-dropd>
              <?php
                // display search form in menu
                $search_form ="<p>Site search disabled</p>";
                if (module_exists("tm_search_api")) {
                  global $tm_search_api_form_template;
                  $search_form = $tm_search_api_form_template;
                } elseif (module_exists("search")) { 
                  $sf = drupal_get_form('search_form');
                  $search_form = render($sf);
                }
                print $search_form;
              ?>
            </div>
          </li>
          <li class="account-wrapper" data-dropd-wrapper>
            <?php print($page['account_menu']); ?>
          </li>
        </ul>
      </nav>
    </div>
  </header>

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
    <?php echo tm_branding_get_element("footer_html"); ?>
    <?php echo tm_branding_get_element("footer_level2_html"); ?>
  </footer>
</div>
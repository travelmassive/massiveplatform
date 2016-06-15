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
        <ul class="prime-nav-wrappers">
          <?php if ($main_menu): ?>
          <li class="browse-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#browse-menu-blk" data-dropd-toggle><span class="hide"><?= t('Browse'); ?></span></a></h2>
            <div id="browse-menu-blk" class="inner dropd dropd-right" data-dropd>
              <?php echo tm_branding_get_element("menu_html"); ?>
            </div>
          </li>
          <?php endif; ?>
          <?php if (module_exists("tm_status_updates") and user_is_logged_in()) : ?>
          <li class="newsfeed-wrapper">
            <h2><a class="toggle" href="/newsfeed"><span class="hide"><?= t('Newsfeed'); ?></span></a></h2>
          </li>
          <?php endif; ?>
          <li class="search-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#search-menu-blk" data-dropd-toggle><span class="hide"><?= t('Search'); ?></span></a></h2>
              <div id="search-menu-blk" class="inner dropd dropd-right" data-dropd>
            <?php if (module_exists("tm_discuss") && (current_path() == $conf['tm_discuss_menu_path'])) { ?>
              
                <form class="search-form" action="/<?php print($conf["tm_discuss_menu_path"]);?>/#/search" method="get" id="search-form" accept-charset="UTF-8">
                  <div>
                    <div class="container-inline form-wrapper" id="edit-basic">
                      <div class="form-item form-type-textfield form-item-keys">
                        <label for="edit-keys">Enter your keywords </label>
                        <input type="text" id="Search" name="Search" value="" size="40" maxlength="255" class="form-text">
                      </div>
                      <input type="submit" id="edit-submit--2" value="Search" class="form-submit" onClick="javascript:document.location.href='/<?php print($conf["tm_discuss_menu_path"]);?>/#search?Search='+document.getElementById('Search').value; return false;">
                    </div>
                  </div>
                </form>
                <p class="helper">Search discussions and comments.</p>
              <!-- https://localdev.travelmassive.com/discuss/#/search?Search=foo -->

            <?php } else { ?>
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
              <?php } // end else  ?>
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
            // frontpage block
            if ($is_front) {
            $frontpage_block_html = tm_branding_get_element("frontpage_block_html");
            if ($frontpage_block_html != "") { ?>
            <div class="column first" style='margin: auto; text-align: center;' id="frontpage_block" style="float: left;">
              <?php echo $frontpage_block_html; ?>
            </div>
            <?php 
              } // end if frontpage_block_html
            } // end if is front
            ?>

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
            if (@isset($conf["tm_enable_flags_feed"])) {
              if ($conf["tm_enable_flags_feed"] and drupal_is_front_page()) {
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
      echo tm_branding_get_element("footer_html");
    if ($is_front) {
      echo tm_branding_get_element("footer_level1_html");
    } else {
      echo tm_branding_get_element("footer_level2_html");
    }
    ?>
  </footer>
</div>

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

<div id="page">

  <header class="header" id="header" role="banner">
    <div class="row">
      <h1 id="site-title">
        <a title="<?php print t('Home'); ?>" rel="home" href="<?php print $front_page; ?>">
          <img src="<?php print $base_path . path_to_theme(); ?>/images/layout/tm-logo.svg" alt="<?php print t('Home'); ?>" width="104" height="48" />
          <span><?php print $site_name; ?></span>
        </a>
      </h1>
      <nav id="prime-nav" role="navigation">
        <h1><?= t('Primary navigation'); ?></h1>
        <ul>
          <?php if ($main_menu): ?>
          <li class="browse-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#browse-menu-blk" data-dropd-toggle><span class="hide"><?= t('Browse'); ?></span></a></h2>
            <div id="browse-menu-blk" class="inner dropd dropd-right" data-dropd>
              <?php
              print theme('links__system_main_menu', array(
                'links' => $main_menu,
                'attributes' => array(
                  'class' => array('links'),
                ),)); ?>
            </div>
          </li>
          <?php endif; ?>
          <li class="search-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#search-menu-blk" data-dropd-toggle><span class="hide"><?= t('Search'); ?></span></a></h2>
            <div id="search-menu-blk" class="inner dropd dropd-right" data-dropd>
              <?php $sf = drupal_get_form('search_form'); print render($sf); ?>
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
    <div class="row">
      <nav id="foot-nav" role="navigation">
        <div class="inner">

        <?php include './'. path_to_theme() .'/templates/block--footer-links.tpl.php';?>

        <!--<?php if ($foot_menu['links']): ?>
          <section class="foot">
          <?php
            print theme('links', array(
              'links' => $foot_menu['links'],
              'heading' => array(
                'text' => $foot_menu['title'],
                'level' => 'h2',
              ),
            )); 
          ?>
          </section>
        <?php endif; ?>-->
        <?php if ($social_menu['links']): ?>
          <section class="social">
          <?php
            print theme('links', array(
              'links' => $social_menu['links'],
              'heading' => array(
                'text' => $social_menu['title'],
                'level' => 'h2',
              ),
            )); 
          ?>
          </section>
        <?php endif ?>
        </div>
      </nav>
      <div id="foot-credits">
        <p>
          <strong class="logo">
            <a title="<?php print t('Home'); ?>" rel="home" href="<?php print $front_page; ?>">
              <img src="<?php print $base_path . path_to_theme(); ?>/images/layout/tm-logo.svg" alt="<?php print t('Home'); ?>" width="104" height="48" />
              <span><?php print $site_name; ?></span>
            </a>
          </strong>
          <small><time datetime="2011/<?= date('Y'); ?>">&copy; 2011-<?= date('Y'); ?> All Rights Reserved</time>Travel Massive Global P.B.C.</small>
        </p>
        <p>Powered by Massive Platform<br>
          Made with <span style='color: red;'>&hearts;</span> around the world</p>
      </div>
    </div>
  </footer>
</div>
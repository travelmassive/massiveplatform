<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
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
          <li class="browse-wrapper">
            <h2><a class="toggle" href="#"><span><?= t('Browse'); ?></span></a></h2>
            <div class="inner">
              <?php
              print theme('links__system_main_menu', array(
                'links' => $main_menu,
                'attributes' => array(
                  'class' => array('links'),
                ),)); ?>
            </div>
          </li>
          <?php endif; ?>
          <li class="search-wrapper">
            <h2><a class="toggle" href="#"><span><?= t('Search'); ?></span></a></h2>
            <div class="inner">
              <form>
                <input name="" type="search" />
                <input name="" value="submit" type="submit">
              </form>
            </div>
          </li>
          <li class="account-wrapper">
            <h2><a class="toggle" href="#"><span><?= t('Account'); ?></span></a></h2>
            <div class="inner">
              <p>Lorem ipsum dolor sit amet.</p>
            </div>
          </li>
        </ul>
      </nav>

      <?php print render($page['header']); ?>
    </div>
  </header>

  <main id="main">
    <div class="row">
      <div id="content" role="main">
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1 class="page__title title" id="page-title"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php print $messages; ?>
        <?php print render($tabs); ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>
      </div>

    </div>
  </main>

  <?php print render($page['footer']); ?>

</div>
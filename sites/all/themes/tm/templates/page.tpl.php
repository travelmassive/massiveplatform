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
          <li class="browse-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#" data-dropd-toggle><span><?= t('Browse'); ?></span></a></h2>
            <div class="inner dropd dropd-right" data-dropd>
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
            <h2><a class="toggle" href="#" data-dropd-toggle><span><?= t('Search'); ?></span></a></h2>
            <div class="inner dropd dropd-right" data-dropd>
              <form>
                <input name="" type="search" />
                <input name="" value="submit" type="submit">
              </form>
            </div>
          </li>
          <li class="account-wrapper" data-dropd-wrapper>
            <h2><a class="toggle" href="#" data-dropd-toggle><span><?= t('Account'); ?></span></a></h2>
            <div class="inner dropd dropd-right" data-dropd>
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

  <footer id="footer">
    <div class="row">
      <nav id="foot-nav" role="navigation" tabindex="-1">
      <?php if ($foot_menu['links']): ?>
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
      <?php endif; ?>
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
      </nav>
      <div class="logo">
        <img src="<?php print $base_path . path_to_theme(); ?>/images/layout/tm-logo.svg" alt="<?php print t('Home'); ?>" width="104" height="48" />
      </div>
      <div id="copyright">
        <?php print variable_get_value('tm_base_copright'); ?><br/>
        2011 - <?php print date('Y'); ?>
      </div>
    </div>
  </footer>
</div>
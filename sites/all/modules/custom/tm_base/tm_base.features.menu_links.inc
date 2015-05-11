<?php
/**
 * @file
 * tm_base.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function tm_base_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_chapters:chapters
  $menu_links['main-menu_chapters:chapters'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'chapters',
    'router_path' => 'chapters',
    'link_title' => 'Chapters',
    'options' => array(
      'identifier' => 'main-menu_chapters:chapters',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: main-menu_companies:companies
  $menu_links['main-menu_companies:companies'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'companies',
    'router_path' => 'companies',
    'link_title' => 'Companies',
    'options' => array(
      'identifier' => 'main-menu_companies:companies',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
  );
  // Exported menu link: main-menu_discuss:https:/discuss/
  $menu_links['main-menu_discuss:https:/discuss/'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'https:/discuss/',
    'router_path' => '',
    'link_title' => 'Discuss',
    'options' => array(
      'attributes' => array(),
      'item_attributes' => array(
        'id' => '',
        'class' => '',
        'style' => '',
      ),
      'identifier' => 'main-menu_discuss:https:/discuss/',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
  );
  // Exported menu link: main-menu_events:events
  $menu_links['main-menu_events:events'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'events',
    'router_path' => 'events',
    'link_title' => 'Events',
    'options' => array(
      'identifier' => 'main-menu_events:events',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: main-menu_leaders:leaders
  $menu_links['main-menu_leaders:leaders'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'leaders',
    'router_path' => 'leaders',
    'link_title' => 'Leaders',
    'options' => array(
      'identifier' => 'main-menu_leaders:leaders',
    ),
    'module' => 'system',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: main-menu_news:https:/blog/
  $menu_links['main-menu_news:https:/blog/'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'https:/blog/',
    'router_path' => '',
    'link_title' => 'News',
    'options' => array(
      'attributes' => array(),
      'item_attributes' => array(
        'id' => '',
        'class' => '',
        'style' => '',
      ),
      'identifier' => 'main-menu_news:https:/blog/',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
  );
  // Exported menu link: main-menu_people:community
  $menu_links['main-menu_people:community'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'community',
    'router_path' => 'community',
    'link_title' => 'People',
    'options' => array(
      'attributes' => array(),
      'item_attributes' => array(
        'id' => '',
        'class' => '',
        'style' => '',
      ),
      'identifier' => 'main-menu_people:community',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-social-links_facebook:https://www.facebook.com/travelmassive
  $menu_links['menu-social-links_facebook:https://www.facebook.com/travelmassive'] = array(
    'menu_name' => 'menu-social-links',
    'link_path' => 'https://www.facebook.com/travelmassive',
    'router_path' => '',
    'link_title' => 'Facebook',
    'options' => array(
      'identifier' => 'menu-social-links_facebook:https://www.facebook.com/travelmassive',
      'attributes' => array(
        'title' => 'Travel Massive on Facebook',
        'class' => array(
          0 => 'facebook',
        ),
        'target' => '_blank',
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-social-links_instagram:http://instagram.com/travelmassive
  $menu_links['menu-social-links_instagram:http://instagram.com/travelmassive'] = array(
    'menu_name' => 'menu-social-links',
    'link_path' => 'http://instagram.com/travelmassive',
    'router_path' => '',
    'link_title' => 'Instagram',
    'options' => array(
      'identifier' => 'menu-social-links_instagram:http://instagram.com/travelmassive',
      'attributes' => array(
        'title' => 'See Travel Massive on Instagram',
        'class' => array(
          0 => 'instagram',
        ),
        'target' => '_blank',
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-social-links_linkedin:http://www.linkedin.com/company/travel-massive
  $menu_links['menu-social-links_linkedin:http://www.linkedin.com/company/travel-massive'] = array(
    'menu_name' => 'menu-social-links',
    'link_path' => 'http://www.linkedin.com/company/travel-massive',
    'router_path' => '',
    'link_title' => 'Linkedin',
    'options' => array(
      'identifier' => 'menu-social-links_linkedin:http://www.linkedin.com/company/travel-massive',
      'attributes' => array(
        'title' => 'Travel Massive on Linkedin',
        'class' => array(
          0 => 'linkedin',
        ),
        'target' => '_blank',
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: menu-social-links_twitter:http://twitter.com/travelmassive
  $menu_links['menu-social-links_twitter:http://twitter.com/travelmassive'] = array(
    'menu_name' => 'menu-social-links',
    'link_path' => 'http://twitter.com/travelmassive',
    'router_path' => '',
    'link_title' => 'Twitter',
    'options' => array(
      'identifier' => 'menu-social-links_twitter:http://twitter.com/travelmassive',
      'attributes' => array(
        'title' => 'Travel Massive on Twitter',
        'class' => array(
          0 => 'twitter',
        ),
        'target' => '_blank',
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Chapters');
  t('Companies');
  t('Discuss');
  t('Events');
  t('Facebook');
  t('Instagram');
  t('Leaders');
  t('Linkedin');
  t('News');
  t('People');
  t('Twitter');


  return $menu_links;
}

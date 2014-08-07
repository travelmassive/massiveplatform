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
  t('Facebook');
  t('Instagram');
  t('Linkedin');
  t('Twitter');


  return $menu_links;
}

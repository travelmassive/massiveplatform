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

  // Exported menu link: menu-footer-menu_about:<front>
  $menu_links['menu-footer-menu_about:<front>'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'About',
    'options' => array(
      'attributes' => array(
        'title' => 'About Travel Massive',
      ),
      'identifier' => 'menu-footer-menu_about:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-menu_contacts:<front>
  $menu_links['menu-footer-menu_contacts:<front>'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Contacts',
    'options' => array(
      'attributes' => array(
        'title' => 'Contact us',
      ),
      'identifier' => 'menu-footer-menu_contacts:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-menu_help:<front>
  $menu_links['menu-footer-menu_help:<front>'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Help',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-menu_help:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-menu_press-kit:<front>
  $menu_links['menu-footer-menu_press-kit:<front>'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Press kit',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'menu-footer-menu_press-kit:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-social-links_facebook:<front>
  $menu_links['menu-social-links_facebook:<front>'] = array(
    'menu_name' => 'menu-social-links',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Facebook',
    'options' => array(
      'attributes' => array(
        'title' => 'Travel Massive on Facebook',
      ),
      'identifier' => 'menu-social-links_facebook:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-social-links_instagram:<front>
  $menu_links['menu-social-links_instagram:<front>'] = array(
    'menu_name' => 'menu-social-links',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Instagram',
    'options' => array(
      'attributes' => array(
        'title' => 'See Travel Massive on Instagram',
      ),
      'identifier' => 'menu-social-links_instagram:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-social-links_linkedin:<front>
  $menu_links['menu-social-links_linkedin:<front>'] = array(
    'menu_name' => 'menu-social-links',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Linkedin',
    'options' => array(
      'attributes' => array(
        'title' => 'Travel Massive on Linkedin',
      ),
      'identifier' => 'menu-social-links_linkedin:<front>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: menu-social-links_twitter:<front>
  $menu_links['menu-social-links_twitter:<front>'] = array(
    'menu_name' => 'menu-social-links',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Twitter',
    'options' => array(
      'attributes' => array(
        'title' => 'Travel Massive on Twitter',
      ),
      'identifier' => 'menu-social-links_twitter:<front>',
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
  t('About');
  t('Contacts');
  t('Facebook');
  t('Help');
  t('Instagram');
  t('Linkedin');
  t('Press kit');
  t('Twitter');


  return $menu_links;
}

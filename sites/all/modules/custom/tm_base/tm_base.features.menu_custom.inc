<?php
/**
 * @file
 * tm_base.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function tm_base_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: main-menu.
  $menus['main-menu'] = array(
    'menu_name' => 'main-menu',
    'title' => 'Main menu',
    'description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
  );
  // Exported menu: menu-footer-menu.
  $menus['menu-footer-menu'] = array(
    'menu_name' => 'menu-footer-menu',
    'title' => 'More massive',
    'description' => 'The footer menu',
  );
  // Exported menu: menu-social-links.
  $menus['menu-social-links'] = array(
    'menu_name' => 'menu-social-links',
    'title' => 'Follow us',
    'description' => 'Menu containing the social links',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Follow us');
  t('Main menu');
  t('Menu containing the social links');
  t('More massive');
  t('The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.');
  t('The footer menu');


  return $menus;
}

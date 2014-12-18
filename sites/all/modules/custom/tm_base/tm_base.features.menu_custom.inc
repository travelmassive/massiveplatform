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
  t('Menu containing the social links');
  t('More massive');
  t('The footer menu');


  return $menus;
}

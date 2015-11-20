<?php
/**
 * @file
 * tm_theme.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tm_theme_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function tm_theme_image_default_styles() {
  $styles = array();

  // Exported image style: avatar.
  $styles['avatar'] = array(
    'label' => 'Avatar',
    'effects' => array(
      5 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 512,
          'height' => 512,
        ),
        'weight' => 1,
      ),
      6 => array(
        'name' => 'image_hires',
        'data' => array(),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: banner.
  $styles['banner'] = array(
    'label' => 'Banner',
    'effects' => array(
      3 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 1610,
          'height' => 906,
        ),
        'weight' => 1,
      ),
      4 => array(
        'name' => 'image_hires',
        'data' => array(),
        'weight' => 2,
      ),
    ),
  );

  return $styles;
}

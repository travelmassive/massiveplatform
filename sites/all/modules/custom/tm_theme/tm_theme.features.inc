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
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 256,
          'height' => 256,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: banner.
  $styles['banner'] = array(
    'label' => 'Banner',
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 805,
          'height' => 453,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

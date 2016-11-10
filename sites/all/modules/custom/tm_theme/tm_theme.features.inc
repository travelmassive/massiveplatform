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
      7 => array(
        'name' => 'coloractions_convert',
        'data' => array(
          'format' => 'image/jpeg',
          'quality' => '',
        ),
        'weight' => -10,
      ),
      4 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 512,
          'height' => 512,
        ),
        'weight' => -9,
      ),
      5 => array(
        'name' => 'image_hires',
        'data' => array(),
        'weight' => -8,
      ),
    ),
  );

  // Exported image style: avatar_grid.
  $styles['avatar_grid'] = array(
    'label' => 'Avatar (Grid)',
    'effects' => array(
      1 => array(
        'name' => 'coloractions_convert',
        'data' => array(
          'format' => 'image/jpeg',
          'quality' => '',
        ),
        'weight' => 1,
      ),
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 200,
          'height' => 200,
        ),
        'weight' => 2,
      ),
      3 => array(
        'name' => 'image_hires',
        'data' => array(),
        'weight' => 3,
      ),
    ),
  );

  // Exported image style: banner.
  $styles['banner'] = array(
    'label' => 'Banner',
    'effects' => array(
      3 => array(
        'name' => 'coloractions_convert',
        'data' => array(
          'format' => 'image/jpeg',
          'quality' => '',
        ),
        'weight' => -10,
      ),
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 1610,
          'height' => 906,
        ),
        'weight' => -9,
      ),
      2 => array(
        'name' => 'image_hires',
        'data' => array(),
        'weight' => -8,
      ),
    ),
  );

  // Exported image style: banner_grid.
  $styles['banner_grid'] = array(
    'label' => 'Banner (Grid)',
    'effects' => array(
      4 => array(
        'name' => 'coloractions_convert',
        'data' => array(
          'format' => 'image/jpeg',
          'quality' => '',
        ),
        'weight' => 1,
      ),
      5 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 800,
          'height' => 450,
        ),
        'weight' => 2,
      ),
      6 => array(
        'name' => 'image_hires',
        'data' => array(),
        'weight' => 3,
      ),
    ),
  );

  return $styles;
}

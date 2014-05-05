<?php

/**
 * @file
 * Display Suite Trilithon configuration.
 */

function ds_trilithon() {
  return array(
    'label' => t('Trilithon'),
    'regions' => array(
      'banner' => t('Banner'),
      'first_column' => t('First column'),
      'second_column' => t('Second column'),
    ),
    // Uncomment if you want to include a CSS file for this layout (trilithon.css)
    // 'css' => TRUE,
    // Uncomment if you want to include a preview for this layout (trilithon.png)
    'image' => TRUE,
  );
}

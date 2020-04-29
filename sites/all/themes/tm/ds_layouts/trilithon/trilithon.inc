<?php

/**
 * @file
 * Display Suite Trilithon configuration.
 */

function ds_trilithon() {
  return array(
    'label' => t('Trilithon'),
    'regions' => array(
      'header_media' => t('Header media'),
      'header_body' => t('Header body'),
      'header_extra' => t('Header extra'),
      'below_header' => t('Below header'),
      'first_column' => t('First column'),
      'second_column' => t('Second column'),
    ),
    // Uncomment if you want to include a CSS file for this layout (trilithon.css)
    // 'css' => TRUE,
    // Uncomment if you want to include a preview for this layout (trilithon.png)
    'image' => TRUE,
  );
}

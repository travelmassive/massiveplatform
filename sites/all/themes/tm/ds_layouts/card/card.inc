<?php

/**
 * @file
 * Display Suite Card configuration.
 */

function ds_card() {
  return array(
    'label' => t('Card'),
    'regions' => array(
      'media' => t('Media'),
      'teaser' => t('Teaser'),
      'extra' => t('Extra'),
    ),
    // Uncomment if you want to include a CSS file for this layout (card.css)
    // 'css' => TRUE,
    // Uncomment if you want to include a preview for this layout (card.png)
    // 'image' => TRUE,
  );
}

<?php

$plugin= array(
    'single' => TRUE,
    'title' => t('Vanilla Embedded Pane'),
    'icon'  => 'icon-vanilla.png',
    'description' => t('A pane to show a vanilla embedded forum.'),
    'category' => t('Vanilla Forum'),
);


function orchid_vanilla_embed_content_type_edit_form(&$form, &$form_state) {
  $settings = ctools_custom_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $form_state['settings'] = $settings;
  if ($settings['custom_type'] == 'fixed') {
    return $form; // no form for this case.
  }

  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($settings['admin_title']) ? $settings['admin_title'] : '',
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['title'],
    '#title' => t('Title'),
  );
  return $form;
}

function orchid_vanilla_embed_content_type_render($subtype, $conf, $panel_args, $context) { 
  $block = new stdClass();
  $block->title = '';
  $block->content = theme('embed_vanilla');

  return $block;
}

function orchid_vanilla_embed_content_type_admin_title($subtype, $conf, $context) {
  return t('Vanilla Embedded Pane');
}
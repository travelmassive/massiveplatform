<?php
/**
 * @file
 * tm_branding.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function tm_branding_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-site_brand-field_brand_asset_files'.
  $field_instances['node-site_brand-field_brand_asset_files'] = array(
    'bundle' => 'site_brand',
    'deleted' => 0,
    'description' => 'Additional file assets for html branding elements to reference.<br>
You can reference __ASSETS_BASE_PATH__/assets/filename.ext in html template<br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 37,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_asset_files',
    'label' => 'Asset files',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'site_branding/assets',
      'file_extensions' => 'jpg jpeg gif png txt pdf svg css js mp4',
      'max_filesize' => '8 MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 27,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_description'.
  $field_instances['node-site_brand-field_brand_description'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Description and notes for brand (not displayed)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 27,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_description',
    'label' => 'Brand Description',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_email_footer_html'.
  $field_instances['node-site_brand-field_brand_email_footer_html'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Insert a branding message into the footer of email notifications.<br>
Use basic html formatting suitable for emails.<br>
Test by triggering a notification such as following or user message.<br>
Excludes event announcements and moderation messages.<br>
Default is $conf[\'tm_branding_email_footer_html\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 13,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_email_footer_html',
    'label' => 'Email footer html',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_enabled'.
  $field_instances['node-site_brand-field_brand_enabled'] = array(
    'bundle' => 'site_brand',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Check this to activate the branding.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 12,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_enabled',
    'label' => 'Activated',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_enabled_timespan'.
  $field_instances['node-site_brand-field_brand_enabled_timespan'] = array(
    'bundle' => 'site_brand',
    'deleted' => 0,
    'description' => 'Duration to display the branding (in UTC/GMT)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 18,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_enabled_timespan',
    'label' => 'Start and end date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'no_fieldset' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 28,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_enabled_timespan_on'.
  $field_instances['node-site_brand-field_brand_enabled_timespan_on'] = array(
    'bundle' => 'site_brand',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Check this to enable branding for a period of time.<br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 30,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_enabled_timespan_on',
    'label' => 'Enable branding for period',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 27,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_favicon'.
  $field_instances['node-site_brand-field_brand_favicon'] = array(
    'bundle' => 'site_brand',
    'deleted' => 0,
    'description' => 'Favicon for the site.<br>
Default is $conf[\'tm_branding_favicon\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 29,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_favicon',
    'label' => 'Favicon',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'site_branding/favicon',
      'file_extensions' => 'ico png',
      'max_filesize' => '1 MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_footer_html'.
  $field_instances['node-site_brand-field_brand_footer_html'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The main footer block.<br>
Default is $conf[\'tm_branding_footer_html\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_footer_html',
    'label' => 'Footer html',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_footer_level1_html'.
  $field_instances['node-site_brand-field_brand_footer_level1_html'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The front page footer (displayed below main footer)<br>
Add sponsor images or other branding here.<br>
Default is $conf[\'tm_branding_footer_level1_html\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_footer_level1_html',
    'label' => 'Footer level1 html',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 19,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_footer_level2_html'.
  $field_instances['node-site_brand-field_brand_footer_level2_html'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The secondary page level footer (displayed below main footer)<br>
Displayed on all pages except front page (ie: profile pages, event listings, etc).<br>
Default is $conf[\'tm_branding_footer_level2_html\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_footer_level2_html',
    'label' => 'Footer Level 2 html',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_footer_logo'.
  $field_instances['node-site_brand-field_brand_footer_logo'] = array(
    'bundle' => 'site_brand',
    'deleted' => 0,
    'description' => 'The header logo (SVG Format)<br>
Default is: $conf[\'tm_branding_footer_logo\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_footer_logo',
    'label' => 'Footer logo',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'site_branding/footer_logo',
      'file_extensions' => 'svg',
      'max_filesize' => '1 MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 15,
    ),
  );

  // Exported field_instance:
  // 'node-site_brand-field_brand_frontpage_block_html'.
  $field_instances['node-site_brand-field_brand_frontpage_block_html'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Custom html block for frontpage.<br>
Will be displayed above the newsfeeds.<br>
Default is $conf[\'tm_branding_frontpage_block_html\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 33,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_frontpage_block_html',
    'label' => 'Frontpage block html',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 24,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_frontpage_image'.
  $field_instances['node-site_brand-field_brand_frontpage_image'] = array(
    'bundle' => 'site_brand',
    'deleted' => 0,
    'description' => 'Front page background image, displayed on mobile devices or if video disabled.<br>
If using a front page video, this should be the first frame of the video.<br>
Default is $conf[\'tm_branding_frontpage_image\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_frontpage_image',
    'label' => 'Frontpage image',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'site_branding/frontpage_image',
      'file_extensions' => 'jpg jpeg',
      'max_filesize' => '1 MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 21,
    ),
  );

  // Exported field_instance:
  // 'node-site_brand-field_brand_frontpage_video_link'.
  $field_instances['node-site_brand-field_brand_frontpage_video_link'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Link for video label.<br>
Default is $conf[\'tm_branding_frontpage_video_link\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 31,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_frontpage_video_link',
    'label' => 'Frontpage video link',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => '',
        'target' => 'default',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'default',
      'title' => 'none',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 20,
    ),
  );

  // Exported field_instance:
  // 'node-site_brand-field_brand_frontpage_video_text'.
  $field_instances['node-site_brand-field_brand_frontpage_video_text'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Text label for background video.<br>
Will be displayed at top left of screen.<br>
Default is $conf[\'tm_branding_frontpage_video_text\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 32,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_frontpage_video_text',
    'label' => 'Frontpage video text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 19,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_frontpage_video_url'.
  $field_instances['node-site_brand-field_brand_frontpage_video_url'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Play an mp4 video stream on the frontpage as a background video.<br>
Recommended to play from CDN (ie: vimeo) in 720p format.<br>
Default is $conf[\'tm_branding_frontpage_video_url\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_frontpage_video_url',
    'label' => 'Frontpage video mp4 url',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => '',
        'target' => 'default',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => '',
      ),
      'enable_tokens' => 0,
      'rel_remove' => 'default',
      'title' => 'none',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 255,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_header_logo'.
  $field_instances['node-site_brand-field_brand_header_logo'] = array(
    'bundle' => 'site_brand',
    'deleted' => 0,
    'description' => 'The header logo (SVG Format).<br>
Default is: $conf[\'tm_branding_header_logo\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_header_logo',
    'label' => 'Header logo',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'site_branding/header_logo',
      'file_extensions' => 'svg',
      'max_filesize' => '1MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 14,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_include_css'.
  $field_instances['node-site_brand-field_brand_include_css'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Include site-wide custom css.<br>
Default is $conf[\'tm_branding_include_css\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_include_css',
    'label' => 'Custom css',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 25,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_include_js'.
  $field_instances['node-site_brand-field_brand_include_js'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Include site-wide custom javascript.<br>
Default is $conf[\'tm_branding_include_js\']<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_include_js',
    'label' => 'Custom js',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 26,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_match_continent'.
  $field_instances['node-site_brand-field_brand_match_continent'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Match continent (GeoIP matching only)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 23,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_match_continent',
    'label' => 'Match continent',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_match_country'.
  $field_instances['node-site_brand-field_brand_match_country'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Match country (based on selected match method)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'countries',
        'settings' => array(),
        'type' => 'country_default',
        'weight' => 24,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_match_country',
    'label' => 'Match country',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 12,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_match_method'.
  $field_instances['node-site_brand-field_brand_match_method'] = array(
    'bundle' => 'site_brand',
    'default_value' => array(
      0 => array(
        'value' => 'geoip',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 22,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_match_method',
    'label' => 'Match method',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_match_probability'.
  $field_instances['node-site_brand-field_brand_match_probability'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Define a probability of returning a valid match if the rule matches a criteria.<br>
This is helpful if you want to match only 50% of visitors with a rule.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 38,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_match_probability',
    'label' => 'Match probability',
    'required' => 0,
    'settings' => array(
      'max' => 100,
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_match_user_chapter'.
  $field_instances['node-site_brand-field_brand_match_user_chapter'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => 'Match user\'s chapters.<br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 36,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_match_user_chapter',
    'label' => 'Match user chapter',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'status' => 0,
        ),
      ),
      'nodeaccess_nodereference' => array(
        'all' => array(
          'view' => 0,
        ),
        'all_published' => 0,
        'author' => array(
          'delete' => 0,
          'update' => 0,
          'view' => 0,
        ),
        'author_published' => 0,
        'priority' => 0,
        'referenced' => array(
          'delete' => array(
            'delete' => 0,
            'update' => 0,
            'view' => 0,
          ),
          'published' => 0,
          'update' => array(
            'delete' => 0,
            'update' => 0,
            'view' => 0,
          ),
          'view' => array(
            'delete' => 0,
            'update' => 0,
            'view' => 0,
          ),
        ),
        'unused' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 14,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_match_user_industry'.
  $field_instances['node-site_brand-field_brand_match_user_industry'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Match industry segment of member',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 21,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_match_user_industry',
    'label' => 'Match industry segment',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'hs_taxonomy',
      'settings' => array(),
      'type' => 'taxonomy_hs',
      'weight' => 13,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_menu_html'.
  $field_instances['node-site_brand-field_brand_menu_html'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The top level menu links.<br>
Default is $conf[\'tm_branding_menu_html\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_menu_html',
    'label' => 'Menu html',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_process_order'.
  $field_instances['node-site_brand-field_brand_process_order'] = array(
    'bundle' => 'site_brand',
    'default_value' => array(
      0 => array(
        'value' => 10,
      ),
    ),
    'deleted' => 0,
    'description' => 'Order in which the brand directive is processed.
<br>A lower number will be processed first. The first match will be displayed.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 11,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_process_order',
    'label' => 'Processing Order',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_search_page_banner'.
  $field_instances['node-site_brand-field_brand_search_page_banner'] = array(
    'bundle' => 'site_brand',
    'deleted' => 0,
    'description' => 'Display an image at the top of the search page.<br>
Will be scaled based on width of screen.<br>
Upload in 16:9 aspect in 2x resolution for retina support.<br>
Default is $conf[\'tm_branding_search_page_banner\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 14,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_search_page_banner',
    'label' => 'Search page banner',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'site_branding/search_banner',
      'file_extensions' => 'gif jpg png',
      'max_filesize' => '1mb',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 22,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_search_page_link'.
  $field_instances['node-site_brand-field_brand_search_page_link'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Link for the search banner image.<br>
Default is $conf[\'tm_branding_search_page_link\']',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 34,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_search_page_link',
    'label' => 'Search page link',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => '',
        'target' => 'default',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'default',
      'title' => 'none',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 23,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_test_enabled'.
  $field_instances['node-site_brand-field_brand_test_enabled'] = array(
    'bundle' => 'site_brand',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Check this to enable testing for a member account or IP.<br>
Branding will be enabled for any matching test criteria.<br>
This will be enabled even if the branding is not activated.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 35,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_test_enabled',
    'label' => 'Enable testing',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_test_ips'.
  $field_instances['node-site_brand-field_brand_test_ips'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'List of IP addresses separated by spaces to match for testing.<br>
This will override any match criteria.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 16,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_test_ips',
    'label' => 'Test on IP addresses',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_brand_test_users'.
  $field_instances['node-site_brand-field_brand_test_users'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => 'Test branding on member accounts.<br>
This will override any match criteria.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 15,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_brand_test_users',
    'label' => 'Test on member account',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'status' => 0,
        ),
      ),
      'nodeaccess_userreference' => array(
        'all' => array(
          'view' => 0,
        ),
        'all_published' => 0,
        'author' => array(
          'delete' => 0,
          'update' => 0,
          'view' => 0,
        ),
        'author_published' => 0,
        'create' => array(
          'chapter' => 0,
          'event' => 0,
          'organization' => 0,
          'page' => 0,
          'site_brand' => 0,
        ),
        'priority' => 0,
        'referenced' => array(
          'delete' => 0,
          'deny_delete' => 0,
          'deny_update' => 0,
          'deny_view' => 0,
          'update' => 0,
          'view' => 0,
        ),
        'referenced_published' => 0,
        'unused' => 0,
        'views' => array(
          'view' => '',
          'view_args' => '',
        ),
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_match_partner_cookie'.
  $field_instances['node-site_brand-field_match_partner_cookie'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A comma separated list of parter codes to match.<br>
Set a partner code by visiting the URL https://yourdomain.com/?partner=mypartner<br>
This will set a cookie called "site_branding_partner" and redirect to the home page.<br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 42,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_match_partner_cookie',
    'label' => 'Match partner cookie',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_match_referer_domain'.
  $field_instances['node-site_brand-field_match_referer_domain'] = array(
    'bundle' => 'site_brand',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A comma separated list of domains (excluding www) to match.<br>
The referrer domain is set in site_branding_referrer_domain cookie.<br>
ie: techcrunch.com, thenextweb.com',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 43,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_match_referer_domain',
    'label' => 'Match referer domain',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-site_brand-field_match_stop_rule'.
  $field_instances['node-site_brand-field_match_stop_rule'] = array(
    'bundle' => 'site_brand',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'If this rule matched, don\'t process any more rules.<br>
This will prevent subsequent rules from branding other elements.<br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 41,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_match_stop_rule',
    'label' => 'Stop rules (don\'t process any more rules)',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 19,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A comma separated list of domains (excluding www) to match.<br>
The referrer domain is set in site_branding_referrer_domain cookie.<br>
ie: techcrunch.com, thenextweb.com');
  t('A comma separated list of parter codes to match.<br>
Set a partner code by visiting the URL https://yourdomain.com/?partner=mypartner<br>
This will set a cookie called "site_branding_partner" and redirect to the home page.<br>');
  t('Activated');
  t('Additional file assets for html branding elements to reference.<br>
You can reference __ASSETS_BASE_PATH__/assets/filename.ext in html template<br>');
  t('Asset files');
  t('Brand Description');
  t('Check this to activate the branding.');
  t('Check this to enable branding for a period of time.<br>');
  t('Check this to enable testing for a member account or IP.<br>
Branding will be enabled for any matching test criteria.<br>
This will be enabled even if the branding is not activated.');
  t('Custom css');
  t('Custom html block for frontpage.<br>
Will be displayed above the newsfeeds.<br>
Default is $conf[\'tm_branding_frontpage_block_html\']');
  t('Custom js');
  t('Define a probability of returning a valid match if the rule matches a criteria.<br>
This is helpful if you want to match only 50% of visitors with a rule.');
  t('Description and notes for brand (not displayed)');
  t('Display an image at the top of the search page.<br>
Will be scaled based on width of screen.<br>
Upload in 16:9 aspect in 2x resolution for retina support.<br>
Default is $conf[\'tm_branding_search_page_banner\']');
  t('Duration to display the branding (in UTC/GMT)');
  t('Email footer html');
  t('Enable branding for period');
  t('Enable testing');
  t('Favicon');
  t('Favicon for the site.<br>
Default is $conf[\'tm_branding_favicon\']');
  t('Footer Level 2 html');
  t('Footer html');
  t('Footer level1 html');
  t('Footer logo');
  t('Front page background image, displayed on mobile devices or if video disabled.<br>
If using a front page video, this should be the first frame of the video.<br>
Default is $conf[\'tm_branding_frontpage_image\']');
  t('Frontpage block html');
  t('Frontpage image');
  t('Frontpage video link');
  t('Frontpage video mp4 url');
  t('Frontpage video text');
  t('Header logo');
  t('If this rule matched, don\'t process any more rules.<br>
This will prevent subsequent rules from branding other elements.<br>');
  t('Include site-wide custom css.<br>
Default is $conf[\'tm_branding_include_css\']');
  t('Include site-wide custom javascript.<br>
Default is $conf[\'tm_branding_include_js\']<br><br>');
  t('Insert a branding message into the footer of email notifications.<br>
Use basic html formatting suitable for emails.<br>
Test by triggering a notification such as following or user message.<br>
Excludes event announcements and moderation messages.<br>
Default is $conf[\'tm_branding_email_footer_html\']');
  t('Link for the search banner image.<br>
Default is $conf[\'tm_branding_search_page_link\']');
  t('Link for video label.<br>
Default is $conf[\'tm_branding_frontpage_video_link\']');
  t('List of IP addresses separated by spaces to match for testing.<br>
This will override any match criteria.');
  t('Match continent');
  t('Match continent (GeoIP matching only)');
  t('Match country');
  t('Match country (based on selected match method)');
  t('Match industry segment');
  t('Match industry segment of member');
  t('Match method');
  t('Match partner cookie');
  t('Match probability');
  t('Match referer domain');
  t('Match user chapter');
  t('Match user\'s chapters.<br>');
  t('Menu html');
  t('Order in which the brand directive is processed.
<br>A lower number will be processed first. The first match will be displayed.');
  t('Play an mp4 video stream on the frontpage as a background video.<br>
Recommended to play from CDN (ie: vimeo) in 720p format.<br>
Default is $conf[\'tm_branding_frontpage_video_url\']');
  t('Processing Order');
  t('Search page banner');
  t('Search page link');
  t('Start and end date');
  t('Stop rules (don\'t process any more rules)');
  t('Test branding on member accounts.<br>
This will override any match criteria.');
  t('Test on IP addresses');
  t('Test on member account');
  t('Text label for background video.<br>
Will be displayed at top left of screen.<br>
Default is $conf[\'tm_branding_frontpage_video_text\']');
  t('The front page footer (displayed below main footer)<br>
Add sponsor images or other branding here.<br>
Default is $conf[\'tm_branding_footer_level1_html\']');
  t('The header logo (SVG Format).<br>
Default is: $conf[\'tm_branding_header_logo\']');
  t('The header logo (SVG Format)<br>
Default is: $conf[\'tm_branding_footer_logo\']');
  t('The main footer block.<br>
Default is $conf[\'tm_branding_footer_html\']');
  t('The secondary page level footer (displayed below main footer)<br>
Displayed on all pages except front page (ie: profile pages, event listings, etc).<br>
Default is $conf[\'tm_branding_footer_level2_html\']');
  t('The top level menu links.<br>
Default is $conf[\'tm_branding_menu_html\']');

  return $field_instances;
}

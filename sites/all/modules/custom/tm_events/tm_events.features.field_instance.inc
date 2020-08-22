<?php

/**
 * @file
 * tm_events.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function tm_events_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-event-body'.
  $field_instances['node-event-body'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_plain',
        'weight' => 6,
      ),
      'grid' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 61,
      ),
      'list' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Description',
    'required' => 0,
    'settings' => array(
      'display_summary' => 0,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 8,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-event-field_approved_members_only'.
  $field_instances['node-event-field_approved_members_only'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Restrict registration and waitlist to members who are verified.<br>
Non-verified members won\'t be able to sign up.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 16,
      ),
      'full' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 56,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_approved_members_only',
    'label' => 'Verified Members Only',
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
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-event-field_chapter'.
  $field_instances['node-event-field_chapter'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'default_value_function' => 'entityreference_prepopulate_field_default_value',
    'deleted' => 0,
    'description' => 'The chapter hosting this event.<br>
<strong>Publish to multiple chapters? See the <i>Event Chapters</i> section.</strong>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 5,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 59,
      ),
      'grid' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 59,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
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
    'field_name' => 'field_chapter',
    'label' => 'Event Chapter',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'action' => 'none',
          'action_on_edit' => 1,
          'fallback' => 'none',
          'providers' => array(
            'og_context' => FALSE,
            'url' => 1,
          ),
          'skip_perm' => 0,
          'status' => 1,
        ),
      ),
      'nodeaccess_nodereference' => array(
        'all' => array(
          'view' => 'view',
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
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-event-field_chapter_can_announce'.
  $field_instances['node-event-field_chapter_can_announce'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Chapters that you publish this event to will be able to announce the event.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 56,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_chapter_can_announce',
    'label' => 'Allow Chapter Leaders to send announcements',
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
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'node-event-field_chapter_publish'.
  $field_instances['node-event-field_chapter_publish'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => 'Select additional chapters associated with the event.
<br>The event will be published in these chapters also.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 34,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 48,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 48,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_chapter_publish',
    'label' => 'Publish to Chapters',
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
          'view' => 'view',
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
            'view' => 'view',
          ),
        ),
        'unused' => 1,
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
      'weight' => 31,
    ),
  );

  // Exported field_instance: 'node-event-field_event_chatbox_enabled'.
  $field_instances['node-event-field_event_chatbox_enabled'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Enable an online chat box for attendee discussion.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 63,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 62,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_chatbox_enabled',
    'label' => 'Display live chat',
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
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-event-field_event_company'.
  $field_instances['node-event-field_event_company'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'default_value_function' => 'entityreference_prepopulate_field_default_value',
    'deleted' => 0,
    'description' => '<span id=\'tm-event-edit-company-description\'>The company hosting this event.</span>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 50,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 44,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 49,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_company',
    'label' => 'Event Company',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'action' => 'none',
          'action_on_edit' => 1,
          'fallback' => 'none',
          'providers' => array(
            'og_context' => FALSE,
            'url' => 1,
          ),
          'skip_perm' => 0,
          'status' => 1,
        ),
      ),
      'nodeaccess_nodereference' => array(
        'all' => array(
          'view' => 'view',
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
            'view' => 'view',
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
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-event-field_event_cover_video'.
  $field_instances['node-event-field_event_cover_video'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Increase your event\'s exposure with a cover video. 
<br>Example: https://www.youtube.com/watch?v=rbtv2zxhufM<br>
<b>Live Streaming?</b> See the <i>Online Events</i> section below.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 53,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 34,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_cover_video',
    'label' => 'Cover Video',
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

  // Exported field_instance: 'node-event-field_event_covid19_message'.
  $field_instances['node-event-field_event_covid19_message'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Share any COVID-19 or health and safety guidelines for this event.<br>
This will be displayed on the event page for all members.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 72,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_plain',
        'weight' => 7,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_covid19_message',
    'label' => 'Health and Safety Guidelines',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 3,
      ),
      'type' => 'text_textarea',
      'weight' => 19,
    ),
  );

  // Exported field_instance: 'node-event-field_event_custom_banner'.
  $field_instances['node-event-field_event_custom_banner'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Add a custom banner at the top of the event.<br>
Provide HTML markup or a template alias.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 58,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_custom_banner',
    'label' => 'Custom Banner',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 3,
      ),
      'type' => 'text_textarea',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-event-field_event_date'.
  $field_instances['node-event-field_event_date'] = array(
    'bundle' => 'event',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'custom_date_format' => '',
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
        ),
        'type' => 'date_default',
        'weight' => 13,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 58,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_date',
    'label' => 'When',
    'required' => 1,
    'settings' => array(
      'default_value' => 'strtotime',
      'default_value2' => 'same',
      'default_value_code' => '+14 days',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'increment' => 15,
        'input_format' => 'Y-m-d H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array(),
        'year_range' => '-0:+3',
      ),
      'type' => 'date_popup',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-event-field_event_extra_guests'.
  $field_instances['node-event-field_event_extra_guests'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'How many non-registered guests are coming?<br>
This will be added to the attendee count on the event page.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 22,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 49,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_extra_guests',
    'label' => 'Non-registered Guests',
    'required' => 0,
    'settings' => array(
      'max' => 100000,
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
      'weight' => 51,
    ),
  );

  // Exported field_instance: 'node-event-field_event_featured_access_rsvp'.
  $field_instances['node-event-field_event_featured_access_rsvp'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Featured members will be able to download the attendee list from the event settings menu',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 38,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 46,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 33,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_featured_access_rsvp',
    'label' => 'Allow featured members to download the attendee list',
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
      'weight' => 38,
    ),
  );

  // Exported field_instance: 'node-event-field_event_featured_label'.
  $field_instances['node-event-field_event_featured_label'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Assign a label for featured members on the event listing. Leave blank for default.
<br>example: Keynote Speakers
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 37,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 47,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_featured_label',
    'label' => 'Featured members label',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 55,
      ),
      'type' => 'text_textfield',
      'weight' => 36,
    ),
  );

  // Exported field_instance: 'node-event-field_event_featured_members'.
  $field_instances['node-event-field_event_featured_members'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => 'Highlight members of this event such as speakers or workshops leaders',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 35,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'use_content_language' => TRUE,
          'view_mode' => 'grid',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 18,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_featured_members',
    'label' => 'Featured members',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'status' => 0,
        ),
      ),
      'nodeaccess_userreference' => array(
        'all' => array(
          'view' => 'view',
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
        ),
        'priority' => 0,
        'referenced' => array(
          'delete' => 0,
          'deny_delete' => 0,
          'deny_update' => 0,
          'deny_view' => 0,
          'update' => 0,
          'view' => 'view',
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
      'weight' => 34,
    ),
  );

  // Exported field_instance: 'node-event-field_event_geoblock_signups'.
  $field_instances['node-event-field_event_geoblock_signups'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 'all_members',
      ),
    ),
    'deleted' => 0,
    'description' => 'You can restrict registrations to members in your chapter, or country.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 65,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 24,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_geoblock_signups',
    'label' => 'Who can register for your event?',
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

  // Exported field_instance: 'node-event-field_event_hide_location'.
  $field_instances['node-event-field_event_hide_location'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Restrict displaying the event location to registered attendees only.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 26,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 51,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_hide_location',
    'label' => 'Only show location to registered attendees',
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
      'weight' => 23,
    ),
  );

  // Exported field_instance: 'node-event-field_event_hide_organizers'.
  $field_instances['node-event-field_event_hide_organizers'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Hide the list of organizers on the event page.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 47,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_hide_organizers',
    'label' => 'Hide Organizers',
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
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-event-field_event_is_online'.
  $field_instances['node-event-field_event_is_online'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Members can participate online (ie: webinar, live stream, Twitter chat)
<br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 52,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_is_online',
    'label' => 'This is an online event',
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
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-event-field_event_is_preview_mode'.
  $field_instances['node-event-field_event_is_preview_mode'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Only people with the link can see the event.<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 54,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_is_preview_mode',
    'label' => 'Preview mode',
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
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-event-field_event_is_promoted'.
  $field_instances['node-event-field_event_is_promoted'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Display event on the front page (for non-chapter events).',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 59,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_is_promoted',
    'label' => 'Promote event to front page',
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
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-event-field_event_livestream_mode'.
  $field_instances['node-event-field_event_livestream_mode'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 'public',
      ),
    ),
    'deleted' => 0,
    'description' => 'Choose who can see your live stream.<br>
Is your live stream private? Set your YouTube Visibility to \'Unlisted\'.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 64,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_livestream_mode',
    'label' => 'Who can watch the live stream?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-event-field_event_livestream_on'.
  $field_instances['node-event-field_event_livestream_on'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Display the live stream on the event page.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 66,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 25,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_livestream_on',
    'label' => 'Live stream on event page',
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

  // Exported field_instance: 'node-event-field_event_livestream_video_url'.
  $field_instances['node-event-field_event_livestream_video_url'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Embed your YouTube live stream on the event page.
<br>Example: https://www.youtube.com/watch?v=rbtv2zxhufM',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 62,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 60,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_livestream_video_url',
    'label' => 'YouTube live stream URL',
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

  // Exported field_instance: 'node-event-field_event_online_instructions'.
  $field_instances['node-event-field_event_online_instructions'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '<b>⬆ This will only be displayed to registered attendees.</b>
',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 71,
      ),
      'full' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 74,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_online_instructions',
    'label' => 'How to join the event',
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

  // Exported field_instance: 'node-event-field_event_payment_commission'.
  $field_instances['node-event-field_event_payment_commission'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The percentage of total event revenue to payout.
<br>Note: Does not include bank fees, forex or other charges.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => '',
        ),
        'type' => 'number_decimal',
        'weight' => 49,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 27,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_payment_commission',
    'label' => 'Payment commission (%)',
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
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-event-field_event_payment_payout_to'.
  $field_instances['node-event-field_event_payment_payout_to'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => 'Who will we send the event payout to?',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 48,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_payment_payout_to',
    'label' => 'Send payout to',
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
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-event-field_event_reassign_owner'.
  $field_instances['node-event-field_event_reassign_owner'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => 'Change the owner of the event.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 51,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_reassign_owner',
    'label' => 'Change Owner',
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
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-event-field_event_send_reminders'.
  $field_instances['node-event-field_event_send_reminders'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Chapter members are invited <b>24 hours</b> before the event starts to register.<br>
Registered attendees are reminded <b>1.5 hours</b> and <b>10 minutes</b> before you go live.<br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 70,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_send_reminders',
    'label' => 'Send reminders',
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

  // Exported field_instance: 'node-event-field_event_sharing_image'.
  $field_instances['node-event-field_event_sharing_image'] = array(
    'bundle' => 'event',
    'deleted' => 0,
    'description' => 'Provide a creative graphic for social media sharing.<br><br>
<strong>Tips</strong>
<ul>
<li>Recommended image size: 1200 x 630 pixels.</li>
<li>Images that consist of more than 20% text may experience reduced delivery when paid promoting.</li>
<li>To preview your page — check <a target="_blank" href="https://developers.facebook.com/tools/debug/">Facebook Debugger</a> or <a target="_blank" href="https://www.linkedin.com/post-inspector/">LinkedIn Post Inspector</a>.</li>
<li>It may take up to 5 minutes for your changes to appear when previewing or sharing the page.</strong></li>
</ul>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => 60,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_sharing_image',
    'label' => 'Social media image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'images/events_social',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '8mb',
      'max_resolution' => '',
      'min_resolution' => '600x315',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 54,
    ),
  );

  // Exported field_instance: 'node-event-field_event_sharing_text'.
  $field_instances['node-event-field_event_sharing_text'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A short description for sharing on social media (124 - 150 characters).',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 61,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_sharing_text',
    'label' => 'Social media description',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 2,
      ),
      'type' => 'text_textarea',
      'weight' => 55,
    ),
  );

  // Exported field_instance: 'node-event-field_event_sponsors'.
  $field_instances['node-event-field_event_sponsors'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => 'Got an event sponsor? Show their company page <span style="color: #e93f33 !important;">♥</span> by listing them as a sponsor.<br>
<ul>
<li>To add a sponsor, search and select a company page from the dropdown.</li>
<li>A sponsor must have a company page in order to be listed as a sponsor.</li>
<li>Sponsors will also be added to our world-wide <a target="_blank" href="/sponsors">sponsors page</a>.</li>
</ul>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 19,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'use_content_language' => TRUE,
          'view_mode' => 'grid',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 10,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 36,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_sponsors',
    'label' => 'Sponsors',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'status' => 0,
        ),
      ),
      'nodeaccess_nodereference' => array(
        'all' => array(
          'view' => 'view',
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
        'unused' => 1,
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
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-event-field_event_timezone'.
  $field_instances['node-event-field_event_timezone'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Leave blank to use the chapter\'s timezone (or UTC if no chapter for event)
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'tzfield',
        'settings' => array(),
        'type' => 'tzfield_default',
        'weight' => 43,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_timezone',
    'label' => 'Event Timezone',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-event-field_event_total_attendance'.
  $field_instances['node-event-field_event_total_attendance'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'This will be displayed on the event page when it\'s a past event.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 25,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 50,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 45,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_total_attendance',
    'label' => 'How many people attended?',
    'required' => 0,
    'settings' => array(
      'max' => 100000,
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
      'weight' => 52,
    ),
  );

  // Exported field_instance: 'node-event-field_event_type'.
  $field_instances['node-event-field_event_type'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The type of event for the event page listing.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 27,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 52,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_type',
    'label' => 'Event Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-event-field_event_venue_name'.
  $field_instances['node-event-field_event_venue_name'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 12,
      ),
      'full' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 12,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_event_venue_name',
    'label' => 'Venue name',
    'required' => 1,
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
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'node-event-field_image'.
  $field_instances['node-event-field_image'] = array(
    'bundle' => 'event',
    'deleted' => 0,
    'description' => 'This will be used as the main image for the event.
<br>To support retina displays, upload your image sized to 1610x906',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => 6,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'banner',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
      'grid' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'banner_grid',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
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
    'field_name' => 'field_image',
    'label' => 'Image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'images/events',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-event-field_is_partner_event'.
  $field_instances['node-event-field_is_partner_event'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Indicate this event is a partner event not associated with a chapter (ie: a conference, exhibition)',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 14,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 57,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_is_partner_event',
    'label' => 'Partner Event',
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
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-event-field_location'.
  $field_instances['node-event-field_location'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Provide the street address or physical location for this event.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'full' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'grid' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 57,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'search_result' => array(
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
    'field_name' => 'field_location',
    'label' => 'Location',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 4,
      ),
      'type' => 'text_textarea',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'node-event-field_map_link'.
  $field_instances['node-event-field_map_link'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Help the attendees find the right location by providing a link to a map, for example <a href="http://maps.google.com/">Google Maps</a>.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(
          'custom_title' => '',
        ),
        'type' => 'link_default',
        'weight' => 3,
      ),
      'full' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 62,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(
          'custom_title' => '',
        ),
        'type' => 'link_default',
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
    'field_name' => 'field_map_link',
    'label' => 'Map link',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => '_blank',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 0,
      'rel_remove' => 'default',
      'title' => 'value',
      'title_label_use_field_label' => FALSE,
      'title_maxlength' => 128,
      'title_value' => 'View map',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 22,
    ),
  );

  // Exported field_instance: 'node-event-field_notify_featured_members'.
  $field_instances['node-event-field_notify_featured_members'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Send event notifications when attendees register or unregister for this event<br><br>
',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 36,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_notify_featured_members',
    'label' => 'Allow featured members to receive event notifications',
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
      'weight' => 37,
    ),
  );

  // Exported field_instance: 'node-event-field_organizer'.
  $field_instances['node-event-field_organizer'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => 'Who is organizing this event? Organizers will be notified of all event registrations.
<br>If set to none, then all organizers will be notified of any registrations and waitlists.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 10,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'use_content_language' => TRUE,
          'view_mode' => 'grid',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 17,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 60,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'bypass_access' => FALSE,
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 10,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_organizer',
    'label' => 'Organizers',
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
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-event-field_register_event_message'.
  $field_instances['node-event-field_register_event_message'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Include any additional information you want to provide to members only when they register.<br>
This will be displayed to the member on the event page and included in the automatic registration notification email.
',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 55,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_register_event_message',
    'label' => 'Message When Registered',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 3,
      ),
      'type' => 'text_textarea',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-event-field_rsvp_link'.
  $field_instances['node-event-field_rsvp_link'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Provide a link to the page with the RSVP (ie: http://www.eventbrite.com/e/your-event-url)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(
          'custom_title' => '',
        ),
        'type' => 'link_default',
        'weight' => 9,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'link',
        'settings' => array(
          'custom_title' => '',
        ),
        'type' => 'link_default',
        'weight' => 1,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 63,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'search_result' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(
          'custom_title' => '',
        ),
        'type' => 'link_default',
        'weight' => 9,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_rsvp_link',
    'label' => 'RSVP link',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => 'external bttn bttn-secondary bttn-m',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => '_blank',
        'title' => 'External RSVP',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'rel_remove_internal',
      'title' => 'value',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 128,
      'title_value' => 'RSVP',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 13,
    ),
  );

  // Exported field_instance: 'node-event-field_rsvp_link_title'.
  $field_instances['node-event-field_rsvp_link_title'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Change the text of the RSVP button (default: RSVP)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 57,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_rsvp_link_title',
    'label' => '(Moderator) RSVP link text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 15,
      ),
      'type' => 'text_textfield',
      'weight' => 14,
    ),
  );

  // Exported field_instance: 'node-event-field_waitlist_event_message'.
  $field_instances['node-event-field_waitlist_event_message'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Include any additional information you want to provide to members only when they are waitlisted.<br>
This will be displayed to the member on the event page and included in the automatic waitlist notification email.
',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 54,
      ),
      'grid' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'list' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_waitlist_event_message',
    'label' => 'Message When Waitlisted',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 3,
      ),
      'type' => 'text_textarea',
      'weight' => 18,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('(Moderator) RSVP link text');
  t('<b>⬆ This will only be displayed to registered attendees.</b>
');
  t('<span id=\'tm-event-edit-company-description\'>The company hosting this event.</span>');
  t('A short description for sharing on social media (124 - 150 characters).');
  t('Add a custom banner at the top of the event.<br>
Provide HTML markup or a template alias.');
  t('Allow Chapter Leaders to send announcements');
  t('Allow featured members to download the attendee list');
  t('Allow featured members to receive event notifications');
  t('Assign a label for featured members on the event listing. Leave blank for default.
<br>example: Keynote Speakers
<br><br>');
  t('Change Owner');
  t('Change the owner of the event.');
  t('Change the text of the RSVP button (default: RSVP)');
  t('Chapter members are invited <b>24 hours</b> before the event starts to register.<br>
Registered attendees are reminded <b>1.5 hours</b> and <b>10 minutes</b> before you go live.<br>');
  t('Chapters that you publish this event to will be able to announce the event.');
  t('Choose who can see your live stream.<br>
Is your live stream private? Set your YouTube Visibility to \'Unlisted\'.');
  t('Cover Video');
  t('Custom Banner');
  t('Description');
  t('Display event on the front page (for non-chapter events).');
  t('Display live chat');
  t('Display the live stream on the event page.');
  t('Embed your YouTube live stream on the event page.
<br>Example: https://www.youtube.com/watch?v=rbtv2zxhufM');
  t('Enable an online chat box for attendee discussion.');
  t('Event Chapter');
  t('Event Company');
  t('Event Timezone');
  t('Event Type');
  t('Featured members');
  t('Featured members label');
  t('Featured members will be able to download the attendee list from the event settings menu');
  t('Got an event sponsor? Show their company page <span style="color: #e93f33 !important;">♥</span> by listing them as a sponsor.<br>
<ul>
<li>To add a sponsor, search and select a company page from the dropdown.</li>
<li>A sponsor must have a company page in order to be listed as a sponsor.</li>
<li>Sponsors will also be added to our world-wide <a target="_blank" href="/sponsors">sponsors page</a>.</li>
</ul>');
  t('Health and Safety Guidelines');
  t('Help the attendees find the right location by providing a link to a map, for example <a href="http://maps.google.com/">Google Maps</a>.');
  t('Hide Organizers');
  t('Hide the list of organizers on the event page.');
  t('Highlight members of this event such as speakers or workshops leaders');
  t('How many non-registered guests are coming?<br>
This will be added to the attendee count on the event page.');
  t('How many people attended?');
  t('How to join the event');
  t('Image');
  t('Include any additional information you want to provide to members only when they are waitlisted.<br>
This will be displayed to the member on the event page and included in the automatic waitlist notification email.
');
  t('Include any additional information you want to provide to members only when they register.<br>
This will be displayed to the member on the event page and included in the automatic registration notification email.
');
  t('Increase your event\'s exposure with a cover video. 
<br>Example: https://www.youtube.com/watch?v=rbtv2zxhufM<br>
<b>Live Streaming?</b> See the <i>Online Events</i> section below.');
  t('Indicate this event is a partner event not associated with a chapter (ie: a conference, exhibition)');
  t('Leave blank to use the chapter\'s timezone (or UTC if no chapter for event)
<br><br>');
  t('Live stream on event page');
  t('Location');
  t('Map link');
  t('Members can participate online (ie: webinar, live stream, Twitter chat)
<br>');
  t('Message When Registered');
  t('Message When Waitlisted');
  t('Non-registered Guests');
  t('Only people with the link can see the event.<br><br>');
  t('Only show location to registered attendees');
  t('Organizers');
  t('Partner Event');
  t('Payment commission (%)');
  t('Preview mode');
  t('Promote event to front page');
  t('Provide a creative graphic for social media sharing.<br><br>
<strong>Tips</strong>
<ul>
<li>Recommended image size: 1200 x 630 pixels.</li>
<li>Images that consist of more than 20% text may experience reduced delivery when paid promoting.</li>
<li>To preview your page — check <a target="_blank" href="https://developers.facebook.com/tools/debug/">Facebook Debugger</a> or <a target="_blank" href="https://www.linkedin.com/post-inspector/">LinkedIn Post Inspector</a>.</li>
<li>It may take up to 5 minutes for your changes to appear when previewing or sharing the page.</strong></li>
</ul>');
  t('Provide a link to the page with the RSVP (ie: http://www.eventbrite.com/e/your-event-url)');
  t('Provide the street address or physical location for this event.');
  t('Publish to Chapters');
  t('RSVP link');
  t('Restrict displaying the event location to registered attendees only.');
  t('Restrict registration and waitlist to members who are verified.<br>
Non-verified members won\'t be able to sign up.');
  t('Select additional chapters associated with the event.
<br>The event will be published in these chapters also.');
  t('Send event notifications when attendees register or unregister for this event<br><br>
');
  t('Send payout to');
  t('Send reminders');
  t('Share any COVID-19 or health and safety guidelines for this event.<br>
This will be displayed on the event page for all members.');
  t('Social media description');
  t('Social media image');
  t('Sponsors');
  t('The chapter hosting this event.<br>
<strong>Publish to multiple chapters? See the <i>Event Chapters</i> section.</strong>');
  t('The percentage of total event revenue to payout.
<br>Note: Does not include bank fees, forex or other charges.');
  t('The type of event for the event page listing.');
  t('This is an online event');
  t('This will be displayed on the event page when it\'s a past event.');
  t('This will be used as the main image for the event.
<br>To support retina displays, upload your image sized to 1610x906');
  t('Venue name');
  t('Verified Members Only');
  t('When');
  t('Who can register for your event?');
  t('Who can watch the live stream?');
  t('Who is organizing this event? Organizers will be notified of all event registrations.
<br>If set to none, then all organizers will be notified of any registrations and waitlists.');
  t('Who will we send the event payout to?');
  t('You can restrict registrations to members in your chapter, or country.');
  t('YouTube live stream URL');

  return $field_instances;
}

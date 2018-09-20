<?php
/**
 * @file
 * tm_events.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function tm_events_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_action_menu_wrapper|node|event|full';
  $field_group->group_name = 'group_action_menu_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Action menu wrapper',
    'weight' => '4',
    'children' => array(
      0 => 'group_action_menu',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Action menu wrapper',
      'instance_settings' => array(
        'classes' => 'actions',
        'element' => 'div',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_action_menu_wrapper|node|event|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_action_menu|node|event|full';
  $field_group->group_name = 'group_action_menu';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_action_menu_wrapper';
  $field_group->data = array(
    'label' => 'Action menu',
    'weight' => '18',
    'children' => array(
      0 => 'field_rsvp_link',
      1 => 'ds_flag_event_waitlist',
      2 => 'ds_flag_event_register',
      3 => 'tm_ds_more_actions',
      4 => 'tm_ds_communication_actions',
      5 => 'tm_ds_anon_flag_placeholder',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Action menu',
      'instance_settings' => array(
        'classes' => 'actions-menu',
        'element' => 'ul',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
        'id' => '',
      ),
    ),
  );
  $field_groups['group_action_menu|node|event|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_community_wrapper|node|event|full';
  $field_group->group_name = 'group_community_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Who\'s Going?',
    'weight' => '10',
    'children' => array(
      0 => 'members_signed_up',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Who\'s Going?',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_community_wrapper|node|event|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details_wrapper|node|event|full';
  $field_group->group_name = 'group_details_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'About',
    'weight' => '6',
    'children' => array(
      0 => 'body',
      1 => 'tm_ds_event_price',
      2 => 'tm_ds_event_refund_policy',
      3 => 'tm_ds_fulldate_event',
      4 => 'tm_ds_event_message',
      5 => 'tm_ds_event_location',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'About',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_details_wrapper|node|event|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_event_chapters|node|event|form';
  $field_group->group_name = 'group_event_chapters';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Event Chapters',
    'weight' => '10',
    'children' => array(
      0 => 'field_chapter_publish',
      1 => 'field_chapter_can_announce',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Event Chapters',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-event-chapters field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_event_chapters|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_event_payout|node|event|form';
  $field_group->group_name = 'group_event_payout';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Payout',
    'weight' => '16',
    'children' => array(
      0 => 'field_event_payment_commission',
      1 => 'field_event_payment_payout_to',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Payout',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-event-payout field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_event_payout|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_event_settings|node|event|form';
  $field_group->group_name = 'group_event_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Event Settings',
    'weight' => '7',
    'children' => array(
      0 => 'field_approved_members_only',
      1 => 'field_register_event_message',
      2 => 'field_waitlist_event_message',
      3 => 'field_event_hide_location',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Event Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-event-settings field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_event_settings|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_events_wrapper|node|chapter|full';
  $field_group->group_name = 'group_events_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'chapter';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Events',
    'weight' => '6',
    'children' => array(
      0 => 'event_list',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Events',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_events_wrapper|node|chapter|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_extra_stuff|node|event|form';
  $field_group->group_name = 'group_extra_stuff';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Extra Stuff',
    'weight' => '12',
    'children' => array(
      0 => 'field_event_extra_guests',
      1 => 'field_event_total_attendance',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Extra Stuff',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-extra-stuff field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_extra_stuff|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_featured_members|node|event|form';
  $field_group->group_name = 'group_featured_members';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Featured members',
    'weight' => '11',
    'children' => array(
      0 => 'field_event_featured_access_rsvp',
      1 => 'field_event_featured_label',
      2 => 'field_event_featured_members',
      3 => 'field_notify_featured_members',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Featured members',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-featured-members field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_featured_members|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_featured_wrapper|node|event|full';
  $field_group->group_name = 'group_featured_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Featured Members',
    'weight' => '8',
    'children' => array(
      0 => 'field_event_featured_members',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'classes' => 'group-featured-wrapper field-group-contained_block',
      ),
    ),
  );
  $field_groups['group_featured_wrapper|node|event|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_about|node|event|form';
  $field_group->group_name = 'group_form_about';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'About',
    'weight' => '1',
    'children' => array(
      0 => 'field_external_rsvp',
      1 => 'field_signup_limit',
      2 => 'body',
      3 => 'field_image',
      4 => 'field_organizer',
      5 => 'field_rsvp_link',
      6 => 'field_chapter',
      7 => 'field_event_company',
      8 => 'field_event_cover_video',
      9 => 'field_rsvp_link_title',
      10 => 'title',
      11 => 'path',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'About',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'group-form-about field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_form_about|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_form_where|node|event|form';
  $field_group->group_name = 'group_form_where';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Where',
    'weight' => '2',
    'children' => array(
      0 => 'field_event_venue_name',
      1 => 'field_location',
      2 => 'field_map_link',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Where',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => 'group-form-where field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_form_where|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_location_wrapper|node|event|full';
  $field_group->group_name = 'group_location_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Location',
    'weight' => '28',
    'children' => array(
      0 => 'field_event_venue_name',
      1 => 'field_location',
      2 => 'field_map_link',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Location',
      'instance_settings' => array(
        'classes' => 'detail-item location',
        'element' => 'div',
        'show_label' => '1',
        'label_element' => 'h2',
        'attributes' => '',
      ),
    ),
  );
  $field_groups['group_location_wrapper|node|event|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_moderator_fields|node|event|form';
  $field_group->group_name = 'group_moderator_fields';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Moderator Settings',
    'weight' => '0',
    'children' => array(
      0 => 'field_is_partner_event',
      1 => 'field_event_type',
      2 => 'field_event_hide_organizers',
      3 => 'field_event_reassign_owner',
      4 => 'field_event_custom_banner',
      5 => 'field_event_is_promoted',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Moderator Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-moderator-fields field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_moderator_fields|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_organizers_wrapper|node|event|full';
  $field_group->group_name = 'group_organizers_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Organizers',
    'weight' => '9',
    'children' => array(
      0 => 'field_organizer',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Organizers',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_organizers_wrapper|node|event|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sponsors|node|event|form';
  $field_group->group_name = 'group_sponsors';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Event Sponsors',
    'weight' => '6',
    'children' => array(
      0 => 'field_event_sponsors',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Event Sponsors',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-sponsors field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sponsors|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sponsors|node|event|full';
  $field_group->group_name = 'group_sponsors';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Sponsors',
    'weight' => '7',
    'children' => array(
      0 => 'field_event_sponsors',
    ),
    'format_type' => 'contained_block',
    'format_settings' => array(
      'label' => 'Sponsors',
      'formatter' => 'open',
    ),
  );
  $field_groups['group_sponsors|node|event|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_summary_wrapper|node|event|search_result';
  $field_group->group_name = 'group_summary_wrapper';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'search_result';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Summary',
    'weight' => '3',
    'children' => array(
      0 => 'body',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Summary',
      'instance_settings' => array(
        'classes' => 'summary',
        'element' => 'div',
        'show_label' => '0',
        'label_element' => '',
        'attributes' => '',
      ),
    ),
  );
  $field_groups['group_summary_wrapper|node|event|search_result'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('About');
  t('Action menu');
  t('Action menu wrapper');
  t('Event Chapters');
  t('Event Settings');
  t('Event Sponsors');
  t('Events');
  t('Extra Stuff');
  t('Featured Members');
  t('Featured members');
  t('Location');
  t('Moderator Settings');
  t('Organizers');
  t('Payout');
  t('Sponsors');
  t('Summary');
  t('Where');
  t('Who\'s Going?');

  return $field_groups;
}

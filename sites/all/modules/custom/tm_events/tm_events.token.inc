<?php

// tm_events.token.inc - token hooks

/**
 * Token replacement for event. Return year, month or day for field_event_date 
 */
function tm_events_token_info() { 
  $info['tokens']['node']['tm-event-date-year'] = array(
    'name' => t('TM Event date (YYYY)'),
    'description' => t('Returns the event date year)'),
  );
  $info['tokens']['node']['tm-event-date-month'] = array(
    'name' => t('TM Event date (mm)'),
    'description' => t('Returns the event date month)'),
  );
  $info['tokens']['node']['tm-event-date-day'] = array(
    'name' => t('TM Event date (dd)'),
    'description' => t('Returns the event date day)'),
  );
  return $info;
}

/** 
 * Token replacement for event. Return year, month or day for field_event_date
 */
function tm_events_tokens($type, $tokens, array $data = array(), array $options = array()) {
 
  $replacements = array();   

  if (($type == 'node') && !empty($data['node'])) {
    $event = $data['node'];

    foreach ($tokens as $name => $original) {     
      switch ($name) {

        case 'tm-event-date-year':
          $event_timezone = _tm_get_event_timezone($event);
          $event_date = new DateTime($event->field_event_date['und'][0]['value'], new DateTimeZone($event_timezone));
          $replacements[$original] = $event_date->format("Y");
          break;

        case 'tm-event-date-month':
          $event_timezone = _tm_get_event_timezone($event);
          $event_date = new DateTime($event->field_event_date['und'][0]['value'], new DateTimeZone($event_timezone));
          $replacements[$original] = $event_date->format("m");
          break;

        case 'tm-event-date-day':
          $event_timezone = _tm_get_event_timezone($event);
          $event_date = new DateTime($event->field_event_date['und'][0]['value'], new DateTimeZone($event_timezone));
          $replacements[$original] = $event_date->format("d");
          break;

      } // end switch
    } // end foreach
  } // end if

  return $replacements;
}
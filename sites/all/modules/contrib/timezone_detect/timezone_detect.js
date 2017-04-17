/**
 * @file
 * Determine and set user's timezone on page load.
 * 
 * @todo: Use Drupal.behaviors?
 */
jQuery(document).ready(function () {

  // Determine timezone from browser client using jsTimezoneDetect library.
  var tz = jstz.determine();

  if (tz.name() != Drupal.settings.timezone_detect.current_timezone) {

    // Post timezone to callback url via ajax.
    jQuery.ajax({
      type: 'POST',
      url: Drupal.settings.basePath + 'timezone-detect/ajax/set-timezone',
      dataType: 'json',
      data: {
        timezone: tz.name(), 
        token: Drupal.settings.timezone_detect.token
      }
    });

    // Set any timezone select on this page to the detected timezone.
    jQuery('select[name="timezone"] > option[value="' + tz.name() + '"]')
      .closest('select')
      .val(tz.name());
  }

});

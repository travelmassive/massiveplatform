/* Event edit methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// Update page title
	if (typeof Drupal.settings.tm_events !== 'undefined') {

		// Fetch form subtitle
		var tm_events_form_subtitle = null;
		if (typeof Drupal.settings.tm_events.form_subtitle !== 'undefined') {
			tm_events_form_subtitle = Drupal.settings.tm_events.form_subtitle;
		}

		// Fetch form heading
		var tm_events_form_heading = null;
		if (typeof Drupal.settings.tm_events.form_heading !== 'undefined') {
			tm_events_form_heading = Drupal.settings.tm_events.form_heading;
		}
		
		// Set page title
		if ((tm_events_form_subtitle != null) && (tm_events_form_heading != null)) {
			$("#page-title").html('<em>' + tm_events_form_subtitle + '</em>' + tm_events_form_heading);
		}
	}

	// If early bird date selected, give the time field a default if not already set
	$("#edit-field-event-payment-early-date-und-0-value-datepicker-popup-0").change(function() {
		if ($("#edit-field-event-payment-early-date-und-0-value-timeEntry-popup-1").val() == "") {
			$("#edit-field-event-payment-early-date-und-0-value-timeEntry-popup-1").val("12:00");
		}
	});

});})(jQuery, Drupal, this, this.document);
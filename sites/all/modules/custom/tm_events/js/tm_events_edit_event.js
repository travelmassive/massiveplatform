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

});})(jQuery, Drupal, this, this.document);
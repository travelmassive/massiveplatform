/* Event announcement methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// Update page title
	$("#page-title").html('<em>Event: <a href="/' + Drupal.settings.tm_events.event_url + '">' + Drupal.settings.tm_events.event_title + '</a></em>Manage Registrations');

});})(jQuery, Drupal, this, this.document);
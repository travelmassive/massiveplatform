/* Organization js methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){
	
	// If no upcoming events, hide
	$('.contained.contained-block :contains("No upcoming events.")').parent().remove();

	// Change link to past events to /events
	$('.more-link :contains("View past events")').parent().html("<a href='/events'>Find more events</a>");

});})(jQuery, Drupal, this, this.document);
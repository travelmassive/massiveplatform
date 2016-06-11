(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// Update 'No upcoming events message'
	if (typeof Drupal.settings.tm_chapters !== 'undefined') {

		var tm_chapters_leaders_needed_message = null;
		if (typeof Drupal.settings.tm_chapters.leaders_needed_message !== 'undefined') {
			tm_chapters_leaders_needed_message = Drupal.settings.tm_chapters.leaders_needed_message;
			$( ".view-empty:contains('No upcoming events.')" ).html(tm_chapters_leaders_needed_message);
		}
	}

});})(jQuery, Drupal, this, this.document);
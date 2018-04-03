jQuery(document).ready(function($) {

	// Update page title
	var original_page_title = $("#page-title").html();
	jQuery("#page-title").html('<em><a href="/' + Drupal.settings.tm_track_views_title.profile_url + '">' + Drupal.settings.tm_track_views_title.profile_label + '</a></em>' + original_page_title);

});
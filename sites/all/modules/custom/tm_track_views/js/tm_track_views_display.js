jQuery(document).ready(function($) {

	var tm_track_view_display = function(message) {

		$.prompt(message, {
			buttons: {}, 
			loaded: function() { 
				$("#main").addClass("tm-blur-filter-large");
				$(".jqiclose").hide();
			},
			close: function() { 
				//$("#main").removeClass("tm-blur-filter-large");
			}
		});
	}
	
	// show message
	var tm_track_views_display_message = Drupal.settings.tm_track_views.display_message;
	tm_track_view_display(tm_track_views_display_message);

});
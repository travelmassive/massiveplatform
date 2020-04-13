(function ($, Drupal, window, document, undefined) {

	jQuery(document).ready(function() {

	// some globals to keep track of stuff
	var tm_connections_base_url = "/match";
	var tm_connections_page_number = 2; // start at page 2
	var tm_connections_load_more_clicked = false;

	// waypoints
	var tm_connections_waypoint_count = 1;
	var tm_connections_waypoint_max = 10;
	var tm_connections_waypoint;

	// user id
	var tm_connections_user_uid = jQuery("#connections-user-uid").val();

	// load results
	loadMoreConnectionItems = function() {

		// Check not already clicked
		if (tm_connections_load_more_clicked) {
			return;
		}

		// clicked
		tm_connections_load_more_clicked = true;
		
		// Loading an extra page of listing result
		$(".pager.pager-load-more").show();
		$(".connections.load-more-spinner").show();
		$("#connections-load-more").text("Loading...");

		// Perform ajax query to match api	
		xhr = $.ajax({
			url: tm_connections_base_url + "/ajax/connections",
			data: { 

				// parameters
				"page": tm_connections_page_number,
				"user_id": tm_connections_user_uid,

			},
			type: "GET",
			success: function(response) {

				// Handle insertion of results
					
				// Keep track of current page
				tm_connections_page_number = tm_connections_page_number + 1;

				// UI updates
				tm_connections_load_more_clicked = false;
				$(".connections.spinner").hide();
				$(".connections.load-more-spinner").hide();
				$(".tm-connections-list").append(response);
				$(".connections.results").show();
				$(".connections.placeholder").addClass("has-results");
				$("#connections-loading-more").hide();
				$("#connections-load-more").text("Show more");
				tm_connections_attach_waypoint();
				
			},
			error: function(xhr) {
				// This is also reached when we cancel the xhr request
			}
		});
	}

	// attach waypoint to load more
	tm_connections_attach_waypoint = function() {

		// check for load more
		if (document.getElementById('connections-load-more') == null) {
			return;
		}

		// don't attach if reached limit
		if (tm_connections_waypoint_count >= tm_connections_waypoint_max) {
			return;
		}

		// remove any existing waypoints
		if (tm_connections_waypoint != null) {
			tm_connections_waypoint.destroy();
		}

		// add waypoint
		tm_connections_waypoint = new Waypoint({
		  element: document.getElementById('connections-load-more'),
		  handler: function() {
		  	tm_connections_waypoint_count++;
		    this.element.click();
		    tm_connections_waypoint.destroy();
		  },
		  offset: '100%'
		})

	}


	// attach onclick
	jQuery("#connections-load-more").on('click', loadMoreConnectionItems);

	// attach first waypoint
	tm_connections_attach_waypoint();


});
})(jQuery, Drupal, this, this.document);
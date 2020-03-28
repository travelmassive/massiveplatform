(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

	// some globals to keep track of stuff
	var tm_lists_base_url = "/lists";
	var tm_lists_page_number = 2; // start at page 2
	var tm_lists_load_more_clicked = false;

	// waypoints
	var tm_list_waypoint_count = 1;
	var tm_list_waypoint_max = 5;
	var tm_list_waypoint;

	// load results
	loadMoreListItems = function() {

		// Check not already clicked
		if (tm_lists_load_more_clicked) {
			return;
		}

		// clicked
		tm_lists_load_more_clicked = true;
		
		// Loading an extra page of listing result
		$(".pager.pager-load-more").show();
		$(".list.load-more-spinner").show();
		$("#list-load-more").text("Loading...");

		// Perform ajax query to list api	
		xhr = $.ajax({
			url: tm_lists_base_url + "/ajax/items",
			data: { 

				// parameters
				"list_id": tm_global_list_id,
				"sort_method": tm_global_list_sort_method,
				"page": tm_lists_page_number,

			},
			type: "GET",
			success: function(response) {

				// Handle insertion of results
					
				// Keep track of current page
				tm_lists_page_number = tm_lists_page_number + 1;

				// UI updates
				tm_lists_load_more_clicked = false;
				$(".list.spinner").hide();
				$(".list.load-more-spinner").hide();
				$(".tm-list-items-list").append(response);
				$(".list.results").show();
				$(".list.placeholder").addClass("has-results");
				$("#list-results-loading-more").hide();
				$("#list-load-more").text("Show more");
				tm_list_attach_waypoint();
				applyDropdownsOnListMenus(tm_lists_page_number - 1);
				
			},
			error: function(xhr) {
				// This is also reached when we cancel the xhr request
			}
		});
	}

	// apply dropd on list menus
	applyDropdownsOnListMenus = function(page) {

		$('.list-page-' + page + ' [data-dropd-toggle]').click(function(e) {

			e.preventDefault();
			e.stopPropagation();
			var _self = $(this);
			var $drop = _self.closest('[data-dropd-wrapper]').find('[data-dropd]');

			// Hide others.
			$('[data-dropd-toggle]').not(_self).removeClass('on');
			$('[data-dropd]').not($drop).removeClass('on');

			// Set top value.
			$drop.css('top', _self.height());

			if (_self.hasClass('on')) {
				_self.removeClass('on');
				$drop.removeClass('on');
			}
			else {
				_self.addClass('on');
				$drop.addClass('on');
			}
		});
	}

	// attach waypoint to load more
	tm_list_attach_waypoint = function() {

		// check for load more
		if (document.getElementById('list-load-more') == null) {
			return;
		}

		// don't attach if reached limit
		if (tm_list_waypoint_count >= tm_list_waypoint_max) {
			return;
		}

		// remove any existing waypoints
		if (tm_list_waypoint != null) {
			tm_list_waypoint.destroy();
		}

		// add waypoint
		tm_list_waypoint = new Waypoint({
		  element: document.getElementById('list-load-more'),
		  handler: function() {
		  	tm_list_waypoint_count++;
		    this.element.click();
		    tm_list_waypoint.destroy();
		  },
		  offset: '100%'
		})

	}

	// show item
	tm_list_feature_item = function(entity_type, entity_id) {

		name = decodeURIComponent(name);  

		var message = "<div id='feature_list_item'><img src='/sites/all/themes/tm/images/load-more-ajax-loader-2.gif'></div>";

		$.prompt(message, { 
			classes: { box: 'tm_list_featured'}, 
			persistent: false,
			buttons: {},
			title: null,
			loaded: function() {
				// show list item
				$("#feature_list_item").load("/lists/ajax/item/" + tm_global_list_id + "/" + entity_type + "/" + entity_id);
			},
			close: function() {
				// remove hash
				window.location.hash = "";
			}
		});

	}

	// feature a list item from a hash
	// ie: #feature-213737
	// first digit is entity_type, second digit is entity_id
	tm_list_feature_hash = function() {
		if (window.location.hash.match("feature-")) {
			$.prompt.close(); // close any existing features
			var hash = window.location.hash.split('#')[1];
			var featured_hash = hash.split('feature-')[1];
			var entity_type = featured_hash[0];
			var entity_id = featured_hash.substring(1);
			tm_list_feature_item(entity_type, entity_id);
		}
	}

	// attach onclick
	$("#list-load-more").on('click', loadMoreListItems);

	// attach first waypoint
	tm_list_attach_waypoint();

	// hash on page load
	if (window.location.hash) {
		tm_list_feature_hash();
	}

	// respond to hash change
	window.onhashchange = function() {
		tm_list_feature_hash();
	}

});})(jQuery, Drupal, this, this.document);

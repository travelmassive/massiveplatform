(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

	// some globals to keep track of stuff
	var tm_lists_base_url = "/lists";
	var tm_lists_page_number = 2; // start at page 2
	var tm_lists_load_more_clicked = false;

	// load results
	loadMoreListItems = function() {

		// Check not already clicked
		if (tm_lists_load_more_clicked) {
			return;
		}
		
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

	// attach onclick
	$("#list-load-more").on('click', loadMoreListItems);

});})(jQuery, Drupal, this, this.document);

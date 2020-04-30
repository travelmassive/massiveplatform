(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// keep track out of timeout
	var tm_event_changed_timeout = null;

	// event nid
	var tm_event_changed_nid = 0;
	if (typeof Drupal.settings.tm_events.tm_event_changed_nid !== 'undefined') {
		tm_event_changed_nid = Drupal.settings.tm_events.tm_event_changed_nid;
	}

	// timestamp when event last updated
	var tm_event_changed_timestamp = 0;
	if (typeof Drupal.settings.tm_events.tm_event_changed_timestamp !== 'undefined') {
		tm_event_changed_timestamp = Drupal.settings.tm_events.tm_event_changed_timestamp;
	}

	// how often to refresh
	var tm_event_changed_refresh = 1000;
	if (typeof Drupal.settings.tm_events.tm_event_changed_refresh !== 'undefined') {
		tm_event_changed_refresh = Drupal.settings.tm_events.tm_event_changed_refresh;
	}

	// check if user is logged in
	var tm_event_changed_user_is_logged_in = false;
	if (typeof Drupal.settings.tm_events.tm_event_changed_user_is_logged_in !== 'undefined') {
		tm_event_changed_user_is_logged_in = Drupal.settings.tm_events.tm_event_changed_user_is_logged_in;
	}

	// set timeout
	tm_event_check_update = function() {

		tm_event_changed_timeout = setTimeout(function() {

			// need a valid node id
			if (tm_event_changed_nid == 0) {
				return;
			}

			// fetch last changed time of event
			// add timestamp so we don't get cached
			jQuery.ajax({
				url: "/events/last-changed/" + tm_event_changed_nid + "?t=" + Date.now(),
				type: "GET",
				success: function(data) {
					if ((typeof data.timestamp !== 'undefined') && (typeof data.url !== 'undefined')) {

						// timestamp has changed
						if (data.timestamp > tm_event_changed_timestamp) {

							// if signed in, load new url
							// if not signed in, only refresh if url changes
							// (because we can get a stale tm_event_changed_timestamp from cached copies of page)
							if (tm_event_changed_user_is_logged_in) {
								window.location = data.url;
							} else if (window.location.pathname != data.url) {
								window.location = data.url;
							} else {
								tm_event_changed_timeout = null;
								tm_event_check_update();
							}

						} else {
							tm_event_changed_timeout = null;
							tm_event_check_update();
						}
					}
				},
				dataType: "json",
				timeout: 5000
			})
		}, tm_event_changed_refresh);
	};

	// check for updates
	tm_event_check_update();

});})(jQuery, Drupal, this, this.document);

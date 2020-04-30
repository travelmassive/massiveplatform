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
						if (data.timestamp > tm_event_changed_timestamp) {
							window.location = data.url; // .reload(true); 
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

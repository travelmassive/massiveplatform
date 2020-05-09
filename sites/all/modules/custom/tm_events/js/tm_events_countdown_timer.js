(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// get seconds to start
	var tm_event_countdown_to_start = null;
	if (typeof Drupal.settings.tm_events.tm_event_seconds_until_start !== 'undefined') {
		tm_event_countdown_to_start = Drupal.settings.tm_events.tm_event_seconds_until_start;
	}
	var tm_event_countdown_seconds = tm_event_countdown_to_start;
	var tm_event_countdown_started = null;
	var previous_seconds = tm_event_countdown_seconds;
	var tm_event_countdown_timer_interval = null;

	// check if user is logged in
	var tm_event_countdown_user_is_logged_in = false;
	if (typeof Drupal.settings.tm_events.tm_event_changed_user_is_logged_in !== 'undefined') {
		tm_event_countdown_user_is_logged_in = Drupal.settings.tm_events.tm_event_changed_user_is_logged_in;
	}

	// event nid
	var tm_event_countdown_nid = 0;
	if (typeof Drupal.settings.tm_events.tm_event_changed_nid !== 'undefined') {
		tm_event_countdown_nid = Drupal.settings.tm_events.tm_event_changed_nid;
	}

	// render a countdown timer into #tm_events_countdown_text 
	function tm_event_countdown_render() {

		if (tm_event_countdown_to_start == null) {
			return;
		}

		// calculate difference since countdown started
		var delta = Date.now() - tm_event_countdown_started;
		var seconds_to_go = Math.floor(((tm_event_countdown_seconds*1000) - delta) / 1000);

		var days        = Math.floor(seconds_to_go/24/60/60);
		var hoursLeft   = Math.floor((seconds_to_go) - (days*86400));
		var hours       = Math.floor(hoursLeft/3600);
		var minutesLeft = Math.floor((hoursLeft) - (hours*3600));
		var minutes     = Math.floor(minutesLeft/60);
		var seconds = seconds_to_go % 60;

		// don't render if seconds are same as last time
		if (seconds_to_go == previous_seconds) {
			return;
		}
		previous_seconds = seconds_to_go;

		function tm_event_timer_pad(n) {
			return (n < 10 ? "0" + n : n);
		}

		function tm_event_countdown_plural(n, text, seperator) {

			if (n == 0) {
				return "";
			}
			if (n == 1) {
				return seperator + n + " " + text;
			}

			return seperator + n + " " + text + "s";
		}

		var countdown_text = "";
		if (days > 0) {
			countdown_text = tm_event_countdown_plural(days, "day", "") + tm_event_countdown_plural(hours, "hour", ", ") + tm_event_countdown_plural(minutes, "minute", ", ") + tm_event_countdown_plural(seconds, "second", ", ");
		} else if (hours > 0) {
			countdown_text = tm_event_countdown_plural(hours, "hour", "") + tm_event_countdown_plural(minutes, "minute", ", ") + tm_event_countdown_plural(seconds, "second", ", ");
		} else if (minutes > 0) {
			countdown_text = tm_event_countdown_plural(minutes, "minute", "") + tm_event_countdown_plural(seconds, "second", ", ");
		} else if (seconds > 0) {
			countdown_text = tm_event_countdown_plural(seconds, "second", "");
		}

		// hide the timer after 1 hour of going live
		// (for signed out visitors looking at a cache)
		if (seconds_to_go < -3600) {
			jQuery(".messages--countdown-timer.messages.countdown-timer").hide();
		}

		// display the countdown timer
		if (seconds_to_go > 0) {
			document.getElementById('tm_events_countdown_text').innerHTML = "in " + countdown_text + ".";
		} else {
			document.getElementById('tm_events_countdown_text').innerHTML = "now...";
			clearInterval(tm_event_countdown_timer_interval); // stop interval
		}
	
	}

	// fetch the countdown time, and then start the timer
	function tm_event_countdown_callback() {

		// check we have nid
		if (tm_event_countdown_nid == 0) {
			return;
		}

		// fetch last changed time of event
		// add timestamp so we don't get cached
		jQuery.ajax({
			url: "/events/seconds-to-event-callback/" + tm_event_countdown_nid + "?t=" + Date.now(),
			type: "GET",
			success: function(data) {

				if (typeof data.seconds !== 'undefined') {

					// set seconds
					tm_event_countdown_seconds = data.seconds;
					
					// start countdown timer and renderer
					tm_event_countdown_started = Date.now();
					tm_event_countdown_timer_interval = setInterval(function(){ tm_event_countdown_render(); }, 1000);

				}
			},
			dataType: "json",
			timeout: 5000
		});	
	}

	// if the member is logged in, we can rely on embedded timer var
	// if the visitor is logged out, we fetch the seconds from a callback to prevent stale cache
	if (tm_event_countdown_user_is_logged_in) {

		// start countdown timer and renderer
		tm_event_countdown_started = Date.now();
		tm_event_countdown_timer_interval = setInterval(function(){ tm_event_countdown_render(); }, 1000);

	} else {

		// fetch the countdown time, and then start the timer
		tm_event_countdown_callback();
	}

});})(jQuery, Drupal, this, this.document);

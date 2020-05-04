(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// get seconds to start
	var tm_event_countdown_to_start = null;
	if (typeof Drupal.settings.tm_events.tm_event_seconds_until_start !== 'undefined') {
		tm_event_countdown_to_start = Drupal.settings.tm_events.tm_event_seconds_until_start;
	}
	var tm_event_countdown_seconds = tm_event_countdown_to_start;
	var tm_event_countdown_started = null;
	var previous_seconds = tm_event_countdown_seconds;

	// render a countdown timer into #tm_events_countdown_text 
	function tm_event_countdown_render() {

		if (tm_event_countdown_to_start == null) {
			return;
		}

		// calculate difference since countdown started
		var delta = Date.now() - tm_event_countdown_started;
		var seconds = Math.floor(((tm_event_countdown_seconds*1000) - delta) / 1000);

		var days        = Math.floor(seconds/24/60/60);
		var hoursLeft   = Math.floor((seconds) - (days*86400));
		var hours       = Math.floor(hoursLeft/3600);
		var minutesLeft = Math.floor((hoursLeft) - (hours*3600));
		var minutes     = Math.floor(minutesLeft/60);
		var seconds = seconds % 60;

		// don't render if seconds are same as last time
		if (seconds == previous_seconds) {
			return;
		}
		previous_seconds = seconds;

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

		if (seconds > 0) {
			document.getElementById('tm_events_countdown_text').innerHTML = "in " + countdown_text + ".";
		}
		if (seconds < 0) {
			document.getElementById('tm_events_countdown_text').innerHTML = "now...";
			clearInterval(tm_event_countdown_timer_interval); // stop interval
		}
		if (seconds == 0) {
			clearInterval(tm_event_countdown_timer_interval);
			document.getElementById('tm_events_countdown_text').innerHTML = "now...";
			clearInterval(tm_event_countdown_timer_interval); // stop interval
		}
	}

	// start countdown timer and renderer
	var tm_event_countdown_started = Date.now();
	var tm_event_countdown_timer_interval = setInterval(function(){ tm_event_countdown_render(); }, 1000);

});})(jQuery, Drupal, this, this.document);

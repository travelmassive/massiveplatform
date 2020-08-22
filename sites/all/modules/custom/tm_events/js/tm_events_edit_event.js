/* Event edit methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// Update page title
	if (typeof Drupal.settings.tm_events !== 'undefined') {

		// Fetch form subtitle
		var tm_events_form_subtitle = null;
		if (typeof Drupal.settings.tm_events.form_subtitle !== 'undefined') {
			tm_events_form_subtitle = Drupal.settings.tm_events.form_subtitle;
		}

		// Fetch form heading
		var tm_events_form_heading = null;
		if (typeof Drupal.settings.tm_events.form_heading !== 'undefined') {
			tm_events_form_heading = Drupal.settings.tm_events.form_heading;
		}
		
		// Set page title
		if ((tm_events_form_subtitle != null) && (tm_events_form_heading != null)) {
			$("#page-title").html('<em>' + tm_events_form_subtitle + '</em>' + tm_events_form_heading);
		}
	}

	// If early bird date selected, give the time field a default if not already set
	$("#edit-field-event-payment-early-date-und-0-value-datepicker-popup-0").change(function() {
		if ($("#edit-field-event-payment-early-date-und-0-value-timeEntry-popup-1").val() == "") {
			$("#edit-field-event-payment-early-date-und-0-value-timeEntry-popup-1").val("12:00");
		}
	});

	// Online event settings
	$("#edit-field-event-is-online-und").change(function() {

		// If online event checked
		if ($("#edit-field-event-is-online-und").prop("checked") == true) {

			// Turn on reminders
			$("#edit-field-event-send-reminders-und").prop("checked", true);

			// Default value for venue name
			if ($("#edit-field-event-venue-name-und-0-value").val() == "") {
				$("#edit-field-event-venue-name-und-0-value").val("This is an online event");
			}

			// Default value for location
			if ($("#edit-field-location-und-0-value").val() == "") {
				$("#edit-field-location-und-0-value").val("Join us online");
			}
		}

		// If not online event, turn off reminders
		if ($("#edit-field-event-is-online-und").prop("checked") == false) {
			$("#edit-field-event-send-reminders-und").prop("checked", false);
		}

	});

	// Event reminder settings
	$("#edit-field-event-send-reminders-und").change(function() {
		if ($("#edit-field-event-is-online-und").prop("checked") == false) {
			jq_alert(null, "Reminders are only available for <i>Online Events</i>.");
			$("#edit-field-event-send-reminders-und").prop("checked", false);
		}
	});

	// Default value for event instructions if YouTube url entered
	$("#edit-field-event-livestream-video-url-und-0-value").change(function() {
		if ($("#edit-field-event-livestream-video-url-und-0-value").val().includes("youtu")) {
			if ($("#edit-field-event-online-instructions-und-0-value").val() == "") {
				if ($("#edit-field-event-chatbox-enabled-und").prop("checked") == true) {
					$("#edit-field-event-online-instructions-und-0-value").val("Watch the live stream on the event page and join the live chat.");
				} else {
					$("#edit-field-event-online-instructions-und-0-value").val("Watch the live stream on the event page.");
				}
			}
		}
	});

	// Handle event payment change
	function tm_event_payment_type_change() {

		var tm_event_payment_type = $("#edit-field-event-payment-type-und").val();

		// no_payments
		if ((tm_event_payment_type == null) || (tm_event_payment_type == "no_payments")) {
			console.log("no payments");

			// hide all fields
			$(".group-event-payment .form-wrapper").hide();
			$(".group-event-early-bird").hide();
			$(".form-field-name-field-event-payment-early-date").hide();
			$(".group-event-payout").hide();

			// show payment option field
			$(".form-field-name-field-event-payment-type").show();
		}

		// fixed_price
		if (tm_event_payment_type == "fixed_price") {

			// show all fields
			$(".group-event-payment .form-wrapper").show();
			$(".group-event-payout").show();

			// hide tiers, show fixed pricing
			$("#field-event-payment-tiers-add-more-wrapper").hide();
			$("#field-event-payment-amount-add-more-wrapper").show();

			// show early bird
			$(".group-event-early-bird").show();
			$(".form-field-name-field-event-payment-early-date").show();
			
		}

		// pay_what_you_wish
		if (tm_event_payment_type == "pay_what_you_wish") {
			
			// show all fields
			$(".group-event-payment .form-wrapper").show();
			$(".group-event-payout").show();

			// show tiers, hide fixed pricing
			$("#field-event-payment-tiers-add-more-wrapper").show();
			$("#field-event-payment-amount-add-more-wrapper").hide();

			// hide early bird
			$(".group-event-early-bird").hide();
			$(".form-field-name-field-event-payment-early-date").hide();
		}

	}

	// Event payments
	$("#edit-field-event-payment-type-und").change(function() {
		tm_event_payment_type_change();
	});

	// Set up event payments on load
	tm_event_payment_type_change();


});})(jQuery, Drupal, this, this.document);
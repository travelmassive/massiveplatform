/* Subscription reports methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){
	
	// Update page title
	if (typeof Drupal.settings.tm_payments_reports !== 'undefined') {

		// Fetch form subtitle
		var tm_payments_subscriptions_form_subtitle = null;
		if (typeof Drupal.settings.tm_payments_reports.form_subtitle !== 'undefined') {
			tm_payments_subscriptions_form_subtitle = Drupal.settings.tm_payments_reports.form_subtitle;
		}

		// Fetch form heading
		var tm_payments_subscriptions_form_heading = null;
		if (typeof Drupal.settings.tm_payments_reports.form_heading !== 'undefined') {
			tm_payments_subscriptions_form_heading = Drupal.settings.tm_payments_reports.form_heading;
		}
		
		// Set page title
		if ((tm_payments_subscriptions_form_subtitle != null) && (tm_payments_subscriptions_form_heading != null)) {
			$("#page-title").html('<em>' + tm_payments_subscriptions_form_subtitle + '</em>' + tm_payments_subscriptions_form_heading);
		}
		
	}

});})(jQuery, Drupal, this, this.document);
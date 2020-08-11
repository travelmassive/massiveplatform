(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// get name of hide_cta cookie
	var hide_cta_cookie_name = Drupal.settings.tm_subscriptions_user.hide_cta_cookie_name;

	// Hide the CTA banner
	$('.hide-cta-banner-user').on('click', function(e) {
		$("#tm-subscriptions-cta-banner").hide();
		var cookie = hide_cta_cookie_name + '=1; domain=.' + window.location.host.toString() + '; path=/;';
		document.cookie = cookie;
	});

	// Show the CTA banner
    $('.show-cta-banner-user').on('click', function(e) {
		$("#tm-subscriptions-cta-banner").show();
		var cookie = hide_cta_cookie_name + '=0; domain=.' + window.location.host.toString() + '; path=/;';
		document.cookie = cookie;
	});

});})(jQuery, Drupal, this, this.document);

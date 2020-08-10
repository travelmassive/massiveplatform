jQuery(document).ready(function($) {

	// Hide the CTA banner
	$('.hide-cta-banner-user').on('click', function(e) {
		$("#tm-subscriptions-cta-banner").hide();
		var cookie = ['tm_subscriptions_hide_user_cta=1; domain=.', window.location.host.toString(), '; path=/;'].join('');
		document.cookie = cookie;
	});

	// Show the CTA banner
    $('.show-cta-banner-user').on('click', function(e) {
		$("#tm-subscriptions-cta-banner").show();
		var cookie = ['tm_subscriptions_hide_user_cta=0; domain=.', window.location.host.toString(), '; path=/;'].join('');
		document.cookie = cookie;
	});

});

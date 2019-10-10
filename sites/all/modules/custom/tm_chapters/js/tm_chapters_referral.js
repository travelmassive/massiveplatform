(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	// Set Drupal.visitor.chapter_referral_id from javascript setting
	if (typeof Drupal.settings.chapter_referral_id !== 'undefined') {

		var chapter_referral_id = null;
		if (typeof Drupal.settings.chapter_referral_id.id !== 'undefined') {
			chapter_referral_id = Drupal.settings.chapter_referral_id.id;
			var cookie = ['Drupal.visitor.chapter_referral_id=', chapter_referral_id, '; domain=.', window.location.host.toString(), '; path=/;'].join('');
	  		document.cookie = cookie;
		}
	}

});})(jQuery, Drupal, this, this.document);
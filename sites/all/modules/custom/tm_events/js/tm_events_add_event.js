/* Add edit methods */
var checkLastEdit = null;
var check_last_edited_notified = false;
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	getCookie = function(cname) {
	    var name = cname + "=";
	    var ca = document.cookie.split(';');
	    for(var i = 0; i <ca.length; i++) {
	        var c = ca[i];
	        while (c.charAt(0)==' ') {
	            c = c.substring(1);
	        }
	        if (c.indexOf(name) == 0) {
	            return c.substring(name.length,c.length);
	        }
	    }
	    return "";
	}

	checkLastEdit = function() {

		var last_edited_event = getCookie("Drupal.visitor.last_edited_event");
		if ((last_edited_event != "") && (!check_last_edited_notified)) {

			check_last_edited_notified = true; // prevent showing multiple times

			setTimeout(function() {
				confirm_title = "Edit event";
				message = "Would you like to edit the event you just created?";
				$.prompt(message, { buttons: { "Yes, edit my event": true, "Close": false },
					title: confirm_title,
					submit: function(e,v,m,f){
						if (v == true) {
							window.location = "/node/" + last_edited_event + "/edit";
						}
					}
				});
			}, 50);
		}
	}

	// Chrome, IE will load this on back
	checkLastEdit();

	// Force Firefox to load js on page back
	// http://stackoverflow.com/questions/2638292/after-travelling-back-in-firefox-history-javascript-wont-run
	window.onload = function(){};
	window.onunload = function(){};

});})(jQuery, Drupal, this, this.document);

// Detect back in Safari
// http://stackoverflow.com/questions/11979156/mobile-safari-back-button
window.onpageshow = function(event) {
    if (event.persisted) {
        checkLastEdit();
    }
};
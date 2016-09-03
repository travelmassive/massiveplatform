/* Add edit methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

	function getCookie(cname) {
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

	var last_edited_event = getCookie("Drupal.visitor.last_edited_event");
	if (last_edited_event != "") {
		
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

});})(jQuery, Drupal, this, this.document);
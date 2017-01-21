(function($) {

	jQuery(document).ready(function () { 

		// hide email, password and change password fields
		jQuery(".form-item-mail").hide();
		jQuery(".form-item-current-pass").hide();
		jQuery(".form-type-password-confirm").hide();

		// position edit links
		if (jQuery("#field-user-last-name-add-more-wrapper").length) { 
			jQuery(".user_edit_change_password").detach().appendTo('#field-user-last-name-add-more-wrapper');
			jQuery(".user_edit_change_email").detach().appendTo('#field-user-last-name-add-more-wrapper');
		} else {
			jQuery(".user_edit_change_password").detach().appendTo('#edit-field-user-last-name');
			jQuery(".user_edit_change_email").detach().appendTo('#edit-field-user-last-name');
		}

		// change timezone heading
		jQuery("#edit-timezone .fieldset-legend").text("Location settings");

	});

	// show email, password
	jq_show_edit_change_email = function() {
		jQuery(".form-item-mail").slideDown();
		jQuery(".form-item-current-pass").slideDown();
		jQuery(".form-type-password-confirm").hide();
	}

	// show password and change password fields
	jq_show_edit_change_password = function() {
		jQuery(".form-item-mail").hide();
		jQuery(".form-item-current-pass").slideDown();
		jQuery(".form-type-password-confirm").slideDown();
	}

})(jQuery);

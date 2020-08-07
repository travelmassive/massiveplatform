(function($) {

	jQuery(document).ready(function () { 

		function tm_add_field_link(item, link_label, placeholder, description) {
			
			try {

				// check item exists
				if (jQuery(item).length == 0) {
					return;
				}

				// set description
				if (description != "") {
					jQuery(item + " .description").html(description);
				}
				if (description == "blank") {
					jQuery(item + " .description").html("");
				}

				// don't add link if field has value
				var input_val = jQuery(item + " input").val();
				if ((input_val != null) && (input_val != "")) {
					return;
				}

				// don't add link if field has value
				var textarea_val = jQuery(item + " textarea").val();
				if ((textarea_val != null) && (textarea_val != "")) {
					return;
				}

				// don't add link if select has value
				var select_val = jQuery(item + " select").val();
				if ((select_val != null) && ((select_val != "_none") && (select_val != ""))) {
					return;
				}

				// set placeholder text
				jQuery(item + " input").attr("placeholder", placeholder);

				// hide input fields and description
				jQuery(item + " input").hide();
				jQuery(item + " select").attr('data-foo', '123');
				var select_container = '<div class="tm-add-field-select-container"></div>';
				jQuery(item).append(select_container);
				jQuery(item + " select").detach().appendTo(item + " .tm-add-field-select-container");
				jQuery(item + " .tm-add-field-select-container").hide();
				jQuery(item + " .resizable-textarea").hide();
				jQuery(item + " .description").hide();

				// add link label to data attribute
				jQuery(item).attr('data-link-label', link_label);

				// add link
				jQuery(item).append(function() {
					var link_label = jQuery(this).attr('data-link-label');
					var link = '<a class="tm-add-field-label"><span class="tm-add-field-add-sign"></span>' + link_label + '</a>';
					return jQuery(link).click(function() {

						// show input fields and description
						jQuery(this).closest(".form-wrapper").find("input").show();
						jQuery(this).closest(".form-type-select").find(".tm-add-field-select-container").show();
						jQuery(this).closest(".form-wrapper").find(".resizable-textarea").show();
						jQuery(this).closest(".form-wrapper").find(".description").show();

						// show hide link
						jQuery(this).closest(".form-wrapper").find(".tm-hide-field-label").show();

						// hide link
						jQuery(this).hide();
					});
				});

				// add hide link
				jQuery(item + " label").append(function() {
					var link_label = jQuery(this).attr('data-link-label');
					var link = ' <a class="tm-hide-field-label" style="display:none;"><span class="tm-add-field-close-sign"></span></a>';
					return jQuery(link).click(function() {

						// hide input fields and description
						jQuery(this).closest(".form-wrapper").find("input").hide();
						jQuery(this).closest(".form-type-select").find(".tm-add-field-select-container").hide();
						jQuery(this).closest(".form-wrapper").find(".resizable-textarea").hide();
						jQuery(this).closest(".form-wrapper").find(".description").hide();

						// show add link
						jQuery(this).closest(".form-wrapper").find(".tm-add-field-label").show();

						// hide link
						jQuery(this).hide();
					});
				});

			}

			catch(err) {
				return;
			}
		}

		// About me fields
		tm_add_field_link(".form-item-field-company-cover-video-und-0-value", "Add YouTube video", "YouTube Video URL", "");
		tm_add_field_link(".form-item-field-friendly-url-und-0-value", "Claim Your URL", "example: your-company", "Create a custom URL that will let people quickly visit your page and connect with you.");

		// COVID-19 response
		tm_add_field_link(".form-item-field-company-covid19-message-und-0-value", "Add response", "", "");

		// Website links
		tm_add_field_link("#edit-field-link-website", "Add link", "https://www.mywebsite.com", "A link to your website");
		tm_add_field_link("#edit-field-link-twitter", "Add link", "@yourtwitter", "Your Twitter handle, or a link to your Twitter profile");
		tm_add_field_link("#edit-field-link-linkedin", "Add link", "https://www.linkedin.com/company/your-company-name", "A link to your LinkedIn page.<p><strong>How to find your LinkedIn URL</strong><br>1. Go to <a style='text-decoration: underline;' target='_blank' href='https://www.linkedin.com'>linkedin.com</a> and log in to your LinkedIn account.<br> 2. Go to your organization's page <br>3. Click \"View as member\" at the top right of your page.<br>4. Once your page loads, look at your browser's URL bar. The URL there is your LinkedIn URL.</p>");
		tm_add_field_link("#edit-field-link-facebook", "Add link", "https://www.facebook.com/your-company-name", "A link to your Facebook page.<p><strong>How to find your Facebook page</strong><br>1. Go to <a style='text-decoration: underline;' target='_blank' href='https://www.facebook.com'>facebook.com</a> and log in to your Facebook account.<br> 2. Go to your Facebook page.<br>3. Once your page loads, look at your browser's URL bar. The URL there is your Facebook URL.</p>");
		tm_add_field_link("#edit-field-link-instagram", "Add link", "@yourinstagram", "Your Instagram handle, or a link to your Instagram page");
		tm_add_field_link("#edit-field-link-youtube", "Add link", "https://youtube.com/user/yourname", "A link to your YouTube channel");
		tm_add_field_link("#edit-field-link-vimeo", "Add link", "https://vimeo.com/yourname", "A link to your Vimeo page");
		tm_add_field_link("#edit-field-link-snapchat", "Add link", "Your Snapchat username", "blank");
		tm_add_field_link("#edit-field-link-strava", "Add link", "A link to your Strava profile", "blank");
		tm_add_field_link("#edit-field-link-tiktok", "Add link", "Your TikTok username", "blank");

	});

})(jQuery);

(function($) {


  Drupal.behaviors.userEditPhotoButtons = {

    attach : function(context) {


    	// get js variables
      var organization_nid = Drupal.settings.tm_organizations.nid;
    	var has_cover_image = Drupal.settings.tm_organizations.has_cover_image;
  		var has_profile_image = Drupal.settings.tm_organizations.has_profile_image;

  		// set text of buttons
  		var change_cover_image_text = "Change cover";
    	var change_profile_image_text = "Change";

  		if (!has_cover_image) {
  			change_cover_image_text = "Add your own cover";
  		}

  		if (!has_profile_image) {
  			change_profile_image_text = "Add photo";
  		}
  		
  		// create buttons
    	$(".media .cover img").before("<button id='cover-image-edit-button' class='image-edit-button' onClick='clickEditCoverPhotoButton();' style='display: none; position: absolute;'> " + change_cover_image_text + "</button>");
    	$(".avatar .badge-organization img").before("<button id='profile-image-edit-button' class='image-edit-button' onClick='clickEditProfilePhotoButton();' style='display: none; position: absolute;'> " + change_profile_image_text + "</button>");

    	// cover image
    	if (has_cover_image) {
    		$(".media .cover img, #cover-image-edit-button").hover(
				function() {
					positionCoverPhotoButton();
					$("#cover-image-edit-button").show();
				},
				function() {
					$("#cover-image-edit-button").hide();
				}
    		);
    	}
    	else {
    		$("#cover-image-edit-button").show();
    	}

    	// profile image
    	if (has_profile_image) {
    		$(".avatar .badge-organization img, #profile-image-edit-button").hover(
				function() {
					positionProfilePhotoButton();
					$("#profile-image-edit-button").show();
				},
				function() {
					$(".avatar .badge-organization").addClass("zoomable");
					$("#profile-image-edit-button").hide();
				}
    		);
    	} else {
    		$("#profile-image-edit-button").show();
    	}

    	positionCoverPhotoButton = function() {
    		var cover_height = $(".media .cover").height();
			$("#cover-image-edit-button").css({top: cover_height - 32, right: 8});
    	}

    	positionProfilePhotoButton = function() {
    		var profile_image_height = $(".avatar .badge-organization").height();
			$("#profile-image-edit-button").css({top: profile_image_height - 16, right: 8});
    	}

    	clickEditCoverPhotoButton = function() {
    		var current_user_uid = $("#current_user_uid").val();
    		document.location = '/node/' + organization_nid + '/edit';
    	}

    	clickEditProfilePhotoButton = function() {
    		var current_user_uid = $("#current_user_uid").val();
    		$(".avatar .badge-organization").removeClass("zoomable");
    		document.location = '/node/' + organization_nid + '/edit';
    	}

    	repositionPhotoButtons = function() {
    		setTimeout(function(){
          		positionCoverPhotoButton();
    			positionProfilePhotoButton();
        	}, 50);	
    	}
    	window.addEventListener('resize', repositionPhotoButtons, false);

    	// on load
    	repositionPhotoButtons();
    }

  };
})(jQuery);

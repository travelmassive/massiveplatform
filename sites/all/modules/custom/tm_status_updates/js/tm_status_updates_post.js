(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

	// STATUS UPDATE POST METHODS
	// actions: create

	var tm_update_status_preview_timer = null; // keep track of preview timer
	var tm_update_status_preview_stack = Array(); // keep track of preview calls
	var tm_update_status_preview_wait = 800; // time to wait after entering url to show preview
	var tm_update_status_last_preview_url = null;
	var tm_update_status_posted = false;
	var tm_update_preview_link_id = null;
	var tm_update_location_granted = false;
	var tm_update_location_text = null;
	var tm_update_location_latitude = null;
	var tm_update_location_longitude = null;
	var tm_update_refesh_feed = true; // whether to reload or refresh newsfeed  

	// event handlers
	$("#tm-status-update-post-as").change(function () {
	  var selected = $('#tm-status-update-post-as').val();
	  var post_as_image = tm_update_post_as_images[selected];
	  $('#tm-status-update-poster-image').attr("src", post_as_image);
	});

	// it all starts here
	tm_user_status_update_main = function() {
	  // preload images
	  tm_user_status_update_preload_images();
	  //tm_user_status_update_show_feedback("init", false);
	}

	// preload post as images
	tm_user_status_update_preload_images = function() {
	  if (typeof(tm_update_post_as_images) !== 'undefined') {
	    for (var key in tm_update_post_as_images) {
	      var imageObject = new Image();
	      imageObject.src = tm_update_post_as_images[key];
	    }
	  }
	}

	// handle key press in status update
	tm_user_status_update_on_key_press = function(val, limit) {

	  // count characters entered
	  tm_countChar(val, limit);

	  // search for submitted urls
	  var urls = tm_findUrls(val.value);
	  if (urls.length > 0) {
	    clearTimeout(tm_update_status_preview_timer);
	    tm_update_status_preview_timer = setTimeout(tm_user_status_update_preview_link, tm_update_status_preview_wait);
	  }
	}

	// abort all existing xhr queries user might have caused
	tm_user_status_updates_cancel_previews = function() {
		for(i = 0; i < tm_update_status_preview_stack.length; i++) {
			tm_update_status_preview_stack[i].abort();
		}
		tm_update_status_preview_stack = Array();
		clearTimeout(tm_update_status_preview_timer);
	}

	// preview a url entered in the status update
	tm_user_status_update_preview_link = function() {

	  // tm_user_status_update_hide_feedback();
	  clearTimeout(tm_update_status_preview_timer);
	  var urls = tm_findUrls(($("#tm-status-update-text").val()));
	  if (urls.length > 0) {
	    var preview_url = urls[urls.length - 1];
	    if (preview_url != tm_update_status_last_preview_url) {

	      // show message
	      tm_user_status_update_show_feedback("Fetching preview", true);

	      // fetch preview of URL
	      tm_user_status_update_fetch_preview_link(preview_url);
	    }

	    tm_update_status_last_preview_url = preview_url; // keep track of last url previewed      
	  }
	}

	// fetch a preview of a url
	tm_user_status_update_fetch_preview_link = function(preview_url) {
	  
	  // Step 1. Cancel all previous previews
	  tm_user_status_updates_cancel_previews();

	  // Step 2. Fetch preview
	  xhr = $.ajax({
	    type: 'POST',
	    url: '/user/' + tm_update_status_uid + '/status/preview_link',
	    data: {
	      'preview_url': preview_url,
	    },
	    success: function(data) {
	      if (data.result == true) {

	        if (typeof(data.preview_link_id) == 'undefined') {

	          tm_update_preview_link_id = null;
	          tm_user_status_update_show_feedback("No preview available", false);

	        } else {

	          // set preview id
	          tm_update_preview_link_id = data.preview_link_id;

	          // render preview
	          tm_user_status_update_show_feedback("Preview link", false);
	          $("#tm-status-update-link-preview-html").html(data.preview_html).show();

	        }
	      } else {
	      	tm_user_status_update_show_feedback("Link preview is not available.", false);
	      }
	    },
	    error: function(data) {
	      tm_user_status_update_show_feedback("Oops, there was a problem previewing your link.", false);
	    }
	  });

	  // Step 3. (note: this is async). Push xhr request to stack
	  tm_update_status_preview_stack.push(xhr);

	}

	// allow member to send a short message on follow to recipient
	tm_user_status_update = function() {
	  
	  // dont allow multiple posts
	  if (tm_update_status_posted) {
	    return;
	  }
	  if ($('#tm-status-update-text').val().trim() == "") {
	    $('#tm-status-update-text').attr("placeholder", "Write something...");
	    return;
	  }

	  // disable posting form
	  tm_user_status_posting_form();

	  // post the update
	  $.ajax({
	    type: 'POST',
	    url: '/user/' + tm_update_status_uid + '/status/update',
	    data: {
	      status_update_text: $('#tm-status-update-text').val(),
	      post_as: $('#tm-status-update-post-as').val(),
	      preview_link_id: tm_update_preview_link_id,
	      location_text: tm_update_location_text,
	      location_latitude: tm_update_location_latitude,
	      location_longitude: tm_update_location_longitude
	    },
	    success: function(data) {
	      if (data.result == true) {

	        // go to page
	        if (data.redirect != null) {
	          if (data.redirect == '/newsfeed') {
	            tm_user_status_reload_newsfeed();
	          } else {
	            window.location = data.redirect;
	          }
	        }

	      } else {
	      	tm_user_status_enable_form();
	        tm_user_status_update_show_feedback(data.error_message);
	      }
	    },
	    error: function(data) {
	    	tm_user_status_enable_form();
	    	tm_user_status_update_show_feedback("Oops, there was a problem updating your status");
	    }
	  });
	}

	// enable post form
	tm_user_status_enable_form = function() {
		tm_update_status_posted = false;
		$("#tm-status-update-post").html("Post");
		$("#tm-status-update-text").attr('readonly',false);
		
	}

	// disable post form
	tm_user_status_posting_form = function() {
		tm_update_status_posted = true;
		$("#tm-status-update-post").html("Posting...");
		$("#tm-status-update-text").attr('readonly',true);
		tm_user_status_update_show_feedback_preview("", true, false);
	}

	// reload newfeed
	tm_user_status_reload_newsfeed = function() {

		if (tm_update_refesh_feed) {

			// cancel any more previews
			tm_user_status_updates_cancel_previews();

			// enable post form
			$("#tm-status-update-text").val("");
			tm_user_status_enable_form();

			// hide preview and feedback
			tm_user_status_update_show_feedback_preview("", true, false);

			// reset page count and load feed
			tm_status_updates_fetch_feed(true);

		} else {

			location.reload();
		}
	}

	// hide feedback message
	tm_user_status_update_hide_feedback = function() {
		$("#tm-status-update-feedback-text").html("");
		$("#tm-status-update-feedback-loading-image").hide();	  
	}

	// show feedback message
	tm_user_status_update_show_feedback = function(feedback_text, show_loading_image) {
		tm_user_status_update_show_feedback_preview(feedback_text, show_loading_image, true);
	}

	// show feedback message
	tm_user_status_update_show_feedback_preview = function(feedback_text, show_loading_image, hide_preview) {

		$("#tm-status-update-feedback-text").html(feedback_text);
		if (show_loading_image == true) {
			$("#tm-status-update-feedback-loading-image").show();
		} else {
			$("#tm-status-update-feedback-loading-image").hide();
		}
		if (hide_preview) {
			// hide preview html
			$("#tm-status-update-link-preview-html").hide();
		}
		
	}

	// hide link preview
	tm_user_status_update_hide_preview = function() {
		$("#tm-status-update-link-preview-html").hide();
	}

	// clear preview
	tm_user_status_update_clear_preview = function() {
		tm_user_status_update_hide_feedback();
	  	$("#tm-status-update-link-preview-html").html("");
	}

	// show user location
	tm_user_status_location_show = function(location_text) {
	  if ((location_text == "") || (location_text == null)) {
	    tm_user_status_location_show("Add location"); // reset
	  } else {
	    $("#tm-status-update-location-text").text(location_text);
	    $("#tm-status-update-location-field").show();
	  }

	  // store last location text in cookie
	  tm_user_status_set_location_cookie(location_text);
	}

	// hide location
	tm_user_status_location_hide = function() {
	  $("#tm-status-update-location-field").hide();
	}

	// unable to access browser location
	tm_user_status_location_error = function() {

	  // disable geocoder
	  tm_status_updates_geocoder = null;
	  tm_update_location_granted = false;

	  // set from cookie so at least we have text
	  tm_update_location_text = tm_user_status_get_location_cookie();
	  tm_user_status_location_show(tm_update_location_text);
	}

	// lookup location
	tm_user_status_location_process = function(position) {

	  // try geocoder
	  if (tm_status_updates_geocoder == null) {
	    return;
	  }

	  var lat = position.coords.latitude;
	  var lng = position.coords.longitude;
	  var latlng = new google.maps.LatLng(lat, lng);

	  // albe to get access
	  tm_update_location_granted = true;

	  // set lat lon
	  tm_update_location_latitude = lat;
	  tm_update_location_longitude = lng;

	  // if user has previously set a location, use that instead
	  if (tm_user_status_get_location_cookie()) {
	    tm_update_location_text = tm_user_status_get_location_cookie()
	    tm_user_status_location_show(tm_update_location_text);
	    return;
	  }

	  // use google maps geocoder and search for nearest locality name
	  try {
	    tm_status_updates_geocoder.geocode({latLng: latlng}, function(results, status) {

	      if (status == google.maps.GeocoderStatus.OK) {

	      	var locality_found = false;

	      	// Method 1
	        if (results[0]) {
	          var arrAddress = results;
	          $.each(arrAddress, function(i, address_component) {
	            if (address_component.types[0] == "locality") {
	              var city_name = address_component.address_components[0].long_name;
	              tm_update_location_text = city_name;
	              tm_user_status_location_show(city_name);
	              locality_found = true;
	            }
	          });

	          // Method 2
	          if (!locality_found) {
	          	var firstAddress = results[0];
	          	$.each(firstAddress.address_components, function(i, address_components) {
	          	  if (address_components.types[0] == "locality") {
	          	    var city_name = address_components.long_name;
	          	    tm_update_location_text = city_name;
	          	    tm_user_status_location_show(city_name);
	          	    locality_found = true;
	          	  }
	          	});
	          }

	          // If we couldn't find a location from the geocoder, ask the user
	          if (!locality_found) {
	          	tm_user_status_location_show(null);
	          }

	        } else {
	          tm_user_status_location_hide();
	          return null;
	        }
	      } else {
	        tm_user_status_location_hide();
	        return null;
	      }
	    });
	  } catch (e) {
	    tm_user_status_location_hide();
	    return null;
	  }
	  
	}

	// request location from user
	tm_user_status_location_request_location = function() {

	  try {
	    if (navigator.geolocation) {
	      navigator.geolocation.getCurrentPosition(tm_user_status_location_process, tm_user_status_location_error);
	    }
	  } catch (e) {
	    // disable geocoder
	    tm_status_updates_geocoder = null;
	    tm_update_location_granted = false;
	  }

	}

	// remember location in cookie
	tm_user_status_set_location_cookie = function(value) {
	  var cookie = ['status_update_location=', value, '; domain=.', window.location.host.toString(), '; path=/;'].join('');
	  document.cookie = cookie;
	}

	// get location from cookie
	tm_user_status_get_location_cookie = function() {
	  var result = document.cookie.match(new RegExp('status_update_location=([^;]+)'));
	  if (result != null) {
	    var location_text = result[1];
	    return location_text;
	  }
	  return null;
	}

	// helper methods

	// character count
	tm_countChar = function(val, limit) {
	  var len = val.value.length;
	  if (len >= limit) {
	    val.value = val.value.substring(0, limit);
	  } else {
	    if (len == 0) {
	      $('#charNum').text(" ");
	    } else {
	      $('#charNum').text((limit - len) + " chars");
	    }
	  }
	};

	// find urls in text
	tm_findUrls = function(text)
	{
	    var source = (text || '').toString();
	    var urlArray = [];
	    var url;
	    var matchArray;

	    // http://stackoverflow.com/questions/3809401/what-is-a-good-regular-expression-to-match-a-url
	    var regexToken = /[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,6}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*)/g;

	    // Iterate through any URLs in the text.
	    while( (matchArray = regexToken.exec( source )) !== null )
	    {
	        var token = matchArray[0];

	        // don't include www or www. as people type them
	        if ((token.toLowerCase() != 'www') && (token.toLowerCase() != 'www.')) {
	        	urlArray.push( token );
			}
	          
	    }

	    return urlArray;
	}

	// get location from user
	tm_user_status_enter_location = function() {
	  
	  var message = "<input type='text' id='tm-status-update-location-input' placeholder='Enter your location...'>";
	  
	  $.prompt(message, { buttons: { "Save": true, "Cancel": false },
	    'title': 'Where are you?',
	    loaded: function() {
	      $('#tm-status-update-location-input').val(tm_update_location_text);
	    },
	    submit: function(e,v,m,f){
	      if (v == true) {
	        if ($('#tm-status-update-location-input').val().trim() != tm_update_location_text) {
	          tm_update_location_text = $('#tm-status-update-location-input').val().trim();
	          tm_user_status_location_show(tm_update_location_text);
	          tm_user_status_set_location_cookie(tm_update_location_text);
	        }
	      }
	    }
	  });
	}

	// run main loader method
	tm_user_status_update_main();

});})(jQuery, Drupal, this, this.document);

// handle google maps initialization
var tm_status_updates_geocoder = null;
function initGoogleMaps() {
	jQuery(document).ready(function() {
		tm_status_updates_geocoder = new google.maps.Geocoder();
		tm_user_status_location_request_location();
	});
}

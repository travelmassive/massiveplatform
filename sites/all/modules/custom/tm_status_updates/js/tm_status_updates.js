(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {


  var tm_update_status_preview_timer = null; // keep track of preview timer
  var tm_update_status_preview_wait = 1000; // time to wait after entering url to show preview
  var tm_update_status_last_preview_url = null;
  var tm_update_status_posted = false;

  // handle key press in status update
  tm_user_status_update_on_key_press = function(val, limit) {

    // count characters entered
    tm_countChar(val, limit);

    // search for submitted urls
    var urls = tm_findUrls(val.value);
    if (urls.length > 0) {
      clearTimeout(tm_update_status_preview_timer);
      tm_update_status_preview_timer = setTimeout(tm_user_status_update_preview_url, tm_update_status_preview_wait);
    }
  }

  // preview a url entered in the status update
  tm_user_status_update_preview_url = function() {

    clearTimeout(tm_update_status_preview_timer);
    var urls = tm_findUrls(($("#tm-status-update-text").val()));
    if (urls.length > 0) {
      var preview_url = urls[urls.length - 1];
      if (preview_url != tm_update_status_last_preview_url) {
        console.log("fetch a preview of last entered url " + preview_url);
      }
      tm_update_status_last_preview_url = preview_url; // keep track of last url previewed      
    }
  }

  // allow member to send a short message on follow to recipient
  tm_user_status_update = function(uid) {
    
    // dont allow multiple posts
    if (tm_update_status_posted) {
      return;
    }
    if ($('#tm-status-update-text').val().trim() == "") {
      $('#tm-status-update-text').attr("placeholder", "Write something...");
      return;
    }
    tm_update_status_posted = true;

    // let user know we're posting
    $("#tm-status-update-post").html("Posting...");

    // post the update
    $.ajax({
      type: 'POST',
      url: '/user/' + uid + '/update_status',
      data: {
        update_status: $('#tm-status-update-text').val(),
        post_as: $('#tm-status-update-post-as').val(),
        action: "create" // create, edit, delete, or moderate
      },
      success: function(data) {
        if (data.result == true) {

          // go to page
          if (data.redirect != null) {
            if (data.redirect == '/newsfeed') {
              location.reload();
            } else {
              window.location = data.redirect;
            }
          }

        } else {
          $(".tm-status-update-error-message").show().html('Oops, there was a problem updating your status.'); 
        }
      },
      error: function(data) {
        $(".tm-status-update-error-message").show().html('Oops, there was a problem updating your status.'); 
      }
    });
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
            urlArray.push( token );        
      }

      return urlArray;
  }

});})(jQuery, Drupal, this, this.document);

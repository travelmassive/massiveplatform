(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

  // STATUS UPDATE ACTION METHODS
  // actions: edit, moderate, delete, featured

  var tm_status_updates_loader_page = 1;

  tm_user_status_apply_event_handlers = function() {
   
  }

  // confirm with user to moderate status
  tm_user_status_edit_form = function(status_update_id) {
    
    var title = 'Edit post';
    var message = '<div id="tm-status-update-edit-placeholder">Loading...</div>';

    $.prompt(message, { buttons: { "Save changes": true, "Cancel": false },
      'title': title,
      loaded: function() {
        // load update form
        $.ajax({
          type: 'POST',
          url: '/user/' + tm_update_status_uid + '/status/edit_form',
          data: {
            'status_update_id': status_update_id,
          },
          success: function(data) {
            $("#tm-status-update-edit-placeholder").html(data); 
          },
          error: function(data) {
            $("#tm-status-update-edit-placeholder").html("There was an error fetching your update");
          }
        });
      },
      submit: function(e,v,m,f){
        if (v == true) {
          var status_update_text = $("#edit_status_update_text").val();
          tm_user_status_edit_save_changes(status_update_id, status_update_text);
        }
      }
    });
  }

  // remove status update
  tm_user_status_edit_save_changes = function(status_update_id, status_update_text) {
    
    // let user know we're moderating
    // $("#tm-status-update-post").html("Saving...");

    // moderate the update
    $.ajax({
      type: 'POST',
      url: '/user/' + tm_update_status_uid + '/status/edit',
      data: {
        'status_update_id': status_update_id,
        'status_update_text': status_update_text
      },
      success: function(data) {
        if (data.result == true) {
          location.reload();
        } else {
          jq_alert(null, data.error_message); 
        }
      },
      error: function(data) {
        jq_alert(null, 'Oops, there was a problem saving your changes.'); 
      }
    });
  }

  // confirm with user to remove status update
  tm_user_status_remove_confirm = function(status_update_id) {
    
    var message = "Are you sure you want to delete this update?";
    $.prompt(message, { buttons: { "Yes": true, "Cancel": false },
      'title': null,
      submit: function(e,v,m,f){
        if (v == true) {
          tm_user_status_remove(status_update_id);
        }
      }
    });
  }

  // remove status update
  tm_user_status_remove = function(status_update_id) {
    
    // let user know we're posting
    // $("#tm-status-update-post").html("Removing...");

    // remove the update
    $.ajax({
      type: 'POST',
      url: '/user/' + tm_update_status_uid + '/status/remove',
      data: {
        'status_update_id': status_update_id
      },
      success: function(data) {
        if (data.result == true) {
          location.reload();
        } else {
          jq_alert(null, 'Oops, there was a problem removing your status update.'); 
        }
      },
      error: function(data) {
        jq_alert(null, 'Oops, there was a problem remove your status update.'); 
      }
    });
  }

  // promote status update
  tm_user_status_promote = function(status_update_id, is_promoted) {
    
    // let user know we're posting
    // $("#tm-status-update-post").html("Removing...");

    if (is_promoted) {
      var action_text = 'promoted';
      var post_url = '/user/' + tm_update_status_uid + '/status/promote';
    } else {
      var action_text = 'un-promoted';
      var post_url = '/user/' + tm_update_status_uid + '/status/unpromote';
    }

    // remove the update
    $.ajax({
      type: 'POST',
      url: post_url,
      data: {
        'status_update_id': status_update_id
      },
      success: function(data) {
        if (data.result == true) {
          location.reload();
        } else {
          jq_alert(null, 'Oops, there was a problem ' + action_text + ' the status update.'); 
        }
      },
      error: function(data) {
        jq_alert(null, 'Oops, there was a problem ' + action_text + ' status update.'); 
      }
    });
  }

  // confirm with user to moderate status
  tm_user_status_moderate_confirm = function(status_update_id) {
    
    var title = 'Moderate this post';
    var message = "Provide a reason why you are moderating this post: <textarea id='moderator_note' value='' placeholder='' rows='3' cols='50' placeholder='Posting spam...'></textarea>";

    $.prompt(message, { buttons: { "Moderate and hide post": true, "Cancel": false },
      'title': title,
      submit: function(e,v,m,f){
        if (v == true) {
          var moderator_note = $("#moderator_note").val();
          tm_user_status_moderate(status_update_id, moderator_note);
        }
      }
    });
  }

  // confirm with user to promote status update
  tm_user_status_promote_confirm = function(status_update_id) {
    
    var message = "Are you sure you want to promote this update?";
    $.prompt(message, { buttons: { "Yes": true, "Cancel": false },
      'title': null,
      submit: function(e,v,m,f){
        if (v == true) {
          tm_user_status_promote(status_update_id, true);
        }
      }
    });
  }

  // confirm with user to unpromote status update
  tm_user_status_unpromote_confirm = function(status_update_id) {
    
    var message = "Are you sure you want to unpromote this update?";
    $.prompt(message, { buttons: { "Yes": true, "Cancel": false },
      'title': null,
      submit: function(e,v,m,f){
        if (v == true) {
          tm_user_status_promote(status_update_id, false);
        }
      }
    });
  }

  // remove status update
  tm_user_status_moderate = function(status_update_id, moderator_note) {
    
    // let user know we're moderating
    // $("#tm-status-update-post").html("Moderating...");

    // moderate the update
    $.ajax({
      type: 'POST',
      url: '/user/' + tm_update_status_uid + '/status/moderate',
      data: {
        'status_update_id': status_update_id,
        'moderator_note': moderator_note
      },
      success: function(data) {
        if (data.result == true) {
          location.reload();
        } else {
          jq_alert(null, 'Oops, there was a problem moderating the status update.'); 
        }
      },
      error: function(data) {
        jq_alert(null, 'Oops, there was a problem moderating the status update.'); 
      }
    });
  }

  // ajax loader
  tm_status_updates_load_more = function(feed_type, display_from, display_to) {

    // ui
    //$(".tm-status-updates-pager").addClass("previous-pager");
    $(".tm-status-update-pager-link").text("Loading more...");
    $(".tm-status-update-pager-loading-image").show();

    var limit_from = tm_status_updates_loader_page * tm_update_status_items_per_load; 
    var limit_to = limit_from + tm_update_status_items_per_load;
    tm_status_updates_loader_page++;

    // load newsfeed
    $.ajax({
      type: 'GET',
      url: '/newsfeed/load',
      data: {
        'feed_type': feed_type,
        'limit_from': limit_from,
        'limit_to': limit_to
      },
      success: function(data) {

        // add data
        $(".status-update-list").append(data);

        // add dropdown handlers
        tm_status_update_init_dropdown_handlers();

        // update pager
        $(".tm-status-update-pager-link").text("Load more");
        $(".tm-status-update-pager-loading-image").hide();
        
      },
      error: function(data) {
        $(".previous-pager").html("Oops, a problem occured");
      }
    });
  }

  // remove the ajax loader
  tm_status_update_remove_loader = function() {
    $('.tm-status-updates-loader').remove();
  }

  // initialise dropdown handlers in update-list
  tm_status_update_init_dropdown_handlers = function() {

    $('.status-update-list [data-dropd-toggle]').unbind();

    $('.status-update-list [data-dropd-toggle]').click(function(e) {

      e.preventDefault();
      e.stopPropagation();
      var _self = $(this);
      var $drop = _self.closest('[data-dropd-wrapper]').find('[data-dropd]');

      // Hide others.
      $('.status-update-list [data-dropd-toggle]').not(_self).removeClass('on');
      $('.status-update-list [data-dropd]').not($drop).removeClass('on');

      // Set top value.
      $drop.css('top', _self.height());

      if (_self.hasClass('on')) {
        _self.removeClass('on');
        $drop.removeClass('on');
      }
      else {
        _self.addClass('on');
        $drop.addClass('on');
      }
    });

  }

});})(jQuery, Drupal, this, this.document);


/* List action menu methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // position item in list
  tm_lists_position_item = function(entity_type, entity_id, list_id, name, num_list_items, current_position) {
    
    name = decodeURIComponent(name);  

    var message = "Move <b>" + name + "</b> to position <select id='tm-lists-select-position'></select> of list?";
    $.prompt(message, { 
      buttons: { "Update position": true, "Cancel": false },
      title: null,
      loaded: function() {
        var html = ''; // '<select id="tm-lists-select-position">';
        for(i = 1; i <= num_list_items; i++) {
          if (i == current_position) {
            html += "<option value='"+i+"' selected disabled>"+i+"</option>";
          } else {
            html += "<option value='"+i+"'>"+i+"</option>";
          }
        }
        //html += '</select>';
        document.getElementById('tm-lists-select-position').innerHTML= html;
      },
      submit: function(e,v,m,f){
        if (v == true) {
          var position = $('#tm-lists-select-position').find(":selected").val();
          window.location = '/lists/position/' + entity_type + '/' + entity_id + '/' + list_id + '/' + position;
        }
      }
    });

  }

  // remove from list
  tm_remove_item_from_list = function(entity_type, entity_id, list_id, name) {
    
    name = decodeURIComponent(name);  

    var message = "Remove <b>" + name + "</b> from this list?";
    $.prompt(message, { 
      buttons: { "Remove from list": true, "Cancel": false },
      title: null,
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/lists/remove/' + entity_type + '/' + entity_id + '/' + list_id + "?redirect_to_list";
        }
      }
    });

  }

  // confirm with user to moderate status
  tm_lists_edit_form = function(entity_type, entity_id, list_id, character_limit) {
    
    var char_limit = character_limit;
    countChar = function(val) {
      var len = val.value.length;
      if (len >= char_limit) {
        val.value = val.value.substring(0, char_limit);
      } else {
        $('#charNum').text((char_limit - len) + " chars");
      }
    };

    var title = 'Edit description';
    var message = '<div id="tm-list-item-edit-placeholder">Loading...</div><div style="width: 100%; text-align: right;"><span id="charNum" style="font-size: 10pt; color: #888;">' + char_limit + ' chars</span></div>';

    $.prompt(message, { buttons: { "Save changes": true, "Cancel": false },
      'title': title,
      loaded: function() {
        // load update form
        $.ajax({
          type: 'GET',
          url: '/lists/edit_form/' + entity_type + '/' + entity_id + '/' + list_id,
          success: function(data) {
            $("#tm-list-item-edit-placeholder").html(data);
            countChar(document.getElementById("edit_list_item_comment"));
          },
          error: function(data) {
            $("#tm-list-item-edit-placeholder").html("There was an error fetching the list item");
          }
        });
      },
      submit: function(e,v,m,f){
        if (v == true) {
          var comment_text = $("#edit_list_item_comment").val();
          tm_lists_edit_save_changes(entity_type, entity_id, list_id, comment_text);
        }
      }
    });
  }

  // remove status update
  tm_lists_edit_save_changes = function(entity_type, entity_id, list_id, comment_text) {
    
    // save the changes
    $.ajax({
      type: 'POST',
      url: '/lists/edit/' + entity_type + '/' + entity_id + '/' + list_id,
      data: {
        'comment': comment_text
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

  // sent notifications
  tm_lists_send_unsent_notifications = function(list_id, count) {

    var message = "Send notifications for " + count + " items (added in Preview Mode).";
    $.prompt(message, { 
      buttons: { "Send Notifications": true, "Cancel": false },
      title: null,
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/lists/send_unsent_notifications/' + list_id;
        }
      }
    });

  }

});})(jQuery, Drupal, this, this.document);
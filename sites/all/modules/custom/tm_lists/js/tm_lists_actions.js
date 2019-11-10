/* List action menu methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // boost list
  tm_lists_boost_item = function(entity_type, entity_id, list_id, name) {
    
    name = decodeURIComponent(name);  

    var message = "Move <b>" + name + "</b> to top of this list?";
    $.prompt(message, { 
      buttons: { "Move to top": true, "Cancel": false },
      title: null,
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/lists/boost/' + entity_type + '/' + entity_id + '/' + list_id;
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
  tm_lists_edit_form = function(entity_type, entity_id, list_id) {
    
    countChar = function(val) {
      var len = val.value.length;
      if (len >= 250) {
        val.value = val.value.substring(0, 250);
      } else {
        $('#charNum').text((250 - len) + " chars");
      }
    };

    var title = 'Edit description';
    var message = '<div id="tm-list-item-edit-placeholder">Loading...</div><div style="width: 100%; text-align: right;"><span id="charNum" style="font-size: 10pt; color: #888;">250 chars</span></div>';

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

});})(jQuery, Drupal, this, this.document);
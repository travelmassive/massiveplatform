/* List moderation methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // allow member to add to a list
  tm_add_to_list = function(entity_type, entity_id, name) {
    
    countChar = function(val) {
      var len = val.value.length;
      if (len >= 250) {
        val.value = val.value.substring(0, 250);
      } else {
        $('#charNum').text((250 - len) + " chars");
      }
    };

    name = decodeURIComponent(name);  

    var message = "Select list &mdash; <div id='moderation_list' style='display: inline-block'>Loading...</div>";
    message = message + "<p>Description <textarea id='form_moderator_comment' onkeyup='countChar(this);' value='' placeholder='Add a short description to be displayed on the list...' rows='4' cols='50'></textarea><div style='margin-top: -16px; float: right;'><span id='charNum' style='font-size: 10pt; color: #888;'>250 chars</span></div></p>",

    $.prompt(message, { 
      buttons: { "Add to list": true, "Cancel": false },
      title: "Add " + name + " to list",
      loaded: function() {
        // load available chapters list
        $("#moderation_list").load("/lists/moderation-list-ajax/" + entity_type + "/" + entity_id + "/add");
      },
      submit: function(e,v,m,f){
        if (v == true) {
          var comment_text = $("#form_moderator_comment").val();
          comment_text = comment_text.replace(/(?:\r\n|\r|\n)/g, '__NEWLINE__');
          comment_text = encodeURIComponent(comment_text);
          window.location = '/lists/add/' + entity_type + '/' + entity_id + '/' + $("#moderation_list_ids option:selected").val() + '?comment=' + comment_text;
        }
      }
    });

  }

  // allow member to remove from a list
  tm_remove_from_list = function(entity_type, entity_id, name) {
    
    name = decodeURIComponent(name);  

    var message = "Select list &mdash; <div id='moderation_list' style='display: inline-block'>Loading...</div>";
    message = message + "<p></p>";

    $.prompt(message, { 
      buttons: { "Remove from list": true, "Cancel": false },
      title: "Remove " + name + " from list",
      loaded: function() {
        // load available chapters list
        $("#moderation_list").load("/lists/moderation-list-ajax/" + entity_type + "/" + entity_id + "/remove");
      },
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/lists/remove/' + entity_type + '/' + entity_id + '/' + $("#moderation_list_ids option:selected").val();
        }
      }
    });

  }

});})(jQuery, Drupal, this, this.document);
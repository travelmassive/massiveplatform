/* Chapter moderation methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // allow chapter leader to add a member to their chapter
  jq_add_member_to_chapter = function(uid, name) {
    
    countChar = function(val) {
      var len = val.value.length;
      if (len >= 150) {
        val.value = val.value.substring(0, 150);
      } else {
        $('#charNum').text((150 - len) + " chars");
      }
    };

    name = decodeURIComponent(name);  

    var message = "Select chapter &mdash; <div id='moderation_chapters_list' style='display: inline-block'>Loading...</div>";
    message = message + "<p>You can add a short message. <textarea id='form_moderator_message' onkeyup='countChar(this);' value='' placeholder='Welcome to our chapter...' rows='2' cols='50'></textarea><div style='margin-top: -16px; float: right;'><span id='charNum' style='font-size: 10pt; color: #888;'>150 chars</span></div></p>",

    $.prompt(message, { 
      buttons: { "Add to chapter": true, "Cancel": false },
      title: "Add " + name + " to chapter",
      loaded: function() {
        // load available chapters list
        $("#moderation_chapters_list").load("/chapters/moderation-chapter-list-ajax/" + uid + "/add");
      },
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/chapters/moderation-chapter-add-member/' + uid + '/' + $("#moderation_chapter_ids option:selected").val() + '?message=' + $("#form_moderator_message").val();
        }
      }
    });

  }

  // allow chapter leader to remove member from their chapter
  jq_remove_member_to_chapter = function(uid, name) {
    
    name = decodeURIComponent(name);  

    var message = "Select chapter &mdash; <div id='moderation_chapters_list' style='display: inline-block'>Loading...</div>";
    message = message + "<p>The member will not be able to join the chapter or register for new chapter events.</p>";

    $.prompt(message, { 
      buttons: { "Remove from chapter": true, "Cancel": false },
      title: "Remove " + name + " from chapter",
      loaded: function() {
        // load available chapters list
        $("#moderation_chapters_list").load("/chapters/moderation-chapter-list-ajax/" + uid + "/remove");
      },
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/chapters/moderation-chapter-remove-member/' + uid + '/' + $("#moderation_chapter_ids option:selected").val();
        }
      }
    });

  }

});})(jQuery, Drupal, this, this.document);
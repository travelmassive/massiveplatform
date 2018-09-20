/* Chapter moderation methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // allow chapter leader to add a member to their event
  jq_register_member_to_event = function(uid, name) {
    
    countChar = function(val) {
      var len = val.value.length;
      if (len >= 150) {
        val.value = val.value.substring(0, 150);
      } else {
        $('#charNum').text((150 - len) + " chars");
      }
    };

    name = decodeURIComponent(name);  

    var message = "Select event &mdash; <div id='moderation_events_list' style='display: inline-block'>Loading...</div>";
    message = message + "<p>You can add a short message. <textarea id='form_moderator_message' onkeyup='countChar(this);' value='' placeholder='Registering you to our event...' rows='2' cols='50'></textarea><div style='margin-top: -16px; float: right;'><span id='charNum' style='font-size: 10pt; color: #888;'>150 chars</span></div></p>",

    $.prompt(message, { 
      buttons: { "Register to event": true, "Cancel": false },
      title: "Register " + name + " to event",
      loaded: function() {
        // load available events list
        $("#moderation_events_list").load("/events/moderation-event-list-ajax/" + uid + "/register");
      },
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/events/moderation-event-register-member/' + uid + '/' + $("#moderation_event_ids option:selected").val() + '?message=' + $("#form_moderator_message").val();
        }
      }
    });

  }

});})(jQuery, Drupal, this, this.document);
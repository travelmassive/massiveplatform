(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

  // allow member to send a short message on follow to recipient
  jq_user_status_update = function(uid) {
    
    countChar = function(val) {
      var len = val.value.length;
      if (len >= 150) {
        val.value = val.value.substring(0, 150);
      } else {
        $('#charNum').text((150 - len) + " chars");
      }
    };

    randomStatusUpdate = function() {
      var status_updates = ["What have you recently achieved?", "Where are you off to next?", "Share an update with the community"];
      return status_updates[Math.floor(Math.random()*status_updates.length)];
    }

    $.prompt({
      state0: {
        //title: 'Send a follow message',
        html: "Send an update to your followers <br><textarea id='form_update_status' onkeyup='countChar(this);' value='' placeholder='" + randomStatusUpdate() + "' rows='2' cols='50'></textarea><div style='float: right;'><span id='charNum' style='font-size: 10pt; color: #888;'>150 chars</span></div><br>",
        buttons: { Cancel: -1, "Update Status": true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {

            // post the update
            $.ajax({
              type: 'POST',
              url: '/user/' + uid + '/update_status',
              data: {update_status: document.getElementById('form_update_status').value},
              success: function(data) {
                if (data.result == true) {
                  $('[name="jqi_state0_buttonPleasewait"]').html("Close");
                  $(".lead.jqititle.undefined").html("Updated sent");
                  $("#message_sending_prompt").html('<span style="color: #555">&#x2714</span> <strong>Update posted</strong>');
                } else {
                  $("#message_sending_prompt").html('Oops, there was a problem updating your status<br><br>Reason: ' + data.error_message); 
                  $('[name="jqi_state0_buttonPleasewait"]').html("Close");
                }
                  
              },
              error: function(data) {
                $("#message_sending_prompt").html('Oops, there was a problem updating your status.'); 
                $('[name="jqi_state0_buttonPleasewait"]').html("Close");
              }
            });

          } 
          //$.prompt.close();
        }
      }
    });
  }

});})(jQuery, Drupal, this, this.document);

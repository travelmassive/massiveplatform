(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

  jq_connection_email = function(sender_uid, recipient_uid, recipient_name) {

    var connection_message = '';
    var connection_message_preview_style = 'max-height: 250px; overflow-y: scroll; color: #20597c; padding: 4px; background-color: #fff; border: 1px solid #888; border-radius: 2px; font-size: 10pt; line-height: 1.2em;';

    var statesdemo = {
    state0: {
      title: 'Send a message to ' + decodeURIComponent(recipient_name),
      html: "<textarea id='form_connection_message' value='' placeholder='Hi...' rows='5' cols='50'></textarea><p class='connection_message_note'>Note: Links to websites will be removed.</p>",
      buttons: { Cancel: false, Preview: true },
      focus: 1,
      submit:function(e,v,m,f){
        if(v){
          e.preventDefault();
          connection_message = $.prompt.getState('state0').find('#form_connection_message').val();
          connection_message_html = connection_message.replace(/<(?:.|\n)*?>/gm, ''); // strip html
          connection_message_html = connection_message_html.replace(/(?:\r\n|\r|\n)/g, '<br />'); // add breaks
          
          // check we have a message
          if (connection_message_html.length < 10) {
            $('.connection_message_note').html("Your message is too short");
            $.prompt.goToState('state0');
            return false;
          }
          if (connection_message_html.length > 800) {
            $('.connection_message_note').html("Your message is too long, make it simpler.");
            $.prompt.goToState('state0');
            return false;
          }
          $.prompt.getState('state1').find('#preview_connection_message').html(connection_message_html); 
          $.prompt.goToState('state1');
          return false;
        }
        $.prompt.close();
      }
    },
    state1: {
      title: 'Preview message',
      html: "<div id='preview_connection_message' style='" + connection_message_preview_style + "'></div>",
      buttons: { "Edit Message": -1, Send: 0 },
      focus: 1,
      submit:function(e,v,m,f){
        e.preventDefault();
        if(v==0) {

          // send the message
          $.ajax({
            type: 'POST',
            url: '/user/' + sender_uid + '/send_message/' + recipient_uid,
            data: {send_message: connection_message},
            success: function(data) {
              if (data.result == true) {
                $('[name="jqi_state2_buttonPleasewait"]').html("Close");
                $(".lead.jqititle.undefined").html("Message sent");
                $("#message_sending_prompt").html('\u2714 <strong>Your message to ' + decodeURIComponent(recipient_name) + ' has been sent</strong>');
              } else {
                $("#message_sending_prompt").html('Oops, there was a problem sending your message<br><br>Reason: ' + data.error_message); 
                $('[name="jqi_state2_buttonPleasewait"]').html("Close");
              }
                
            },
            error: function(data) {
              $("#message_sending_prompt").html('Oops, there was a problems sending your message.'); 
            }
          });

          $.prompt.goToState('state2');
          return false;
        }
        else if(v==-1) {
          $.prompt.goToState('state0');
        }
      }
    },
    state2: {
      title: 'Sending message',
      html: "<p id='message_sending_prompt'>Please wait...</p>",
      buttons: { "Please wait": 0 },
      focus: 0,
      }
    } 

    $.prompt(statesdemo);
  }

});})(jQuery, Drupal, this, this.document);

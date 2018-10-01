/* Chapter moderation methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // allow members to submit a net promoter score
  jq_net_promoter_score = function(site_name) {
    
    countChar = function(val) {
      var len = val.value.length;
      if (len >= 250) {
        val.value = val.value.substring(0, 250);
      } else {
        $('#charNum').text((250 - len) + " chars");
      }
    };

    updateFeedbackLabel = function(val) {
      var nps_score = $('input[name=tm_nps]:checked').val();
      if (nps_score > 6) { 
      	$('#tm_nps_feedback_label').text('How can we be better?');
      }
      if (nps_score > 7) { 
      	$('#tm_nps_feedback_label').text('How can we be even better?');
      }
      if (nps_score < 5) { 
      	$('#tm_nps_feedback_label').text('How can we improve?');
      }
      if (nps_score == 5) { 
      	$('#tm_nps_feedback_label').text('You can add a comment');
      }
    };

    name = decodeURIComponent(name);  

    var message = "How likely are you to recommend " + site_name + " to a friend or colleague?";

    message = message + "<div class='tm_nps_container'><table style='width: 100%; margin-top: 0.75rem;'><tbody style='border-top: none;'><tr>";
    message = message + "<td style='width: 9%' align='center'><span class='tm_nps'><input onChange='updateFeedbackLabel()' type='radio' name='tm_nps' id='tm_nps_10' value='10'><br><label for='tm_nps_10'>10</label></span></td>";
 	for (var i = 9; i > 0; i--) {
    	message = message + "<td style='width: 9%' align='center'><span class='tm_nps'><input onChange='updateFeedbackLabel()' type='radio' class='tm_nps' name='tm_nps' id='tm_nps_" + i + "' value='" + i + "'><br><label for='tm_nps_" + i + "'>" + i + "</label></span></td>";
 	}
    message = message + "<td style='width: 9%' align='center'><span class='tm_nps'><input onChange='updateFeedbackLabel()' type='radio' class='tm_nps' name='tm_nps' id='tm_nps_0' value='0'><br><label for='tm_nps_0'>0</label></span></td>";
	message = message + "</tr>";
	message = message + "<tr><td colspan='6' style='width: 50%;'><i>&nbsp;&nbsp;Very likely</i></td><td colspan='5' style='width: 50%;' align='right'><i>Unlikely&nbsp;&nbsp;</i></td></tr>";
	message = message + "</tbody></table></div>";

    message = message + "<p><span id='tm_nps_feedback_label'>You can add a comment</span><textarea id='form_nps_comment' onkeyup='countChar(this);' value='' placeholder='Please share your feedback with us...' rows='2' cols='50'></textarea><div style='margin-top: -16px; float: right;'><span id='charNum' style='font-size: 10pt; color: #888;'>250 chars</span></div></p>",

    $.prompt(message, { 
      buttons: { "Send Feedback": true, "No thanks": false },
      title: "Tell us what you think",
      submit: function(e,v,m,f){
        if (v == true) {
          $('#tm_nps_index_message').hide();
          $('#tm_nps_index_submitting').show();
          var nps_score = $('input[name=tm_nps]:checked').val();
          window.location = '/review/submit?score=' + nps_score + '&comment=' + $("#form_nps_comment").val();
        }
      }
    });

  }


});})(jQuery, Drupal, this, this.document);
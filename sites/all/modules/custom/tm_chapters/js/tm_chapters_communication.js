/* Chapter announcement methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

var tm_communications_chapter_id = null;
var tm_communications_message = "";
var tm_communications_message_format = "";
var tm_communications_submitted = false;
var tm_communications_completed = false;
var tm_communications_confirm_chapters = false;
var tm_communications_recipients = "";

// send emails
// action is either test_email or send_emails
tm_chapter_communication_send_emails = function(form_action) {

	// 1. GET CHAPTER ID
	// get chapter id from drupal form
	// note: workaround as usual jquery targeting does not work with drupal hidden fields
	tm_communications_chapter_id = $('input[name^="chapterid"]').val();

	// 2. GET RECIPIENTS
	tm_communications_recipients = $('#edit-recipients').val();

	// 3. GET HTML OR TEXT MESSAGE
	// get the html message from the CKEditor
	var message;
	
	// Check if CKEditor enabled
	var message_format;
	if (typeof CKEDITOR !== 'undefined') {
  		tm_communications_message = CKEDITOR.instances["edit-body-value"].getData();
  		tm_communications_message_format = "html";
	} else {
		tm_communications_message = $('#edit-body-value').val();
		tm_communications_message_format = "text";
	}

	// 4. VALIDATION
	// check message is not empty
	if (tm_communications_message == "") {
	  jq_alert(null, "Please enter an announcement message.");
	  return false;
	}

	// check subject is not empty
	if ($('#edit-subject').val() == "") {
	  jq_alert(null, "Please provide a subject line.");
	  return false;
	}

	// check reply-to is empty or valid
	// trim spaces
	var inputEmail = $('#edit-reply-to').val().replace(/ /g,'');
	if (inputEmail != "") {
	  var isValid = true;
	  var emailReg = /^([\w-\.\+]+@([\w-]+\.)+[\w-]{2,10})?$/;
	  if(!emailReg.test(inputEmail)){
	      isValid = false;
	      $('#chapter-test-email-submit').val("Send test email");
	      $('#chapter-email-members-submit').val("Send Email To All Recipients");
	      jq_alert(null, "Reply-to address is not valid.<br>Tip: You may leave it blank for no reply address.");
	      return false;
	  }
	}

	// 5. SEND EMAILS
	// Either test or all emails

	// TEST EMAILS
	if (form_action == "test_email") {
		tm_chapter_communication_send_test_email();
	}

	// ALL EMAILS
	if (form_action == "send_emails") {
		tm_chapter_communication_send_recipient_emails();
	}	
}

// Send test email
tm_chapter_communication_send_test_email = function() {

	  // check address is not empty
	  if ($('#edit-testemail').val() == "") {
	    $('#chapter-test-email-submit').val("Send test email");
	    jq_alert(null, "Please provide a test email address.");
	    return false;
	  }

	  // check address is valid
	  // trim spaces
	  var inputEmail = $('#edit-testemail').val().replace(/ /g,'');;
	  if (inputEmail != "") {
	    var isValid = true;
	    var emailReg = /^([\w-\.\+]+@([\w-]+\.)+[\w-]{2,10})?$/;
	    if(!emailReg.test(inputEmail)){
	        isValid = false;
	        jq_alert(null, "Oops, your test email address is not valid.");
	        return false;
	    }
	  }

	  // check address is not empty
	  if ($('#edit-testemail-name').val() == "") {
	    $('#chapter-test-email-submit').val("Send test email");
	    jq_alert(null, "Name can't be empty.");
	    return false;
	  }

	  // Sending
	  $('#chapter-test-email-submit').val("Sending...");

	  // Set callback url
	  var callback_url = "/chapters/send-announcement-test/" + tm_communications_chapter_id;

      var include_cover_image = 0;
      if ($('[name="include_cover_image"]').is(':checked')) {
	     include_cover_image = 1;
      }

	  // Callback
	  $.ajax({
	    type: 'post',
	    dataType: 'json',
	    data: {'subject': $('#edit-subject').val(),
	          'message': tm_communications_message, //$('#edit-body').val(),
	          'message_format': tm_communications_message_format, // html, or text
	          'chapterid': tm_communications_chapter_id,
	          'replyto': $('#edit-reply-to').val(),
	          'address': $('#edit-testemail').val(),
	          'include_cover_image': include_cover_image,
	          'first_name': $('[name="test_email_name"]').val()},
	    url: callback_url}).done(function(return_data) {
	      if (typeof return_data.sent !== 'undefined') {
	        $('#chapter-test-email-submit').val("Send test email");
	         jq_alert("Your test email was sent", "A test email was sent to <i>" + $('#edit-testemail').val() + "</i>");
	       } else {
	         jq_alert("Error", "Error sending emails");
	       }
	      return true;
	  });
}

// Send emails to recipients
tm_chapter_communication_send_recipient_emails = function() {

	// confirm send to all chapter members
	if (!tm_communications_confirm_chapters) {

	 $.prompt('Your email will be delivered to <i>verified</i> addresses subscribed to <i>Chapter and Event Announcements</i>.', 
	 	{ 
			buttons: { "OK": true, "Cancel": false },
		  title: 'Send message to selected Chapter Members?',
		  submit: function(e,v,m,f){
		    if (v == true) {
		      tm_communications_confirm_chapters = true;
		      return tm_chapter_communication_send_recipient_emails("send_emails");
		    }
		 }
	 	});

	 	$('#chapter-email-members-submit').val("Send Email To All Recipients");
	  return;
	 }

	// Check if already completed
  if (tm_communications_completed) {
    jq_alert("Already sent. As a precaution you can only use this button once.");
    return false;
  }

  // Check already submitted
  if (tm_communications_submitted) {
    jq_alert("Already sending, please wait.");
    return false;
  }

  // Update
 	$('#chapter-email-members-submit').val("Sending (this may take a few moments)...");

 	// Set callback url
 	var callback_url = "/chapters/send-announcement-callback/" + tm_communications_chapter_id;

    var include_cover_image = 0;
    if ($('[name="include_cover_image"]').is(':checked')) {
       include_cover_image = 1;
    }

  // Callback
  tm_communications_submitted = true;
  $.ajax({
    type: 'post',
    dataType: 'json',
    data: {'subject': $('#edit-subject').val(),
          'message': tm_communications_message, //$('#edit-body').val(),
          'message_format': tm_communications_message_format, // html, or text
          'recipients': tm_communications_recipients,
          'chapterid': tm_communications_chapter_id,
          'approved_members': $('[name="approved_members"]').val(),
          'replyto': $('#edit-reply-to').val(),
          'include_cover_image': include_cover_image,
          'address': $('#edit-testemail').val()},
    url: callback_url}).done(function(return_data) {
      if (typeof return_data.sent !== 'undefined') {
          
          // Sent at least one email
          if (return_data.sent > 0) {
          	
          	$('#chapter-email-members-submit').val("Successfully sent announcement.");
	          $('#chapter-email-members-submit').attr("disabled", true);
	          $('#chapter-email-members-confirm').attr("disabled", true);
	          $('#edit-recipients').attr("disabled", true);

          	tm_communications_completed = true;
          	$.prompt("Congratulations, your announcement has been sent.<br><br><em>Tip: It may take a few minutes for your emails to be delivered. If you use gmail, check the 'Promotions' tab.</em>", 
					 	{ buttons: { "View chapter": true},
					  title: 'Your chapter announcement is on its way...',
						  submit: function(e,v,m,f){
						    if (v == true) {
						    	window.location = "/" + Drupal.settings.tm_chapters.chapter_url;
			    			}
			  			},
			  			close: function() {
			  				window.location = "/" + Drupal.settings.tm_chapters.chapter_url;
			  			}
		 				});
          }
          else {
          	// No emails sent
          	jq_alert("No recipients", "We couldn't find any members that matched your recipient list.");
          	$('#chapter-email-members-submit').val("Send Email To All Recipients");
          	tm_communications_submitted = false;
          }
          
       // Error
       } else {
        jq_alert("Error", "Error sending emails");
       }
  });

}


// EVENT HANDLERS

// chapter handler - test email
$('#chapter-test-email-submit').click(function (event) {
	event.preventDefault();
	return tm_chapter_communication_send_emails("test_email");
});

// event handler - send to all recipients
$('#chapter-email-members-submit').click(function (event) {
	event.preventDefault();
	if ($('#chapter-email-members-confirm').prop("checked")) {
	  return tm_chapter_communication_send_emails("send_emails");
	} else {
	  jq_alert("Have you sent a test email?", "Please confirm first that you are ready to proceed by checking the <strong>I'm ready</strong> checkbox.");
	  return false;
	}
});

// ON PAGE LOAD

// hide filter guidelines and help text
$('.filter-wrapper.form-wrapper').hide();

// Update page title
$("#page-title").html('<em>Chapter: <a href="/' + Drupal.settings.tm_chapters.chapter_url + '">' + Drupal.settings.tm_chapters.chapter_title + '</a></em>Send Chapter Announcement');


});})(jQuery, Drupal, this, this.document);

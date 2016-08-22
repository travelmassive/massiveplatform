/* Event announcement methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

var tm_communications_event_id = null;
var tm_communications_message = "";
var tm_communications_message_format = "";
var tm_communications_submitted = false;
var tm_communications_completed = false;
var tm_communications_confirm_chapters = false;
var tm_communications_recipients = "";
var tm_communications_chapter_id = null;


// send emails
// action is either test_email or send_emails
tm_communication_event_send_emails = function(form_action) {

	// 1. GET EVENT ID
	// get event id from drupal form
	// note: workaround as usual jquery targeting does not work with drupal hidden fields
	tm_communications_event_id = $('input[name^="eventid"]').val();

	
	// 2. GET RECIPIENTS AND CHAPTER ID
	// get recipient parts from options
	var recipients_parts = $('#edit-recipients').val().split("-");

	// no chapter id
	// ie: rsvp
	if (recipients_parts.length == 1) {
		tm_communications_recipients = recipients_parts[0];
	}

	// chapter id
	// ie: chapter-1234
	if (recipients_parts.length == 2) {
		tm_communications_recipients = recipients_parts[0];
		tm_communications_chapter_id = recipients_parts[1];
	}

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
	  var emailReg = /^([\w-\.\+]+@([\w-]+\.)+[\w-]{2,4})?$/;
	  if(!emailReg.test(inputEmail)){
	      isValid = false;
	      $('#event-test-email-submit').val("Send test email");
	      $('#event-email-attendees-submit').val("Send Email To All Recipients");
	      jq_alert(null, "Reply-to address is not valid.<br>Tip: You may leave it blank for no reply address.");
	      return false;
	  }
	}

	// 5. SEND EMAILS
	// Either test or all emails

	// TEST EMAILS
	if (form_action == "test_email") {
		tm_communication_send_test_email();
	}

	// ALL EMAILS
	if (form_action == "send_emails") {
		tm_communication_send_recipient_emails();
	}	
}

// Send test email
tm_communication_send_test_email = function() {

	  // check address is not empty
	  if ($('#edit-testemail').val() == "") {
	    $('#event-test-email-submit').val("Send test email");
	    jq_alert(null, "Please provide a test email address.");
	    return false;
	  }

	  // check address is valid
	  // trim spaces
	  var inputEmail = $('#edit-testemail').val().replace(/ /g,'');;
	  if (inputEmail != "") {
	    var isValid = true;
	    var emailReg = /^([\w-\.\+]+@([\w-]+\.)+[\w-]{2,4})?$/;
	    if(!emailReg.test(inputEmail)){
	        isValid = false;
	        jq_alert(null, "Oops, your test email address is not valid.");
	        return false;
	    }
	  }

	  // check address is not empty
	  if ($('#edit-testemail-name').val() == "") {
	    $('#event-test-email-submit').val("Send test email");
	    jq_alert(null, "Name can't be empty.");
	    return false;
	  }

	  // Sending
	  $('#event-test-email-submit').val("Sending...");

	  // Set callback url
	  var callback_url = "/events/send-announcement-test/" + tm_communications_event_id;

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
	          'eventid': tm_communications_event_id,
	          'replyto': $('#edit-reply-to').val(),
	          'address': $('#edit-testemail').val(),
	          'include_cover_image': include_cover_image,
	          'first_name': $('[name="test_email_name"]').val()},
	    url: callback_url}).done(function(return_data) {
	      if (typeof return_data.sent !== 'undefined') {
	        $('#event-test-email-submit').val("Send test email");
	         jq_alert("Your test email was sent", "A test email was sent to <i>" + $('#edit-testemail').val() + "</i>");
	       } else {
	         jq_alert("Error", "Error sending emails");
	       }
	      return true;
	  });
}

// Send emails to recipients
tm_communication_send_recipient_emails = function() {

	// confirm send to all chapter members
	if ((tm_communications_recipients == "chapter") && (!tm_communications_confirm_chapters)) {

	 $.prompt('Send message to all Chapter Members?', 
	 	{ 
			buttons: { "OK": true, "Cancel": false },
		  title: null,
		  submit: function(e,v,m,f){
		    if (v == true) {
		      tm_communications_confirm_chapters = true;
		      return tm_communication_send_recipient_emails("send_emails");
		    }
		 }
	 	});

	 	$('#event-email-attendees-submit').val("Send Email To All Recipients");
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
 	$('#event-email-attendees-submit').val("Sending (this may take a few moments)...");

 	// Set callback url
 	var callback_url = "/events/send-announcement-callback/" + tm_communications_event_id;

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
          'eventid': tm_communications_event_id,
          'replyto': $('#edit-reply-to').val(),
          'include_cover_image': include_cover_image,
          'address': $('#edit-testemail').val()},
    url: callback_url}).done(function(return_data) {
      if (typeof return_data.sent !== 'undefined') {
          
          // Sent at least one email
          if (return_data.sent > 0) {
          	
          	$('#event-email-attendees-submit').val("Successfully sent " + return_data.sent + " emails.");
	          $('#event-email-attendees-submit').attr("disabled", true);
	          $('#event-email-attendees-confirm').attr("disabled", true);
	          $('#edit-recipients').attr("disabled", true);

          	tm_communications_completed = true;
          	$.prompt("<b>We successfully sent " + return_data.sent + " emails.</b><br>It may take a few minutes to deliver them all.", 
					 	{ buttons: { "View event": true},
					  title: 'Good job, your announcement has been sent',
						  submit: function(e,v,m,f){
						    if (v == true) {
						    	window.location = "/" + Drupal.settings.tm_events.event_url;
			    			}
			  			},
			  			close: function() {
			  				window.location = "/" + Drupal.settings.tm_events.event_url;
			  			}
		 				});
          }
          else {
          	// No emails sent
          	jq_alert("No recipients", "We couldn't find any members that matched your recipient list. That\'s ok, try another list.");
          	$('#event-email-attendees-submit').val("Send Email To All Recipients");
          	tm_communications_submitted = false;
          }
          
       // Error
       } else {
        jq_alert("Error", "Error sending emails");
       }
  });

}


// EVENT HANDLERS

// event handler - test email
$('#event-test-email-submit').click(function (event) {
	event.preventDefault();
	return tm_communication_event_send_emails("test_email");
});

// event handler - send to all recipients
$('#event-email-attendees-submit').click(function (event) {
	event.preventDefault();
	if ($('#event-email-attendees-confirm').prop("checked")) {
	  return tm_communication_event_send_emails("send_emails");
	} else {
	  jq_alert("Have you sent a test email?", "Please confirm first that you are ready to proceed by checking the <strong>I'm ready</strong> checkbox.");
	  return false;
	}
});

// ON PAGE LOAD

// hide filter guidelines and help text
$('.filter-wrapper.form-wrapper').hide();

// Update page title
$("#page-title").html('<em>Event: <a href="/' + Drupal.settings.tm_events.event_url + '">' + Drupal.settings.tm_events.event_title + '</a></em>Send Announcement');


});})(jQuery, Drupal, this, this.document);

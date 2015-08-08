/* Event announcement methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

$('#event-test-email-submit').click(function (event) {
	event.preventDefault();
	$('#event-test-email-submit').val("Sending...");
	return event_send_emails("test_email");
	});

	var contact_eventattendess_submitted = false;
	var contact_eventattendess_completed = false;
	$('#event-email-attendees-submit').click(function (event) {
	event.preventDefault();
	if ($('#event-email-attendees-confirm').prop("checked")) {
	  $('#event-email-attendees-submit').val("Sending (this may take a few moments)...");
	  return event_send_emails("send_emails");
	} else {
	  alert("Have you tested your email? Please confirm first that you are ready to proceed.");
	  return false;
	}
	});

	// send emails
	// action is either test_email or send_emails
	event_send_emails = function(form_action) {

	// 1. GET EVENT ID
	// get event id from drupal form
	// note: workaround as usual jquery targeting does not work with drupal hidden fields
	var eventid = $('input[name^="eventid"]').val();

	// 2. GET HTML OR TEXT MESSAGE
	// get the html message from the CKEditor
	var message;
	if (CKEDITOR.instances["edit-body-value"] != undefined) {
	  message = CKEDITOR.instances["edit-body-value"].getData();
	}
	else {
	  // plain text selected, so CKEditor is disabled
	  message = $('#edit-body-value').val();
	}

	// 3. VALIDATION
	// check message is not empty
	if (message == "") {
	  alert ("Message can't be empty.");
	  return false;
	}

	// check subject is not empty
	if ($('#edit-subject').val() == "") {
	  $('#event-test-email-submit').val("Send test email");
	  $('#event-email-attendees-submit').val("Send Email To All Recipients");
	  alert ("Message can't be empty.");
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
	      alert ("Reply-to address is not valid.\n\nTip: You may leave it blank for no reply address.");
	      return false;
	  }
	}

	if ($('#edit-subject').val() == "") {
	  alert ("Message can't be empty.");
	  return false;
	}

	// 4. SEND EMAILS
	// Either test or all emails

	// TEST EMAILS
	if (form_action == "test_email") {

	  // check address is not empty
	  if ($('#edit-testemail').val() == "") {
	    $('#event-test-email-submit').val("Send test email");
	    alert ("Address can't be empty.");
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
	        alert ("Test email address is not valid.");
	        return false;
	    }
	  }

	  // check address is not empty
	  if ($('#edit-testemail-name').val() == "") {
	    $('#event-test-email-submit').val("Send test email");
	    alert ("Name can't be empty.");
	    return false;
	  }

	  $.ajax({
	    type: 'post',
	    data: {'subject': $('#edit-subject').val(),
	          'message': message, //$('#edit-body').val(),
	          'eventid': eventid,
	          'replyto': $('#edit-reply-to').val(),
	          'address': $('#edit-testemail').val(),
	          'first_name': $('#edit-testemail-name').val()},
	    url: "/node/" + eventid + "/test_email"}).done(function(return_data) {
	      if (!isNaN(return_data)) {
	        $('#event-test-email-submit').val("Send test email");
	         alert("Sent a test email to " + $('#edit-testemail').val());
	       } else {
	        alert("Error sending email: " + return_data);
	       }
	      return true;
	  });
	}

	// ALL EMAILS
	if (form_action == "send_emails") {

	  if ($('#edit-recipients').val() == "chapter") {
	    if (!confirm('Send message to all Chapter Members?')) {
	      $('#event-email-attendees-submit').val("Send Email To All Recipients");
	      return;
	    }
	  }

	  if (contact_eventattendess_completed) {
	    alert("Already sent. As a precaution you can only use this button once.");
	    return false;
	  }

	  if (contact_eventattendess_submitted) {
	    alert("Already sending, please wait.");
	    return false;
	  }
	  contact_eventattendess_submitted = true;
	  $.ajax({
	    type: 'post',
	    data: {'subject': $('#edit-subject').val(),
	          'message': message, //$('#edit-body').val(),
	          'recipients': $('#edit-recipients').val(),
	          'approved_members': $('#edit-approved-members').prop("checked"),
	          'eventid': eventid,
	          'replyto': $('#edit-reply-to').val(),
	          'address': $('#edit-testemail').val()},
	    url: "/node/" + eventid + "/send_emails"}).done(function(return_data) {
	      if (!isNaN(return_data)) {
	          $('#event-email-attendees-submit').val("Successfully sent " + return_data + " emails.");
	          $('#event-email-attendees-submit').attr("disabled", true);
	          $('#event-email-attendees-confirm').attr("disabled", true);
	          $('#edit-recipients').attr("disabled", true);
	          contact_eventattendess_completed = true;
	         alert("Successfully sent " + return_data + " emails. \nIt may take a few minutes to be delivered.");
	       } else {
	        alert("Error sending emails: " + return_data);
	       }
	  });
	}	
}

});})(jQuery, Drupal, this, this.document);

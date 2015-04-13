/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {

// To understand behaviors, see https://drupal.org/node/756722#behaviors
Drupal.behaviors.base_scripts = {
  attach: function(context, settings) {

    $('[data-dropd-toggle]').click(function(e){
      e.preventDefault();
      e.stopPropagation();
      var _self = $(this);
      var $drop = _self.closest('[data-dropd-wrapper]').find('[data-dropd]');

      // Hide others.
      $('[data-dropd-toggle]').not(_self).removeClass('on');
      $('[data-dropd]').not($drop).removeClass('on');

      // Set top value.
      $drop.css('top', _self.height());

      if (_self.hasClass('on')) {
        _self.removeClass('on');
        $drop.removeClass('on');
      }
      else {
        _self.addClass('on');
        $drop.addClass('on');
      }

    });

    $(document).click(function(e){
      if($(e.target).closest('[data-dropd]').length === 0) {
        $('[data-dropd-toggle], [data-dropd]').removeClass('on');
      }
    });

    $('.search-wrapper .toggle').once('search-menu-blk', function() {
      $(this).click(function() {
        // Probably because of the css animation the focus trigger must be
        // delayed. We guess that is because the element's visibility is
        // set to hidden and focus doesn't work for hidden elements.
        setTimeout(function(){
          $('.search-wrapper .form-text').focus();
        }, 50);
      });
    });

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
  }
};


})(jQuery, Drupal, this, this.document);

// Set the "to date" to be the "from date" when changed
// from http://tylerfrankenstein.com/code/drupal-automatically-set-date-when-date-changes-popup-calendar
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

      // When the 'from date' is changed on the following date fields, set
      // the 'to date' equal to the 'from date'.
      var date_fields_default_to_from = new Array(
        'field_event_date',
        'field_event_date_range'
      );
      var css_selector = '';
      $.each(date_fields_default_to_from, function(index, field){
          css_selector += '#edit-' + field.replace(/_/g,'-') + '-und-0-value-datepicker-popup-0, ';
      });
      css_selector = css_selector.substring(0, css_selector.length - 2); // Remove last ', '
      $(css_selector).change(function(){
          $('#' + $(this).attr('id').replace(/value/g, 'value2')).val($(this).val());
      });
});})(jQuery, Drupal, this, this.document);


// Set the "to date" to be the "from date" when changed
// from http://tylerfrankenstein.com/code/drupal-automatically-set-date-when-date-changes-popup-calendar
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // page load
  $('.form-item-check-company').find('.description').hide();
  if ($('#edit-check-company').prop('checked')) {
          $('.form-item-check-company').find('.description').show();
  }

  // if company is checked
  $('#edit-check-company').change(function() {
    if ($('#edit-check-company').prop('checked')) {
          $('.form-item-check-company').find('.description').show();
          //$('#edit-submit').attr('disabled', 'disabled');
    }
  });
     
});})(jQuery, Drupal, this, this.document);

(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  var current_user_score = $('#current_user_score').val();
  var current_user_uid = $('#current_user_uid').val();
  var confirm_title = "Oops, your profile is only " + current_user_score + "% complete.";
  var confirm_message = "Please fill out your community profile first and then request approval so we can review your profile.";
  var score_threshold = 50;
  var profile_edit_url = "/user/" + current_user_uid + "/edit#user-profile-options";

  $(".approval-link").each(function() {
    $(this).click(function(event) {

      // don't ask if the user score is above the threshold
      if (current_user_score > score_threshold) {
        return;
      }
      
      // prompt to complete profile
      event.preventDefault();
      $.prompt(confirm_message, {
          title: confirm_title,
          buttons: { "OK": false, "Edit Profile": true },
          submit: function(e,v,m,f){
            if (v==true) {
              window.location = profile_edit_url;
            }
          },
        }
      );

    });
  });
     
});})(jQuery, Drupal, this, this.document);

(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // set default classes for impromptu
  $.prompt.setDefaults({classes: {
    button: 'pure-button',
    defaultButton: 'pure-button-primary',
  }});

  // wrapper for alert
  jq_alert = function(alert_title, message) {
    $.prompt(message, {title: alert_title});
  } 

  // wrapper for confirm
  jq_confirm_url = function(confirm_title, message, ok_url) {
    $.prompt(message, { buttons: { "OK": true, "Cancel": false },
      title: confirm_title,
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = ok_url;
        }
      },
    });
  }

  jq_confirm_leave_chapter = function(chapter_title) {
    confirm_title = "Are you sure you want to leave this chapter?";
    message = "If you leave you won\'t be notified about upcoming events in " + chapter_title + ".";
    $.prompt(message, { buttons: { "OK": true, "Cancel": false },
      title: confirm_title,
      submit: function(e,v,m,f){
        if (v == true) {
          // this worked but now now
          // $( ".flag-confirm" ).unbind("click.confirm");
          //$( ".flag-confirm" ).click();

          // plan b, get the unflag url from the href and visit it
          href = $( "a.unflag-action" ).attr('href');
          window.location = href;
        }
      },
    });
  }

  jq_request_approval = function(uid) {
    confirm_title = "Approve my account";
    message = ""; //We\'re a growing community, and we need your help to ensure everyone fits in. ";
    message = message + "<p style='margin-top: 0px;'>In a few words, please tell us why you would like to join the community.</p>";
    if (reason_for_joining != "") {

    }
    // get reason for joining from hidden form field
    // we don't want to move this around in the js call as it\'s visible
    //reason_for_joining = $("#reason_for_joining").val();
    message = message + "<input type='text' id='form_reason_for_approval' value='' maxlength='150' placeholder='I want to join because...' size='100'>";

    $.prompt(message, { buttons: { "Request Approval": true, "Cancel": false },
      title: confirm_title,
      loaded: function() {
        // copy in reason for approval from hidden field
        // avoids uri encoding
        $("#form_reason_for_approval").val(decodeURIComponent($("#reason_for_joining").val().replace(/\+/g, " ", true)));
      },
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/user/' + uid + '/request_approval?reason_for_joining=' + $("#form_reason_for_approval").val();
        }
      },
    });
  }

  jq_approval_already_requested = function() {
    jq_alert(null, "Please allow 12-24 hours for a Chapter Leader to review your account.<br><br>Our community is important to us, so please ensure you\'ve filled out your profile so we can approve you.<br><br>If your account has not been approved please <a href='/contact'>contact us</a> so we can assist you.");
  }

  jq_confirm_approve_member = function(uid) {
    jq_confirm_url('Do you want to approve this account?', 'Guidelines for approval:<li>Account is a real person</li><li>Profile is filled out</li><li>Profile is not a company or brand</li>', '/user/' + uid + '/approve');
  }

  jq_confirm_unapprove_user = function(uid) {
    jq_confirm_url('Do you want to un-approve this account?', 'The user will not be notified automatically. Please contact the user to address the issue.', '/user/' + uid + '/unapprove');
  }

  jq_confirm_incomplete_profile = function(uid) {

    $.prompt({
      state0: {
        title: 'Flag this account as incomplete?',
        html: 'This action will:<li>Notify the member to update their profile</li><li>Notify you when the member requests approval</li>',
        buttons: { Cancel: false, Next: true },
        focus: 1,
        submit:function(e,v,m,f){
          if(v){
            e.preventDefault();
            $.prompt.goToState('state1');
            return false;
          }
          $.prompt.close();
        }
      },
      state1: {
        title: 'Add a helpful comment?',
        html: "You can send a short message to the member. <textarea id='form_moderator_message' value='' placeholder='Please complete your profile...' rows='3' cols='50'></textarea>",
        buttons: { Back: -1, OK: true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {
            // replace new lines with __NL__ as we moving this via regular GET and cant pass it via XHR
            window.location = '/user/' + uid + '/approval_need_more_info?moderator_message=' + document.getElementById('form_moderator_message').value.replace(/\n/mg,"__NL__"); 
          } 
          else if(v==-1) {
            $.prompt.goToState('state0');
          }
        }
      }
    });

  }

  jq_confirm_company_profile = function(uid) {

    $.prompt({
      state0: {
        title: 'Flag this account as a company or brand?',
        html: 'This action will:<li>Notify the member to personalize their profile</li><li>Inform member on how to list their company</li><li>Notify you when the member requests approval</li>',
        buttons: { Cancel: false, Next: true },
        focus: 1,
        submit:function(e,v,m,f){
          if(v){
            e.preventDefault();
            $.prompt.goToState('state1');
            return false;
          }
          $.prompt.close();
        }
      },
      state1: {
        title: 'Add a helpful comment?',
        html: "You can send a short message to the member. <textarea id='form_moderator_message' value='' placeholder='Please complete your profile...' rows='3' cols='50'></textarea>",
        buttons: { Back: -1, OK: true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {
            // replace new lines with __NL__ as we moving this via regular GET and cant pass it via XHR
            window.location = '/user/' + uid + '/approval_is_company_or_brand?moderator_message=' + document.getElementById('form_moderator_message').value.replace(/\n/mg,"__NL__"); 
          } 
          else if(v==-1) {
            $.prompt.goToState('state0');
          }
        }
      }
    });

  }


});})(jQuery, Drupal, this, this.document);



  

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



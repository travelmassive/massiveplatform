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

  // Drupal behaviours
  // Called after loading new content (ie: views_more) to reattach event handlers, etc
  // To understand behaviors, see https://drupal.org/node/756722#behaviors
  Drupal.behaviors.base_scripts = {
    attach: function(context, settings) {
      // include any methods to be attached on new content loaded
    }
  };

  // Load once on page load
  jQuery(document).ready(function(){
    $('[data-dropd-toggle]').click(function(e) {

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

      // if not enough room to display account menu, scroll to top and turn off sticky
      // otherwise we might not be able to see all menu items
      if (_self.attr("href") == "#account-menu-blk") {
        if ($(window).height() <= $("#account-menu-blk").height()) {
          if (_self.hasClass('on')) {
            if ($(".header").unstick !== undefined) {
              $(".header").unstick();
            }
            window.scrollTo(0, 0);
          } else {
            makeHeaderSticky();
          }
        }
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
  }); // close jquery document.ready

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


// Show sign up community validation message
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  tm_community_values_show_validation_message = function() {
    $(".tm_community_values_validation_message").hide();
    selected_option = $('input[name=check_community_values]:checked').val();
    $(".tm_community_values_validation_message.validation_message-" + selected_option).show();
  }

  // show validation message when radio button selected
  $('input[name=check_community_values]').change(function() {
    tm_community_values_show_validation_message();
  });

  // show validation message on page load (if held up by validation)
  tm_community_values_show_validation_message();

     
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
          }
        }
      );

    });
  });
     
});})(jQuery, Drupal, this, this.document);


// impromptu alerts
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

  // set default classes for impromptu
  $.prompt.setDefaults({
    classes: {
      button: 'pure-button',
      defaultButton: 'pure-button-primary'
    }
    });

  // wrapper for alert
  jq_alert = function(alert_title, message) {
    $.prompt(message, {title: alert_title});
  }

  // show create member event message
  jq_create_member_event_message = function() {
    if (typeof Drupal.settings.tm_events !== 'undefined') {
      if (typeof Drupal.settings.tm_events.create_member_event_message !== 'undefined') {
        var message = Drupal.settings.tm_events.create_member_event_message;
        if ((message != null) && (message != "")) {
          $.prompt(message, {title: null});
        }
      }
    }
  }

  // show create company event message
  jq_create_company_event_message = function() {
    if (typeof Drupal.settings.tm_events !== 'undefined') {
      if (typeof Drupal.settings.tm_events.create_company_event_message !== 'undefined') {
        var message = Drupal.settings.tm_events.create_company_event_message;
        if ((message != null) && (message != "")) {
          $.prompt(message, {title: null});
        }
      }
    }
  }

  // wrapper for confirm
  jq_confirm_url = function(confirm_title, message, ok_url) {
    $.prompt(message, { buttons: { "OK": true, "Cancel": false },
      title: confirm_title,
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = ok_url;
        }
      }
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
      }
    });
  }

  jq_confirm_unregister_event = function(event_title) {
    confirm_title = "Are you sure you want to unregister?";
    message = "If you cancel your registration, you will miss out and everyone will have fun without you.";
    $.prompt(message, { buttons: { "OK": true, "I like fun, I'm still going": false },
      title: confirm_title,
      submit: function(e,v,m,f){
        if (v == true) {
          //get the unflag url from the href and visit it
          href = $( "a.unflag-action" ).attr('href');
          window.location = href;
        }
      }
    });
  }

  jq_confirm_unregister_paid_event = function(event_title) {
    confirm_title = "Are you sure you want to cancel your ticket?";
    message = "Please check the refund policy for this event as cancellation may not be refundable.";
    $.prompt(message, { buttons: { "Yes, cancel my ticket": true, "I'm still going": false },
      title: confirm_title,
      submit: function(e,v,m,f){
        if (v == true) {
          //get the unflag url from the href and visit it
          href = $( "a.unflag-action" ).attr('href');
          window.location = href;
        }
      }
    });
  }

  jq_confirm_unregister_waitlist = function(event_title) {
    confirm_title = "Are you sure you want to leave the waitlist?";
    message = "If you leave the waitlist the event organizers will not be able to select you if a space becomes available.";
    $.prompt(message, { buttons: { "OK": true, "Cancel": false },
      title: confirm_title,
      submit: function(e,v,m,f){
        if (v == true) {
          //get the unflag url from the href and visit it
          href = $( "a.unflag-action" ).attr('href');
          window.location = href;
        }
      }
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
      }
    });
  }

  // prompt if user can request approval
  // allow a 30 minute wait time if user opts for "not yet"
  jq_prompt_request_approval = function(uid) {

    var current_user_score = $('#current_user_score').val();

    // if user has previouslly chosen "No, not yet" then don't show
    if ($.cookie("Drupal.visitor.dont_request_approval")) {
      return;
    }

    $.prompt({
      state0: {
        title: 'Would you like to request approval?',
        html: 'Your profile is now ' + current_user_score + '% complete and we can review your account.',
        buttons: { "Yes, request approval": true, "No, not yet": false },
        //focus: 1,
        submit:function(e,v,m,f){
          if(v){
            e.preventDefault();
            $.prompt.close();
            jq_request_approval(uid);
            return false;
          } else {
            // set not to request for next 30 minutes
            var date = new Date();
            var minutes = 30;
            date.setTime(date.getTime() + (minutes * 60 * 1000));
            $.cookie("Drupal.visitor.dont_request_approval", 1, { expires : date });
          }
          $.prompt.close();
        }
      }
    });
  }

  jq_approval_already_requested = function() {
    jq_alert(null, "Please allow 12-24 hours for a Chapter Leader to review your account.<br><br>Our community is important to us, so please ensure you\'ve filled out your profile so we can approve you.<br><br>If your account has not been approved please <a href='/contact'>contact us</a> so we can assist you.");
  }

  jq_confirm_approve_member = function(uid, community_values_url) {
      
    $.prompt({
      state0: {
        title: 'Do you want to approve this account?',
        html: 'Guidelines for approval:<li>Account is a real person</li><li>Profile is not a company or brand</li><li>Profile picture is not a brand logo</li><li>At least one link to a personal account</li><li>Profile meets our <a target="_blank" href="' + community_values_url + '">community values</a></li>',
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
        title: 'Add a welcome message?',
        html: "You can send a short message to the member. <textarea id='form_moderator_message' value='' placeholder='Thanks for joining...' rows='3' cols='50'></textarea>",
        buttons: { Back: -1, OK: true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {
            // replace new lines with __NL__ as we moving this via regular GET and cant pass it via XHR
            window.location = '/user/' + uid + '/approve?moderator_message=' + document.getElementById('form_moderator_message').value.replace(/\n/mg,"__NL__"); 
          } 
          else if(v==-1) {
            $.prompt.goToState('state0');
          }
        }
      }
    });

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

  jq_confirm_non_community_profile = function(uid, community_values_url) {

    $.prompt({
      state0: {
        title: 'Flag this account as non-community profile?',
        html: 'This action will:<li>Set this account to <i>un-approved</i></li><li>Inform account owner of <a target="_blank" href="' + community_values_url + '">membership guidelines</a></li><li>Notify you if the account owner requests approval</li>',
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
        html: "You can send a short message to the person. <textarea id='form_moderator_message' value='' placeholder='Hello, it looks like your profile does not meet our community guidelines...' rows='3' cols='50'></textarea>",
        buttons: { Back: -1, OK: true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {
            // replace new lines with __NL__ as we moving this via regular GET and cant pass it via XHR
            window.location = '/user/' + uid + '/moderate_non_community_profile?moderator_message=' + document.getElementById('form_moderator_message').value.replace(/\n/mg,"__NL__"); 
          } 
          else if(v==-1) {
            $.prompt.goToState('state0');
          }
        }
      }
    });

  }

  jq_confirm_report_member = function(uid, community_values_url) {

    $.prompt({
      state0: {
        title: 'Report an issue',
        html: 'Help us understand the issue with this member.'
        + '<p><input type="radio" id="reason_1" name="form_report_reason" value="spam"> <label style="display: inline;" for="reason_1">Posting spam<br>'
        + '<input type="radio" id="reason_2" name="form_report_reason" value="abusive"> <label style="display: inline;" for="reason_2">They are being abusive<br>'
        + '<input type="radio" id="reason_3" name="form_report_reason" value="non-community"> <label style="display: inline;" for="reason_3">They don\'t meet our <a target="_blank" href="' + community_values_url + '">community values</a><br>'
        + '<input type="radio" id="reason_4" name="form_report_reason" value="other"> <label style="display: inline;" for="reason_4">Other reason'
        + '</p>',
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
        title: 'Report an issue',
        html: "Additional information (optional) <textarea id='form_report_message' value='' placeholder='Provide any additional information that will help us review this issue.' rows='3' cols='50'></textarea>",
        buttons: { Back: -1, OK: true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {
            var report_reason = $('input[name=form_report_reason]:checked').val();
            if (report_reason === 'undefined') {
              report_reason = 'other';
            }
            // replace new lines with __NL__ as we moving this via regular GET and cant pass it via XHR
            window.location = '/user/' + uid + '/moderate_report_member?reason=' + report_reason + '&report_message=' + document.getElementById('form_report_message').value.replace(/\n/mg,"__NL__"); 
          } 
          else if(v==-1) {
            $.prompt.goToState('state0');
          }
        }
      }
    });

  }

  jq_confirm_unreport_member = function(uid) {

    $.prompt({
      state0: {
        title: 'Remove report flags',
        html: 'Has the reported issue been resolved?',
        buttons: { No: false, Yes: true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {
            window.location = '/user/' + uid + '/moderate_report_member_resolved'; 
          } 
          $.prompt.close();
        }
      }
    });

  }

  jq_confirm_cancel_account = function(uid) {

    confirm_title = "Are you sure you want to delete your account?";
    message = "<p style='margin-top: 0px;'>If you continue you will be prompted to confirm your password to cancel your account. <br></p>";   
    message = message + "<strong>Reason for leaving</strong><br><input type='text' name='cancel_account_feedback' id='cancel_account_feedback' value='' maxlength='150' placeholder='I am cancelling my account because...' size='100'>";

    $.prompt(message, { buttons: { "Continue": true, "Cancel": false },
      title: confirm_title,
      submit: function(e,v,m,f){
        if (v == true) {
          window.location = '/user/' + uid + '/confirm_cancel_account?feedback=' + $("#cancel_account_feedback").val();
        }
      }
    });
  }

  // show the login/signup box
  // called from _tm_anon_flag_placeholder
  // this method copies the account-menu-blk rendered in tm_users_account_menu for anon users
  jq_login_signup_box = function() {

    // improvements for IE support
    // clone the form and update ids
    // add event listener to login button to submit the form via js
    html = $("#account-menu-blk").clone(false).find("*[id]").andSelf().each(function() { $(this).attr("id", $(this).attr("id") + "-cloned"); }).html();
    message = '<div id="account-menu-blk">' + html + '</div>';
    $.prompt(message, {
        buttons: {},
        loaded: function() { 
          $("#edit-submit-cloned").click(function(e) {
            e.preventDefault();
            $("#user-login-form-cloned")[0].submit();
          });
        }
    });
  
  }

  // show full avatar or logo
  jq_show_avatar = function() {
    if (Drupal.settings.currentUser > 0) {
      avatar_img = $(".badge-organization img, .badge-user img")[0];
      if ($(".badge-organization, .badge-user").hasClass("zoomable")) {
        img_html = "<div id='zoom_picture_container'><img class='zoom_picture' src='" + avatar_img.src.replace(/'/g, "") + "'></div>";
        $.prompt(img_html, {
          title: null, 
          persistent: false, 
          classes: { box: 'tm_zoom_picture'}, 
          buttons: {}, 
          overlayspeed: 'fast',
          loaded: function() { $("#page").addClass("tm-blur-filter"); },
          close: function() { $("#page").removeClass("tm-blur-filter"); }
        });
      }
    } else {
      // show login box if logged out
      jq_login_signup_box();
    }
  }
  $(".badge-organization.zoomable, .badge-user.zoomable").click(jq_show_avatar);


  // allow member to send a short message on follow to recipient
  jq_follow_message_member = function(uid) {
    
    countChar = function(val) {
      var len = val.value.length;
      if (len >= 150) {
        val.value = val.value.substring(0, 150);
      } else {
        $('#charNum').text((150 - len) + " chars");
      }
    };

    randomHello = function() {
      var hellos = ["Hola!", "Bonjour!", "Konnichiwa!"];
      return hellos[Math.floor(Math.random()*hellos.length)];
    }

    $.prompt({
      state0: {
        //title: 'Send a follow message',
        html: "Why do you want to connect? <br><textarea id='form_follow_message' onkeyup='countChar(this);' value='' placeholder='" + randomHello() + " Send a short message. Links will be removed.' rows='2' cols='50'></textarea><div style='float: right;'><span id='charNum' style='font-size: 10pt; color: #888;'>150 chars</span></div><br>",
        buttons: { Back: -1, Follow: true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {
            // replace new lines with __NL__ as we moving this via regular GET and cant pass it via XHR
            window.location = '/user/' + uid + '/follow?follow_message=' + document.getElementById('form_follow_message').value.replace(/\n/mg,"__NL__"); 
          } 
          $.prompt.close();
        }
      }
    });
  }

  // allow member to send a short message on follow to organization owners
  jq_follow_message_organization = function(nid) {
    
    countChar = function(val) {
      var len = val.value.length;
      if (len >= 150) {
        val.value = val.value.substring(0, 150);
      } else {
        $('#charNum').text((150 - len) + " chars");
      }
    };

    randomHello = function() {
      var hellos = ["Hola!", "Bonjour!", "Konnichiwa!"];
      return hellos[Math.floor(Math.random()*hellos.length)];
    }

    $.prompt({
      state0: {
        //title: 'Send a follow message',
        html: "Why do you want to connect? <br><textarea id='form_follow_message' onkeyup='countChar(this);' value='' placeholder='" + randomHello() + " Send a short message. Links will be removed.' rows='2' cols='50'></textarea><div style='float: right;'><span id='charNum' style='font-size: 10pt; color: #888;'>150 chars</span></div><br>",
        buttons: { Back: -1, Follow: true },
        focus: 1,
        submit:function(e,v,m,f){
          if (v == true) {
            // replace new lines with __NL__ as we moving this via regular GET and cant pass it via XHR
            window.location = '/node/' + nid + '/follow_organization?follow_message=' + document.getElementById('form_follow_message').value.replace(/\n/mg,"__NL__"); 
          } 
          $.prompt.close();
        }
      }
    });

  }

});})(jQuery, Drupal, this, this.document);


// sticky headers
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

  // get IE version
  // http://stackoverflow.com/questions/19999388/jquery-check-if-user-is-using-ie
  msieversion = function() {
    var ua = window.navigator.userAgent;
    var msie = ua.indexOf("MSIE ");
    // If Internet Explorer, return version number
    if (msie > 0 || !!navigator.userAgent.match(/Trident.*rv\:11\./)) {
      return (parseInt(ua.substring(msie + 5, ua.indexOf(".", msie))));
    }
    else {
      // If another browser, return 0
      return 0;
    }
  }

  isMobileDevice = function() {
    return ( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent));
  }

  // set sticky header
  makeHeaderSticky = function() {

    if (typeof($(".header").sticky) == "function") {
      $(".header").sticky({topSpacing:0, className: 'sticky'});

      $('.prime-nav-wrappers').css({'margin-top': '0rem'});

      // animate if non-IE
      if (msieversion() == 0) {

        // shrink
        $('.header').on('sticky-start', function() {

          if (isMobileDevice()) {
            // no animation
            $('.header').css({height: "3rem", 'padding-top': "0rem"});
            $('.header-logo').css({height: "1em", 'margin-top': "0.5rem"});
          } else {
            // animate
            $('.header').animate({height: "3rem", 'padding-top': "0rem"}, 500);
            $('.header-logo').animate({height: "1em", 'margin-top': "0.5rem"}, 500);
          }
          
        });
      
        // expand
        $('.header').on('sticky-end', function() { 
          if (isMobileDevice()) {
            $('.header').css({height: "5rem", 'padding-top': "1rem"});
            $('.header-logo').css({height: "3rem", 'margin-top': "0"});
          } else {
            // animate
            $('.header').animate({height: "5rem", 'padding-top': "1rem"}, 150);
            $('.header-logo').animate({height: "3rem", 'margin-top': "0"}, 150);
          }
          
        });
      }
    }
  }

  makeHeaderSticky();

});})(jQuery, Drupal, this, this.document);

// account menu functions
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

  tm_show_account_menu_moderator_actions = function() {
    $("#account_menu_moderator_actions_show").hide();
    $("#account_menu_moderator_actions_items").show();
  }

});})(jQuery, Drupal, this, this.document);


// front page video controls
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function() {

  tm_frontpage_video_play = function() {
    var video_instance = $(document.body).data('vide');
    video_instance.getVideoObject().play();
  }

  tm_frontpage_video_pause = function() {
    var video_instance = $(document.body).data('vide');
    video_instance.getVideoObject().pause();
  }

  // handlers if front page video showing and not mobile
  if ($('#tm-frontpage-video-controls').length && !(isMobileDevice())) {

    // click handler for pause
    $(".tm-frontpage-video-pause").click(function() { 
      $(".tm-frontpage-video-pause").hide();
      $(".tm-frontpage-video-play").show();
      tm_frontpage_video_pause();
    });

    // click handler for play
    $(".tm-frontpage-video-play").click(function() { 
      $(".tm-frontpage-video-pause").show();
      $(".tm-frontpage-video-play").hide();
      tm_frontpage_video_play();
    });

    // show video controls after 5 seconds
    $("#tm-frontpage-video-controls").delay(5000).fadeIn();
  }

});})(jQuery, Drupal, this, this.document);



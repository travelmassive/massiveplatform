/**
 * @file tm_event_signup_flag.js
 * Specific functions related to the signup flag.
 * 
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */

alert("hello!!");

Drupal.behaviors.tm_event_signup_flag = {

  attach: function (context, settings) {
    // Local jQuery.
    var $ = jQuery;
    
    // We only want to bind the listener once.
    $('body').once('tmEventsSignupFlag', function() {
      // Listener for when the flag link was updated.
      $(document).bind('flagGlobalAfterLinkUpdate', function(event, data) {
        // Only update the seat count if the flag was successful and if is
        // the signup flag.
        if (data.flagSuccess && data.flagName == 'signup') {
          $('.js-tm-seats-count').html(Drupal.theme('tm_event_seats_count', data.updatedSeatsLeft, data.totalSeats));
        }
      });
    });
  }
};

/**
 * Custom function to render the attendee count.
 * Since the join process is done through AJAX there needs to
 * be an equivalent theming function on the javascript side.
 * The server side version of this function is on the tm_events.module
 */
Drupal.theme.prototype.tm_event_seats_count = function (left, total) {
  return Drupal.t('@left of @total seats left', {'@left' : left, '@total' : total});
};
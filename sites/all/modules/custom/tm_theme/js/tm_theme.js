/**
 * @file event_signup_flag.js
 * General function related to this tm theme in specific.
 * 
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */

Drupal.behaviors.tm_theme = {
  attach: function (context, settings) {
    // Local jQuery.
    var $ = jQuery;
    
    // Disable click in every element with the "disabled" class.
    // Despite this, is not guaranteed that other click listeners won't fire.
    // If they are added at a later stage there's not much that can be done.
    $('.disabled').once('disabledItem', function() {
      $(this).click(function(e) {
        e.preventDefault();
        e.stopImmediatePropagation();
      });
    });
    
  }
};
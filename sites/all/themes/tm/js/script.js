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

    $('#test-submit').click(function (event) {
      // need to write an email sending page
      // send the various form elements as parameters
      $.ajax({
        type: 'post',
        data: {'subject': $('#edit-subject').val(),
              'message': $('#edit-body').val(),
              'address': $('#edit-testemail').val()},
        url: "/testemail"}).done(function() {
        alert("Sent a test email to " + $('#edit-testemail').val());
      });
      event.preventDefault();
    });

  }
};


})(jQuery, Drupal, this, this.document);

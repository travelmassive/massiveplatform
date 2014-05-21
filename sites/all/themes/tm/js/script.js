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

    $('[data-dropdown-toggle]').click(function(e){
      e.preventDefault();
      e.stopPropagation();
      var _self = $(this);
      var $drop = _self.closest('[data-dropdown-wrapper]').find('[data-dropdown]');
      
      // Hide others.
      $('[data-dropdown-toggle]').not(_self).removeClass('on');
      $('[data-dropdown]').not($drop).removeClass('on');
      
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
      if($(e.target).closest('[data-dropdown]').length === 0) {
        $('[data-dropdown-toggle], [data-dropdown]').removeClass('on');
      }
    });
    
  }
};


})(jQuery, Drupal, this, this.document);

// JavaScript Document

// jQuery is now namespaced to avoid conflicts with other Javascript libraries such as Prototype.
// All your code that expects to use jQuery as $ should be wrapped in an outer context like so:

(function($) {
Drupal.behaviors.date_popup_custom = {};

Drupal.behaviors.date_popup_custom.attach = function (context, settings) {
  var $widget = $('.form-type-date-popup').parents('fieldset').once('date');
  var i;
  for (i = 0; i < $widget.length; i++) {
    new Drupal.date.EndDatePopupHandler($widget[i]);
  }
};

Drupal.date = Drupal.date || {};

/**
 * Constructor for the EndDateHandler object.
 *
 * The EndDatePopupHandler is responsible for synchronizing a date popup widget's
 * end date with its start date. This behavior lasts until the user
 * interacts with the end date widget.
 *
 * @param widget
 *   The fieldset DOM element containing the from and to dates.
 */
Drupal.date.EndDatePopupHandler = function (widget) {
  this.$widget = $(widget);
  this.$start = this.$widget.find('.form-type-date-popup[class$=value]').find('.form-type-textfield[class$=value-date]').find('input');
  this.$end = this.$widget.find('.form-type-date-popup[class$=value2]').find('.form-type-textfield[class$=value2-date]').find('input');
  if (this.$end.length == 0) {
    return;
  }
  // Only act on date fields where the end date is completely blank or already
  // the same as the start date. Otherwise, we do not want to override whatever
  // the default value was.
  this.bindClickHandlers();
  // Start out with identical start and end dates.
  this.syncEndDate();
};


/**
 * Returns true if all dropdowns in the end date widget are blank.
 */
Drupal.date.EndDatePopupHandler.prototype.endDateIsBlank = function () {
  return this.$end.val() == '';
};

/**
 * Returns true if the end date widget has the same value as the start date.
 */
Drupal.date.EndDatePopupHandler.prototype.endDateIsSame = function () {
  return this.$start.val() == this.$end.val();
};

/**
 * Returns true if the end date widget has a lesser value than the start date.
 */
Drupal.date.EndDatePopupHandler.prototype.endDateIsLess = function () {
  return (new Date(this.$end.val())).valueOf() - (new Date(this.$start.val())).valueOf() < 0;
};

/**
 * Add a click handler to each of the start date's select dropdowns.
 */
Drupal.date.EndDatePopupHandler.prototype.bindClickHandlers = function () {
  this.$start.bind('change.EndDatePopupHandler', this.startClickHandler.bind(this));
  this.$end.bind('change', this.endchangeHandler.bind(this));
};

/**
 * Click event handler for each of the start date's select dropdowns.
 */
Drupal.date.EndDatePopupHandler.prototype.startClickHandler = function (event) {
  this.syncEndDate();
};

/**
 * change event handler for each of the end date's select dropdowns.
 */
Drupal.date.EndDatePopupHandler.prototype.endchangeHandler = function (event) {
  //this.$start.unbind('change.EndDatePopupHandler');
  //$(event.target).unbind('change', this.endchangeHandler);
};

Drupal.date.EndDatePopupHandler.prototype.syncEndDate = function () {
  if (this.endDateIsBlank() || this.endDateIsSame() || this.endDateIsLess()) {
    this.$end.val(this.$start.val());
  }
};  
})(jQuery);

/**
 * Function.prototype.bind polyfill for older browsers.
 * https://developer.mozilla.org/en/JavaScript/Reference/Global_Objects/Function/bind
 */
if (!Function.prototype.bind) {
  Function.prototype.bind = function (oThis) {
    if (typeof this !== "function") // closest thing possible to the ECMAScript 5 internal IsCallable function
      throw new TypeError("Function.prototype.bind - what is trying to be fBound is not callable");

    var aArgs = Array.prototype.slice.call(arguments, 1),
        fToBind = this,
        fNOP = function () {},
        fBound = function () {
          return fToBind.apply(this instanceof fNOP ? this : oThis || window, aArgs.concat(Array.prototype.slice.call(arguments)));
        };

    fNOP.prototype = this.prototype;
    fBound.prototype = new fNOP();

    return fBound;
  };
}
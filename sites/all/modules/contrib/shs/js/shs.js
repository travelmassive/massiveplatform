/**
 * @file
 * Custom behaviors for Simple hierarchical select.
 */

(function ($, Drupal) {

  /**
   * Creates the widget for Simple hierarchical select.
   */
  Drupal.behaviors.shsWidgetCreate = {

    // Default function to attach the behavior.
    attach: function (context, settings) {
      var self = this;
      var settingsDefault = {
        display: {
          animationSpeed: 400,
        }
      };
      $('select.shs-enabled:not([disabled])')
        .once('shs')
        .addClass('element-invisible')
        .hide()
        .each(function() {
          $field = $(this);
          var fieldName = $(this).attr('name');
          // Multiform messes up the names of the fields
          // to the format multiform[something][fieldname][...].
          if (fieldName.indexOf('multiform') == 0) {
            var split = fieldName.split('][');
            split.splice(0, 1);
            fieldName = split.splice(0, 1) + '[' + split.join('][');
          }

          if (fieldName in settings.shs) {
            var fieldSettings = {};
            // Since we store the field settings within an associative array with
            // random strings as keys (reason: http://drupal.org/node/208611) we
            // need to get the last setting for this field.
            $.each(settings.shs[fieldName], function(hash, setting) {
              fieldSettings = setting;
            });
            fieldSettings = $.extend({}, fieldSettings, settingsDefault, {
              fieldName: fieldName
            });
            var level = 0;
            var parent_id = 0;
            // Update class of wrapper element.
            $field.parent('.form-item').not('.shs-wrapper-processed').once('shs-wrapper');
            // Create elements for each parent of the current value.
            $.each(fieldSettings.parents, function(index, parent) {
              level++;
              // Create select element.
              $select = shsElementCreate($field.attr('id'), fieldSettings, level);
              if ($field.hasClass('error')) {
                // Add error-class if there was an error with the original field.
                $select.addClass('error');
              }
              // Add label to dropdown.
              $label = shsLabelCreate($field.attr('id'), fieldSettings, level);
              if ($label !== false) {
                $label.appendTo($field.parent());
              }
              $select.appendTo($field.parent());
              // Retrieve data for this level.
              getTermChildren($select, fieldSettings, parent_id, parent.tid, $field.attr('id'));
              // Use current term id as parent id for the next level.
              if (fieldSettings.multiple) {
                parent_id = parent['tid'];
              }
              else {
                parent_id = parent.tid;
              }
            });
            var addNextLevel = false;
            if ((level > 1 || parent_id) && ((fieldSettings.settings.create_new_terms && fieldSettings.settings.create_new_levels) || fieldSettings.settings.test_create_new_levels)) {
              // Add next level in hierarchy if new levels may be created.
              addNextLevel = true;
            }
            if (fieldSettings.default_value && (fieldSettings.default_value === parent_id) && (fieldSettings.default_value !== '')) {
              addNextLevel = true;
            }
            if (addNextLevel) {
              // Add label to dropdown.
              $label = shsLabelCreate($field.attr('id'), fieldSettings, level);
              if ($label !== false) {
                $label.appendTo($field.parent());
              }
              // Try to add one additional level.
              $select = shsElementCreate($field.attr('id'), fieldSettings, ++level);
              $select.appendTo($field.parent());
              // Retrieve data for this level.
              getTermChildren($select, fieldSettings, parent_id, 0, $field.attr('id'));
            }
          }
        });
    }
  }

  /**
   * Load direct children of a selected term.
   *
   * @param $element
   *   Element to fill with terms.
   * @param settings
   *   Field settings.
   * @param parent_value
    *   Value which has been selected in the parent element (== "selected term").
    * @param default_value
    *   Value to use as default.
    * @param base_id
    *   ID of original field which is rewritten as "taxonomy_shs".
    */
  getTermChildren = function($element, settings, parent_value, default_value, base_id) {
    // Check if parent_value is number and convert it.
    if (!$.isArray(parent_value) && typeof parent_value != "object") {
      parent_value = [parent_value];
    }

    $.ajax({
      url: Drupal.settings.basePath + '?q=' + Drupal.settings.pathPrefix + 'js/shs/json',
      type: 'POST',
      dataType: 'json',
      cache: true,
      data: {
        callback: 'shs_json_term_get_children',
        arguments: {
          vid: settings.vid,
          parent: parent_value,
          settings: settings.settings,
          field: settings.fieldName
        }
      },
      success: function(data) {
        if (data.success == true) {
          if ($element.prop) {
            var options = $element.prop('options');
          }
          else {
            var options = $element.attr('options');
          }

          if (((data.data.length == 0) || ((data.data.length == 1 && !data.data[0].tid))) && !(settings.settings.create_new_terms && (settings.settings.create_new_levels || (parent_value[0] == settings.any_value && default_value == 0)))) {
            // Remove element.
            $element.prev('label').remove();
            $element.remove();
            return;
          }

          // Remove all existing options.
          $('option', $element).remove();
          // Add empty option (if field is not required or this is not the
          // first level.
          if (!settings.settings.required || (settings.settings.required && (default_value === 0 || parent_value !== 0))) {
            options[options.length] = new Option(settings.any_label, settings.any_value);
          }

          // Add retrieved list of options.
          $.each(data.data, function(key, term) {
            if (term.vid && settings.settings.create_new_terms) {
              // Add option to add new item.
              options[options.length] = new Option(Drupal.t('<Add new item>', {}, {context: 'shs'}), '_add_new_');
            }
            else if (term.tid) {
              option = new Option(term.label, term.tid);
              options[options.length] = option;
              if (term.has_children) {
                option.setAttribute("class", "has-children");
              }
            }
          });
          // Set default value.
          $element.val(default_value);
          if (0 === default_value) {
            $element.val(settings.any_value);
          }

          // Try to convert the element to a "Chosen" element.
          if (!elementConvertToChosen($element, settings)) {
            // Display original dropdown element.
            $element.fadeIn(settings.display.animationSpeed);
            $element.css('display','inline-block');
          }
          else {
            $element.trigger('chosen:updated');
          }

          // If there is no data, the field is required and the user is allowed
          // to add new terms, trigger click on "Add new".
          if (data.data.length == 0 && settings.settings.required && settings.settings.create_new_terms && (settings.settings.create_new_levels || (parent_value[0] == settings.any_value && default_value == 0))) {
            updateElements($element, base_id, settings, 1);
          }
        }
      },
      error: function(xhr, status, error) {
      }
    });
  }

  /**
   * Add a new term to database.
   *
   * @param $triggering_element
   *   Element to add the new term to.
   * @param $container
   *   Container for "Add new" elements.
   * @param term
   *   The new term object.
   * @param base_id
   *   ID of original field which is rewritten as "taxonomy_shs".
   * @param level
   *   Current level in hierarchy.
   * @param settings
   *   Field settings.
   */
  termAddNew = function($triggering_element, $container, term, base_id, level, settings) {
    $.ajax({
      url: Drupal.settings.basePath + '?q=' + Drupal.settings.pathPrefix + 'js/shs/json',
      type: 'POST',
      dataType: 'json',
      cache: true,
      data: {
        callback: 'shs_json_term_add',
        arguments: {
          token: settings.token,
          vid: term.vid,
          parent: term.parent,
          name: term.name,
          field: settings.fieldName
        }
      },
      success: function(data) {
        if (data.success == true && data.data.tid) {
          if ($triggering_element.prop) {
            var options = $triggering_element.prop('options');
          }
          else {
            var options = $triggering_element.attr('options');
          }

          // Add new option with data from created term.
          options[options.length] = new Option(data.data.name, data.data.tid);
          // Set new default value.
          $triggering_element.val(data.data.tid);
          // Set value of original field.
          updateFieldValue($triggering_element, base_id, level, settings);
          // Add new child element if adding new levels is allowed.
          if (settings.settings.create_new_levels) {
            $element_new = shsElementCreate(base_id, settings, level + 1);
            $element_new.appendTo($triggering_element.parent());
            if ($element_new.prop) {
              var options_new = $element_new.prop('options');
            }
            else {
              var options_new = $element_new.attr('options');
            }
            // Add "none" option.
            options_new[options_new.length] = new Option(settings.any_label, settings.any_value);
            if (settings.settings.create_new_terms) {
              // Add option to add new item.
              options_new[options_new.length] = new Option(Drupal.t('<Add new item>', {}, {context: 'shs'}), '_add_new_');
            }
            // Try to convert the element to a "Chosen" element.
            if (!elementConvertToChosen($element_new, settings)) {
              // Display original dropdown element.
              $element_new.fadeIn(settings.display.animationSpeed);
              $element_new.css('display','inline-block');
            }
          }
        }
      },
      error: function(xhr, status, error) {
        // Reset value of triggering element.
        $triggering_element.val(0);
      },
      complete: function(xhr, status) {
        // Remove container.
        $container.prev('label').remove();
        $container.remove();
        // Display triggering element.
        $triggering_element.fadeIn(settings.display.animationSpeed);
        $triggering_element.css('display','inline-block');
        $triggering_element.trigger('change');
      }
    });
  }

  /**
   * Update the changed element.
   *
   * @param $triggering_element
   *   Element which has been changed.
   * @param base_id
   *   ID of original field which is rewritten as "taxonomy_shs".
   * @param settings
   *   Field settings.
   * @param level
   *   Current level in hierarchy.
   */
  updateElements = function($triggering_element, base_id, settings, level) {
    // Remove all following elements.
    $triggering_element.nextAll('select').each(function() {
      if (Drupal.settings.chosen) {
        // Remove element created by chosen.
        var elem_id = $(this).attr('id');
        $element_chosen = $('#' + elem_id.replace(/-/g, '_') + '_chosen');
        if ($element_chosen) {
          $element_chosen.prev('label').remove();
          $element_chosen.remove();
        }
      }
      // Remove element.
      $(this).prev('label').remove();
      $(this).remove();
    });
    $triggering_element.nextAll('.shs-term-add-new-wrapper').remove();
    // Create next level (if the value is != 0).
    if ($triggering_element.val() == '_add_new_') {
      // Hide element.
      $triggering_element.hide();
      if (Drupal.settings.chosen) {
        // Remove element created by chosen.
        var elem_id = $triggering_element.attr('id');
        $('#' + elem_id.replace(/-/g, '_') + '_chosen').remove();
      }
      // Create new container with textfield and buttons ("cancel", "save").
      $container = $('<div>')
        .addClass('shs-term-add-new-wrapper')
        .addClass('clearfix');
      // Append container to parent.
      $container.appendTo($triggering_element.parent());

      // Add textfield for term name.
      $nameField = $('<input type="text">')
        .attr('maxlength', 255)
        .attr('size', 10)
        .addClass('shs-term-name')
        .addClass('form-text');
      $nameField.appendTo($container);

      // Add buttons.
      $buttons = $('<div>')
        .addClass('buttons');
      $buttons.appendTo($container);
      $cancel = $('<a>')
        .attr('href', '#')
        .html(Drupal.t('Cancel'))
        .bind('click', function(event) {
          event.preventDefault();
          // Remove container.
          $container.prev('label').remove();
          $container.remove();
          // Reset value of triggering element.
          $triggering_element.val(settings.settings.any_value);

          if (!elementConvertToChosen($triggering_element, settings)) {
            // Display triggering element.
            $triggering_element.fadeIn(settings.display.animationSpeed);
            $triggering_element.css('display','inline-block');
          }
        });
      $cancel.appendTo($buttons);
      if (level == 1 && settings.settings.required && $('option', $triggering_element).length == 1) {
        // Hide cancel button since the term selection is empty (apart from
        // "Add new term") and the field is required.
        $cancel.hide();
      }

      $save = $('<a>')
        .attr('href', '#')
        .html(Drupal.t('Save'))
        .bind('click', function(event) {
          event.preventDefault();
          // Get the new term name.
          var termName = $(this).parents('.shs-term-add-new-wrapper').find('input.shs-term-name').val();
          // Create a term object.
          var term = {
            vid: settings.vid,
            parent: (level === 1) ? 0 : ($triggering_element.prevAll('.shs-select').val() || 0),
            name: termName
          };
          if (termName.length > 0) {
            termAddNew($triggering_element, $container, term, base_id, level, settings);
          }
          else {
            // Remove container.
            $container.prev('label').remove();
            $container.remove();
            // Reset value of triggering element.
            $triggering_element.val(0);
            // Display triggering element.
            $triggering_element.fadeIn(settings.display.animationSpeed);
            $triggering_element.css('display', 'inline-block');;
          }
        });
      $save.appendTo($buttons);
    }
    else if ($triggering_element.val() != 0 && $triggering_element.val() != settings.any_value) {
      level++;
      $label = shsLabelCreate(base_id, settings, level);
      if ($label !== false) {
        $label.appendTo($triggering_element.parent());
      }
      $element_new = shsElementCreate(base_id, settings, level);
      $element_new.appendTo($triggering_element.parent());
      // Retrieve list of items for the new element.
      getTermChildren($element_new, settings, $triggering_element.val(), 0, base_id);
    }

    // Set value of original field.
    updateFieldValue($triggering_element, base_id, level, settings);
  }

  /**
   * Create a new <select> element.
   *
   * @param base_id
   *   ID of original field which is rewritten as "taxonomy_shs".
   * @param settings
   *   Field settings.
   * @param level
   *   Current level in hierarchy.
   *
   * @return
   *   The new (empty) <select> element.
   */
  shsElementCreate = function(base_id, settings, level) {
    // Create element and initially hide it.
    $element = $('<select>')
      .attr('id', base_id + '-select-' + level)
      .addClass('shs-select')
      // Add core class to apply default styles to the element.
      .addClass('form-select')
      .addClass('shs-select-level-' + level)
      .bind('change', function() {
        updateElements($(this), base_id, settings, level);
      })
      .hide();
    if (settings.multiple) {
      $element.attr('multiple', 'multiple')
    }
    if (settings.settings.hasOwnProperty('required') && settings.settings.required) {
      $element.addClass('required');
    }
    // Return the new element.
    return $element;
  }

  /**
   * Create label for dropdown in hierarchy.
   *
   * @param base_id
   *   ID of original field which is rewritten as "taxonomy_shs".
   * @param settings
   *   Field settings.
   * @param level
   *   Current level in hierarchy.
   *
   * @return
   *   The new <label> element or false if no label should be created.
   */
  shsLabelCreate = function(base_id, settings, level) {
    var labelKey = level - 1;
    if (!settings.hasOwnProperty('labels')) {
      return false;
    }
    if (!settings.labels.hasOwnProperty(labelKey) || settings.labels[labelKey] === false) {
      return false;
    }
    // Create element.
    $element = $('<label>')
      .attr('for', base_id + '-select-' + level)
      .addClass('element-invisible')
      .html(settings.labels[labelKey]);
    // Return the new element.
    return $element;
  }

  /**
   * Update value of original (hidden) field.
   *
   * @param $triggering_element
   *   Element which has been changed.
   * @param base_id
   *   ID of original field which is rewritten as "taxonomy_shs".
   * @param level
   *   Current level in hierarchy.
   * @param settings
   *   Field settings.
   */
  updateFieldValue = function($triggering_element, base_id, level, settings) {
    // Reset value of original field.
    $field_orig = $('#' + base_id);
    $field_orig.val(settings.any_value);
    // Set original field value.
    if ($triggering_element.val() === settings.any_value || $triggering_element.val() == '_add_new_') {
      if ($triggering_element.prev('select').length) {
        // Use value from parent level.
        $field_orig.val($triggering_element.prev('select').val());
      }
    }
    else {
      var new_val = $triggering_element.val();
      if (level > 1 && settings.multiple) {
        var new_value = '';
        for (i = 0; i < level - 1; i++) {
          var prev_value = $('.shs-select:eq(' + i + ')').val();
          if (i == 0) {
            new_value = prev_value;
          }
          else {
            new_value = new_value + '+' + prev_value;
          }
        }
        new_val = new_value;
      }
      // Use value from current field.
      if ($.isArray(new_val)) {
        $field_orig.val(new_val.join(','));
      }
      else {
        if ($field_orig.children('option[value="' + new_val + '"]').length > 0) {
          // Value exists.
          $field_orig.val(new_val);
        }
        else {
          // We need to append the new option.
          if ($field_orig.prop) {
            var options = $field_orig.prop('options');
          }
          else {
            var options = $field_orig.attr('options');
          }
          options[options.length] = new Option(new_val, new_val);
          $field_orig.val(new_val);
        }
      }
    }
    // Notify listeners about the change in the original select.
    $field_orig.trigger({
      type: 'change',
      shs: {
        triggeringElement: $triggering_element,
        level: level,
        settings: settings,
        value: $triggering_element.val()
      }
    });
  }

  /**
   * Convert a dropdown to a "Chosen" element.
   *
   * @see http://drupal.org/project/chosen
   */
  elementConvertToChosen = function($element, settings) {
    // Returns false if chosen is not available or its settings are undefined.
    if ($.fn.chosen === void 0 || !Drupal.settings.hasOwnProperty('chosen') || Drupal.settings.chosen === void 0) {
      return false;
    }

    var name = $element.attr('name');
    settings.chosen = settings.chosen || Drupal.settings.chosen;
    var minWidth = settings.chosen.minimum_width;
    var multiple = Drupal.settings.chosen.multiple;
    var maxSelectedOptions = Drupal.settings.chosen.max_selected_options;

    // Define options.
    var options = {
      inherit_select_classes: true
    };

    var minimum = multiple && multiple[name] ? settings.chosen.minimum_multiple : settings.chosen.minimum_single;

    if (maxSelectedOptions && maxSelectedOptions[name]) {
      options.max_selected_options = maxSelectedOptions[name];
    }

    // Merges the user defined settings for chosen.
    options = $.extend(options, settings.chosen);

    // Get element selector from settings (and remove "visible" option since
    // our select element is hidden by default).
    var selector = settings.chosen.selector.replace(/:visible/, '');
    if ((settings.settings.use_chosen === 'always') || ((settings.settings.use_chosen === 'chosen') && $element.is(selector) && ($element.find('option').size() >= minimum || minimum === 'Always Apply'))) {
      options = $.extend(options, {
        width: (($element.width() < minWidth) ? minWidth : $element.width()) + 'px'
      });

      // Apply chosen to the element.
      return $element.chosen(options);
    }
    else if ((settings.settings.use_chosen === 'never') && (!$element.hasClass('chosen-disable'))) {
      // Tell chosen to not process this element.
      $element.addClass('chosen-disable');
    }

    return false;
  }

})(jQuery, Drupal);

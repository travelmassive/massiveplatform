/**
 * @file
 * Custom behaviors for Simple hierarchical select.
 */

(function ($) {

  /**
   * Creates the widget for Simple hierarchical select.
   */
  Drupal.behaviors.shsWidgetCreate = {

    // Default function to attach the behavior.
    attach: function (context, settings) {
      var self = this;
      $('input.shs-enabled')
        .not('.shs-processed')
        .once('shs')
        .addClass('element-invisible')
        .each(function() {
          var $field = $(this);
          var fieldName = $(this).attr('name');

          if (fieldName in settings.shs) {
            var fieldSettings = {};
            // Since we store the field settings within an associative array with
            // random strings as keys (reason: http://drupal.org/node/208611) we
            // need to get the last setting for this field.
            $.each(settings.shs[fieldName], function(hash, setting) {
              fieldSettings = setting;
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
            if ((level > 1 || parent_id) && (fieldSettings.settings.create_new_terms && fieldSettings.settings.create_new_levels)) {
              // Add next level in hierarchy if new levels may be created.
              addNextLevel = true;
            }
            if (fieldSettings.default_value && (fieldSettings.default_value.tid == parent_id)) {
              addNextLevel = true;
            }
            if (addNextLevel) {
              // Try to add one additional level.
              $select = shsElementCreate($field.attr('id'), fieldSettings, level);
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

    // Check if default_value is object and convert it.
    if (!$.isArray(default_value) && typeof default_value == "object") {
      var arr = new Array;
      $.each(default_value, function(delta, value){
        arr.push(value);
      });
      default_value = arr;
    }

    $.ajax({
      url: Drupal.settings.basePath + 'js/shs/json',
      type: 'POST',
      dataType: 'json',
      cache: true,
      data: {
        callback: 'shs_json_term_get_children',
        arguments: {
          vid: settings.vid,
          parent: parent_value,
          settings: settings.settings
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

          if (data.data.length == 0 && !(settings.settings.create_new_terms && (settings.settings.create_new_levels || (parent_value + default_value == 0)))) {
            // Remove element.
            $element.remove();
            return;
          }

          // Remove all existing options.
          $('option', $element).remove();
          // Add empty option (if field is not required and not multiple
          // or this is not the first level and not multiple).
          if (!settings.settings.required || (settings.settings.required && parent_value != 0 && !settings.multiple)) {
            options[options.length] = new Option(Drupal.t('- None -'), 0);
          }

          if (settings.settings.create_new_terms) {
            // Add option to add new item.
            options[options.length] = new Option(Drupal.t('<Add new item>', {}, {context: 'shs'}), '_add_new_');
          }

          // Add retrieved list of options.
          $.each(data.data, function(key, term) {
            options[options.length] = new Option(term.label, term.tid);
          });
          // Set default value.
          $element.val(default_value);

          // Try to convert the element to a "Chosen" element.
          if (!elementConvertToChosen($element, settings)) {
            // Display original dropdown element.
            $element.fadeIn();
            $element.css('display','inline-block');
          }

          // If there is no data, the field is required and the user is allowed
          // to add new terms, trigger click on "Add new".
          if (data.data.length == 0 && settings.settings.required && settings.settings.create_new_terms && (settings.settings.create_new_levels || (parent_value + default_value == 0))) {
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
   */
  termAddNew = function($triggering_element, $container, term, base_id, level) {
    $.ajax({
      url: Drupal.settings.basePath + 'js/shs/json',
      type: 'POST',
      dataType: 'json',
      cache: true,
      data: {
        callback: 'shs_json_term_add',
        arguments: {
          vid: term.vid,
          parent: term.parent,
          name: term.name
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
          updateFieldValue($triggering_element, base_id, level);
        }
      },
      error: function(xhr, status, error) {
        // Reset value of triggering element.
        $triggering_element.val(0);
      },
      complete: function(xhr, status) {
        // Remove container.
        $container.remove();
        // Display triggering element.
        $triggering_element.fadeIn();
        $triggering_element.css('display','inline-block');
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
        $('#' + elem_id.replace(/-/g, '_') + '_chzn').remove();
      }
      // Remove element.
      $(this).remove();
    });
    //$triggering_element.nextAll('.chzn-container').remove();
    $triggering_element.nextAll('.shs-term-add-new-wrapper').remove();
    // Create next level (if the value is != 0).
    if ($triggering_element.val() == '_add_new_') {
      // Hide element.
      $triggering_element.hide();
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
          $container.remove();
          // Reset value of triggering element.
          $triggering_element.val(0);
          // Display triggering element.
          $triggering_element.fadeIn();
          $triggering_element.css('display','inline-block');
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
            parent: $triggering_element.prev('select').val() || 0,
            name: termName
          };
          if (termName.length > 0) {
            termAddNew($triggering_element, $container, term, base_id, level);
          }
          else {
            // Remove container.
            $container.remove();
            // Reset value of triggering element.
            $triggering_element.val(0);
            // Display triggering element.
            $triggering_element.fadeIn();
            $triggering_element.css('display','inline-block');;
          }
        });
      $save.appendTo($buttons);
    }
    else if ($triggering_element.val() != 0) {
      level++;
      $element_new = shsElementCreate(base_id, settings, level);
      $element_new.appendTo($triggering_element.parent());
      // Retrieve list of items for the new element.
      getTermChildren($element_new, settings, $triggering_element.val(), 0, base_id);
    }

    // Set value of original field.
    updateFieldValue($triggering_element, base_id, level, settings.multiple);
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
    if (settings.multiple) {
      $element = $('<select>')
        .attr('id', base_id + '-select-' + level)
        .attr('multiple', 'multiple')
        .addClass('shs-select')
        // Add core class to apply default styles to the element.
        .addClass('form-select')
        .addClass('shs-select-level-' + level)
        .bind('change', function() {
          updateElements($(this), base_id, settings, level);
        })
        .hide();
    }
    else {
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
    }
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
   */
  updateFieldValue = function($triggering_element, base_id, level, multiple) {
    // Reset value of original field.
    $field_orig = $('#' + base_id);
    $field_orig.val(0);
    // Set original field value.
    if ($triggering_element.val() == 0 || $triggering_element.val() == '_add_new_') {
      if (level > 1) {
        // Use value from parent level.
        $field_orig.val($triggering_element.prev('select').val());
      }
    }
    else {
      var new_val = $triggering_element.val();
      if (level > 1 && multiple) {
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
        $field_orig.val(new_val);
      }
    }
  }

  /**
   * Convert a dropdown to a "Chosen" element.
   *
   * @see http://drupal.org/project/chosen
   */
  elementConvertToChosen = function($element, settings) {
    if (Drupal.settings.chosen) {
      var minWidth = Drupal.settings.chosen.minimum_width;
      // Define options for chosen.
      var options = {};
      options.search_contains = Drupal.settings.chosen.search_contains;
      options.placeholder_text_multiple = Drupal.settings.chosen.placeholder_text_multiple;
      options.placeholder_text_single = Drupal.settings.chosen.placeholder_text_single;
      options.no_results_text = Drupal.settings.chosen.no_results_text;

      // Get element selector from settings (and remove "visible" option since
      // our select element is hidden by default).
      var selector = Drupal.settings.chosen.selector.replace(/:visible/, '');

      if ((settings.settings.use_chosen == 'always') || ((settings.settings.use_chosen == 'chosen') && ($element.is(selector) && $element.find('option').size() >= Drupal.settings.chosen.minimum))) {
        $element.css({
          width : ($element.width() < minWidth) ? minWidth : $element.width()
        }).chosen(options);
        return true;
      }
    }
    return false;
  }

})(jQuery);

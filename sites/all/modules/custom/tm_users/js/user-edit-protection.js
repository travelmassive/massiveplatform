(function($) {

  // modification of node_edit_protection
  // see: https://www.drupal.org/project/node_edit_protection

  Drupal.node_edit_protection = {};
  var click = false; // Allow Submit/Edit button
  var edit = false; // Dirty form flag
  var profile_message = "Do you want to save your profile settings?";

  Drupal.behaviors.nodeEditProtection = {
    attach : function(context) {
      // If they leave an input field, assume they changed it.
      $("#user-profile-form :input").each(function() {
        $(this).blur(function() {
          edit = true;
        });
      });

      // Let all form submit buttons through
      $("#user-profile-form input[type='submit']").each(function() {
        $(this).addClass('user-edit-protection-processed');
        $(this).click(function() {
          click = true;
        });
      });

      $("a:not(.cancel), button, input[type='submit']:not(.user-edit-protection-processed)")
          .each(function() {
            $(this).click(function(event) {
              // return when a "#" link is clicked so as to skip the
              // window.onbeforeunload function
              if (edit && $(this).attr("href").indexOf("#") < 0) {
                var r = confirm(profile_message);
                if (r==true) {
                  event.preventDefault();
                  $("#edit-submit").click();
                } else {
                  window.location = $(this).attr('href');
                }
              }
            });
          });

    }
  };
})(jQuery);

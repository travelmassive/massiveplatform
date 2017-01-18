Description: 
Multiple fields remove button add functionality to remove unlimited input fields
from node form.

Configurations: 
Enable this module and select unlimited option in field form. 
Go to node form and you will see Remove button against each field for which you
were selected "unlimited" option.

For Developers:
You can implement this functionality to others fields which have not containing
remove button.
function hook_multiple_field_remove_button_field_widgets_alter(&$fieldwidgets) {
  // Add new widget type in $fieldwidgets
}
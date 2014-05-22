<?php
/**
 * Class ColorFieldRgbHandler
 *
 * Primary value is the hex code of the color, including the pound sign.
 * Example: '#000000'
 */
class ColorFieldRgbHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('color_field_rgb'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();
    if (isset($values['arguments'])) {
      $arguments = array_filter($values['arguments']);
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $return[$language][$delta] = array(
        'rgb' => $value,
      );
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}

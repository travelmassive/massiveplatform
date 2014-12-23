<?php
/**
 * @file tm_notifications.module
 * 
 * @author Daniel Silva (daniel.silva@flipside.org)
 * 
 */

/**
 * TMNotification class.
 * Used to manage the travel massive notifications.
 */
class TMNotification {
  
  /**
   * The notification name.
   */
  private $name;
  
  /**
   * The variable name for this notification.
   * Used to store the settings from the admin page.
   */
  private $variable_name = NULL;
  
  /**
   * Class constructor.
   */
  function __construct($notification_name) {
    $this->name = $notification_name;
    $this->variable_name = 'tm_notifications__' . $notification_name;
  }
  
  /**
   * Return the notification name.
   * 
   * @access public
   * @return String
   */
  public function getName() {
    return $this->name;
  }
  
  /**
   * Return the notification's variable name.
   * To be used as the field name in the admin form.
   * 
   * @access public
   * @return String
   */
  public function getVarName() {
    return $this->variable_name;
  }
  
  /**
   * Return the value of a given index stored in the variable
   * or the default value if the variable is not set.
   * 
   * @param $field
   *   The field to get.
   * @param $default
   *   The value to return if the varible is not set.
   * 
   * @access protected
   * @return Mixed
   */
  protected function getVarField($field, $default) {
    $data = variable_get($this->getVarName(), array());
    return isset($data[$field]) ? $data[$field] : $default;
  }
  
  /**
   * Returns the user friendly label for this notification.
   * It's a field set through the admin interface.
   * 
   * @param $default
   *   The value to return if the variable is not set.
   * 
   * @return String
   */
  public function getLabel($default = '') {
    return $this->getVarField('user_label', $default);
  }
  
  /**
   * Returns the email subject for this notification.
   * It's a field set through the admin interface.
   * 
   * @param $default
   *   The value to return if the variable is not set.
   * 
   * @return String
   */
  public function getEmailSubject($default = '') {
    $email_data = $this->getVarField('email', FALSE);
    return $email_data === FALSE ? $default : $email_data['subject'];
  }
  
  /**
   * Returns the email message for this notification.
   * It's a field set through the admin interface.
   * 
   * @param $default
   *   The value to return if the variable is not set.
   * 
   * @return String
   */
  public function getEmailMessage($default = '') {
    $email_data = $this->getVarField('email', FALSE);
    return $email_data === FALSE ? $default : $email_data['message'];
  }
  
  /**
   * Returns whether the user is allowed to change the status
   * of this notification.
   * It's a field set through the admin interface.
   * 
   * @param $default
   *   The value to return if the variable is not set.
   * 
   * @return Boolean
   */
  public function isChangeAllowed($default = TRUE) {
    return $this->getVarField('change_allowed', $default);
  }


  // get array of roles allowed to change the setting
  // allows to limit roles to chapter leader, moderator, etc.
  public function getChangeAllowedRoles($default = array('authenticated user')) {
    return $this->getVarField('change_allowed_roles', $default);
  }

  /**
   * Returns the default state for this notification.
   * It's a field set through the admin interface.
   * 
   * @param $default
   *   The value to return if the variable is not set.
   * 
   * @return Boolean
   */
  public function getDefaultState($default = 1) {
    return $this->getVarField('default_state', $default);
  }
  
  /**
   * Returns whether the notification is enabled for a given user.
   * If there is no value for the checking user, the default value
   * will be set and therefore returned.
   * 
   * @param $uid
   *   The user id.
   * 
   * @return Boolean
   */
  public function isEnabled($uid) {
    $result = db_query('SELECT value FROM {tm_notifications} WHERE bundle = :notification_name AND uid = :uid', array(
      ':notification_name' => $this->name,
      ':uid' => $uid
    ))->fetchField();
    
    if ($result === FALSE) {
      $default_state = (bool) $this->getDefaultState();
      $this->setForUser($uid, $default_state);
      return $default_state;
    }
    
    return $result == 1 ? TRUE : FALSE;
  }
  
  /**
   * Sets the status of this notification for a given user.
   * 
   * @param $uid
   *   The user id.
   * @param boolean $value
   *   The value to set.
   * 
   * @return self
   */
  public function setForUser($uid, $value) {
    $value = $value === TRUE ? 1 : 0;
  
    db_merge('tm_notifications')
    ->insertFields(array(
      'bundle' => $this->getName(),
      'uid' => $uid,
      'value' => $value,
    ))
    ->updateFields(array(
      'value' => $value,
    ))
    ->key(array('bundle' => $this->getName(), 'uid' => $uid))
    ->execute();
  
    return $this;
  }
  
  /**
   * Enables this notification for a given user.
   * 
   * @param $uid
   *   The user id.
   * 
   * @return self
   */
  public function enable($uid) {
    return $this->setForUser($uid, TRUE);
  }

  /**
   * Disables this notification for a given user.
   * 
   * @param $uid
   *   The user id.
   * 
   * @return self
   */
  public function disable($uid) {
    return $this->setForUser($uid, FALSE);
  }
  
}
<?php

// tm_users.register.inc - user registration methods

/**
 * Using Email Registration Form we automatically set the users email address
 * https://www.drupal.org/project/email_registration
 */
function tm_users_email_registration_name($edit, $account) {
  // Your hook implementation should ensure that the resulting string
  // works as a username. You can use email_registration_cleanup_username($name)
  // to clean up the name.
  //return email_registration_cleanup_username('u' . $account->uid);

  // Make username from first and last name and a digit
  //print_r($account);
  $first_name = $account->field_user_first_name['und'][0]['value'];
  $last_name = $account->field_user_last_name['und'][0]['value'];
  // remove spaces
  $first_name = str_replace(" ", "", $first_name);
  $last_name = str_replace(" ", "", $last_name);
  $username = email_registration_cleanup_username($first_name . $last_name); 

  // check if username exists
  if (!db_query("SELECT COUNT(*) FROM {users} WHERE name = :name;", array(':name' => $username))->fetchField()) {
    return $username;
  }

  // username exists so lets add some digits
  for ($i = 2; $i < 100; $i++) {
    $new_username = $username . $i;
    if (!db_query("SELECT COUNT(*) FROM {users} WHERE name = :name;", array(':name' => $new_username))->fetchField()) {
    return $new_username;
    }
  }

  // still haven't found a username, generate a really long random number
  // could still happen but this is enough work
  return $username . rand(100, 1000000);

}

/**
 * Modify the user registration form
 */
function _tm_users_modify_register_form(&$form, $form_state, $form_id) {

  // Step 1. Allow user to join up to chapter when they sign up

  // get a list of chapters
  $chapters = node_load_multiple(array(), array('type' => "chapter"));

  // create dropdown list
  $chapter_list = array();
  foreach ($chapters as $chapter) { // See this line in your original code
    $chapter_list[$chapter->nid] = $chapter->title; 
  }
  asort($chapter_list);
  $chapter_list = array("" => "Select a chapter", "-1" => "Choose later") + $chapter_list;
  
  // add chapter list to registraton form
  $form['chapter_list']=array(
    '#type'=>'select',
    '#title' => t('Home Chapter'),
    '#options' => $chapter_list,
    '#multiple' => false,
    '#description'=>'Please select a home chapter to join.',
    '#weight'=>9,
  );

  // set default chapter if set from cookie
  if (isset($_COOKIE['Drupal_visitor_chapter_referral_id'])) {
    $chapter_referral_id = $_COOKIE['Drupal_visitor_chapter_referral_id'];
    $form['chapter_list']['#default_value'] = $chapter_referral_id;
  }
  
  // Step 2. Add community connection to validate membership
  global $conf;
  $check_community_values_options = $conf["tm_community_values_options"];
  $check_community_values_options_validate = $conf["tm_community_values_options_validate"];
  $check_community_values_description = $conf["tm_community_values_description"];

  // Create divs for validation messages
  // These can be displayed in javascript via tm_community_values_show_validation_message
  foreach($check_community_values_options as $index => $val) {

    $validation_message = "";
    $validation_class = "";
    if (!$check_community_values_options_validate[$index]["valid"]) {
      $validation_message = $check_community_values_options_validate[$index]["message"];
      $validation_class = "validation_message-" . $index;
    }

    $check_community_values_options[$index] = $val . "<div style='display: none;' class='tm_community_values_validation_message " . $validation_class . "'>" . $validation_message . "</div>";
  }
  
  // Add how do you connect form
  $form['check_community_values_text']['active'] = array(
  '#type' => 'item',
  '#markup' => "<label>How do you connect with our community?</label>",
  '#weight'=>1,
  );  
  $form['check_community_values'] = array(
  '#type' => 'radios',
  '#options' => $check_community_values_options,
  '#element_validate' => array('_tm_users_community_validation_callback'),
  '#description' => $check_community_values_description . " See our " . l(t('Community Values'), $conf["tm_community_values_url"], array('attributes' => array('target' => '_blank'))) . ".",
  '#weight'=> 1,
  );    

  // Step 3. Add terms and conditions agreement to sign up form
  $form['cutomtext'] = array(
    '#type' => 'item',
    '#markup' => '<p class="description" style="text-align: center; margin-bottom: 12px;">By creating an account you agree to our <a target="_blank" href="/content/terms-use">Terms Of Use</a> and <a target = "_blank" href="/content/privacy-policy">Privacy Policy</a></p>',
    '#weight' => 10, // Adjust so that you can place it whereever 
  );

  // Step 4. Add reason for joining
  $form['reason_for_joining'] = array('#type' => 'textfield',
    '#title' => t('Why do you want to join?'),
    '#size' => 60,
    '#maxlength' => 150,
    '#required' => false,
    '#attributes' => array('placeholder' => 'I want to join because...'),
    '#description'=> 'In a few short words, tell us why you\'d like to join the community.',
    '#weight' => 2
  );
}

/**
 * Validate community connection option on signup page
 */
function _tm_users_community_validation_callback($element, &$form_state, $form) {

  global $conf;

  // check not using custom register page
  if (isset($conf["tm_users_custom_signup_page"])) {
    if ($conf["tm_users_custom_signup_page"]) {
      return;
    }
  }

  $validation = $conf["tm_community_values_options_validate"];

  // Check if form value was set
  if (!isset($form_state['values']['check_community_values'])) {
    form_set_error('check_community', "Please select how you connect with the community");
    return;
  }

  // Get item checked
  $item_checked = $form_state['values']['check_community_values'];

  // If not valid, use message in validation array
  if (!$validation[$item_checked]["valid"]) {
    form_set_error('check_community', $validation[$item_checked]["message"]);
  }
  
}

/**
 * Implement hook_user_insert()
 */
function tm_users_user_insert(&$edit, $account, $category) {

  // approve member on signup
  tm_users_approval_on_signup($account);

  // set home chapter
  tm_users_set_chapter_on_insert($edit, $account, $category);
}

/**
 * If user selected a home chapter on signup, then join it
 * todo: should check for a valid chapter id
 */
function tm_users_set_chapter_on_insert(&$edit, $account, $category) {

  // need chapter
  if (!isset($edit['chapter_list'])) {
    return;
  }

  $chapter_id = $edit['chapter_list'];
  if (($chapter_id != null) && ($chapter_id > 0)) {    
    
    // flag the user so they join the chapter
    $signup_flag = flag_get_flag('signup');
    if ($signup_flag) {
      if (!$signup_flag->is_flagged($chapter_id, $account->uid)) {
        flag('flag', 'signup', $chapter_id, $account);
      } 
    }
  }

}

/**
 * Implements hook_form_form_id_form_alter().
 */
function tm_users_form_user_register_form_alter(&$form, &$form_state, $form_id) {
  $form['#validate'][] = 'tm_users_user_register_validate_email';
}

/**
 * Checks if the domain in the email address is on a list of allowed domains.
 */
function tm_users_user_register_validate_email(&$form, &$form_state) {

  // Ignore validation if registration already has an error.
  $errors = form_get_errors();
  if (!empty($errors)) {
    return;
  }

  global $conf;
  $email = $form_state['values']['mail']; // username@example.com
  $email_parts = explode('@', $form_state['values']['mail']); // username, emample.com
  $username_parts = explode('+', $email_parts[0]); // username+label@example.com
  $email_without_plus = $username_parts[0] . "@" . $email_parts[1]; // username@example.com

  // check config
  if ((!isset($conf['tm_users_disallow_email_usernames'])) or (!isset($conf['tm_users_disallow_email_addresses']))) {
    return;
  }

  // email username start or ends with
  foreach ($conf["tm_users_disallow_email_usernames"] as $check_email) {

    // check username starts with
    if ((stripos($email_parts[0], $check_email) !== false) and (stripos($email_parts[0], $check_email) == 0)) {
      form_set_error('account', 'Please register with a personal email address.<br>Shared or company emails such as ' . $email . ' are not supported.');
      return;
    }

    // check username ends with
    if ((stripos($email_parts[0], $check_email) !== false) and (stripos($email_parts[0], $check_email) == (strlen($email_parts[0]) - strlen($check_email)))) {
      form_set_error('account', 'Please register with a personal email address.<br>Shared or company emails such as ' . $email . ' are not supported.');
      return;
    }
  }

  // entire email address
  foreach ($conf["tm_users_disallow_email_addresses"] as $check_email) {

    // check full email and without +
    if ((stripos($email, $check_email) !== false) or (stripos($email_without_plus, $check_email) !== false)) {
      form_set_error('account', 'Sorry, there was a problem creating your account. Please contact support.');
      return;
    }

  }
}


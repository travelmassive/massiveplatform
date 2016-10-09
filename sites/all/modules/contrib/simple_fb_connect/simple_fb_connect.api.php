<?php

/**
 * @file
 * Hooks provided by the Simple FB Connect module.
 */
/**
 * @addtogroup hooks
 * @{
 */

/**
 * This hook allows other modules to change $fields array before new user is created.
 *
 * @param $fields
 *   The fields array to be stored with user profile in user_save.
 *   Modify these values with values from $fb_user_profile to populate
 *   User Profile with values from FB while creating new user.
 *
 * @param $fb_user_profile
 *   Facebook GraphObject representing the user (response to "/me" API request)
 *   See: https://developers.facebook.com/docs/php/GraphObject/4.0.0
 *
 * @deprecated This function is deprecated.
 *   This hook is deprecated but has not been removed for backwards
 *   compatibility. Facebook changed their API in version 2.4 so that we have to
 *   explicitely list the fields that we want Facebook to return in the /me
 *   query (previously Facebook returned all user fields).
 *
 *   If you want to map Facebook fields to Drupal user fields, impelement either
 *   hook_simple_fb_connect_registration hook_simple_fb_connect_login and
 *   make call Facebook API for the desired field there. Example is provided in
 *   hook_simple_fb_connect_registration.
 */
function hook_simple_fb_connect_register_alter(&$fields, $fb_user_profile) {
  // Implement this hook in your own module to modify $fields array
}

/**
 * This hook allows other modules to add permissions to $scope array
 *
 * $scope[] = 'email' is added automatically by simple_fb_connect
 * Please note that if your app requires some additional permissions, you may
 * have to submit your Facebook App to Facebook Review process
 *
 * Read more about FB review process:
 * https://developers.facebook.com/docs/apps/review/login
 *
 * @param $scope
 *   The scope array listing the permissions requested by the app
 *
 * @return
 *   The updated scope array
 */
function hook_simple_fb_connect_scope_info($scope) {
  // Implement this hook in your own module to add items to $scope array
  return $scope;
}

/**
 * This hook allows other modules to act on Facebook login.
 *
 * There is also a Rules event triggered that can be used to react
 * on user login via Simple FB Connect.
 *
 * @param $drupal_user
 *   Drupal user that was just logged in via Simple FB Connect.
 */
function hook_simple_fb_connect_login($drupal_user) {
  // Implement this hook in your own module to act on Facebook login.

  // See hook_simple_fb_connect_registration for an example on how to make
  // additional queries to Facebook API.

  // If you modify the $drupal_user, remember to save it with user_save.
}

/**
 * This hook allows other modules to act on user creation via Facebook login.
 *
 * There is also a Rules event triggered that can be used to react
 * on user creation via Simple FB Connect.
 *
 * The example code shows how to make an additional query to Facebook API in
 * order to request user's first and last name and how to map these to
 * corresponding Drupal user fields.
 *
 * This example assumes that you have added fields "field_first_name" and
 * "field_last_name" to User entity at admin/config/people/accounts/fields and
 * that the length of these text fields is 255 characters.
 *
 * List of User fields on Facebook:
 * https://developers.facebook.com/docs/graph-api/reference/user
 *
 * @param $drupal_user
 *   Drupal user that was just created via Simple FB Connect.
 */
function hook_simple_fb_connect_registration($drupal_user) {
  // Implement this hook in your own module to act on user creation.
  // The code here is just an example.

  // Get FacebookSession for current user.
  $fb_session = simple_fb_connect_get_session();

  // Try to read first and last name from Facebook API.
  try {
    $request = new FacebookRequest($fb_session, 'GET', '/me?fields=first_name,last_name');
    $object = $request->execute()->getGraphObject();

    // Truncate Facebook values to 255 characters.
    $first_name = substr($object->getProperty('first_name'), 0, 255);
    $last_name = substr($object->getProperty('last_name'), 0, 255);

    // Save Facebook valuest to Drupal user
    $drupal_user->field_first_name[LANGUAGE_NONE][0]['value'] = $first_name;
    $drupal_user->field_last_name[LANGUAGE_NONE][0]['value'] = $last_name;

    // Save the user.
    user_save($drupal_user);
  }
  catch (FacebookRequestException $ex) {
    watchdog(
      'YOURMODULE',
      'Could not load fields from Facebook: FacebookRequestException. Error details: @message',
      array('@message' => json_encode($ex->getResponse())),
      WATCHDOG_ERROR
    );
  }
  catch (\Exception $ex) {
    watchdog(
      'YOURMODULE',
      'Could not load fields from Facebook: Unhandled exception. Error details: @message',
      array('@message' => $ex->getMessage()),
      WATCHDOG_ERROR
    );
  }
}

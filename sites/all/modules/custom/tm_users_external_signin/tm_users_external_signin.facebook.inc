<?php

// tm_users_external_signin.facebook.inc - Facebook signin methods

use Facebook\FacebookRequest;
use Facebook\FacebookRequestException;

/**
 * Implemet hook_simple_fb_connect_scope_info($scope)
 */
function tm_users_external_signin_simple_fb_connect_scope_info($scope) {
	$scope[] = "public_profile";
	$scope[] = "user_about_me";
	$scope[] = "user_location";
	$scope[] = "user_work_history";
	$scope[] = "user_website";
	return $scope;
}

/**
 * Fallback method in case FB graph fails
 * We need to save at least first and last name
 */
function tm_users_external_signin_fb_fallback_save_name($drupal_user, $fid) {

	$parts = explode(" ", $drupal_user->name);
	$drupal_user->field_user_first_name[LANGUAGE_NONE][0]['value'] = $parts[0];
	if (sizeof($parts) > 1) {
	  $drupal_user->field_user_last_name[LANGUAGE_NONE][0]['value'] = $parts[1];
	} else {
	  $drupal_user->field_user_last_name[LANGUAGE_NONE][0]['value'] = "Last Name";
	}

	$drupal_user->field_facebook_signin_id[LANGUAGE_NONE][0]['value'] = $fid;

	user_save($drupal_user);
}

/**
 * Implement hook_external_signin_simple_fb_connect_registration()
 */
function tm_users_external_signin_simple_fb_connect_registration($drupal_user) {

  tm_users_external_signin_facebook_sync($drupal_user, "register");

  // Join chapter
  // Since we don't ask for it on Facebook signup, use the most recently visited chapter
  if (isset($_COOKIE['Drupal_visitor_chapter_referral_id'])) {
   $chapter_referral_id = $_COOKIE['Drupal_visitor_chapter_referral_id'];
   $chapter = node_load($chapter_referral_id);
   if ($chapter) {
     if ($chapter->type == "chapter") {
       // flag the user so they join the chapter
       $signup_flag = flag_get_flag('signup');
       if (!$signup_flag->is_flagged($chapter->nid, $drupal_user->uid)) {
         flag('flag', 'signup', $chapter->nid, $drupal_user);
       }
     }
   }
  }

}

/**
 * Implements hook_simple_fb_connect_login($drupal_user)
 */
function tm_users_external_signin_simple_fb_connect_login($drupal_user) {
	global $conf;
	if ($conf["tm_signin_facebook_always_sync"]) {
		tm_users_external_signin_facebook_sync($drupal_user, "login");
	}
}

/**
 * Synchronise an account from Facebook
 * Method:
 * register = for new accounts
 * login = sync empty fields of existing account
 */
function tm_users_external_signin_facebook_sync($drupal_user, $method) {

  global $conf;
  
  // Get FacebookSession for current user.
  $fb_session = simple_fb_connect_get_session();
  $facebook_id = null;
  
  // Try to read first and last name from Facebook API.
  try {

  	$user_updated = false;

    $request = new FacebookRequest($fb_session, 'GET', '/me?fields=first_name,last_name,about,location,website,work,link,cover');
    $object = $request->execute()->getGraphObject();
    $obj_array = $request->execute()->getGraphObject()->asArray();

    // Get facebook id
    $facebook_id = $object->getProperty('id');
    if (!isset($drupal_user->field_facebook_signin_id[LANGUAGE_NONE][0]['value'])) {
      $user_updated = true;
      $drupal_user->field_facebook_signin_id[LANGUAGE_NONE][0]['value'] = $facebook_id;
    } elseif ($drupal_user->field_facebook_signin_id[LANGUAGE_NONE][0]['value'] != $facebook_id) {
      // updated value
      $user_updated = true;
      $drupal_user->field_facebook_signin_id[LANGUAGE_NONE][0]['value'] = $facebook_id;
    }

    // Link and id (for moderator)
    $facebook_link = substr($object->getProperty('link'), 0, 255);
    if (!isset($drupal_user->field_facebook_signin_link[LANGUAGE_NONE][0]['value'])) {
      $user_updated = true;
      $drupal_user->field_facebook_signin_link[LANGUAGE_NONE][0]['value'] = $facebook_link;
    } elseif ($drupal_user->field_facebook_signin_link[LANGUAGE_NONE][0]['value'] != $facebook_link) {
      // updated value
      $user_updated = true;
      $drupal_user->field_facebook_signin_link[LANGUAGE_NONE][0]['value'] = $facebook_link;
    }

    // First and last name
    if ($method == "register") {
    	$user_updated = true;
    	$first_name = substr($object->getProperty('first_name'), 0, 255);
    	$last_name = substr($object->getProperty('last_name'), 0, 255);
    	$drupal_user->field_user_first_name[LANGUAGE_NONE][0]['value'] = $first_name;
    	$drupal_user->field_user_last_name[LANGUAGE_NONE][0]['value'] = $last_name;
    }

    // About me (requires permission - user_about_me)
    if ($object->getProperty('about') != null) {
    	if (($method == "register") or ($method == "login" and !isset($drupal_user->field_about_me[LANGUAGE_NONE][0]['value']))) {
	    	$about_me = $object->getProperty('about');
	    	if (strlen($about_me) > 0) {
	    		$user_updated = true;
	    		$drupal_user->field_about_me[LANGUAGE_NONE][0]['value'] = $about_me;
	    	}
    	}
    }

    // Organization and job role (requires permission - work_history)
    if ($object->getProperty('work') != null) {

    	if (isset($obj_array['work'][0]->employer->name)) {
    		$job_organization = $obj_array['work'][0]->employer->name;
    		if (($method == "register") or ($method == "login" and !isset($drupal_user->field_job_organization[LANGUAGE_NONE][0]['value']))) {
    			$user_updated = true;
    			$drupal_user->field_job_organization[LANGUAGE_NONE][0]['value'] = $job_organization;
    		}
    	}

    	if (isset($obj_array['work'][0]->position->name)) {
    		if (($method == "register") or ($method == "login" and !isset($drupal_user->field_job_role[LANGUAGE_NONE][0]['value']))) {
    			$user_updated = true;
    			$job_role = $obj_array['work'][0]->position->name;
    			$drupal_user->field_job_role[LANGUAGE_NONE][0]['value'] = $job_role;
    		}
    	}
    }
    
    // Country Location (requires permission - user_location)
    if ($object->getProperty('location') != null) {

    	if (isset($obj_array['location']->name)) {
    		if (($method == "register") or ($method == "login" and !isset($drupal_user->field_location_city[LANGUAGE_NONE][0]['value']))) {
    			$user_updated = true;
    			$location = $obj_array['location']->name;
    			$drupal_user->field_location_city[LANGUAGE_NONE][0]['value'] = $location;
    		}
    	}

    	if (isset($obj_array['location']->id)) {
    		$location = tm_users_get_fb_location($fb_session, $obj_array['location']->id);
    		if (isset($location["location"]->country)) {
    			$country_name = $location["location"]->country;
    			$country_lookup = countries_country_lookup($country_name);
    			if ($country_lookup) {
    				if (($method == "register") or ($method == "login" and !isset($drupal_user->field_user_country[LANGUAGE_NONE][0]['iso2']))) {
    					$user_updated = true;
    					$drupal_user->field_user_country[LANGUAGE_NONE][0]['iso2'] = $country_lookup->iso2;
    				}
    			}
    		}
    	}
    }

    // Website (requires permission website)
    if ($object->getProperty('website') != "") {
      $parts = preg_split('/\s+/', $object->getProperty('website'));
      $first_website = $parts[0];
      $website = substr($first_website, 0, 255);
      if (strlen($website) > 0) {
        if (($method == "register") or ($method == "login" and !isset($drupal_user->field_link_website[LANGUAGE_NONE][0]['url']))) {
          $user_updated = true;
          $drupal_user->field_link_website[LANGUAGE_NONE][0]['url'] = $website;
        }
      }
    }

  	// Save Facebook profile picture
    if ($conf['tm_signin_facebook_sync_avatar']) {
      if (($method == "register") or ($method == "login" and (sizeof($drupal_user->field_avatar) == 0))) {
        $avatar_file = tm_users_get_fb_profile_pic($fb_session);
        if ($avatar_file) {
          $user_updated = true;
          $drupal_user->field_avatar = array(LANGUAGE_NONE => array('0' => (array)$avatar_file));
        }
      }
    }
  	
    // Save Facebook cover picture
    if ($conf['tm_signin_facebook_sync_cover']) {
      if (($method == "register") or ($method == "login" and (sizeof($drupal_user->field_image) == 0))) {
  	    if (isset($obj_array["cover"]->source)) {
          $cover_url = $obj_array["cover"]->source;
          $cover_file = tm_users_get_fb_cover_pic($fb_session, $cover_url);
          if ($cover_file) {
            $user_updated = true;
            $drupal_user->field_image = array(LANGUAGE_NONE => array('0' => (array)$cover_file));
          }
        }
      }
    }

    // Save the user.
    if ($user_updated) {
    	user_save($drupal_user);
    }
  }
  catch (FacebookRequestException $ex) {
    watchdog(
      'tm_users_external_signin',
      'Could not load fields from Facebook: FacebookRequestException. Error details: @message',
      array('@message' => json_encode($ex->getResponse())),
      WATCHDOG_ERROR
    );

    // fallback to saving first, last name if registering
    if ($method == "register") {
    	tm_users_external_signin_fb_fallback_save_name($drupal_user, $facebook_id);
    }

  }
  catch (\Exception $ex) {
    watchdog(
      'tm_users_external_signin',
      'Could not load fields from Facebook: Unhandled exception. Error details: @message',
      array('@message' => $ex->getMessage()),
      WATCHDOG_ERROR
    );

    // fallback to saving first, last name if registering
    if ($method == "register") {
    	tm_users_external_signin_fb_fallback_save_name($drupal_user, $facebook_id);
    }

  }

}

/**
 * Fetch location information
 */
function tm_users_get_fb_location(Facebook\FacebookSession $fb_session, $location_id) {

	// Call Graph API to request profile picture
	try {
	  $request = new FacebookRequest(
	  	$fb_session, 'GET', '/' . $location_id . '?fields=location');
	  $location = $request->execute()->getGraphObject()->asArray();
	  return $location;

	} catch (\Exception $ex) {
		return null;
	}

}

/**
 * Fetch and save user's profile picture
 * Adapted from simple_db_connect.module simple_fb_connect_get_fb_profile_pic()
 */
function tm_users_get_fb_profile_pic(Facebook\FacebookSession $fb_session) {      

  // Get desired dimensions from module settings
  $height = 800;

  // Check that target directory is writeable
  $picture_directory = 'public://facebook_avatars';
  if (!file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY)) {
    watchdog(
      'simple_fb_connect',
      'Could not save FB profile picture. Directory is not writeable: @picture_directory',
      array('@picture_directory' => $picture_directory),
      WATCHDOG_ERROR
    );
    return FALSE;
  }

  // Call Graph API to request profile picture
  try {
    $request = new FacebookRequest(
    	$fb_session, 'GET', '/me/picture?height=' . $height . '&type=square&redirect=false');
    $picture_url = $request->execute()->getGraphObject()->getProperty('url');

    // Save the picture locally. Use FB user_id as file name.
    $fb_user_id = $fb_session->getSessionInfo()->getProperty('user_id');
    $destination = file_default_scheme() . '://facebook_avatars/' . check_plain($fb_user_id) . '_picture.jpg';
    if ($file = system_retrieve_file($picture_url, $destination, TRUE, FILE_EXISTS_REPLACE)) {
      return $file;
    }
    else {
      watchdog(
        'simple_fb_connect',
        'Could not save FB profile picture. Check that directory is writeable: @destination',
        array('@destination' => $destination),
        WATCHDOG_ERROR
      );
    }
  }
  catch (FacebookRequestException $ex) {
    watchdog(
      'simple_fb_connect',
      'Could not load FB profile picture: FacebookRequestException. Error details: @message',
      array('@message' => json_encode($ex->getResponse())),
      WATCHDOG_ERROR
    );
  }
  catch (\Exception $ex) {
    watchdog(
      'simple_fb_connect',
      'Could not load FB profile picture: Unhandled exception. Error details: @message',
      array('@message' => $ex->getMessage()),
      WATCHDOG_ERROR
    );
  }

  // Something went wrong and the picture could not be loaded / saved.
  return FALSE;
}

/**
 * Fetch and save user's cover picture
 * Adapted from simple_db_connect.module simple_fb_connect_get_fb_profile_pic()
 */
function tm_users_get_fb_cover_pic(Facebook\FacebookSession $fb_session, $cover_url) {      

  // Check that target directory is writeable
  $picture_directory = 'public://facebook_covers';
  if (!file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY)) {
    watchdog(
      'simple_fb_connect',
      'Could not save FB profile picture. Directory is not writeable: @picture_directory',
      array('@picture_directory' => $picture_directory),
      WATCHDOG_ERROR
    );
    return FALSE;
  }
  
  // Save the cover photo locally. Use FB user_id as file name.
  $fb_user_id = $fb_session->getSessionInfo()->getProperty('user_id');
  $destination = file_default_scheme() . '://facebook_covers/' . check_plain($fb_user_id) . '_cover.jpg';
  if ($file = system_retrieve_file($cover_url, $destination, TRUE, FILE_EXISTS_REPLACE)) {
    return $file;
  }

  // Something went wrong and the picture could not be loaded / saved.
  return FALSE;
}

/**
 * Retrieve a user based on Facebook id
 */
function tm_users_get_user_by_fb_id($fid) {

	// get timestamp of users's most recent flagging action
	$query = "select entity_id from field_data_field_facebook_signin_id where field_facebook_signin_id_value = :fid and entity_type = 'user' order by entity_id desc limit 1";
	$result = db_query($query, array(":fid" => $fid))->fetch();

	// get user id of matching fb user
	if (isset($result->entity_id)) {
		$loaded = user_load($result->entity_id);
		return $loaded;
	}

	// no matching user
	return false;
}


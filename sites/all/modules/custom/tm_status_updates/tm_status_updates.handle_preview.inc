<?php

// tm_status_updates.link_preview.inc - methods to fetch link preview

/**
 * Get link preview data
 */
function tm_status_updates_get_preview_link($link_preview_id) {

  $query = "SELECT * FROM tm_newsfeed_preview_links WHERE id = :id";
  $result = db_query($query, array(':id' => $link_preview_id))->fetch(PDO::FETCH_ASSOC);
  return $result;
}

/**
 * Get how many preview links a user has generated within past 24 hours
 */
function tm_status_updates_get_preview_link_count($uid) {

  $query = "SELECT COUNT(*) as total_preview_links FROM tm_newsfeed_preview_links WHERE creator_uid = :creator_uid AND created >= DATE_SUB(NOW(), INTERVAL 1 DAY)";
  $result = db_query($query, array(':creator_uid' => $uid))->fetch();
  return $result->total_preview_links;
}

/**
 * Store link preview data
 */
function tm_status_updates_insert_preview_link($creator_uid, $link_url, $link_og_title, $link_og_description, $link_og_image, $image_file) {

  $query = "INSERT INTO tm_newsfeed_preview_links (creator_uid, link_url, link_og_title, link_og_description, link_og_image, image_file) VALUES (:creator_uid, :link_url, :link_og_title, :link_og_description, :link_og_image, :image_file)";
  $insert_id = db_query($query, array(':creator_uid' => $creator_uid, ':link_url' => $link_url, ':link_og_title' => $link_og_title, ':link_og_description' => $link_og_description, ':link_og_image' => $link_og_image, ':image_file' => $image_file), array('return' => Database::RETURN_INSERT_ID));

  return $insert_id;
}

/**
 * Check if user can preview an url
 */
function tm_status_updates_preview_link_check($account) {

  global $user;
  global $conf;

  // need to be logged in
  if (!user_is_logged_in()) {
    return false;
  }

  // check if status_updates are enabled
  if (!$conf["tm_status_updates_enabled"]) {
    return false;
  }

  // check its your uid
  if ($user->uid != $account->uid) {
    return false;
  }

  // need preview_url
  if ((!isset($_POST["preview_url"]) and (!isset($_GET["preview_url"])))) {
    return false;
  }

  return true;
}

/** 
 * Preview URL
 * tm_status_updates_check_user_preview_link should be called first to verify access
 * returns array. ie: array("result" => false, "error_message" => "something", "preview_link_id" => 123); 
 */
function tm_status_updates_preview_link_ajax() {

  global $conf;

  // Step 1. Validate

  // get posting uid from url
  // ie: /user/10527/post_update
  $uid = arg(1);

  // rate limiting on generating preview links
  $num_preview_links = tm_status_updates_get_preview_link_count($uid);
  if ($num_preview_links > $conf["tm_status_updates_preview_max_per_day"]) {
    $ret = array("result" => false, "error_message" => "Hey, slow down! Try again later.");
    drupal_json_output($ret);
    return;
  }

  // Load user posting update
  $posting_user = user_load($uid);
  if ($posting_user == false) {
    $ret = array("result" => false, "error_message" => "Unable to retrieve user account");
    drupal_json_output($ret);
    return;
  }

  // Get preview_url
  // ie: www.expedia.com
  $preview_url = null;
  if (isset($_GET["preview_url"])) {
    $preview_url = trim($_GET["preview_url"]);
  }
  if (isset($_POST["preview_url"])) {
    $preview_url = trim($_POST["preview_url"]);
  }
  if ($preview_url == null) {
   $ret = array("result" => false, "error_message" => "No preview_url provided");
   drupal_json_output($ret);
   return;
  }

  // Let's not preview ourselves...
  if (preg_match("!/user/.*/status\/preview_link!i", $preview_url)) {
    $ret = array("result" => false, "error_message" => "Invalid preview url");
    drupal_json_output($ret);
    return;
  }

  // Check if preview URL enabled
  if (!$conf["tm_status_updates_preview_enabled"]) {
    $ret = array("result" => false, "error_message" => "Preview not enabled");
    drupal_json_output($ret);
    return;
  }

  // Step 2. Fetch URL and parse opengraph data
  $preview_data = tm_status_updates_fetch_link_meta($preview_url);
  if ($preview_data == null) {
    // No preview data, but that's ok
    $ret = array("result" => true, "error_message" => "");
    drupal_json_output($ret);
    return;
  }

  // Step 3. Process opengraph image file
  $image_file = tm_status_updates_fetch_meta_image($preview_data["link_og_image"]);
  $preview_data["image_file"] = $image_file;

  // Step 4. Store preview data
  $preview_link_id = tm_status_updates_insert_preview_link($posting_user->uid, $preview_url, $preview_data["link_og_title"], $preview_data["link_og_description"], $preview_data["link_og_image"], $preview_data["image_file"]); 

  // Step 5. Get public URL from image file
  if ($preview_data["image_file"] != null) {
    $preview_data["image_file"] = file_create_url($preview_data["image_file"]);
  }

  // Step 6. Generate html preview
  $html_preview = tm_status_updates_render_link_preview($preview_data);

  // Step 7. Return preview data
  $ret = array("result" => true, "error_message" => "", "preview_link_id" => $preview_link_id, "preview_data" => $preview_data, "preview_html" => $html_preview);
  drupal_json_output($ret);
  return;
}

/**
 * Fetch URL meta data
 */
function tm_status_updates_fetch_link_meta($preview_url) {

  // Load Fusonic opengraph library on demand
  // https://github.com/fusonic/opengraph
  include_once 'includes/vendor/autoload.php';

  global $conf;
  $curl_timeout = $conf["tm_status_updates_preview_curl_timeout"];

  $preview_data = array();  
  $preview_data["link_url"] = $preview_url;
  $preview_data["link_og_title"] = null;
  $preview_data["link_og_description"] = null;
  $preview_data["link_og_image"] = null;

  // Fetch url preview
  try {

    // Create a new crawler
    $consumer = new Fusonic\OpenGraph\Consumer();
    $consumer->useFallbackMode = true;
    $object = $consumer->loadUrl($preview_url);
    //print_r($object);

    // title
    if (isset($object->title)) {
      $preview_data["link_og_title"] = $object->title;
    }

    // description
    if (isset($object->description)) {
      $preview_data["link_og_description"] = $object->description;
    }

    // image
    if (sizeof($object->images) > 0) {
      if (isset($object->images[0]->url)) {
        $preview_data["link_og_image"] = urldecode($object->images[0]->url);
      }
    }

  } catch (Exception $e) {
    return null;
  }
  
  return $preview_data;
}

/**
 * Fetch image URL, resize and store it
 * Returns public:// image file path
 * Returns null if there is an error fetching
 */
function tm_status_updates_fetch_meta_image($image_url) {

  global $conf;

  $image_path = $conf["tm_status_updates_preview_image_path"];
  $image_quality = $conf["tm_status_updates_preview_image_quality"];
  $max_width = $conf["tm_status_updates_preview_image_width"];
  $max_height = $conf["tm_status_updates_preview_image_height"];

  if ($image_url == null) {
    return null;
  }

  // handle non-protocal reference ie:
  // ie: img.travel.co.jp/images/common/ico_fb_200_200.png
  if (strpos($image_url, "//") == 0) {
    $image_url = "http:" . $image_url;
  }

  // Step 1. Fetch raw image file
  $raw_image = tm_status_updates_fetch_external_image($image_url);

  // Step 2. Resize image to maximum dimensions and store in public:// folder
  try {

    // generate unique file path to store image
    // ie: public://preview_link_images/123.jpg
    $image_file_path = $image_path . str_replace("/", "_", md5($image_url . time())) . ".jpg";
    
    // get image size
    $size = getimagesizefromstring($raw_image);

    // check image is not greater than limit to transform
    $max_pixel_limit = 8000;
    if (($size[0] > $max_pixel_limit) or ($size[1] > $max_pixel_limit)) {
      return null;
    }

    // calculate resized dimensions
    $ratio = $size[0] / $size[1]; // width/height
    if ($ratio > 1) {
        $width = $max_width;
        $height = $max_height / $ratio;
    } else {
        $width = $max_width * $ratio;
        $height = $max_height;
    }
    
    // source image to resize
    $src = imagecreatefromstring($raw_image);

    // set white background and resample image to new size
    $dst = imagecreatetruecolor($width, $height);
    $white = imagecolorallocate($dst, 255, 255, 255);
    imagefill($dst, 0, 0, $white);
    imagecopyresampled($dst, $src, 0, 0, 0, 0, $width, $height, $size[0], $size[1]);
    imagedestroy($src);

    // save image to file
    if (imagejpeg($dst, drupal_realpath($image_file_path), $image_quality) === true) {
      
      imagedestroy($dst);
      return $image_file_path;

    } else {
      
      // error saving file
      imagedestroy($dst);
      return null;
    }

  } catch (Exception $e) {
    return null;
  } 

  // Image retrieved
  return $image_url;
}

/**
 * Fetch image from external source
 */
function tm_status_updates_fetch_external_image($image_url) {

  global $conf;

  // timeout and download limit
  $curl_timeout = $conf["tm_status_updates_preview_curl_timeout"];
  $curl_max_mb = $conf["tm_status_updates_preview_curl_max_mb"];

  //open connection
  $ch = curl_init();

  //set the url
  $CURLOPT_SSL_VERIFYHOST = 2;
  curl_setopt($ch, CURLOPT_URL, $image_url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_VERBOSE, false);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
  curl_setopt($ch, CURLOPT_TIMEOUT, $curl_timeout); // timeout in seconds

  // We need progress updates to break the connection mid-way
  // http://stackoverflow.com/questions/17641073/how-to-set-a-maximum-size-limit-to-php-curl-downloads
  curl_setopt($cURL_Handle, CURLOPT_BUFFERSIZE, 128); // more progress info
  curl_setopt($cURL_Handle, CURLOPT_NOPROGRESS, false);
  curl_setopt($cURL_Handle, CURLOPT_PROGRESSFUNCTION, function(
      $DownloadSize, $Downloaded, $UploadSize, $Uploaded
  ) {
      // If $Downloaded exceeds $curl_max_mb MB, returning non-0 breaks the connection
      return ($Downloaded > ($curl_max_mb * 1024 * 1024)) ? 1 : 0;
  });

  // get the image file
  $image_file = curl_exec($ch);

  $curl_error = "";
  if ($result === false) {
    $curl_error = "CURL error: " . curl_error($ch);
    return null;
  }

  //close connection
  curl_close($ch);

  // return raw image
  return $image_file;
}


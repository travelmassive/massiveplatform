<?php
/**
 * @file
 * Integration layer to communicate with the Twitter REST API 1.1.
 * https://dev.twitter.com/docs/api/1.1
 *
 * Original work my James Walker (@walkah).
 * Upgraded to 1.1 by Juampy (@juampy72).
 * Custom Travelmassive by @danielfdsilva
 */

module_load_include('php', 'oauth_common', 'lib/OAuth');

/**
 * Exception handling class.
 */
class TwitterException extends Exception {}

/**
 * Primary Twitter API implementation class.
 */
class Twitter {
  /**
   * @var $source the twitter api 'source'
   */
  protected $source = 'drupal';

  protected $signature_method;

  protected $consumer;

  protected $token;


  /********************************************//**
   * Authentication
   ***********************************************/
  /**
   * Constructor for the Twitter class
   */
  public function __construct($consumer_key, $consumer_secret, $oauth_token = NULL,
                              $oauth_token_secret = NULL) {
    $this->signature_method = new OAuthSignatureMethod_HMAC_SHA1();
    $this->consumer = new OAuthConsumer($consumer_key, $consumer_secret);
    if (!empty($oauth_token) && !empty($oauth_token_secret)) {
      $this->token = new OAuthConsumer($oauth_token, $oauth_token_secret);
    }
  }

  public function get_request_token() {
    $url = variable_get('tm_twitter_api', TWITTER_API) . '/oauth/request_token';
    try {
      $params = array('oauth_callback' => url('tm_twitter/oauth/callback', array('absolute' => TRUE)));
      $response = $this->auth_request($url, $params);
    }
    catch (TwitterException $e) {
      watchdog('twitter', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }
    parse_str($response, $token);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  public function get_authorize_url($token) {
    $url = variable_get('tm_twitter_api', TWITTER_API) . '/oauth/authorize';
    $url.= '?oauth_token=' . $token['oauth_token'];

    return $url;
  }

  public function get_authenticate_url($token) {
    $url = variable_get('tm_twitter_api', TWITTER_API) . '/oauth/authenticate';
    $url.= '?oauth_token=' . $token['oauth_token'];

    return $url;
  }

  /**
   * Request an access token to the Twitter API.
   * @see https://dev.twitter.com/docs/auth/implementing-sign-twitter
   *
   * @param string$oauth_verifier
   *   String an access token to append to the request or NULL.
   * @return
   *   String the access token or FALSE when there was an error.
   */
  public function get_access_token($oauth_verifier = NULL) {
    $url = variable_get('tm_twitter_api', TWITTER_API) . '/oauth/access_token';

    // Adding parameter oauth_verifier to auth_request
    $parameters = array();
    if (!empty($oauth_verifier)) {
      $parameters['oauth_verifier'] = $oauth_verifier;
    }

    try {
      $response = $this->auth_request($url, $parameters);
    }
    catch (TwitterException $e) {
      watchdog('twitter', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }
    parse_str($response, $token);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  /**
   * Performs an authenticated request.
   */
  public function auth_request($url, $params = array(), $method = 'GET') {
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, $method, $url, $params);
    $request->sign_request($this->signature_method, $this->consumer, $this->token);
    switch ($method) {
      case 'GET':
        return $this->request($request->to_url());
      case 'POST':
        return $this->request($request->get_normalized_http_url(), $request->get_parameters(), 'POST');
    }
  }

  /**
   * Performs a request.
   *
   * @throws TwitterException
   */
  protected function request($url, $params = array(), $method = 'GET') {
    $data = '';
    if (count($params) > 0) {
      if ($method == 'GET') {
        $url .= '?'. http_build_query($params, '', '&');
      }
      else {
        $data = http_build_query($params, '', '&');
      }
    }

    $headers = array();

    $headers['Authorization'] = 'Oauth';
    $headers['Content-type'] = 'application/x-www-form-urlencoded';

    $response = $this->doRequest($url, $headers, $method, $data);
    if (!isset($response->error)) {
      return $response->data;
    }
    else {
      $error = $response->error;
      $data = $this->parse_response($response->data);
      if (isset($data['error'])) {
        $error = $data['error'];
      }
      throw new TwitterException($error);
    }
  }

  /**
   * Actually performs a request.
   *
   * This method can be easily overriden through inheritance.
   *
   * @param string $url
   *   The url of the endpoint.
   * @param array $headers
   *   Array of headers.
   * @param string $method
   *   The HTTP method to use (normally POST or GET).
   * @param array $data
   *   An array of parameters
   * @return
   *   stdClass response object.
   */
  protected function doRequest($url, $headers, $method, $data) {
    return drupal_http_request($url, array('headers' => $headers, 'method' => $method, 'data' => $data));
  }

  protected function parse_response($response) {
    // http://drupal.org/node/985544 - json_decode large integer issue
    $length = strlen(PHP_INT_MAX);
    $response = preg_replace('/"(id|in_reply_to_status_id)":(\d{' . $length . ',})/', '"\1":"\2"', $response);
    return json_decode($response, TRUE);
  }
  /**
   * Creates an API endpoint URL.
   *
   * @param string $path
   *   The path of the endpoint.
   * @param string $format
   *   The format of the endpoint to be appended at the end of the path.
   * @return
   *   The complete path to the endpoint.
   */
  protected function create_url($path, $format = '.json') {
    $url =  variable_get('tm_twitter_api', TWITTER_API) .'/1.1/'. $path . $format;
    return $url;
  }

  /********************************************//**
   * Utilities
   ***********************************************/
  /**
   * Calls a Twitter API endpoint.
   */
  public function call($path, $params = array(), $method = 'GET') {
    $url = $this->create_url($path);

    try {
      $response = $this->auth_request($url, $params, $method);
    }
    catch (TwitterException $e) {
      watchdog('twitter', '!message', array('!message' => $e->__toString()), WATCHDOG_ERROR);
      return FALSE;
    }

    if (!$response) {
      return FALSE;
    }

    return $this->parse_response($response);
  }
  
  /**
   * Get call to a Twitter API endpoint.
   */
  public function get($path, $params = array()) {
    return $this->call($path, $params);
  }
  
  /**
   * Post call to a Twitter API endpoint.
   */
  public function post($path, $params = array()) {
    return $this->call($path, $params, 'POST');
  }
  
  /**
   * Returns a variety of information about the user specified by the
   * required user_id or screen_name parameter.
   *
   * @param mixed $id
   *   The numeric id or screen name of a Twitter user.
   * @param bool $include_entities
   *   Whether to include entities or not.
   * @see https://dev.twitter.com/docs/api/1.1/get/users/show
   */
  public function users_show($id, $include_entities = NULL) {
    $params = array();
    if (is_numeric($id)) {
      $params['user_id'] = $id;
    }
    else {
      $params['screen_name'] = $id;
    }
    if ($include_entities !== NULL) {
      $params['include_entities'] = $include_entities;
    }
    $values = $this->call('users/show', $params, 'GET');
    return new TwitterUser($values);
  }
}

class TwitterUser {

  public $id;

  public $screen_name;

  public $name;

  public $location;

  public $description;

  public $followers_count;

  public $friends_count;

  public $statuses_count;

  public $favourites_count;

  public $url;

  public $protected;

  public $profile_image_url;

  public $profile_background_color;

  public $profile_text_color;

  public $profile_link_color;

  public $profile_sidebar_fill_color;

  public $profile_sidebar_border_color;

  public $profile_background_image_url;

  public $profile_background_tile;

  public $verified;

  public $created_at;

  public $created_time;

  public $utc_offset;

  public $status;

  protected $oauth_token;

  protected $oauth_token_secret;

  public function __construct($values = array()) {
    $this->id = $values['id'];
    $this->screen_name = $values['screen_name'];
    $this->name = $values['name'];
    $this->location = $values['location'];
    $this->description = $values['description'];
    $this->url = $values['url'];
    $this->followers_count = $values['followers_count'];
    $this->friends_count = $values['friends_count'];
    $this->statuses_count = $values['statuses_count'];
    $this->favourites_count = $values['favourites_count'];
    $this->protected = $values['protected'];
    $this->profile_image_url = $values['profile_image_url'];
    $this->profile_background_color = $values['profile_background_color'];
    $this->profile_text_color = $values['profile_text_color'];
    $this->profile_link_color = $values['profile_link_color'];
    $this->profile_sidebar_fill_color = $values['profile_sidebar_fill_color'];
    $this->profile_sidebar_border_color = $values['profile_sidebar_border_color'];
    $this->profile_background_image_url = $values['profile_background_image_url'];
    $this->profile_background_tile = $values['profile_background_tile'];
    $this->verified = $values['verified'];
    $this->created_at = $values['created_at'];
    if (!empty($values['uid'])) {
      $this->uid = $values['uid'];
    }
    if (!empty($values['created_at']) && $created_time = strtotime($values['created_at'])) {
      $this->created_time = $created_time;
    }
    $this->utc_offset = $values['utc_offset']?$values['utc_offset']:0;
/*
    if (isset($values['status'])) {
      $this->status = new TwitterStatus($values['status']);
    }
*/
  }

  /**
   * Returns an array with the authentication tokens.
   *
   * @return
   *   array with the oauth token key and secret.
   */
  public function get_auth() {
    return array('oauth_token' => $this->oauth_token, 'oauth_token_secret' => $this->oauth_token_secret);
  }

  /**
   * Sets the authentication tokens to a user.
   *
   * @param array $values
   *   Array with 'oauth_token' and 'oauth_token_secret' keys.
   */
  public function set_auth($values) {
    $this->oauth_token = isset($values['oauth_token'])?$values['oauth_token']:NULL;
    $this->oauth_token_secret = isset($values['oauth_token_secret'])?$values['oauth_token_secret']:NULL;
  }

  /**
   * Checks whether the account is authenticated or not.
   *
   * @return
   *   boolean TRUE when the account is authenticated.
   */
  public function is_auth() {
    return !empty($this->oauth_token) && !empty($this->oauth_token_secret);
  }
}

<?php

// tm_newsfeed.email.inc - mail methods
// to produce email newsletter/notifications

// https://dev.travelmassive.com/jobs/api/search?country=united+states


/**
 * Approval callback for test callback
 */
function tm_newsfeed_email_callback() {

	// need to be logged in
	if (user_is_logged_in()) {
		return true;
	}

	return true;
}


/**
 * Newsfeed render
 */

function tm_newsfeed_email() {

	global $debug_timer;

	$debug_timer['01start'] = time();

	// params
	// $user_ids = array(29540,29538,29537,29536,29535,29534,29533,29532,29531,29530,29529,29528,29527,29526,29525,29524,29523,29522,29521,29520,29519,29518,29517,29516,29515,29514,29513,29512,29511,29510,29509,29508,29507,29506,29505,29504,29503,29502,29501,29500,29499,29498,29497,29496,29495,29494,29493,29492,29491,29490,29489,29488,29487,29486,29485,29484,29483,29482,29481,29480,29479,29478,29477,29476,29475,29474,29473,29472,29471,29470,29468,29467,29466,29465,29464,29463,29462,29461,29460,29459,29458,29457,29456,29455,29454,29453,29452,29451,29450,29449,29447,29446,29445,29444,29443,29442,29441,29440,29439,29438);
	$user_ids = array(29540,29538,29537,29536,29535);
	// $user_ids = array(29540);

	$start_date = new DateTime('2016-05-01');
	$end_date = new DateTime();
	$start_date = $start_date->format('Y-m-d H:i:s');
	$end_date = $end_date->format('Y-m-d H:i:s');
	// $start_date = new DateTime('-2 week');
	// $end_date = new DateTime('-1 week');
	// $start_date = new DateTime('2015-04-24');
	// $end_date = new DateTime('2017-04-30');

	$response = "";

	for ($i=0; $i < count($user_ids); $i++) {
		$user_id = $user_ids[$i];
		$str = tm_newsfeed_generate_email($user_id, $start_date, $end_date);
		$response = $response . $str;
	}

	$debug_timer['99end'] = time();
	$debug_timer['total'] = $debug_timer['99end'] - $debug_timer['01start'];

	var_dump('<pre>'.'start_date: '. $start_date .'<br>'.'  end_date: ' . $end_date.'</pre>');
	var_dump('<pre>');
	var_dump($debug_timer);
	var_dump('</pre>');

	return $response;
}


function tm_newsfeed_generate_email($user_id, $start_date, $end_date) {

	global $conf;

	// process inputs
	$account = user_load($user_id);
	$user_name = $account->field_user_first_name[LANGUAGE_NONE][0]['value'];
	$search_country = tm_newsfeed_get_search_country($account);

	// start building email
	$email = TmNewsfeedEmailTemplates::$tm_newsfeed_email_template;

	$greeting = TmNewsfeedEmailTemplates::$tm_newsfeed_email_greetings[
		array_rand(TmNewsfeedEmailTemplates::$tm_newsfeed_email_greetings)];
	$email = str_replace("__GREETING__", $greeting, $email);

	$email = str_replace("__USER_FIRST_NAME__", $user_name, $email);

	$opening_line = TmNewsfeedEmailTemplates::$tm_newsfeed_email_opening_lines[
		array_rand(TmNewsfeedEmailTemplates::$tm_newsfeed_email_opening_lines)];
	$email = str_replace("__OPENING_LINE__", $opening_line, $email);

	$tip = tm_newsfeed_email_get_tip($account, $start_date, $end_date);
	$email = str_replace("__EMAIL_TIP__", $tip, $email);

	$content = tm_newsfeed_email_populate_section_content($user_id, $search_country, $start_date, $end_date);
	$email = str_replace("__EMAIL_CONTENT__", $content, $email);

	$quote = TmNewsfeedEmailTemplates::$tm_newsfeed_email_quotes[
		array_rand(TmNewsfeedEmailTemplates::$tm_newsfeed_email_quotes)];
	$email = str_replace("__QUOTE_OF_THE_DAY__", $quote['text'], $email);
	$email = str_replace("__QUOTE_OF_THE_DAY_AUTHOR__", $quote['author'], $email);

	$email = str_replace("__LET_US_KNOW_FEEDBACK__", "Feedback? Suggestions? <a href='".$tm_domain."/contact'>Let us know</a>", $email);

	$email = str_replace("__THANK_YOU_PHRASE__", "Cheers", $email);
	$email = str_replace("__PLATFORM_NAME__", $conf['tm_site_name'], $email);

	// strip all exclamation marks as they trigger spam filters
	$email = str_replace("!", "", $email);

	return $email;
}


function tm_newsfeed_email_get_tip($account, $start_date, $end_date) {

	$uid = $account->uid;
  $edit_url = "user/" . $uid . "/edit";


	// important tips
	// these will always be shown in order of importance if applicable

  if (!in_array("approved user", $account->roles)){
  	return "[Tip: you aren't approved yet text TODO]";
  }


  // 
	// [Tip: Create a free company profile for Rock Climb Travel and join more than 1,300 other companies on our network.]
  $field_job_organization = $account->field_job_organization;
  if (sizeof($field_job_organization) == 0) {
    "Share the organization " . l(t('where you work'), $edit_url, array('fragment' => 'edit-field-job-organization')) . ", except if you are a top secret agent.";
	}


  // cycle tips
  // create an array of other tips that apply to the user
  // and select a random one for inclusion in the email
  $tips = array();

  if (sizeof($account->field_about_me) == 0) {
    $tips[] = "Share a little bit " . l(t('about yourself'), $edit_url, array('fragment' => 'edit-field-about-me')) . " and your story with the community.";
  }

  if ((sizeof($account->field_location_city) == 0) or (sizeof($account->field_user_country) == 0)) {
		$tips[] = "Tell us " . l(t('where you are located'), $edit_url, array('fragment' => 'edit-field-location-city')) . " so other members can discover you.";
  }

  if (sizeof($account->field_job_role) == 0) {
    $tips[] = "Share your " . l(t('job role'), $edit_url, array('fragment' => 'edit-field-job-role')) . " so like-minded members can connect with you.";
  }


  // select random tip
  if (sizeof($tips) > 0) {
		$rand = $tips[array_rand($tips)];
		$tip = "[Tip: $rand]";
  } else {
  	// general fall back tips if no other tips were selected based on user conditions
  	$tip = "[Tip: ensure your profile is up to date if you relocate so that you can meet other people in the Travel Massive community]";
  }

	return $tip;
}

function tm_newsfeed_email_populate_section_content($user_id, $search_country, $start_date, $end_date) {
	
	global $conf;
	global $debug_timer;

	$content = "";

	// note: this is startdate for testing
	//       TODO: it should be end_date, or the current date, in order to get upcoming events
	$debug_timer['11_start_events'] = time();
	$content = $content . tm_newsfeed_email_populate_events($user_id, $start_date);

	$debug_timer['12_start_updates'] = time();
	$content = $content . tm_newsfeed_email_populate_updates($user_id, $start_date, $end_date);

	$debug_timer['13_start_new_members'] = time();
	$content = $content . tm_newsfeed_email_populate_new_members($user_id, $start_date, $end_date);

	$debug_timer['14_start_new_companies'] = time();
	$content = $content . tm_newsfeed_email_populate_new_companies($user_id, $start_date, $end_date);

	if ($conf['tm_newsfeed_marketplace_enabled']) {
		$debug_timer['15_start_jobs'] = time();
		$content = $content . tm_newsfeed_email_populate_jobs($search_country, $start_date, $end_date);
	}

	if ($conf['tm_newsfeed_discuss_enabled']) {
		$debug_timer['16_start_discussions'] = time();
		$content = $content . tm_newsfeed_email_populate_discussions($user_id);
	}
	
	if ($conf['tm_newsfeed_blog_enabled']) {
		$debug_timer['17_start_blog_posts'] = time();
		$content = $content . tm_newsfeed_email_populate_blog_posts($user_id, $start_date, $end_date);
		# code...
	}

	$debug_timer['20_done_content'] = time();

	return $content;
}

function tm_newsfeed_email_populate_updates($user_id, $start_date, $end_date) {
	global $conf;

	$items = array();
	$results = tm_newsfeed_get_updates($user_id, $start_date, $end_date);

	if (count($results) < 1) return "";

	foreach($results as $result) {
		$href = "";
		if ($result->entity_type == 'user') {
			$href = $tm_domain . "/user/" . $result->entity_id;
		} elseif ($result->entity_type == 'chapter') {
			$href = $tm_domain . "/node/" . $result->entity_id;
		} else {
			$href = $tm_domain . "/node/" . $result->entity_id;
		}

		$link = "";
		if ($result->entity_type == 'user') {
			$link = $result->entity_name;
		} elseif ($result->entity_type == 'chapter') {
			// add site name to chapter link text, but only if it doesn't already exist
			if (strpos(strtolower($result->entity_name), strtolower($conf['tm_site_name'])) !== sample) {
				$link = $result->entity_name;;
			} else {
				$link = $result->entity_name . ' Travel Massive';
			}
		} else {
			$link = $result->entity_name;
		}

		$time = time_elapsed_string($result->created);
		$text = " posted an update $time";

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('updates', $items);
}


function tm_newsfeed_email_populate_events($user_id, $from_date) {
	// note that for the email, we want events taking place, from the time of the email in to the future
	$items = array();
	$results = tm_newsfeed_get_events($user_id, $from_date);

	if (count($results) < 1) return "";

	// sort results purely by date; query will be sorted by relevance columns first
	usort($results, 'sort_event_date');

	foreach($results as $result) {
		$href = $tm_domain . "/node/" . $result->event_id;
		$link = $result->event_title;
		$text = ', '. date_format(new DateTime($result->event_date), 'l, F j');

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('events', $items);
}


function tm_newsfeed_email_populate_new_members($user_id, $start_date, $end_date) {
	$items = array();
	$results = tm_newsfeed_get_new_members($user_id, $start_date, $end_date);

	if (count($results) < 1) return "";

	foreach($results as $result) {
		$href = $tm_domain . "/user/" . $result->uid;
		$link = $result->user_full_name;

		$home_chapter_url = $tm_domain.'/node/'.$result->home_chapter_id;
		$home_chapter = "<a href='$home_chapter_url'>".$result->home_chapter."</a>";
		$num_signups = $result->signup_count;

		if ($num_signups > 2) {
			$text = " + ".($num_signups-1)." other people joined $home_chapter";
		} elseif ($num_signups == 2) {
			$text = " + 1 other person joined $home_chapter";
		} elseif ($num_signups == 1) {
			$text = " joined $home_chapter";
		} else {
			$text = " joined Travel Massive";
		}

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('new_members', $items);
}


function tm_newsfeed_email_populate_new_companies($user_id, $start_date, $end_date) {
	$items = array();
	$results = tm_newsfeed_get_new_companies($user_id, $start_date, $end_date);

	if (count($results) < 1) return "";

	foreach($results as $result) {
		$href = $tm_domain . "/node/" . $result->nid;
		$link = $result->title;
		$text = ", ".$result->category;

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('new_companies', $items);
}


function tm_newsfeed_email_populate_jobs($search_country, $start_date, $end_date) {
	$items = array();

	$results = array();

	try {
		$results = tm_newsfeed_marketplace_fetch($search_country);
	}
	catch(Exception $e) {
		return "<p><b>TESTING: Error with Newsfeed Jobs Fetch</b></p>";
	}

	if (count($results) < 1) return "";

	// process results
	usort($results, 'sort_seconds_ago');
	$results = array_slice($results, 0, 3);

	foreach($results as $result) {
		$href = $url;
		$link = $result->title;
		$text = ", ".$result->location;

		// add featured
		if ($result->is_featured) {
			$text = " (Featured)" . $text;
		}

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('jobs', $items);
}


function tm_newsfeed_email_populate_discussions($user_id) {
	$items = array();
	$results = tm_newsfeed_discuss_fetch_for_user($user_id);

	if (count($results) < 1) return "";

	// process results
	// usort($results, 'sort_discussions');
	$results = array_slice($results, 0, 3);

	foreach($results as $result) {
		$href = $result->url;
		$link = $result->discussion_name;

		$text = "";
		$num_replies = $result->count_comments;

		if ($num_replies > 1) {
			$text = " ($num_replies replies)";
		} elseif ($num_replies == 1) {
			$text = " (1 reply)";
		} else {
			$text = " (Be the first to reply)";
		}

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('discussions', $items);
}


function tm_newsfeed_email_populate_blog_posts($user_id, $start_date, $end_date) {
	$items = array();
	$results = tm_newsfeed_blog_fetch();

	if (count($results) < 1) return "";

	// only add first 3
	for ($i=0; $i < 3; $i++) { 
		$result = $results[$i];

		$href = $result->url;
		$link = $result->title;
		$text = "";

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('blog_posts', $items);
}



function tm_newsfeed_email_populate_section($section_name, $items) {

	$section = TmNewsfeedEmailTemplates::$tm_newsfeed_email_section;
	$section_contents = "";

	foreach($items as $item) {
		$row = TmNewsfeedEmailTemplates::$tm_newsfeed_email_section_row;
		$row = str_replace("__EMAIL_ITEM_LINK__", $item['href'], $row);
		$row = str_replace("__EMAIL_ITEM_LINK_TEXT__", $item['link'], $row);
		$row = str_replace("__EMAIL_ITEM_EXTRA_TEXT__", $item['text'], $row);
		$section_contents = $section_contents . $row;
	}

	$section = str_replace("__EMAIL_SECTION_HEADER__", TmNewsfeedEmailTemplates::$tm_newsfeed_email_section_headers[$section_name], $section);
	$section = str_replace("__EMAIL_SECTION_ROWS__", $section_contents, $section);

	return $section;
}




class TmNewsfeedEmailTemplates
{

	/* Templates */
	public static $tm_newsfeed_email_template = <<<EOT
<hr>
<div class="email-intro">
	<p>__GREETING__, __USER_FIRST_NAME__</p>
	<p>__OPENING_LINE__</p>
</div>
<div class="email-tip">
	<p><em>__EMAIL_TIP__</em></p>
</div>
<div class="email-content">
	__EMAIL_CONTENT__
</div>
<div class="email-end">
	<p>Quote of the day:<br>
	"__QUOTE_OF_THE_DAY__" – <em>__QUOTE_OF_THE_DAY_AUTHOR__</em>
	</p>
	<p>__LET_US_KNOW_FEEDBACK__</p>
	<p>__THANK_YOU_PHRASE__,<br>The __PLATFORM_NAME__ Team</p>
</div>
EOT;

	public static $tm_newsfeed_email_section = <<<EOT
<div class="email-section">
	<strong>__EMAIL_SECTION_HEADER__</strong><br>
	__EMAIL_SECTION_ROWS__
</div>
<br>
EOT;

	public static $tm_newsfeed_email_section_row = <<<EOT
<a href="__EMAIL_ITEM_LINK__">__EMAIL_ITEM_LINK_TEXT__</a>__EMAIL_ITEM_EXTRA_TEXT__<br>
EOT;

	public static $tm_newsfeed_css = <<<EOT
<style>
</style>
EOT;


	/* Lookups */
	public static $tm_newsfeed_email_section_headers = array(
		'blog_posts' => 'New blog posts',
		'discussions' => 'Trending discussions',
		'jobs' => 'Jobs and opportunities',
		'new_members' => 'New members in your network',
		'new_companies' => 'New companies in your network',
		'updates' => 'Latest status updates',
		'events' => 'Upcoming events'
		);


	public static $tm_newsfeed_email_greetings = array(
		"Hello",
		"Hey",
		"Hola",
		"Konnichiwa"
		);

	public static $tm_newsfeed_email_opening_lines = array(
		"Here’s what’s been happening at Travel Massive this week.",
		"Check out what’s been happening at Travel Massive this week."
		);

	public static $tm_newsfeed_email_quotes = array(
		array("text" => "Travel makes one modest, you see what a tiny place you occupy in the world.",
					"author" => "Gustave Flaubert"),
		array("text" => "Oh the places you’ll go!",
					"author" => "Dr. Seuss"),
		array("text" => "Investment in travel is an investment in yourself.",
					"author" => "Matthew Karsten")
		);
}



// helpers

function time_elapsed_string($datetime, $full = false) {
  $now = new DateTime;
  $ago = new DateTime($datetime);
  $diff = $now->diff($ago);

  $diff->w = floor($diff->d / 7);
  $diff->d -= $diff->w * 7;

  $string = array(
    'y' => 'year',
    'm' => 'month',
    'w' => 'week',
    'd' => 'day',
    'h' => 'hour',
    'i' => 'minute',
    's' => 'second',
  );
  foreach ($string as $k => &$v) {
    if ($diff->$k) {
      $v = $diff->$k . ' ' . $v . ($diff->$k > 1 ? 's' : '');
    } else {
      unset($string[$k]);
    }
  }

  if (!$full) $string = array_slice($string, 0, 1);
  return $string ? implode(', ', $string) . ' ago' : 'just now';
}

// use to sort an array of objects by 'seconds_ago' attribute
function sort_seconds_ago($a, $b)
{
  return strcmp($a->seconds_ago, $b->seconds_ago);
}

// use to sort an array of objects returned from dicussions fetch
function sort_discussions($a, $b)
{
  return strcmp($a->seconds_ago, $b->seconds_ago);
}

// use to sort an array of objects by 'seconds_ago' attribute
function sort_event_date($a, $b)
{
  return strcmp($a->event_date, $b->event_date);
}


function tm_newsfeed_get_search_country($account)
{
	$user_home_chapter = $account->field_home_chapter[LANGUAGE_NONE][0]['target_id'];
	$user_country = $account->field_user_country[LANGUAGE_NONE][0]['iso2'];

	// if country in profile is set, use that
	// if home chapter is set, use country of home chapter
	// otherwise, show everything
	$search_country = "";  // shows everything

	if (is_null($user_country)) {
		if (is_null($user_home_chapter)) {
			// use ""
		} else {
			$chapter = node_load($user_home_chapter);
			$country_code = $chapter->field_country[LANGUAGE_NONE][0]['iso2'];
			$search_country = country_load($country_code)->name;
		}
	} else {
		$search_country = $user_country;
	}

	return $search_country;
}

// this probably already exists somewhere!
global $tm_domain;
$tm_domain = "http://localdev.massiveplatform.com";


global $debug_timer;
$debug_timer = array();

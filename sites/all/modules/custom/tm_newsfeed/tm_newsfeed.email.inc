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
	// $user_id = 29540;
	$user_ids = array(29540,29538,29537,29536,29535,29534,29533,29532,29531,29530,29529,29528,29527,29526,29525,29524,29523,29522,29521,29520,29519,29518,29517,29516,29515,29514,29513,29512,29511,29510,29509,29508,29507,29506,29505,29504,29503,29502,29501,29500,29499,29498,29497,29496,29495,29494,29493,29492,29491,29490,29489,29488,29487,29486,29485,29484,29483,29482,29481,29480,29479,29478,29477,29476,29475,29474,29473,29472,29471,29470,29468,29467,29466,29465,29464,29463,29462,29461,29460,29459,29458,29457,29456,29455,29454,29453,29452,29451,29450,29449,29447,29446,29445,29444,29443,29442,29441,29440,29439,29438);
	// $user_ids = array(29540,29538,29537,29536,29535);
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

	// globals
	global $tm_newsfeed_email_template;
	global $tm_newsfeed_email_greetings;
	global $tm_newsfeed_email_opening_lines;
	global $tm_newsfeed_email_quotes;

	// process inputs
	$account = user_load($user_id);
	$user_name = $account->field_user_first_name[LANGUAGE_NONE][0]['value'];
	$search_country = tm_newsfeed_get_search_country($account);

	// start building email
	$email = $tm_newsfeed_email_template;

	$greeting = $tm_newsfeed_email_greetings[array_rand($tm_newsfeed_email_greetings)];
	$email = str_replace("__GREETING__", $greeting, $email);

	$email = str_replace("__USER_FIRST_NAME__", $user_name, $email);

	$opening_line = $tm_newsfeed_email_opening_lines[array_rand($tm_newsfeed_email_opening_lines)];
	$email = str_replace("__OPENING_LINE__", $opening_line, $email);

	$tip = tm_newsfeed_email_get_tip($user_id);
	$email = str_replace("__EMAIL_TIP__", $tip, $email);

	$content = tm_newsfeed_email_populate_section_content($user_id, $search_country, $start_date, $end_date);
	$email = str_replace("__EMAIL_CONTENT__", $content, $email);

	$quote = $tm_newsfeed_email_quotes[array_rand($tm_newsfeed_email_quotes)];
	$email = str_replace("__QUOTE_OF_THE_DAY__", $quote['text'], $email);
	$email = str_replace("__QUOTE_OF_THE_DAY_AUTHOR__", $quote['author'], $email);

	$email = str_replace("__THANK_YOU_PHRASE__", "Cheers", $email);
	$email = str_replace("__PLATFORM_NAME__", "Travel Massive", $email);

	return $email;
}


function tm_newsfeed_email_get_tip($user_id) {
	// [Tip: Create a free company profile for Rock Climb Travel and join more than 1,300 other companies on our network.]

	// for no tip:
	// return "";

	return "[Tip: ensure your profile is up to date. BLAH BLAH]";
}

function tm_newsfeed_email_populate_section_content($user_id, $search_country, $start_date, $end_date) {
	
	global $debug_timer;

	$content = "";
	$debug_timer['11_start_events'] = time();
	$content = $content . tm_newsfeed_email_populate_events($user_id, $end_date);
	$debug_timer['12_start_updates'] = time();
	$content = $content . tm_newsfeed_email_populate_updates($user_id, $start_date, $end_date);
	$debug_timer['13_start_new_members'] = time();
	$content = $content . tm_newsfeed_email_populate_new_members($user_id, $start_date, $end_date);
	$debug_timer['14_start_new_companies'] = time();
	$content = $content . tm_newsfeed_email_populate_new_companies($user_id, $start_date, $end_date);
	$debug_timer['15_start_jobs'] = time();
	$content = $content . tm_newsfeed_email_populate_jobs($search_country, $start_date, $end_date);
	$debug_timer['16_start_discussions'] = time();
	$content = $content . tm_newsfeed_email_populate_discussions($user_id, $start_date, $end_date);
	$debug_timer['17_start_blog_posts'] = time();
	$content = $content . tm_newsfeed_email_populate_blog_posts($user_id, $start_date, $end_date);
	$debug_timer['20_done_content'] = time();

	return $content;
}

function tm_newsfeed_email_populate_updates($user_id, $start_date, $end_date) {
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

		$link = $result->entity_name;
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

	foreach($results as $result) {
		$href = $tm_domain . "/node/" . $result->e_id;
		$link = $result->event;
		$text = ', '. date_format(new DateTime($result->date), 'l, F j');

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
	usort($results, 'cmp');
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


function tm_newsfeed_email_populate_discussions($user_id, $start_date, $end_date) {
	$items = array();
	$results = array(); // tm_newsfeed_discussions_fetch($user_id, $start_date, $end_date);

	if (count($results) < 1) return "";

	foreach($results as $result) {
		$href = "A";
		$link = "A";
		$text = "A";

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('discussions', $items);
}


function tm_newsfeed_email_populate_blog_posts($user_id, $start_date, $end_date) {
	$items = array();
	$results = array(); // tm_newsfeed_blog_posts_fetch($user_id, $start_date, $end_date);

	if (count($results) < 1) return "";

	foreach($results as $result) {
		$href = "A";
		$link = "A";
		$text = "A";

		$items[] = array("href" => $href, "link" => $link, "text" => $text);
	}

	return tm_newsfeed_email_populate_section('blog_posts', $items);
}



function tm_newsfeed_email_populate_section($section_name, $items) {
	global $tm_newsfeed_email_section_headers;
	global $tm_newsfeed_email_section;
	global $tm_newsfeed_email_section_row;

	$section = $tm_newsfeed_email_section;
	$section_contents = "";

	foreach($items as $item) {
		$row = $tm_newsfeed_email_section_row;
		$row = str_replace("__EMAIL_ITEM_LINK__", $item['href'], $row);
		$row = str_replace("__EMAIL_ITEM_LINK_TEXT__", $item['link'], $row);
		$row = str_replace("__EMAIL_ITEM_EXTRA_TEXT__", $item['text'], $row);
		$section_contents = $section_contents . $row;
	}

	$section = str_replace("__EMAIL_SECTION_HEADER__", $tm_newsfeed_email_section_headers[$section_name], $section);
	$section = str_replace("__EMAIL_SECTION_ROWS__", $section_contents, $section);

	return $section;
}




/* Templates */

global $tm_newsfeed_email_template;
$tm_newsfeed_email_template = <<<EOT
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
	<p>__THANK_YOU_PHRASE__,<br>The __PLATFORM_NAME__ Team</p>
</div>
EOT;

global $tm_newsfeed_email_section;
$tm_newsfeed_email_section = <<<EOT
<div class="email-section">
	<strong>__EMAIL_SECTION_HEADER__</strong><br>
	__EMAIL_SECTION_ROWS__
</div>
<br>
EOT;

global $tm_newsfeed_email_section_row;
$tm_newsfeed_email_section_row = <<<EOT
<a href="__EMAIL_ITEM_LINK__">__EMAIL_ITEM_LINK_TEXT__</a>__EMAIL_ITEM_EXTRA_TEXT__<br>
EOT;

global $tm_newsfeed_css;
$tm_newsfeed_css = <<<EOT
<style>
</style>
EOT;


/* Lookups */
global $tm_newsfeed_email_section_headers;
$tm_newsfeed_email_section_headers = array(
	'blog_posts' => 'Most popular on the blog',
	'discussions' => 'Recent discussions',
	'jobs' => 'Jobs and opportunities',
	'new_members' => 'New members in your network',
	'new_companies' => 'New companies in your network',
	'updates' => 'Latest status updates',
	'events' => 'Upcoming events'
	);


global $tm_newsfeed_email_greetings;
$tm_newsfeed_email_greetings = array(
	"Hello",
	"Hey",
	"Hola",
	"Konnichiwa"
	);

global $tm_newsfeed_email_opening_lines;
$tm_newsfeed_email_opening_lines = array(
	"Here’s what’s been happening at Travel Massive this week.",
	"Check out what’s been happening at Travel Massive this week."
	);

global $tm_newsfeed_email_quotes;
$tm_newsfeed_email_quotes = array(
	array("text" => "Travel makes one modest, you see what a tiny place you occupy in the world.",
				"author" => "Gustave Flaubert"),
	array("text" => "Oh the places you’ll go!",
				"author" => "Dr. Seuss"),
	array("text" => "Investment in travel is an investment in yourself.",
				"author" => "Matthew Karsten")
	);




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
function cmp($a, $b)
{
  return strcmp($a->seconds_ago, $b->seconds_ago);
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

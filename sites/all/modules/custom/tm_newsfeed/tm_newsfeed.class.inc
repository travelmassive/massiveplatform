<?php

// tm_newsfeed.class.inc - class file

/**
 * Generate an email newsfeed notification
 */
class TmNewsfeedEmailGenerator
{
	public $_debug_log = array();

	public function debug($debug_text) {
		$this->_debug_log[] = $debug_text;
	}

	public function get_debug_log() {
		return "Debug log:\n" . implode("\n", $this->_debug_log);
	}

	public function start($user_ids, $start_date, $end_date) {

		global $conf;
		global $debug_timer;

		// debug info
		$this->debug(print_r('#users: '. count($user_ids), true));
		$loop_start = time();
		$this->debug(print_r("loop start: $loop_start", true));

		$notifications = array();

		for ($i=0; $i < count($user_ids); $i++) {

			$user_id = $user_ids[$i];

			// check we have a valid uid
			if (!tm_newsfeed_check_uid($user_id)) {
				continue;
			}

			// store debug info
			$debug_timer['01_start_email'] = time();

			// generate email
			$e = new TmNewsfeedEmail();
			$email = $e->tm_newsfeed_generate_email($user_id, $start_date, $end_date);
			$email_subject = $email['subject'];
			$email_body = $email['body'];
			$notifications[] = array("user_id" => $user_id, "subject" => $email_subject, "body" => $email_body);

			// debug info
			$debug_timer['90_end_email'] = time();
			$debug_timer['total_email'] = $debug_timer['90_end_email'] - $debug_timer['01_start_email'];
			$debug_str = "Debug:\n";
			$debug_str = $debug_str . 'start_date: '. $start_date .'\nend_date: '. $end_date . '\n';
			array_walk($debug_timer, function(&$a, $b) { $a = "$a :: $b"; });
			$debug_str = $debug_str . implode('\n', $debug_timer);
			$this->debug($debug_str);
		}

		// debug info
		$loop_stop = time();
		$this->debug(print_r("loop  stop: $loop_stop", true));
		$loop_total = $loop_stop - $loop_start;
		$this->debug(print_r("loop total: $loop_total", true));

		return $notifications;
	}
}

class TmNewsfeedEmail
{
	private $first_update_from;

	public function tm_newsfeed_generate_email($user_id, $start_date, $end_date) {

		global $conf;
		global $debug_timer;

		// process inputs
		$debug_timer['03a_user_load'] = time();
		$account = user_load($user_id);
		$debug_timer['03b_done'] = time();
		$user_name = $account->field_user_first_name[LANGUAGE_NONE][0]['value'];
		$debug_timer['04a_get_search_country'] = time();
		$search_country = $this::tm_newsfeed_get_search_country($account);
		$debug_timer['04b_done'] = time();

		// start building email
		$email = TmNewsfeedEmailTemplates::$newsfeed_email_template;

		$greeting_lookup = $conf['tm_newsfeed_email_greetings'];
		$greeting = $greeting_lookup[array_rand($greeting_lookup)];
		$email = str_replace("__GREETING__", $greeting, $email);

		$email = str_replace("__USER_FIRST_NAME__", strip_tags($user_name), $email);

		$opening_line = $conf['tm_newsfeed_email_opening_lines'][array_rand($conf['tm_newsfeed_email_opening_lines'])];
		$email = str_replace("__OPENING_LINE__", $opening_line, $email);

		$debug_timer['05_start_tips'] = time();
		$tip = $this->tm_newsfeed_email_get_tip($account, $start_date, $end_date);
		$email = str_replace("__EMAIL_TIP__", $tip, $email);
		$debug_timer['06_done_tips'] = time();

		$content = $this->tm_newsfeed_email_populate_section_content($user_id, $search_country, $start_date, $end_date);
		$email = str_replace("__EMAIL_CONTENT__", $content, $email);

		$quote = $conf['tm_newsfeed_email_quotes'][array_rand($conf['tm_newsfeed_email_quotes'])];
		$email = str_replace("__QUOTE_OF_THE_DAY__", $quote['text'], $email);
		$email = str_replace("__QUOTE_OF_THE_DAY_AUTHOR__", $quote['author'], $email);

		$feedback_base = $conf['tm_newsfeed_feedback_base'];
		$feedback_link_text = $conf['tm_newsfeed_feedback_link_text'];
		$feedback = "$feedback_base <a href='".url("contact")."'>$feedback_link_text</a>";
		$email = str_replace("__LET_US_KNOW_FEEDBACK__", $feedback, $email);

		$email = str_replace("__THANK_YOU_PHRASE__", "Cheers", $email);
		$email = str_replace("__PLATFORM_NAME__", $conf['tm_site_name'], $email);

		// strip all exclamation marks as they trigger spam filters
		$email = str_replace("!", "", $email);
		$email = str_replace("$", "", $email);

		// set the subject base text
		$subject_base = $conf['tm_newsfeed_subject_base'];
		$date_interval = date_diff(date_create($start_date), date_create($end_date));
		if ($date_interval->days <= 1) {
			$subject_base = $conf['tm_newsfeed_subject_base_daily'];
		}
		if ($date_interval->days > 5) {
			$subject_base = $conf['tm_newsfeed_subject_base_weekly'];
		}

		// add extra text
		$subject_extra = "";
		if (strlen($this->first_update_from) > 0) {
			$subject_extra = "(Updates from $this->first_update_from + more)";
		} else {
			// subject extra text if no updates available to customise with
			$subject_extra = $conf['tm_newsfeed_subject_extra_fallbacks'][array_rand($conf['tm_newsfeed_subject_extra_fallbacks'])];
		}

		$subject = "$subject_base $subject_extra";
			
		return array('body' => $email, 'subject' => $subject);
	}


	public function tm_newsfeed_email_get_tip($account, $start_date, $end_date) {

		$uid = $account->uid;
		$edit_url = tm_newfeed_absolute_url("user/" . $uid);

		// important tips
		// these will always be shown in order of importance if applicable

		if (!in_array("approved user", $account->roles)){
			$tip = "your profile is not approved, please complete your profile and request approval";
			return "[Tip: $tip]";
		}

		$following_no_one = (0 == db_query("select count(*) from flagging where fid=1 and uid = :uid", array(":uid" => $uid))->fetchField());
		if ($following_no_one){
			$tip = "you aren't following anyone - connect with other members of the community to receive updates";
			return "[Tip: $tip]";
		}

		$no_chapters = (0 == db_query("select count(*) from flagging where fid=3 and uid = :uid", array(":uid" => $uid))->fetchField());
		if ($no_chapters){
			$tip = "you haven't joined a chapter yet - connect with other members of the community to receive updates";
			return "[Tip: $tip]";
		}

		// cycle tips
		// create an array of other tips that apply to the user
		// and select a random one for inclusion in the email
		$tips = array();

		// create company profile
		// if user has populated field_job_organization, show tip to create a company profile
		if (sizeof($account->field_job_organization) > 0) {

			$is_owner_of = db_query("select count(*) from field_data_field_owner where field_owner_target_id = :uid", array(":uid" => $uid))->fetchField();
			if ($is_owner_of > 0) {
				// already the profile manager of one or more companies, so don't suggest another
				// i.e. do nothing
			} else {
				$field_job_organization = $account->field_job_organization['und'][0]['value'];

				$org_to_match = strtolower($field_job_organization);

				$query = db_query("select count(*) from node where type = 'organization' and lower(title) like :org",
					array(':org' => db_like($org_to_match) . '%'));
				$no_match = $query->fetchField() == 0;

				if ($no_match) {
					$num_companies = number_format(db_query("select count(*) as 'count' from node where node.type='organization'")->fetchField());

					$company_profile_link = "<a href='".url("companies")."'>company profile</a>";
					$tip = "Create a $company_profile_link for $field_job_organization and join more than $num_companies other companies on our network.";

					$tips[] = $tip;
				}
			}
		}

		if (sizeof($account->field_about_me) == 0) {
			$tips[] = "Share a little bit " . l(t('about yourself'), $edit_url) . " and your story with the community.";
		}

		if ((sizeof($account->field_location_city) == 0) or (sizeof($account->field_user_country) == 0)) {
			$tips[] = "Tell us " . l(t('where you are located'), $edit_url) . " so other members can discover you.";
		}

		if (sizeof($account->field_job_role) == 0) {
			$tips[] = "Share your " . l(t('job role'), $edit_url) . " so like-minded members can connect with you.";
		}

		if (sizeof($tips) == 0) {
			// general fall back tips if no other tips were selected based on user conditions
			$tips[] = "if you relocate, be sure to keep your profile up to date so that you can meet other people in the Travel Massive community";
			// $tips[] = "";
			// $tips[] = "";
		}
		
		// select random tip
		$rand = $tips[array_rand($tips)];
		$tip = "[Tip: $rand]";

		return $tip;
	}

	public function tm_newsfeed_email_populate_section_content($user_id, $search_country, $start_date, $end_date) {
		
		global $conf;
		global $debug_timer;

		$content = "";

		// note that for the email, we want 'upcoming events' - those taking place after the data period for this email, hence $end_date
		$debug_timer['11_start_events'] = time();
		$content = $content . $this->tm_newsfeed_email_populate_events($user_id, $end_date);

		$debug_timer['12_start_updates'] = time();
		$content = $content . $this->tm_newsfeed_email_populate_updates($user_id, $start_date, $end_date);

		$debug_timer['13_start_new_members'] = time();
		$content = $content . $this->tm_newsfeed_email_populate_new_members($user_id, $start_date, $end_date);

		$debug_timer['14_start_new_companies'] = time();
		$content = $content . $this->tm_newsfeed_email_populate_new_companies($user_id, $start_date, $end_date);

		if ($conf['tm_newsfeed_marketplace_enabled']) {
			$debug_timer['15_start_jobs'] = time();
			$content = $content . $this->tm_newsfeed_email_populate_jobs($search_country, $start_date, $end_date);
		}

		if ($conf['tm_newsfeed_discuss_enabled']) {
			$debug_timer['16_start_discussions'] = time();
			$content = $content . $this->tm_newsfeed_email_populate_discussions($user_id);
		}
		
		if ($conf['tm_newsfeed_blog_enabled']) {
			$debug_timer['17_start_blog_posts'] = time();
			$content = $content . $this->tm_newsfeed_email_populate_blog_posts($user_id, $start_date, $end_date);
		}

		$debug_timer['20_done_content'] = time();

		return $content;
	}

	public function tm_newsfeed_email_populate_updates($user_id, $start_date, $end_date) {
		global $conf;

		$items = array();
		$results = tm_newsfeed_get_updates($user_id, $start_date, $end_date);

		if (count($results) < 1) return "";

		foreach($results as $index => $result) {
			$href = "";
			if ($result->entity_type == 'user') {
				$href = tm_newfeed_absolute_url("user/" . $result->entity_id);
			} elseif ($result->entity_type == 'chapter') {
				$href = tm_newfeed_absolute_url("node/" . $result->entity_id);
			} else {
				$href = tm_newfeed_absolute_url("node/" . $result->entity_id);
			}

			$link = "";
			if ($result->entity_type == 'user') {
				$link = $result->entity_name;
			} elseif ($result->entity_type == 'chapter') {
				$chapter = node_load($result->entity_id);
				$link = tm_chapters_formal_name($chapter);
			} else {
				$link = $result->entity_name;
			}

			$time = tm_newsfeed_time_elapsed_string($result->created);
			$text = " posted an update $time";

			// save link text of first update item, for use email subject line
			if ($index == 0) {
				$this->first_update_from = $link;
			}

			$items[] = array("href" => $href, "link" => strip_tags($link), "text" => $text);
		}

		$read_more = "<a href='" . tm_newfeed_absolute_url("newsfeed") . "'>View your news feed</a><br>";
		return $this->tm_newsfeed_email_populate_section('updates', $items, $read_more);
	}


	public function tm_newsfeed_email_populate_events($user_id, $from_date) {
		// note that for the email, we want events taking place, from the time of the email in to the future
		$items = array();
		$results = tm_newsfeed_get_events($user_id, $from_date);

		if (count($results) < 1) return "";

		// sort results purely by date; query will be sorted by relevance columns first
		usort($results, 'sort_event_date');

		foreach($results as $result) {
			$href = tm_newfeed_absolute_url("node/" . $result->event_id);
			$link = $result->event_title;
			$text = ', '. date_format(new DateTime($result->event_date), 'l, F j');

			$items[] = array("href" => $href, "link" => strip_tags($link), "text" => $text);
		}

		return $this->tm_newsfeed_email_populate_section('events', $items);
	}


	public function tm_newsfeed_email_populate_new_members($user_id, $start_date, $end_date) {
		$items = array();
		$results = tm_newsfeed_get_new_members($user_id, $start_date, $end_date);

		if (count($results) < 1) return "";

		foreach($results as $result) {
			$href = tm_newfeed_absolute_url("user/" . $result->uid);
			$link = $result->user_full_name;

			$home_chapter_url = tm_newfeed_absolute_url('node/'.$result->home_chapter_id);
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

			$items[] = array("href" => $href, "link" => strip_tags($link), "text" => $text);
		}

		return $this->tm_newsfeed_email_populate_section('new_members', $items);
	}


	public function tm_newsfeed_email_populate_new_companies($user_id, $start_date, $end_date) {
		$items = array();
		$results = tm_newsfeed_get_new_companies($user_id, $start_date, $end_date);

		if (count($results) < 1) return "";

		foreach($results as $result) {
			$href = tm_newfeed_absolute_url("node/" . $result->nid);
			$link = $result->title;
			$text = ", ".$result->category;

			$items[] = array("href" => $href, "link" => strip_tags($link), "text" => strip_tags($text));
		}

		return $this->tm_newsfeed_email_populate_section('new_companies', $items);
	}


	public function tm_newsfeed_email_populate_jobs($search_country, $start_date, $end_date) {
		$items = array();

		$results = array();

		try {
			$results = tm_newsfeed_marketplace_fetch($search_country);
		}
		catch(Exception $e) {
			return "";
		}

		if (count($results) < 1) return "";

		// process results
		usort($results, 'sort_seconds_ago');
		$results = array_slice($results, 0, 3);

		foreach($results as $result) {
			$href = $result->url;
			$link = $result->title;
			$text = ", ".$result->location;

			// add featured
			if ($result->is_featured) {
				$text = " (Featured)" . $text;
			}

			$items[] = array("href" => $href, "link" => $link, "text" => $text);
		}

		return $this->tm_newsfeed_email_populate_section('jobs', $items);
	}


	public function tm_newsfeed_email_populate_discussions($user_id) {
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

		return $this->tm_newsfeed_email_populate_section('discussions', $items);
	}


	public function tm_newsfeed_email_populate_blog_posts($user_id, $start_date, $end_date) {
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

		return $this->tm_newsfeed_email_populate_section('blog_posts', $items);
	}


	public function tm_newsfeed_email_populate_section($section_name, $items, $read_more = "") {

		global $conf;

		$section = TmNewsfeedEmailTemplates::$newsfeed_email_section;
		$section_contents = "";

		foreach($items as $item) {
			$row = TmNewsfeedEmailTemplates::$newsfeed_email_section_row;
			$row = str_replace("__EMAIL_ITEM_LINK__", $item['href'], $row);
			$row = str_replace("__EMAIL_ITEM_LINK_TEXT__", $item['link'], $row);
			$row = str_replace("__EMAIL_ITEM_EXTRA_TEXT__", $item['text'], $row);
			$section_contents = $section_contents . $row;
		}

		$section = str_replace("__EMAIL_SECTION_HEADER__", $conf['tm_newsfeed_email_section_headers'][$section_name], $section);
		$section = str_replace("__EMAIL_SECTION_ROWS__", $section_contents, $section);

		if ($read_more != "") {
			$section = str_replace("__EMAIL_READ_MORE__", $read_more, $section);
		} else {
			$section = str_replace("__EMAIL_READ_MORE__", "", $section);
		}

		return $section;
	}

	private static function tm_newsfeed_get_search_country($account)
	{

		$user_home_chapter = null;
		if (isset($account->field_home_chapter[LANGUAGE_NONE][0]['target_id'])) {
			$user_home_chapter = $account->field_home_chapter[LANGUAGE_NONE][0]['target_id'];
		}

		$user_country = null;
		if (isset($account->field_user_country[LANGUAGE_NONE][0]['iso2'])) {
			$user_country = $account->field_user_country[LANGUAGE_NONE][0]['iso2'];
		}

		// if country in profile is set, use that
		// if home chapter is set, use country of home chapter
		// otherwise, show everything
		$search_country = "";  // shows everything

		if (is_null($user_country)) {
			if (is_null($user_home_chapter)) {
				// use "" as search country
			} else {
				$chapter = node_load($user_home_chapter);

				// use chapter country if it exists, otherwise search with ""
				if (isset($account->field_country[LANGUAGE_NONE][0]['iso2'])) {
					$country_code = $chapter->field_country[LANGUAGE_NONE][0]['iso2'];
					$search_country = country_load($country_code)->name;
				}
			}
		} else {
			$search_country = $user_country;
		}

		return $search_country;
	}

}


class TmNewsfeedEmailTemplates
{
	/* Templates */
	public static $newsfeed_email_template = <<<EOT
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
	"__QUOTE_OF_THE_DAY__" - <em>__QUOTE_OF_THE_DAY_AUTHOR__</em>
	</p>
	<p>__LET_US_KNOW_FEEDBACK__</p>
	<p>__THANK_YOU_PHRASE__,<br>The __PLATFORM_NAME__ Team</p>
</div>
EOT;

	public static $newsfeed_email_section = <<<EOT
<div class="email-section">
	<strong>__EMAIL_SECTION_HEADER__</strong><br>
	__EMAIL_SECTION_ROWS__
	__EMAIL_READ_MORE__
</div>
<br>
EOT;

	public static $newsfeed_email_section_row = <<<EOT
<a href="__EMAIL_ITEM_LINK__">__EMAIL_ITEM_LINK_TEXT__</a>__EMAIL_ITEM_EXTRA_TEXT__<br>
EOT;

}

/**
 * Helper method to create absolute url from drupal path (ie: /node/123, /user/123, etc)
 */
function tm_newfeed_absolute_url($path) {

	global $conf;

	// We can't rely on url($url,array('absolute'=>true)) as calling from drush command
	// Instead we construct the url from conf variables
	$absolute_url = $conf['tm_email_server_http'] . "://" . $conf['tm_email_server_url_domain'] . "/" . drupal_get_path_alias($path);

	return $absolute_url;
}


/**
 * Helper method to get time since
 */
function tm_newsfeed_time_elapsed_string($datetime, $full = false) {
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

/**
 * Helper method
 * Sort an array of objects by 'seconds_ago' attribute
 */
function sort_seconds_ago($a, $b)
{
	return strcmp($a->seconds_ago, $b->seconds_ago);
}

/**
 * Helper method
 * Sort an array of objects returned from dicussions fetch
 */
function sort_discussions($a, $b)
{
	return strcmp($a->seconds_ago, $b->seconds_ago);
}

/**
 * Helper method
 * Sort an array of objects by 'seconds_ago' attribute
 */
function sort_event_date($a, $b)
{
	return strcmp($a->event_date, $b->event_date);
}

/**
 * Helper method
 * Check if uid is valid
 */
function tm_newsfeed_check_uid($uid) {

	// check uid is int
	if (!is_numeric($uid)) {
		return false;
	}

	// check db
	$query = "SELECT COUNT(*) as found_uid FROM {users} u WHERE u.uid = :uid";
	$result = db_query($query, array(':uid' => $uid))->fetch();
	return ($result->found_uid >= 1);
}

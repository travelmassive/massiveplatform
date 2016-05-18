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

	$gen = new TmNewsfeedEmailGenerator();

	// params
	// $user_ids = array(29540,29538,29537,29536,29535,29534,29533,29532,29531,29530,29529,29528,29527,29526,29525,29524,29523,29522,29521,29520,29519,29518,29517,29516,29515,29514,29513,29512,29511,29510,29509,29508,29507,29506,29505,29504,29503,29502,29501,29500,29499,29498,29497,29496,29495,29494,29493,29492,29491,29490,29489,29488,29487,29486,29485,29484,29483,29482,29481,29480,29479,29478,29477,29476,29475,29474,29473,29472,29471,29470,29468,29467,29466,29465,29464,29463,29462,29461,29460,29459,29458,29457,29456,29455,29454,29453,29452,29451,29450,29449,29447,29446,29445,29444,29443,29442,29441,29440,29439,29438);
	$user_ids = array(29540,29538,29537,29536,29535,10450,10454,10455);
	// $user_ids = array(29540);

	$start_date = new DateTime('2016-05-01');
	$end_date = new DateTime();
	$start_date = $start_date->format('Y-m-d H:i:s');
	$end_date = $end_date->format('Y-m-d H:i:s');
	// $start_date = new DateTime('-2 week');
	// $end_date = new DateTime('-1 week');
	// $start_date = new DateTime('2015-04-24');
	// $end_date = new DateTime('2017-04-30');

	return $gen->start($user_ids, $start_date, $end_date);
}

// 
class TmNewsfeedEmailGenerator
{
	public function start($user_ids, $start_date, $end_date) {

		global $conf;
		global $debug_timer;

		print_r('<pre>');
		print_r('#users: '. count($user_ids) .'<br>');
		$loop_start = time();
		print_r("loop start: $loop_start");
		print_r('</pre>');

		for ($i=0; $i < count($user_ids); $i++) {

			$user_id = $user_ids[$i];
			$e = new TmNewsfeedEmail();
			$debug_timer['01_start_email'] = time();
			$email = $e->tm_newsfeed_generate_email($user_id, $start_date, $end_date);
			$debug_timer['90_end_email'] = time();
			$debug_timer['total_email'] = $debug_timer['90_end_email'] - $debug_timer['01_start_email'];


			$debug_str = "Debug:<br>";
			$debug_str = $debug_str . '<pre>start_date: '. $start_date .'<br>  end_date: '. $end_date .'</pre>';
			array_walk($debug_timer, function(&$a, $b) { $a = "$a :: $b"; });
			$debug_str = $debug_str .'<pre>'. implode('<br>', $debug_timer) .'</pre>';

			$email_subject = $email['subject'];
			$email_body = $email['body'] . $debug_str;

			$send = $conf['tm_newsfeed_send_emails_enabled'];
			if ($send) {
				$e->tm_newsfeed_notification_send_email(user_load($user_id), $email_subject, $email_body);
			} else {
				// web page testing
				$response = $response . '<hr>';
				$response = $response . '<p>Subject: <strong>'.$email_subject . '</strong></p>';
				$response = $response . $email_body;
			}
		}

		print_r('<pre>');
		$loop_stop = time();
		print_r("loop  stop: $loop_stop");
		$loop_total = $loop_stop - $loop_start;
		print_r('<br>');
		print_r("loop total: $loop_total");
		print_r('</pre>');

		return $response;
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
		$feedback = "$feedback_base <a href='".tm_newsfeed_domain()."/contact'>$feedback_link_text</a>";
		$email = str_replace("__LET_US_KNOW_FEEDBACK__", $feedback, $email);

		$email = str_replace("__THANK_YOU_PHRASE__", "Cheers", $email);
		$email = str_replace("__PLATFORM_NAME__", $conf['tm_site_name'], $email);

		// strip all exclamation marks as they trigger spam filters
		$email = str_replace("!", "", $email);
		$email = str_replace("$", "", $email);


		$subject_base = $conf['tm_newsfeed_subject_base'];
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
		$edit_url = tm_newsfeed_domain() . "/user/" . $uid;

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

					$company_profile_link = "<a href='".tm_newsfeed_domain()."/companies'>company profile</a>";
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
				$href = tm_newsfeed_domain() . "/user/" . $result->entity_id;
			} elseif ($result->entity_type == 'chapter') {
				$href = tm_newsfeed_domain() . "/node/" . $result->entity_id;
			} else {
				$href = tm_newsfeed_domain() . "/node/" . $result->entity_id;
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

			$time = time_elapsed_string($result->created);
			$text = " posted an update $time";

			// save link text of first update item, for use email subject line
			if ($index == 0) {
				$this->first_update_from = $link;
			}

			$items[] = array("href" => $href, "link" => strip_tags($link), "text" => $text);
		}

		return $this->tm_newsfeed_email_populate_section('updates', $items);
	}


	public function tm_newsfeed_email_populate_events($user_id, $from_date) {
		// note that for the email, we want events taking place, from the time of the email in to the future
		$items = array();
		$results = tm_newsfeed_get_events($user_id, $from_date);

		if (count($results) < 1) return "";

		// sort results purely by date; query will be sorted by relevance columns first
		usort($results, 'sort_event_date');

		foreach($results as $result) {
			$href = tm_newsfeed_domain() . "/node/" . $result->event_id;
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
			$href = tm_newsfeed_domain() . "/user/" . $result->uid;
			$link = $result->user_full_name;

			$home_chapter_url = tm_newsfeed_domain().'/node/'.$result->home_chapter_id;
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
			$href = tm_newsfeed_domain() . "/node/" . $result->nid;
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
			$href = $url;
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


	public function tm_newsfeed_email_populate_section($section_name, $items) {

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

		return $section;
	}

	public function tm_newsfeed_notification_send_email($user, $subject, $body) {

		global $conf;
		$email = $user->mail;

		// Get Users Name
		if (empty($user->language)) $language = LANGUAGE_NONE;
		else $language = $user->language;
		$name = $user->name;
		if (isset($user->field_user_first_name[$language])){
			$name = $user->field_user_first_name[$language][0]['value'];
			if (isset($user->field_user_first_name[$language])){
				$name .= ' '.$user->field_user_last_name[$language][0]['value'];
			}
		}

		// SEND EMAIL
		$module = 'tm_newsfeed';
		$key = 'key';
		$language = language_default();
		$params = array();
		$from = NULL;
		$reply = $user->mail;
		$send = FALSE;
		

		// CONSTRUCT MESSAGE
		// $member_url = url('user/'.$user->uid, array('absolute'=>TRUE));
		// $chapter_url = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . drupal_get_path_alias('node/' . $chapter->nid);

		$message = $body;

		// STEP 1. LOAD HTML TEMPLATE
		$template_file = drupal_get_path('module', 'tm_notifications') . '/templates/tm_notifications-email_template.tpl.php';
		$template = file_get_contents($template_file);


		// STEP 2. SET MAIL HTML HEADERS
		$email_message = drupal_mail($module, $key, $email, $language, $params, NULL, $send);
		$email_message['headers']['MIME-Version'] = '1.0';
		$email_message['headers']['Content-Type'] = 'multipart/mixed;';
		$email_message['headers']['Content-Type'] = 'text/html;';
		$email_message['headers']['Reply-to'] = $reply; // set reply-to to the user

		// STEP 3. SET SUBJECT 
		$email_message['subject'] = check_plain($subject);

		// STEP 4. REPLACE BODY AND FOOTER IN HTML TEMPLATE
		$template = str_replace("__EMAIL_BODY__", $message, $template);
		$footer = $conf["tm_email_default_footer"];
		if (isset ($params['footer'])) {
			$footer = $params['footer'];
		}
		$template = str_replace("__EMAIL_FOOTER__", $footer, $template);

		// Other vars
		$template = str_replace("__SERVER_NAME__", $_SERVER["SERVER_NAME"], $template); 
		$template = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $template);  
		$template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $template);  
		$template = str_replace("__EMAIL_SIGNOFF__", $conf["tm_email_signoff"], $template); 

		// STEP 5. LOAD HTML TEMPLATE INTO EMAIL BODY
		$email_message['body'] = $template;

		// STEP 6. SEND EMAIL
		// Retrieve the responsible implementation for this message.
		$system = drupal_mail_system($module, $key);

		// Format the message body.
		$format_message = $system->format($email_message);

		// Send e-mail.
		$result = $system->mail($format_message);
	}



	private static function tm_newsfeed_get_search_country($account)
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
	"__QUOTE_OF_THE_DAY__" – <em>__QUOTE_OF_THE_DAY_AUTHOR__</em>
	</p>
	<p>__LET_US_KNOW_FEEDBACK__</p>
	<p>__THANK_YOU_PHRASE__,<br>The __PLATFORM_NAME__ Team</p>
</div>
EOT;

	public static $newsfeed_email_section = <<<EOT
<div class="email-section">
	<strong>__EMAIL_SECTION_HEADER__</strong><br>
	__EMAIL_SECTION_ROWS__
</div>
<br>
EOT;

	public static $newsfeed_email_section_row = <<<EOT
<a href="__EMAIL_ITEM_LINK__">__EMAIL_ITEM_LINK_TEXT__</a>__EMAIL_ITEM_EXTRA_TEXT__<br>
EOT;

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


function tm_newsfeed_domain() {
	global $conf;
	$conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"];
}

<?php

// tm_commissions.summary.inc - commission summary methods


function tm_commissions_reports_chapter_summary($nid, $chapter_code, $days = 30) {

	$results = tm_commissions_get_chapter_commissions_summary($chapter_code, $days);

	// check we got a result
	if ($results == null) {
		return "";
	}

	// check valid result
	if ($results->success == false) {
		return "";
	}

	// construct summary

	// Example:
	// Congratulations! Your chapter has earned $1150 AUD in the past 30 days. View commissions.
	// Your chapter has a balance of $1150 USD and $550 AUD. View commissions.
	// Chapters have earned a total of than $4,800 from community sales in the past 30 days. View all commissions.

	// Get summary
	$summary = $results->data->summary;
	$summary_text = "";

	// sanitize display
	$MAX_LIMIT = 1000000;

	// view commisions link
	$view_chapter_commissions_text = "<a href='__SERVER_HTTP__://__SERVER_NAME__/chapters/" . drupal_get_path_alias("node/" . $nid) . "/commissions'>View commissions</a>";
	$view_all_commissions_text = "<a href='__SERVER_HTTP__://__SERVER_NAME__/chapters/all-chapters/commissions'>View all commissions</a>";

	// commissions
	$has_commissions = false;
	if (sizeof($summary->commissions) > 0) {
		$has_commissions = true;
		$total_parts = array();

		// each currency
		foreach($summary->commissions as $commissions) {

			// check limits
			if (($commissions->total > 0) and ($commissions->total < $MAX_LIMIT)) {
				$total_parts[] = tm_commissions_currency_symbol($commissions->currency, true) . $commissions->total . " " . $commissions->currency;
			}
		}

		$total_text = _tm_commissions_format_multiple_currencies($total_parts);
		$summary_text .= "Congratulations! Your chapter has earned " . $total_text . " in the past " . $days . " days &mdash; " . $view_chapter_commissions_text . "\n";
	}

	// balance	
	if (!$has_commissions) {
		if (sizeof($summary->balance) > 0) {
			$total_parts = array();

			// each currency
			foreach($summary->balance as $balance) {

				// check limits
				if (($balance->total > 0) and ($balance->total < $MAX_LIMIT)) {
					$total_parts[] = tm_commissions_currency_symbol($balance->currency, true) . $balance->total . " " . $balance->currency;
				}
			}

			$total_text = _tm_commissions_format_multiple_currencies($total_parts);
			$summary_text .= "Your chapter has a balance of " . $total_text . " &mdash; " . $view_chapter_commissions_text . "\n";
		}
	}

	// show other chapters
	// only show usd or eur
	if (sizeof($summary->all_chapters) > 0) {
		$total_parts = array();
		foreach($summary->all_chapters as $all_chapters) {

			// each currency
			if ($all_chapters->currency == "usd" or $all_chapters->currency == "eur") {

				// check limits
				if (($all_chapters->total > 0) and ($all_chapters->total < $MAX_LIMIT)) {
					$total_parts[] = tm_commissions_currency_symbol($all_chapters->currency, true) . floor($all_chapters->total) . " " . $all_chapters->currency;
				}

			}
		}
		if (sizeof($total_parts) > 0) {
			$total_text = _tm_commissions_format_multiple_currencies($total_parts);
			$summary_text .= "Combined, all chapters have earned a total of " . $total_text . " from community sales in the past " . $days . " days &mdash; " . $view_all_commissions_text . "\n";
		}
	}
	
	// add breaks
	$summary_text = str_replace("\n", "<br>\n", $summary_text);
	return $summary_text;
}

/**
 * Helper method to format multiple text
 * ie: X, Y and Z
 */
function _tm_commissions_format_multiple_currencies($total_parts) {

	$formatted_text = "";

	// single
	if (sizeof($total_parts) == 1) {
		return $total_parts[0];
	}

	// multiple
	for ($i = 0; $i < sizeof($total_parts); $i++) {

		if ($i == sizeof($total_parts) - 1) {
			$formatted_text .= " and " . $total_parts[$i];
		} elseif ($i > 1) {
			$formatted_text .= ", " . $total_parts[$i];
		} elseif ($i == 0) {
			$formatted_text = $total_parts[$i];
		}
	}

	return $formatted_text;
}

/**
 * Fetch chapter commissions from commissions processor callback
 */
function tm_commissions_get_chapter_commissions_summary($chapter_code = "", $days = "") {

  global $conf;

  if ($days == 0) {
    $days = "";
  }

  // construct url
  $token = $conf["tm_payments_reports_secret_token"];
  $url = $conf["tm_commissions_summary_url"] . "?token=" . $token . "&chapter_code=" . $chapter_code . "&days=" . $days;

  //open connection
  $ch = curl_init();

  //set the url, number of POST vars, POST data
  $CURLOPT_SSL_VERIFYHOST = 2;
  $CURLOPT_SSL_VERIFYPEER = true;
  if ($conf["tm_payments_process_check_certificate"] == false) {
    $CURLOPT_SSL_VERIFYHOST = 0;
    $CURLOPT_SSL_VERIFYPEER = false;
  }
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_VERBOSE, false);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, $CURLOPT_SSL_VERIFYPEER);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
  curl_setopt($ch, CURLOPT_TIMEOUT, $conf["tm_payments_process_timeout"]); // timeout in seconds

  //execute post
  $result = curl_exec($ch);

  if ($result === false) {
    return null;
  }

  //close connection
  curl_close($ch);

  // return results
  $res = json_decode($result);
  return $res;
}


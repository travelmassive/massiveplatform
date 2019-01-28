<?php

// take a "strava" URL and format it
// can take a variety of human entered formats

// fix user typos
$url = trim($url);
$url = str_replace(" ", "", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url

$strava_url = "";
$display_url = "";
$found_match = false;

// Case 1
// strava url
if (!$found_match) {
	if ((strpos(strtolower($url), "https://strava.com/") !== FALSE) or (strpos($url, "http://strava.com/") !== FALSE)) {
		$strava_url = str_ireplace("https://strava.com/", "https://strava.com/", $url);
		$strava_url = str_ireplace("http://strava.com/", "https://strava.com/", $strava_url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://www.strava.com/") !== FALSE) or (strpos($url, "http://www.strava.com/") !== FALSE)) {
		$strava_url = str_ireplace("https://www.strava.com/", "https://strava.com/", $url);
		$strava_url = str_ireplace("http://www.strava.com/", "https://strava.com/", $strava_url);
		$found_match = true;
	}
}


// Case 2
// strava username
// Anything not starting with http or www
if (!$found_match) {
	if (($strava_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE) and (strpos(strtolower($url), "%20") === FALSE) and (strtolower($url) != "n/a")) {
		$strava_url = "https://www.strava.com/athletes/" . $url;
		$found_match = true;
	}
}


// remove http and https
if ($display_url == "") {
	$display_url = $strava_url;
	$display_url = str_ireplace("http://www.", "", $display_url);
	$display_url = str_ireplace("https://www.", "", $display_url);
	$display_url = str_ireplace("http://", "", $display_url);
	$display_url = str_ireplace("https://", "", $display_url);
}

// sanitize
$strava_url = check_url($strava_url);
$display_url = check_plain($display_url);

if (($strava_url != "") && ($url != "")) { ?>
<a href="<?php print $strava_url; ?>" rel="nofollow noopener" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-strava")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>
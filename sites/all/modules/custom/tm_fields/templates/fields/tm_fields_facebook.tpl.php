<?php

// take a "facebook" URL and format it
// can take a variety of human entered formats

// fix user typos
$url = trim($url);
$url = str_replace(" ", "", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url

$facebook_url = "";
$display_url = "";
$found_match = false;

// Case 1
// facebook url
if (!$found_match) {
	if ((strpos(strtolower($url), "https://facebook.com/") !== FALSE) or (strpos($url, "http://facebook.com/") !== FALSE)) {
		$facebook_url = str_ireplace("https://facebook.com/", "https://facebook.com/", $url);
		$facebook_url = str_ireplace("http://facebook.com/", "https://facebook.com/", $facebook_url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://www.facebook.com/") !== FALSE) or (strpos($url, "http://www.facebook.com/") !== FALSE)) {
		$facebook_url = str_ireplace("https://www.facebook.com/", "https://facebook.com/", $url);
		$facebook_url = str_ireplace("http://www.facebook.com/", "https://facebook.com/", $facebook_url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://m.facebook.com/") !== FALSE) or (strpos($url, "http://m.facebook.com/") !== FALSE)) {
		$facebook_url = str_ireplace("https://m.facebook.com/", "https://facebook.com/", $url);
		$facebook_url = str_ireplace("http://m.facebook.com/", "https://facebook.com/", $facebook_url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://web.facebook.com/") !== FALSE) or (strpos($url, "http://web.facebook.com/") !== FALSE)) {
		$facebook_url = str_ireplace("https://web.facebook.com/", "https://facebook.com/", $url);
		$facebook_url = str_ireplace("http://web.facebook.com/", "https://facebook.com/", $facebook_url);
		$found_match = true;
	}
}


// Case 2
// facebook username
// Anything not starting with http or www
if (!$found_match) {
	if (($facebook_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
		$facebook_url = "https://facebook.com/" . $url;
		$found_match = true;
	}
}


// remove http and https
if ($display_url == "") {
	$display_url = $facebook_url;
	$display_url = str_ireplace("http://", "", $display_url);
	$display_url = str_ireplace("https://", "", $display_url);
}

// sanitize
$facebook_url = check_url($facebook_url);
$display_url = check_plain($display_url);

if (($facebook_url != "") && ($url != "")) { ?>
<a href="<?php print $facebook_url; ?>" rel="nofollow" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-facebook")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>
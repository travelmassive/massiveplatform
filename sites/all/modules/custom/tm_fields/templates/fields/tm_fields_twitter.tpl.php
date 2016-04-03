<?php

// take a "twitter" URL and format it
// can take a variety of human entered formats

// fix user typos
$url = trim($url);
$url = str_replace(" ", "", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url

$twitter_url = "";
$display_url = "";
$found_match = false;

// Case 1.
// @twittername
if (!$found_match) {
	if ((strpos($url, "@") == 0) && (strpos($url, "@") !== FALSE)) {
		$twitter_url = "https://twitter.com/" . str_replace("@", "", $url);
		//$display_url = $url;
		$found_match = true;
	}
}

// Case 2
// #hashtag
// link to search
if (!$found_match) {
	if ((strpos($url, "#") == 0) && (strpos($url, "#") !== FALSE)) {
		$twitter_url = "https://twitter.com/search?q=%23" . str_replace("#", "", $url);
		$display_url = $url;
		$found_match = true;
	}
}


// Case 3
// twitter url
if (!$found_match) {
	if ((strpos(strtolower($url), "https://twitter.com/") !== FALSE) or (strpos($url, "http://twitter.com/") !== FALSE)) {
		$twitter_url = str_ireplace("https://twitter.com/", "https://twitter.com/", $url);
		$twitter_url = str_ireplace("http://twitter.com/", "https://twitter.com/", $url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://www.twitter.com/") !== FALSE) or (strpos($url, "http://www.twitter.com/") !== FALSE)) {
		$twitter_url = str_ireplace("https://www.twitter.com/", "https://twitter.com/", $url);
		$twitter_url = str_ireplace("http://www.twitter.com/", "https://twitter.com/", $url);
		$found_match = true;
	}
}


// Case 4.
// twittername
// Anything not starting with http or www
if (!$found_match) {
	if (($twitter_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
		$twitter_url = "https://twitter.com/" . $url;
		$found_match = true;
	}
}



// remove http and https
if ($display_url == "") {
	$display_url = $twitter_url;
	$display_url = str_ireplace("http://", "", $display_url);
	$display_url = str_ireplace("https://", "", $display_url);
}

// sanitize
$twitter_url = check_url($twitter_url);
$display_url = check_plain($display_url);

if (($twitter_url != "") && ($url != "")) { ?>
<a href="<?php print $twitter_url; ?>" rel="nofollow" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-twitter")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>
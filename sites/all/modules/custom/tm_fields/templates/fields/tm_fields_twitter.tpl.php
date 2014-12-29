<?php

// take a "twitter" URL and format it
// can take a variety of human entered formats

// strip spaces
$url = str_replace(" ", "", $url);
$twitter_url = "";
$display_url = "";

// Case 1.
// @twittername
if (strpos($url, "@") == 0) {
	$twitter_url = "https://twitter.com/" . str_replace("@", "", $url);
}

// Case 2
// #hashtag
// link to search
if (strpos($url, "#") === 0) {
	$twitter_url = "https://twitter.com/search?q=%23" . str_replace("#", "", $url);
	$display_url = $url;
}

// Case 3
// twitter url
if ((strpos(strtolower($url), "https://twitter.com/") === 0) or (strpos($url, "http://twitter.com/") === 0)) {
	$twitter_url = str_ireplace("https://twitter.com/", "https://twitter.com/", $url);
	$twitter_url = str_ireplace("http://twitter.com/", "https://twitter.com/", $url);
}
if (strpos(strtolower($url), "http://www.twitter.com/") === 0) {
	$twitter_url = str_ireplace("http://www.twitter.com/", "https://twitter.com/", $url);
}

// Case 4.
// twittername
// Anything not starting with http or www
if (($twitter_url == "") and (strpos(strtolower($url), "http") == FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
	$twitter_url = "https://twitter.com/" . $url;
}

// remove http and https
if ($display_url == "") {
	$display_url = $twitter_url;
	$display_url = str_ireplace("http://", "", $display_url);
	$display_url = str_ireplace("https://", "", $display_url);
}

if ($twitter_url != "") { ?>
<a href="<?php print $twitter_url; ?>" rel="nofollow" target="_blank"><?php print $display_url; ?></a>
<?php } ?>
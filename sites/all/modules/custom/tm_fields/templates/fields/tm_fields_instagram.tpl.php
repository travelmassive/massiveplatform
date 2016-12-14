<?php

// take a "instagram" URL and format it
// can take a variety of human entered formats

// fix user typos
$url = trim($url);
$url = str_replace(" ", "", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url

$instagram_url = "";
$display_url = "";
$found_match = false;

// Case 1.
// @instaname
if (!$found_match) {
	if ((strpos($url, "@") == 0) && (strpos($url, "@") !== FALSE)) {
		$instagram_url = "https://instagram.com/" . str_replace("@", "", $url);
		//$display_url = $url;
		$found_match = true;
	}
}

// Case 2
// #hashtag
// link to tag search
if (!$found_match) {
	if ((strpos($url, "#") == 0) && (strpos($url, "#") !== FALSE)) {
		$instagram_url = "https://instagram.com/explore/tags/" . str_replace("#", "", $url);
		$display_url = $url;
		$found_match = true;
	}
}


// Case 3
// instagram url
if (!$found_match) {
	if ((strpos(strtolower($url), "https://instagram.com/") !== FALSE) or (strpos($url, "http://instagram.com/") !== FALSE)) {
		$instagram_url = str_ireplace("https://instagram.com/", "https://instagram.com/", $url);
		$instagram_url = str_ireplace("http://instagram.com/", "https://instagram.com/", $instagram_url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://www.instagram.com/") !== FALSE) or (strpos($url, "http://www.instagram.com/") !== FALSE)) {
		$instagram_url = str_ireplace("https://www.instagram.com/", "https://instagram.com/", $url);
		$instagram_url = str_ireplace("http://www.instagram.com/", "https://instagram.com/", $instagram_url);
		$found_match = true;
	}
}


// Case 4.
// instagramname
// Anything not starting with http or www
if (!$found_match) {
	if (($instagram_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
		$instagram_url = "https://instagram.com/" . $url;
		$found_match = true;
	}
}

// remove http and https
if ($display_url == "") {
	$display_url = $instagram_url;
	$display_url = str_ireplace("http://", "", $display_url);
	$display_url = str_ireplace("https://", "", $display_url);
}

// sanitize
$instagram_url = check_url($instagram_url);
$display_url = check_plain($display_url);

if (($instagram_url != "") && ($url != "")) { ?>
<a href="<?php print $instagram_url; ?>" rel="nofollow" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-instagram")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>

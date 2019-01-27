<?php

// take a "youtube" URL and format it
// can take a variety of human entered formats

// fix user typos
$url = trim($url);
$url = str_replace(" ", "", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url

$youtube_url = "";
$display_url = "";
$found_match = false;

// Case 1
// youtube url
if (!$found_match) {
	if ((strpos(strtolower($url), "https://youtube.com/") !== FALSE) or (strpos($url, "http://youtube.com/") !== FALSE)) {
		$youtube_url = str_ireplace("https://youtube.com/", "https://youtube.com/", $url);
		$youtube_url = str_ireplace("http://youtube.com/", "https://youtube.com/", $youtube_url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://www.youtube.com/") !== FALSE) or (strpos($url, "http://www.youtube.com/") !== FALSE)) {
		$youtube_url = str_ireplace("https://www.youtube.com/", "https://youtube.com/", $url);
		$youtube_url = str_ireplace("http://www.youtube.com/", "https://youtube.com/", $youtube_url);
		$found_match = true;
	}
}


// Case 2
// youtube user
// Anything not starting with http or www
if (!$found_match) {
	if (($youtube_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
		$youtube_url = "https://youtube.com/user/" . $url;
		$found_match = true;
	}
}


// remove http and https
if ($display_url == "") {
	$display_url = $youtube_url;
	$display_url = str_ireplace("http://", "", $display_url);
	$display_url = str_ireplace("https://", "", $display_url);
}

// sanitize
$youtube_url = check_url($youtube_url);
$display_url = check_plain($display_url);

if (($youtube_url != "") && ($url != "")) { ?>
<a href="<?php print $youtube_url; ?>" rel="nofollow noopener" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-youtube")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>
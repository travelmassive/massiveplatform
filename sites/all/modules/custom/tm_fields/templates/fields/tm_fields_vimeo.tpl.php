<?php

// take a "vimeo" URL and format it
// can take a variety of human entered formats

// fix user typos
$url = trim($url);
$url = str_replace(" ", "", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url

$vimeo_url = "";
$display_url = "";
$found_match = false;

// Case 1
// vimeo url
if (!$found_match) {
	if ((strpos(strtolower($url), "https://vimeo.com/") !== FALSE) or (strpos($url, "http://vimeo.com/") !== FALSE)) {
		$vimeo_url = str_ireplace("https://vimeo.com/", "https://vimeo.com/", $url);
		$vimeo_url = str_ireplace("http://vimeo.com/", "https://vimeo.com/", $vimeo_url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://www.vimeo.com/") !== FALSE) or (strpos($url, "http://www.vimeo.com/") !== FALSE)) {
		$vimeo_url = str_ireplace("https://www.vimeo.com/", "https://vimeo.com/", $url);
		$vimeo_url = str_ireplace("http://www.vimeo.com/", "https://vimeo.com/", $vimeo_url);
		$found_match = true;
	}
}


// Case 2
// vimeo username
// Anything not starting with http or www
if (!$found_match) {
	if (($vimeo_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
		$vimeo_url = "https://vimeo.com/" . $url;
		$found_match = true;
	}
}


// remove http and https
if ($display_url == "") {
	$display_url = $vimeo_url;
	$display_url = str_ireplace("http://", "", $display_url);
	$display_url = str_ireplace("https://", "", $display_url);
}

// sanitize
$vimeo_url = check_url($vimeo_url);
$display_url = check_plain($display_url);

if (($vimeo_url != "") && ($url != "")) { ?>
<a href="<?php print $vimeo_url; ?>" rel="nofollow noopener" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-vimeo")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>

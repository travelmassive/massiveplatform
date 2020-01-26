<?php

// take a "tiktok" username and format it
// can take a variety of human entered formats

// fix user typos
$url = trim($url);
$url = str_replace(" ", "", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url
$url = str_replace("@", "", $url); // remove @
$url = str_replace("'", "", $url); // remove '
$url = str_replace("\"", "", $url); // remove "
$url = str_replace("<", "", $url); // remove <
$url = str_replace("/", "", $url); // remove /


$tiktok_url = "";
$display_url = "";
$found_match = false;

// Case 1
// tiktok username
if (!$found_match) {
	if (($tiktok_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE) and (strpos(strtolower($url), "%20") === FALSE) and (strtolower($url) != "na")) {
		$display_url = "tiktok.com/@" . trim($url);
		// https://tiktok.com/@travelmassive
		$tiktok_url = "https://tiktok.com/@" . trim($url);
		$found_match = true;
	}
}

// sanitize
$tiktok_url = check_url($tiktok_url);
$display_url = check_plain($display_url);

// show tiktok url
if (($tiktok_url != "") && ($url != "")) { ?>
<a href="<?php print $tiktok_url; ?>" rel="nofollow noopener" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-tiktok")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>
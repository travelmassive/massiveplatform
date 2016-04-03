<?php

// take a "snapchat" username and format it
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


$snapchat_url = "";
$display_url = "";
$found_match = false;

// Case 1
// snapchat username
if (!$found_match) {
	if (($snapchat_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
		$display_url = trim($url);
		// https://www.snapchat.com/add/travelmassive
		$snapchat_url = "https://www.snapchat.com/add/" . trim($url);
		$found_match = true;
	}
}

// sanitize
$snapchat_url = check_url($snapchat_url);
$display_url = check_plain($display_url);

// show snapchat url
if (($snapchat_url != "") && ($url != "")) { ?>
<a href="<?php print $snapchat_url; ?>" rel="nofollow" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-snapchat")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>
<?php

$display_url = $url;

// check url
$render_url = true;
if (filter_var($url, FILTER_VALIDATE_URL) === FALSE) {
	$render_url = false;
}

// add http when it's not given
if (strpos("http", $url) === 0) {
	$url = "http://" . $url; // add http://
}

// remove http and https
$display_url = str_ireplace("http://", "", $display_url);
$display_url = str_ireplace("https://", "", $display_url);

// if trailing slash on domain, remove it
if (substr_count($display_url, "/") == 1) {
	$display_url = rtrim($display_url, '/');
}

if ($render_url) {
?>
<a href="<?php print $url; ?>" rel="nofollow" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
&nbsp;
<?php } ?>
<?php

$display_url = $url;

// remove http and https
$display_url = str_ireplace("http://", "", $display_url);
$display_url = str_ireplace("https://", "", $display_url);

// if trailing slash on domain, remove it
if (substr_count($display_url, "/") == 1) {
	$display_url = rtrim($display_url, '/');
}

?>
<a href="<?php print $url; ?>" rel="nofollow" target="_blank"><?php print $display_url; ?></a>
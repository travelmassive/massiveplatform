<?php


// fix user typos
$url = trim($url);
$url = str_replace(" ", "", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_replace("http//", "http://", $url); // replace http// with http:// in url

$display_url = $url;

// add http when it's not given
if (strpos(strtolower($url), "http") === FALSE) {
	$url = "http://" . $url; // add http://
}

// check url
$render_url = true;
if (filter_var($url, FILTER_VALIDATE_URL) === FALSE) {
	$render_url = false;
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
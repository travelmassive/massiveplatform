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
// Anything not starting with http or www
// Link to snapchat protocol (iOS and Android)
if (!$found_match) {
	if (($snapchat_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
		$display_url = trim($url);
		$snapchat_url = "snapchat://?u=" . $url; // deprecated
		$found_match = true;
	}
}

// sanitize
$snapchat_url = str_replace("<", "", $snapchat_url);
$display_url = check_plain($display_url);

/* deprecated uri */
/* <a onClick="javascript:jq_confirm_url('View <?php print(check_url($url));?> on Snapchat', 'View this members\'s snaps? Requires <a href=\'https://www.snapchat.com/\' target=\'_blank\' rel=\'nofollow\'>Snapchat</a> app', '<?php print($snapchat_url);?>');" href="javascript:void(0);"><?php print $display_url; ?></a> */
if ($snapchat_url != "") { ?>
<?php print ($display_url); ?>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-snapchat")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>
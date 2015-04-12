<?php

$feedme_mode = ""; // frontpage, chapter, company
$feedme_query = ""; // ie: Berlin 
$feedme_theme = ""; // empty, or "dark" for dark theme
$feedme_off = true; // turn off by default
$feedme_style = ""; // add style to the #feedme div

// user
if (arg(0) == 'user' and is_numeric(arg(1)) and arg(2) == FALSE) {
	$profile = user_load(arg(1));
	$feedme_mode = "member";
    $feedme_query = check_plain($profile->realname);
    $feedme_theme = "light";
    $feedme_off = false;
   	$feedme_append = ".column.first";
}

// node (chapter or company)
if (arg(0) == 'node' and is_numeric(arg(1)) and arg(2) == FALSE) {

	$node = node_load(arg(1));
	$feedme_theme = "light";
	$feedme_append = ".column.first";

	if ($node->type == "chapter") {
		$feedme_mode = "chapter";
		$feedme_query = $node->title; // ie: Sydney
		$feedme_off = false;
	}

	if ($node->type == "organization") {
		$feedme_mode = "company";
		$feedme_query = $node->title; // ie: GoEuro
		$feedme_off = false;
	}
}

// front page
if ($is_front) {
	$feedme_mode = "frontpage";
    $feedme_query = "";
    $feedme_theme = "light";
    $feedme_off = false;
    $feedme_append = "#main";
    $feedme_style = "margin: 0 auto; width: 100%; text-align: center; margin-top: 64px;";
}

if (!$feedme_off) {
?>

<script>
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

feedme_append = "<?php print($feedme_append);?>";
$( feedme_append ).append( "<div id='feedme' style='<?php print($feedme_style);?>'></div>" );

function load_feedme() {
	// set feedme_mode and feedme_q
	feedme_mode = "<?php print($feedme_mode);?>";
    feedme_query = "<?php print($feedme_query);?>";
    feedme_theme = "<?php print($feedme_theme);?>";
    feedme_frontpage_id = "<?php global $conf; print($conf["tm_wordpress_feedme_frontpage_id"]);?>";
    feedme_base_url = "<?php global $conf; print($conf["tm_wordpress_feedme_url"]);?>";
    feedme_url = feedme_base_url + "?mode=" + feedme_mode + "&q=" + encodeURIComponent(feedme_query) + "&theme=" + feedme_theme;
    if (feedme_mode == "frontpage") {
    	feedme_url = feedme_url + "&frontpage_id=" + feedme_frontpage_id;
    }
    //console.log(feedme_url);
    $("#feedme").load(feedme_url);
}

load_feedme(); // This will run on page load

});})(jQuery, Drupal, this, this.document);
</script>

<?php 
} // end if
?>
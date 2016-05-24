<?php

$feedme_mode = ""; // frontpage, chapter, company
$feedme_company = ""; // ie: Travelstart
$feedme_chapter = ""; // ie: Berlin
$feedme_country = ""; // ie: Germany 
$feedme_theme = ""; // empty, or "dark" for dark theme
$feedme_off = true; // turn off by default
$feedme_style = ""; // add style to the #feedme div
$feedme_num_items = 5; // number of items to display

// node (chapter or company)
if (arg(0) == 'node' and is_numeric(arg(1)) and arg(2) == FALSE) {

	$node = node_load(arg(1));
	$feedme_theme = "light";
	$feedme_append = ".column.first";

    if ($node != null) {
        
        if ($node->type == "chapter") {
            $feedme_mode = "chapter";

            $feedme_chapter = $node->title; // ie: Sydney
            
            // lookup country
            try {
                if (@isset($node->field_country['und'][0]['iso2'])) {
                    $country_code = $node->field_country['und'][0]['iso2'];
                    $country = country_load($country_code);
                    if ($country != null) {
                        $feedme_country = $country->name;
                    }
                }
            } catch (Exception $e) {
                // can't find country code
            }

            $feedme_off = false;
        }

        if ($node->type == "organization") {
            $feedme_mode = "company";
            $feedme_company = $node->title; // ie: GoEuro
            $feedme_off = false;
        }
    }
	
}

if (!$feedme_off) {
?>

<script>
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

feedme_append = "<?php print($feedme_append);?>";
$( feedme_append ).append( "<div id='marketplace_feedme' style='<?php print($feedme_style);?>'></div>" );

function load_marketplace_feedme() {
	// set feedme_mode and feedme_q
	feedme_mode = "<?php print(htmlentities($feedme_mode, ENT_QUOTES));?>";
    feedme_company = "<?php print(htmlentities($feedme_company, ENT_QUOTES));?>";
    feedme_chapter = "<?php print(htmlentities($feedme_chapter, ENT_QUOTES));?>";
    feedme_country = "<?php print(htmlentities($feedme_country, ENT_QUOTES));?>";
    feedme_theme = "<?php print($feedme_theme);?>";
    feedme_num_items = "<?php print($feedme_num_items);?>";
    feedme_base_url = "<?php global $conf; print($conf["tm_marketplace_feedme_url"]);?>";
    feedme_url = feedme_base_url;

    if (feedme_mode == "chapter") {
        feedme_url = feedme_url + "?city=" + encodeURIComponent(feedme_chapter) + '&country=' + encodeURIComponent(feedme_country);
    }

    if (feedme_mode == "company") {
        feedme_url = feedme_url + "?company=" + encodeURIComponent(feedme_company);
    }

    feedme_url = feedme_url + "&limit=" + feedme_num_items;
    
    $.get(feedme_url, function(data) {
        $("#marketplace_feedme").replaceWith(data);
        // if marketplace_feedme_loaded() function defined, call it
        // this can be used to display a div when the feed loads
        if (typeof(marketplace_feedme_loaded) == "function") {
            marketplace_feedme_loaded();
        }
    });
}

load_marketplace_feedme(); // This will run on page load

});})(jQuery, Drupal, this, this.document);
</script>

<?php 
} // end if
?>
<?php

$feedme_mode = ""; // chapter, or person
$feedme_category_id = ""; // vanilla forum category
$feedme_user_id = ""; // 
$feedme_off = true; // turn off by default
$feedme_num_items = 5; // number of items to display
$feedme_style = ""; 

// user
if (arg(0) == 'user' and is_numeric(arg(1)) and arg(2) == FALSE) {
	$profile = user_load(arg(1));
	$feedme_mode = "user";
	$feedme_user_id = $profile->uid;
   	$feedme_append = ".column.first";
   	$feedme_off = false;
}

// chapter or event
if (arg(0) == 'node' and is_numeric(arg(1)) and arg(2) == FALSE) {

    $node = node_load(arg(1));
    
    // chapter
    if ($node != null) {

    	if ($node->type == "chapter") {
    		$node = node_load(arg(1));
    		$feedme_mode = "category";
    		$feedme_category_id = "all"; // default is all

    		// category_id is either
    		// 1. a vanilla forum category (ie: category_id=30)
    		// 2. a comma delimeted list of classes to search (ie: chapter-berlin,country-germany,continent-eu)
    		if (isset($node->field_discuss_category_id[LANGUAGE_NONE][0]['value'])) {
    			$feedme_category_id = trim($node->field_discuss_category_id[LANGUAGE_NONE][0]['value']);
    		} else {
    			// ie: chapter-berlin
    			$feedme_category_id = "chapter-" . strtolower(str_replace(" ", "_", $node->title));

    			// lookup country and continent
                if (isset($node->field_country[LANGUAGE_NONE][0]['iso2'])) {
                    $country_code = $node->field_country[LANGUAGE_NONE][0]['iso2'];
                    $country = country_load($country_code);
                    if ($country != null) {
                    	// ie: country-germany
                    	$feedme_category_id .= ",country-" . strtolower($country_code);

                    	// ie: continent-europe
                    	$feedme_category_id .= ",continent-" . strtolower($country->continent);
                    }
                }
    		}
    		$feedme_append = ".column.first";
    		$feedme_off = false;
    	}

        // event
        if ($node->type == "event") {
            $node = node_load(arg(1));
            $feedme_mode = "category";
            
            // category_id is either
            // 1. a vanilla forum category (ie: category_id=30)
            // 2. a comma delimeted list of classes to search
            if (isset($node->field_event_discuss_category_id[LANGUAGE_NONE][0]['value'])) {
                $feedme_category_id = trim($node->field_event_discuss_category_id[LANGUAGE_NONE][0]['value']);
                $feedme_append = ".column.first";
                $feedme_off = false;
            }
        }
    }
}

if (!$feedme_off) {
?>

<script>
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){

feedme_append = "<?php print($feedme_append);?>";
$( feedme_append ).append( "<div id='discuss_feedme' style='<?php print($feedme_style);?>'></div>" );

function load_discuss_feedme() {

	// set parameters for query
	// ie: discussions/massiveplatform/feedme/MODE/NUM_ITEMS/CATEGORY_ID/USER_ID
	feedme_mode = "<?php print(htmlentities($feedme_mode, ENT_QUOTES));?>";
    feedme_num_items = "<?php print($feedme_num_items);?>";
    feedme_category_id = "<?php print($feedme_category_id);?>";
    feedme_user_id = "<?php print($feedme_user_id);?>";
    feedme_base_url = "/<?php global $conf; print($conf["tm_discuss_full_path"]);?>/massiveplatform/feedme";
    feedme_url = feedme_base_url + "/render?mode=" + feedme_mode + "&category_id=" + feedme_category_id + "&user_id=" + feedme_user_id + "&num_items=" + feedme_num_items;
    
    $.get(feedme_url, function(data) {
        $("#discuss_feedme").replaceWith(data);
        // if feedme_loaded() function defined, call it
        // this can be used to display a div when the feed loads
        if (typeof(discuss_feedme_loaded) == "function") {
            discuss_feedme_loaded();
        }
    });
}

load_discuss_feedme(); // This will run on page load

});})(jQuery, Drupal, this, this.document);
</script>

<?php 
} // end if
?>
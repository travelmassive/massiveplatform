<?php

$entity_type = "";
$entity_id = "";
$feedme_num_items = 5; // number of items to display
$feedme_append = ".column.first";

// user
if (arg(0) == 'user' and is_numeric(arg(1)) and arg(2) == FALSE) {
	$entity_type = "user";
    $entity_id = arg(1);
}

// node (chapter or company)
if (arg(0) == 'node' and is_numeric(arg(1)) and arg(2) == FALSE) {
	$entity_type = "node";
    $entity_id = arg(1);	
}

// check if turned off on event pages
if (isset($conf["tm_lists_feedme_hide_events"])) {
    if ($conf["tm_lists_feedme_hide_events"]) {
        $current_path = current_path();
        $path_alias = drupal_get_path_alias($current_path);
        if (strpos($path_alias, "events") !== false) {
            return;
        }
    }
}
?>

<script>
jQuery(document).ready(function(){

feedme_append = "<?php print($feedme_append);?>";
jQuery(feedme_append).append( "<div id='lists-feedme'></div>" );

function load_feedme_lists() {
	// set parameters
	entity_type = "<?php print($entity_type); ?>";
    entity_id = "<?php print($entity_id); ?>";
    feedme_num_items = "<?php print($feedme_num_items);?>";
    feedme_base_url = "/lists/feedme";
    feedme_url = feedme_base_url + "?entity_type=" + entity_type + "&entity_id=" + entity_id + "&num_items=" + feedme_num_items;
    jQuery.get(feedme_url, function(data) {
        jQuery("#lists-feedme").replaceWith(data);
        // if feedme_lists_loaded() function defined, call it
        // this can be used to display a div when the feed loads
        if (typeof(feedme_lists_loaded) == "function") {
            feedme_lists_loaded();
        }
    });
}

load_feedme_lists(); // This will run on page load

});
</script>

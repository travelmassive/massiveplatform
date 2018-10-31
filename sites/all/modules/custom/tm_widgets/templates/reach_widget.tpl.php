<?php
global $conf;
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Community Reach</title>
	<link rel="stylesheet" href="<?php print($conf["tm_widgets_chosen_css_uri"]);?>">
</head>
<body>

<style>
.exploreReachSelect {
	width: 400px ;
}

#exploreReachResults {
	margin-top: 2rem;
}

.exploreReachResult {
	display: block;
}

.exploreReachResultLHS {
	font-style: italic;
	margin-right: 8px;
	min-width: 180px;
    display: inline-block;
}

.exploreReachResultRHS {
}

.exploreReachResultContainer { 
	margin-top: 1rem;
}

</style>
<script src="<?php print($conf["tm_widgets_jquery_js_uri"]);?>"></script>
<script src="<?php print($conf["tm_widgets_chosen_js_uri"]);?>"></script>

<script>

$(document).ready(function(){
	
	//Chosen
	$(".exploreReachSelect").chosen({placeholder_text_multiple: "Filter by region, country, or chapter"});

	//Logic
	$(".exploreReachSelect").change(function(){
		doExploreReach();
	});

	function doExploreReach() {

		$.ajax({
			url: "/widgets/public/reach-results",
			data: {
				chapters: $('.exploreReachSelect').val()
			},
			type: "GET",
			dataType: "html",
			success: function (data) {
				$('#exploreReachResults').html(data);
			},
			error: function (xhr, status) {
				$('#exploreReachResults').html("Sorry, there was a problem fetching the results. Try again later.");
			},
			complete: function (xhr, status) {
				//$('#showresults').slideDown('slow')
			}
		});

		$("#exploreReachResults").html('<img class="tm-status-update-pager-loading-image" src="/sites/all/themes/tm/images/load-more-ajax-loader.gif">');
	}

	// First load
	doExploreReach();

});

</script>

<?php

$region_names = array(
  "AF" => "Africa",
  "AN" => "Antarctica",
  "AS" => "Asia",
  "EU" => "Europe",
  "NA" => "North America",
  "OC" => "Oceania",
  "SA" => "South Africa");

$query = "SELECT field_data_field_country.entity_id, node.title, countries_country.name as country, countries_country.iso2 as iso2, countries_country.continent as region 
FROM field_data_field_country 
LEFT JOIN countries_country ON countries_country.iso2 = field_data_field_country.field_country_iso2 
LEFT JOIN node ON field_data_field_country.entity_id = node.nid 
WHERE field_data_field_country.bundle = 'chapter' AND field_data_field_country.deleted = 0 ORDER BY countries_country.continent";
  $all_chapters = db_query($query)->fetchAll();

  $chapter_uids = array();

$regions = tm_base_get_all_regions();
?>
<select multiple class="exploreReachSelect">

<option value="ALL-CHAPTERS">Worldwide &gt; All Chapters</option>

<?php 
	$last_region = null;
	$last_country = null;
	foreach($all_chapters as $chapter) {
?>

<?php
	// All Countries in Region
	if ($chapter->region != $last_region) {
		$last_region = $chapter->region;
?>
<option value="REGION-<?php print($chapter->region);?>"><?php print($region_names[$chapter->region]);?> &gt; All Chapters</option>
<?php	} // end new region ?>

<?php
	// All Chapters in Country
	if ($chapter->country != $last_country) {
		$last_country = $chapter->country;
?>
<option value="COUNTRY-<?php print($chapter->iso2);?>"><?php print($region_names[$chapter->region]);?> &gt; <?php print($chapter->country);?> &gt; All Chapters</option>
<?php	} // end new country ?>

<option value="CHAPTER-<?php print($chapter->entity_id);?>"><?php print($region_names[$chapter->region]);?> &gt; <?php print($chapter->country);?> &gt; <?php print($chapter->title);?></option>

<?php } // end foreach ?>

</select>

<div id='exploreReachResults'></div>

</body>
</html>
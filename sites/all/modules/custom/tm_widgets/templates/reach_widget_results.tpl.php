<?php

// STEP 1. PARSE INPUT DATA
$chapter_ids = array();
$all_chapters = false;
if (isset($_GET["chapters"])) {
	if (is_array($_GET["chapters"])) {
		foreach ($_GET["chapters"] as $input) {

			// ALL-CHAPTERS
			if ((strpos($input, "ALL-CHAPTERS")) === 0) {
				$ids = tm_chapters_get_all_chapters();
				$all_chapters = true;
				foreach ($ids as $id) {
					$chapter_ids[] = $id;
				}
			}

			// REGION-OC, REGION-NA, etc
			if ((strpos($input, "REGION-")) === 0) {
				$parts = explode("REGION-", $input);
				if (sizeof($parts) > 1) {
					$ids = _tm_chapters_get_chapters_in_continent(strip_tags($parts[1]));
					foreach ($ids as $id) {
						$chapter_ids[] = $id;
					}
				}
			}

			// COUNTRY-AU, COUNTRY-US, etc
			if ((strpos($input, "COUNTRY-")) === 0) {
				$parts = explode("COUNTRY-", $input);
				if (sizeof($parts) > 1) {
					$ids = _tm_chapters_get_chapters_in_country(strip_tags($parts[1]));
					foreach ($ids as $id) {
						$chapter_ids[] = $id;
					}
				}
			}

			// CHAPTER-123, CHAPTER-124, etc
			if ((strpos($input, "CHAPTER-")) === 0) {
				$parts = explode("CHAPTER-", $input);
				if (sizeof($parts) > 1) {
					if (is_numeric($parts[1])) {
						$chapter = node_load($parts[1]);
						if ($chapter != null) {
							if ($chapter->type == "chapter") {
								$chapter_ids[] = $parts[1];
							}
						}
					}
				}
			}
		}
	}
}

// default to global
if (sizeof($chapter_ids) == 0) {
	$chapter_ids = tm_chapters_get_all_chapters();
	$all_chapters = true;
}

// remove duplicates
$chapter_ids = array_unique($chapter_ids);

// STEP 2. FETCH CHAPTER INSIGHTS
$insights = array();
if (sizeof($chapter_ids) > 1) {
	$insights[] = array("Chapters", sizeof($chapter_ids));
}

// fetch chapter stats
$cache_seconds = 0;
if ($all_chapters) {
	$chapter_insights = _tm_reports_get_global_insights();
} else {
	$chapter_insights = _tm_reports_get_chapter_insights($chapter_ids, true, $cache_seconds);
}

if ($all_chapters) {
	$insights[] = array("Member Connections", $chapter_insights['data_values']['num_connections']);
} else {
	$insights[] = array("Member Connections", $chapter_insights['data_values']['total_chapter_member_connections']);
}

$insights[] = array("All event registrations", $chapter_insights['data_values']['num_event_registrations']);

if ($chapter_insights['data_values']['num_events_upcoming'] == 0) {
	$chapter_insights['data_values']['num_events_upcoming'] = "None";
}
$insights[] = array("Upcoming events", $chapter_insights['data_values']['num_events_upcoming']);
$insights[] = array("Events in past 12 months", $chapter_insights['data_values']['num_events_past_12_months']);


// STEP 3. FETCH INDUSTRY SEGMENTS
$industry_insights = array();

// get segment stats
if ($all_chapters) {
	$chapter_ids = "";
}

// industry data
$members_with_segment_pct = $chapter_insights['data_values']['members_with_segment_pct'];

// total members
if ($chapter_insights['data_values']['members_with_segment_pct'] == 100) {
	$industry_insights[] = array("All segments", $chapter_insights['data_values']['members_total'] . " members");
} else {
	$industry_insights[] = array("All segments", intval(($chapter_insights['data_values']['members_with_segment_pct']/100) * $chapter_insights['data_values']['members_total']) . "-" . $chapter_insights['data_values']['members_total'] . " members");
}

// segments
$industry_data = _tm_reports_get_industry_segment_data($chapter_ids);
foreach($industry_data as $data) {
	if ($data['total'] == intval($data['total'] * (100/$members_with_segment_pct))) {
		$industry_insights[] = array($data['name'], $data['total'] . " " . tm_widgets_pluralize_members($data['total']));
	} else {
		$industry_insights[] = array($data['name'], $data['total'] . "-" . intval($data['total'] * (100/$members_with_segment_pct)) . " " . tm_widgets_pluralize_members(intval($data['total'] * (100/$members_with_segment_pct))));
	}
}

// STEP 4. RENDER RESULTS
print "<div class='exploreReachResultContainer'>";
foreach ($industry_insights as $insight) {
	print "<span class='exploreReachResult'><span class='exploreReachResultLHS'>" . $insight[0] . "</span><span class='exploreReachResultRHS'>" . $insight[1] . "</span></span>";
}
print "</div>";

// render insights
print "<div class='exploreReachResultContainer'>";
foreach ($insights as $label => $insight) {
	print "<span class='exploreReachResult'><span class='exploreReachResultLHS'>" . $insight[0] . "</span><span class='exploreReachResultRHS'>" . $insight[1] . "</span></span>";
}
print "</div>";


?>
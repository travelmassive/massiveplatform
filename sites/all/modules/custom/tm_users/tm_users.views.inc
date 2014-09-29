<?php

/* https://api.drupal.org/api/views/views.api.php/function/hook_views_query_alter/7 */
function tm_users_views_query_alter(&$view, &$query) {
   
   if($view->name=="chapter_leaders") {

   	// fetch the ids of chapter leaders
   	$result = db_query("select field_chapter_leaders_target_id from field_data_field_chapter_leaders group by field_chapter_leaders_target_id");
   	$chapter_leader_ids = array();
   	foreach ($result as $record) {
   		$chapter_leader_ids[] = $record->field_chapter_leaders_target_id;
   	}

 		// add a where clause to the chapter leaders query to only show chapter leaders
   	$query->add_where(1,'users.uid', $chapter_leader_ids, 'IN');
	}
}

?>
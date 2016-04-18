<?php

// tm_chapters.helpers.inc - chapter helper methods

/* Helper method get the number of members in a chapter 
 *  Will include non-approved members in the count
 */
function _tm_chapters_get_num_members($chapter_id) {
    $flag = flag_get_flag('signup');
    return $flag->get_count($chapter_id);
}


<?php

// people suggestions

function tm_people_suggest_global() {
    // get a pool of results to randomly select from
    $pool = 25;

    $results = db_query_range(TmStatusUpdatesSqlLibrary::$tm_sql_people_suggest_global, 0, $pool);

    return $results->fetchAll();
}

function tm_people_suggest_chapters_and_locations($user_id) {
    // get a pool of results to randomly select from
    $pool = 15;

    $results = db_query_range(TmStatusUpdatesSqlLibrary::$tm_sql_people_suggest_chapters_and_locations, 0, $pool,
        array(":user_id" => $user_id)
        );

    return $results->fetchAll();
}

function tm_people_suggest_job_roles($user_id) {
    // get a pool of results to randomly select from
    $pool = 15;

    $results = db_query_range(TmStatusUpdatesSqlLibrary::$tm_sql_people_suggest_job_roles, 0, $pool,
        array(":user_id" => $user_id)
        );

    return $results->fetchAll();
}

function tm_people_suggest_industry_segments($user_id) {
    // get a pool of results to randomly select from
    $pool = 15;

    $results = db_query_range(TmStatusUpdatesSqlLibrary::$tm_sql_people_suggest_industry_segments, 0, $pool,
        array(":user_id" => $user_id)
        );

    return $results->fetchAll();
}

class TmStatusUpdatesSqlLibrary
{

//
// Suggestions of members to follow
//

// global suggestions for users not logging in
public static $tm_sql_people_suggest_global = <<<SQL

select
        users.uid,
        IFNULL(user_follower_count.count, 0) as 'user_follower_count',
        IFNULL(user_chapter_count.count, 0) as 'user_chapter_count'

from users

left join flag_counts user_follower_count
on  user_follower_count.fid=1
and user_follower_count.entity_id = users.uid
and user_follower_count.entity_type='user'

left join flag_counts user_chapter_count
on  user_chapter_count.fid=3
and user_chapter_count.entity_id = users.uid
and user_chapter_count.entity_type='user'

left join users_roles
on  users.uid = users_roles.uid
and users_roles.rid = 6  -- approved_member

where
    -- exclude members that arent approved
    users_roles.uid is not null

order by
    -- matching hierarchy
    user_follower_count desc,
    users.created desc

SQL;


// suggestions based on similar chapters and locations
public static $tm_sql_people_suggest_chapters_and_locations = <<<SQL

select
        users.uid,
        CONCAT(users_first_name.field_user_first_name_value, ' ', users_last_name.field_user_last_name_value) as 'user_full_name',
        IFNULL(user_follower_count.count, 0) as 'user_follower_count',

        users_home_chapter_object.nid as 'home_chapter_id',
        users_home_chapter_object.title as 'home_chapter',

        case when LOWER(REPLACE(REPLACE(users_location.field_location_city_value,' ',''),',','')) = LOWER(REPLACE(REPLACE(my_location.field_location_city_value,' ',''),',',''))
        then 1 else 0 end as 'same_location_as_me',

        case when my_home_chapter.field_home_chapter_target_id = users_home_chapter.field_home_chapter_target_id
        then 1 else 0 end as 'same_home_chapter_as_me',

        case when my_home_chapter_country.field_country_iso2 = users_home_chapter_country.field_country_iso2
        then 1 else 0 end as 'our_home_chapters_in_same_country',

        case when users_home_country.field_user_country_iso2 = my_home_chapter_country.field_country_iso2
        then 1 else 0 end as 'same_country_as_my_home_chapter',

        case when users_home_country.field_user_country_iso2 = my_home_country.field_user_country_iso2
        then 1 else 0 end as 'same_country'

from users

left join field_data_field_location_city users_location
on users_location.entity_id=users.uid and users_location.bundle='user'
left join field_data_field_location_city my_location
on my_location.entity_id=:user_id and my_location.bundle='user'

left join field_data_field_user_first_name users_first_name
on users_first_name.entity_id=users.uid and users_first_name.bundle='user'
left join field_data_field_user_last_name users_last_name
on users_last_name.entity_id=users.uid and users_last_name.bundle='user'

left join field_data_field_home_chapter users_home_chapter
on users_home_chapter.entity_id = users.uid and users_home_chapter.entity_type='user'
left join field_data_field_home_chapter my_home_chapter
on my_home_chapter.entity_id = :user_id and my_home_chapter.entity_type='user'

left join node users_home_chapter_object
on users_home_chapter.field_home_chapter_target_id = users_home_chapter_object.nid and users_home_chapter_object.type='chapter'

left join field_data_field_country users_home_chapter_country
on users_home_chapter_country.bundle='chapter' and users_home_chapter_country.entity_id=users_home_chapter.field_home_chapter_target_id
left join field_data_field_country my_home_chapter_country
on my_home_chapter_country.bundle='chapter' and my_home_chapter_country.entity_id=my_home_chapter.field_home_chapter_target_id

left join field_data_field_user_country users_home_country
on users_home_country.bundle='user' and users_home_country.entity_id=users.uid
left join field_data_field_user_country my_home_country
on my_home_country.bundle='user' and my_home_country.entity_id=:user_id

left join flag_counts user_follower_count
on  user_follower_count.fid=1
and user_follower_count.entity_id = users.uid
and user_follower_count.entity_type='user'

left join flagging already_following
on  already_following.fid=1
and already_following.uid = :user_id
and already_following.entity_id = users.uid

left join users_roles
on  users.uid = users_roles.uid
and users_roles.rid = 6  -- approved_member

where
    -- dont include me
    users.uid <> :user_id
    -- exclude members that arent approved
    and users_roles.uid is not null
    -- exclude members that i already follow
    and already_following.uid IS NULL

order by
    -- matching hierarchy
    same_location_as_me desc,
    same_home_chapter_as_me desc,
    our_home_chapters_in_same_country desc,
    same_country_as_my_home_chapter desc,
    same_country desc,
    user_follower_count desc,
    users.created desc

SQL;


// suggestions based on similar job roles
public static $tm_sql_people_suggest_job_roles = <<<SQL

select
        users.uid,
        CONCAT(users_first_name.field_user_first_name_value, ' ', users_last_name.field_user_last_name_value) as 'user_full_name',
        IFNULL(user_follower_count.count, 0) as 'user_follower_count',

        my_job_role.field_job_role_value as 'my_job_role',
        user_job_role.field_job_role_value as 'user_job_role',

        case when LOWER(my_job_role.field_job_role_value) = LOWER(user_job_role.field_job_role_value)
        then 1 else 0 end as 'job_role_exact_match',

        -- first word matches
        case when SUBSTRING_INDEX(LOWER(my_job_role.field_job_role_value),' ',1) = SUBSTRING_INDEX(LOWER(user_job_role.field_job_role_value),' ',1)
        then 1 else 0 end as 'job_role_first_word_matches',

        -- second word matches
        case when SUBSTRING_INDEX(LOWER(my_job_role.field_job_role_value),' ',2) = SUBSTRING_INDEX(LOWER(user_job_role.field_job_role_value),' ',2)
        then 1 else 0 end as 'job_role_second_word_matches'

from users

left join field_data_field_user_first_name users_first_name
on users_first_name.entity_id=users.uid and users_first_name.bundle='user'
left join field_data_field_user_last_name users_last_name
on users_last_name.entity_id=users.uid and users_last_name.bundle='user'

left join flag_counts user_follower_count
on  user_follower_count.fid=1
and user_follower_count.entity_id = users.uid
and user_follower_count.entity_type='user'

left join flagging already_following
on  already_following.fid=1
and already_following.uid = :user_id
and already_following.entity_id = users.uid

left join users_roles
on  users.uid = users_roles.uid
and users_roles.rid = 6  -- approved_member

left join field_data_field_job_role my_job_role
on  my_job_role.entity_id = :user_id
and my_job_role.entity_type = 'user'

left join field_data_field_job_role user_job_role
on  user_job_role.entity_id = users.uid
and user_job_role.entity_type = 'user'

where
    -- dont include me
    users.uid <> :user_id
    -- exclude members that arent approved
    and users_roles.uid is not null
    -- exclude members that i already follow
    and already_following.uid IS NULL

order by
    -- matching hierarchy
    -- job_role_exact_match desc,  -- too specific, restricts diversity of results
    job_role_first_word_matches desc,
    -- job_role_second_word_matches desc,  -- too specific, restricts diversity of results
    user_follower_count desc,
    users.created desc

SQL;



// suggestions based on similar industry segments
public static $tm_sql_people_suggest_industry_segments = <<<SQL

select
        users.uid,
        CONCAT(users_first_name.field_user_first_name_value, ' ', users_last_name.field_user_last_name_value) as 'user_full_name',
        IFNULL(user_follower_count.count, 0) as 'user_follower_count',

        case -- 'my_industry_first_level'
        when my_ttd_parent.tid is null
        then my_ttd_child.name
        else my_ttd_parent.name
        end as 'my_industry_first_level',

        case -- 'my_industry_second_level'
        when my_ttd_parent.tid is null
        then null
        else my_ttd_child.name
        end as 'my_industry_second_level',

        case -- 'user_industry_first_level'
        when user_ttd_parent.tid is null
        then user_ttd_child.name
        else user_ttd_parent.name
        end as 'user_industry_first_level',

        case -- 'user_industry_second_level'
        when user_ttd_parent.tid is null
        then null
        else user_ttd_child.name
        end as 'user_industry_second_level',

        case
        when
            case -- 'user_industry_first_level'
            when user_ttd_parent.tid is null
            then user_ttd_child.tid
            else user_ttd_parent.tid
            end
            =
            case -- 'my_industry_first_level'
            when my_ttd_parent.tid is null
            then my_ttd_child.tid
            else my_ttd_parent.tid
            end
        then 1 else 0
        end as 'same_industry_first_level',

        case
        when
            case -- 'user_industry_second_level'
            when user_ttd_parent.tid is null
            then null
            else user_ttd_child.tid
            end
            =
            case -- 'my_industry_second_level'
            when my_ttd_parent.tid is null
            then null
            else my_ttd_child.tid
            end
        then 1 else 0
        end as 'same_industry_second_level'

from users

left join field_data_field_user_first_name users_first_name
on users_first_name.entity_id=users.uid and users_first_name.bundle='user'
left join field_data_field_user_last_name users_last_name
on users_last_name.entity_id=users.uid and users_last_name.bundle='user'

left join flag_counts user_follower_count
on  user_follower_count.fid=1
and user_follower_count.entity_id = users.uid
and user_follower_count.entity_type='user'

left join flagging already_following
on  already_following.fid=1
and already_following.uid = :user_id
and already_following.entity_id = users.uid

left join users_roles
on  users.uid = users_roles.uid
and users_roles.rid = 6  -- approved_member

left join taxonomy_entity_index user_tei on user_tei.entity_id = users.uid and user_tei.entity_type = 'user'
left join taxonomy_term_data user_ttd_child on user_tei.tid = user_ttd_child.tid
left join taxonomy_term_hierarchy user_tth on user_tei.tid=user_tth.tid
left join taxonomy_term_data user_ttd_parent on user_tth.parent=user_ttd_parent.tid

left join taxonomy_entity_index my_tei on my_tei.entity_id = :user_id and my_tei.entity_type = 'user'
left join taxonomy_term_data my_ttd_child on my_tei.tid = my_ttd_child.tid
left join taxonomy_term_hierarchy my_tth on my_tei.tid=my_tth.tid
left join taxonomy_term_data my_ttd_parent on my_tth.parent=my_ttd_parent.tid

where
    -- dont include me
    users.uid <> :user_id
    -- exclude members that arent approved
    and users_roles.uid is not null
    -- exclude members that i already follow
    and already_following.uid IS NULL

order by
    -- matching hierarchy
    same_industry_first_level desc,
    -- same_industry_second_level desc,  -- too specific, restricts diversity of results
    user_follower_count desc,
    users.created desc

SQL;

}

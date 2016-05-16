<?php

// tm_newsfeed.display.inc - newsfeed builder methods

// all queries in here

function tm_newsfeed_get_updates($user_id, $start_date, $end_date) {
	$results = db_query(TmSqlLibrary::$tm_sql_updates,
		array(":user_id" => $user_id, ":start_date" => strtotime($start_date), ":end_date" => strtotime($end_date))
		);

	return $results->fetchAll();
}


function tm_newsfeed_get_events($user_id, $from_date) {
	$results = db_query(TmSqlLibrary::$tm_sql_events,
		array(":user_id" => $user_id, ":from_date" => strtotime($from_date))
		);

	return $results->fetchAll();
}


function tm_newsfeed_get_new_members($user_id, $start_date, $end_date) {
	$results = db_query(TmSqlLibrary::$tm_sql_new_members, 
		array(":user_id" => $user_id, ":start_date" => strtotime($start_date), ":end_date" => strtotime($end_date))
		);

	return $results->fetchAll();
}

function tm_newsfeed_get_new_companies($user_id, $start_date, $end_date) {
	$results = db_query(TmSqlLibrary::$tm_sql_new_companies,
		array(":user_id" => $user_id, ":start_date" => strtotime($start_date), ":end_date" => strtotime($end_date))
		);

	return $results->fetchAll();
}

// How to get the row limit binding to work?
// $results->bindParam(':user_id', $user_id, \PDO::PARAM_INT);
// $results->bindParam(':row_limit', 5, \PDO::PARAM_INT);


// function tm_newsfeed_get_new_chapter_signups($start_date, $end_date) {
// 	$results = db_query(TmSqlLibrary::$tm_sql_chapter_signups,
// 		array(":start_date" => strtotime($start_date), ":end_date" => strtotime($end_date))
// 		);
//
// 	return $results->fetchAll();
// }


/**
* 
*/
class TmSqlLibrary
{

public static $tm_sql_events = <<<SQL

select SQL_NO_CACHE
	event.nid 'e_id', event.title 'event',
	ed.field_event_date_value as 'date',
	case when ed.field_event_date_timezone is not null
	then ed.field_event_date_timezone
	else chapter_tz.field_chapter_timezone_value
	end as 'timezone',
	event_type.field_event_type_value as 'event_type',
	chapter.title 'chapter',
	event.uid as 'event_creator_id',
	following_event_creater.entity_id as 'following_event_creater',
	-- count(following.entity_id) as 'attendees_that_i_follow',
	-- already_going.entity_id as 'already_going',
	have_joined_chapter.entity_id as 'have_joined_chapter',
	-- my_home_chapter.field_home_chapter_target_id as 'is_home_chapter'

		-- TODO "this event belongs to a company that i follow"
		-- TODO "this event is a member event, and i follow the member"
	null
from node event
left join field_data_field_event_date ed on event.nid = ed.entity_id
left join field_data_field_event_type event_type on event_type.entity_id = event.nid

left join field_data_field_chapter event_chapter on event_chapter.entity_id=event.nid
left join node chapter on event_chapter.field_chapter_target_id=chapter.nid
left join field_data_field_chapter_timezone chapter_tz on chapter_tz.entity_id=chapter.nid

left join flagging have_joined_chapter
on have_joined_chapter.entity_id = event_chapter.field_chapter_target_id
and have_joined_chapter.fid = 3
and have_joined_chapter.uid = :user_id

-- left join field_data_field_home_chapter my_home_chapter on event_chapter.field_chapter_target_id = my_home_chapter.field_home_chapter_target_id

left join flagging following_event_creater on event.uid = following_event_creater.entity_id
and following_event_creater.uid = :user_id and following_event_creater.fid=1

-- left join flagging already_going on event.nid = already_going.entity_id
-- left join flagging event_register on event.nid = event_register.entity_id
-- left join flagging following on following.entity_id=event_register.uid

where
event.type = 'event'
-- and event_chapter.bundle = 'event' and event_type.bundle = 'event'

-- and my_home_chapter.entity_type='user'
-- and my_home_chapter.entity_id = :user_id

-- and already_going.fid=8
-- and already_going.uid = :user_id

-- and event_register.fid=8

-- and following.uid = :user_id
-- and following.fid=1

-- get events taking place after the from date
and UNIX_TIMESTAMP(ed.field_event_date_value) > :from_date
-- but give a grace period of 1 hour before a result appears here
and UNIX_TIMESTAMP() > (event.created + 3600)

group by event.nid
-- TODO relevance sort
order by date
limit 3
SQL;


public static $tm_sql_updates = <<<SQL
select distinct SQL_NO_CACHE
	updates.entity_id,
	updates.entity_type,
	updates.created,
	case updates.entity_type
	when 'user' then
	CONCAT(users_first_name.field_user_first_name_value, ' ',
	users_last_name.field_user_last_name_value)
	when 'chapter' then
	chapters.title
	end as 'entity_name',
	case when num_followers.count is null then 0
	else num_followers.count end as 'num_followers',
	case when total_mutuals.count is null then 0
	else total_mutuals.count end as 'total_mutuals'
from tm_newsfeed_status_updates updates
inner join flagging following
on  following.entity_id = updates.entity_id
and following.uid = :user_id
left join node chapters on updates.entity_id=chapters.uid and updates.entity_type='node'
left join users on updates.entity_id=users.uid and updates.entity_type='user'
left join field_data_field_user_first_name users_first_name
on users_first_name.entity_id=users.uid and users_first_name.bundle='user'
left join field_data_field_user_last_name users_last_name
on users_last_name.entity_id=users.uid and users_last_name.bundle='user'
left join
(
select users2.uid, count(followers.entity_id) as 'count'
from users users2
left join flagging followers on followers.entity_id=users2.uid
where followers.fid=1
group by users2.uid
) num_followers
on users.uid = num_followers.uid
left join
(
select flagging.uid, count(mutual.flagging_id) as 'count'
from flagging
left join flagging mutual on flagging.entity_id=mutual.uid and flagging.uid=mutual.entity_id
where
	flagging.fid=1 and mutual.fid=1
	and mutual.flagging_id is not null
group by
	flagging.uid
) total_mutuals
on users.uid = total_mutuals.uid
where following.fid in (1,2,3)
and UNIX_TIMESTAMP(updates.created) >= :start_date and UNIX_TIMESTAMP(updates.created) <= :end_date
group by updates.entity_id  -- mysql gimmick
order by num_followers desc, total_mutuals desc, updates.created asc
limit 3
SQL;


public static $tm_sql_new_members = <<<SQL
select SQL_NO_CACHE
users.uid, users.created,
CONCAT(users_first_name.field_user_first_name_value, ' ',
users_last_name.field_user_last_name_value) as 'user_full_name',
users_home_chapter_object.nid as 'home_chapter_id',
users_home_chapter_object.title as 'home_chapter',
chapter_signups.signup_count,

follows_me.entity_id as 'follows_me',

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

left join flagging follows_me
on follows_me.fid=1 and follows_me.uid=users.uid and follows_me.entity_id = :user_id

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

left join
(
select chapter.nid, chapter.title, count(flagging.uid) as signup_count
from flagging
inner join node chapter on flagging.entity_id=chapter.nid
where flagging.fid=3 and chapter.type='chapter'
and flagging.timestamp > :start_date and flagging.timestamp < :end_date
group by chapter.nid, chapter.title
) chapter_signups
on chapter_signups.nid = users_home_chapter.field_home_chapter_target_id

where
  users.created > :start_date and users.created < :end_date
  and users.uid <> :user_id

order by
  same_location_as_me desc,
  same_home_chapter_as_me desc,
  our_home_chapters_in_same_country desc,
  same_country_as_my_home_chapter desc,
  same_country desc
  , users.created desc
limit 3
SQL;


public static $tm_sql_new_companies = <<<SQL
select SQL_NO_CACHE
node.nid, node.created, node.title, ttd.name as 'category',
case when num_followers.count is null then 0
else num_followers.count end as 'num_followers',
case when LOWER(REPLACE(REPLACE(location.field_location_city_value,' ',''),',','')) = LOWER(REPLACE(REPLACE(my_location.field_location_city_value,' ',''),',',''))
then 1 else 0 end as 'same_location_as_me',
case when company_country.field_country_iso2 = my_home_chapter_country.field_country_iso2
then 1 else 0 end as 'same_country_as_my_home_chapter'

from node
inner join taxonomy_index ti on ti.nid=node.nid
inner join taxonomy_term_data ttd on ttd.tid=ti.tid

left join field_data_field_location_city location
on location.entity_id=node.nid and location.bundle='organization'
left join field_data_field_location_city my_location
on location.entity_id=:user_id and location.bundle='user'
left join field_data_field_home_chapter my_home_chapter
on my_home_chapter.entity_id = :user_id and my_home_chapter.entity_type='user'
left join field_data_field_country my_home_chapter_country
on my_home_chapter_country.bundle='chapter' and my_home_chapter_country.entity_id=my_home_chapter.field_home_chapter_target_id
left join field_data_field_country company_country
on company_country.bundle='organization' and company_country.entity_id=node.nid

left join
(
select users2.uid, count(followers.entity_id) as 'count'
from users users2
left join flagging followers on followers.entity_id=users2.uid
where followers.fid=2
group by users2.uid
) num_followers
on node.nid = num_followers.uid

where
node.type = 'organization'
-- and node.created > (UNIX_TIMESTAMP() - 7 * 24*60*60)
and node.created > :start_date and node.created < :end_date

order by
  same_location_as_me desc, same_country_as_my_home_chapter desc, num_followers desc
limit 3;
SQL;



// public static $tm_sql_chapter_signups = <<<SQL
// select  SQL_NO_CACHE
// chapter.nid, chapter.title, count(flagging.uid) as signup_count
// from flagging
// inner join node chapter on flagging.entity_id=chapter.nid
// where flagging.fid=3 and chapter.type='chapter'
// and flagging.timestamp > :start_date and flagging.timestamp < :end_date
// group by chapter.nid, chapter.title
// SQL;


// public static $tm_sql_updates_test = <<<SQL
// select entity_id, status_update, created from {tm_newsfeed_status_updates};
// SQL;

// public static $tm_sql_updates_test_param = <<<SQL
// select
// entity_id, status_update,
// created
// from {tm_newsfeed_status_updates}
// where -- entity_id <> :user_id and 
// UNIX_TIMESTAMP(created) >= :start_date and UNIX_TIMESTAMP(created) <= :end_date
// limit 5
// SQL;


}

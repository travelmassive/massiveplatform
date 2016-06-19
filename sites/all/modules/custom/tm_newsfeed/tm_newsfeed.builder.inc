<?php

// tm_newsfeed.display.inc - newsfeed builder methods

// all queries in here

function tm_newsfeed_get_updates($user_id, $start_date, $end_date) {
	$results = db_query_range(TmNewsFeedSqlLibrary::sql('tm_sql_updates'), 0, 10,
		array(":user_id" => $user_id, ":start_date" => strtotime($start_date), ":end_date" => strtotime($end_date))
		);

	return $results->fetchAll();
}


function tm_newsfeed_get_events($user_id, $from_date) {
	$results = db_query_range(TmNewsFeedSqlLibrary::sql('tm_sql_events'), 0, 3,
		array(":user_id" => $user_id, ":from_date" => strtotime($from_date))
		);

	return $results->fetchAll();
}


function tm_newsfeed_get_new_members($user_id, $start_date, $end_date) {
	$results = db_query_range(TmNewsFeedSqlLibrary::sql('tm_sql_new_members'), 0, 3,
		array(":user_id" => $user_id, ":start_date" => strtotime($start_date), ":end_date" => strtotime($end_date))
		);

	return $results->fetchAll();
}

function tm_newsfeed_get_new_companies($user_id, $start_date, $end_date) {
	$results = db_query_range(TmNewsFeedSqlLibrary::sql('tm_sql_new_companies'), 0, 3,
		array(":user_id" => $user_id, ":start_date" => strtotime($start_date), ":end_date" => strtotime($end_date))
		);

	return $results->fetchAll();
}

function tm_newsfeed_select_tip($user_id, $start_date, $end_date) {
	$results = db_query_range(TmNewsFeedSqlLibrary::sql('tm_sql_select_tip'), 0, 1,
		array(":user_id" => $user_id, ":start_date" => strtotime($start_date), ":end_date" => strtotime($end_date))
		);

	return $results->fetchAll();
}


/**
* 
*/
class TmNewsFeedSqlLibrary
{
	// same as in TmStatusUpdatesSqlLibrary
	public static function sql($var_name)
	{
		$sql = TmNewsFeedSqlLibrary::$$var_name;
		$sql = str_replace('__FID_FOLLOW_MEMBERS__', flag_get_flag('follow_members')->fid, $sql);
		$sql = str_replace('__FID_FOLLOW_ORGANIZATIONS__', flag_get_flag('follow_organizations')->fid, $sql);
		$sql = str_replace('__FID_SIGNUP__', flag_get_flag('signup')->fid, $sql);
		$sql = str_replace('__FID_EVENT_REGISTER__', flag_get_flag('event_register')->fid, $sql);
		$sql = str_replace('__RID_APPROVED_MEMBER__', user_role_load_by_name('approved user')->rid, $sql);

		return $sql;
	}

public static $tm_sql_events = <<<SQL

select
	event.nid as 'event_id',
	event.title as 'event_title',
	ed.field_event_date_value as 'event_date',
	case when ed.field_event_date_timezone is not null
	then ed.field_event_date_timezone
	else chapter_tz.field_chapter_timezone_value
	end as 'timezone',

	-- #dev#
	-- event_type.field_event_type_value as 'event_type',
	-- chapter.title 'chapter',

	-- #dev#
	-- event.uid as 'event_creator_id',
	-- following_event_creator.entity_id as 'following_event_creator',
	-- event_company.field_event_company_target_id as 'event_company_id',
	-- following_event_company.entity_id as 'following_event_company',

	case
	when event_type.field_event_type_value = 'member'
	and following_event_creator.entity_id is not null
	then '1'
	else 0
	end as 'event_belongs_to_a_member_that_i_follow',

	case
	when event_type.field_event_type_value = 'company'
	and following_event_company.entity_id is not null
	then '1'
	else 0
	end as 'event_belongs_to_a_company_that_i_follow',

	is_my_home_chapter.field_home_chapter_target_id as 'is_my_home_chapter',
	have_joined_chapter.entity_id as 'have_joined_chapter',

	already_going.entity_id as 'already_going'

from node event
left join field_data_field_event_date ed on event.nid = ed.entity_id

left join field_data_field_event_type event_type
on event_type.entity_id = event.nid
and event_type.bundle = 'event'

left join field_data_field_chapter event_chapter
on event_chapter.entity_id=event.nid
and event_chapter.bundle = 'event' 

left join node chapter on event_chapter.field_chapter_target_id=chapter.nid
left join field_data_field_chapter_timezone chapter_tz on chapter_tz.entity_id=chapter.nid

left join flagging have_joined_chapter
on have_joined_chapter.entity_id = event_chapter.field_chapter_target_id
and have_joined_chapter.fid = __FID_SIGNUP__  -- signup (join chapter)
and have_joined_chapter.uid = :user_id

left join field_data_field_home_chapter is_my_home_chapter
on event_chapter.field_chapter_target_id = is_my_home_chapter.field_home_chapter_target_id
and is_my_home_chapter.entity_type = 'user'
and is_my_home_chapter.entity_id = :user_id

left join flagging following_event_creator
on event.uid = following_event_creator.entity_id
and following_event_creator.fid = __FID_FOLLOW_MEMBERS__  -- follow_members
and following_event_creator.uid = :user_id

left join field_data_field_event_company event_company
on event.nid = event_company.entity_id

left join flagging following_event_company
on event_company.field_event_company_target_id = following_event_company.entity_id
and following_event_company.fid = __FID_FOLLOW_ORGANIZATIONS__  -- follow_organizations
and following_event_company.uid = :user_id

left join flagging already_going on event.nid = already_going.entity_id
and already_going.fid = __FID_EVENT_REGISTER__  -- event_register
and already_going.uid = :user_id

where
event.type = 'event'
-- get events taking place after the from date
and UNIX_TIMESTAMP(ed.field_event_date_value) > :from_date
-- but give a grace period of 1 hour before a result appears here
and UNIX_TIMESTAMP() > (event.created + 3600)

order by
	event_belongs_to_a_member_that_i_follow desc,
	event_belongs_to_a_company_that_i_follow desc,
	is_my_home_chapter desc,
	have_joined_chapter desc,
	event_date asc

SQL;


public static $tm_sql_updates = <<<SQL

select
	updates.entity_id,

	updates.entity_type,
	case updates.entity_type
	when 'user'         then CONCAT(users_first_name.field_user_first_name_value, ' ', users_last_name.field_user_last_name_value)
	when 'chapter'      then chapter.title
	when 'organization' then organization.title
	end as 'entity_name',

	-- give weighting to the different follower counts
	case updates.entity_type
	when 'chapter'      then chapter_followers.count
	when 'user'         then 5 * user_followers.count
	when 'organization' then 5 * organization_followers.count
	end as 'entity_followers_weighted',

	-- #dev#
	-- case when user_followers.count is null then 0
	-- else user_followers.count end as 'user_followers',
	-- case when organization_followers.count is null then 0
	-- else organization_followers.count end as 'organization_followers',
	-- case when chapter_followers.count is null then 0
	-- else chapter_followers.count end as 'chapter_followers',

	updates.created

from tm_newsfeed_status_updates updates

-- from users/organizations/chapters that i am following
-- in conjunction with following.fid in (1,2,3)
left join flagging following
on  following.entity_id = updates.entity_id
and following.uid = :user_id

left join node organization on updates.entity_id=organization.nid and updates.entity_type='organization'
left join node chapter on updates.entity_id=chapter.nid and updates.entity_type='chapter'
left join users on updates.entity_id=users.uid and updates.entity_type='user'

left join field_data_field_user_first_name users_first_name
on users_first_name.entity_id=users.uid and users_first_name.bundle='user'
left join field_data_field_user_last_name users_last_name
on users_last_name.entity_id=users.uid and users_last_name.bundle='user'

left join flag_counts user_followers
on  user_followers.fid = __FID_FOLLOW_MEMBERS__  -- follow_members
and updates.entity_type = 'user'
and user_followers.entity_id = updates.entity_id

left join flag_counts organization_followers
on  organization_followers.fid = __FID_FOLLOW_ORGANIZATIONS__  -- follow_organizations
and updates.entity_type = 'organization'
and organization_followers.entity_id = updates.entity_id

left join flag_counts chapter_followers
on  chapter_followers.fid = __FID_SIGNUP__  -- signup
and updates.entity_type = 'chapter'
and chapter_followers.entity_id = updates.entity_id

left join users_roles
on  updates.entity_id = users_roles.uid
and updates.entity_type = 'user'
and users_roles.rid = __RID_APPROVED_MEMBER__  -- approved_member

where
	following.fid in (__FID_FOLLOW_MEMBERS__,__FID_FOLLOW_ORGANIZATIONS__,__FID_SIGNUP__)  -- follow_members, follow_organizations, signup
	and UNIX_TIMESTAMP(updates.created) >= :start_date
	and UNIX_TIMESTAMP(updates.created) <= :end_date
	-- exclude members that aren't approved
	and ( (users_roles.uid is not null and updates.entity_type = 'user') or updates.entity_type <> 'user' )
	-- exclude status updates that users have removed themselves, or moderators have hidden (i.e. spam)
	and updates.is_removed = 0 and updates.moderator_hide = 0
group by
	-- mysql cheat, so entities only appear once (can't use distinct because 'created' values are different)
	updates.entity_id
order by
	entity_followers_weighted desc,
	updates.created asc

SQL;


public static $tm_sql_new_members = <<<SQL

select
	users.uid,
	users.created,
	CONCAT(users_first_name.field_user_first_name_value, ' ', users_last_name.field_user_last_name_value) as 'user_full_name',
	users_home_chapter_object.nid as 'home_chapter_id',
	users_home_chapter_object.title as 'home_chapter',
	chapter_signups.signup_count,

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

left join
(
	-- cant use flag_counts here because we need to look for signups in a certain time period
	select chapter.nid, chapter.title, count(flagging.uid) as signup_count
	from flagging
	inner join node chapter on flagging.entity_id=chapter.nid
	where flagging.fid=__FID_SIGNUP__ and chapter.type='chapter'
	and flagging.timestamp > :start_date and flagging.timestamp < :end_date
	group by chapter.nid, chapter.title
) chapter_signups
on chapter_signups.nid = users_home_chapter.field_home_chapter_target_id

left join users_roles
on  users.uid = users_roles.uid
and users_roles.rid = __RID_APPROVED_MEMBER__  -- approved_member

where
	users.uid <> :user_id  -- dont include me
	and users.created > :start_date and users.created < :end_date
	-- exclude members that arent approved
	and users_roles.uid is not null

group by
	-- mysql hack - to only have one item per home chapter
	users_home_chapter.field_home_chapter_target_id

order by
	same_location_as_me desc,
	same_home_chapter_as_me desc,
	our_home_chapters_in_same_country desc,
	same_country_as_my_home_chapter desc,
	same_country desc
	, users.created desc

SQL;


public static $tm_sql_new_companies = <<<SQL

select
	node.nid,
	node.created,
	node.title,
	ttd.name as 'category',

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

-- left join flag_counts organization_followers
-- on  organization_followers.fid = __FID_FOLLOW_ORGANIZATIONS__  -- follow_organizations
-- and node.type = 'organization'
-- and organization_followers.entity_id = node.nid

left join
(
select users2.uid, count(followers.entity_id) as 'count'
from users users2
left join flagging followers on followers.entity_id=users2.uid
where followers.fid=__FID_FOLLOW_ORGANIZATIONS__
group by users2.uid
) num_followers
on node.nid = num_followers.uid

where
node.type = 'organization'
-- and node.created > (UNIX_TIMESTAMP() - 7 * 24*60*60)
and node.created > :start_date and node.created < :end_date

order by
	same_location_as_me desc,
	same_country_as_my_home_chapter desc,
	num_followers desc

SQL;


public static $tm_sql_select_tip = <<<SQL
select
	users.uid,
	users_roles.rid,
	org.field_job_organization_value,
	company_match.title,
	num_companies.count as 'num_companies'
from users
left join users_roles on users.uid = users_roles.uid and users_roles.rid = __RID_APPROVED_MEMBER__
left join field_data_field_job_organization org on users.uid = org.entity_id and org.entity_type = 'user'
left join node company_match on company_match.type='organization'
and
LOWER(REPLACE(REPLACE(company_match.title,' ',''),',','')) =
LOWER(REPLACE(REPLACE(org.field_job_organization_value,' ',''),',',''))

left join (select count(*) as 'count' from node where node.type='organization') num_companies on 1=1
-- TODO profile stats columns
where
	users.uid = :user_id
	and :start_date < :end_date  -- temp

SQL;

}

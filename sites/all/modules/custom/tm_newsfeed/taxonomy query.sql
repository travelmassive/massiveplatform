
set @user_id = 29540;

select
    node.nid,
    node.created,
    node.title,
    ttd_parent.tid as 'company_taxonomy_parent_tid',
    ttd_parent.name as 'company_taxonomy_parent_name',
    ttd.tid as 'company_taxonomy_child_tid',
    ttd.name as 'company_taxonomy_child_name',

    case when num_followers.count is null then 0
    else num_followers.count end as 'num_followers',

    case when LOWER(REPLACE(REPLACE(location.field_location_city_value,' ',''),',','')) = LOWER(REPLACE(REPLACE(my_location.field_location_city_value,' ',''),',',''))
    then 1 else 0 end as 'same_location_as_me',

    case when company_country.field_country_iso2 = my_home_chapter_country.field_country_iso2
    then 1 else 0 end as 'same_country_as_my_home_chapter',

    my_taxonomy.parent_tid as 'my_taxonomy_parent_tid',
    my_taxonomy.parent_name as 'my_taxonomy_parent_name',
    my_taxonomy.child_tid as 'my_taxonomy_child_tid',
    my_taxonomy.child_name as 'my_taxonomy_child_name'

from node
left join taxonomy_index ti on ti.nid=node.nid
left join taxonomy_term_data ttd on ttd.tid=ti.tid
left join taxonomy_term_hierarchy tth on ti.tid=tth.tid
left join taxonomy_term_data ttd_parent on tth.parent=ttd_parent.tid

left join field_data_field_location_city location
on location.entity_id=node.nid and location.bundle='organization'
left join field_data_field_location_city my_location
on location.entity_id=@user_id and location.bundle='user'
left join field_data_field_home_chapter my_home_chapter
on my_home_chapter.entity_id = @user_id and my_home_chapter.entity_type='user'
left join field_data_field_country my_home_chapter_country
on my_home_chapter_country.bundle='chapter' and my_home_chapter_country.entity_id=my_home_chapter.field_home_chapter_target_id
left join field_data_field_country company_country
on company_country.bundle='organization' and company_country.entity_id=node.nid

left join
(
  select 
  tei.entity_id as uid,
  ttd_parent.tid as parent_tid,
  ttd_parent.name as parent_name,
  ttd_child.tid as child_tid,
  ttd_child.name as child_name
  from taxonomy_entity_index tei
  left join taxonomy_term_data ttd_child on tei.tid=ttd_child.tid
  left join taxonomy_term_hierarchy tth on tei.tid=tth.tid
  left join taxonomy_term_data ttd_parent on tth.parent=ttd_parent.tid
  where tei.bundle='user'
) my_taxonomy
on my_taxonomy.uid=@user_id

-- left join flag_counts organization_followers
-- on  organization_followers.fid = 2  -- follow_organizations
-- and node.type = 'organization'
-- and organization_followers.entity_id = node.nid

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
-- and node.created > :start_date and node.created < :end_date

order by
  same_location_as_me desc,
  same_country_as_my_home_chapter desc,
  num_followers desc

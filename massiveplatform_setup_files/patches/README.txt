Patches in this folder are currently applied.

Note: If modules are manually updated then these patches may need to be reapplied.

# Core patches
3026443-d7-72.patch - fix phar for geoip (https://www.drupal.org/project/drupal/issues/3026443) 
image_derivatives-drupal769.patch - fix for image derivates to be processed

# Contrib modules
flag_record_uid_2087797_48.patch - flag module to allow tracking uid of global flag
patch_lookup_user_facebook_id.patch - lookup facebook id from user before looking up email
hierarchical_select-fix_selection_keys-2719141-22-7.x-3.x-dev.patch - prevent undefined index tid error in taxonomy.module
field_group-remove-array_parents-2494385-11.patch - fix field group update (https://www.drupal.org/project/field_group/issues/2926605)
link_rsvp_title.patch - allow moderators to edit rsvp link title

# Optional (if these modules are installed)
google_tag_cache_noscript_s3fs.patch - If using s3fs, load noscript from cache for performance
s3fs_preserve_chapter_event_images.patch - Preserve chapter and event images in s3fs
search_api_db_sort_flag_count.patch - sorting patch for search api db
search_api_solr_allow_quotes.patch - allow quote search with solr


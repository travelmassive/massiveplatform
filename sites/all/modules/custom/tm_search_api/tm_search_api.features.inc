<?php
/**
 * @file
 * tm_search_api.features.inc
 */

/**
 * Implements hook_default_search_api_index().
 */
function tm_search_api_default_search_api_index() {
  $items = array();
  $items['tm_search_multi_index'] = entity_import('search_api_index', '{
    "name" : "tm_search_multi_index",
    "machine_name" : "tm_search_multi_index",
    "description" : null,
    "server" : "search_db",
    "item_type" : "multiple",
    "options" : {
      "datasource" : { "types" : [ "node", "user" ] },
      "index_directly" : 0,
      "cron_limit" : "50",
      "fields" : {
        "item_bundle" : { "type" : "string" },
        "item_type" : { "type" : "string" },
        "node" : { "type" : "integer", "entity_type" : "node" },
        "node:body:value" : { "type" : "text" },
        "node:created" : { "type" : "date" },
        "node:field_chapter" : { "type" : "integer", "entity_type" : "node" },
        "node:field_chapter:title" : { "type" : "text" },
        "node:field_company_cover_video" : { "type" : "string" },
        "node:field_country" : { "type" : "string", "entity_type" : "country" },
        "node:field_country:continent" : { "type" : "text" },
        "node:field_country:continent_name" : { "type" : "string" },
        "node:field_country:iso2" : { "type" : "string" },
        "node:field_country:name" : { "type" : "text" },
        "node:field_country:official_name" : { "type" : "text" },
        "node:field_event_date:value" : { "type" : "date" },
        "node:field_event_is_preview_mode" : { "type" : "boolean" },
        "node:field_event_type" : { "type" : "string" },
        "node:field_event_venue_name" : { "type" : "text" },
        "node:field_link_facebook:url" : { "type" : "text" },
        "node:field_link_instagram:url" : { "type" : "text" },
        "node:field_link_linkedin:url" : { "type" : "text" },
        "node:field_link_snapchat:url" : { "type" : "text" },
        "node:field_link_twitter:url" : { "type" : "text" },
        "node:field_link_vimeo:url" : { "type" : "text" },
        "node:field_link_website:url" : { "type" : "text" },
        "node:field_link_youtube:url" : { "type" : "text" },
        "node:field_location" : { "type" : "text" },
        "node:field_location_city" : { "type" : "text" },
        "node:field_organizer" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "user" },
        "node:field_segment:name" : { "type" : "text" },
        "node:field_segment:parent:name" : { "type" : "list\\u003Ctext\\u003E" },
        "node:flag_event_register_count" : { "type" : "integer" },
        "node:flag_follow_organizations_count" : { "type" : "integer" },
        "node:flag_signup_count" : { "type" : "integer" },
        "node:flag_tm_sponsor" : { "type" : "boolean" },
        "node:nid" : { "type" : "integer" },
        "node:title" : { "type" : "text", "boost" : "5.0" },
        "node:type" : { "type" : "string" },
        "search_api_language" : { "type" : "string" },
        "user" : { "type" : "integer", "entity_type" : "user" },
        "user:field_about_me" : { "type" : "text" },
        "user:field_friendly_url" : { "type" : "string" },
        "user:field_home_chapter" : { "type" : "integer", "entity_type" : "node" },
        "user:field_job_organization" : { "type" : "text", "boost" : "2.0" },
        "user:field_job_role" : { "type" : "text" },
        "user:field_link_facebook:url" : { "type" : "text" },
        "user:field_link_instagram:url" : { "type" : "text" },
        "user:field_link_linkedin:url" : { "type" : "text" },
        "user:field_link_snapchat:url" : { "type" : "text" },
        "user:field_link_strava:url" : { "type" : "text" },
        "user:field_link_twitter:url" : { "type" : "text" },
        "user:field_link_vimeo:url" : { "type" : "text" },
        "user:field_link_website:url" : { "type" : "text" },
        "user:field_link_youtube:url" : { "type" : "text" },
        "user:field_location_city" : { "type" : "text" },
        "user:field_segment" : { "type" : "integer", "entity_type" : "taxonomy_term" },
        "user:field_segment:name" : { "type" : "text" },
        "user:field_segment:parent:name" : { "type" : "list\\u003Ctext\\u003E" },
        "user:field_user_country" : { "type" : "string", "entity_type" : "country" },
        "user:field_user_country:continent" : { "type" : "text" },
        "user:field_user_country:continent_name" : { "type" : "text" },
        "user:field_user_country:name" : { "type" : "text" },
        "user:field_user_country:official_name" : { "type" : "text" },
        "user:field_user_cover_video" : { "type" : "string" },
        "user:field_user_first_name" : { "type" : "text" },
        "user:field_user_has_private_profile" : { "type" : "boolean" },
        "user:field_user_last_name" : { "type" : "text", "boost" : "3.0" },
        "user:field_user_question_1" : { "type" : "text" },
        "user:field_user_question_2" : { "type" : "text" },
        "user:field_user_question_3" : { "type" : "text" },
        "user:field_user_question_4" : { "type" : "text" },
        "user:field_user_question_5" : { "type" : "text" },
        "user:flag_featured_member" : { "type" : "boolean" },
        "user:flag_follow_members_count" : { "type" : "integer" },
        "user:mail" : { "type" : "text" },
        "user:name" : { "type" : "text", "boost" : "5.0" },
        "user:uid" : { "type" : "integer" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 1,
          "weight" : "-10",
          "settings" : { "default" : "1", "bundles" : { "node:page" : "node:page" } }
        },
        "search_api_alter_role_filter_multi" : {
          "status" : 1,
          "weight" : "-10",
          "settings" : { "default" : "0", "roles" : { "6" : "6" } }
        },
        "search_api_alter_add_hierarchy" : { "status" : 1, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 1,
          "weight" : "0",
          "settings" : { "fields" : {
              "node:field_event_venue_name" : true,
              "node:field_location" : true,
              "node:field_location_city" : true,
              "user:mail" : true,
              "user:field_location_city" : true,
              "user:field_segment" : true,
              "user:field_about_me" : true,
              "user:field_job_organization" : true,
              "user:field_job_role" : true,
              "user:field_user_first_name" : true,
              "user:field_user_last_name" : true,
              "user:field_user_question_1" : true,
              "user:field_user_question_2" : true,
              "user:field_user_question_3" : true,
              "user:field_user_question_4" : true,
              "user:field_user_question_5" : true,
              "user:field_user_country" : true,
              "node:field_link_facebook:url" : true,
              "node:field_link_linkedin:url" : true,
              "node:field_link_twitter:url" : true,
              "node:field_link_website:url" : true,
              "node:field_country:iso2" : true,
              "node:field_country:name" : true,
              "node:field_country:official_name" : true,
              "node:field_country:continent" : true,
              "node:field_country:continent_name" : true,
              "node:field_segment:name" : true,
              "node:field_link_instagram:url" : true,
              "node:field_link_snapchat:url" : true,
              "node:field_link_vimeo:url" : true,
              "node:field_link_youtube:url" : true,
              "user:field_link_facebook:url" : true,
              "user:field_link_linkedin:url" : true,
              "user:field_link_twitter:url" : true,
              "user:field_link_website:url" : true,
              "user:field_segment:name" : true,
              "user:field_user_country:name" : true,
              "user:field_user_country:official_name" : true,
              "user:field_user_country:continent" : true,
              "user:field_user_country:continent_name" : true,
              "user:field_link_instagram:url" : true,
              "user:field_link_snapchat:url" : true,
              "user:field_link_vimeo:url" : true,
              "user:field_link_youtube:url" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : {
              "node:title" : true,
              "node:field_event_venue_name" : true,
              "node:field_location" : true,
              "node:field_location_city" : true,
              "user:mail" : true,
              "user:field_location_city" : true,
              "user:field_about_me" : true,
              "user:field_job_organization" : true,
              "user:field_job_role" : true,
              "user:field_user_first_name" : true,
              "user:field_user_last_name" : true,
              "user:field_user_question_1" : true,
              "user:field_user_question_2" : true,
              "user:field_user_question_3" : true,
              "user:field_user_question_4" : true,
              "user:field_user_question_5" : true
            },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : {
              "node:title" : true,
              "node:field_event_venue_name" : true,
              "node:field_location" : true,
              "node:field_location_city" : true,
              "user:mail" : true,
              "user:field_location_city" : true,
              "user:field_about_me" : true,
              "user:field_job_organization" : true,
              "user:field_job_role" : true,
              "user:field_user_first_name" : true,
              "user:field_user_last_name" : true,
              "user:field_user_question_1" : true,
              "user:field_user_question_2" : true,
              "user:field_user_question_3" : true,
              "user:field_user_question_4" : true,
              "user:field_user_question_5" : true
            },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : {
              "node:title" : true,
              "node:field_event_venue_name" : true,
              "node:field_location" : true,
              "node:field_location_city" : true,
              "user:mail" : true,
              "user:field_location_city" : true,
              "user:field_about_me" : true,
              "user:field_job_organization" : true,
              "user:field_job_role" : true,
              "user:field_user_first_name" : true,
              "user:field_user_last_name" : true,
              "user:field_user_question_1" : true,
              "user:field_user_question_2" : true,
              "user:field_user_question_3" : true,
              "user:field_user_question_4" : true,
              "user:field_user_question_5" : true
            },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        },
        "search_api_highlighting" : {
          "status" : 0,
          "weight" : "35",
          "settings" : {
            "prefix" : "\\u003Cstrong\\u003E",
            "suffix" : "\\u003C\\/strong\\u003E",
            "excerpt" : 1,
            "excerpt_length" : "256",
            "exclude_fields" : [],
            "highlight" : "always"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function tm_search_api_default_search_api_server() {
  $items = array();
  $items['search_db'] = entity_import('search_api_server', '{
    "name" : "search_db",
    "machine_name" : "search_db",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "3",
      "partial_matches" : 0,
      "indexes" : { "tm_search_multi_index" : {
          "item_type" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "item_type",
            "type" : "string",
            "boost" : "1.0"
          },
          "item_bundle" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "item_bundle",
            "type" : "string",
            "boost" : "1.0"
          },
          "user" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user",
            "type" : "integer",
            "boost" : "1.0"
          },
          "node" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node",
            "type" : "integer",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "user:mail" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:flag_featured_member" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user_flag_featured_member",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "user:field_about_me" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_friendly_url" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user_field_friendly_url",
            "type" : "string",
            "boost" : "1.0"
          },
          "user:field_job_organization" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "2.0"
          },
          "user:field_job_role" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_location_city" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_segment" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user_field_segment",
            "type" : "integer",
            "boost" : "1.0"
          },
          "user:field_user_first_name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_last_name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "3.0"
          },
          "user:field_user_question_1" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_question_2" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_question_3" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_question_4" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_question_5" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_country" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user_field_user_country",
            "type" : "string",
            "boost" : "1.0"
          },
          "user:field_home_chapter" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user_field_home_chapter",
            "type" : "integer",
            "boost" : "1.0"
          },
          "node:type" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_type",
            "type" : "string",
            "boost" : "1.0"
          },
          "node:title" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "5.0"
          },
          "node:created" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_created",
            "type" : "date",
            "boost" : "1.0"
          },
          "node:field_location_city" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_country" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_field_country",
            "type" : "string",
            "boost" : "1.0"
          },
          "node:flag_tm_sponsor" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_flag_tm_sponsor",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "node:field_chapter" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_field_chapter",
            "type" : "integer",
            "boost" : "1.0"
          },
          "node:field_event_venue_name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_location" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_organizer" : {
            "table" : "search_api_db_tm_search_multi_index_node_field_organizer",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "node:field_event_type" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_field_event_type",
            "type" : "string",
            "boost" : "1.0"
          },
          "user:field_link_facebook:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "user_field_link_facebook_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_link_linkedin:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "user_field_link_linkedin_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_link_twitter:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "user_field_link_twitter_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_link_website:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "user_field_link_website_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_segment:name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_country:name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_country:official_name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_country:continent" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_country:continent_name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_link_instagram:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "user_field_link_instagram_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_link_snapchat:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "user_field_link_snapchat_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_link_vimeo:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "user_field_link_vimeo_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_link_youtube:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "user_field_link_youtube_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:body:value" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_link_facebook:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "node_field_link_facebook_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_link_linkedin:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "node_field_link_linkedin_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_link_twitter:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "node_field_link_twitter_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_link_website:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "node_field_link_website_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_segment:name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_link_instagram:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "node_field_link_instagram_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_link_snapchat:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "node_field_link_snapchat_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_link_vimeo:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "node_field_link_vimeo_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_link_youtube:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "column" : "node_field_link_youtube_url",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_country:iso2" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_field_country_iso2",
            "type" : "string",
            "boost" : "1.0"
          },
          "node:field_country:name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_country:official_name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_country:continent" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_country:continent_name" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_field_country_continent_name",
            "type" : "string",
            "boost" : "1.0"
          },
          "user:field_segment:parent:name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "list\\u003Ctext\\u003E",
            "boost" : "1.0"
          },
          "node:field_segment:parent:name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "list\\u003Ctext\\u003E",
            "boost" : "1.0"
          },
          "node:field_chapter:title" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "node:field_event_date:value" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_field_event_date_value",
            "type" : "date",
            "boost" : "1.0"
          },
          "user:name" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "5.0"
          },
          "node:nid" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_nid",
            "type" : "integer",
            "boost" : "1.0"
          },
          "user:uid" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user_uid",
            "type" : "integer",
            "boost" : "1.0"
          },
          "user:field_user_cover_video" : {
            "table" : "search_api_db_tm_search_multi_index",
            "type" : "string",
            "boost" : "1.0",
            "column" : "user_field_user_cover_video"
          },
          "node:field_company_cover_video" : {
            "table" : "search_api_db_tm_search_multi_index",
            "type" : "string",
            "boost" : "1.0",
            "column" : "node_field_company_cover_video"
          },
          "node:field_event_is_preview_mode" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_field_event_is_preview_mode",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "user:field_link_strava:url" : {
            "table" : "search_api_db_tm_search_multi_index_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "user:field_user_has_private_profile" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user_field_user_has_private_profile",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "user:flag_follow_members_count" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "user_flag_follow_members_count",
            "type" : "integer",
            "boost" : "1.0"
          },
          "node:flag_follow_organizations_count" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_flag_follow_organizations_count",
            "type" : "integer",
            "boost" : "1.0"
          },
          "node:flag_signup_count" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_flag_signup_count",
            "type" : "integer",
            "boost" : "1.0"
          },
          "node:flag_event_register_count" : {
            "table" : "search_api_db_tm_search_multi_index",
            "column" : "node_flag_event_register_count",
            "type" : "integer",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}

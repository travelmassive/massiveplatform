---
title: Development
has_children: false
nav_order: 4
---

## Keeping up to date

Keeping your installation up to date is easy.

Here's the general tasks you'll run to update your codebase, in the following order.

1. Update code *(git pull)*
2. Add any new settings.php variables *(git log -p massiveplatform_setup_files/settings.php)*
3. Update any code features *(drush fd; drush fr tm_changed_module)*
4. Apply any database updates *(drush updb)*
5. Clear cache *(drush cc all)*

New versions of Drupal core and modules are maintained and included, you don't need to update modules seperately.


### View changes to settings.php

```console
git log -p massiveplatform_setup_files/settings.php
```

### Example of updating code features

First, we clear the cache

```console
drush cc all
```

Next, view the features list. This will show you if there are any database changes (ie: a new field) to platform modules.

```console
drush fd
```

The output will look like this:

```console
user@ubuntu:/var/www/massiveplatform# drush fd
 Date Migration Example               date_migrate_example                 Disabled  7.x-2.11-beta3               
 Features Tests                       features_test                        Disabled  7.x-2.11                     
 Imagecache_actions Test Suite        imagecache_testsuite                 Disabled  7.x-1.12                     
 TM Base                              tm_base                              Enabled   7.x-1.0-dev                  
 TM Branding                          tm_branding                          Enabled   7.x-1.0-dev                  
 TM Chapters                          tm_chapters                          Enabled   7.x-1.0-dev                  
 TM Chapters Map                      tm_chapters_leaflet_map              Enabled   7.x-1.0                      
 TM Commissions                       tm_commissions                       Disabled  7.x-1.0                      
 TM Event signup                      tm_event_signup                      Enabled   7.x-1.0-dev                  
 TM Events                            tm_events                            Enabled   7.x-1.0-dev     Needs Review             
 TM Fields                            tm_fields                            Enabled   7.x-1.0-dev                  
 TM Flags                             tm_flags                             Enabled   7.x-1.0-dev                  
 TM Invitations                       tm_invitations                       Enabled   7.x-1.0-dev                  
 TM Lists                             tm_lists                             Disabled  7.x-1.0-dev                  
 TM Marketplace                       tm_marketplace                       Disabled  7.x-1.0-dev                  
 TM Messaging                         tm_messaging                         Enabled   7.x-1.0-dev                  
 TM Meta                              tm_meta                              Enabled   7.x-1.0-dev                  
 TM Newsfeed                          tm_newsfeed                          Enabled   7.x-1.0-dev                  
 TM Notifications                     tm_notifications                     Enabled   7.x-1.0-dev                  
 TM Notifications Approval            tm_notifications_approval            Enabled   7.x-1.0-dev                  
 TM Notifications Chapter             tm_notifications_chapter             Enabled   7.x-1.0-dev                  
 TM Notifications Events              tm_notifications_events              Enabled   7.x-1.0-dev                  
 TM Notifications Lists               tm_notifications_lists               Disabled  7.x-1.0-dev                  
 TM Notifications Messaging           tm_notifications_messaging           Enabled   7.x-1.0-dev                  
 TM Notifications Moderation          tm_notifications_moderation          Enabled   7.x-1.0-dev                  
 TM Notifications Following           tm_notifications_new_follower        Enabled   7.x-1.0-dev        
 TM Notifications Newsfeed            tm_notifications_newsfeed            Enabled   7.x-1.0-dev                  
 TM Notifications Newsletters         tm_notifications_newsletters         Enabled   7.x-1.0-dev                  
 TM Notifications User Subscriptions  tm_notifications_subscriptions_user  Disabled  7.x-1.0-dev                  
 TM Notifications Upcoming Event      tm_notifications_upcoming_event      Disabled  7.x-1.0-dev                  
 TM Net Promoter Score                tm_nps                               Enabled   7.x-1.0-dev                  
 TM Organizations                     tm_organizations                     Enabled   7.x-1.0-dev      
 TM Payments                          tm_payments                          Disabled  7.x-1.0-dev                  
 TM Reports                           tm_reports                           Enabled   7.x-1.0-beta1                
 TM Search                            tm_search                            Disabled  7.x-1.0-dev                  
 TM Search API                        tm_search_api                        Enabled   7.x-1.0-dev                  
 TM Status Updates                    tm_status_updates                    Enabled   7.x-1.0-dev                  
 TM User Subscriptions                tm_subscriptions_user                Disabled  7.x-1.0-dev                  
 TM Theme                             tm_theme                             Enabled   7.x-1.0-dev                  
 TM Track Views                       tm_track_views                       Disabled  7.x-1.0-dev                  
 TM Users                             tm_users                             Enabled   7.x-1.0-dev                  
 TM External signin                   tm_users_external_signin             Enabled   7.x-1.0-dev
user@ubuntu:/var/www/massiveplatform#
```

In the example above, tm_events has been updated ('Needs Review'). You can bring the module up to date by running the following command:

```console
drush fr tm_events
```

Check to make sure the features have been updated.

```console
drush fd
```

If features have not updated, try clearing the cache, or running with update with the --force command (ie: drush fr tm_events --force)


## Drupal cron

You must run the Drupal cron regularly to support tasks such as search indexing and email announcements.

```console
drush run-cron --uri=https://yoursite.com --root=/path/to/massiveplatform
```

You can run the cron every minute by adding an entry to your /etc/crontab:

```console
# Run Drupal ultimate cron every minute
* * * * * root /usr/bin/drush cron-run --uri=https://yoursite.com --root=/path/to/massiveplatform 2>&1 > /dev/null
```

## Automated tasks

The following is a list of drush commands you can run. 

**You will need to implement your own scheduling (ie: via /etc/crontab) of these tasks.**

### Performance

These commands will cache data that might take a little longer to generate.

```console
drush tm-api-cache-stats --uri=https://yoursite.com --root=/path/to/massiveplatform
drush tm-chapters-update-display-stats-cache --uri=https://yoursite.com --root=/path/to/massiveplatform
drush status-updates-index-view-counter --uri=https://yoursite.com --root=/path/to/massiveplatform
drush tm-reports-chapter-insights-cache --uri=https://yoursite.com --root=/path/to/massiveplatform
```

### Community Management tasks

These commands provide automated chapter reports and management.

```console
drush tm-community-management-reports --uri=https://yoursite.com --root=/path/to/massiveplatform
drush tm-community-management-tasks --uri=https://yoursite.com --root=/path/to/massiveplatform
```

See $conf["tm_chapters_leader_*"] in settings.php for configuration options.

### Event tasks

If you plan on enabling reminders, or live-streams, you will need to run these commands every 5 minutes.

```console
drush tm-events-livestream-tasks --uri=https://yoursite.com --root=/path/to/massiveplatform
drush tm-events-reminder-tasks --uri=https://yoursite.com --root=/path/to/massiveplatform
```

See $conf["tm_event_online_*"] in settings.php for configuration options.

### Data export

Run these commands one a day to generate export data.

```console
drush tm-generate-newsletter-csv --uri=https://yoursite.com --root=/path/to/massiveplatform
drush tm-chapters-members-csv-cache --uri=https://yoursite.com --root=/path/to/massiveplatform
drush tm-generate-facebook-csv --uri=https://yoursite.com --root=/path/to/massiveplatform
```

### Newsfeed Emails

Run this command every 4 hours to schedule weekly newsfeed emails.

```console
drush newsfeed-schedule-deliveries --uri=https://yoursite.com --root=/path/to/massiveplatform
```

See $conf["tm_newsfeed_*"] in settings.php for configuration options.

### Post RSS updates to organization pages

Run this command hourly if you want to enable automatic posting to company pages from an RSS feed.

```console
drush status-updates-post-organization-updates --uri=https://yoursite.com --root=/path/to/massiveplatform
```

## Development

### Customizing the sidebar

If you want to customize your sidebar, make a copy of 'example-sidebar.tpl.php' and update $conf['tm_theme_custom_sidebar_template'].

You can enable the traditional navbar by commenting out the following line in settings.php:

```console
// $conf['tm_theme_custom_sidebar_template'] = 'example-sidebar.tpl.php';
```

### Moving images to S3 / Cloudfront

See the file 's3fs_install_notes.txt' in massiveplatform_setup_files.

### How to edit SASS stylesheets with Compass

To edit SASS stylesheets you'll need compass installed:

```console
sudo gem install sass -v 3.2.19
sudo gem install compass -v 0.12.6
```

See [this comment regarding compass version](https://www.drupal.org/node/2353067#comment-9535245)

Run "compass watch sites/all/themes/tm" to update css files

### How to export member questions to csv:

```console
SELECT field_user_question_1_value, field_user_country_iso2, concat("https://example.massiveplatform.com/user/", field_revision_field_user_country.entity_id)
FROM field_data_field_user_question_1
LEFT JOIN field_revision_field_user_country
ON field_revision_field_user_country.entity_id = field_data_field_user_question_1.entity_id
INTO OUTFILE '/tmp/user_questions_1.csv'
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n';
```

*Note: Replace 1 with 1,2,3 or 4 for question index*

### How to increase performance of flagging table with many rows

```console
show index from flagging;
ALTER TABLE `flagging` ADD INDEX `tm_uid` (`uid`);
```

### How to increase performance of tm_track_views lookups

```console
ALTER TABLE `tm_track_views` ADD INDEX `tm_entity_type` (`entity_type`);
ALTER TABLE `tm_track_views` ADD INDEX `tm_entity_id` (`entity_id`);
```

### How to test emails

Add the following to sites/default/settings.php to write emails to /tmp/DevelMailLog:

```console
$conf['mail_system'] = array('default-system' => 'DevelMailLog',);
```

### How to enable Wordpress feeds

You can display a feed of related wordpress blog posts to companies/members/chapters from your blog.

1. Edit sites/default/settings.php and set $conf['tm_enable_wordpress_feedme'] = true;
2. Install the tm-feeds wordpress plugin (https://github.com/travelmassive/massiveplatform-wordpress-feedme) 
3. Set $conf['tm_wordpress_feedme_url'] to the URL to call on page loads to embed feeds.


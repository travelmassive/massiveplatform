<?php

/**
 * @file
 * Drupal site-specific configuration file.
 *
 * IMPORTANT NOTE:
 * This file may have been set to read-only by the Drupal installation program.
 * If you make changes to this file, be sure to protect it again after making
 * your modifications. Failure to remove write permissions to this file is a
 * security risk.
 *
 * The configuration file to be loaded is based upon the rules below. However
 * if the multisite aliasing file named sites/sites.php is present, it will be
 * loaded, and the aliases in the array $sites will override the default
 * directory rules below. See sites/example.sites.php for more information about
 * aliases.
 *
 * The configuration directory will be discovered by stripping the website's
 * hostname from left to right and pathname from right to left. The first
 * configuration file found will be used and any others will be ignored. If no
 * other configuration file is found then the default configuration file at
 * 'sites/default' will be used.
 *
 * For example, for a fictitious site installed at
 * http://www.drupal.org:8080/mysite/test/, the 'settings.php' file is searched
 * for in the following directories:
 *
 * - sites/8080.www.drupal.org.mysite.test
 * - sites/www.drupal.org.mysite.test
 * - sites/drupal.org.mysite.test
 * - sites/org.mysite.test
 *
 * - sites/8080.www.drupal.org.mysite
 * - sites/www.drupal.org.mysite
 * - sites/drupal.org.mysite
 * - sites/org.mysite
 *
 * - sites/8080.www.drupal.org
 * - sites/www.drupal.org
 * - sites/drupal.org
 * - sites/org
 *
 * - sites/default
 *
 * Note that if you are installing on a non-standard port number, prefix the
 * hostname with that number. For example,
 * http://www.drupal.org:8080/mysite/test/ could be loaded from
 * sites/8080.www.drupal.org.mysite.test/.
 *
 * @see example.sites.php
 * @see conf_path()
 */

/**
 * Database settings:
 *
 * The $databases array specifies the database connection or
 * connections that Drupal may use.  Drupal is able to connect
 * to multiple databases, including multiple types of databases,
 * during the same request.
 *
 * Each database connection is specified as an array of settings,
 * similar to the following:
 * @code
 * array(
 *   'driver' => 'mysql',
 *   'database' => 'databasename',
 *   'username' => 'username',
 *   'password' => 'password',
 *   'host' => 'localhost',
 *   'port' => 3306,
 *   'prefix' => 'myprefix_',
 *   'collation' => 'utf8_general_ci',
 * );
 * @endcode
 *
 * The "driver" property indicates what Drupal database driver the
 * connection should use.  This is usually the same as the name of the
 * database type, such as mysql or sqlite, but not always.  The other
 * properties will vary depending on the driver.  For SQLite, you must
 * specify a database file name in a directory that is writable by the
 * webserver.  For most other drivers, you must specify a
 * username, password, host, and database name.
 *
 * Some database engines support transactions.  In order to enable
 * transaction support for a given database, set the 'transactions' key
 * to TRUE.  To disable it, set it to FALSE.  Note that the default value
 * varies by driver.  For MySQL, the default is FALSE since MyISAM tables
 * do not support transactions.
 *
 * For each database, you may optionally specify multiple "target" databases.
 * A target database allows Drupal to try to send certain queries to a
 * different database if it can but fall back to the default connection if not.
 * That is useful for master/slave replication, as Drupal may try to connect
 * to a slave server when appropriate and if one is not available will simply
 * fall back to the single master server.
 *
 * The general format for the $databases array is as follows:
 * @code
 * $databases['default']['default'] = $info_array;
 * $databases['default']['slave'][] = $info_array;
 * $databases['default']['slave'][] = $info_array;
 * $databases['extra']['default'] = $info_array;
 * @endcode
 *
 * In the above example, $info_array is an array of settings described above.
 * The first line sets a "default" database that has one master database
 * (the second level default).  The second and third lines create an array
 * of potential slave databases.  Drupal will select one at random for a given
 * request as needed.  The fourth line creates a new database with a name of
 * "extra".
 *
 * For a single database configuration, the following is sufficient:
 * @code
 * $databases['default']['default'] = array(
 *   'driver' => 'mysql',
 *   'database' => 'databasename',
 *   'username' => 'username',
 *   'password' => 'password',
 *   'host' => 'localhost',
 *   'prefix' => 'main_',
 *   'collation' => 'utf8_general_ci',
 * );
 * @endcode
 *
 * You can optionally set prefixes for some or all database table names
 * by using the 'prefix' setting. If a prefix is specified, the table
 * name will be prepended with its value. Be sure to use valid database
 * characters only, usually alphanumeric and underscore. If no prefixes
 * are desired, leave it as an empty string ''.
 *
 * To have all database names prefixed, set 'prefix' as a string:
 * @code
 *   'prefix' => 'main_',
 * @endcode
 * To provide prefixes for specific tables, set 'prefix' as an array.
 * The array's keys are the table names and the values are the prefixes.
 * The 'default' element is mandatory and holds the prefix for any tables
 * not specified elsewhere in the array. Example:
 * @code
 *   'prefix' => array(
 *     'default'   => 'main_',
 *     'users'     => 'shared_',
 *     'sessions'  => 'shared_',
 *     'role'      => 'shared_',
 *     'authmap'   => 'shared_',
 *   ),
 * @endcode
 * You can also use a reference to a schema/database as a prefix. This may be
 * useful if your Drupal installation exists in a schema that is not the default
 * or you want to access several databases from the same code base at the same
 * time.
 * Example:
 * @code
 *   'prefix' => array(
 *     'default'   => 'main.',
 *     'users'     => 'shared.',
 *     'sessions'  => 'shared.',
 *     'role'      => 'shared.',
 *     'authmap'   => 'shared.',
 *   );
 * @endcode
 * NOTE: MySQL and SQLite's definition of a schema is a database.
 *
 * Advanced users can add or override initial commands to execute when
 * connecting to the database server, as well as PDO connection settings. For
 * example, to enable MySQL SELECT queries to exceed the max_join_size system
 * variable, and to reduce the database connection timeout to 5 seconds:
 *
 * @code
 * $databases['default']['default'] = array(
 *   'init_commands' => array(
 *     'big_selects' => 'SET SQL_BIG_SELECTS=1',
 *   ),
 *   'pdo' => array(
 *     PDO::ATTR_TIMEOUT => 5,
 *   ),
 * );
 * @endcode
 *
 * WARNING: These defaults are designed for database portability. Changing them
 * may cause unexpected behavior, including potential data loss.
 *
 * @see DatabaseConnection_mysql::__construct
 * @see DatabaseConnection_pgsql::__construct
 * @see DatabaseConnection_sqlite::__construct
 *
 * Database configuration format:
 * @code
 *   $databases['default']['default'] = array(
 *     'driver' => 'mysql',
 *     'database' => 'databasename',
 *     'username' => 'username',
 *     'password' => 'password',
 *     'host' => 'localhost',
 *     'prefix' => '',
 *   );
 *   $databases['default']['default'] = array(
 *     'driver' => 'pgsql',
 *     'database' => 'databasename',
 *     'username' => 'username',
 *     'password' => 'password',
 *     'host' => 'localhost',
 *     'prefix' => '',
 *   );
 *   $databases['default']['default'] = array(
 *     'driver' => 'sqlite',
 *     'database' => '/path/to/databasefilename',
 *   );
 * @endcode
 */
$databases = array (
  'default' => 
  array (
    'default' => 
    array (
      'database' => 'massiveplatform',
      'username' => 'massiveplatform',
      'password' => 'massiveplatform',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => '',
    ),
  ),
);

/**
 * Access control for update.php script.
 *
 * If you are updating your Drupal installation using the update.php script but
 * are not logged in using either an account with the "Administer software
 * updates" permission or the site maintenance account (the account that was
 * created during installation), you will need to modify the access check
 * statement below. Change the FALSE to a TRUE to disable the access check.
 * After finishing the upgrade, be sure to open this file again and change the
 * TRUE back to a FALSE!
 */
$update_free_access = FALSE;

/**
 * Salt for one-time login links and cancel links, form tokens, etc.
 *
 * This variable will be set to a random value by the installer. All one-time
 * login links will be invalidated if the value is changed. Note that if your
 * site is deployed on a cluster of web servers, you must ensure that this
 * variable has the same value on each server. If this variable is empty, a hash
 * of the serialized database credentials will be used as a fallback salt.
 *
 * For enhanced security, you may set this variable to a value using the
 * contents of a file outside your docroot that is never saved together
 * with any backups of your Drupal files and database.
 *
 * Example:
 *   $drupal_hash_salt = file_get_contents('/home/example/salt.txt');
 *
 */
$drupal_hash_salt = '';

/**
 * Base URL (optional).
 *
 * If Drupal is generating incorrect URLs on your site, which could
 * be in HTML headers (links to CSS and JS files) or visible links on pages
 * (such as in menus), uncomment the Base URL statement below (remove the
 * leading hash sign) and fill in the absolute URL to your Drupal installation.
 *
 * You might also want to force users to use a given domain.
 * See the .htaccess file for more information.
 *
 * Examples:
 *   $base_url = 'http://www.example.com';
 *   $base_url = 'http://www.example.com:8888';
 *   $base_url = 'http://www.example.com/drupal';
 *   $base_url = 'https://www.example.com:8888/drupal';
 *
 * It is not allowed to have a trailing slash; Drupal will add it
 * for you.
 */
# $base_url = 'http://www.example.com';  // NO trailing slash!

/**
 * PHP settings:
 *
 * To see what PHP settings are possible, including whether they can be set at
 * runtime (by using ini_set()), read the PHP documentation:
 * http://www.php.net/manual/ini.list.php
 * See drupal_environment_initialize() in includes/bootstrap.inc for required
 * runtime settings and the .htaccess file for non-runtime settings. Settings
 * defined there should not be duplicated here so as to avoid conflict issues.
 */

/**
 * Some distributions of Linux (most notably Debian) ship their PHP
 * installations with garbage collection (gc) disabled. Since Drupal depends on
 * PHP's garbage collection for clearing sessions, ensure that garbage
 * collection occurs by using the most common settings.
 */
ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);

/**
 * Set session lifetime (in seconds), i.e. the time from the user's last visit
 * to the active session may be deleted by the session garbage collector. When
 * a session is deleted, authenticated users are logged out, and the contents
 * of the user's $_SESSION variable is discarded.
 */
ini_set('session.gc_maxlifetime', (60 * 60 * 24 * 90)); // 90 days

/**
 * Set session cookie lifetime (in seconds), i.e. the time from the session is
 * created to the cookie expires, i.e. when the browser is expected to discard
 * the cookie. The value 0 means "until the browser is closed".
 */
ini_set('session.cookie_lifetime', (60 * 60 * 24 * 90)); // 90 days


/**
 * If you encounter a situation where users post a large amount of text, and
 * the result is stripped out upon viewing but can still be edited, Drupal's
 * output filter may not have sufficient memory to process it.  If you
 * experience this issue, you may wish to uncomment the following two lines
 * and increase the limits of these variables.  For more information, see
 * http://php.net/manual/pcre.configuration.php.
 */
# ini_set('pcre.backtrack_limit', 200000);
# ini_set('pcre.recursion_limit', 200000);

/**
 * Drupal automatically generates a unique session cookie name for each site
 * based on its full domain name. If you have multiple domains pointing at the
 * same Drupal site, you can either redirect them all to a single domain (see
 * comment in .htaccess), or uncomment the line below and specify their shared
 * base domain. Doing so assures that users remain logged in as they cross
 * between your various domains. Make sure to always start the $cookie_domain
 * with a leading dot, as per RFC 2109.
 */
# $cookie_domain = '.example.com';

/**
 * Variable overrides:
 *
 * To override specific entries in the 'variable' table for this site,
 * set them here. You usually don't need to use this feature. This is
 * useful in a configuration file for a vhost or directory, rather than
 * the default settings.php. Any configuration setting from the 'variable'
 * table can be given a new value. Note that any values you provide in
 * these variable overrides will not be modifiable from the Drupal
 * administration interface.
 *
 * The following overrides are examples:
 * - site_name: Defines the site's name.
 * - theme_default: Defines the default theme for this site.
 * - anonymous: Defines the human-readable name of anonymous users.
 * Remove the leading hash signs to enable.
 */
# $conf['site_name'] = 'My Drupal site';
# $conf['theme_default'] = 'garland';
# $conf['anonymous'] = 'Visitor';

/**
 * A custom theme can be set for the offline page. This applies when the site
 * is explicitly set to maintenance mode through the administration page or when
 * the database is inactive due to an error. It can be set through the
 * 'maintenance_theme' key. The template file should also be copied into the
 * theme. It is located inside 'modules/system/maintenance-page.tpl.php'.
 * Note: This setting does not apply to installation and update pages.
 */
# $conf['maintenance_theme'] = 'bartik';

/**
 * Reverse Proxy Configuration:
 *
 * Reverse proxy servers are often used to enhance the performance
 * of heavily visited sites and may also provide other site caching,
 * security, or encryption benefits. In an environment where Drupal
 * is behind a reverse proxy, the real IP address of the client should
 * be determined such that the correct client IP address is available
 * to Drupal's logging, statistics, and access management systems. In
 * the most simple scenario, the proxy server will add an
 * X-Forwarded-For header to the request that contains the client IP
 * address. However, HTTP headers are vulnerable to spoofing, where a
 * malicious client could bypass restrictions by setting the
 * X-Forwarded-For header directly. Therefore, Drupal's proxy
 * configuration requires the IP addresses of all remote proxies to be
 * specified in $conf['reverse_proxy_addresses'] to work correctly.
 *
 * Enable this setting to get Drupal to determine the client IP from
 * the X-Forwarded-For header (or $conf['reverse_proxy_header'] if set).
 * If you are unsure about this setting, do not have a reverse proxy,
 * or Drupal operates in a shared hosting environment, this setting
 * should remain commented out.
 *
 * In order for this setting to be used you must specify every possible
 * reverse proxy IP address in $conf['reverse_proxy_addresses'].
 * If a complete list of reverse proxies is not available in your
 * environment (for example, if you use a CDN) you may set the
 * $_SERVER['REMOTE_ADDR'] variable directly in settings.php.
 * Be aware, however, that it is likely that this would allow IP
 * address spoofing unless more advanced precautions are taken.
 */
# $conf['reverse_proxy'] = TRUE;

/**
 * Specify every reverse proxy IP address in your environment.
 * This setting is required if $conf['reverse_proxy'] is TRUE.
 */
# $conf['reverse_proxy_addresses'] = array('a.b.c.d', ...);

/**
 * Set this value if your proxy server sends the client IP in a header
 * other than X-Forwarded-For.
 */
# $conf['reverse_proxy_header'] = 'HTTP_X_CLUSTER_CLIENT_IP';

/**
 * Page caching:
 *
 * By default, Drupal sends a "Vary: Cookie" HTTP header for anonymous page
 * views. This tells a HTTP proxy that it may return a page from its local
 * cache without contacting the web server, if the user sends the same Cookie
 * header as the user who originally requested the cached page. Without "Vary:
 * Cookie", authenticated users would also be served the anonymous page from
 * the cache. If the site has mostly anonymous users except a few known
 * editors/administrators, the Vary header can be omitted. This allows for
 * better caching in HTTP proxies (including reverse proxies), i.e. even if
 * clients send different cookies, they still get content served from the cache.
 * However, authenticated users should access the site directly (i.e. not use an
 * HTTP proxy, and bypass the reverse proxy if one is used) in order to avoid
 * getting cached pages from the proxy.
 */
# $conf['omit_vary_cookie'] = TRUE;

/**
 * CSS/JS aggregated file gzip compression:
 *
 * By default, when CSS or JS aggregation and clean URLs are enabled Drupal will
 * store a gzip compressed (.gz) copy of the aggregated files. If this file is
 * available then rewrite rules in the default .htaccess file will serve these
 * files to browsers that accept gzip encoded content. This allows pages to load
 * faster for these users and has minimal impact on server load. If you are
 * using a webserver other than Apache httpd, or a caching reverse proxy that is
 * configured to cache and compress these files itself you may want to uncomment
 * one or both of the below lines, which will prevent gzip files being stored.
 */
# $conf['css_gzip_compression'] = FALSE;
# $conf['js_gzip_compression'] = FALSE;

/**
 * String overrides:
 *
 * To override specific strings on your site with or without enabling the Locale
 * module, add an entry to this list. This functionality allows you to change
 * a small number of your site's default English language interface strings.
 *
 * Remove the leading hash signs to enable.
 */
# $conf['locale_custom_strings_en'][''] = array(
#   'forum'      => 'Discussion board',
#   '@count min' => '@count minutes',
# );

/**
 *
 * IP blocking:
 *
 * To bypass database queries for denied IP addresses, use this setting.
 * Drupal queries the {blocked_ips} table by default on every page request
 * for both authenticated and anonymous users. This allows the system to
 * block IP addresses from within the administrative interface and before any
 * modules are loaded. However on high traffic websites you may want to avoid
 * this query, allowing you to bypass database access altogether for anonymous
 * users under certain caching configurations.
 *
 * If using this setting, you will need to add back any IP addresses which
 * you may have blocked via the administrative interface. Each element of this
 * array represents a blocked IP address. Uncommenting the array and leaving it
 * empty will have the effect of disabling IP blocking on your site.
 *
 * Remove the leading hash signs to enable.
 */
# $conf['blocked_ips'] = array(
#   'a.b.c.d',
# );

/**
 * Fast 404 pages:
 *
 * Drupal can generate fully themed 404 pages. However, some of these responses
 * are for images or other resource files that are not displayed to the user.
 * This can waste bandwidth, and also generate server load.
 *
 * The options below return a simple, fast 404 page for URLs matching a
 * specific pattern:
 * - 404_fast_paths_exclude: A regular expression to match paths to exclude,
 *   such as images generated by image styles, or dynamically-resized images.
 *   If you need to add more paths, you can add '|path' to the expression.
 * - 404_fast_paths: A regular expression to match paths that should return a
 *   simple 404 page, rather than the fully themed 404 page. If you don't have
 *   any aliases ending in htm or html you can add '|s?html?' to the expression.
 * - 404_fast_html: The html to return for simple 404 pages.
 *
 * Add leading hash signs if you would like to disable this functionality.
 */
$conf['404_fast_paths_exclude'] = '/\/(?:styles)\//';
$conf['404_fast_paths'] = '/\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
$conf['404_fast_html'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL "@path" was not found on this server.</p></body></html>';

/**
 * By default the page request process will return a fast 404 page for missing
 * files if they match the regular expression set in '404_fast_paths' and not
 * '404_fast_paths_exclude' above. 404 errors will simultaneously be logged in
 * the Drupal system log.
 *
 * You can choose to return a fast 404 page earlier for missing pages (as soon
 * as settings.php is loaded) by uncommenting the line below. This speeds up
 * server response time when loading 404 error pages and prevents the 404 error
 * from being logged in the Drupal system log. In order to prevent valid pages
 * such as image styles and other generated content that may match the
 * '404_fast_html' regular expression from returning 404 errors, it is necessary
 * to add them to the '404_fast_paths_exclude' regular expression above. Make
 * sure that you understand the effects of this feature before uncommenting the
 * line below.
 */
# drupal_fast_404();

/**
 * External access proxy settings:
 *
 * If your site must access the Internet via a web proxy then you can enter
 * the proxy settings here. Currently only basic authentication is supported
 * by using the username and password variables. The proxy_user_agent variable
 * can be set to NULL for proxies that require no User-Agent header or to a
 * non-empty string for proxies that limit requests to a specific agent. The
 * proxy_exceptions variable is an array of host names to be accessed directly,
 * not via proxy.
 */
# $conf['proxy_server'] = '';
# $conf['proxy_port'] = 8080;
# $conf['proxy_username'] = '';
# $conf['proxy_password'] = '';
# $conf['proxy_user_agent'] = '';
# $conf['proxy_exceptions'] = array('127.0.0.1', 'localhost');

/**
 * Authorized file system operations:
 *
 * The Update manager module included with Drupal provides a mechanism for
 * site administrators to securely install missing updates for the site
 * directly through the web user interface. On securely-configured servers,
 * the Update manager will require the administrator to provide SSH or FTP
 * credentials before allowing the installation to proceed; this allows the
 * site to update the new files as the user who owns all the Drupal files,
 * instead of as the user the webserver is running as. On servers where the
 * webserver user is itself the owner of the Drupal files, the administrator
 * will not be prompted for SSH or FTP credentials (note that these server
 * setups are common on shared hosting, but are inherently insecure).
 *
 * Some sites might wish to disable the above functionality, and only update
 * the code directly via SSH or FTP themselves. This setting completely
 * disables all functionality related to these authorized file operations.
 *
 * @see http://drupal.org/node/244924
 *
 * Remove the leading hash signs to disable.
 */
# $conf['allow_authorize_operations'] = FALSE;

/**
 * Smart start:
 *
 * If you would prefer to be redirected to the installation system when a
 * valid settings.php file is present but no tables are installed, remove
 * the leading hash sign below.
 */
# $conf['pressflow_smart_start'] = TRUE;

/** TWITTER OATH **/
$conf['tm_twitter_consumer_key'] = '';
$conf['tm_twitter_consumer_secret'] = '';

/** ENABLE FEEDME **/
/** This calls a module hosted on wordpress to embed blog (plus other) content based on current page **/
/** You will need this wordpress plugin: https://github.com/travelmassive/massiveplatform-wordpress-feedme **/
$conf['tm_enable_wordpress_feedme'] = false;
$conf['tm_wordpress_feedme_url'] = "/blog/wp-content/plugins/tm-feeds/feedme.php";
$conf['tm_wordpress_feedme_frontpage_id'] = "1831";

/** ENABLE FLAG FEEDS **/
/** This will display a feed of recent actions on the front page, such as signup, new members, and following **/
$conf['tm_enable_flags_feed'] = true;

/** Limit Follow and Join **/
/** NOTE: Administrator and chapter leaders do not have limits **/
$conf['tm_following_ratio_limit'] = '100'; // difference between following/followers (for approved members)
$conf['tm_following_ratio_limit_unapproved'] = '20'; // difference between following/followers (unapproved members)
$conf['tm_following_ratio_limit_daily'] = '0.5'; // number of followers added to limit each day since approval
$conf['tm_chapter_join_limit'] = '16'; // "join" limit for the non chapter leaders
$conf['tm_add_company_limit'] = '8'; // maximum number of companies a user can add (no limit for chapter leaders, moderators)

/** Settings for Email Templates **/
$conf['tm_site_name'] = "Travel Massive"; // used by emails and in templates to refer to the site
$conf['tm_email_server_http'] = "https"; // http or https depending on your setup. Will be used in email settings
$conf['tm_email_signoff'] = "Cheers,<br>- The Travel Massive Team"; // default email sign off at bottom of emails
$conf['tm_email_default_footer'] = "__UNSUBSCRIBE_LINK__<br>
Travel Massive P.B.C. 385 Grove St, San Francisco CA 94102<br> Phone +1 779-999-0042 &middot; community@travelmassive.com"; // used by emails and in templates to refer to the site

/** Send a copy of any announcement to this address **/
$conf['tm_announcement_copy_email'] = "commmunity@travelmassive.com";

/** Default reply_to address if left blank **/
$conf['tm_announcement_reply_email'] = "commmunity@travelmassive.com";

/** tm_invitations_custom_message **/
$conf['tm_invitations_custom_message'] = "I'd like to invite you to Travel Massive, a global community that connects travel industry insiders, leaders and innovators to collaborate and share ideas at locally organized events.";

/** tm_tips_start_chapter_link **/
$conf['tm_tips_start_chapter_link'] = "https://travelmassive.com/blog/start-a-chapter-guide/";

/** tm_tips_start_chapter_link **/
$conf['tm_tips_sponsor_page_link'] = "https://travelmassive.com/blog/sponsors-guide/";

/** tm_tips_chapter_leaders_guide **/
$conf['tm_tips_chapter_leaders_link'] = "https://travelmassive.com/blog/chapter-leader-resources/";

/** tm_community_values_url **/
$conf['tm_community_values_url'] = "https://travelmassive.com/blog/community-values/";

/** Where to send contact form emails to **/
$conf['tm_contact_page_email'] = "hello@travelmassive.com";

/** Member label for displaying membership counts **/
$conf["tm_member_label"] = "travel insider"; // singular
$conf["tm_members_label"] = "travel insiders"; // plural

/** Background video on front page **/
/** Set to true to enable. Set external url if you want mp4 to be delivered from external url (ie: vimeo) **/
/** Save your videos in tm/themes/videos/frontpage/background_video.mp4,ogv,webm,jpg **/
$conf['tm_background_bg'] = true;
$conf['tm_background_bg_title'] = "NYC Travel Massive";
$conf['tm_background_bg_link_url'] = "/chapters/new-york-city";
$conf['tm_background_bg_link_ext'] = false; // load external
$conf['tm_background_bg_mp4_external_url'] = "https://player.vimeo.com/external/129766268.hd.mp4?s=759daa867154be27e52a57d4d147073f&profile_id=113";

/** MESSAGING Options **/
/** Allow approved members to message each other via email if they follow each other **/
/** Emails will be sent by notification system and reply-to set to the sender's email address **/
$conf['tm_messaging_enabled'] = true; // turn on or off email messaging if following each other
$conf['tm_messaging_wait_time'] = 1800; // number of seconds between messaging the same person
$conf['tm_messaging_limit_days'] = 14; // number of days to check for rate limit
$conf['tm_messaging_limit_max'] = 30; // limit of messages you can send to different people within period
$conf['tm_messaging_from_email'] = "communications@travelmassive.com"; // from address. reply-to will be senders.

/** Event Options **/
$conf['tm_event_show_spots_left'] = 20; // show number of free seats when less than this number
$conf['tm_event_auto_register_waitlist'] = true; // if a member unregisters, automatically add next person from waitlist if there are free spots
$conf['tm_event_chapter_events_enabled'] = true; // allow chapters to create their own events
$conf['tm_event_company_events_enabled'] = true; // allow companies to create their own events
$conf['tm_event_member_events_enabled'] = true; // allow members to create their own events
$conf['tm_event_company_event_min_followers'] = 10; // how many followers a company needs to create an event. Set to 0 to disable.
$conf['tm_event_member_event_min_followers'] = 0; // how many followers a member needs to create an event. Set to 0 to disable.
$conf['tm_event_send_announcements_intro'] = "Get more people to your event by sending a great announcement. See our guide below."; // tip
$conf['tm_event_announcement_enable_cover_image'] = true; // enable including event cover image in announcement email
$conf['tm_event_online_timezones'] = array("America/Los_Angeles" => "Los Angeles", "America/New_York" => "New York", "Europe/London" => "London", "Europe/Athens" => "Athens", "Asia/Singapore" => "Singapore", "Australia/Sydney" => "Sydney"); // display online event date in these timezones

/** Provides a list of validated options on the signup page to indicate how the user connects with the community **/
/** These options are not saved to the user account **/
$conf["tm_community_values_description"] = "Our community is for travel industry insiders, leaders, and innovators.";
$conf["tm_community_values_options"] = array(
    0 => t('I work in the travel industry (example: startup, tour operator, DMO)'),
    1 => t('I work with travel industry customers (example: marketing, Tourism PR)'),
    2 => t('I create travel content (example: blogger, vlogger, presenter)'),
    3 => t('I study tourism or a closely related course'),
    4 => t('I am signing up as a travel company / brand'),
    5 => t('I love travel'));
$conf["tm_community_values_options_validate"] = array(
    0 => array("valid" => true, "message" => ""),
    1 => array("valid" => true, "message" => ""),
    2 => array("valid" => true, "message" => ""),
    3 => array("valid" => true, "message" => ""),
    4 => array("valid" => false, "message" => "<strong>Travel Massive accounts are for individuals.</strong> Please choose a community connection that applies to you personally. Once your account is approved you can create a company profile."),
    5 => array("valid" => false, "message" => "<strong>Why can't I join?</strong> Our community provides professional networking and development for members of the travel and tourism industry. We wish you safe travels and instead please become a <a href='https://travelmassive.com/blog/i-love-travel/'>friend of our community</a>."));

/** Specify the 5 questions users are asked in their profile **/
/** Note: Once you launch, you can't change these **/
$conf["tm_field_user_question_1_title"] = "What is your favorite travel destination?";
$conf["tm_field_user_question_2_title"] = "Where do you dream of traveling to?";
$conf["tm_field_user_question_3_title"] = "What was your first travel job?";
$conf["tm_field_user_question_4_title"] = "What do you want to learn more about?";
$conf["tm_field_user_question_5_title"] = "Three words that describe why we should travel?";

/** Site specific title and descriptions in profile and node edit **/
$conf["tm_field_job_role_description"] = "examples: Founder, CEO, VP Marketing, Social Media Manager, Chief Explorer";
$conf["tm_field_friendly_url_title"] = "Travel Massive URL (travelmassive.com/your-name)";
$conf["tm_field_friendly_url_description"] = "Make your profile available through a friendly URL such as https://travelmassive.com/your-name.<br>Example: \"your-name\"";
$conf["tm_field_company_friendly_url_title"] = "Travel Massive URL (travelmassive.com/your-company)";
$conf["tm_field_company_friendly_url_description"] = "Make your company profile available through a friendly URL such as https://travelmassive.com/your-company.<br>Example: \"your-company\"";

/** Approval welcome message when approved **/
$conf["tm_notification_approval_welcome_message"] = '<strong>ARE YOU READY TO CHANGE TRAVEL?</strong>
<br>
<br>Our mission is to connect travel insiders in every city in the world in order to empower change in travel.
<br>
<br>Travel Massive has always been an open and collaborative community founded on three simple principles – to be open to everyone in the travel industry, to be free to participate, and to connect and share globally.
<br>
<br>In addition to our core values we have the following community values:
<br>
<br>* Building Meaningful Partnerships
<br>* Fostering Innovation And Big Ideas
<br>* Supporting Diversity And New Talent
<br>* Empowering Change In Travel
<br>* Promoting Responsible Travel
<br>
<br>You can read more on our public plan at <a href="http://www.travelmassive.org">www.travelmassive.org</a>
<br>
<br>If there\'s a chapter in your city, then please join it and attend our events to connect with like-minded travel industry professionals. If you can\'t find a chapter, why don\'t you consider starting a Travel Massive chapter? Get in touch with us to find out more.';

/** Membership criteria provided if member flagged as non-community member **/
$conf["tm_notification_moderation_non_community_message"] = 'To be a member of the Travel Massive community, you must demonstrate one of the following:
<br>* You are active in the travel industry (example: tour operator, travel brand, DMO)
<br>* You are active in a field closely related to the travel industry (example: Tourism PR)
<br>* You are are a travel content creator (example: blogger, vlogger, presenter)
<br>* You are studying tourism or a closely related course
<br>* Any other criteria that reasonably demonstrates a professional connection to the tourism or travel industry.
<br>* You can read our community values at https://travelmassive.com/blog/community-values/
<br>
<br>
<br><strong>But I love travel, can I join still?</strong>
<br>Our community provides professional networking and development for members of the travel and tourism industry. If you are an expert or avid traveler, then thanks for stopping by and we wish you all the best on your journeys.';

/** Set a front page launch message and hide the stats. Comment out to disable **/
//$conf['tm_frontpage_launch_message'] = "We've just launched our new community platform. Read our <a style='color: #fff; text-decoration: underline;' href='/blog/'>announcement</a>.";

/* Stats option for front page */
/* Options: members, chapters, organizations, connections, mutual_follows */
$conf['tm_frontpage_show_anon_stats'] = array('members', 'chapters', 'connections');
$conf['tm_frontpage_meta_og_title'] = "Travel Massive";
$conf['tm_frontpage_meta_og_image'] = "";
$conf['tm_frontpage_meta_description'] = "Travel Massive connects thousands of travel insiders to meet, learn and collaborate at free events all around the world. We are a global community of travel industry insiders, leaders, and innovators.
.";

/** Event types **/
$conf['tm_event_types_system'] = array("community" => "Chapter Event", "member" => "Member Event", "company" => "Company Event"); // system event types - don't alter
$conf['tm_event_types_custom'] = array("workshop" => "Workshop", "conference" => "Conference"); // add your own custom events here
$conf['tm_event_types'] = array_merge($conf['tm_event_types_system'], $conf['tm_event_types_custom']);
$conf['tm_event_types_display'] = array("all" => "All Events", "community" => "Community", "workshop" => "Workshops", "conference" => "Conferences", "member" => "Member Events", "company" => "Company Events");
$conf['tm_event_types_tips'] = array("workshop" => "Learn and share with other members around the world. have an idea for a workshop you would like to attend or host? Please <a href='/contact'>let us know</a>.", "conference" => "We're proud community partners with the world's leading travel industry conferences and exhibitions. <a href='/blog/about-travel-massive'>Learn more</a>");
$conf['tm_event_types_default'] = "all";
$conf['tm_event_types_sticker_icon_color'] = "#000000"; // color of sticker for non-chapter events
$conf['tm_event_types_edit_tips'] = array();
$conf['tm_event_types_edit_tips']['community'] = "Make your event stand out. Here's a few <a target='_blank' href='#'>tips</a>.";
$conf['tm_event_types_edit_tips']['company'] = "Create a company event in our community.<br>Please follow our <a target='_blank' href='" . $conf['tm_community_values_url'] . "'>community guidelines</a>.";
$conf['tm_event_types_edit_tips']['member'] = "Create a member event in our community.<br>Please follow our <a target='_blank' href='" . $conf['tm_community_values_url'] . "'>community guidelines</a>.";

/** If user removes their account, who do we notify? */
$conf['tm_user_remove_own_account_notify'] = 'community@travelmassive.com';

/** Discussion settings **/
$conf['tm_discuss_menu_hide'] = true; // hide the discussions unless you access /discuss
$conf['tm_discuss_menu_class'] = 'menu-1458'; // class of the menu item to hide
$conf['tm_discuss_meta_og_title'] = "Travel Massive Discuss";
$conf['tm_discuss_meta_og_image'] = "https://localdev.travelmassive.com/discussions/uploads/0O56BOQETSB2.png";
$conf['tm_discuss_meta_description'] = "Travel Massive Discuss is the place to ask questions and get answers about the tourism and travel industry from members of our community.";
$conf['tm_discuss_menu_path'] = "discuss"; // page where vanilla will be embedded
$conf['tm_discuss_full_path'] = "discussions"; // where vanilla is installed
// $conf['tm_discuss_signup_message'] = ""; // customise the signup message. leave unset for default
$conf['tm_discuss_enable_feedme'] = true; // show recent discussions for a chapter or member

/** Marketplace settings **/
$conf['tm_marketplace_menu_hide'] = true; // hide the marketplace unless you access /marketplace
$conf['tm_marketplace_menu_class'] = 'menu-1510'; // class of the menu item to hide
$conf['tm_marketplace_menu_path'] = "marketplace"; // where jobskee is installed
$conf['tm_marketplace_cookie_enable'] = true; // for marketplace subscription email field
$conf['tm_marketplace_cookie_secret_key'] = 'randomstringabc'; // must match jobskee config
$conf['tm_marketplace_cookie_secret_iv'] = 'randomstring123'; // must match jobskee config
$conf['tm_marketplace_enable_feedme'] = true; // embed marketplace jobs into chapter and company pages
$conf['tm_marketplace_feedme_url'] = '/marketplace/api/search/'; // url for search

/* Profile and company link fields */
/* Options available: website, twitter, facebook, linkedin, instagram, youtube, vimeo, snapchat */
$conf["tm_users_link_fields"] = array('website', 'twitter', 'facebook', 'linkedin', 'instagram', 'youtube', 'vimeo', 'snapchat');
$conf["tm_organizations_link_fields"] = array('website', 'twitter', 'facebook', 'linkedin', 'instagram', 'youtube', 'vimeo');

/* Display a tip on chapter insights page */
$conf["tm_reports_chapter_insights_tip"] = "Join our Facebook group to share tips with other members on growing your chapter and be part of our global founders team!";

/* Randomize default images */
/* After editing, run drush image-flush */
$conf["tm_images_default_path"] = "public://default_images/"; // Unset if you don't want to use random default images
$conf["tm_images_default_field_avatar"] = "public://default_images/avatar-default.png"; // The default image for field_avatar
$conf["tm_images_default_field_image"] = "public://default_images/cover-default.png"; // The default image for field_image
$conf["tm_images_default_avatar"] = array("default_avatar_1.jpg", "default_avatar_2.jpg", "default_avatar_3.jpg", "default_avatar_4.jpg", "default_avatar_5.jpg");
$conf["tm_images_default_cover_user"] = array("default_cover_user_1.jpg", "default_cover_user_2.jpg", "default_cover_user_3.jpg", "default_cover_user_4.jpg", "default_cover_user_5.jpg");
$conf["tm_images_default_cover_organization"] = array("default_cover_organization_1.jpg", "default_cover_organization_2.jpg", "default_cover_organization_3.jpg", "default_cover_organization_4.jpg");
$conf["tm_images_default_cover_event"] = array("default_cover_event_1.jpg", "default_cover_event_2.jpg", "default_cover_event_3.jpg");
$conf["tm_images_default_cover_chapter"] = array("default_cover_chapter_1.jpg");

/* Event payments setting */
$conf["tm_payments_stripe_publishable_key"] = "publishable"; // stripe publishable key
$conf["tm_payments_stripe_image_url"] = '/sites/all/themes/tm/images/travel_massive_stripe_payment_icon.png'; // stripe image url
$conf["tm_payments_process_url"] = "https://localdev.travelmassive.com/payments/process.php"; // external payment processor
$conf["tm_payments_process_check_certificate"] = false; // turn on for production 
$conf["tm_payments_process_timeout"] = 10; // seconds to wait for process url before timeout 
$conf["tm_payments_process_error_email"] = "support@travelmassive.com"; // if payment process fails, who to email
$conf["tm_payments_error_message"] = "A problem occured processing your payment, please contact support@travelmassive.com";
$conf["tm_payments_currencies"] = array("usd" => "\$", "aud" => "AUD \$", "cad" => "CAD \$", "eur" => "€", "gbp" => "GBP £"); // available currencies (must be supported by stripe)
$conf["tm_payments_currencies_default"] = "usd"; // default currency
$conf["tm_payments_currency_symbols"] = array("usd" => "$", "aud" => "$", "cad" => "$", "eur" => "€", "gbp" => "£");
$conf["tm_payments_refund_policy"] = "You can get a refund until 7 days before the event";
$conf["tm_payments_refund_url"] = "https://travelmassive.com/blog/event-refund/";
$conf["tm_payments_receipt_details"] = "<strong>TAX RECEIPT</strong><br>Your company name<br>Your company address<br>Email: your@receiptemail";
$conf["tm_payments_early_bird_label"] = "Early Bird Ticket"; // default label for early bird ticket
$conf["tm_payments_stripe_logo"] = "<a target='_blank' href='https://stripe.com/gallery'><img style='width: 119px; height: 26px;' width='119' height='26' src='/sites/all/themes/tm/images/stripe_logo_solid@2x.png'></a><br>Secure payment gateway trusted by <a target='_blank' href='https://stripe.com/gallery'>global brands</a>"; // displayed when payment is enabled
$conf["tm_payments_handler_name"] = "Travel Massive"; // merchant name to display on payment screen
$conf["tm_payments_handler_description"] = "Event Registration"; // default description to display on payment screen
$conf["tm_payments_enable_chapter_min_members"] = 100; // enable payments if event chapter has minimum member count (set to 0 to disable)
$conf["tm_payments_enable_help_text"] = "Congratulations, you can accept payments for events.<br>By accepting payments you agree to our event <a target='_blank' href='#'>terms and conditions</a>.";
$conf["tm_payments_commission_default_chapter"] = "80"; // default commission % for chapter event 
$conf["tm_payments_commission_default_company"] = "75"; // default commission % for company event
$conf["tm_payments_commission_default_member"] = "70"; // default commission % for member event
$conf["tm_payments_reports_url"] = "https://localdev.travelmassive.com/payments/reports.php"; // external payment reports
$conf["tm_payments_reports_secret_token"] = "randomstring123"; // secret token to verify payment report 
$conf["tm_payments_reports_help_message"] = "Here's your ticket sales for this event."; // message to show user

/* Featured members settings */
$conf["tm_featured_members_event_types"] = array("all" => "Special Guests", "community" => "Special Guests", "workshop" => "Workshop Leaders", "conference" => "Speakers");
$conf["tm_featured_members_badge_text"] = "A Travel Massive Rockstar!";

/* Allow short message to be sent on follow */
$conf["tm_following_enable_message_user"] = true;
$conf["tm_following_enable_message_organization"] = true;

/* Chapter settings */
$conf["tm_chapters_allow_edit_chapter_leaders"] = false; // Allow chapter leaders to edit leaders for their own chapter (default false)
$conf["tm_chapters_groups_enabled"] = true; // Allow group chapters. Exposes the /groups url.

/* Set the theme color for mobile Android, iOS */
$conf["tm_theme_meta_color"] = "#007DB7"; 

/* Search settings */
$conf["tm_search_api_results_per_page"] = 10; // how many results to show per page
$conf["tm_search_api_max_search_terms"] = 8; // max terms allowed in search
$conf["tm_search_api_top_results_display"] = 5; // perform a quote search first, and insert N results at top
$conf["tm_search_api_top_results_strip_spaces"] = true; // if enabled, will also search for string with no spaces in top results
$conf["tm_search_cache_results_seconds"] = 60; // set to 0 to disable cache and rely on search_api_db cache
$conf["tm_search_api_catch_exceptions"] = true; // will display friendly error message if error occurs
$conf["tm_search_api_pluralize_keywords"] = true; // search for keyword plurals - ie: nomad = "nomad", "nomads"
$conf["tm_search_api_ignore_keywords_people"] = array(); // ignore these keywords when searching people
$conf["tm_search_api_ignore_keywords_events"] = array(); // ignore these keywords when searching events
$conf["tm_search_api_ignore_keywords_chapters"] = array("travel", "massive", "travel massive"); // ignore these keywords when searching chapters
$conf["tm_search_api_ignore_keywords_companies"] = array(); // ignore thesee keywords when searching companies
$conf["tm_search_open_external"] = true; // open search result in new window
$conf['tm_search_meta_og_title'] = "Travel Massive Search";
//$conf['tm_search_meta_og_image'] = ""; // url of search
$conf['tm_search_meta_description'] = "Search our community.";
$conf['tm_search_meta_opensearch_title'] = "travelmassive.com"; // title for opensearch header link

// example search tips
$conf["tm_search_api_tips"] = array(
  "Founders in Italy: <i><a href='#' class='search-example' data-search-example='role:founder in:Italy'>role:founder in:Italy</i></a>",
  "Snapchatters in Brazil: <i><a href='#' class='search-example' data-search-example='has:snapchat in:Brazil'>has:snapchat in:Brazil</i></a>",
  "Instagrammers in Kenya: <i><a href='#' class='search-example' data-search-example='has:instagram in:Kenya  '>has:instagram in:Kenya</i></a>",
  "Video creators in Europe: <i><a href='#' class='search-example' data-search-example='has:vimeo in:Europe'>has:vimeo in:Europe</i></a>",
  "Startups in San Francisco: <i><a href='#' class='search-example' data-search-example='startup in:\"San Francisco\"'>startup in:\"San Francisco\"</i></a>",
  "Travel Startups in Europe: <i><a href='#' class='search-example' data-search-example='startups in:Europe'>startups in:Europe</i></a>",
  "Tour companies in Africa: <i><a href='#' class='search-example' data-search-example='segment:tour in:Africa'>segment:tour in:Africa</a></i>",
  "Content creators in India: <i><a href='#' class='search-example' data-search-example='segment:content in:India'>segment:content in:India</a></i>",
  "Digital Nomads: <i><a href='#' class='search-example' data-search-example='coworking nomad'>nomad</a></i>",
  "Tourism students: <i><a href='#' class='search-example' data-search-example='segment:education student'>segment:education student</a></i>",
  "Marketing in Miami: <i><a href='#' class='search-example' data-search-example='segment:marketing in:Miami'>segment:marketing in:Miami</a></i>",
  "Luxury travel in Asia: <i><a href='#' class='search-example' data-search-example='luxury in:Asia'>luxury in:Asia</a></i>",
  "Event Sponsors in United States: <i><a href='#' class='search-example' data-search-example='flag:sponsor in:\"United States\"'>flag:sponsor in:\"United States\"</a></i>",
  "Investors: <i><a href='#' class='search-example' data-search-example='investors'>investors</a></i>",
  "Hospitality: <i><a href='#' class='search-example' data-search-example='segment:hospitality'>segment:hospitality</a></i>",
  "Aviation: <i><a href='#' class='search-example' data-search-example='segment:aviation'>segment:aviation</a></i>",
  "TV hosts: <i><a href='#' class='search-example' data-search-example='role:host segment:media'>role:host segment:media</a></i>",
  "Search by topic. Example <i><a href='#' class='search-example' data-search-example='photographer'>photographer</a></i>",
  "Filter by location. Example: <i><a href='#' class='search-example' data-search-example='in:Tokyo'>in:Tokyo</a></i>",
  "Filter by segment. Example: <i><a href='#' class='search-example' data-search-example='segment:marketing'>segment:marketing</a></i>",
  "Filter by role. Example: <i><a href='#' class='search-example' data-search-example='role:manager'>role:manager</a></i>",
  "Filter by company. Example: <i><a href='#' class='search-example' data-search-example='at:Airbnb'>at:Airbnb</a></i>",
  "Exact match a name. Example: <i><a href='#' class='search-example' data-search-example='\"Lauren Nicholl\"'>\"Lauren Nicholl\"</a></i>",
  "Find a company. Example: <i><a href='#' class='search-example' data-search-example='\"Urban Adventures\"'>\"Urban Adventures\"</a></i>",
  "Help us improve search. Send us <i><a href='/contact'>feedback</a></i>."
);

// what to show if no results
$conf["tm_search_api_no_results_text"] = "<strong>There's more places we can help you search...</strong>
        <ul>
          <li><a class='search-external-blog' target='_blank' href='#'>Search on our blog</a> for news articles and interviews.</li>
          <li><a class='search-external-jobs' target='_blank' href='#'>Search jobs</a> for new opportunities.</li>
          <li><a class='search-external-qa' target='_blank' href='#'>Search our community Q&amp;A</a>.</li>
          <li>Or, try a <a class='search-external-google' target='_blank' href='#'>Google Search</a> on our site.</li>
          <li>Still can't find it? Get in <a target='_blank' href='/contact'>contact with us</a>.</li>
        </ul>";

// help text to show
$conf["tm_search_api_help_text"] = "<p style='font-size: 16px;'><strong>More places</strong></p>
        <ul>
          <li><a class='search-external-blog' target='_blank' href='#'>Search on our blog</a></li>
          <li><a class='search-external-jobs' target='_blank' href='#'>Search jobs</a></li>
          <li><a class='search-external-qa' target='_blank' href='#'>Search community Q&amp;A</a></li>
          <li>Or, try a <a class='search-external-google' target='_blank' href='#'>Google Search</a></li>
        </ul>
<p style='border-top: 1px solid #eee; padding-top: 1rem; margin-right: 1.2rem; font-size: 10pt; color: #79828c;'>
Welcome to Travel Massive Search - the largest open database of the travel industry. Help us keep this up to date by adding your <a href='/companies'>company profile</a>.
<br>PS: This product is made with <span style='color: #e93f33 !important;'>♥</span> for the benefit of our community. Keep building the future of travel! - <a href='/ian'>Ian</a>.</p>
";

// search tips to show
$conf["tm_search_api_help_tips"] = "<p style='font-size: 16px;'><strong>Example filters</strong></p>
<p style='border-top: 1px solid #eee; padding-top: 1rem; margin-right: 1.2rem; font-size: 10pt; color: #79828c;'>
Add options to your search text to filter your results. Here's some examples.
  <ul>
    <li><a href='#' class='search-example' data-search-example='in: \"New York\"'>in: \"New York\"</a></li>
    <li><a href='#' class='search-example' data-search-example='in: Thailand'>in: Thailand</a></li>
    <li><a href='#' class='search-example' data-search-example='segment: marketing'>segment: marketing</a></li>
    <li><a href='#' class='search-example' data-search-example='role: manager'>role: manager</a></li>
    <li><a href='#' class='search-example' data-search-example='at: Expedia'>at: Expedia</a></li>
    <li><a href='#' class='search-example' data-search-example='has: snapchat'>has: snapchat</a></li>
  </ul>
</p>";

// enable cookie consent
$conf["tm_cookieconsent_enable"] = true; // enable EU cookie consent notice
$conf["tm_cookieconsent_privacy_url"] = ""; // link to privacy policy

// notifications unsubscribe link
$conf["tm_notifications_subscription_secret_key"] = "randomstring123"; // random string

// cover videos
$conf["tm_cover_videos_enabled"] = true; // enable cover video

// newsfeed
$conf["tm_newsfeed_curl_timeout"] = 2; // seconds for curl timeout
$conf["tm_newsfeed_curl_check_certificate"] = false; // check ssl certificate
$conf["tm_newsfeed_marketplace_enabled"] = true; // enable marketplace newsfeed
$conf["tm_newsfeed_marketplace_api_url"] = ""; // url for marketplace api
$conf["tm_newsfeed_marketplace_cache"] = 60; // how long to cache results in seconds
// $conf["tm_newsfeed_marketplace_api_userpass"] = "user:pass"; // uncomment to use basic httpauth 
$conf["tm_newsfeed_blog_enabled"] = true; // enable blog newsfeed
$conf["tm_newsfeed_blog_api_url"] = ""; // url for blog api
$conf["tm_newsfeed_blog_cache"] = 60; // how long to cache results in seconds
// $conf["tm_newsfeed_blog_api_userpass"] = "user:pass"; // uncomment to use basic httpauth 


$conf['install_profile'] = 'tm';


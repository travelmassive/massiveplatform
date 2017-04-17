<?php
/**
 * @file
 * Defines module constants. Provided via a separate file to fix issues
 * with installing the module in a custom provile.
 *
 * @see https://www.drupal.org/node/2283255
 */

// jsTimezeDetect library information.
define('TIMEZONE_DETECT_LIBRARY_WEBSITE', 'http://pellepim.bitbucket.org/jstz/');
define('TIMEZONE_DETECT_LIBRARY_FILENAME', 'jstz.js');
define('TIMEZONE_DETECT_LIBRARY_DOWNLOAD_URL', 'https://bitbucket.org/pellepim/jstimezonedetect/raw/default/jstz.js');

// Detection modes configured via administration form.
// Note these are strings instead of integers because system_settings_form()
// saves variable values as strings.
define('TIMEZONE_DETECT_MODE_DEFAULT', '0');
define('TIMEZONE_DETECT_MODE_LOGIN', '1');
define('TIMEZONE_DETECT_MODE_ALWAYS', '2');

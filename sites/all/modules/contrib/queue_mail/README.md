Queue mail
----------
Queues all mail sent by your Drupal site so that it is sent via cron
using the Drupal 7 Queue API. This is helpful for large traffic sites where
sending a lot of emails per page request can slow things down considerably.

INSTALLATION
------------

Queue mail can be installed like any other Drupal module
- place it in the modules directory for your site and enable it on the `admin/modules` page.
- Copy the module url and goto `admin/modules/install`, then paste
 the url to `Install from a URL` textfield, click `Install` and enable it on the
 `admin/modules` page at the last time.

REQUIREMENTS
------------

* system (>=7.12)

CONFIGURATION
-------------

The configuration page is at admin/config/system/queue_mail,
where you can configure the queue mail module
and enable challenges for the desired mailId.

FOR DEVELOPERS
--------------

Please read Drupal queue api for more information.


IP address manager README

CONTENTS OF THIS FILE
----------------------
  * Introduction
  * Installation
  * Configuration
  * Usage
  

INTRODUCTION
------------
Project page: http://drupal.org/project/ip.


INSTALLATION
------------
1. Copy ip folder to modules (usually 'sites/all/modules') directory.
2. At the 'admin/modules' page, enable the IP address manager module.

Optional: this module provides drush commands to recovery IP address data loaded
prior to module instalation.

- drush ip-backlog-nodes
  It imports nodes IP data from watchdog (dblog must be installed).

- drush ip-backlog-comments
  It imports comments IP data from comment table.


CONFIGURATION
-------------
Enable permissions at 'admin/people/permissions'.  Users with the permission 
'manage ip addresses' will be able to use this module.


USAGE
-----
Go to /user/%user/ip to see user's IPs.
Go to /admin/people/ip to see all user's IPs.

This module is integrated with views, create your own views!

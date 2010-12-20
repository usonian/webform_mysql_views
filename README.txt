/* $Id$ */

-- SUMMARY --

The Webform MySQL Views builds flattened, read-only MySQL views of webform
submission data. These views may be useful when you need to access this data
from an external application in an automated fashion without exporting,
importing, or the use of a web-based API.


-- REQUIREMENTS --

* Your Drupal database must be using the MySQL backend.

* Your MySQL server must be version 5.0 or later

* The MySQL user specified in your Drupal settings.php file must have permission
  to create views.

* Webform Module

* Elements Module


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* To manage MySQL views for your webforms, log in as an administrator and go to
  the Administer > Content Management > Web Forms page and click on the MySQL
  Views tab.


-- CONTACT --

Current maintainers:
* Andy Chase (usonian) - http://drupal.org/user/164378


This project has been sponsored by:
* The Proof Group LLC
  Visit http://proofgroup.com for more information.

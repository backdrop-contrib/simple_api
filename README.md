Simple API
==========

This module creates 8 endpoints which return data in JSON format and are accessible via a GET or POST request.
It features the ability to create, update, or delete a node.

HOW TO INSTALL:
---------------
- Install this module using the official Backdrop CMS instructions at
https://backdropcms.org/guide/modules


HOW TO SETUP:
-------------
1. This module creates two user roles, "Simple API User" and "Simple API Full".

  * 1a. A user with the role "Simple API User" may access the API for one hour after which they will need to re-authenticate.

  * 1b. A user with the role "Simple API Full" has access to the API with no time limit.

2. Assign one of the roles to any user you wish to give API access to.

3. It's recommended that the admin set up a system cron job or scheduled task to execute Backdrop Cron at least once an hour.
    as described here: https://backdropcms.org/cron.

HOW TO USE:
-----------
- Simple API provides a list of authenticated users here: /admin/reports/simple_api

- To revoke a users access to the API, un-check any API user roles in the users profile, then save to update the user.

- Running Cron at https://sitename.com/admin/config/system/cron will remove the users access immediately.

- Authentication is simplified: a properly configured user only has to access https://sitename.com/api/%username%/login
  to be authenticated for one hour.

- No passwords are needed to authenticate.

1. https://sitename.com/api/%username%/login -- A Backdrop user name can be passed to login to the API
2. https://sitename.com/api/%username%/user/UID -- where the UID is a number like 23
3. https://sitename.com/api/%username%/user/list -- a list of UIDs and user names
4. https://sitename.com/api/%username%/node/list -- a list of nodes with NID, type, title and status
5. https://sitename.com/api/%username%/node/NID -- where the NID is a number like 32 or
6. https://sitename.com/api/%username%/node/0/create -- POST an array of variables to create a new node of any type
7. https://sitename.com/api/%username%/node/NID/update -- POST an array of variables to update a node of any type where the NID is a number like 32
8. https://sitename.com/api/%username%/node/NID/delete -- POST an array of variables to delete a node of any type where the NID is a number like 32

* NOTE that %username% should be the Backdrop user's name who has been assigned one of the roles mentioned in HOW TO SETUP: #2 .

- After authentication, all API endpoints can be accessed with the username following /api/ in the path path ex.
  https://somesite.net/api/someuser/node/42

EXAMPLE USAGE:

/* --- LOGIN --- */

$handle = curl_init();
$url = "http://mysite.net/api/username/login";
curl_setopt_array($handle, array(
	CURLOPT_URL => $url,
	CURLOPT_RETURNTRANSFER => TRUE,
	)
);
$data = curl_exec($handle);
curl_close($handle);
echo $data;


/* --- CREATE A NODE --- */

$handle = curl_init();
$url = "http://mysite.net/api/username/node/0/create";
$postData = array(
	'title' => 'My New Page',
	'type' => 'page',
	'status' => 1,
	'promote' => 0,
	'comment' => 1,
	'body[und][0][value]' => 'Place some interesting text into the body of the new node.',
);
curl_setopt_array($handle, array(
	CURLOPT_URL => $url,
	CURLOPT_POST => TRUE,
	CURLOPT_POSTFIELDS => $postData,
	CURLOPT_RETURNTRANSFER => TRUE,
	)
);
$data = curl_exec($handle);
curl_close($handle);
echo $data;


/* --- UPDATE A NODE --- */

$handle = curl_init();
$url = "https://mysite.net/api/username/node/57/update";
$postData = array(
	'nid' => '57',
	'body[und][0][value]' => 'Place some other even more interesting text into the body of the node.',
);
curl_setopt_array($handle, array(
	CURLOPT_URL => $url,
	CURLOPT_POST => TRUE,
	CURLOPT_POSTFIELDS => $postData,
	CURLOPT_RETURNTRANSFER => TRUE,
	)
);
$data = curl_exec($handle);
curl_close($handle);
echo $data;


/* --- DELETE A NODE --- */

$handle = curl_init();
$url = "https://mysite.net/api/username/node/57/delete";
$postData = array(
	'nid' => '57',
);
curl_setopt_array($handle, array(
	CURLOPT_URL => $url,
	CURLOPT_POST => true,
	CURLOPT_POSTFIELDS => $postData,
	CURLOPT_RETURNTRANSFER => true,
	)
);
$data = curl_exec($handle);
curl_close($handle);
echo $data;


LICENSE:
---------------    
This project is GPL v2 software. See the LICENSE.txt file in this directory
for complete text.

CURRENT MAINTAINERS:
---------------    
- [earlyburg](https://github.com/earlyburg).

CREDITS:
---------------
- [earlyburg](https://github.com/earlyburg).


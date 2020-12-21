<?php

/**
 * @file
 * Authentication for API users
 */
 function simple_api_auth($uname) {
   /* Backdrop user name check */
   $backdrop_usercheck = db_select('users', 'n')
     ->fields('n', array('uid'))
     ->condition('name', $uname)
     ->execute()
     ->fetchAssoc();
   $backdrop_user = user_load($backdrop_usercheck['uid']);
   /* get the IP that the request originated from if possible */
   if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
     $ip = $_SERVER['HTTP_CLIENT_IP'];
   }
   else if (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
     $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
   }
   else if (!empty($_SERVER['REMOTE_ADDR'])) {
     $ip = $_SERVER['REMOTE_ADDR'];
   }
   else {
     $ip = 'No IP address available.';
   }
   if ( user_has_role('simpleApiUser', $backdrop_user) || user_has_role('simpleApiFull', $backdrop_user) ) {
     $loaded_username = $backdrop_user->name;
     $loaded_uid = $backdrop_user->uid;
     /* timestamp */
     $ts = time();
     $login_msg = array(
       'MESSAGE' => 'Welcome, ' . $backdrop_user->name,
       'INFO' => 'Simple API v1.x-1.x',
       'ROUTES' => array(
         '1' => '/api/' . $backdrop_user->name . '/login',
         '2' => '/api/' . $backdrop_user->name . '/node/{nid}',
         '3' => '/api/' . $backdrop_user->name . '/node/{nid}/delete',
         '4' => '/api/' . $backdrop_user->name . '/node/{nid}/update',
         '5' => '/api/' . $backdrop_user->name . '/node/0/create',
         '6' => '/api/' . $backdrop_user->name . '/{nodetype}/list',
         '7' => '/api/' . $backdrop_user->name . '/user/{uid}',
         '8' => '/api/' . $backdrop_user->name . '/user/list',
         ),
       );
     if ( simple_api_check_user($loaded_username) ) {
       /* authenticate a user */
       $query = db_update('simple_api')
         ->condition('usersname', $loaded_username)
         ->fields(array(
           'authorized' => 'yes',
           'timestamp' => $ts))
         ->execute();
     }
     else {
       /* create a new API user */
       $query = db_insert('simple_api')
         ->fields(array(
           'usersname' => $loaded_username,
           'usersid' => $loaded_uid,
           'timestamp' => $ts,
           'ipaddress' => $ip,
           'authorized' => 'yes'))
         ->execute();
     }
     $type = "simple_api";
     $message = "Simple API User " . $loaded_username . " was authenticated from " . $ip;
     watchdog($type, $message, $variables = array(), $severity = WATCHDOG_INFO, $link = NULL);
   }
   else {
     $login_msg = array("ERROR" => "Not authorized.");
     $type = "simple_api";
     $message = "Simple API: " . $uname . " from ". $ip ." tried to access the API but was not authorized.";
     watchdog($type, $message, $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL);
   } /* END bakdrop user role check */
   backdrop_json_output($login_msg);
   backdrop_exit();
 }
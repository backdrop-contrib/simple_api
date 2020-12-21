<?php

/**
 * @file
 * CRUD controller for Backdrop nodes
 */
function simple_api_crud($user, $nid, $pathop) {
  // Make sure it's a POST request.
  if ( $_SERVER['REQUEST_METHOD'] == 'POST') {
    if ( simple_api_check_auth($user) ) {
      // sanitize the POST input
      $params = filter_input_array(INPUT_POST, FILTER_SANITIZE_STRING);
      $backdrop_usercheck = db_select('users', 'n')
        ->fields('n', array('uid'))
        ->condition('name', $user)
        ->execute()
        ->fetchAssoc();
      $currentUser = user_load($backdrop_usercheck['uid']);
      $uid = $currentUser->uid;
      switch ($pathop) {
        case "create":
          /* CREATE A NODE ---> */
          $node = new Node();
          foreach ($params as $key => $value) {
            if ($key != 'nid') {
              $node->$key = $value;
            }
          }
          $node->uid = $uid;
          $node->langcode = LANGUAGE_NONE;
          node_object_prepare($node);
          $node = node_submit($node);
          node_save($node);
          if (isset($node->nid)) {
            $var = $node;
          }
          else {
            $var = array("ERROR" => "node not created");
          }
            break;
        case "update":
          /* UPDATE A NODE ---> */
          $node = node_load($params['nid'], NULL, TRUE);
          if ($node) {
             foreach ($params as $key => $value) {
              if ($key != 'nid') {
                $node->$key = $value;
              }
            }
            try {
              node_save($node);
              $success = TRUE;
            }
            catch (Exception $e) {
              $success = FALSE;
            }
            if ($success) {
              $var = node_load($params['nid'], NULL, TRUE);
            }
            else {
              $var = array("ERROR" => "node not updated", "EXCEPTION" => $e->getMessage());
            }
          }
          else {
            $var = array("ERROR" => "node not found");
          }
            break;
        case "delete":
          /* DELETE A NODE ---> */
          if (isset($params['nid'])) {
            $node = node_load($params['nid'], NULL, TRUE);
            if ($node) {
              node_delete($params['nid']);
              $var = array("SUCCESS" => "Node with ID " . $node->nid . " successfuly deleted");
            }
            else {
              $var = array("ERROR" => "Node with ID " . $params['nid'] . " not found");
            }
          }
          else {
            $var = array("ERROR" => "Node ID is required");
          }
            break;
        default:
          $var = simple_api_view();
      }
      backdrop_json_output($var);
      backdrop_exit();
    }
    else {
      simple_api_unauth();
    }
  }
}
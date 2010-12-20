<?php //$Id$

/**
 * @file
 *
 * Administration pages provided by the Webform MySQL Views module.
 */

/**
 * Page callback for admin/content/webform/mysql.
 *
 * Implemented as its own admin page instead of integrating with the node
 * form because of chicken-and-egg logic hassles.
 */
function webform_mysql_views_admin_form() {
  // Get list of webform-enabled content types
  $types = webform_variable_get('webform_node_types');

  //Get list of all webform nodes
  $query = db_rewrite_sql("SELECT nid, title FROM {node} n WHERE type IN ('%s') AND status = 1 ORDER BY nid");
  $result = db_query($query, implode("','", $types));


  $options = array();
  while ($node = db_fetch_object($result)) {
    $options[$node->nid] = array('title' => $node->title);
  }


  $form = array();

  $header = array(
    'title' => t('Webform Name'),
  );

  $form['help'] = array(
    '#value' => t("Select the webform(s) you would like to generated MySQL views for.")
  );

  $form['nodes'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;


}

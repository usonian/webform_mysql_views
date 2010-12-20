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

  //Get list of nids that already have views
  $views = variable_get('webform_mysql_views_views');
  $view_nids = array_keys($views);

  //Get list of all webform nodes
  $query = db_rewrite_sql("SELECT nid, title FROM {node} n WHERE type IN ('%s') AND status = 1 ORDER BY nid");
  $result = db_query($query, implode("','", $types));


  $options = array();
  $titles = array();
  $default_value = array();
  while ($node = db_fetch_object($result)) {
    $titles[$node->nid] = $node->title;
    if (in_array($node->nid, $view_nids)) {
      $default_value[$node->nid] = $node->nid;
    }
    if (in_array($node->nid, $view_nids)) {
      $view_str = $views[$node->nid];
    }
    else {
      $view_str = t('-');
    }
    $options[$node->nid] = array('title' => $node->title, 'view' => $view_str);

  }

  $form = array();

  //Pass the titles along
  $form['titles'] = array(
    '#type' => 'value',
    '#value' => $titles,
  );
  $header = array(
    'title' => t('Webform Name'),
    'view' => t('View Name'),
  );
  $form['help'] = array(
    '#value' => t("Select the webform(s) you would like to generate MySQL views for.")
  );
  $form['nodes'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $default_value,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;

}

/**
 * Submit handler for the admin form.
 */
function webform_mysql_views_admin_form_submit($form, &$form_state) {

  //Load existing view info
  $webform_views = variable_get('webform_mysql_views_views', array());
  $webform_view_nids = array_keys($webform_views);

  //Parse submitted settings;
  $submitted_nids = array();
  foreach ($form_state['values']['nodes'] as $key => $val) {
    if ($key == $val) {
      $submitted_nids[] = $key;
    }
  }

  //If there are any nids in the saved array not in the newly submitted one, drop those views.
  $drop_nids = array_diff($webform_view_nids, $submitted_nids);
  foreach ($drop_nids as $nid) {
    $query = "DROP VIEW %s";
    db_query($query, $webform_views[$nid]);
    unset($webform_views[$nid]);
  }
  if (count($drop_nids)) {
    drupal_set_message(format_plural(count($drop_nids),
      'Dropped 1 view.',
      'Dropped @count views.',
      array('@count' => count($drop_nids))));
  }

  //If there are any nids in the new array not in the saved one, create new views
  $new_nids = array_diff($submitted_nids, $webform_view_nids);

  foreach ($new_nids as $nid) {
    $view_name = webform_mysql_views_get_view_name($form_state['values']['titles'][$nid], $nid);
    $query = webform_mysql_views_build_query($nid, $view_name);
    db_query($query);

    //Add it to the webform_views array.
    $webform_views[$nid] = $view_name;
  }

  if (count($new_nids)) {
    drupal_set_message(format_plural(count($new_nids),
      'Created 1 view.',
      'Created @count views.',
      array('@count' => count($drop_nids))));
  }

  //Save the webform_views variable
  variable_set('webform_mysql_views_views', $webform_views);

}
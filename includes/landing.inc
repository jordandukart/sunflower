<?php

function sunflower_landing_form($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');
  module_load_include('inc', 'sunflower', 'includes/utilities');

  if (sunflower_is_valid_submitter() === FALSE) {
    drupal_set_message(t('Sorry you do not appear to be eligible to submit a proposal! Proposal submission is only available for paid members of the CSSR.'), 'warning');

  }
  else {
    global $user;
    $admin = FALSE;
    $rows = array();

    if (user_access('administer sunflower')) {
      $admin = TRUE;
      $results = sunflower_retrieve_proposals($user->uid, TRUE);
    }
    else {
      $results = sunflower_retrieve_proposals($user->uid);
    }
    foreach ($results as $row) {
      if (!$admin) {
        $rows[] = array(
          ucwords($row['type']),
          $row['last_modified'],
          t('<a href="@url">summary</a>', array('@url' => url('sunflower/proposal/summary/' . $row['proposal_id']))),
          (sunflower_is_submission_open()) ?  t('<a href="@url">edit</a>', array('@url' => url('sunflower/proposal/edit/' . $row['proposal_id']))) : '',
        );
      }
      else {
        $submitter_user = user_load($row['submitter_uid']);
        $rows[] = array(
          ucwords($row['type']),
          $submitter_user->name,
          $row['last_modified'],
          t('<a href="@url">summary</a>', array('@url' => url('sunflower/proposal/summary/' . $row['proposal_id']))),
          t('<a href="@url">edit</a>', array('@url' => url('sunflower/proposal/edit/' . $row['proposal_id'])))
        );
      }
    }
    $header = array();
    $header[] = array('data' => t('Proposal Type'));
    if ($admin) {
      $header[] = array('data' => t('Submitter'));
    }
    $header[] = array('data' => t('Last modified date'));
    $header[] = array('data' => t('Operations'), 'colspan' => '2');

    $form['proposal_table'] = array(
      '#markup' => theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'empty' => t('No proposals currently submitted!'),
      )),
    );

    if (sunflower_is_submission_open() === FALSE) {
      $opening_date = variable_get('sunflower_opening_date', '');
      $ending_date = variable_get('sunflower_ending_date', '');
      if (!is_array($opening_date) || !is_array($ending_date)) {
        if (!user_access('administer sunflower')) {
          drupal_set_message(t('Sorry submissions are not currently open!'), 'error');
        }
      }
      else {
        $current_time = strtotime(date('Y-m-d'));
        $opening_time = sunflower_date_array_to_time($opening_date);
        $ending_time = sunflower_date_array_to_time($ending_date);
        $opening_string = $opening_date['year'] . '-' . $opening_date['month'] . '-' . $opening_date['day'];
        $ending_string = $ending_date['year'] . '-' . $ending_date['month'] . '-' . $ending_date['day'];
        if ($current_time < $opening_time) {
          if (!user_access('administer sunflower')) {
            drupal_set_message(t('Sorry submissions are set to open on @date!', array('@date' => $opening_string)), 'error');
          }
        }
        elseif ($current_time > $ending_time) {
          if (!user_access('administer sunflower')) {
            drupal_set_message(t('Sorry submissions closed on @date!', array('@date' => $ending_string)), 'error');
          }
        }
      }
    }
    else {
      if (user_access('administer sunflower') || (!user_access('adminisiter sunflower') && count($rows) == 0)) {
        $form['proposal_submit'] = array(
          '#name' => 'sunflower_proposal_submit',
          '#type' => 'submit',
          '#value' => t('Submit a new proposal'),
        );
        $form['proposal_csv_export'] = array(
          '#type' => 'submit',
          '#value' => t('Export proposals'),
        );
      }
    }
  }
  return $form;
}

function sunflower_landing_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#name'] == 'sunflower_proposal_submit') {
    $form_state['redirect'] = 'sunflower/proposal/create';
  }
  else {
    sunflower_export_as_csv();
  }
}

function sunflower_export_as_csv() {
  global $user;
  module_load_include('inc', 'sunflower', 'includes/db');
  $db_data = sunflower_retrieve_proposals($user, TRUE);
  $csv_array = array();
  foreach ($db_data as $proposal) {
    $individual_proposal = sunflower_retrieve_data($proposal['proposal_id']);
    dsm($individual_proposal);
    $submitter_user = user_load($individual_proposal['proposal']['submitter_uid']);
    $submitter = $submitter_user->name;
    $csv_array[] = array(
      $individual_proposal['proposal']['proposal_id'],
      $individual_proposal['proposal']['type'],
      $submitter,
      $individual_proposal['proposal']['panel_roundtable_workshop_title'],
      $individual_proposal['proposal']['presider_chair'],
      $individual_proposal['presenters'][1]['presenter_name'],
      $individual_proposal['presenters'][1]['paper_title'],
      $individual_proposal['presenters'][1]['abstract'],
      isset($individual_proposal['presenters'][2]) ? $individual_proposal['presenters'][2]['presenter_name'] : '',
      isset($individual_proposal['presenters'][2]) ? $individual_proposal['presenters'][2]['paper_title'] : '',
      isset($individual_proposal['presenters'][2]) ? $individual_proposal['presenters'][2]['abstract'] : '',
      isset($individual_proposal['presenters'][3]) ? $individual_proposal['presenters'][3]['presenter_name'] : '',
      isset($individual_proposal['presenters'][3]) ? $individual_proposal['presenters'][3]['paper_title'] : '',
      isset($individual_proposal['presenters'][3]) ? $individual_proposal['presenters'][3]['abstract'] : '',
      isset($individual_proposal['presenters'][4]) ? $individual_proposal['presenters'][4]['presenter_name'] : '',
      isset($individual_proposal['presenters'][4]) ? $individual_proposal['presenters'][4]['paper_title'] : '',
      isset($individual_proposal['presenters'][4]) ? $individual_proposal['presenters'][4]['abstract'] : '',
      isset($individual_proposal['presenters'][5]) ? $individual_proposal['presenters'][5]['presenter_name'] : '',
      isset($individual_proposal['presenters'][5]) ? $individual_proposal['presenters'][5]['paper_title'] : '',
      isset($individual_proposal['presenters'][5]) ? $individual_proposal['presenters'][5]['abstract'] : '',
      isset($individual_proposal['presenters'][6]) ? $individual_proposal['presenters'][6]['presenter_name'] : '',
      isset($individual_proposal['presenters'][6]) ? $individual_proposal['presenters'][6]['paper_title'] : '',
      isset($individual_proposal['presenters'][6]) ? $individual_proposal['presenters'][6]['abstract'] : '',
      isset($individual_proposal['presenters'][7]) ? $individual_proposal['presenters'][7]['presenter_name'] : '',
      isset($individual_proposal['presenters'][7]) ? $individual_proposal['presenters'][7]['paper_title'] : '',
      isset($individual_proposal['presenters'][7]) ? $individual_proposal['presenters'][7]['abstract'] : '',
      isset($individual_proposal['presenters'][8]) ? $individual_proposal['presenters'][8]['presenter_name'] : '',
      isset($individual_proposal['presenters'][8]) ? $individual_proposal['presenters'][8]['paper_title'] : '',
      isset($individual_proposal['presenters'][8]) ? $individual_proposal['presenters'][8]['abstract'] : '',
      $individual_proposal['proposal']['special_requests'],
      $individual_proposal['proposal']['av_requests'],
    );
  }

  // Send response headers to the browser.
  drupal_add_http_header('Content-Type', 'text/csv');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=sunflower_proposal_export.csv');

  $fp = fopen('php://output', 'w');
   fputcsv($fp, array(
    t('ID'),
    t('Type'),
    t('Submitted by'),
    t('Panel, Roundtable, Workshop Title'),
    t('Presider'),
    t('Presenter 1 Name'),
    t('Presenter 1 Paper Title'),
    t('Presenter 1 Abstract'),
    t('Presenter 2 Name'),
    t('Presenter 2 Paper Title'),
    t('Presenter 2 Abstract'),
    t('Presenter 3 Name'),
    t('Presenter 3 Paper Title'),
    t('Presenter 3 Abstract'),
    t('Presenter 4 Name'),
    t('Presenter 4 Paper Title'),
    t('Presenter 4 Abstract'),
    t('Presenter 5 Name'),
    t('Presenter 5 Paper Title'),
    t('Presenter 5 Abstract'),
    t('Presenter 6 Name'),
    t('Presenter 6 Paper Title'),
    t('Presenter 6 Abstract'),
    t('Presenter 7 Name'),
    t('Presenter 7 Paper Title'),
    t('Presenter 7 Abstract'),
    t('Presenter 8 Name'),
    t('Presenter 8 Paper Title'),
    t('Presenter 8 Abstract'),
    t('Special Requests'),
    t('AV Requests'),
  ));
  foreach ($csv_array as $line) {
    fputcsv($fp, $line);
  }
  fclose($fp);
  drupal_exit();
}



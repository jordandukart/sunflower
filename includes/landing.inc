<?php

function sunflower_landing_form($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');
  module_load_include('inc', 'sunflower', 'includes/utilities');

  if (sunflower_is_valid_submitter() === FALSE) {
    drupal_set_message(t('Sorry you do not appear to be eligible to submit a proposal! Proposal submission is only available for paid members or friends of the CSSR.'), 'warning');

  }
  else {
    global $user;
    $admin = FALSE;
    $rows = array();

    if (user_access('administer sunflower')) {
      $admin = TRUE;
      $results = sunflower_retrieve_proposals($user, TRUE);
    }
    else {
      $results = sunflower_retrieve_proposals($user);
    }

    foreach ($results as $row) {
      if (!$admin) {
        $rows[] = array(
          l(t('@description', array('@description' => $row['description'])), 'sunflower/proposal/edit/' . $row['proposal_id']),
          $row['last_modified'],
        );
      }
      else {
        $submitter_user = user_load($row['submitter_uid']);
        $rows[] = array(
          l(t('@description', array('@description' => $row['description'])), 'sunflower/proposal/edit/' . $row['proposal_id']),
          $submitter_user->name,
          $row['last_modified'],
        );
      }
    }
    if ($admin) {
      $header = array(
        t('Proposal'),
        t('Submitter'),
        t('Last modified date'),
      );
    }
    else {
      $header = array(
        t('Proposal'),
        t('Last modified date'),
      );
    }
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
        drupal_set_message(t('Sorry submissions are not currently open!'), 'error');
      }
      else {
        $current_time = strtotime(date('Y-m-d'));
        $opening_time = sunflower_date_array_to_time($opening_date);
        $ending_time = sunflower_date_array_to_time($ending_date);
        $opening_string = $opening_date['year'] . '-' . $opening_date['month'] . '-' . $opening_date['day'];
        $ending_string = $ending_date['year'] . '-' . $ending_date['month'] . '-' . $ending_date['day'];
        if ($current_time < $opening_time) {
          drupal_set_message(t('Sorry submissions are set to open on @date!', array('@date' => $opening_string)), 'error');
        }
        elseif ($current_time > $ending_time) {
          drupal_set_message(t('Sorry submissions closed on @date!', array('@date' => $ending_string)), 'error');
        }
      }
    }
    else {
      $form['proposal_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit a new proposal'),
      );
    }
  }
  return $form;
}

function sunflower_landing_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'sunflower/proposal/create';
}



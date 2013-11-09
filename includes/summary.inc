<?php

/**
 * @file
 * Creates and renders the summary form for viewing submissions in Sunflower.
 */

/**
 * The summary form for viewing submissions in the Sunflower module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param string $proposal_id
 *   The ID of the proposal we want a summary of.
 *
 * @return array
 *   An array representing the proposal form.
 */
function sunflower_proposal_summary_form($form, &$form_state, $proposal_id) {
  module_load_include('inc', 'sunflower', 'includes/db');
  $db_data = sunflower_retrieve_data($proposal_id);
  $form = array();
  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proposal details'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['fieldset']['type'] = array(
    '#type' => 'item',
    '#title' => t('Type'),
    '#markup' => t('@type', array('@type' => ucwords($db_data['proposal']['type']))),
  );
  if (!empty($db_data['proposal']['panel_roundtable_workshop_title'])) {
    $form['fieldset']['panel_roundtable_workshop_title'] = array(
      '#type' => 'item',
      '#title' => t('Title'),
      '#markup' => t('@title', array('@title' => $db_data['proposal']['panel_roundtable_workshop_title'])),
    );
  }
  if (!empty($db_data['proposal']['panel_roundtable_workshop_abstract'])) {
    $form['fieldset']['panel_roundtable_workshop_abstract'] = array(
      '#type' => 'item',
      '#title' => t('Abstract'),
      '#markup' => t('@abstract', array('@abstract' => $db_data['proposal']['panel_roundtable_workshop_abstract'])),
    );
  }
  if (!empty($db_data['proposal']['presider_chair'])) {
    $form['fieldset']['presider_chair'] = array(
      '#type' => 'item',
      '#title' => t('Presider'),
      '#markup' => t('@presider', array('@presider' => $db_data['proposal']['presider_chair'])),
    );
  }
  if (!empty($db_data['proposal']['presider_email'])) {
    $form['fieldset']['presider_email'] = array(
      '#type' => 'item',
      '#title' => t('Presider E-mail'),
      '#markup' => t('@email', array('@email' => $db_data['proposal']['presider_email'])),
    );
  }
  if (!empty($db_data['proposal']['presider_institution'])) {
    $form['fieldset']['presider_institution'] = array(
      '#type' => 'item',
      '#title' => t('Presider Institutional Affiliation'),
      '#markup' => t('@institutional_affiliation', array('@institutional_affiliation' => $db_data['proposal']['presider_institution'])),
    );
  }
  if (!empty($db_data['proposal']['category_1'])) {
    $form['categories'] = array(
      '#type' => 'fieldset',
      '#title' => t('Categories'),
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
      '#weight' => '19',
    );
    $form['categories']['category_1'] = array(
      '#type' => 'item',
      '#title' => t('Category 1'),
      '#markup' => t('@category', array('@category' => $db_data['proposal']['category_1'])),
    );
  }
  if (!empty($db_data['proposal']['category_2'])) {
    $form['categories']['category_2'] = array(
      '#type' => 'item',
      '#title' => t('Category 2'),
      '#markup' => t('@category', array('@category' => $db_data['proposal']['category_2'])),
    );
  }
  if (!empty($db_data['proposal']['category_3'])) {
    $form['categories']['category_3'] = array(
      '#type' => 'item',
      '#title' => t('Category 3'),
      '#markup' => t('@category', array('@category' => $db_data['proposal']['category_3'])),
    );
  }
  if (!empty($db_data['proposal']['special_requests']) || !empty($db_data['proposal']['av_requests']) || !empty($db_data['proposal']['respondent_name_institutional_affiliation'])) {
    $form['requests'] = array(
      '#type' => 'fieldset',
      '#title' => t('Requests'),
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
      '#weight' => 123,
    );
  }
  if (!empty($db_data['proposal']['special_requests'])) {
    $form['requests']['special_requests'] = array(
      '#type' => 'item',
      '#title' => t('Special Requests'),
      '#markup' => t('@special', array('@special' => $db_data['proposal']['special_requests'])),
    );
  }
  if (!empty($db_data['proposal']['av_requests'])) {
    $form['requests']['av_requests'] = array(
      '#type' => 'item',
      '#title' => t('AV Requests'),
      '#markup' => t('@av', array('@av' => $db_data['proposal']['av_requests'])),
    );
  }
  if (!empty($db_data['proposal']['respondent_name_institutional_affiliation'])) {
    $form['requests']['respondent_name_institutional_affiliation'] = array(
      '#type' => 'item',
      '#title' => t('Respondent Name and Institutional Affiliation'),
      '#markup' => t('@respondent_name_institutional_affiliation', array('@respondent_name_institutional_affiliation' => $db_data['proposal']['respondent_name_institutional_affiliation'])),
    );
  }
  foreach ($db_data['presenters'] as $num => $presenter) {
    if (!empty($presenter['presenter_name'])) {
      $form["presenter_$num"] = array(
        '#type' => 'fieldset',
        '#title' => t('Presenter @num', array('@num' => $num)),
        '#collapsed' => FALSE,
        '#collapsible' => FALSE,
      );
      $form["presenter_$num"]['name'] = array(
        '#type' => 'item',
        '#title' => t('Name'),
        '#markup' => t('@name', array('@name' => $presenter['presenter_name'])),
      );
      $form["presenter_$num"]['paper_title'] = array(
        '#type' => 'item',
        '#title' => t('Paper Title for Conference Program'),
        '#markup' => t('@title', array('@title' => $presenter['paper_title'])),
      );
      $form["presenter_$num"]['abstract'] = array(
        '#type' => 'item',
        '#title' => t('Abstract'),
        '#markup' => t('@abstract', array('@abstract' => $presenter['abstract'])),
      );
      $form["presenter_$num"]['institutional_affiliation'] = array(
        '#type' => 'item',
        '#title' => t('Institutional Affiliation'),
        '#markup' => t('@institutional_affiliation', array('@institutional_affiliation' => $presenter['institutional_affiliation'])),
      );
    }
  }
  return $form;
}

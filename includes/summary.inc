<?php

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
  if (!empty($db_data['proposal']['presider_chair'])) {
    $form['fieldset']['presider_chair'] = array(
      '#type' => 'item',
      '#title' => t('Presider'),
      '#markup' => t('@presider', array('@presider' => $db_data['proposal']['presider_chair'])),
    );
  }
  if (!empty($db_data['proposal']['special_requests']) || !empty($db_data['proposal']['av_requests'])) {
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
        '#title' => t('Paper title'),
        '#markup' => t('@title', array('@title' => $presenter['paper_title'])),
      );
      $form["presenter_$num"]['abstract'] = array(
        '#type' => 'item',
        '#title' => t('Abstract'),
        '#markup' => t('@abstract', array('@abstract' => $presenter['abstract'])),
      );
    }
  }
  return $form;
}
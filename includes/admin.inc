<?php
/**
 * @file
 * Houses the administration form for Sunflower.
 */

/**
 * Admin settings form.
 */
function sunflower_admin_form($form, &$form_state) {
  $form = array();
  $start_date = variable_get('sunflower_opening_date', '');
  $end_date = variable_get('sunflower_ending_date', '');
  $form['start_date'] = array(
    '#type' => 'date',
    '#title' => t('Start date'),
    '#description' => t('Opening date to accept proposals'),
    '#default_value' => $start_date,
  );
  $form['end_date'] = array(
    '#type' => 'date',
    '#title' => t('End date'),
    '#description' => t('Ending date to accept proposals'),
    '#default_value' => $end_date,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['delete_data'] = array(
    '#type' => 'submit',
    '#value' => t('Delete all data'),
    '#submit' => array('sunflower_admin_delete_all_data'),
  );
  return $form;
}

function sunflower_admin_form_validate($form, $form_state) {
  module_load_include('inc', 'sunflower', 'includes/utilities');
  $start_time = sunflower_date_array_to_time($form_state['values']['start_date']);
  $end_time = sunflower_date_array_to_time($form_state['values']['end_date']);

  if ($end_time <= $start_time) {
    form_set_error('end_date', t('The ending date must be after the starting date!'));
  }
}

function sunflower_admin_form_submit($form, $form_state) {
  variable_set('sunflower_opening_date', $form_state['values']['start_date']);
  variable_set('sunflower_ending_date', $form_state['values']['end_date']);
  drupal_set_message(t('The configuration has been saved.'));
}

function sunflower_admin_delete_all_data($form, &$form_state) {
  $form_state['redirect'] = 'admin/sunflower/delete';
}

/**
 * The delete confirm_form for the Sunflower module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 *
 * @return array
 *   An array representing the confirm form.
 */
function sunflower_admin_delete_confirm_form($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');
  return confirm_form($form,
    t('Are you sure you want to delete all proposal data?'),
    "admin/sunflower",
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for the Sunflower delete confirm_form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function sunflower_admin_delete_confirm_form_submit($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');
  sunflower_delete_all_proposals();
  $form_state['redirect'] = 'admin/sunflower';
  drupal_set_message(t('All proposal data has been deleted!'));
}
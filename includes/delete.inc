<?php
/**
 * @file
 * Houses the confirm_form for the Sunflower module.
 */

/**
 * The delete confirm_form for the Sunflower module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param int $proposal_id
 *   The ID of the proposal we are currently editing.
 *
 * @return array
 *   An array representing the confirm form.
 */
function sunflower_proposal_delete_confirm_form($form, &$form_state, $proposal_id) {
  module_load_include('inc', 'sunflower', 'includes/db');
  $form_state['proposal_id'] = $proposal_id;
  return confirm_form($form,
    t('Are you sure you want to delete this proposal?'),
    "sunflower/proposal/edit/$proposal_id",
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
function sunflower_proposal_delete_confirm_form_submit($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');
  sunflower_delete_proposal($form_state['proposal_id']);
  $form_state['redirect'] = 'sunflower';
  drupal_set_message(t('The proposal has been deleted!'));
}

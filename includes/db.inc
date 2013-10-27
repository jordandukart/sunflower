<?php

/**
 * @file
 * Database utility functions for Sunflower.
 */

/**
 * Retrieves proposals used on the landing page.
 *
 * @param string $user
 *   The UID of the person we are retrieving proposals for.
 * @param bool $all
 *   Whether we are displaying all proposals (used for admins).
 *
 * @return array
 *   An array containing the proposal results from the database.
 */
function sunflower_retrieve_proposals($user, $all = FALSE) {
  if (!$all) {
    return db_select('sunflower_proposals', 'p')
      ->fields('p', array('proposal_id', 'last_modified', 'type'))
      ->condition('submitter_uid', $user)
      ->orderBy('last_modified', 'DESC')
      ->execute()
      ->fetchAllAssoc('proposal_id', PDO::FETCH_ASSOC);
  }
  else {
    return db_select('sunflower_proposals', 'p')
      ->fields('p', array(
        'proposal_id',
        'last_modified',
        'submitter_uid',
        'type',
       ))
      ->orderBy('last_modified', 'DESC')
      ->execute()
      ->fetchAllAssoc('proposal_id', PDO::FETCH_ASSOC);
  }
}

/**
 * Returns the submitter of the specified proposal.
 *
 * @param string $proposal_id
 *   The ID of the proposal we want the submitter of.
 *
 * @return string
 *   The submitter UID for the proposal.
 */
function sunflower_retrieve_submitter_uid($proposal_id) {
  return db_select('sunflower_proposals', 'p')
    ->fields('p', array('submitter_uid'))
    ->condition('proposal_id', $proposal_id)
    ->execute()
    ->fetchField();
}

/**
 * Inserts data into the database on submission.
 *
 * @param array $proposal_data
 *   An array containing all the fields for the proposal table.
 * @param array $presenter_data
 *   An array containing all the fields for the presenter table.
 * @param string $proposal_id
 *   The proposal ID if the submission is being edited, empty for a new one.
 */
function sunflower_insert_data(array $proposal_data, array $presenter_data, $proposal_id) {
  if (!$proposal_id) {
    $proposal_id = db_insert('sunflower_proposals')
      ->fields($proposal_data)
      ->execute();
    foreach ($presenter_data as $presenter) {
      $presenter['proposal_id'] = $proposal_id;
      db_insert('sunflower_presenters')
        ->fields($presenter)
        ->execute();
    }
  }
  else {
    db_update('sunflower_proposals')
      ->condition('proposal_id', $proposal_id)
      ->fields($proposal_data)
      ->execute();
    // To avoid a headache first delete all existing presenters before inserting
    // the new ones.
    db_delete('sunflower_presenters')
      ->condition('proposal_id', $proposal_id)
      ->execute();
    foreach ($presenter_data as $presenter) {
      $presenter['proposal_id'] = $proposal_id;
      db_insert('sunflower_presenters')
        ->fields($presenter)
        ->execute();
    }
  }
}

/**
 * Retrieves all database data for one proposal.
 *
 * @param string $proposal_id
 *   The proposal ID we want data about.
 *
 * @return array
 *   An array containing all the database values for the proposal.
 */
function sunflower_retrieve_data($proposal_id) {
  $data = array();
  $data['presenters'] = db_select('sunflower_presenters', 'p')
    ->fields('p')
    ->condition('proposal_id', $proposal_id)
    ->execute()
    ->fetchAllAssoc('presenter_id', PDO::FETCH_ASSOC);
  $data['proposal'] = db_select('sunflower_proposals', 'p')
    ->fields('p')
    ->condition('proposal_id', $proposal_id)
    ->execute()
    ->fetchAssoc();
  return $data;
}

/**
 * Deletes an individual proposal from the database.
 *
 * @param string $proposal_id
 *   The proposal ID which we are deleting.
 */
function sunflower_delete_proposal($proposal_id) {
  db_delete('sunflower_proposals')
    ->condition('proposal_id', $proposal_id)
    ->execute();
  db_delete('sunflower_presenters')
    ->condition('proposal_id', $proposal_id)
    ->execute();
}

/**
 * Deletes all proposals out of the database.
 */
function sunflower_delete_all_proposals() {
  db_delete('sunflower_proposals')
    ->execute();
  db_delete('sunflower_presenters')
    ->execute();
}

/**
 * Retrieves all valid paid members for autocomplete purposes.
 *
 * @return array
 *   An array containing all valid members.
 */
function sunflower_retrieve_all_paid_members() {
  $query = db_select('field_data_field_membership_type', 'f');
  $query->join('field_data_field_first_name', 'n', 'n.entity_id = f.entity_id');
  $query->join('field_data_field_last_name', 'l', 'l.entity_id = f.entity_id');
  $query->fields('n', array('field_first_name_value'))
    ->fields('l', array('field_last_name_value'))
    ->fields('f', array('entity_id'))
    ->condition('field_membership_type_value', 'friend', '!=')
    ->orderBy('l.field_last_name_value', 'ASC');
  $results = $query->execute()->fetchAllAssoc('entity_id', PDO::FETCH_ASSOC);
  return $results;
}

/**
 * Retrieves all users for autocomplete purposes.
 *
 * @return array
 *   An array containing all users members.
 */
function sunflower_retrieve_all_users() {
  $query = db_select('field_data_field_first_name', 'n');
  $query->join('field_data_field_last_name', 'l', 'l.entity_id = n.entity_id');
  $query->fields('n', array('field_first_name_value', 'entity_id'))
    ->fields('l', array('field_last_name_value'))
    ->orderBy('l.field_last_name_value', 'ASC');
  $results = $query->execute()->fetchAllAssoc('entity_id', PDO::FETCH_ASSOC);
  return $results;
}

/**
 * Determines whether a given member is valid to be used in a proposal or not.
 *
 * @param string $full_name
 *   The full name of the presenter we are validating.
 *
 * @return bool
 *   TRUE if they can be added to a proposal, FALSE otherwise.
 */
function sunflower_validate_presenter_paid_and_exists($full_name) {
  $query = db_select('field_data_field_membership_type', 'f');
  $query->join('field_data_field_first_name', 'n', 'n.entity_id = f.entity_id');
  $query->join('field_data_field_last_name', 'l', 'l.entity_id = n.entity_id');
  $query->fields('n', array('field_first_name_value', 'entity_id'))
    ->fields('l', array('field_last_name_value'))
    ->condition('field_membership_type_value', 'friend', '!=');
  $results = $query->execute()->fetchAllAssoc('entity_id', PDO::FETCH_ASSOC);
  $valid_presenter = FALSE;
  foreach ($results as $result) {
    $full_db_string = ucwords($result['field_first_name_value'] . ' ' . $result['field_last_name_value']);
    if ($full_db_string === $full_name) {
      $valid_presenter = TRUE;
      break;
    }
  }
  return $valid_presenter;
}

/**
 * Determines whether a given member is valid to be used in a proposal or not.
 *
 * @param string $full_name
 *   The full name of the presenter we are validating.
 *
 * @return bool
 *   TRUE if they can be added to a proposal, FALSE otherwise.
 */
function sunflower_validate_presenter_exists($full_name) {
  $results = sunflower_retrieve_all_users();
  $valid_presenter = FALSE;
  foreach ($results as $result) {
    $full_db_string = ucwords($result['field_first_name_value'] . ' ' . $result['field_last_name_value']);
    if ($full_db_string === $full_name) {
      $valid_presenter = TRUE;
      break;
    }
  }
  return $valid_presenter;
}

/**
 * Takes a full name and grabs the entity_id for it.
 *
 * @param string $full_name
 *   The full name of the presenter we want to get the entity_id for.
 *
 * @return int
 *   The entity_id of the presenter.
 */
function sunflower_retrieve_entity_id_from_full_name($full_name) {
  $query = db_select('field_data_field_first_name', 'n');
  $query->join('field_data_field_last_name', 'l', 'l.entity_id = n.entity_id');
  $query->fields('n', array('field_first_name_value', 'entity_id'))
    ->fields('l', array('field_last_name_value'));
  $results = $query->execute()->fetchAllAssoc('entity_id', PDO::FETCH_ASSOC);
  $entity_id = FALSE;
  foreach ($results as $result) {
    $full_db_string = ucwords($result['field_first_name_value'] . ' ' . $result['field_last_name_value']);
    if ($full_db_string === $full_name) {
      $entity_id = $result['entity_id'];
      break;
    }
  }
  return (int) $entity_id;
}

/**
 * Validates that a presenter based upon rules given by CSSR.
 *
 * A presenter can only be in two total submissions. They also can not exist in
 * a panel and an individual proposal concurrently.
 *
 * @param string $full_name
 *   The full name of the presenter we are validating.
 * @param string $type
 *   The type of proposal which is currently trying to be submitted.
 * @param NULL|string $proposal_id
 *   What proposal ID this is being validated from. Occurs when editing to
 *   handle an edge case described below.
 *
 * @return array|bool
 *   FALSE if the user does not exist in the database, an array describing the
 *   outcome otherwise:
 *     -valid: Whether the presenter is valid for additional submissions,
 *     -type (Optional): The reason as to why the presenter is not valid.
 */
function sunflower_validate_proposal_amount($full_name, $type, $proposal_id = NULL) {
  $entity_id = sunflower_retrieve_entity_id_from_full_name($full_name);
  if ($entity_id) {
    $query = db_select('sunflower_presenters', 's');
    $query->join('sunflower_proposals', 'p', 's.proposal_id = p.proposal_id');
    $query->fields('p', array('type', 'proposal_id'));
    $query->condition('entity_id', $entity_id, '=');
    if ($proposal_id) {
      $query->condition('type', $type, '!=');
    }
    $results = $query->execute()->fetchAllAssoc('proposal_id', PDO::FETCH_ASSOC);;

    $result_types = array();
    $presenter_proposal_amount = count($results);
    foreach ($results as $result) {
      $result_types[] = $result['type'];
    }
    // Need to check if the proposal_id is set such that if a user is editing
    // a submission we do not flag an existing user as being invalid.
    if ($proposal_id) {
      foreach ($results as $result) {
        if ($result['proposal_id'] == $proposal_id) {
          $presenter_proposal_amount--;
          break;
        }
      }
    }
    if ($presenter_proposal_amount == 2) {
      return array(
        'valid' => FALSE,
        'type' => 'two',
      );
    }
    elseif ($type == 'individual' || $type == 'panel') {
      if (array_intersect(array('individual', 'panel'), $result_types)) {
        if (in_array($type, $result_types)) {
          return array(
            'valid' => FALSE,
            'type' => 'same',
          );
        }
        else {
          return array(
            'valid' => FALSE,
            'type' => 'individual_panel',
          );
        }
      }
      else {
        return array(
          'valid' => TRUE,
        );
      }
    }
  }
  return FALSE;
}

/**
 * Retrieves all members currently in proposals and their membership status.
 *
 * @return array
 *   Returns an array of members containing:
 *     -entity_id: The entity_id of the member.
 *     -presenter_name: The name of the presenter.
 *     -proposal_ids: The IDs of the proposals that the member exists in.
 */
function sunflower_retrieve_presenters_membership_status() {
  $paid_enforcement = variable_get('sunflower_enforce_paid_memberships', FALSE);

  $query = db_select('sunflower_presenters', 's');
  $query->join('field_data_field_membership_type', 'f', 's.entity_id = f.entity_id');
  $query->fields('s', array('presenter_name', 'entity_id'))
    ->fields('f', array('field_membership_type_value'));
  $results = $query->execute()->fetchAllAssoc('entity_id', PDO::FETCH_ASSOC);

  // If we aren't enforcing paid memberships there could be presenters that do
  // not exist in the field_data_field_membership_type table because they do
  // not have a value. So let's force them into our results.
  if (!$paid_enforcement) {
    $presenter_query = db_select('sunflower_presenters', 's');
    $db_and = db_and();
    foreach (array_keys($results) as $entity_id) {
      $db_and->condition('entity_id', $entity_id, '!=');
    }
    // We have some NULL values in the database depending on the amount of
    // presenters per proposals so we don't want to return these.
    $db_and->condition('entity_id', '0', '!=');
    $presenter_query->fields('s', array('presenter_name', 'entity_id'))
      ->condition($db_and);
    $presenter_results = $presenter_query->execute()->fetchAllAssoc('entity_id', PDO::FETCH_ASSOC);
    foreach ($presenter_results as $presenter) {
      $results[$presenter['entity_id']] = $presenter;
      $results[$presenter['entity_id']]['field_membership_type_value'] = t('N/A');
    }
  }

  foreach ($results as &$result) {
    $proposal_query = db_select('sunflower_presenters', 's');
    $proposal_query->fields('s', array('proposal_id'))
      ->condition('entity_id', $result['entity_id'], '=');
    $proposal_results = $proposal_query->execute()->fetchCol();
    $result['proposal_ids'] = implode(', ', $proposal_results);
  }
  return $results;
}

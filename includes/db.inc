<?php


/**
 * @file
 * Database utility functions for Sunflower.
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
      ->fields('p', array('proposal_id', 'last_modified', 'submitter_uid', 'type'))
      ->orderBy('last_modified', 'DESC')
      ->execute()
      ->fetchAllAssoc('proposal_id', PDO::FETCH_ASSOC);
  }
}

function sunflower_retrieve_submitter_uid($proposal_id) {
  return db_select('sunflower_proposals', 'p')
    ->fields('p', array('submitter_uid'))
    ->condition('proposal_id', $proposal_id)
    ->execute()
    ->fetchField();
}

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

function sunflower_delete_proposal($proposal_id) {
  db_delete('sunflower_proposals')
    ->condition('proposal_id', $proposal_id)
    ->execute();
  db_delete('sunflower_presenters')
    ->condition('proposal_id', $proposal_id)
    ->execute();
}

function sunflower_delete_all_proposals() {
  db_delete('sunflower_proposals')
    ->execute();
  db_delete('sunflower_presenters')
    ->execute();
}

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

function sunflower_validate_presenter_paid_and_exists($full_name) {
  $query = db_select('field_data_field_first_name', 'n');
  $query->join('field_data_field_last_name', 'l', 'l.entity_id = n.entity_id');
  $query->fields('n', array('field_first_name_value', 'entity_id'))
    ->fields('l', array('field_last_name_value'));
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
function sunflower_validate_proposal_amount($full_name, $type) {
  $entity_id = sunflower_retrieve_entity_id_from_full_name($full_name);
  if ($entity_id) {
    $query = db_select('sunflower_presenters', 's');
    $query->join('sunflower_proposals', 'p', 's.proposal_id = p.proposal_id');
    $query->fields('p', array('type'));
    $query->condition('entity_id', $entity_id, '=');
    $results = $query->execute()->fetchCol();
    if (count($results) == 2) {
      return array(
        'valid' => FALSE,
        'type' => 'two',
      );
    }
    elseif ($type == 'individual' || $type == 'panel') {
      if (array_intersect(array('individual', 'panel'), $results)) {
        if (in_array($type, $results)) {
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
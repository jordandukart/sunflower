<?php

function sunflower_is_valid_submitter() {
  global $user;
  $user_type = db_select('field_data_field_membership_type', 'f')
    ->fields('f', array('field_membership_type_value'))
    ->condition('f.entity_id', $user->uid, '=')
    ->execute()
    ->fetchCol();
  if (count($user_type) && $user_type[0] != 'friend') {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

function sunflower_is_submission_open() {
  $current_date_string = date('Y-m-d');
  $current_time = strtotime($current_date_string);
  $opening_date = variable_get('sunflower_opening_date', '');
  $ending_date = variable_get('sunflower_ending_date', '');
  if (!is_array($opening_date) || !is_array($ending_date)) {
    return FALSE;
  }
  else {
    $opening_time = sunflower_date_array_to_time($opening_date);
    $ending_time = sunflower_date_array_to_time($ending_date);

    if ($current_time >= $opening_time && $current_time <= $ending_time) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
}

function sunflower_date_array_to_time(array $date) {
  $date_string = $date['year'] . '-' . $date['month'] . '-' . $date['day'];
  return strtotime($date_string);
}
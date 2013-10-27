<?php

/**
 * @file
 * Holds general utility functions used in Sunflower.
 */

/**
 * Determines whether the current user is a valid submitter within Sunflower.
 *
 * @return bool
 *   TRUE if the user can submit, FALSE otherwise.
 */
function sunflower_is_valid_submitter() {
  global $user;
  $paid_enforcement = variable_get('sunflower_enforce_paid_memberships', FALSE);
  if ($paid_enforcement) {
    $user_type = db_select('field_data_field_membership_type', 'f')
      ->fields('f', array('field_membership_type_value'))
      ->condition('f.entity_id', $user->uid, '=')
      ->execute()
      ->fetchCol();
    // The user exists in some facet in the database and is a paid member.
    if (count($user_type) && $user_type[0] != 'friend') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  // We aren't enforcing any sort of paid restrictions, any authenticated user
  // can submit proposals.
  else {
    return user_is_logged_in();
  }
}

/**
 * Determines whether submissions to Sunflower are currently open.
 *
 * @return bool
 *   TRUE if Sunflower is accepted submissions, FALSE otherwise.
 */
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

/**
 * Utility function to take a date array and convert to a timestamp.
 *
 * @param array $date
 *   An array in Drupal's date form that contains:
 *     -year: The year.
 *     -month: The month.
 *     -day: The day.
 *
 * @return int
 *   A timestamp used for date comparisons.
 */
function sunflower_date_array_to_time(array $date) {
  $date_string = $date['year'] . '-' . $date['month'] . '-' . $date['day'];
  return strtotime($date_string);
}

<?php
/**
 * @file
 * Autocomplete functionality for Sunflower.
 */

/**
 * Callback that performs autocomplete operations.
 */
function sunflower_presenter_autocomplete($user_string) {
  module_load_include('inc', 'sunflower', 'includes/db');
  $all_paid = sunflower_retrieve_all_paid_members();
  $output = array();
  foreach ($all_paid as $value) {
    $full_name = ucwords($value['field_first_name_value'] . ' ' . $value['field_last_name_value']);
    if ($user_string != '*') {
      if (strpos(drupal_strtoupper($full_name), drupal_strtoupper($user_string)) !== FALSE) {
        $output[$full_name] = $full_name;
      }
    }
    else {
      $output[$full_name] = $full_name;
    }
  }
  drupal_json_output($output);
}

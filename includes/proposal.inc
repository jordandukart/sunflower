<?php

/**
 * @file
 * Creates and handles the proposal form used for submissions in Sunflower.
 */

/**
 * The proposal form for the Sunflower module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 * @param bool $edit_submission
 *   Whether the proposal is being edited, or if it's a new submission.
 *
 * @return array
 *   An array representing the proposal form.
 */
function sunflower_proposal_form($form, &$form_state, $edit_submission = NULL) {
  module_load_include('inc', 'sunflower', 'includes/utilities');
  module_load_include('inc', 'sunflower', 'includes/db');
  $form = array(
    '#tree' => TRUE,
  );
  $form['error_description'] = array(
    '#type' => 'item',
    '#description' => t('Most the fields in this form are required. Please fill them out
    completely and accurately to the best of your ability. When the form is submitted, any missing
    required fields will be highlighted with red and an error message directing you to the missing field will be displayed.
    If, for a given field, you are not sure what to fill in please type "N/A". Thank you.'),
  );
  $db_data = array();
  if ($edit_submission !== NULL) {
    $db_data = sunflower_retrieve_data($edit_submission);
  }

  $form['proposal_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proposal Type'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );
  $form['proposal_type']['proposal_choices'] = array(
    '#type' => 'radios',
    '#title' => t('Proposal type'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['type'] : 'individual',
    '#options' => array(
      'individual' => t('Individual Paper'),
      'panel' => t('Panel Session'),
      'roundtable' => t('Roundtable Session'),
      'workshop' => t('Workshop Session'),
    ),
  );
  $form['categories'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#weight' => 123,
    '#title' => t('Categories'),
    '#description' => t('Please identify a broad thematic group which your proposal might fall under. You can choose up to three category groups.
    Doing so will help our Program Co-Chairs organize individual papers into panel sessions. As you type in the Category field below,
    a list of predefined topics may appear. You can select from the predefined topics that will be displayed, and to see a list of all categories type in "*".
    Alternatively, you can type your own category in the box.'),
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('value' => 'individual'),
      ),
    ),
  );
  for ($i = 1; $i <= 3; $i++) {
    $enabled = FALSE;
    $category_default = NULL;
    if (isset($db_data['proposal'])) {
      if (!empty($db_data['proposal']["category_$i"])) {
        $enabled = TRUE;
        $category_default = $db_data['proposal']["category_$i"];
      }
    }
    $form['categories']['category_' . $i . '_checkbox'] = array(
      '#type' => 'checkbox',
      '#title' => t("Category @num", array('@num' => $i)),
      '#default_value' => $enabled,
      '#states' => array(
        'visible' => array(
          ':input[name="proposal_type[proposal_choices]"]' => array('value' => 'individual'),
        ),
      ),
    );
    $form['categories']['category_' . $i . '_textfield'] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#autocomplete_path' => 'sunflower/autocomplete/category',
      '#description' => t('Type "*" to list all categories'),
      '#default_value' => $category_default,
      '#states' => array(
        'visible' => array(
          ':input[name="categories[category_' . $i . '_checkbox]"]' => array('checked' => TRUE),
          ':input[name="proposal_type[proposal_choices]"]' => array('value' => 'individual'),
        ),
      ),
    );
  }
  $form['proposal_details'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#title' => t('Proposal Details'),
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
      ),
    ),
  );
  $form['proposal_details']['panel_roundtable_workshop_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Panel, Roundtable or Workshop Title for Conference Program'),
    '#description' => t('Maximum of 100 characters'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['panel_roundtable_workshop_title'] : '',
    '#maxlength' => 100,
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
      ),
    ),
  );
  $form['proposal_details']['panel_roundtable_workshop_abstract'] = array(
    '#type' => 'textarea',
    '#title' => t('Panel, Roundtable or Workshop Abstract for Conference Program'),
    '#description' => t('Maximum of 150 words'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['panel_roundtable_workshop_abstract'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
      ),
    ),
  );
  $form['proposal_details']['presider_chair'] = array(
    '#type' => 'textfield',
    '#title' => t('Full name of Presider'),
    '#description' => t('Who is presiding over this panel, roundtable or workshop?'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['presider_chair'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
      ),
    ),
  );
  $form['proposal_details']['presider_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail of Presider'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['presider_email'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
      ),
    ),
  );
  $form['proposal_details']['presider_institution'] = array(
    '#type' => 'textfield',
    '#title' => t('Institutional Affiliation'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['presider_institution'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
      ),
    ),
  );
  $form['requests'] = array(
    '#type' => 'fieldset',
    '#title' => t('Requests'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#weight' => 149,
  );
  $form['requests']['special_requests'] = array(
    '#type' => 'textarea',
    '#title' => t('Special Requests'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['special_requests'] : '',
  );
  $form['requests']['av_requests'] = array(
    '#type' => 'textarea',
    '#title' => t('AV Requests'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['av_requests'] : '',
  );
  $form['requests']['respondent_name_institutional_affiliation'] = array(
    '#type' => 'textarea',
    '#title' => t('Respondent Name and Institutional Affiliation'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['respondent_name_institutional_affiliation'] : '',
  );

  for ($i = 1; $i <= 8; $i++) {
    $default_presenter_value = FALSE;
    if (isset($db_data['presenters'])) {
      if (isset($db_data['presenters'][$i])) {
        $default_presenter_value = TRUE;
      }
    }
    $form['fieldset_' . $i] = array(
      '#type' => 'fieldset',
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
      '#title' => t('Presenter @num', array('@num' => $i)),
    );
    $form['fieldset_' . $i]['presenter_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of presenter'),
      '#autocomplete_path' => 'sunflower/autocomplete/presenter',
      '#description' => variable_get('sunflower_enforce_paid_memberships', FALSE) ? t('Type "*" to list all paid members') : t('Type "*" to list all members'),
      '#default_value' => $default_presenter_value ? $db_data['presenters'][$i]['presenter_name'] : FALSE,
    );
    $form['fieldset_' . $i]['paper_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Paper Title for Conference Program'),
      '#description' => t('Maximum of 100 characters'),
      '#maxlength' => 100,
      '#default_value' => $default_presenter_value ? $db_data['presenters'][$i]['paper_title'] : FALSE,
    );
    $form['fieldset_' . $i]['abstract'] = array(
      '#type' => 'textarea',
      '#title' => t('Abstract for Conference Program'),
      '#description' => t('Maximum of 150 words'),
      '#default_value' => $default_presenter_value ? $db_data['presenters'][$i]['abstract'] : FALSE,
    );
    $form['fieldset_' . $i]['institutional_affiliation'] = array(
      '#type' => 'textfield',
      '#title' => t('Institutional Affiliation'),
      '#default_value' => $default_presenter_value ? $db_data['presenters'][$i]['institutional_affiliation'] : FALSE,
    );
    if ($i > 2 && $i < 4) {
      $form['fieldset_' . $i]['#states'] = array(
        'visible' => array(
          ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
        ),
      );
    }
    if ($i == 4) {
      $form['fieldset_' . $i]['#states'] = array(
        'visible' => array(
          ':input[name="proposal_type[proposal_choices]"]' => array(
            array('value' => 'workshop'),
            array('value' => 'roundtable'),
          ),
        ),
      );
    }
    if ($i >= 5 && $i <= 8) {
      $form['fieldset_' . $i]['#states'] = array(
        'visible' => array(
          ':input[name="proposal_type[proposal_choices]"]' => array('value' => 'workshop'),
        ),
      );
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#name' => 'sunflower_proposal_submit',
    '#value' => empty($db_data) ? t('Submit') : t('Update'),
    '#weight' => 150,
  );
  if (!empty($db_data)) {
    $form['proposal_id'] = array(
      '#type' => 'value',
      '#value' => $db_data['proposal']['proposal_id'],
    );
    $form['submitter_uid'] = array(
      '#type' => 'value',
      '#value' => $db_data['proposal']['submitter_uid'],
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 151,
      '#submit' => array('sunflower_proposal_delete'),
    );
  }
  return $form;
}

/**
 * Submit handler to re-direct to a confirm form before nuking a proposal.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function sunflower_proposal_delete($form, &$form_state) {
  $proposal_id = $form_state['values']['proposal_id'];
  $form_state['redirect'] = "sunflower/proposal/delete/$proposal_id";
}

/**
 * Performs validation for the Sunflower proposal form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function sunflower_proposal_form_validate($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');
  if ($form_state['clicked_button']['#name'] == 'sunflower_proposal_submit') {
    $type = $form_state['values']['proposal_type']['proposal_choices'];
    $required_presenters = array(
      'individual' => 1,
      'roundtable' => 3,
      'panel' => 2,
      'workshop' => 5,
    );
    $max_presenters = array(
      'individual' => 2,
      'roundtable' => 4,
      'panel' => 3,
      'workshop' => 8,
    );
    $all_presenters = array();
    // Presider and workshop_panel_roundtable title required if not individual.
    if ($type != 'individual') {
      if (empty($form_state['values']['proposal_details']['panel_roundtable_workshop_title'])) {
        form_set_error('proposal_details][panel_roundtable_workshop_title', t('A title is required for the @type!', array('@type' => $type)));
      }
      if (empty($form_state['values']['proposal_details']['panel_roundtable_workshop_abstract'])) {
        form_set_error('proposal_details][panel_roundtable_workshop_abstract', t('An abstract is required for the @type!', array('@type' => $type)));
      }
      else {
        $number_of_words = str_word_count($form_state['values']['proposal_details']['panel_roundtable_workshop_abstract']);
        if ($number_of_words > 150) {
          form_set_error("proposal_details][panel_roundtable_workshop_abstract", t('The maximum number of words for the abstract of the @type is 150! You have entered @words words!', array(
            '@type' => $type,
            '@words' => $number_of_words,
          )));
        }
      }
      if (empty($form_state['values']['proposal_details']['presider_chair'])) {
        form_set_error('proposal_details][presider_chair', t('A name is required for the presider of the @type!', array('@type' => $type)));
      }
      if (empty($form_state['values']['proposal_details']['presider_email'])) {
        form_set_error('proposal_details][presider_email', t('An email is required for the presider of the @type!', array('@type' => $type)));
      }
      if (empty($form_state['values']['proposal_details']['presider_institution'])) {
        form_set_error('proposal_details][presider_institution', t('An institutional affiliation is required for the presider of the @type!', array('@type' => $type)));
      }
    }
    else {
      $category_values = array();
      // Category checking time.
      for ($i = 1; $i <= 3; $i++) {
        $enabled = $form_state['values']['categories']['category_' . $i . '_checkbox'];
        if ($i == 1 && !$enabled) {
          form_set_error('categories][category_1_checkbox', t('At least one category is required per proposal!'));
        }

        if ($enabled) {
          if (empty($form_state['values']['categories']['category_' . $i . '_textfield'])) {
            form_set_error('categories][category_' . $i . '_textfield', t('The category for category @num must not be empty!', array('@num' => $i)));
          }
          else {
            $value = $form_state['values']['categories']['category_' . $i . '_textfield'];
            $existing = array_search($value, $category_values);
            if ($existing) {
              form_set_error('categories][category_' . $i . '_textfield', t('The value %value already is entered in category @num!', array(
                '%value' => $value,
                '@num' => $existing,
              )));
            }
            else {
              $category_values[$i] = $value;
            }
          }
        }
      }
    }
    $readable_fields = array(
      'presenter_name' => t('presenter name'),
      'paper_title' => t('paper title'),
      'abstract' => t('abstract'),
      'institutional_affiliation' => t('institutional affiliation'),
    );
    $minimum_required = FALSE;
    // Logic to check minimum required fields.
    for ($i = 1; $i <= $required_presenters[$type]; $i++) {
      $error_count = 0;
      foreach ($form_state['values']["fieldset_$i"] as $field => $value) {
        if (empty($value)) {
          form_set_error("fieldset_$i][$field", t('The @field field must not be empty for presenter @num!', array(
            '@field' => $readable_fields[$field],
            '@num' => $i,
          )));
          $error_count++;
        }
        elseif ($field == 'presenter_name') {
          // Go make sure who they entered is in the database.
          $paid_enforcement = variable_get('sunflower_enforce_paid_memberships', FALSE);
          if ($paid_enforcement) {
            $valid = sunflower_validate_presenter_paid_and_exists($value);
          }
          else {
            $valid = sunflower_validate_presenter_exists($value);
          }
          if (!$valid) {
            form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter does not exist in our database @paid for presenter @num!', array(
            '%presenter' => $value,
            '@paid' => $paid_enforcement ? t('or is not a paid member') : '',
            '@num' => $i,
            )));
          }
          else {
            $proposal_id = isset($form_state['values']['proposal_id']) ? $form_state['values']['proposal_id'] : NULL;
            $validity = sunflower_validate_proposal_amount($value, $type, $proposal_id);
            if ($validity['valid'] === FALSE) {
              if ($validity['type'] == 'same') {
                form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter in presenter @num already exists in a @type proposal!', array(
                  '%presenter' => $value,
                  '@num' => $i,
                  '@type' => $type,
                )));
              }
              elseif ($validity['type'] == 'two') {
                form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter in presenter @num already exists in two proposals!', array(
                  '%presenter' => $value,
                  '@num' => $i,
                )));
              }
              elseif ($validity['type'] == 'individual_panel') {
                if ($type == 'individual') {
                  $restrict = 'panel';
                }
                else {
                  $restrict = 'individual';
                }
                form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter in presenter @num already exists in a @restrict proposal! Members may not be a part of a @type and a @restrict proposal at the same time!', array(
                  '%presenter' => $value,
                  '@num' => $i,
                  '@type' => $type,
                  '@restrict' => $restrict,
                )));
              }
            }
            else {
              $existing = array_search($value, $all_presenters);
              if ($existing) {
                form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter for presenter @pres has already been entered in presenter @num!', array(
                  '%presenter' => $value,
                  '@num' => $existing,
                  '@pres' => $i,
                )));
              }
              else {
                $all_presenters[$i] = $value;
              }
            }
          }
        }
        elseif ($field == 'abstract') {
          $number_of_words = str_word_count($value);
          if ($number_of_words > 150) {
            form_set_error("fieldset_$i][$field", t('The maximum number of words for the abstract of presenter @num is 150! You have entered @words words!', array(
              '@num' => $i,
              '@words' => $number_of_words,
            )));
          }
        }
      }
      if ($error_count == 3) {
        $minimum_required = TRUE;
      }
    }
    // For the non-required presenters make sure they aren't submitting half
    // filled out fields.
    for ($i > $required_presenters[$type]; $i <= $max_presenters[$type]; $i++) {
      $all_empty = TRUE;
      foreach ($form_state['values']["fieldset_$i"] as $field => $value) {
        if (!empty($value)) {
          $all_empty = FALSE;
        }
      }
      if (!$all_empty) {
        foreach ($form_state['values']["fieldset_$i"] as $field => $value) {
          if (empty($value)) {
            form_set_error("fieldset_$i][$field", t('The @field field must not be empty for presenter @num!', array(
              '@field' => $readable_fields[$field],
              '@num' => $i,
            )));
          }
          elseif ($field == 'presenter_name') {
            // Go make sure who they entered is in the database.
            $paid_enforcement = variable_get('sunflower_enforce_paid_memberships', FALSE);
            if ($paid_enforcement) {
              $valid = sunflower_validate_presenter_paid_and_exists($value);
            }
            else {
              $valid = sunflower_validate_presenter_exists($value);
            }
            if (!$valid) {
              form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter does not exist in our database @paid for presenter @num!', array(
                '%presenter' => $value,
                '@paid' => $paid_enforcement ? t('or is not a paid member') : '',
                '@num' => $i,
              )));
            }
            else {
              $proposal_id = isset($form_state['values']['proposal_id']) ? $form_state['values']['proposal_id'] : NULL;
              $validity = sunflower_validate_proposal_amount($value, $type, $proposal_id);
              if ($validity['valid'] === FALSE) {
                if ($validity['type'] == 'same') {
                  form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter in presenter @num already exists in a @type proposal!', array(
                    '%presenter' => $value,
                    '@num' => $i,
                    '@type' => $type,
                  )));
                }
                elseif ($validity['type'] == 'two') {
                  form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter in presenter @num already exists two proposals!', array(
                    '%presenter' => $value,
                    '@num' => $i,
                  )));
                }
                elseif ($validity['type'] == 'individual_panel') {
                  if ($type == 'individual') {
                    $restrict = 'panel';
                  }
                  else {
                    $restrict = 'individual';
                  }
                  form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter in presenter @num already exists in a @restrict proposal! Members may not be a part of a @type and a @restrict proposal at the same time!', array(
                    '%presenter' => $value,
                    '@num' => $i,
                    '@type' => $type,
                    '@restrict' => $restrict,
                  )));
                }
              }
              else {
                $existing = array_search($value, $all_presenters);
                if ($existing) {
                  form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter for presenter @pres has already been entered in presenter @num!', array(
                    '%presenter' => $value,
                    '@num' => $existing,
                    '@pres' => $i,
                  )));
                }
                else {
                  $all_presenters[$i] = $value;
                }
              }
            }
          }
          elseif ($field == 'abstract') {
            $number_of_words = str_word_count($value);
            if ($number_of_words > 150) {
              form_set_error("fieldset_$i][$field", t('The maximum number of words for the abstract of presenter @num is 150! You have entered @words words!', array(
                '@num' => $i,
                '@words' => $number_of_words,
              )));
            }
          }
        }
      }
    }
    if ($minimum_required) {
      drupal_set_message(t('Please note that for @type proposals at least @num presenters are required!', array(
        '@type' => $type,
        '@num' => $required_presenters[$type],
      )), 'error', FALSE);
    }
  }
}

/**
 * Submit handler for the Sunflower proposal form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function sunflower_proposal_form_submit($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');
  global $user;
  $update = FALSE;
  if (isset($form_state['values']['proposal_id'])) {
    $update = $form_state['values']['proposal_id'];
  }
  $proposal_data = array();
  $presenter_data = array();
  $max_presenters = array(
    'individual' => 2,
    'roundtable' => 4,
    'panel' => 3,
    'workshop' => 8,
  );
  $proposal_data['submitter_uid'] = isset($form_state['values']['submitter_uid']) ? $form_state['values']['submitter_uid'] : $user->uid;
  $proposal_data['last_modified'] = date("Y-m-d H:i:s");
  $type = $form_state['values']['proposal_type']['proposal_choices'];
  $field_num = $max_presenters[$type];
  for ($i = 1; $i <= $field_num; $i++) {
    $presenter_data["presenter_$i"] = $form_state['values']["fieldset_$i"];
    $presenter_data["presenter_$i"]['presenter_id'] = $i;
    $presenter_data["presenter_$i"]['entity_id'] = sunflower_retrieve_entity_id_from_full_name($form_state['values']["fieldset_$i"]['presenter_name']);
    // Grok the values and do shit with them.
  }
  // Get the proposal type, description, special and av requests as these are
  // the same across all types.
  $proposal_data['type'] = $type;
  $proposal_data['special_requests'] = $form_state['values']['requests']['special_requests'];
  $proposal_data['av_requests'] = $form_state['values']['requests']['av_requests'];
  $proposal_data['respondent_name_institutional_affiliation'] = $form_state['values']['requests']['respondent_name_institutional_affiliation'];

  // Category checking time.
  if ($type == 'individual') {
    for ($i = 1; $i <= 3; $i++) {
      $enabled = $form_state['values']['categories']['category_' . $i . '_checkbox'];
      if ($enabled) {
        $proposal_data["category_$i"] = $form_state['values']['categories']['category_' . $i . '_textfield'];
      }
    }
  }
  // If it's not an individual presentation get the title and the chair.
  if ($type != 'individual') {
    $proposal_data['panel_roundtable_workshop_title'] = $form_state['values']['proposal_details']['panel_roundtable_workshop_title'];
    $proposal_data['panel_roundtable_workshop_abstract'] = $form_state['values']['proposal_details']['panel_roundtable_workshop_abstract'];
    $proposal_data['presider_chair'] = $form_state['values']['proposal_details']['presider_chair'];
    $proposal_data['presider_email'] = $form_state['values']['proposal_details']['presider_email'];
    $proposal_data['presider_institution'] = $form_state['values']['proposal_details']['presider_institution'];
  }
  else {
    $proposal_data['panel_roundtable_workshop_title'] = '';
    $proposal_data['panel_roundtable_workshop_abstract'] = '';
    $proposal_data['presider_chair'] = '';
    $proposal_data['presider_email'] = '';
    $proposal_data['presider_institution'] = '';
  }
  $form_state['redirect'] = 'sunflower';
  drupal_set_message(t('The proposal has been submitted!'));
  sunflower_insert_data($proposal_data, $presenter_data, $update);
}

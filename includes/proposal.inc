<?php

function sunflower_proposal_form($form, &$form_state, $edit_submission = NULL) {
  module_load_include('inc', 'sunflower', 'includes/utilities');
  module_load_include('inc', 'sunflower', 'includes/db');
  $form = array(
    '#tree' => TRUE,
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
  $form['proposal_type']['panel_description'] = array(
    '#type' => 'item',
    '#markup' => t('A Panel Session proposal is a proposal for a complete session of different papers on a theme, complete with a presider, multiple papers, and (optionally) a respondent. A Short abstract and a longer description is required for each paper in the session. Presenters in a Panel Session must submit their proposals to the Panel Session organizer, who in turn is responsible for inputting them into our Sunflower system.'),
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('value' => 'panel'),
      ),
    ),
  );
  $form['proposal_type']['roundtable_description'] = array(
    '#type' => 'item',
    '#markup' => t('A Roundtable Session Proposal is a proposal of a complete session, including a presider, a list of panelists, and (optionally) a respondent, all of whom will speak (ex tempore) on a common theme.'),
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('value' => 'roundtable'),
      ),
    ),
  );
  $form['proposal_type']['workshop_description'] = array(
    '#type' => 'item',
    '#markup' => t('A Workshop Session Proposal includes up to eight papers circulated in advance on a common theme.'),
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('value' => 'workshop'),
      ),
    ),
  );
  $form['optional_individual'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#weight' => 123,
    '#title' => t('Section C TBD'),
    '#description' => t('Indicate three broad thematic groups which your proposal might fall under'),
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('value' => 'individual'),
      ),
    ),
  );
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
    '#title' => t('Panel, Roundtable or Workshop Title'),
    '#description' => t('Maximum of 100 characters'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['panel_roundtable_workshop_title'] : '',
    '#maxlength' => 100,
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
      '#autocomplete_path' => 'sunflower/autocomplete',
      '#description' => t('Type "*" to list all paid members'),
      '#default_value' => $default_presenter_value ? $db_data['presenters'][$i]['presenter_name'] : FALSE,
    );
    $form['fieldset_' . $i]['paper_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Paper Title'),
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
    if ($i > 2 && $i < 5) {
      $form['fieldset_' . $i]['#states'] = array(
        'visible' => array(
          ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
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

function sunflower_proposal_delete($form, &$form_state) {
  $proposal_id = $form_state['values']['proposal_id'];
  $form_state['redirect'] = "sunflower/proposal/delete/$proposal_id";
}

function sunflower_proposal_form_validate($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');

  if ($form_state['clicked_button']['#name'] == 'sunflower_proposal_submit') {
    $type = $form_state['values']['proposal_type']['proposal_choices'];
    $required_presenters = array(
      'individual' => 1,
      'roundtable' => 3,
      'panel' => 4,
      'workshop' => 8,
    );
    $max_presenters = array(
      'individual' => 2,
      'roundtable' => 4,
      'panel' => 4,
      'workshop' => 8,
    );
    $all_presenters = array();
    // Presider and workshop_panel_roundtable title required if not individual.
    if ($type != 'individual') {
      if (empty($form_state['values']['proposal_details']['panel_roundtable_workshop_title'])) {
        form_set_error('proposal_details][panel_roundtable_workshop_title', t('A title is required for the @type!', array('@type' => $type)));
      }
      if (empty($form_state['values']['proposal_details']['presider_chair'])) {
        form_set_error('proposal_details][presider_chair', t('A name is required for the presider of the @type!', array('@type' => $type)));
      }
    }
    $readable_fields = array(
      'presenter_name' => t('presenter name'),
      'paper_title' => t('paper title'),
      'abstract' => t('abstract'),
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
          $valid = sunflower_validate_presenter_paid_and_exists($value);
          if (!$valid) {
            form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter does not exist in our database or is not a paid member for presenter @num!', array(
            '%presenter' => $value,
            '@num' => $i,
            )));
          }
          else {
            $validity = sunflower_validate_proposal_amount($value, $type);
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
                form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter in presenter @num already exists in a @type proposal! Members may not be a part of a @type and a @restrict proposal at the same time!', array(
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
            $valid = sunflower_validate_presenter_paid_and_exists($value);
            if (!$valid) {
              form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter does not exist in our database or is not a paid member for presenter @num!', array(
                '%presenter' => $value,
                '@num' => $i,
              )));
            }
            else {
              $validity = sunflower_validate_proposal_amount($value, $type);
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
                  form_set_error("fieldset_$i][$field", t('The presenter by the name of %presenter in presenter @num already exists in a @type proposal! Members may not be a part of a @type and a @restrict proposal at the same time!', array(
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
    //form_set_error('herp', 'derp');
  }
}

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
    'panel' => 4,
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

  // If it's not an individual presentation get the title and the chair.
  if ($type != 'individual') {
    $proposal_data['panel_roundtable_workshop_title'] = $form_state['values']['proposal_details']['panel_roundtable_workshop_title'];
    $proposal_data['presider_chair'] = $form_state['values']['proposal_details']['presider_chair'];
  }
  else {
    $proposal_data['panel_roundtable_workshop_title'] = '';
    $proposal_data['presider_chair'] = '';
  }
  $form_state['redirect'] = 'sunflower';
  drupal_set_message(t('The proposal has been submitted!'));
  sunflower_insert_data($proposal_data, $presenter_data, $update);
}

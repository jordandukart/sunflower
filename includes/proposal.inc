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
    dsm($db_data);
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
  );
  $form['proposal_details']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description of Proposal for Program Review'),
    '#description' => t('Maximum of 150 words'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['description'] : '',
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
    '#title' => t('Full name of Presider/Chair'),
    '#description' => t('Who is presiding over this panel, roundtable or workshop?'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['presider_chair'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="proposal_type[proposal_choices]"]' => array('!value' => 'individual'),
      ),
    ),
  );
  $form['proposal_details']['special_requests'] = array(
    '#type' => 'textarea',
    '#title' => t('Special Requests'),
    '#default_value' => isset($db_data['proposal']) ? $db_data['proposal']['special_requests'] : '',
  );
  $form['proposal_details']['av_requests'] = array(
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
    $name_parts = array(
      'last_name',
      'first_name',
      'initials',
    );
    foreach ($name_parts as $key) {
      if ($key == 'last_name') {
        $title = t('Last Name');
      }
      elseif ($key == 'first_name') {
        $title = t('First Name');
      }
      else {
        $title = t('Initials');
      }
      $form['fieldset_' . $i][$key] = array(
        '#type' => 'textfield',
        '#title' => $title,
        '#default_value' => $default_presenter_value ? $db_data['presenters'][$i][$key] : FALSE,
      );
    }
    $form['fieldset_' . $i]['paper_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Paper Title'),
      '#description' => t('Maximum of 100 characters'),
      '#maxlength' => 100,
      '#default_value' => $default_presenter_value ? $db_data['presenters'][$i]['paper_title'] : FALSE,
    );
    $form['fieldset_' . $i]['abstract'] = array(
      '#type' => 'textfield',
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
    '#value' => empty($db_data) ? t('Submit') : t('Update'),
    '#weight' => 150,
  );
  if (!empty($db_data)) {
    $form['proposal_id'] = array(
      '#type' => 'value',
      '#value' => $db_data['proposal']['proposal_id'],
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
  // @TODO: Clarify required fields etc.
}

function sunflower_proposal_form_submit($form, &$form_state) {
  module_load_include('inc', 'sunflower', 'includes/db');

  global $user;
  $update = FALSE;
  if (isset($form_state['values']['proposal_id'])) {
    $update = TRUE;
  }
  $proposal_data = array();
  $presenter_data = array();
  $max_presenters = array(
    'individual' => 2,
    'roundtable' => 4,
    'panel' => 4,
    'workshop' => 8,
  );
  $proposal_data['submitter_uid'] = $user->uid;
  $proposal_data['last_modified'] = date("Y-m-d H:i:s");
  $type = $form_state['values']['proposal_type']['proposal_choices'];
  $field_num = $max_presenters[$type];
  for ($i = 1; $i <= $field_num; $i++) {
    $presenter_data["presenter_$i"] = $form_state['values']["fieldset_$i"];
    $presenter_data["presenter_$i"]['presenter_id'] = $i;
    // Grok the values and do shit with them.
  }
  // Get the proposal type, description, special and av requests as these are
  // the same across all types.
  $proposal_data['type'] = $form_state['values']['proposal_type'];
  $proposal_data['description'] = $form_state['values']['proposal_details']['description'];
  $proposal_data['special_requests'] = $form_state['values']['proposal_details']['special_requests'];
  $proposal_data['av_requests'] = $form_state['values']['proposal_details']['av_requests'];

  // If it's not an individual presentation get the title and the chair.
  if ($type != 'individual') {
    $proposal_data['panel_roundtable_workshop_title'] = $form_state['values']['proposal_details']['panel_roundtable_workshop_title'];
    $proposal_data['presider_chair'] = $form_state['values']['proposal_details']['presider_chair'];
  }
  $form_state['redirect'] = 'sunflower';
  drupal_set_message(t('The proposal has been submitted!'));
  sunflower_insert_data($proposal_data, $presenter_data, $update);
}

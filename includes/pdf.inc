<?php

/**
 * @file
 * Creates and renders a PDF summary for a proposal in Sunflower.
 */

/**
 * Constructs a printable PDF for a proposal.
 *
 * @param string $proposal_id
 *   The proposal ID of the proposal we want to create a pdf of.
 *
 * @return string
 *   Download link containing our created PDF.
 */
function sunflower_printable_pdf($proposal_id) {
  @include_once 'sites/all/libraries/tcpdf/config/lang/eng.php';
  @include_once 'sites/all/libraries/tcpdf/tcpdf.php';

  // Extend the TCPDF class to create custom Header and Footer.
  class MYPDF extends TCPDF {

    /**
     * Constructor.
     */
    public function __construct($proposal_id, $orientation = 'P', $unit = 'mm', $format = 'A4', $unicode = TRUE, $encoding = 'UTF-8', $diskcache = FALSE, $pdfa = FALSE) {
      $this->proposal_id = $proposal_id;
      parent::__construct($orientation, $unit, $format, $unicode, $encoding, $diskcache);
    }

    /**
     * Page header.
     */
    public function Header() {
      $this->Cell(0, 15, t('Sunflower Proposal Summary'), 0, FALSE, 'C', 0, '', 0, FALSE, 'M', 'M');
    }
  }
  // Create new PDF document.
  $pdf = new MYPDF($proposal_id, PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, TRUE, 'UTF-8', FALSE);

  $title = t('Sunflower Proposal Summary');
  // Set document information.
  $pdf->SetTitle($title);
  $pdf->SetSubject($title);

  // Set default monospaced font.
  $pdf->SetDefaultMonospacedFont(PDF_FONT_MONOSPACED);

  // Set margins.
  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
  $pdf->SetHeaderMargin(PDF_MARGIN_HEADER);

  // Set auto page breaks.
  $pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

  // Set font.
  $pdf->SetFont('times', 'BI', 12);

  // Add a page.
  $pdf->AddPage();
  $rendered_summary = sunflower_format_proposal_as_html($proposal_id);

  // Output the HTML content.
  $pdf->writeHTML($rendered_summary, TRUE, FALSE, TRUE, FALSE, '');
  // Close and output PDF document.
  $time = time();
  $pdf->Output("sunflower_proposal_summary_$time" . '.pdf', 'D');
}

/**
 * Formats Sunflower proposal data into an HTML format for PDF printing.
 *
 * @param string $proposal_id
 *   The proposal ID of the PDF being created.
 *
 * @return string
 *   HTML markup to be passed off to TCPDF.
 */
function sunflower_format_proposal_as_html($proposal_id) {
  module_load_include('inc', 'sunflower', 'includes/db');
  $db_data = sunflower_retrieve_data($proposal_id);
  $form = array();

  $proposal_items = array();
  $proposal_items[] = t('Type: @type', array('@type' => ucwords($db_data['proposal']['type'])));

  if (!empty($db_data['proposal']['panel_roundtable_workshop_title'])) {
    $proposal_items[] = t('Title: @title', array('@title' => $db_data['proposal']['panel_roundtable_workshop_title']));
  }
  if (!empty($db_data['proposal']['presider_chair'])) {
    $proposal_items[] = t('Presider: @presider', array('@presider' => $db_data['proposal']['presider_chair']));
  }
  if (!empty($db_data['proposal']['presider_email'])) {
    $proposal_items[] = t('Presider E-mail: @email', array('@email' => $db_data['proposal']['presider_email']));
  }

  $proposal_list = array(
    'title' => t('Proposal Details'),
    'type' => 'ul',
    'items' => $proposal_items,
  );
  $form['proposal_details']['#markup'] = theme('item_list', $proposal_list);

  foreach ($db_data['presenters'] as $num => $presenter) {
    if (!empty($presenter['presenter_name'])) {
      $presenter_list = array();
      $presenter_list[] = t('Name: @name', array('@name' => $presenter['presenter_name']));
      $presenter_list[] = t('Paper title: @title', array('@title' => $presenter['paper_title']));
      $presenter_list[] = t('Abstract: @abstract', array('@abstract' => $presenter['abstract']));
      $form["presenter_$num"] = array(
        '#markup' => theme('item_list', array(
          'title' => t("Presenter @num", array('@num' => $num)),
          'type' => 'ul',
          'items' => $presenter_list,
        )),
      );
    }
  }
  $category_list = array();
  if (!empty($db_data['proposal']['category_1'])) {
    $category_list[] = $db_data['proposal']['category_1'];
  }
  if (!empty($db_data['proposal']['category_2'])) {
    $category_list[] = $db_data['proposal']['category_2'];
  }
  if (!empty($db_data['proposal']['category_3'])) {
    $category_list[] = $db_data['proposal']['category_3'];
  }
  if (count($category_list)) {
    $form['categories'] = array(
      '#markup' => theme('item_list', array(
        'title' => t('Categories'),
        'type' => 'ol',
        'items' => $category_list,
      )),
    );
  }
  $request_list = array();

  if (!empty($db_data['proposal']['special_requests'])) {
    $request_list[] = t('Special Requests: @special', array('@special' => $db_data['proposal']['special_requests']));
  }
  if (!empty($db_data['proposal']['av_requests'])) {
    $request_list[] = t('AV Requests: @av', array('@av' => $db_data['proposal']['av_requests']));
  }
  if (count($request_list)) {
    $form['requests'] = array(
      '#markup' => theme('item_list', array(
        'title' => t('Requests'),
        'type' => 'ul',
        'items' => $request_list,
      )),
    );
  }
  return drupal_render($form);
}

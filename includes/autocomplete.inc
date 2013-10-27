<?php
/**
 * @file
 * Autocomplete functionality for Sunflower.
 */

/**
 * Callback that performs autocomplete operations.
 *
 * @param string $user_string
 *   The string that the user entered in the presenter autocomplete field.
 */
function sunflower_presenter_autocomplete($user_string) {
  module_load_include('inc', 'sunflower', 'includes/db');
  $paid_enforcement = variable_get('sunflower_enforce_paid_memberships', FALSE);
  if (!$paid_enforcement) {
    $users = sunflower_retrieve_all_users();
  }
  else {
    $users = sunflower_retrieve_all_paid_members();
  }
  $output = array();
  foreach ($users as $value) {
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

/**
 * Callback that performs autocomplete operations.
 *
 * @param string $user_string
 *   The string that the user entered in the categories autocomplete field.
 */
function sunflower_categories_autocomplete($user_string) {
  $output = array();
  $categories = array(
    t('Africa'),
    t('Amerindian Religions'),
    t('Ancient Religions'),
    t('Anglican Communion'),
    t('Anthropology / Ethnology'),
    t('Archaeology / Antiquities'),
    t('Archives / Special Collections'),
    t('Art / Architecture'),
    t('Asia'),
    t('Atlantic Ocean Area'),
    t('Australia / New Zealand'),
    t('Baptists'),
    t('Bible Versions'),
    t('Biblical Hermeneutics'),
    t('Biblical Studies'),
    t('Bioethics / Medical Ethics'),
    t('Biography / Autobiography'),
    t('Black Theologies - Christian'),
    t('Buddhism'),
    t('Buddhist History'),
    t('Buddhist Religious Writings'),
    t('Catholic Church'),
    t('Catholic Church Doctrine'),
    t('Catholic Church History'),
    t('Catholic Church Orders'),
    t('Central America'),
    t('Christianity - Church History'),
    t('Church Administration / Management and Leadership'),
    t('Church and State'),
    t('Church Liturgy / Worship / Music'),
    t('Computer Software'),
    t('Confucianism'),
    t('Counseling / Therapy / Support Groups'),
    t('Critical Editions and Translations of Texts and Manuscripts'),
    t('Devotional Literature - Christian'),
    t('Eastern Christianity'),
    t('Eastern Christianity - Chalcedonian'),
    t('Eastern Christianity - Non-Chalcedonian'),
    t('Economics'),
    t('Ecumenism / Christian Unity Movements'),
    t('Education'),
    t('Education - Christian'),
    t('Education - Religious'),
    t('Environmental Studies / Ecology'),
    t('Esoterism'),
    t('Ethics / Moral Theology'),
    t('Europe'),
    t('Evangelicalism'),
    t('Family'),
    t('Fiction and Poetry'),
    t('Free Churches / Anabaptists'),
    t('Gender and Sexuality Issues'),
    t('General Reference'),
    t('General Site Content'),
    t('Hebrew Scriptures / Old Testament'),
    t('Hindu History'),
    t('Hindu Religious Writings'),
    t('Hinduism'),
    t('Historical Theology - Christian'),
    t('History / Historiography'),
    t('Holocaust 1939-1945'),
    t('Indian Ocean Area'),
    t('Interdisciplinary Studies'),
    t('Interreligious Dialogue / Theology of Religions / Comparative Theology'),
    t('Intertestamental / Apocryphal and Pseudepigraphical Literature'),
    t('Islam'),
    t('Islamic History'),
    t('Islamic Religious Writings / Qur’an'),
    t('Islamic Thought and Philosophy'),
    t('Jewish History'),
    t('Jewish Thought and Philosophy'),
    t('Judaism'),
    t('Languages / Linguistics'),
    t('Law'),
    t('Liberation Theologies - Christian'),
    t('Libraries / Library Science'),
    t('Literature'),
    t('Lutheranism'),
    t('Manuscript Studies'),
    t('Media'),
    t('Medical Science / Health'),
    t('Methodism / Wesleyanism'),
    t('Minority Churches'),
    t('Missions / Evangelism / Apologetics - Christian'),
    t('Modernity / Post-Modernity / Pluralism'),
    t('Monasticism'),
    t('Mormon Church'),
    t('Mysticism - Christian'),
    t('Nation of Islam'),
    t('Natural and Applied Sciences'),
    t('New Religious Movements'),
    t('New Testament'),
    t('North America'),
    t('Occult / Witchcraft / Magic / Parapsychology / Astrology'),
    t('Other Denominations - Christian'),
    t('Other Religions'),
    t('Pacific Ocean Area'),
    t('Pentecostal / Charismatic Movements'),
    t('Performing Arts'),
    t('Philosophy'),
    t('Philosophy of Religion'),
    t('Politics / Political Science'),
    t('Practical / Pastoral Theology - Christian'),
    t('Preaching / Sermons - Christian'),
    t('Protestantism'),
    t('Psychology / Psychiatry'),
    t('Rabbinic Writings'),
    t('Reformed / Calvinist Churches'),
    t('Ritual'),
    t('Sacred Art / Architecture'),
    t('Social Studies'),
    t('Sociology'),
    t('South America'),
    t('Spiritual Life'),
    t('Spiritual Life / Christian'),
    t('Study of Religion and Mythology'),
    t('Systematic Theology - Christian'),
    t('Tablets / Ostraca / Inscriptions / Papyri'),
    t('Taoism'),
    t('Technology'),
    t('Theology'),
    t('War and Peace'),
    t('West Indies'),
    t("Women’s Histories"),
    t("Women’s Religious Thought"),
    t("Women’s Theologies - Christian"),
  );
  foreach ($categories as $value) {
    if ($user_string != '*') {
      if (strpos(drupal_strtoupper($value), drupal_strtoupper($user_string)) !== FALSE) {
        $output[$value] = $value;
      }
    }
    else {
      $output[$value] = $value;
    }
  }
  drupal_json_output($output);
}

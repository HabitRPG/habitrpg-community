<?php

/**
 * @file
 * Page callbacks for redhen contact.
 */

module_load_include('inc', 'redhen', 'includes/redhen.forms');

/**
 * Page callback for contact overview page.
 */
function redhen_contact_page() {
  // Setup the header for both the query and table.
  $header = array(
    'type' => array(
      'field' => 'type',
      'type' => 'property',
      'data' => 'Type',
      'specifier' => 'type',
    ),
    'first_name' => array(
      'field' => 'first_name',
      'type' => 'property',
      'data' => 'First Name',
      'specifier' => 'first_name',
    ),
    'last_name' => array(
      'field' => 'last_name',
      'type' => 'property',
      'data' => 'Last Name',
      'specifier' => 'last_name',
    ),
    'email' => array('data' => t('Email')),
    'updated' => array(
      'field' => 'updated',
      'type' => 'property',
      'data' => 'Updated',
      'sort' => 'desc',
      'specifier' => 'updated',
    ),
  );

  $header['operations'] = array('data' => t('Operations'));

  // Need to ensure the query doesn't execute when posing the form.
  $result = FALSE;
  if (!isset($_POST['form_id'])) {
    $bundle = isset($_GET['bundle']) ? $_GET['bundle'] : '';
    $properties = isset($_GET['properties']) ? $_GET['properties'] : array();
    $fields = isset($_GET['fields']) ? $_GET['fields'] : array();
    $result = redhen_filter_query('redhen_contact', $header, $bundle, $properties, $fields);
  }

  $contacts = array();
  if ($result) {
    $contacts = redhen_contact_load_multiple(array_keys($result['redhen_contact']));
  }

  $rows = array();
  if (!empty($contacts)) {
    $destination = drupal_get_destination();
    foreach ($contacts as $contact) {
      $uri = entity_uri('redhen_contact', $contact);
      $redhen_contact_type = redhen_contact_type_load($contact->type);
      $email = $contact->email();
      $rows[$contact->contact_id] = array(
        'type' => check_plain($redhen_contact_type->label),
        'first_name' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $contact->first_name,
            '#href' => $uri['path'],
          ),
         ),
        'last_name' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $contact->last_name,
            '#href' => $uri['path'],
          ),
        ),
        'email' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $email,
            '#href' => 'mailto:' . $email,
          ),
        ),
        'updated' => redhen_format_date($contact->updated, 'short'),
      );
      // Build a list of all the accessible operations for the current contact.
      $ops = array();
      if (redhen_contact_access('update', $contact)) {
        $ops['edit'] = array(
          'title' => t('edit'),
          'href' => $uri['path'] . '/view/edit',
          'query' => $destination,
        );
      }
      if (redhen_contact_access('delete', $contact)) {
        $ops['delete'] = array(
          'title' => t('delete'),
          'href' => $uri['path'] . '/view/delete',
          'query' => $destination,
        );
      }
      if (count($ops) > 1) {
        // Render an unordered list of operations links.
        $rows[$contact->contact_id]['operations'] = array(
          'data' => array(
            '#theme' => 'links__node_operations',
            '#links' => $ops,
            '#attributes' => array('class' => array('links', 'inline')),
          ),
        );
      }
      elseif (!empty($ops)) {
        // Render the first and only operation as a link.
        $link = reset($ops);
        $rows[$contact->contact_id]['operations'] = array(
          'data' => array(
            '#type' => 'link',
            '#title' => $link['title'],
            '#href' => $link['href'],
            '#options' => array('query' => $link['query']),
          ),
        );
      }
      else {
        unset($header['operations']);
      }
    }
  }

  return array(
    'form' => drupal_get_form('redhen_filter_form', 'redhen_contact'),
    'contacts' => array(
      '#theme' => 'redhen_contact_list',
      '#contacts' => $contacts,
      '#header' => $header,
      '#rows' => $rows,
    )
  );
}

/**
 * Page callback for adding a contact type.
 *
 * Return a list of available contacts to add or the add contact page if there's
 * only a single contact type.
 */
function redhen_contact_types_list_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // Bypass the rehen_contact/add listing if only one contact type is available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  return theme('redhen_contact_add_list', array('content' => $content));
}

/**
 * Page callback for adding a contact.
 *
 * @param $type
 *
 * @return array|mixed
 */
function redhen_contact_add_page($type) {
  $contact = redhen_contact_create(array('type' => $type));
  module_load_include('inc', 'redhen_contact', 'includes/redhen_contact.forms');
  return drupal_get_form('redhen_contact_contact_form', $contact);
}

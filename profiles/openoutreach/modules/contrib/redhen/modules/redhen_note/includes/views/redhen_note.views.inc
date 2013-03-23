<?php

/**
 * @file
 * RedHen Note Views integration
 */

/**
 * Implement hook_views_data_alter().
 *
 * Create relationships between redhen_notes and their related entities.
 */
function redhen_note_views_data_alter(&$data) {

  $entity_info = entity_get_info();

  foreach ($entity_info as $entity_type => $info) {

    // Note to entity.
    $data['redhen_note']['redhen_note_related_' . $entity_type] = array(
      'group' => t('Note'),
      'title' => t('Note to @entity',
        array('@entity' => drupal_ucfirst($info['label']))),
      'help' => t('The @entity entity that is associated with the Note.',
        array('@entity' => $info['label'])),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'label' => t('@entity from Note',
          array('@entity' => $entity_type)),
        'base' => $info['base table'],
        'base field' => $info['entity keys']['id'],
        'relationship field' => 'entity_id',
        'extra' => array(
          array('table' => 'redhen_note', 'field' => 'entity_type', 'value' => $entity_type),
        ),
      ),
    );

  }
}

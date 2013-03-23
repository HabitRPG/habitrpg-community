<?php

/**
 * Implements hook_apachesolr_field_mappings().
 */
function geofield_apachesolr_field_mappings() {
  $mappings['geofield'] = array(
    'indexing_callback' => 'geofield_apachesolr_indexing_callback',
    'map callback' => 'geofield_apachesolr_map_callback',
    'index_type' => 'geohash',
    'facets' => TRUE,
    'query types' => array('term','geo'),
    'query type' => 'term',
  );
  
  return $mappings;
}

function geofield_apachesolr_indexing_callback($entity, $field_name, $index_key, $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name})) {
    $field = $entity->$field_name;
    list($lang, $values) = each($field);
    
    foreach ($values as $value) {
      $fields[] = array(
        'key' => $index_key,
        'value' => $value['lat'] . ',' . $value['lon'],
      );
    }
  }
  return $fields;
}

/**
 * Map of the facet labels.
 *
 * @param array $values
 * @param array $options
 * @return type
 */
function geofield_apachesolr_map_callback(array $values, array $options) {
  $map = array();
  foreach ($values as $key) {
    $map[$key] = substr($key, 1) . 'km';
  }
  return $map;
}

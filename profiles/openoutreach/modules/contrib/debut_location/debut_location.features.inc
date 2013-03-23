<?php
/**
 * @file
 * debut_location.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function debut_location_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "openlayers" && $api == "openlayers_maps") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "openlayers" && $api == "openlayers_styles") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function debut_location_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function debut_location_node_info() {
  $items = array(
    'location' => array(
      'name' => t('Location'),
      'base' => 'node_content',
      'description' => t('A physical location.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}

<?php

/**
 * @file
 *
 */

/**
 * Wrapper for system_admin_menu_block_page().
 */
function redhen_landing_page() {
  $items = array(
    'contact' => array(
      'module' => 'redhen_contact',
      'path' => 'redhen/contact',
      'title' => 'Contacts',
      'description' => t('View, manage, and search contacts.'),
      'permission' => 'access redhen contacts'
    ),
    'org' => array(
      'module' => 'redhen_org',
      'path' => 'redhen/org',
      'title' => 'Organizations',
      'description' => t('View, manage, and search organizations.'),
      'permission' => 'access redhen orgs'
    ),
    'membership' => array(
      'module' => 'redhen_membership',
      'path' => 'redhen/memberships',
      'title' => 'Memberships',
      'description' => t('View, manage, and search memberships.'),
      'permission' => 'access redhen memberships'
    ),
    'activity' => array(
      'module' => 'redhen_activity',
      'path' => 'redhen/activity',
      'title' => 'Activity',
      'description' => t('RedHen activity stream.'),
      'permission' => 'access redhen activity'
    ),
    'structure' => array(
      'path' => 'admin/structure/redhen',
      'title' => 'Structure',
      'description' => t('Define bundles and manage fields for RedHen entities.'),
      'permission' => 'administer redhen'
    ),
    'settings' => array(
      'path' => 'admin/config/redhen',
      'title' => 'Settings',
      'description' => t('RedHen configuration and settings.'),
      'permission' => 'administer redhen'
    )
  );

  foreach ($items as $key => $item) {
    if ((isset($item['module']) && !module_exists($item['module'])) || !user_access($item['permission'])) {
      unset($items[$key]);
    }
  }

  return theme('redhen_home', array('items' => $items));
}

/**
 * Settings form for Redhen CRM.
 *
 * @return array
 */
function redhen_settings() {
  $form['intro'] = array(
    '#type' => 'item',
    '#markup' => t('RedHen CRM settings.'),
  );

  // allow other modules to inject their own settings.
  $form += module_invoke_all('redhen_settings');
  return system_settings_form($form);
}

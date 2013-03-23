<?php
/**
 * @file
 * debut_media.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function debut_media_user_default_permissions() {
  $permissions = array();

  // Exported permission: add media from remote sources.
  $permissions['add media from remote sources'] = array(
    'name' => 'add media from remote sources',
    'roles' => array(
      0 => 'administrator',
      1 => 'contributor',
      2 => 'editor',
    ),
    'module' => 'media_internet',
  );

  // Exported permission: create files.
  $permissions['create files'] = array(
    'name' => 'create files',
    'roles' => array(
      0 => 'administrator',
      1 => 'contributor',
      2 => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: delete any files.
  $permissions['delete any files'] = array(
    'name' => 'delete any files',
    'roles' => array(
      0 => 'administrator',
      1 => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: delete own files.
  $permissions['delete own files'] = array(
    'name' => 'delete own files',
    'roles' => array(
      0 => 'administrator',
      1 => 'contributor',
      2 => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: edit any files.
  $permissions['edit any files'] = array(
    'name' => 'edit any files',
    'roles' => array(
      0 => 'administrator',
      1 => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: edit own files.
  $permissions['edit own files'] = array(
    'name' => 'edit own files',
    'roles' => array(
      0 => 'administrator',
      1 => 'contributor',
      2 => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: import media.
  $permissions['import media'] = array(
    'name' => 'import media',
    'roles' => array(
      0 => 'administrator',
      1 => 'contributor',
      2 => 'editor',
    ),
    'module' => 'media',
  );

  // Exported permission: view files.
  $permissions['view files'] = array(
    'name' => 'view files',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
      3 => 'contributor',
      4 => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: view own private files.
  $permissions['view own private files'] = array(
    'name' => 'view own private files',
    'roles' => array(
      0 => 'administrator',
      1 => 'contributor',
      2 => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: view own unpublished files.
  $permissions['view own unpublished files'] = array(
    'name' => 'view own unpublished files',
    'roles' => array(
      0 => 'administrator',
      1 => 'contributor',
      2 => 'editor',
    ),
    'module' => 'file_admin',
  );

  return $permissions;
}

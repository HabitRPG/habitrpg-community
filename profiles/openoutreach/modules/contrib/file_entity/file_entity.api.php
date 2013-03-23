<?php

/**
 * @file
 * Hooks provided by the File Entity module.
 */

/**
 * Declare that your module provides default file types.
 *
 * Your module may already implement this hook for other CTools plugin types.
 * If so, copy the body of this function into the existing hook.
 */
function hook_ctools_plugin_api($owner, $api) {
  if ($owner == 'file_entity' && $api == 'file_type') {
    return array('version' => 1);
  }
}

/**
 * Define default file types.
 *
 * File types are implemented as CTools exportables, so modules can alter the
 * defaults via hook_file_default_types_alter(), and the administrator can
 * save overridden and custom types to the {file_type} database table.
 *
 * @return array
 *   An array whose keys are file type names and whose values are objects
 *   representing the file type, with the following key/value pairs:
 *   - api_version: The version of this data.
 *   - type: The file type name.
 *   - label: The human-readable name of the file type.
 *   - description: The description of this file type.
 *   - mimetypes: An array of mimetypes that this file type will map to.
 */
function hook_file_default_types() {
  return array(
    'image' => (object) array(
      'api_version' => 1,
      'type' => 'image',
      'label' => t('Image'),
      'description' => t("An <em>Image</em> is a two-dimensional picture that has a similar appearance to some subject, usually a physical object or a person."),
      'mimetypes' => array(
        'image/jpeg',
        'image/gif',
        'image/png',
      ),
    ),
  );
}

/**
 * Alter default file types.
 *
 * @see hook_file_default_types()
 */
function hook_file_default_types_alter(&$types) {
  $types['image']->mimetypes[] = 'image/svg+xml';
}

/**
 * Define file formatters.
 *
 * @return
 *   An array whose keys are file formatter names and whose values are arrays
 *   describing the formatter.
 *
 * @todo Document key/value pairs that comprise a formatter.
 *
 * @see hook_file_formatter_info_alter()
 */
function hook_file_formatter_info() {
  // @todo Add example.
}

/**
 * Perform alterations on file formatters.
 *
 * @param $info
 *   Array of information on file formatters exposed by
 *   hook_file_formatter_info() implementations.
 */
function hook_file_formatter_info_alter(&$info) {
  // @todo Add example.
}

/**
 * @todo Add documentation.
 *
 * Note: This is not really a hook. The function name is manually specified via
 * 'view callback' in hook_file_formatter_info(), with this recommended callback
 * name pattern.
 */
function hook_file_formatter_FORMATTER_view($file, $display, $langcode) {
}

/**
 * @todo Add documentation.
 *
 * Note: This is not really a hook. The function name is manually specified via
 * 'settings callback' in hook_file_formatter_info(), with this recommended
 * callback name pattern.
 */
function hook_file_formatter_FORMATTER_settings($form, &$form_state, $settings) {
}

/**
 * @todo Add documentation.
 */
function hook_file_displays_alter($displays, $file, $view_mode) {
}

/**
 * @todo Add documentation.
 */
function hook_file_view($file, $view_mode, $langcode) {
}

/**
 * @todo Add documentation.
 */
function hook_file_view_alter($build, $type) {
}

/**
 * Control access to a file.
 *
 * Modules may implement this hook if they want to have a say in whether or not
 * a given user has access to perform a given operation on a file.
 *
 * The administrative account (user ID #1) always passes any access check,
 * so this hook is not called in that case. Users with the "bypass file access"
 * permission may always view and edit files through the administrative
 * interface.
 *
 * Note that not all modules will want to influence access on all
 * file types. If your module does not want to actively grant or
 * block access, return FILE_ENTITY_ACCESS_IGNORE or simply return nothing.
 * Blindly returning FALSE will break other file access modules.
 *
 * @param $op
 *   The operation to be performed. Possible values:
 *   - "create"
 *   - "delete"
 *   - "update"
 *   - "view"
 * @param $file
 *   The file on which the operation is to be performed, or, if it does
 *   not yet exist, the type of file to be created.
 * @param $account
 *   A user object representing the user for whom the operation is to be
 *   performed.
 *
 * @return
 *   FILE_ENTITY_ACCESS_ALLOW if the operation is to be allowed;
 *   FILE_ENTITY_ACCESS_DENY if the operation is to be denied;
 *   FILE_ENTITY_ACCESS_IGNORE to not affect this operation at all.
 *
 * @ingroup file_entity_access
 */
function hook_file_entity_access($op, $file, $account) {
  $type = is_string($file) ? $file : $file->type;

  if ($op !== 'create' && (REQUEST_TIME - $file->timestamp) < 3600) {
    // If the file was uploaded in the last hour, deny access to it.
    return FILE_ENTITY_ACCESS_DENY;
  }

  // Returning nothing from this function would have the same effect.
  return FILE_ENTITY_ACCESS_IGNORE;
}

/**
 * Control access to listings of files.
 *
 * @param $query
 *   A query object describing the composite parts of a SQL query related to
 *   listing files.
 *
 * @see hook_query_TAG_alter()
 * @ingroup file_entity_access
 */
function hook_query_file_entity_access_alter(QueryAlterableInterface $query) {
  // Only show files that have been uploaded more than an hour ago.
  $query->condition('timestamp', REQUEST_TIME - 3600, '<=');
}

/**
 * Decides which file type (bundle) should be assigned to a file entity.
 *
 * @param $file
 *   File object.
 *
 * @return
 *   Array of file type machine names that can be assigned to a given file type.
 *   If there are more proposed file types the one, that was returned the first,
 *   wil be chosen. This can be, however, changed in alter hook.
 *
 * @see hook_file_type_alter()
 */
function hook_file_type($file) {
  // Assign all files uploaded by anonymous users to a special file type.
  if (user_is_anonymous()) {
    return array('untrusted_files');
  }
}

/**
 * Alters list of file types that can be assigned to a file.
 *
 * @param $types
 *   List of proposed types.
 * @param $file
 *   File object.
 */
function hook_file_type_alter(&$types, $file) {
  // Choose a specific, non-first, file type.
  $types = array($types[4]);
}

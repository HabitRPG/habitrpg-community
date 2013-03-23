<?php

/**
 * @file
 * Provides functionality to be used at install time.
 */

/**
 * Generate an install task to install apps.
 *
 * Adapted from panopoly_install_tasks().
 */
function openoutreach_install_tasks($install_state) {

  // Kick off the tasks.
  $tasks = array();

  // Summon the power of the Apps module.
  require_once(drupal_get_path('module', 'apps') . '/apps.profile.inc');

  if ($install_state['interactive']) {
    $default_apps = array(
      'debut_article',
      'debut_bio',
      'debut_blog',
      'debut_comment',
      'debut_highlighted',
      'debut_link',
      'debut_media',
      'debut_section',
      'debut_seo',
      'debut_social',
      'debut_wysiwyg',
    );
  }
  // When installing automatically, there is no opportunity to select which
  // apps to install. Therefore, don't enable any and let a site admin choose
  // post-install.
  else {
    $default_apps = array();
  }

  // Set up the openoutreach Apps install task.
  $debut_server = array(
    'machine name' => 'debut',
    'default apps' => $default_apps,
    'required apps' => array(),
  );

  // Insert the apps support verification test after the apps select form so
  // that it shows only if needed.
  $apps_tasks = apps_profile_install_tasks($install_state, $debut_server);
  $select_task_name = 'apps_profile_apps_select_form_debut';
  $verify_task_name = 'apps_install_verify';
  if (isset($apps_tasks[$select_task_name]) && isset($apps_tasks[$verify_task_name])) {
    $apps_select_form_task = $apps_tasks[$select_task_name];
    $apps_verify_form_task = $apps_tasks[$verify_task_name];
    unset($apps_tasks[$select_task_name]);
    unset($apps_tasks[$verify_task_name]);
    $tasks[$select_task_name] = $apps_select_form_task;
    $apps_verify_form_task['run'] = (!empty($_SESSION['apps_downloads'])) ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP;
    $tasks[$verify_task_name] = $apps_verify_form_task;
  }

  $tasks = $tasks + $apps_tasks;

  // Rename one of the default apps tasks. In the case of a non-interactive
  // installation, apps_profile_install_tasks() never defines this task, so we
  // need to make sure we don't accidentally create it when it doesn't exist.
  if (isset($tasks['apps_profile_apps_select_form_debut'])) {
    $tasks['apps_profile_apps_select_form_debut']['display_name'] = t('Install apps for Open Outreach');
  }

  // Enable any selected Open Outreach features.
  $tasks['openoutreach_enable_features'] = array(
    'display' => FALSE,
    'type' => 'batch',
    'run' => (isset($_SESSION['openoutreach_features'])) ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
    'function' => 'openoutreach_enable_features',
  );

  // Set up the theme selection task.
  $tasks['openoutreach_theme_form'] = array(
    'display_name' => t('Choose a theme'),
    'type' => 'form',
  );

  // Set up the prepare task to close it out.
  $tasks['openoutreach_prepare'] = array(
    'display_name' => t('Prepare site'),
    'type' => 'form',
  );

  return $tasks;
}

/**
 * Implements hook_install_tasks_alter()
 */
function openoutreach_install_tasks_alter(&$tasks, $install_state) {
  // Create a more informative finished page.
  $tasks['install_finished']['function'] = 'openoutreach_finished';
  $tasks['install_finished']['display_name'] = t('Finished!');
  $tasks['install_finished']['type'] = 'form';
}

/**
 * Implements hook_install_configure_form_alter().
 */
function openoutreach_form_install_configure_form_alter(&$form, &$form_state) {
  // Set some reasonable defaults for site configuration form.
  $form['site_information']['site_name']['#default_value'] = 'Open Outreach';
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  // Don't set the email address to "admin@localhost" as that will fail D7's
  // email address validation.
  if ($_SERVER['HTTP_HOST'] != 'localhost') {
    $form['site_information']['site_mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
    $form['admin_account']['account']['mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
  }
  // Hide update notifications.
  $form['update_notifications']['#access'] = FALSE;
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adapted from panopoly_apps_profile_apps_select_form_alter().
 *
 * @todo: restore hidden content fieldset when Debut apps support it.
 */
function openoutreach_form_apps_profile_apps_select_form_alter(&$form, $form_state) {
  // For some things there are no need.
  $form['apps_message']['#access'] = FALSE;
  $form['apps_fieldset']['apps']['#title'] = NULL;

  // Debut doesn't yet support default content.
  $form['default_content_fieldset']['#access'] = FALSE;

  // Improve style of apps selection form.
  if (isset($form['apps_fieldset'])) {
    $options = array();
    foreach($_SESSION['apps_manifest'] as $name => $app) {
      if ($name != '#theme') {
        $options[$name] = '<strong>' . $app['name'] . '</strong><p><div class="admin-options"><div class="form-item">' . theme('image', array('path' => $app['logo']['path'], 'height' => '32', 'width' => '32')) . '</div>' . $app['description'] . '</div></p>';
      }
    }
    ksort($options);
    $form['apps_fieldset']['apps']['#options'] = $options;
    // Remove the deprecated Debut app.
    unset($form['apps_fieldset']['apps']['#options']['debut']);
  }

  // Enable installation of Open Outreach features.
  // Extract features checkboxes from the features admin form.
  module_load_include('inc', 'features', 'features.admin');
  $features_admin_form = features_admin_form(array(), array());
  $features = $features_admin_form['features']['status'];
  // Core will be enabled already.
  unset($features['openoutreach_core']);
  // Only show openoutreach features.
  foreach (array_keys($features) as $name) {
    if (strpos($name, 'openoutreach_') !== 0) {
      unset($features[$name]);
    }
    else {
      // Remove links to the feature, since they won't work here.
      $features[$name]['#title'] = strip_tags($features[$name]['#title']);
      // Default to enabled.
      $features[$name]['#default_value'] = TRUE;
    }
  }
  $form['openoutreach_features'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Open Outreach features'),
    '#description' => t('Select the features you wish to enable. You will also have a chance to enable or disable features after installing.'),
    '#weight' => -1,
    '#tree' => TRUE,
  ) + $features;

  $form['#submit'][] = 'openoutreach_apps_profile_apps_select_form_submit';
}

/**
 * Submit handler for install_configure_form. Set a variable specifying the
 * features to be installed.
 */
function openoutreach_apps_profile_apps_select_form_submit($form, &$form_state) {
  if (isset($form_state['values']['op']) && ($form_state['values']['op'] == t('Install Apps'))) {
    $features = array_keys(array_filter($form_state['values']['openoutreach_features']));
    if (!empty($features)) {
      $_SESSION['openoutreach_features'] = $features;
    }
  }
}

/**
 * Install selected Open Outreach features.
 */
function openoutreach_enable_features(&$install_state) {
  $features = $_SESSION['openoutreach_features'];

  if (!empty($features)) {
    $operations = array();
    foreach ($features as $module) {
      $module_info = system_get_info('module', $module);
      $module_name = isset($module_info['name']) ? $module_info['name'] : $module;
      $operations[] = array('app_profile_enable_module', array($module, $module_name));
    }
    if (!empty($operations)) {
      $batch = array(
        'title' => t('Enable Open Outreach features'),
        'init_message' => t('Enabling features...'),
        'progress_message' => t('Enabling @current out of @total features.'),
        'operations' => $operations,
        'error_message' => t('Feature enabling encountered an error.'),
        'file' => drupal_get_path('module', 'apps') . '/apps.profile.inc',
      );
    }
    return $batch;
  }
}

/**
 * Pseudo implementation of hook_user_default_permissions().
 *
 * @see openoutreach_install_permissions().
 */
function openoutreach_install_user_default_permissions() {
  $permissions = array();

  // Exported permission: access contextual links
  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(
      0 => 'administrator',
      1 => 'editor',
    ),
    'module' => 'contextual',
  );

  // Exported permission: search content
  $permissions['search content'] = array(
    'name' => 'search content',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'search',
  );

  // Exported permission: use advanced search
  $permissions['use advanced search'] = array(
    'name' => 'use advanced search',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'search',
  );

  // Exported permission: view advanced help index
  $permissions['view advanced help index'] = array(
    'name' => 'view advanced help index',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'advanced_help',
  );

  // Exported permission: view advanced help popup
  $permissions['view advanced help popup'] = array(
    'name' => 'view advanced help popup',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'advanced_help',
  );

  // Exported permission: view advanced help topic
  $permissions['view advanced help topic'] = array(
    'name' => 'view advanced help topic',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'advanced_help',
  );

  return $permissions;
}

/**
 * Install permissions for optional modules.
 *
 * Optional modules - identified by the "recommends" array in
 * openoutreach.info - cannot have their permissions in features; recommended
 * modules are enabled after features permissions are implemented, and in any
 * case such features would break if the optional module was disabled.
 *
 * Take advantage of existing support in the Features module for processing
 * default user permissions. Use a pseudo module name, 'openoutreach_install'.
 */
function openoutreach_install_permissions() {
  features_include();
  module_load_include('inc', 'features', 'features.export');
  user_permission_features_rebuild('openoutreach_install');
}

/**
 * Form to choose the starting theme from list of available options.
 */
function openoutreach_theme_form($form, &$form_state) {

  // Declare a white list of themes offered for selection along with data used
  // to determine what modules should be enabled and what blocks configured.
  $form_state['theme dependencies'] = array(
    'outreach' => array('superfish'),
    'sky' => array('superfish'),
    'bartik' => array(),
  );

  // Create list of theme options.
  $theme_data = array_intersect_key(system_rebuild_theme_data(), $form_state['theme dependencies']);
  $themes = array();
  foreach($theme_data as $theme) {
    $themes[$theme->name] = theme('image', array('path' => $theme->info['screenshot'])) . '<strong>' . $theme->info['name'] . '</strong><br><p><em>' . strip_tags($theme->info['description']) . '</em></p><p class="clearfix"></p>';
  }

  // Move Bartik to last.
  $bartik = $themes['bartik'];
  unset($themes['bartik']);
  $themes['bartik'] = $bartik;

  $form['theme'] = array(
    '#title' => t('Starting Theme'),
    '#type' => 'radios',
    '#options' => $themes,
    '#default_value' => 'outreach',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Choose theme'),
  );

  $form['#attached']['css'] = array(
    drupal_get_path('profile', 'openoutreach') . '/openoutreach.install.css',
  );

  return $form;
}

/**
 * Form submit handler to select the theme.
 */
function openoutreach_theme_form_submit($form, &$form_state) {
  // Enable and set the theme of choice.
  $theme = $form_state['values']['theme'];
  theme_enable(array($theme));
  variable_set('theme_default', $theme);
  // Enable and set the theme of choice.
  $dependencies = $form_state['theme dependencies'];

  if (!empty($dependencies[$theme])) {
    module_enable($dependencies[$theme]);
  }

  // Assign regions for the theme.
  $regions = system_region_list($theme);
  $blocks = array();

  $assignments = array();
  $assignments['system'] = array(
    'main' => 'content',
    'help' => 'help',
  );

  if (module_exists('superfish')) {
    $assignments['superfish'] = array(
      // 'menu_bar' is the region used by adaptivetheme subthemes.
      'superfish_1' => 'menu_bar',
    );
  }

  foreach ($assignments as $module => $module_blocks) {
    foreach ($module_blocks as $delta => $region) {
      if (isset($regions[$region])) {
        $blocks[] = array(
          'module' => $module,
          'delta' => $delta,
          'theme' => $theme,
          'status' => 1,
          'weight' => 0,
          'region' => $region,
          'pages' => '',
          'cache' => -1,
        );
      }
    }
  }
  if ($blocks) {
    foreach ($blocks as $record) {
      $query = db_merge('block')
        ->key(array('theme' => $record['theme'], 'module' => $record['module'], 'delta' => $record['delta']))
        ->fields($record)
        ->execute();
    }
  }

  // Flush theme caches so things are right.
  system_rebuild_theme_data();
  drupal_theme_rebuild();
}

/**
 * Form to talk about preparing the site for prime time.
 */
function openoutreach_prepare($form, &$form_state) {
  $form = array();

  $form['opening'] = array(
    '#markup' => '<h1>' . t('Prepare Site') . '</h1>',
  );

  $form['openingtext'] = array(
    '#markup' => '<h2>' . t('Open Outreach now needs to do a bit more to get everything set up.') . '</h2><p>' . t('This step may take a few minutes.') . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Prepare your site'),
  );

  return $form;
}

/**
 * Submit form to prepare site for prime time.
 */
function openoutreach_prepare_submit($form, &$form_state) {
  openoutreach_install_permissions();

  // Try to allocate enough time to rebuild features.
  drupal_set_time_limit(240);

  // Flush all caches to ensure that any full bootstraps during the installer
  // do not leave stale cached data, and that any content types or other items
  // registered by the install profile are registered correctly. Flushing also
  // triggers features component rebuilding.
  drupal_flush_all_caches();

  // Remember the profile which was used.
  variable_set('install_profile', drupal_get_profile());

  // Install profiles are always loaded last.
  db_update('system')
    ->fields(array('weight' => 1000))
    ->condition('type', 'module')
    ->condition('name', drupal_get_profile())
    ->execute();

  // Cache a fully-built schema.
  drupal_get_schema(NULL, TRUE);

  // Run cron to populate update status tables (if available) so that users
  // will be warned if they've installed an out of date Drupal version.
  // Will also trigger indexing of profile-supplied content or feeds.
  drupal_cron_run();
}

/**
 * Form to finish it all out and send us on our way.
 */
function openoutreach_finished($form, &$form_state) {
  $form = array();

  $form['opening'] = array(
    '#markup' => '<h1>' . t('Finished!') . '</h1>',
  );

  $form['openingtext'] = array(
    '#markup' => '<h2>' . t('Congratulations, you just installed Open Outreach!') . '</h2>',
  );

  $form['openoutreach_logo'] = array(
    '#markup' => theme('image', array('path' => drupal_get_path('profile', 'openoutreach') . '/images/openoutreach_logo.png')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Visit your new site'),
  );

  return $form;
}

/**
 * Submit form to finish it out and send us on our way!
 */
function openoutreach_finished_submit($form, &$form_state) {
  // Once more for good measure.
  drupal_flush_all_caches();

  unset($_SESSION['openoutreach_features']);

  // And away we go.
  // $form_state['redirect'] won't work here since we are still in the
  // installer, so use drupal_goto() (for interactive installs only) instead.
  $install_state = $form_state['build_info']['args'][0];
  if ($install_state['interactive']) {
    drupal_goto('<front>');
  }
}

<?php

/**
 * Register our directory as a place to find provision classes.
 */
function provision_storage_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_drush_init().
 */
function provision_storage_drush_init() {
  provision_storage_provision_register_autoload();
}

/**
 *  Implements hook_provision_services().
 */
function provision_storage_provision_services() {
  provision_storage_provision_register_autoload();
  return array('storage' => NULL);
}

/**
 * if we should hook this module at all
 *
 * this is necessary because the drush extension may be deployed in the
 * hostmaster site-specific modules directory, in which case it will load even
 * if the feature is disabled in the frontend.
 *
 * for now we just skip hostmaster sites, but we should really be checking
 * for drush_get_option('hosting_features') or something to that effect.
 *
 * XXX: this should really be handled by Aegir core, not contrib modules.
 */
function _provision_storage_enabled() {
  return (d()->type == 'site' && d()->profile != 'hostmaster');
}

/**
 * Implements hook_provision_drupal_create_directories_alter().
 */
function provision_storage_provision_drupal_create_directories_alter(&$dirs, $url) {
  if (_provision_storage_enabled()) {
    return d()->service('storage')->create_directories_alter($dirs, $url);
  }
}

/**
 * Implements hook_provision_provision_drupal_chgrp_directories_alter().
 */
function provision_storage_provision_drupal_chgrp_directories_alter(&$chgrp, $url) {
  if (_provision_storage_enabled()) {
    return d()->service('storage')->chgrp_directories_alter($chgrp, $url);
  }
}

// Allow storage services to respond to normal provision events.
function drush_provision_storage_pre_provision_delete() {
  if (_provision_storage_enabled()) {
    d()->service('storage')->pre_delete();
  }
}
function drush_provision_storage_post_provision_delete() {
  if (_provision_storage_enabled()) {
    d()->service('storage')->post_delete();
  }
}
function drush_provision_storage_pre_provision_verify() {
  if (_provision_storage_enabled()) {
    d()->service('storage')->pre_verify();
  }
}
function drush_provision_storage_post_provision_verify() {
  if (_provision_storage_enabled()) {
    d()->service('storage')->post_verify();
  }
}
function drush_provision_storage_pre_provision_migrate() {
  if (_provision_storage_enabled()) {
    d()->service('storage')->pre_migrate();
  }
}
function drush_provision_storage_post_provision_migrate() {
  if (_provision_storage_enabled()) {
    d()->service('storage')->post_migrate();
  }
}
function drush_provision_storage_pre_provision_clone() {
  if (_provision_storage_enabled()) {
    d()->service('storage')->pre_clone();
  }
}
function drush_provision_storage_post_provision_clone() {
  if (_provision_storage_enabled()) {
    d()->service('storage')->post_clone();
  }
}

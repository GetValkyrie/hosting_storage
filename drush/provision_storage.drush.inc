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
 * Implements hook_provision_drupal_create_directories_alter().
 */
function provision_storage_provision_drupal_create_directories_alter(&$dirs, $url) {
  return d()->service('storage')->create_directories_alter($dirs, $url);
}

/**
 * Implements hook_provision_provision_drupal_chgrp_directories_alter().
 */
function provision_storage_provision_drupal_chgrp_directories_alter(&$chgrp, $url) {
  return d()->service('storage')->chgrp_directories_alter($chgrp, $url);
}

// Allow storage services to respond to normal provision events.
function drush_provision_storage_pre_provision_delete() {
  d()->service('storage')->pre_delete();
}
function drush_provision_storage_post_provision_delete() {
  d()->service('storage')->post_delete();
}

function drush_provision_storage_pre_provision_verify() {
  d()->service('storage')->pre_verify();
}
function drush_provision_storage_post_provision_verify() {
  d()->service('storage')->post_verify();
}

function drush_provision_storage_pre_provision_migrate() {
  d()->service('storage')->pre_migrate();
}
function drush_provision_storage_post_provision_migrate() {
  d()->service('storage')->post_migrate();
}

function drush_provision_storage_pre_provision_clone() {
  d()->service('storage')->pre_clone();
}
function drush_provision_storage_post_provision_clone() {
  d()->service('storage')->post_clone();
}

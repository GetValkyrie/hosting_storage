<?php
/**
 * @file
 * Expose the storage service to Hostmaster.
 */

/**
 * Implementation of hook_hosting_feature().
 */
function hosting_storage_hosting_feature() {
  $features['storage'] = array(
    'title' => t('Storage service support'),
    'description' => t('Allows Aegir to store the Drupal files directory in a shared filesystem'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_storage',
    'group' => 'optional'
  );
  return $features;
}

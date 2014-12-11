<?php

/**
 * Implementation of the site storage service.
 *
 * This will store the site directory in a location (storage_location,
 * inherited from parent) completely outside platforms.
 */
class Provision_Service_storage_site extends Provision_Service_storage_files {

  /**
   * @inheritdoc
   */
  public function create_directories_alter(&$dirs, $url) {
    // Instead of creating the files directories in the platform/sites/sitename.com
    // directory, create the directories in the filestorage directory, and symlink
    // them into place.

    // First, make sure sites/$url is ready to go.
    parent::create_directories_alter($dirs, $url);

    // Next, create the file storage area for the site within the filestorage dir.
    $site_storage = $this->server->storage_location . '/' . $url;
    provision_file()->create_dir($site_storage, 'Site files directory', 0755);

    // Then, create the machine writable folders in this new directory instead.
    unset($dirs["sites/$url"]);

    // Finally, symlink everything into place (remove the link first, in case it aleady exists for some reason).
    provision_file()->symlink($site_storage, "sites/$url")
      ->succeed('Symlinked sites directory into place.')
      ->fail('Could not symlink sites directory into place.', 'DRUSH_PERM_ERROR');
  }

  /**
   * @inheritdoc
   */
  public function chgrp_directories_alter(&$chgrp, $url) {
    // Remove old directories...
    unset($chgrp["sites/$url/private"]);
    unset($chgrp["sites/$url/files"]);
    unset($chgrp["sites/$url/files/tmp"]);
    unset($chgrp["sites/$url/files/images"]);
    unset($chgrp["sites/$url/files/pictures"]);
    unset($chgrp["sites/$url/files/css"]);
    unset($chgrp["sites/$url/files/js"]);
    unset($chgrp["sites/$url/files/ctools"]);
    unset($chgrp["sites/$url/files/imagecache"]);
    unset($chgrp["sites/$url/files/locations"]);
    unset($chgrp["sites/$url/private/files"]);
    unset($chgrp["sites/$url/private/temp"]);

    // ...and replace with the new ones.
    $site_storage = $this->server->storage_location . '/' . $url;
    $chgrp["$site_storage/private"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files/tmp"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files/images"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files/pictures"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files/css"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files/js"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files/ctools"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files/imagecache"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/files/locations"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/private/files"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/private/temp"] = d('@server_master')->web_group;;
  }

  /**
   * Clean up the site storage directory when a site is deleted.
   */
  public function post_delete() {
    $url = d()->uri;
    $site_storage = $this->server->storage_location . '/' . $url;

    // Recursively delete the site storage directory.
    _provision_recursive_delete($site_storage);
  }

  /**
   * Clean up during migrate operations.
   */
  public function post_migrate() {
    // At the end of a migrate operation, d()->uri is still set to the old
    // site's URI, so we can use it to clean up the old filestorage directory.
    // Fortunately, this code is already written, so we'll just call the function.
    drush_log(dt('Cleaning up the source site file storage.'), 'notice');
    $this->post_delete();
  }
}

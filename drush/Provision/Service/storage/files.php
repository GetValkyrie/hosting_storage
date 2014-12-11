<?php

/**
 * Implementation of the basic storage service.
 */
class Provision_Service_storage_files extends Provision_Service_storage {

  /**
   * Initialize this class, including option handling.
   */
  function init_server() {
    // REMEMBER TO CALL THE PARENT!
    parent::init_server();
    $this->server->setProperty('storage_location');
    $this->server->setProperty('preinstall_script');
    $this->server->setProperty('postinstall_script');
    $this->server->setProperty('predelete_script');
    $this->server->setProperty('postdelete_script');
  }

  /**
   * Implementation of service verify.
   */
  public function verify_server_cmd() {
    // Ensure the storage directory exists.
    provision_file()->create_dir($this->server->storage_location, dt("Storage location"), 0755);
    $this->sync($this->server->storage_location, array(
      'exclude' => $this->server->storage_location . '/*',  // Make sure remote directory is created
    ));
  }

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
    unset($dirs["sites/$url/private"]);
    unset($dirs["sites/$url/files"]);
    unset($dirs["sites/$url/files/tmp"]);
    unset($dirs["sites/$url/files/images"]);
    unset($dirs["sites/$url/files/pictures"]);
    unset($dirs["sites/$url/files/css"]);
    unset($dirs["sites/$url/files/js"]);
    unset($dirs["sites/$url/files/ctools"]);
    unset($dirs["sites/$url/files/imagecache"]);
    unset($dirs["sites/$url/files/locations"]);
    unset($dirs["sites/$url/files/styles"]);
    unset($dirs["sites/$url/private/config"]);
    unset($dirs["sites/$url/private/config/active"]);
    unset($dirs["sites/$url/private/config/staging"]);
    unset($dirs["sites/$url/private/files"]);
    unset($dirs["sites/$url/private/temp"]);
    $dirs["$site_storage/private"] = 02770;
    $dirs["$site_storage/files"] = 02770;
    $dirs["$site_storage/files/tmp"] = 02770;
    $dirs["$site_storage/files/images"] = 02770;
    $dirs["$site_storage/files/pictures"] = 02770;
    $dirs["$site_storage/files/css"] = 02770;
    $dirs["$site_storage/files/js"] = 02770;
    $dirs["$site_storage/files/ctools"] = 02770;
    $dirs["$site_storage/files/imagecache"] = 02770;
    $dirs["$site_storage/files/locations"] = 02770;
    $dirs["$site_storage/files/styles"] = 02770;
    $dirs["$site_storage/private/config"] = 02770;
    $dirs["$site_storage/private/config/active"] = 02770;
    $dirs["$site_storage/private/config/staging"] = 02770;
    $dirs["$site_storage/private/files"] = 02770;
    $dirs["$site_storage/private/temp"] = 02770;

    // Finally, symlink everything into place (remove the link first, in case it aleady exists for some reason).
    provision_file()->unlink("sites/$url/files");
    provision_file()->symlink($site_storage . '/files', "sites/$url/files")
      ->succeed('Symlinked files directory into place.')
      ->fail('Could not symlink files directory into place.', 'DRUSH_PERM_ERROR');

    provision_file()->unlink("sites/$url/private");
    provision_file()->symlink($site_storage . '/private', "sites/$url/private")
      ->succeed('Symlinked private files directory into place.')
      ->fail('Could not symlink private files directory into place.', 'DRUSH_PERM_ERROR');
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
    unset($chgrp["sites/$url/files/styles"]);
    unset($chgrp["sites/$url/private/config"]);
    unset($chgrp["sites/$url/private/config/active"]);
    unset($chgrp["sites/$url/private/config/staging"]);
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
    $chgrp["$site_storage/files/styles"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/private/config"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/private/config/active"] = d('@server_master')->web_group;;
    $chgrp["$site_storage/private/config/staging"] = d('@server_master')->web_group;;
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

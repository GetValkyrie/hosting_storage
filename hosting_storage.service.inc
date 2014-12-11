<?php
/**
 * @file
 * Provide the hosting service classes for the storage service.
 */

/**
 * Storage service base class. This provides most of the frontend functionality
 * for the storage services, so only minor overrides of this class should be
 * necessary.
 */
class hostingService_storage extends hostingService {
  public $service = 'storage';

  public function form(&$form) {
    parent::form($form);

    $form['storage_location'] = array(
      '#type' => 'textfield',
      '#title' => t('File storage location'),
      '#description' => t('The base directory within which all files directories will be stored.'),
      '#size' => 40,
      '#default_value' => isset($this->storage_location) ? $this->storage_location : '',
      '#maxlength' => 255,
    );

    $form['preinstall_script'] = array(
      '#type' => 'textfield',
      '#title' => t('Preinstall script'),
      '#description' => t('A path to a script to run before site installation. The site\'s node ID will be passed to this script as an argument.'),
      '#default_value' => '',
      '#size' => 40,
      '#maxlength' => 255,
    );

    $form['postinstall_script'] = array(
      '#type' => 'textfield',
      '#title' => t('Postinstall script'),
      '#description' => t('A path to a script to run after site installation. The site\'s node ID will be passed to this script as an argument.'),
      '#default_value' => '',
      '#size' => 40,
      '#maxlength' => 255,
    );

    $form['predelete_script'] = array(
      '#type' => 'textfield',
      '#title' => t('Predelete script'),
      '#description' => t('A path to a script to run before site deletion. The site\'s node ID will be passed to this script as an argument.'),
      '#default_value' => '',
      '#size' => 40,
      '#maxlength' => 255,
    );

    $form['postdelete_script'] = array(
      '#type' => 'textfield',
      '#title' => t('Postdelete script'),
      '#description' => t('A path to a script to run after site deletion. The site\'s node ID will be passed to this script as an argument.'),
      '#default_value' => '',
      '#size' => 40,
      '#maxlength' => 255,
    );
  }

  /**
   * Load associated values for the service.
   */
  public function load() {
    parent::load();
    $this->mergeData("SELECT storage_location, preinstall_script, postinstall_script, predelete_script, postdelete_script FROM {hosting_storage} WHERE vid = :vid AND nid = :nid", array(':vid' => $this->server->vid, ':nid' => $this->server->nid));
  }

  /**
   * Display the storage settings on the server node page.
   */
  public function view(&$render) {
    parent::view($render);

    $render['storage_location'] = array(
      '#type' => 'item',
      '#title' => t('Storage location'),
      '#markup' => filter_xss($this->storage_location),
    );

    if ($this->preinstall_script !== '') {
      $render['preinstall_script'] = array(
        '#type' => 'item',
        '#title' => t('Preinstall script'),
        '#markup' => filter_xss($this->preinstall_script),
      );
    }

    if ($this->postinstall_script !== '') {
      $render['postinstall_script'] = array(
        '#type' => 'item',
        '#title' => t('Postinstall script'),
        '#markup' => filter_xss($this->postinstall_script),
      );
    }

    if ($this->predelete_script !== '') {
      $render['predelete_script'] = array(
        '#type' => 'item',
        '#title' => t('Predelete script'),
        '#markup' => filter_xss($this->predelete_script),
      );
    }

    if ($this->postdelete_script !== '') {
      $render['postdelete_script'] = array(
        '#type' => 'item',
        '#title' => t('Postdelete script'),
        '#markup' => filter_xss($this->postdelete_script),
      );
    }
  }

  /**
   * Save the storage engine configuration into the database.
   */
  public function insert() {
    parent::insert();

    $id = db_insert('hosting_storage')
      ->fields(array(
        'vid' => $this->server->vid,
        'nid' => $this->server->nid,
        'storage_location' => $this->storage_location,
        'preinstall_script' => $this->preinstall_script,
        'postinstall_script' => $this->postinstall_script,
        'predelete_script' => $this->predelete_script,
        'postdelete_script' => $this->postdelete_script,
      ))
      ->execute();
  }

  /**
   * Save the storage engine configuration into the database.
   */
  public function update() {
    $this->delete_revision();
    $this->insert();
  }

  /**
   * Delete storage configuration from the database.
   */
  public function delete() {
    parent::delete();
    db_query('DELETE FROM {hosting_storage} WHERE nid = :nid', array(':nid' => $this->server->nid));
  }

  /**
   * Delete storage configuration for a given revision from the database.
   */
  public function delete_revision() {
    parent::delete();
    db_query('DELETE FROM {hosting_storage} WHERE vid = :nid', array(':nid' => $this->server->vid));
  }

  /**
   * Pass values to the provision backend.
   */
  public function context_options($task_type, $ref_type, &$task) {
    parent::context_options($task_type, $ref_type, $task);

    $task->context_options['storage_location'] = $this->storage_location;

    if ($this->preinstall_script !== '') {
      $task->context_options['preinstall_script'] = $this->preinstall_script;
    }
    if ($this->preinstall_script !== '') {
      $task->context_options['postinstall_script'] = $this->postinstall_script;
    }
    if ($this->preinstall_script !== '') {
      $task->context_options['predelete_script'] = $this->predelete_scrip;
    }
    if ($this->preinstall_script !== '') {
      $task->context_options['postdelete_script'] = $this->postdelete_script;
    }
  }

  /**
   * Import values from an existing context.
   *
   * @TODO: Implement this functionality.
   */
  public function context_import($context) {
    parent::context_import($context);
  }
}

/**
 * Class hostingService_storage_files
 *
 * Provide the files storage engine, which moves all Drupal files directories into a single location.
 */
class hostingService_storage_files extends hostingService_storage {
  public $type = 'files';

  /**
   * Hide the script fields, as the files storage backend doesn't require them.
   */
  public function form(&$form) {
    parent::form($form);

    $form['preinstall_script']['#access'] = FALSE;
    $form['postinstall_script']['#access'] = FALSE;
    $form['predelete_script']['#access'] = FALSE;
    $form['postdelete_script']['#access'] = FALSE;
    $form['storage_location']['#default_value'] = isset($this->storage_location) ? $this->storage_location : '/var/aegir/filestorage';
  }
}


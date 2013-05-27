<?php

/**
 * @file
 * Redhen Contact type classes.
 */


/**
 * The class used for model type entities
 */
class RedhenContactType extends Entity {

  public $name;
  public $label;
  public $locked;

  /**
   * Override parent constructor to set entity type.
   *
   * @param array $values
   *   Default entity values.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'redhen_contact_type');
  }

  /**
   * Override parent save method to ensure we have an email field for each type.
   */
  public function save() {
    $ret = parent::save();

    // Associate email field instance with contact.
    if ($ret) {
      // Ensure the field instance doesn't already exist; problem with features.
      $instance = field_info_instance('redhen_contact', REDHEN_CONTACT_EMAIL_FIELD, $this->name);
      if (!$instance) {
        $email_field = array(
          'field_name' => REDHEN_CONTACT_EMAIL_FIELD,
          'entity_type' => 'redhen_contact',
          'bundle' => $this->name,
          'label' => t('Email'),
        );
        field_create_instance($email_field);
      }
    }
  }

}

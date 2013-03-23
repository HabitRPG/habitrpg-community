<?php
/**
 * @file
 * Redhen Contact entity classses
 */

/**
 * The class used for contact entities.
 */
class RedhenContact extends Entity {

  public
    $first_name = '',
    $middle_name = '',
    $last_name = '',
    $contact_id = NULL,
    $uid = NULL,
    $author_uid = NULL,
    $redhen_state = REDHEN_STATE_ACTIVE,
    $type = '',
    $created = '',
    $updated = '';

  public function __construct(array $values = array()) {
    parent::__construct($values, 'redhen_contact');
  }

  /**
   * Override buildContent() to add contact properties.
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {

    $wrapper = entity_metadata_wrapper('redhen_contact', $this);
    $user = $wrapper->user->value();

    $content['redhen_state'] = array(
      '#theme' => 'redhen_property_field',
      '#label' => t('State'),
      '#items' => array(
        array(
          '#markup' => ($this->redhen_state == REDHEN_STATE_ACTIVE) ? t('Active') : t('Inactive')
        ),
      ),
      '#classes' => 'field field-label-inline clearfix',
    );

    if ($user) {
      $user_uri = entity_uri('user', $user);
      $content['user'] = array(
        '#theme' => 'redhen_property_field',
        '#label' => t('Drupal User'),
        '#items' => array(
          array(
            '#markup' => l($user->name, $user_uri['path'])
          ),
        ),
        '#classes' => 'field field-label-inline clearfix',
      );
    }

    return entity_get_controller($this->entityType)
      ->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Specifies the default label, which is picked up by label() by default.
   */
  protected function defaultLabel() {
    $type = redhen_contact_get_types($this->type);
    $wrapper = entity_metadata_wrapper('redhen_contact', $this);
    $name = $wrapper->full_name->value();
    return $name . ' (' . $type->label . ')';
  }

  /**
   * Specify URI
   */
  protected function defaultUri() {
    return array('path' => 'redhen/contact/' . $this->identifier());
  }

  /**
   * Return all emails and their properties in an array..
   *
   * @return array
   */
  public function email() {
    $wrapper = entity_metadata_wrapper($this->entityType(), $this);
    return $wrapper->email->value();
  }

  /**
   * Return all emails and their properties in an array..
   *
   * @return array
   */
  public function allEmail() {
    $emails = array();
    if (!empty($this->{REDHEN_CONTACT_EMAIL_FIELD})) {
      $emails = $this->{REDHEN_CONTACT_EMAIL_FIELD}[LANGUAGE_NONE];
    }
    return $emails;
  }

  /**
   * Determine if contact has a given email.
   *
   * @param $email
   *
   * @return bool
   */
  public function hasEmail($email) {
    if (!empty($this->{REDHEN_CONTACT_EMAIL_FIELD})) {
      foreach ($this->{REDHEN_CONTACT_EMAIL_FIELD}[LANGUAGE_NONE] as $contact_email) {
        if ($contact_email['value'] === $email) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Sets an email for a contact.
   *
   * @param $email
   * @param int $default
   * @param int $bulk
   * @param int $hold
   * @param int $label_id
   */
  public function setEmail($email, $default = 1, $bulk = 1, $hold = 0, $label_id = 0) {
    $this->{REDHEN_CONTACT_EMAIL_FIELD}[LANGUAGE_NONE][] = array(
      'value' => $email,
      'default' => $default,
      'bulk' => $bulk,
      'hold' => $hold,
      'label_id' => $label_id
    );
  }

  public function setUser() {
    return entity_get_controller($this->entityType)->setUser($this);
  }

  public function deleteUser($delete) {
    return entity_get_controller($this->entityType)->deleteUser($this, $delete);
  }

  /**
   * Set the redhen_state for a contact
   *
   * @param $state
   *  The REDHEN_STATE_* value to use
   */
  public function setState($state) {
    // allow other modules to act on a state change
    module_invoke_all('redhen_contact_set_state', $this->redhen_state, $state);
    $this->redhen_state = $state;
    return entity_get_controller($this->entityType)->save($this);
  }

}

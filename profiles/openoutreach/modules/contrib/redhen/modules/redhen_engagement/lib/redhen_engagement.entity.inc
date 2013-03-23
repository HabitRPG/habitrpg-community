<?php

/**
 * @file
 * RedhenEngagement entity classes
 */

/**
 * The class used for note entities.
 */
class RedhenEngagement extends Entity {

  public
    $engagement_id = NULL,
    $type = 'engagement',
    $description,
    $engagement_score,
    $contact_id,
    $source_entity_type,
    $source_entity_id,
    $author_uid,
    $created;

  public function __construct(array $values = array()) {
    parent::__construct($values, 'redhen_engagement');

    // New engagement. is_new might not be set so check for id
    if (!$this->engagement_id) {
      $this->created = REQUEST_TIME;
    }
  }

  /**
   * Override label() to add note label.
   */
  public function label() {
    return t('Engagement !id', array('!id' => $this->engagement_id));
  }

  /**
   * Specifify URI
   */
  protected function defaultUri() {
    return array('path' => 'redhen/engagement/' . $this->engagement_id);
  }

}

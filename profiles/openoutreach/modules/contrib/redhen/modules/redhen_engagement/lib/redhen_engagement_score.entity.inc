<?php

/**
 * The class used for note entities.
 */
class RedhenEngagementScore extends Entity {

  public $name;
  public $label;
  public $locked;
  public $score;

  public function __construct($values = array()) {
    parent::__construct($values, 'redhen_engagement_score');
  }

}

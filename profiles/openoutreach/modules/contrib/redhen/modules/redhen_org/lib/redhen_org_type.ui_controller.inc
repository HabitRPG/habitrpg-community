<?php

/**
 * Redhen Contact Type UI controller.
 */
class RedhenOrgTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage organization entity types, including adding and removing fields and the display of fields.';
    return $items;
  }

}

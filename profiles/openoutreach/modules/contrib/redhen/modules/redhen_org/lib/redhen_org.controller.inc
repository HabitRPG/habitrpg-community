<?php
/**
 * @file
 * The controller for the org entity containing the CRUD operations.
 */

/**
 * The controller class for orgs contains methods for the org CRUD
 * operations. The load method is inherited from the default controller.
 */
class RedhenOrgEntityController extends EntityAPIController {

  /**
   * Saves an org.
   *
   * @param RedhenOrg $org
   *   The full org object to save.
   *
   * @return
   *   The saved org object.
   */
  public function save($org) {
    $transaction = db_transaction();
    try {
      $org->updated = REQUEST_TIME;

      parent::save($org, $transaction);

      return $org;
    } catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
  }

  /**
   * Deletes multiple orgs by id.
   *
   * @param $org_ids
   *   An array of org IDs to delete.
   *
   * @return
   *   TRUE on success, FALSE otherwise.
   */
  public function delete($org_ids, DatabaseTransaction $transaction = NULL) {
    if (!empty($org_ids)) {
      $orgs = $this->load($org_ids, array());

      // Ensure the orgs can actually be deleted.
      foreach ((array) $orgs as $org_id => $org) {
        if (in_array(FALSE, module_invoke_all('redhen_org_can_delete', $org))) {
          unset($orgs[$org_id]);
        }
        else {
          module_invoke_all('redhen_entity_predelete', $org, 'redhen_org');
        }
      }

      $transaction = db_transaction();
      try {
        parent::delete($org_ids, $transaction);
      } catch (Exception $e) {
        if (isset($transaction)) {
          $transaction->rollback();
        }
        watchdog_exception($this->entityType, $e);
        throw $e;
      }
    }

    return TRUE;
  }

}

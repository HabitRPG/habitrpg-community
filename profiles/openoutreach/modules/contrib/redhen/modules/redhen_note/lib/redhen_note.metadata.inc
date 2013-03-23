<?php

/**
 * Controls metadata for Redhen notes
 *
 * Redhen utilizes custom property attributes to determine if a property
 * should be available as a filter on the listing page. These attributes are
 *   filter: TRUE to add as a filter.
 *   filter_operator: EFQ supported operators. Defaults to = or IN depending on value submitted
 *   field_type: textfield, select, etc.
 *   options list: callback that returns the options for this field.
 */
class RedhenNoteMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['created'] = array(
      'label' => t("Created"),
      'description' => t("The date the note was created."),
      'type' => 'date',
      'filter' => TRUE,
      'field_type' => 'textfield',
      'schema field' => 'created',
      'setter callback' => 'entity_property_verbatim_set'
    );
    $properties['updated'] = array(
      'label' => t("Updated"),
      'description' => t("The date the note was updated."),
      'type' => 'date',
      'schema field' => 'updated',
      'setter callback' => 'entity_property_verbatim_set'
    );

    // Entities.
    $properties['author'] = array(
      'label' => t("Author entity"),
      'type' => 'user',
      'description' => t("The author of the note."),
      'schema field' => 'author_uid',
    );

    $properties['entity'] = array(
      'label' => t("Host entity"),
      'type' => 'entity',
      'schema field' => 'entity',
      'description' => t("The host entity."),
      'getter callback' => 'redhen_note_property_host_get',
      'setter callback' => 'entity_property_verbatim_set',
    );

    return $info;
  }

}

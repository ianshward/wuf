<?php
// $Id:$

/**
 * @file
 * WufProcessor class.
 */

/**
 * Feeds processor plugin. Store weather data from Weather Underground WXCurrentObXML API.
 */
class WufProcessor extends FeedsDataProcessor {

  /**
   * Implementation of FeedsProcessor::process().
   */
  public function process(FeedsParserResult $parserResult, FeedsSource $source) {

    // Count number of created and updated nodes.
    $inserted  = $updated = 0;

    foreach ($parserResult->value['items'] as $item) {
      if (!($id = $this->existingItemId($item, $source)) || $this->config['update_existing']) {
        // Map item to a data record, feed_nid and timestamp are mandatory.
        $data = array();
        $data['feed_nid'] = $source->feed_nid;
        $data['timestamp'] = FEEDS_REQUEST_TIME;
        $data = $this->map($item, $data);

        // Save data.
        if ($id) {
          $data['id'] = $id;
          $this->handler()->update($data, 'id');
          $updated++;
        }
        else {
          $this->handler()->insert($data);
          $inserted++;
        }
      }
    }

    // Set messages.
    if ($inserted) {
      drupal_set_message(t('Created !number items.', array('!number' => $inserted)));
    }
    elseif ($updated) {
      drupal_set_message(t('Updated !number items.', array('!number' => $updated)));
    }
    else {
      drupal_set_message(t('There are no new items.'));
    }
  }
}
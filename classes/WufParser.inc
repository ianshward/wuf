<?php
// $Id: FeedsSyndicationParser.inc,v 1.8 2009/11/02 19:58:37 alexb Exp $

/**
 * Class definition for Common Syndication Parser.
 *
 * Parses RSS and Atom feeds.
 */
class WufParser extends FeedsParser {

  /**
   * Parses a raw string and returns a Feed object from it.
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    module_load_include('inc', 'wuf', 'wuf.parser');
    $result = wuf_parse($batch->getRaw());
    $batch->setItems($result['items']);    
  }
  
  /**
   * Return mapping sources.
   *
   * At a future point, we could expose data type information here,
   * storage systems like Data module could use this information to store
   * parsed data automatically in fields with a correct field type.
   */
  public function getMappingSources() {
    return array(
      'station_name' => array(
        'name' => t('Station Name'),
        'description' => t('The name of the station.'),
      ),
      'latitude' => array(
        'name' => t('Latitude'),
        'description' => t('Station latitude.'),
      ),
      'longitude' => array(
        'name' => t('Station longitude'),
        'description' => t('Station longitude.'),
      ),
      'temp_f' => array(
        'name' => t('Temperature - farenheit'),
        'description' => t('Station current temperature in farenheit.'),
      ),
      'observation_time' => array(
        'name' => t('Observation time'),
        'description' => t('Time of data observation.'),
      ),
     );
  }
}
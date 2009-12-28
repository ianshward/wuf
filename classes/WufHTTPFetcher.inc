<?php
// $Id:$

/**
 * @file
 * Home of the WufHTTPFetcher.
 */

/**
 * Fetches data via HTTP.
 */
class WufHTTPFetcher extends FeedsHTTPFetcher {
  /**
   * Fetch a resource via http.
   *
   * @param $resource
   *   A resource description of type FeedsResource.
   *
   * @return
   *   A string from the requested location if successful, or FALSE if not.
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    $code = $source_config['source'];
    $type = $source_config['type'];
    $airport_endpoint = 'http://api.wunderground.com/auto/wui/geo/WXCurrentObXML/index.xml';
    $personal_endpoint = 'http://api.wunderground.com/weatherstation/WXCurrentObXML.asp';
    
    if ($type == 'airport') {
      // TODO - is code always UPPER?
      $url = $airport_endpoint . '?query=' . $code;
    }
    else {
      $url = $personal_endpoint . '?ID=' . $code;
    }
    
    return new FeedsImportBatch($url);

    feeds_include_library('http_request.inc', 'http_request');
    if ($this->config['auto_detect_feeds']) {
      $result = http_request_get_common_syndication($url);
    }
    else {
      $result = http_request_get($url);
    }
    if ($result->code != 200) {
      throw new Exception(t('Download of @url failed with code !code.', array('@url' => $url, '!code' => $result->code)));
    }
    return new FeedsImportBatch($result->data, 'text/xml');
  }
  
  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['source'] = array(
      '#type' => 'textfield',
      '#title' => t('Station ID'),
      '#description' => t('Enter airport code or weather station ID.'),
      '#default_value' => isset($source_config['source']) ? $source_config['source'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Station type'),
      '#description' => t('Choose whether the station is an airport or personal weather station.'),
      '#default_value' => isset($source_config['type']) ? $source_config['type'] : '',
      '#required' => TRUE,
      '#options' => array('airport' => t('Airport'), 'personal' => t('Personal')),
    );
    return $form;
  }
}
<?php

function wuf_parse($string) {
  @ $xml = simplexml_load_string($string, NULL, LIBXML_NOERROR | LIBXML_NOWARNING | LIBXML_NOCDATA);
  // Got a malformed XML.
  if ($xml === FALSE || is_null($xml)) {
    return FALSE;
  }
  // TODO: limited set.  Add support for other fields on the feed.
  $parsed_source = $item = array();
  $item['station_name'] = "{$xml->display_location->full}";
  $item['observation_time'] = _wuf_parse_date("{$xml->observation_time_rfc822}");
  $item['latitude'] = "{$xml->display_location->latitude}";
  $item['longitude'] = "{$xml->display_location->longitude}";
  $item['temp_f'] = "{$xml->temp_f}";
  $parsed_source['items'][] = $item;
  return $parsed_source;
}

/**
 * Parse a date comes from a feed.
 *
 * @param $date_string
 *  The date string in various formats.
 * @return
 *  The timestamp of the string or the current time if can't be parsed
 */
function _wuf_parse_date($date_str) {
  $parsed_date = strtotime($date_str);
  if ($parsed_date === FALSE || $parsed_date == -1) {
    $parsed_date = _wuf_parse_w3cdtf($date_str);
  }
  return $parsed_date === FALSE ? time() : $parsed_date;
}

/**
 * Parse the W3C date/time format, a subset of ISO 8601.
 *
 * PHP date parsing functions do not handle this format.
 * See http://www.w3.org/TR/NOTE-datetime for more information.
 * Originally from MagpieRSS (http://magpierss.sourceforge.net/).
 *
 * @param $date_str
 *   A string with a potentially W3C DTF date.
 * @return
 *   A timestamp if parsed successfully or FALSE if not.
 */
function _wuf_parse_w3cdtf($date_str) {
  if (preg_match('/(\d{4})-(\d{2})-(\d{2})T(\d{2}):(\d{2})(:(\d{2}))?(?:([-+])(\d{2}):?(\d{2})|(Z))?/', $date_str, $match)) {
    list($year, $month, $day, $hours, $minutes, $seconds) = array($match[1], $match[2], $match[3], $match[4], $match[5], $match[6]);
    // Calculate the epoch for current date assuming GMT.
    $epoch = gmmktime($hours, $minutes, $seconds, $month, $day, $year);
    if ($match[10] != 'Z') { // Z is zulu time, aka GMT
      list($tz_mod, $tz_hour, $tz_min) = array($match[8], $match[9], $match[10]);
      // Zero out the variables.
      if (!$tz_hour) {
        $tz_hour = 0;
      }
      if (!$tz_min) {
        $tz_min = 0;
      }
      $offset_secs = (($tz_hour * 60) + $tz_min) * 60;
      // Is timezone ahead of GMT?  If yes, subtract offset.
      if ($tz_mod == '+') {
        $offset_secs *= -1;
      }
      $epoch += $offset_secs;
    }
    return $epoch;
  }
  else {
    return FALSE;
  }
}
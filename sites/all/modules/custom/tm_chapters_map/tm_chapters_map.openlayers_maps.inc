<?php
/**
 * @file
 * tm_chapters_map.openlayers_maps.inc
 */

/**
 * Implements hook_openlayers_maps().
 */
function tm_chapters_map_openlayers_maps() {
  $export = array();

  $openlayers_maps = new stdClass();
  $openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'chapters_map';
  $openlayers_maps->title = 'Chapters Map';
  $openlayers_maps->description = 'This is a map of Chapters.';
  $openlayers_maps->data = array(
    'width' => 'auto',
    'height' => '500px',
    'image_path' => 'sites/all/modules/contrib/openlayers/themes/default_dark/img/',
    'css_path' => 'sites/all/modules/contrib/openlayers/themes/default_dark/style.css',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '4.921874991931841, 19.973348789432055',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_geofield' => array(),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 0,
        'zoomBoxEnabled' => 1,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_panzoombar' => array(
        'zoomWorldIcon' => 1,
        'panIcons' => 1,
      ),
      'openlayers_behavior_popup' => array(
        'layers' => array(),
        'panMapIfOutOfView' => 0,
        'keepInMap' => 1,
        'zoomToPoint' => 0,
        'zoomToCluster' => 0,
      ),
      'openlayers_behavior_zoomtomaxextent' => array(),
    ),
    'default_layer' => 'tm_osm_stamen',
    'layers' => array(
      'tm_osm_stamen' => 'tm_osm_stamen',
      'chapters_map_openlayers_1' => 'chapters_map_openlayers_1',
    ),
    'layer_weight' => array(
      'openlayers_geojson_picture_this' => '0',
      'openlayers_kml_example' => '0',
      'geofield_formatter' => '0',
      'chapters_map_openlayers_1' => '0',
    ),
    'layer_styles' => array(
      'chapters_map_openlayers_1' => 'default_marker_red',
      'geofield_formatter' => '0',
      'openlayers_kml_example' => '0',
      'openlayers_geojson_picture_this' => '0',
    ),
    'layer_styles_select' => array(
      'chapters_map_openlayers_1' => 'default_marker_gold',
      'geofield_formatter' => '0',
      'openlayers_kml_example' => '0',
      'openlayers_geojson_picture_this' => '0',
    ),
    'layer_styles_temporary' => array(
      'chapters_map_openlayers_1' => '0',
      'geofield_formatter' => '0',
      'openlayers_kml_example' => '0',
      'openlayers_geojson_picture_this' => '0',
    ),
    'layer_activated' => array(
      'chapters_map_openlayers_1' => 'chapters_map_openlayers_1',
      'geofield_formatter' => 0,
      'openlayers_kml_example' => 0,
      'openlayers_geojson_picture_this' => 0,
    ),
    'layer_switcher' => array(
      'chapters_map_openlayers_1' => 0,
      'geofield_formatter' => 0,
      'openlayers_kml_example' => 0,
      'openlayers_geojson_picture_this' => 0,
    ),
    'projection' => 'EPSG:3857',
    'displayProjection' => 'EPSG:4326',
    'styles' => array(
      'default' => 'default_marker_black',
      'select' => 'default',
      'temporary' => 'default',
    ),
  );
  $export['chapters_map'] = $openlayers_maps;

  return $export;
}

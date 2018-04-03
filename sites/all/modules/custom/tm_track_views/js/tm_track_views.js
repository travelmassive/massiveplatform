jQuery(document).ready(function($) {
  var tm_track_view_ajax = function(entity_id, entity_type) {
    var tm_track_views_path = Drupal.settings.tm_track_views.path;
    $.ajax({
      type: 'GET',
      url: Drupal.settings.basePath + tm_track_views_path + '/' + entity_type + '/' + entity_id,
      dataType: 'json'
    });
  }
  var tm_track_view_entity_id = Drupal.settings.tm_track_views.entity_id;
  var tm_track_view_entity_type = Drupal.settings.tm_track_views.entity_type;

  tm_track_view_ajax(tm_track_view_entity_id, tm_track_view_entity_type);
});
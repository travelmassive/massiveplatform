(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){
        
	var video_container_sizes = [1, 0.75, 0.55];
	var video_container_index = 0;
	var original_width = null;  

	console.log("here");

	// Reduce video size
	tm_livestream_smaller_video = function() {

		if (original_width == null) {
			original_width = jQuery("#livestream_video_container").width();
		}

		if (video_container_index < video_container_sizes.length - 1) {
			video_container_index++;
		}

		var new_width = Math.round(original_width * video_container_sizes[video_container_index]);
		jQuery("#livestream_video_container").width(new_width);

	}

	// Increase video size
	tm_livestream_larger_video = function() {

		if (original_width == null) {
			original_width = jQuery("#livestream_video_container").width();
		}

		if (video_container_index >= 1) {
			video_container_index--;
		}

		var new_width = Math.round(original_width * video_container_sizes[video_container_index]);
		jQuery("#livestream_video_container").width(new_width);
	}

});})(jQuery, Drupal, this, this.document);

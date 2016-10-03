(function($) {

	// video id
	var cover_video_id = null;
	var cover_image_height = 453;

	// defer youtube api until jquery loaded
	var YTdeferred = $.Deferred();
	var timeoutCheckMuted = null;
	var cover_player = null;
	window.onYouTubeIframeAPIReady = function() {
	   YTdeferred.resolve(window.YT);
	};

	$(document).ready(function() {

		// Play a cover video
		$('.trilithon-header .media .cover img').fadeTo('slow', 0.5);

		// Get cover video id
		if (Drupal.settings.tm_theme.cover_video_id !== 'undefined') {
			cover_video_id = Drupal.settings.tm_theme.cover_video_id;
		}

		// Deferred youtube loader
		YTdeferred.done(function(YT) {

			// make sure we have a cover video
			if (cover_video_id == null) {
				return;
			}

			// replace cover image with video
			$('.trilithon-header .media .cover').html('<div id="cover_video" style="margin-bottom: -9px;"></div>').show();

			// caculate width of cover video
			var video_width = calcVideoCoverWidth();

			// load player
			cover_player = new YT.Player('cover_video', {
			height: cover_image_height,
			width: video_width,
			videoId: cover_video_id,
			playerVars: { 
				'autoplay': 1, 
				'autohide': 1,
				'color': 'white',
				'controls': 1,
				'disablekb': 0,
				'loop': 1,
				'modestbranding': 1,
				'playlist': cover_video_id
			},
			events: {
				'onReady': onPlayerReady
				}
			});

		});

	 	// load youtube API
		var tag = document.createElement('script');
		tag.src = "https://www.youtube.com/iframe_api";
		var firstScriptTag = document.getElementsByTagName('script')[0];
		firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);

	});
	
	// event handler on ready
	onPlayerReady = function(event) {

		// resize
		resizeCoverVideo();

		// remember from cookie if it was muted
		cover_video_muted_cookie = getPlayerMutedCookie();
		if (cover_video_muted_cookie != false) {
			event.target.mute();
		}

		checkIfPlayerMuted();
	}

	checkIfPlayerMuted = function() {
		timeoutCheckMuted = setTimeout(function() {
			if (cover_player != null) {
				if (!cover_player.isMuted()) {
					rememberPlayerMutedCookie(false);
				} else {
					rememberPlayerMutedCookie(true)
				}
			}
			clearTimeout(timeoutCheckMuted);
			checkIfPlayerMuted();
		}, 5000); 
	}

	rememberPlayerMutedCookie = function(value) {
		var cookie = ['cover_video_is_muted=', value, '; domain=.', window.location.host.toString(), '; path=/;'].join('');
		document.cookie = cookie;
	}

	getPlayerMutedCookie = function() {
		var result = document.cookie.match(new RegExp('cover_video_is_muted=([^;]+)'));
		result && (result = JSON.parse(result[1]));
		return result;
	}

	resizeCoverVideoTimeout = function() {
		setTimeout(function(){
			resizeCoverVideo();
		}, 50); 
	}

	calcVideoCoverWidth = function() {
	  
		var actions_width = $(".actions").outerWidth();
		var header_width = $('.trilithon-header.contained').width();

		if ((actions_width + 32) < header_width) {
			var video_width = (header_width - actions_width);
		} else {
			var video_width = header_width;
		}
		return video_width;
	}

	calcVideoCoverHeight = function() {
	  
		var actions_width = $(".actions").outerWidth();
		var header_width = $('.trilithon-header.contained').width();

		if ((actions_width + 32) < header_width) {
			var video_height = cover_image_height;
		} else {
			var video_height = Math.round(cover_image_height / 2);
		}
		return video_height;
	}

	resizeCoverVideo = function() {
		video_width = calcVideoCoverWidth();
		video_height = calcVideoCoverHeight();
		$("#cover_video").width(video_width);
		$("#cover_video").height(video_height);
	}

	// listen for event
	window.addEventListener('resize', resizeCoverVideoTimeout, false);

})(jQuery);

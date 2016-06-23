<style>
.tm-status-update-container { padding-top: 12px; }
.tm-status-update-image-container { left: 8px; top: 0px; width: 100px;}
.tm-status-update-image { width: 48px; border-radius: 4px; }
.tm-status-update-text-container { position: absolute; left: 72px; right: 16px; top: 12px; }
.tm-status-update-post-actions { text-align: right; margin-top: 6px; }
#tm-status-update-post-as { margin-right: 8px; }
#charNum { font-size: 14px; color: #888; margin-right: 8px; position: absolute; top: -16px; right: 8px;}
#tm-status-update-feedback-container { font-size: 14px; margin-bottom: -12px; min-height: 24px;}
#tm-status-update-location-text { font-size: 14px; color: #213040; text-decoration: underline; }
#tm-status-update-feedback-loading-image { margin-top: 6px; padding-right: 8px; }
#tm-status-update-text { resize: none; }
</style>

<script>
var tm_update_post_as_images = __POST_AS_IMAGES__;
</script>

<div class="tm-status-update-container">

	<span class="tm-status-update-image-container">
		<div style="float: left; height: 56px; width: 0px;"></div>
		<img id="tm-status-update-poster-image" class="tm-status-update-image" src="__MEMBER_IMAGE_SRC__">
	</span>

	<div class="tm-status-update-text-container">
		<textarea id="tm-status-update-text" placeholder='__PLACEHOLDER_TEXT__' onkeyup='tm_user_status_update_on_key_press(this, 250);'></textarea>
	</div>

	<div style="clear: both;"></div>

	<div class="tm-status-update-post-actions">
		
		<span id="charNum">&nbsp;</span>
		<span id='tm-status-update-location-field' class='tm-status-update-location' style='display: none;'><span id='tm-status-update-location-text' onClick='tm_user_status_enter_location()'>Add location</span></span>
		<select id="tm-status-update-post-as" name="tm-status-update-post-as" style="margin-bottom: 8px;">
			__POST_AS_OPTIONS__
		</select>
		<a id="tm-status-update-post" class='bttn bttn-secondary bttn-m edit' href='javascript:void(0);' onClick='tm_user_status_update();'>Post</a>
	</div>
	
</div>

<div id="tm-status-update-link-preview-container">
	<div id="tm-status-update-feedback-container">
		<img id="tm-status-update-feedback-loading-image" style="display:none;" src='/sites/all/themes/tm/images/load-more-ajax-loader.gif'>
		<span id="tm-status-update-feedback-text"></span>
	</div>
	<div id="tm-status-update-link-preview-html"></div>
</div>

<div style="clear: both;"></div>


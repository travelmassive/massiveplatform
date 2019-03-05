<style>
.tm-status-update-form-container { padding-top: 12px; }
.tm-status-update-image-container { left: 8px; top: 0px; width: 100px; }
.tm-status-update-image { width: 48px; border-radius: 50%; }
.tm-status-update-text-container { position: absolute; left: 72px; right: 16px; top: 12px; }
.tm-status-update-post-actions { text-align: right; margin-top: 6px; }
#tm-status-update-post-button { margin-left: 8px; }
#charNum { font-size: 14px; color: #888; margin-right: 8px; position: absolute; top: -16px; right: 8px; }
#tm-status-update-feedback-container { font-size: 14px; margin-bottom: -12px; min-height: 28px; }
#tm-status-update-location-text { font-size: 14px; color: #213040; text-decoration: underline; }
#tm-status-update-feedback-loading-image { margin-top: 6px; padding-right: 8px; }
#tm-status-update-text { resize: none; }
#tm-status-update-location-container { float: left; min-width: 150px; min-height: 2rem; text-align: left; margin-left: 56px; }
@media only screen and (max-width: 650px) {
	#tm-status-update-location-container { float: right; min-width: 300px; text-align: right; margin-left: 0px; }
}
</style>

<script>
var tm_update_post_as_images = __POST_AS_IMAGES__;
</script>

<div class="tm-status-update-form-container">

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
		<div id="tm-status-update-location-container">
			<span id='tm-status-update-location-field' style='display: none;'>
				<span id='tm-status-update-location-text' onClick='tm_user_status_enter_location()'>Add location</span>
			</span>
		</div>
		<select id="tm-status-update-post-as" name="tm-status-update-post-as" style="margin-bottom: 8px;">
			__POST_AS_OPTIONS__
		</select>
		<span id="tm-status-update-post-button">
			<a id="tm-status-update-post" class='bttn bttn-secondary bttn-m edit' href='javascript:void(0);' onClick='tm_user_status_update();'>Post</a>
		</span>
	</div>
	
</div>

<div id="tm-status-update-link-preview-container">
	<div id="tm-status-update-feedback-container">
		<img id="tm-status-update-feedback-loading-image" style="display:none;" style="width: 24px; height: 16px;" src='/sites/all/themes/tm/images/load-more-ajax-loader-2.gif'>
		<span id="tm-status-update-feedback-text"></span>
	</div>
	<div id="tm-status-update-link-preview-html"></div>
</div>

<div style="clear: both;"></div>



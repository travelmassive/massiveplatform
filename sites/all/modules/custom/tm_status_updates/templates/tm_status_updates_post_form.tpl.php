<style>
.tm-status-update-container { padding-top: 12px; }
.tm-status-update-image-container { left: 8px; top: 0px; width: 100px; }
.tm-status-update-image { width: 48px; border-radius: 4px; }
.tm-status-update-text-container { position: absolute; left: 72px; right: 16px; top: 12px; }
.tm-status-update-actions { text-align: right; margin-top: 8px; }
.tm-status-update-feedback { text-align: right; }
#charNum { font-size: smaller; color: #888; margin-right: 8px; }
</style>

<div class="tm-status-update-container">

	<span class="tm-status-update-image-container">
		<img class="tm-status-update-image" src="__MEMBER_IMAGE_SRC__">
	</span>

	<div class="tm-status-update-text-container">
		<textarea id="tm-status-update-text" placeholder='__PLACEHOLDER_TEXT__' onkeyup='tm_user_status_update_on_key_press(this, 250);'></textarea>
	</div>

	<div class="tm-status-update-actions">
		<span id="charNum">&nbsp;</span>
		<select id="tm-status-update-post-as" name="tm-status-update-post-as" style="margin-bottom: 8px;">
			__POST_AS_OPTIONS__
		</select>
		<a id="tm-status-update-post" class='bttn bttn-secondary bttn-m edit' href='javascript:void(0);' onClick='tm_user_status_update(__MEMBER_UID__);'>Post
		</a>

	</div>
	<div class="tm-status-update-feedback"
		
	</div>
</div>

<div class="tm-status-update-preview" style="display: none;"></div>
<div class="tm-status-update-error-message" style="display: none;"></div>
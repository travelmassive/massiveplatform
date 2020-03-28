<div class='tm-list-items'>
<ul class='tm-list-items-list featured'>
__LIST_ITEM__
</ul>
</div>

<style>

/* Featured */

.tm_list_featured .jqi { 
	margin-top: 0px;
	width: 90% !important;
	max-width: 1150px;
	margin-left: auto !important;
	margin-right: auto !important;
	left: 0 !important;
	right: 0 !important;
	background: none !important;
	border: none !important;
	padding: 0 !important;
}

.tm_list_featured .jqimessage {
	padding: 0 !important;
}

.tm_list_featured .jqiform, .tm_list_featured .jqistate { 
	background: none !important;
}

.tm_list_featured .tm-list-item-actions-container {
	display: none;
}

.tm-list-items-list {
	filter: blur(4px);
}

.tm-list-items-list.featured {
	filter: blur(0px);
}


</style>

<script>
// position below heading
title_offset = jQuery('.tm-list-intro-title').offset().top;
title_height = jQuery('.tm-list-intro-title').height();
offset = (title_offset + title_height + 16);

// set featured item position
jQuery('.tm_list_featured .jqi').css({ top: offset + "px" });

// scroll to top
window.scroll({
  top: title_offset - 64, 
  left: 0, 
  behavior: 'smooth'
});
</script>
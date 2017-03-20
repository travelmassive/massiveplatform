 <?php
/**
 * @file
 * Template for the tm_organization_sticker field, which is custom DS preprocess field
 */
if ($is_subscriber): ?>
<a class="organization-sticker organization-sticker-icon" href="<?php print url($custom_url); ?>" style="background-color: #ffffff;">
  <span class='organization-sticker-icon-text' style='color: #222222;'><?php print $label;?></span>
</a>
<?php endif; ?>


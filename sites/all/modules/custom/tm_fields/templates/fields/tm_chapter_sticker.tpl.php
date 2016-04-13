 <?php
/**
 * @file
 * Template for the tm_chapter_sticker field, which is custom DS preprocess field
 */

if ($chapter_id != null): ?>
<a class="chapter-sticker" href="<?php print url('node/' . $chapter_id); ?>" style="background-color: <?php print $color; ?>">
  <?php print $shortcode; ?>
</a>
<?php endif; ?>
<?php if (($chapter_id == null) && ($custom_url != null)): ?>
<a class="chapter-sticker chapter-sticker-icon" href="<?php print url($custom_url); ?>" style="background-color: <?php print $color; ?>">
  <span class='chapter-sticker-icon-text'><?php print $shortcode; ?></span>
</a>
<?php endif; ?>


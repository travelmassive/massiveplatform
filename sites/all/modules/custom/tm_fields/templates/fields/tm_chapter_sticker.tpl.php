 <?php
/**
 * @file
 * Template for the tm_chapter_sticker field, which is custom DS preprocess field
 */
?>

<a class="chapter-sticker" href="/node/<?php print $chapter_id; ?>" style="background-color: <?php print $color; ?>">
  <?php print $shortcode; ?>
</a>
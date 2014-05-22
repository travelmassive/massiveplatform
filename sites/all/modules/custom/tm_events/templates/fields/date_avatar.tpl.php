<?php
/**
 * @file
 * Template for the tm_date_avatar field, which is custom DS preprocess field
 */
?>
 
<div class="avatar">
  <time datetime="<?php print $date->format('c'); ?>">
    <span class="month"><?php print $date->format('m'); ?></span> 
    <span class="day"><?php print $date->format('d'); ?></span> 
    <span class="year"><?php print $date->format('Y'); ?></span>
  </time>
</div>
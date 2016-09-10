<?php
/**
 * @file
 * Template for the tm_date_avatar field, which is custom DS preprocess field
 */
?>

<?php
// check if single or multi-day event
if ($date->format('Y-M-d') == $date2->format('Y-M-d')) {
?>
<div class="avatar">
  <time datetime="<?php print $date->format('c'); ?>" class="badge-cal">
    <span class="month"><?php print $date->format('M'); ?></span> 
    <span class="day"><?php print $date->format('d'); ?></span> 
    <span class="year"><?php print $date->format('Y'); ?></span>
  </time>
</div>
<?php } else { ?>
	<div class="avatar">
	  <time datetime="<?php print $date->format('c'); ?>" class="badge-cal">
	    <span class="month"><?php print $date->format('M'); if (($date->format('M') != ($date2->format('M')))) { print "-" . $date2->format('M'); }?></span> 
	    <span class="day" style="font-size: 1.5rem;"><?php print $date->format('j'); ?>-<?php print $date2->format('j'); ?></span> 
	    <span class="year"><?php print $date->format('Y'); ?></span>
	  </time>
	</div>
<?php } ?>
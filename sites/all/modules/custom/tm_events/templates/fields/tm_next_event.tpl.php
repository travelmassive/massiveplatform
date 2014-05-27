<?php
/**
 * @file
 * Template for the tm_next_event field, which is custom DS field
 */
?>
<?php if (isset($time_until)): ?>
<span class="meta event"><?php print t('Event in @time.', array('@time' => $time_until));?></span>
<?php else: ?>
<span class="meta event"><?php print t('No event scheduled yet.'); ?></span>
<?php endif; ?>
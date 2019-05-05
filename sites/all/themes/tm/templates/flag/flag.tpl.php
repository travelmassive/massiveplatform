<?php

/**
 * @file
 * Default theme implementation to display a flag link, and a message after the action
 * is carried out.
 *
 * Available variables:
 *
 * - $flag: The flag object itself. You will only need to use it when the
 *   following variables don't suffice.
 * - $flag_name_css: The flag name, with all "_" replaced with "-". For use in 'class'
 *   attributes.
 * - $flag_classes: A space-separated list of CSS classes that should be applied to the link.
 *
 * - $action: The action the link is about to carry out, either "flag" or "unflag".
 * - $status: The status of the item; either "flagged" or "unflagged".
 *
 * - $link_href: The URL for the flag link.
 * - $link_text: The text to show for the link.
 * - $link_title: The title attribute for the link.
 *
 * - $message_text: The long message to show after a flag action has been carried out.
 * - $message_classes: A space-separated list of CSS classes that should be applied to
 *   the message.
 * - $after_flagging: This template is called for the link both before and after being
 *   flagged. If displaying to the user immediately after flagging, this value
 *   will be boolean TRUE. This is usually used in conjunction with immedate
 *   JavaScript-based toggling of flags.
 * - $needs_wrapping_element: Determines whether the flag displays a wrapping
 *   HTML DIV element.
 *
 * Template suggestions available, listed from the most specific template to
 * the least. Drupal will use the most specific template it finds:
 * - flag--name.tpl.php
 * - flag--link-type.tpl.php
 *
 * NOTE: This template spaces out the <span> tags for clarity only. When doing some
 * advanced theming you may have to remove all the whitespace.
 */
?>

<?php
 // allow $flag_upvote_class
 if (!isset($flag_upvote_class)) {
  $flag_upvote_class = "";
 }
?>

<?php if (isset($event_flag) && $event_flag == "show_closed") : ?>

<li class="<?php print $flag_wrapper_classes; ?>">
  <span class="follow bttn bttn-secondary bttn-m disabled" rel="nofollow">Past Event </span>
</li>

<?php elseif (isset($event_flag) && $event_flag == "show_not_approved") : ?>

<li class="<?php print $flag_wrapper_classes; ?>">

<?php
  $who_flagged = flag_get_entity_flags("user", $user->uid, "approval_requested_by_user");
  if (sizeof($who_flagged) > 0): ?>
  <span onClick="jq_alert('This event is for <?php print(tm_users_get_approved_member_label("plural"));?>', ' Please wait for your account to be reviewed.<br>If you need assistance please <a href=\'/contact\'>contact us</a>.');" class="follow bttn bttn-secondary bttn-m <?php if ($status == 'flagged'): ?>on<?php endif; ?> <?php print $flag_classes ?>" rel="nofollow"><?php print $link_text; ?></span>
<?php else: ?>
  <span onClick="jq_unapproved_member_event_register('<?php print(tm_users_get_approved_member_label("single"));?>', '<?php print(tm_users_get_approved_member_label("plural"));?>');" class="follow bttn bttn-secondary bttn-m <?php if ($status == 'flagged'): ?>on<?php endif; ?> <?php print $flag_classes ?>" rel="nofollow"><?php print $link_text; ?></span>
<?php endif; ?>    
</li>

<?php elseif (!isset($hide_flag) || $hide_flag == FALSE) : ?>

<li class="<?php print $flag_wrapper_classes; ?>">
	<?php if ($link_href): ?>
    <a href="<?php print $link_href; ?>" title="<?php print $link_title; ?>" class="follow bttn bttn-secondary bttn-m <?php if ($status == 'flagged'): ?>on<?php endif; ?> <?php print $flag_classes ?> <?php print $flag_upvote_class ?>" rel="nofollow"><span><?php print $link_text; ?></span></a>
  <?php else: ?>
    <div class ="<?php print $flag_button_class; ?>"><span class="<?php print $flag_classes ?>"><?php print $link_text; ?></span></div>
  <?php endif; ?>
</li>

<?php endif; ?>

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

// set in tm_event_signup_preprocess_flag
if (isset($do_not_display_flag)) {
  return;
}

// allow $flag_upvote_class
if (!isset($flag_upvote_class)) {
  $flag_upvote_class = "";
}

// default button labels
$ticket_label = "Ticket";
$donate_label = "Donate";

global $conf;
if (isset($conf["tm_checkout_event_button_label_ticket"])) {
  $ticket_label = $conf["tm_checkout_event_button_label_ticket"];
}

if (isset($conf["tm_checkout_event_button_label_donate"])) {
  $donate_label = $conf["tm_checkout_event_button_label_donate"];
}

?>
<?php if (isset($event_flag) && $event_flag == "show_checkout") : ?>
  
<li class="<?php print $flag_wrapper_classes; ?>">
  <a href='/checkout/event/<?php print($event_id);?>'><span class="follow checkout-event bttn bttn-secondary bttn-m" rel="nofollow"><?php print $ticket_label; ?></span></a>
</li>

<?php elseif (isset($event_flag) && $event_flag == "show_donate") : ?>
  
<li class="<?php print $flag_wrapper_classes; ?>">
  <a href='/checkout/event/<?php print($event_id);?>'><span class="follow donate-event bttn bttn-secondary bttn-m" rel="nofollow"><?php print $donate_label; ?></span></a>
</li>

<?php elseif (isset($event_flag) && $event_flag == "show_external_rsvp_closed") : ?>

<li class="<?php print $flag_wrapper_classes; ?>">
  <span class="past-event bttn bttn-secondary bttn-m disabled" rel="nofollow">Past Event</span>
  <style>
    .actions-menu .external { display: none; }
  </style>
</li>

<?php elseif (isset($event_flag) && $event_flag == "show_closed") : ?>

<li class="<?php print $flag_wrapper_classes; ?>">
  <span class="past-event bttn bttn-secondary bttn-m disabled" rel="nofollow">Past Event</span>
</li>

<?php elseif (isset($event_flag) && $event_flag == "show_feedback") : ?>

<li class="<?php print $flag_wrapper_classes; ?>">
  <a href="javascript:jq_net_promoter_score('this event');"><span class="follow bttn bttn-secondary bttn-m" rel="nofollow">Feedback</span></a>
</li>

<?php elseif (isset($event_flag) && $event_flag == "show_geoblocked") : ?>

<?php 
$flag_event = node_load($event_id);
$geoblock_signups = $flag_event->field_event_geoblock_signups[LANGUAGE_NONE][0]['value'];
$geoblocked_message = "This event is for <b>local chapter members</b>.";
if ($geoblock_signups == "same_country_as_event") {
  $event_country_name = tm_events_get_event_country_name($flag_event);
  $geoblocked_message = "This event is for members in <b>" . $event_country_name . "</b>.";
  if ($event_country_name == "United States") {
     $geoblocked_message = "This event is for members in the <b>United States</b>.";
  }
}
?>

<li class="<?php print $flag_wrapper_classes; ?>">
  <span onClick="jq_alert('Sorry, you cannot register for this event', ' <?php print($geoblocked_message);?><br>If you need assistance, please <a href=\'/events/message/<?php print($event_id);?>\'>contact the organizers</a>.');" class="follow bttn bttn-secondary bttn-m <?php if ($status == 'flagged'): ?>on<?php endif; ?> <?php print $flag_classes ?>" rel="nofollow"><?php print $link_text; ?></span>
</li>

<?php elseif (isset($event_flag) && $event_flag == "show_not_approved") : ?>

<li class="<?php print $flag_wrapper_classes; ?>">

<?php
  $who_flagged = flag_get_entity_flags("user", $user->uid, "approval_requested_by_user");
  if (sizeof($who_flagged) > 0): ?>
  <span onClick="jQuery('.flag').unbind('click'); jq_alert('This event is for <?php print(tm_users_get_approved_member_label("plural"));?>', ' Please wait for your account to be reviewed.<br>If you need assistance please <a href=\'/contact\'>contact us</a>.');" class="follow bttn bttn-secondary bttn-m <?php if ($status == 'flagged'): ?>on<?php endif; ?> <?php print $flag_classes ?>" rel="nofollow"><?php print $link_text; ?></span>
<?php else: ?>
  <span onClick="jQuery('.flag').unbind('click'); jq_unapproved_member_event_register('<?php print(tm_users_get_approved_member_label("single"));?>', '<?php print(tm_users_get_approved_member_label("plural"));?>');" class="follow bttn bttn-secondary bttn-m <?php if ($status == 'flagged'): ?>on<?php endif; ?> <?php print $flag_classes ?>" rel="nofollow"><?php print $link_text; ?></span>
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

<?php
/**
 * @file
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */
global $user;
?>

<?php
  // TM_SUBSCRIPTIONS_USER CTA
  // check that module is enabled and user is logged in
  if ($user->uid > 0) {
    if (module_exists("tm_subscriptions_user")) {
      $subscription_cta = tm_subscriptions_user_cta_banner($user->uid);
      if ($subscription_cta != "") {
        // print CTA
        print $subscription_cta;
      }
      
    }
  }
?>

<div class="tm-front-page-before-stats"></div>

<section id="welcome">
	<header class="hd" style="margin-top: 3em;">
    <?php
      $user_first_name = "Guest";
      if (user_is_logged_in()) { 
        $user_loaded = user_load($user->uid);
        $user_first_name = $user_loaded->field_user_first_name[LANGUAGE_NONE][0]['value'];
      }
    ?>
		<h1 class="prime-title"><?php print (str_replace("__FIRST_NAME__", $user_first_name, $homepage_title)); ?></h1>
		<p class="subtitle"><?php print ($homepage_description) ?></p>
	</header>
  
	<div class="bd">
    <?php
      // if tm_frontpage_beta_message is set, show message instead of the stats block
      if (variable_get('tm_frontpage_launch_message', NULL) != null) {
        print "<span class='beta_message'>" . (variable_get('tm_frontpage_launch_message', NULL)) . "</span>";
      }
      else { 
        print render($homepage_stats_block);
      }
    ?>
  	<?php if (!$user->uid): ?>
  	<p class="cta-wrapper"><?php print l(t('Join now'), 'user/register', array('attributes' => array('class' => 'bttn bttn-secondary bttn-xl'))) ?></p>
  	<?php endif; ?>
	</div>
</section>

<div class="tm-front-page-after-stats"></div>

<?php if (!empty($homepage_upcoming_events)): ?>
<section id="upcoming-events">
  <header class="hd">
    <h1 class="prime-title"><?php print check_plain($homepage_upcoming_events['title']) ?></h1>
  </header>
  <?php print $homepage_upcoming_events['content']; ?>
</section>
<?php endif; ?>

<div class="tm-front-page-after-events"></div>
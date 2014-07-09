<?php
/**
 * @file
 * @author Daniel da Silva (daniel.silva@flipside.org)
 */
global $user;
?>

<section id="homepage">
	<header>
		<h1 class="prime-title"><?php print check_plain($homepage_title) ?></h1>
		<p class="subtitle"><?php print check_plain($homepage_description) ?></p>
		<?php if ($homepage_stats_block) : ?>
		  <?php print render($homepage_stats_block); ?>
		<?php endif; ?>
		<?php if (!$user->uid): ?>
		<p class="cta-wrapper"><a href="" class="bttn bttn-secondary bttn-xl"><?php print t('Join'); ?></a></p>
		<?php endif; ?>
	</header>
	Bloco do Olaf!
</section>

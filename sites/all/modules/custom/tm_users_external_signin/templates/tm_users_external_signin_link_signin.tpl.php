<?php
/**
 * @file tm_users_external_signin_link_signin.tpl.php
 * 
 * @author Daniel da Silva (daniel.silva@flipside.org)
 * 
 * Available variables:
 * $url    The url the user has to follow to login
 */
?>

<p style="margin-bottom: 0.8em;">
	<a href="<?php print $url ?>" class="twitter-login" style="width: 200px;"><?php print t('Sign in with Twitter') ?></a>
</p>
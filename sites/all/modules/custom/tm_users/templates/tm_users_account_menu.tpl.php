<?php global $user;
// Only a loaded user has values for the fields.
$loaded = user_load($user->uid);
if (empty($loaded->field_avatar)) {
  // It is not possible to get an imagefield default value
  // using the standard default function. Load the file instead.
  $field = field_info_field('field_avatar');
  $default = file_load($field['settings']['default_image']);
  $img_uri = $default->uri;
}
else {
  $img_uri = $loaded->field_avatar[LANGUAGE_NONE][0]['uri'];
}

$image = theme('image_style', array(
  'style_name' => 'avatar',
  'path' => $img_uri,
  'alt' => 'user image',
  'title' => 'The user image',
));
?>

<h2>
  <a class="toggle" href="#account-menu-blk" data-dropd-toggle>
    <span class="hide"><?= t('Account'); ?></span>
    <?php if ($user->uid) : ?>
    <span class="avatar"><?php print $image; ?></span>
    <?php endif; ?>
  </a>
</h2>
<div id="account-menu-blk" class="inner dropd dropd-right" data-dropd>

  <?php if ($user->uid) : ?>
    <ul class="dropd-menu">
      <li>
        <div class="media-obj">
          <a href="<?php print url('user/' . $loaded->uid . '/edit', array('fragment' => 'user-profile-options')); ?>">
            <div class="media-fig">
              <span class="avatar"><?php print $image; ?></span>
            </div>
            <div class="media-bd">
              <strong><?php print check_plain($loaded->realname); ?></strong>
              <?php print t('Edit profile'); ?>
              <?php 
// show score
$user_score = tm_users_signup_score();

// store user score and uid in form fields to access via js
?>
<input style='display: none;' id='current_user_score' value='<?php print($user_score); ?>'>
<input style='display: none;' id='current_user_uid' value='<?php print($loaded->uid); ?>'>
<?php
if ($user_score >= 100) { ?>
<span style='padding-left: 0.2em; font-size: smaller; font-style: normal; background-color: green; color: #fff; border-radius: 2px; padding: 2px;'><?php print($user_score); ?>% complete</span>
<?php } else {
if ($user_score < 100) {
  $css_color = "green";
}
if ($user_score < 50) {
  $css_color = "orange";
}
if ($user_score < 20) {
  $css_color = "orange"; // could be red but we don't want to alarm
}
?>
<span style='padding-left: 0.2em; font-size: smaller; font-style: normal; background-color: <?php print($css_color); ?>; color: #fff; border-radius: 2px; padding: 2px;'><?php print($user_score); ?>% complete</span>
<?php
} // end user score display
?>
            </div>
          </a>
        </div>
      </li>
    </ul>
    <ul class="dropd-menu">
      <li><?php print l(t('Public profile'), 'user/' . $loaded->uid); ?></li>
      <li><?php print l(t('Account settings'), 'user/' . $loaded->uid . '/edit', array('fragment' => 'user-account-options')); ?></li>
      <li><?php print l(t('Notification settings'), 'user/' . $loaded->uid . '/edit', array('fragment' => 'user-notifications-options')); ?></li>
      <li><?php print l(t('Invite members'), 'invite'); ?></li>
<?php
$twitter_data = tm_twitter_account_load($loaded->uid);
if (!$twitter_data) { 
?>
      <li><?php print l(t('Connect with Twitter'), 'tm_twitter/oauth'); ?></li>
<?php
} // end if
?>

<?php if (!in_array("approved user", $loaded->roles)) { 
    // show last time request info was flagged
    $who_flagged = flag_get_entity_flags("user", $loaded->uid, "approval_requested_by_user");
    if (sizeof($who_flagged) > 0) {
      foreach ($who_flagged as $flagger) {
        $difference = time() - $flagger->timestamp;
      }
      $flagged_time = format_interval($difference, 1) . " ago";
?>
<li><?php print l(t('Approval requested (' . $flagged_time . ')'), 'javascript:jq_approval_already_requested();', array('fragment' => '','external'=>true)); ?></li>
<?php } else { ?>
<li><?php
$reason_for_joining = $loaded->field_reason_for_joining[LANGUAGE_NONE][0]['value'];
print ("<input style='display: none;' id='reason_for_joining' value='" . urlencode($reason_for_joining) . "'>");
print l(t('Approve my account'), 'javascript:jq_request_approval(' . $loaded->uid . ')', array('fragment' => '','external'=>true, 'attributes' => array('class' => array('approval-link')))); ?></li>
<?php
} // end if flagged
} // end if not approved
?>

    </ul>
  <?php if (in_array("approved user", $loaded->roles)) : ?>
    <ul class="dropd-menu">
      <?php print tm_users_menu_companies($loaded->uid); ?>
      <!--<li><?php print l(t('My companies'), 'user/' . $loaded->uid . '/companies', array('fragment' => 'user-notifications-options')); ?></li>-->
      <?php if (tm_organizations_check_user_can_create_company($loaded->uid)): ?>
      <li><?php print l(t('Add company profile'), 'node/add/organization'); ?></li>
      <?php endif; ?>
    </ul>
  <?php endif; ?>

  <?php if (!in_array("approved user", $loaded->roles)) : ?>
    <ul class="dropd-menu">
      <li><?php print l(t('Add company profile (pending approval)'), 'javascript:void(0);', array('fragment' => '','external'=>true)); ?></li>
    </ul>
  <?php endif; ?>

  <!-- Link to chapters the user has joined -->
    <ul class="dropd-menu">
        <?php print tm_users_menu_chapters($loaded->uid); ?>
        <?php
        if (in_array("chapter leader", $loaded->roles)) { ?>
        <li><?php
        global $conf;
        print l(t('Chapter leader resources'), $conf['tm_tips_chapter_leaders_link'], array('fragment' => '','external'=>true)); 
        } // end if ?></li>        
    </ul>

  <?php if (in_array("moderator", $loaded->roles)) : ?>
      <ul class="dropd-menu">
        <li><?php print l(t('Add event'), 'node/add/event'); ?></li>
        <li><?php print l(t('Add chapter'), 'node/add/chapter'); ?></li>
        <li><?php print l(t('All unapproved members'), 'admin/unapproved-members'); ?></li>
      </ul>
  <?php endif; ?>

    <ul class="dropd-menu">
      <li><?php print l(t('Sign out'), 'user/logout'); ?></li>
    </ul>

  <?php else : ?>

    <h3 class="menu-blk-title">Sign in</h3>
    <p><?php print l(t('Twitter'), 'tm_twitter/oauth', array('attributes' => array('class' => 'twitter-login'))); ?></p>
    <i class="or">or</i>

    <?php $login_form = drupal_get_form('user_login_block'); ?>
    <?php print render($login_form); ?>

    <?php if (variable_get('user_register', 1)) : ?>
      <h3 class="menu-blk-title">New to Travel Massive?</h3>
      <p><?php print l(t('Sign up now'), 'user/register', array('attributes' => array('title' => 'Create account', 'class' => array('cta-inline')))); ?></p>
    <?php endif; ?>

  <?php endif; ?>

</div>

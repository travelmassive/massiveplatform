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
  'style_name' => 'thumbnail',
  'path' => $img_uri,
  'alt' => 'user image',
  'title' => 'The user image',
));
?>
<?php if ($user->uid) : ?>
  
  <ul>
    <li>
      <a href="<?php print url('user/' . $loaded->uid . '/edit'); ?>">
        <div class="media"><?php print $image; ?></div>
        <div class="body"><strong><?php print check_plain($loaded->name); ?></strong><span><?php t('Edit profile'); ?></span></div>
      </a>
    </li>
    <li><?php print l(t('Public profile'), ''); ?></li>
    <li><?php print l(t('Account settings'), ''); ?></li>
  </ul>
  <ul>
    <li><?php print l(t('Link'), ''); ?></li>
    <li><?php print l(t('Link'), ''); ?></li>
  </ul>
  <ul>
    <li><?php print l(t('Link'), ''); ?></li>
    <li><?php print l(t('Link'), ''); ?></li>
  </ul>
  <ul><li><?php print l(t('Sign out'), 'user/logout'); ?></li></ul>
  
<?php else : ?>
  
  <h3>Sign in</h3>
  <p><?php print l(t('Twitter'), 'tm_twitter/oauth', array('attributes' => array('class' => 'twitter-login'))); ?></p>
  <span class="or">or</span>
  
  <?php $login_form = drupal_get_form('user_login_block'); ?>
  <?php print render($login_form); ?>
  
  <?php if (variable_get('user_register', 1)) : ?>
    <h3>New to Travel Massive?</h3>
    <p><?php print l(t('Sign up now »'), 'user/register'); ?></p>
  <?php endif; ?>
  
<?php endif; ?>
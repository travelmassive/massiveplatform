<?php 

// account user menu template

global $user;
global $conf;

// Only a loaded user has values for the fields.
$loaded = user_load($user->uid);
$user_score = 0;

// Set hidden fields
$reason_for_joining = "";
if (isset($loaded->field_reason_for_joining[LANGUAGE_NONE][0]['value'])) {
  $reason_for_joining = $loaded->field_reason_for_joining[LANGUAGE_NONE][0]['value'];
}
$user_score = tm_users_signup_score();
?>
<div id='tm-user-hidden-fields' style='display: none;'>
<input style='display: none;' id='reason_for_joining' value='<?php print urlencode($reason_for_joining);?>'>
<input style='display: none;' id='current_user_score' value='<?php print($user_score); ?>'>
<input style='display: none;' id='current_user_uid' value='<?php print($loaded->uid); ?>'>
</div>

<?php

// Set avatar
if (empty($loaded->field_avatar)) {
  $img_uri = $conf["tm_images_default_field_avatar"];
}  else {
  $img_uri = $loaded->field_avatar[LANGUAGE_NONE][0]['uri'];
}

// If image is default, replace with random image from folder
if (isset($conf["tm_images_default_path"])) {
  if ($img_uri == $conf["tm_images_default_field_avatar"]) {
    $image_id = $loaded->uid;
    $cover_files = $conf["tm_images_default_avatar"];
    $image_index = $image_id % sizeof($cover_files);
    $img_uri = $conf["tm_images_default_path"] . $cover_files[$image_index];
  }
}

$image = theme('image_style', array(
  'style_name' => 'avatar',
  'path' => $img_uri,
  'alt' => 'user image',
  'title' => 'The user image',
));

?>

<h2>
  <?php
    // allow customization of account icon
    $account_css_class = "";
    if (isset($conf["tm_users_custom_account_css_class"])) {
      $account_css_class = " " . $conf["tm_users_custom_account_css_class"];
    }
  ?>
  <a class="toggle<?php echo $account_css_class; ?>" href="#account-menu-blk" data-dropd-toggle>
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
          <a href="<?php print url('user/' . $loaded->uid); ?>">
            <div class="media-fig">
              <span class="avatar"><?php print $image; ?></span>
            </div>
            <div class="media-bd">
              <strong><?php print check_plain($loaded->realname); ?></strong>
              <?php print t('View profile'); ?>
                <?php
                if ($user_score >= 100) {
                  $css_color = "green";
                }
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
                <span style='padding-left: 0.2em; font-size: smaller; font-style: normal; background-color: <?php print($css_color); ?>; color: #fff; border-radius: 2px; padding: 2px; padding-left: 4px; padding-right: 4px;'><?php print($user_score); ?>% complete</span>
            </div>
          </a>
        </div>
      </li>
    </ul>

    <ul class="dropd-menu dropdown-account-settings">
      <li><?php print l(t('Account settings'), 'user/' . $loaded->uid . '/edit', array('fragment' => 'user-account-options')); ?></li>
      <li><?php print l(t('Notification settings'), 'user/' . $loaded->uid . '/edit', array('fragment' => 'user-notifications-options')); ?></li>
      <?php if (in_array("approved user", $loaded->roles)) { ?>
      <?php if (!tm_users_is_member_reported($loaded->uid)) { ?>
      <li><?php print l(t('Invite members'), 'invite'); ?></li>
      <?php } // ?>
      <?php } // ?>

      <?php 
      // show review link
      if ($conf["tm_users_feedback_show_menu"] and in_array("approved user", $loaded->roles) and (!(tm_users_is_member_reported($loaded->uid)))) { 
        $user_review_min_age = 30; // 30 days
        if (isset($conf["tm_users_review_min_age"])) {
          $user_review_min_age = $conf["tm_users_review_min_age"];
        }
        $account_age_days = ((time() - $loaded->created) / (60 * 60 * 24));
        if ($account_age_days > $user_review_min_age) { ?>
      <li><a href='javascript:jq_net_promoter_score("<?php print($conf["tm_site_name"]);?>");'><?php print($conf["tm_users_feedback_label"]);?></a></li>
      <?php 
        } // end if
      } // end if
      ?>

      <?php
      // user links
      if (isset($conf["tm_users_account_menu_links"])) {
        foreach($conf["tm_users_account_menu_links"] as $account_menu_title => $account_menu_link) {
          print "<li>" . l(t($account_menu_title), $account_menu_link, array('fragment' => '', 'external' =>true, 'html' => true)) . "</li>";
        }
      }
      ?>

      <?php

      // join user subscriber and company subscriber links
      $subscriber_links = array();

      // subscriber links
      if (module_exists("tm_subscriptions_user")) {
        if (tm_subscriptions_is_user_subscription_enabled($loaded->uid)) { 
          if (isset($conf["tm_users_subscriber_menu_links"])) {
            foreach($conf["tm_users_subscriber_menu_links"] as $account_menu_title => $account_menu_link) {
              $subscriber_links[$account_menu_title] = $account_menu_link;
            }
          }
        }
      }

      // company subscriber links
      if (module_exists("tm_subscriptions")) {
        $subscribed_companies = tm_subscriptions_get_users_subscribed_companies($loaded->uid);
        if (sizeof($subscribed_companies) > 0) {
          if (isset($conf["tm_users_company_subscriber_menu_links"])) {
            foreach($conf["tm_users_company_subscriber_menu_links"] as $account_menu_title => $account_menu_link) {
                $subscriber_links[$account_menu_title] = $account_menu_link;
            }
          }
        }
      }

      // print links
      foreach($subscriber_links as $account_menu_title => $account_menu_link) {
        print "<li>" . l(t($account_menu_title), $account_menu_link, array('fragment' => '', 'external' =>true, 'html' => true)) . "</li>";
      }

      ?>

      <?php if (in_array("chapter leader", $loaded->roles) or in_array("moderator", $loaded->roles)): ?>
      <?php if (isset($conf["tm_users_chapter_leader_menu_links"])): ?>
          <?php
            // chapter leader links
            foreach($conf["tm_users_chapter_leader_menu_links"] as $account_menu_title => $account_menu_link) {
              print "<li>" . l(t($account_menu_title), $account_menu_link, array('fragment' => '', 'external' =>true, 'html' => true)) . "</li>";
            }
          ?>
      <?php endif; ?>
      <?php endif; ?>

      <?php if (!in_array("approved user", $loaded->roles)) { 
      // show last time request info was flagged
      $who_flagged = flag_get_entity_flags("user", $loaded->uid, "approval_requested_by_user");
      if (sizeof($who_flagged) > 0) {
        foreach ($who_flagged as $flagger) {
          $difference = time() - $flagger->timestamp;
        }
        $flagged_time = format_interval($difference, 1) . " ago";
      ?>
      <li><?php print l(t('Request verification (' . $flagged_time . ')'), 'javascript:jq_approval_already_requested();', array('fragment' => '','external'=>true, 'attributes' => array('class' => 'tm-account-menu-verification'))); ?></li>
      <?php } else { ?>
      <li><?php
        print l('Request verification', 'javascript:jq_request_approval(' . $loaded->uid . ')', array('fragment' => '','external'=>true, 'attributes' => array('class' => array('approval-link', 'tm-account-menu-verification')))); ?></li>
      <?php
      } // end if flagged
      } // end if not approved
      ?>
    </ul>

    

  <?php
    $include_chapter_events = in_array("chapter leader", $loaded->roles);
    $user_is_admin_or_moderator = (in_array("administrator", $loaded->roles) or in_array("moderator", $loaded->roles));
    $member_events_html = tm_users_menu_events($loaded->uid, $include_chapter_events, 5, $user_is_admin_or_moderator);
    if ($member_events_html != ""): ?>
    <ul class="dropd-menu dropdown-company-profiles">
    <?php print $member_events_html; ?>
    </ul>
  <?php endif; ?>

  <?php if (in_array("approved user", $loaded->roles)) : ?>
      <?php
        $company_profiles = tm_users_menu_companies($loaded->uid);
        $can_create_company = tm_organizations_check_user_can_create_company($loaded->uid); 
        if (($company_profiles != "") or ($can_create_company)):
      ?>
      <ul class="dropd-menu dropdown-company-profiles">
        <?php print $company_profiles; ?>
        <?php if (tm_organizations_check_user_can_create_company($loaded->uid)): ?>
        <li><?php print l(t('Add company page'), 'node/add/organization'); ?></li>
        <?php endif; ?>
      </ul>
      <?php endif; ?>
  <?php endif; ?>

 <?php if (!in_array("approved user", $loaded->roles)) : ?>
    <ul class="dropd-menu">
    <li><?php print l(t('Add company page'), 'javascript:jq_alert("How can list my company page?", "To create a <a href=\"/companies\">company page</a> you need a <i>verified</i> account.<br><br><strong>What should I do next?</strong><br>Complete your personal profile and request verification. When you are verified this feature will be enabled.");', array('fragment' => '','external'=>true)); ?></li>
    </ul>
  <?php endif; ?>

    <ul class="dropd-menu dropdown-chapters">
        <?php print tm_users_menu_chapters($loaded->uid); ?>
    </ul>     

  <?php if ((in_array("moderator", $loaded->roles)) or (in_array("administrator", $loaded->roles))) : ?>
      <ul class="dropd-menu" id="account_menu_moderator_actions_show">
        <li><?php print l(t('Moderator tools'), 'javascript:tm_show_account_menu_moderator_actions();', array('fragment' => '','external'=>true)); ?></li>
      </ul>
      <ul class="dropd-menu dropdown-moderator-tools" id="account_menu_moderator_actions_items" style="display: none;">
        <?php if (module_exists("tm_checkout")) { ?>
          <li><?php print l(t('Payments'), 'checkout/reports/all-payments/'); ?></li>
        <?php } // end if ?>
        <li><?php print l(t('Add event'), 'node/add/event'); ?></li>
        <li><?php print l(t('Add chapter'), 'node/add/chapter'); ?></li>
        <li><?php print l(t('Past events'), 'events/list/past'); ?></li>
        <li><?php print l(t('Preview events'), 'events/list/preview'); ?></li>
        <li><?php print l(t('All ' . strtolower(tm_users_get_unapproved_member_label("plural"))), 'admin/unapproved-members'); ?></li>
        <li><?php print l(t('Chapter leaders'), 'admin/tm_reports'); ?></li>
        <li><?php print l(t('Global insights'), 'admin/global_insights'); ?></li>
        <li><?php print l(t('Member feedback'), 'feedback/results/all'); ?></li>
        <li><?php print l(t('Member testimonials'), 'admin/member-testimonials'); ?></li>
        <li><?php print l(t('Member COVID-19 messages'), 'admin/member-covid19-messages'); ?></li>
        <li><?php print l(t('Company COVID-19 messages'), 'admin/company-covid19-messages'); ?></li>
        <?php if (in_array("brand-editor", $loaded->roles) or in_array("administrator", $loaded->roles)): ?>
          <li><?php print l(t('Manage branding'), 'admin/branding'); ?></li>
        <?php endif; ?>
        <li><?php print l(t('Export chapter insights'), 'admin/export_chapter_insights'); ?></li>
        <?php if (tm_users_download_global_newsletter_csv_check()): ?>
        <li><?php print l(t('Export newsletter subscribers'), 'javascript:jq_alert_no_buttons("<img style=\'width: 24px; height: 16px; margin-right: 4px;\' src=\'/sites/all/themes/tm/images/load-more-ajax-loader-2.gif\'> Please wait", "Your download will begin in a moment.<br><br><b>IMPORTANT:</b> This information is confidential and may only be used in compliance with the ' . $conf["tm_site_name"] . ' Privacy Terms.<iframe style=\'display: none\' src=\'/admin/export_global_newsletter\'></iframe>");', array('fragment' => '','external'=>true)); ?></li>
        <?php endif; ?>
         <?php if (tm_users_download_chapter_leaders_csv_check()): ?>
         <li><?php print l(t('Export chapter leaders'), 'javascript:jq_alert_no_buttons("<img style=\'width: 24px; height: 16px; margin-right: 4px;\' src=\'/sites/all/themes/tm/images/load-more-ajax-loader-2.gif\'> Please wait", "Your download will begin in a moment.<br><br><b>IMPORTANT:</b> This information is confidential and may only be used in compliance with the ' . $conf["tm_site_name"] . ' Privacy Terms.<iframe style=\'display: none\' src=\'/admin/export_chapter_leaders\'></iframe>");', array('fragment' => '','external'=>true)); ?></li>
        <?php endif; ?>
        <?php if (tm_users_download_facebook_csv_check()): ?>
        <li><?php print l(t('Export Facebook customer data'), 'javascript:jq_alert_no_buttons("<img style=\'width: 24px; height: 16px; margin-right: 4px;\' src=\'/sites/all/themes/tm/images/load-more-ajax-loader-2.gif\'> Please wait", "Your download will begin in a moment.<br><br><b>IMPORTANT:</b> This information is confidential and may only be used in compliance with the ' . $conf["tm_site_name"] . ' Privacy Terms.<iframe style=\'display: none\' src=\'/admin/export_facebook_customers\'></iframe>");', array('fragment' => '','external'=>true)); ?></li>
        <?php endif; ?>
      </ul>
  <?php endif; ?>

    <ul class="dropd-menu">
      <li><?php print l(t('Sign out'), 'user/logout'); ?></li>
    </ul>

  <?php else : ?>

    <h3 class="menu-blk-title"><?php if (!isset($conf["tm_signin_title"])) { print "JOIN OUR COMMUNITY"; } else { print($conf["tm_signin_title"]); }?></h3>
    <?php

      // signin buttons
      $has_external_signin = false;
      if (isset($conf["tm_signin_buttons"])) {
        foreach($conf["tm_signin_buttons"] as $signin_method) {
          if (($signin_method == "facebook") and $conf["tm_signin_facebook"]) {
            $has_external_signin = true;
            print "<p style='margin-bottom: 0.5rem;'>" . l(t($conf['tm_signin_facebook_label']), 'user/simple-fb-connect', array('attributes' => array('class' => 'facebook-login'))) . "</p>";
          }
          if (($signin_method == "twitter") and $conf["tm_signin_twitter"]) {
            $has_external_signin = true;
            print "<p style='margin-bottom: 0.5rem;'>" . l(t($conf['tm_signin_twitter_label']), 'tm_twitter/oauth', array('attributes' => array('class' => 'twitter-login'))) . "</p>";
          }
        } 
      }

      // signin links
      if (isset($conf["tm_signin_links"])) {
        foreach($conf["tm_signin_links"] as $signin_method) {
          if (($signin_method == "facebook") and $conf["tm_signin_facebook"]) {
            $has_external_signin = true;
            print "<p>" . l(t($conf['tm_signin_facebook_label']), 'user/simple-fb-connect') . "</p>";
          }
          if (($signin_method == "twitter") and $conf["tm_signin_twitter"]) {
            $has_external_signin = true;
            print "<p>" . l(t($conf['tm_signin_twitter_label']), 'tm_twitter/oauth') . "</p>";
          }
        } 
      }
    ?>
   
    <?php if ($has_external_signin) { ?>
    <i class="or">or</i>
    <?php } // end if ?>

    <?php $login_form = drupal_get_form('user_login_block'); ?>
    <?php print render($login_form); ?>

    <?php if (variable_get('user_register', 1)) : ?>
      <h3 class="menu-blk-title">New to <?php print($conf["tm_site_name"]);?>?</h3>
      <p><?php print l(t('Sign up now'), 'user/register', array('attributes' => array('title' => 'Create account', 'class' => array('cta-inline')))); ?></p>
    <?php endif; ?>

  <?php endif; ?>

</div>

<?php 

// Prompt user to request approval.
// Conditions:
// 1. User is not approved
// 2. User score > 50
// 3. User account has changed
// 4. User is not flagged as requested approval
// 5. (in jq_prompt_request_approval) user has not clicked "not yet" in past day
// Then show a prompt to request approval of account

$tm_account_changed = false;
$tm_account_changed_set = false;
if (isset($_SESSION["tm_account_changed"])) {
  $tm_account_changed_set = true;
  $tm_account_changed = $_SESSION["tm_account_changed"];
}

// show last time request info was flagged
$who_flagged = flag_get_entity_flags("user", $loaded->uid, "approval_requested_by_user");
$user_requested_approval = (sizeof($who_flagged) > 0);

if ((!in_array("approved user", $loaded->roles)) 
  and ($user_score > 50) 
  and ($tm_account_changed)
  and (!$user_requested_approval)) {

    $message = "Do you want to unlock more features for your account?";

    // add cookie library  
    drupal_add_library('system', 'jquery.cookie');

    // Load the prompt
    drupal_add_js("jQuery(document).ready(function($) {
        setTimeout(function(){
          if (typeof jq_prompt_request_approval == 'function') { 
            jq_prompt_request_approval(" . $loaded->uid . ", '" . $message . "');
          }
        }, 500);
        });
      ", "inline");
} 

//  unset account changed session
if ($tm_account_changed_set) {
  unset($_SESSION["tm_account_changed"]);
}
?>

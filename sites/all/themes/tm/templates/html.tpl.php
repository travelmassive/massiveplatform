<?php
/**
 * @file
 * Returns the HTML for the basic html structure of a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728208
 */
?><!DOCTYPE html>
<!--[if IEMobile 7]><html class="iem7" <?php print $html_attributes; ?>><![endif]-->
<!--[if lte IE 6]><html class="lt-ie9 lt-ie8 lt-ie7" <?php print $html_attributes; ?>><![endif]-->
<!--[if (IE 7)&(!IEMobile)]><html class="lt-ie9 lt-ie8" <?php print $html_attributes; ?>><![endif]-->
<!--[if IE 8]><html class="lt-ie9" <?php print $html_attributes; ?>><![endif]-->
<!--[if (gte IE 9)|(gt IEMobile 7)]><!--><html <?php print $html_attributes . $rdf_namespaces; ?>><!--<![endif]-->

  <head>
    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
    <?php if ($default_mobile_metatags): ?>
    <meta name="MobileOptimized" content="width" />
    <meta name="HandheldFriendly" content="true" />
    <meta content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0" name="viewport" />
    <?php endif; ?>
    <meta http-equiv="cleartype" content="on" />

    <link rel="shortcut icon" href="<?php echo tm_branding_get_element("favicon");?>" type="image/x-icon" />
    <link rel="apple-touch-icon" href="<?php echo tm_branding_get_base_element("apple_touch_icon", "apple-touch-icon.png");?>"/>
    <link rel="apple-touch-icon" sizes="57x57" href="<?php echo tm_branding_get_base_element("apple_touch_icon", "apple-touch-icon-57x57.png");?>"/>
    <link rel="apple-touch-icon" sizes="72x72" href="<?php echo tm_branding_get_base_element("apple_touch_icon", "apple-touch-icon-72x72.png");?>"/>
    <link rel="apple-touch-icon" sizes="76x76" href="<?php echo tm_branding_get_base_element("apple_touch_icon", "apple-touch-icon-76x76.png");?>"/>
    <link rel="apple-touch-icon" sizes="114x114" href="<?php echo tm_branding_get_base_element("apple_touch_icon", "apple-touch-icon-114x114.png");?>"/>
    <link rel="apple-touch-icon" sizes="120x120" href="<?php echo tm_branding_get_base_element("apple_touch_icon", "apple-touch-icon-120x120.png");?>"/>
    <link rel="apple-touch-icon" sizes="144x144" href="<?php echo tm_branding_get_base_element("apple_touch_icon", "apple-touch-icon-144x144.png");?>"/>
    <link rel="apple-touch-icon" sizes="152x152" href="<?php echo tm_branding_get_base_element("apple_touch_icon", "apple-touch-icon-152x152.png");?>"/>
    
    <link type="text/plain" rel="author" href="<?php print $base_path . path_to_theme(); ?>/humans.txt" />
    <link href="//fonts.googleapis.com/css?family=Raleway:400,800" rel="stylesheet" type="text/css" />
    <?php print $styles; ?>
    
    <?php if ($add_html5_shim and !$add_respond_js): ?>
      <!--[if lt IE 9]>
      <script src="<?php print $base_path . $path_to_zen; ?>/js/html5.js"></script>
      <![endif]-->
    <?php elseif ($add_html5_shim and $add_respond_js): ?>
      <!--[if lt IE 9]>
      <script src="<?php print $base_path . $path_to_zen; ?>/js/html5-respond.js"></script>
      <![endif]-->
    <?php elseif ($add_respond_js): ?>
      <!--[if lt IE 9]>
      <script src="<?php print $base_path . $path_to_zen; ?>/js/respond.js"></script>
      <![endif]-->
    <?php endif; ?>
    
    <!-- http://writing.colin-gourlay.com/safely-using-ready-before-including-jquery/ -->
    <script>(function(w,d,u){w.readyQ=[];w.bindReadyQ=[];function p(x,y){if(x=="ready"){w.bindReadyQ.push(y);}else{w.readyQ.push(x);}};var a={ready:p,bind:p};w.$=w.jQuery=function(f){if(f===d||f===u){return a}else{p(f)}}})(window,document)</script>

    <?php
      // include brand css
      $tm_branding_css = tm_branding_get_element("include_css");
      if ($tm_branding_css) {
        echo "<style>" . $tm_branding_css . "</style>";
      }
    ?>

  </head>
  <?php
    global $conf;
    // https://github.com/VodkaBears/Vide#readme
    $bg_video_body_attributes = "";
    if ($is_front) {

      $frontpage_video_url = tm_branding_get_element("frontpage_video_url");
      $frontpage_image = tm_branding_get_element("frontpage_image");

      // background video and background image
      if (($frontpage_video_url != "") and ($frontpage_image != "")) {
        $bg_video_body_attributes = "data-vide-bg=\"mp4: $frontpage_video_url, poster: $frontpage_image\" data-vide-options=\"posterType: jpg, playbackRate: 1\"";
      }

      // background video only
      if (($frontpage_video_url != "") and ($frontpage_image == "")) {
        $bg_video_body_attributes = "data-vide-bg=\"mp4: $frontpage_video_url\" data-vide-options=\"playbackRate: 1\"";
      }

      // background image only
      if (($frontpage_video_url == "") and ($frontpage_image != "")) {
        $bg_video_body_attributes = "data-vide-bg=\"poster: $frontpage_image\" data-vide-options=\"posterType: jpg, playbackRate: 1\"";
      }
      
    } // end if frront page
  ?>
  <body class="<?php print $classes; ?>" <?php print $attributes;?> <?php if ($is_front) { print($bg_video_body_attributes); } ?>>
    
  <?php 
    if ($is_front) { 
      if ($frontpage_video_url != "") { 
  ?>
    <div id="tm-frontpage-video-controls" style="display:none;">
      <div id="tm-frontpage-video-buttons"><span class="tm-frontpage-video-pause"></span><span class="tm-frontpage-video-play"></span></div>
      <div id="tm-frontpage-video-info">Now playing: <a href="<?php echo tm_branding_get_element("frontpage_video_link"); ?>"><?php echo tm_branding_get_element("frontpage_video_text");?></a>
      </div>
    </div>
  <?php 
      } // end front page video
    } // end if front page
  ?>

    <!--[if lt IE 9]>
      <div id="nocando">
        <h1>Your browser is not supported.</h1>
        <p>To view this website, we recommend using the <strong>latest version</strong> of Chrome, Safari or Firefox.</p>
        <p>Here: <a href="http://browsehappy.com/" title="Visit Browser happy">upgrade your browser</a>!</p>
      </div>
    <![endif]-->
    
    <?php if ($skip_link_text && $skip_link_anchor): ?>
    <p id="skip-link">
      <a href="#<?php print $skip_link_anchor; ?>" class="element-invisible element-focusable"><?php print $skip_link_text; ?></a>
    </p>
    <?php endif; ?>
    <?php print $page_top; ?>
    <?php print $page; ?>
    <?php print $scripts; ?>
    <?php print $page_bottom; ?>

    <?php
      // include brand js
      $tm_branding_js = tm_branding_get_element("include_js");
      if ($tm_branding_js) {
        echo "<script>" . $tm_branding_js . "</script>";
      }
    ?>

    <!-- http://writing.colin-gourlay.com/safely-using-ready-before-including-jquery/ -->
    <script>(function($,d){$.each(readyQ,function(i,f){$(f)});$.each(bindReadyQ,function(i,f){$(d).bind("ready",f)})})(jQuery,document)</script>

  </body>
</html>

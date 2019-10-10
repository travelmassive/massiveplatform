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
    
    <?php
    global $conf;
    $fonts_url = "//fonts.googleapis.com/css?family=Lato:300,400,700&display=swap";
    if (isset($conf["tm_theme_google_fonts_url"])) {
      $fonts_url = $conf["tm_theme_google_fonts_url"];
    }
    ?>
    <link href="<?php print $fonts_url; ?>" rel="stylesheet" type="text/css">

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
    <script type="text/javascript">(function(w,d,u){w.readyQ=[];w.bindReadyQ=[];function p(x,y){if(x=="ready"){w.bindReadyQ.push(y);}else{w.readyQ.push(x);}};var a={ready:p,bind:p};w.$=w.jQuery=function(f){if(f===d||f===u){return a}else{p(f)}}})(window,document)</script>

    <!-- Header JS -->
    <?php print $scripts; ?>

    <?php
      // include brand css
      $tm_branding_css = tm_branding_get_element("include_css");
      if ($tm_branding_css) {
        echo "<style>" . $tm_branding_css . "</style>";
      }
    ?>

  </head>

  <style>
  /* Fix for 'Aggregate and compress CSS files' */
  .trilithon-header .meta span:after {
    content: ', ';
  }
  .card.contained .meta span:after, .node-form>div.card .meta span:after, #user-profile-form>div.card .meta span:after, #user-register-form>div.card .meta span:after, #content .card.search-form .meta span:after {
    content: ', ';
  }
  .card.contained .meta span:last-of-type:after, .node-form>div.card .meta span:last-of-type:after, #user-profile-form>div.card .meta span:last-of-type:after, #user-register-form>div.card .meta span:last-of-type:after, #content .card.search-form .meta span:last-of-type:after {
    content: '';
  }
  .contained-block .detail-item.field-location span:after, .contained-block .detail-item.location span:after {
    content: ', ';
  }
  </style>

  <?php
  // background video
  if ($is_front) { 
  ?>

  <style>
  .fullscreen-bg {
      position: absolute;
      top: 0;
      right: 0;
      bottom: 0;
      left: 0;
      overflow: hidden;
      z-index: -100;
      opacity: <?php print (round(tm_branding_get_element("frontpage_opacity"), 2)); ?> !important;
  }

  .fullscreen-bg__video {
      position: absolute;
      top: 50%;
      left: 50%;
      width: auto;
      height: auto;
      min-width: 100%;
      min-height: 100%;
      -webkit-transform: translate(-50%, -50%);
         -moz-transform: translate(-50%, -50%);
          -ms-transform: translate(-50%, -50%);
        transform: translate(-50%, -50%);
  }

  video::-webkit-media-controls {
      display:none !important;
  }

  video::-webkit-media-controls-start-playback-button {
    display: none !important;
  }

  </style>
  <?php

      $frontpage_image = tm_branding_get_element("frontpage_image");
      $frontpage_video_url = tm_branding_get_element("frontpage_video_url");

      // create <video> tag if image or video url provided
      $bg_video_body_html = "";
      if (($frontpage_image != "") or ($frontpage_video_url != "")) {

        // div wrapper
        $bg_video_body_html = '<div class="fullscreen-bg">';

        // video cover
        if ($frontpage_image != '') {
          $bg_video_body_html .= '<video loop autoplay muted poster="' . $frontpage_image . '" class="fullscreen-bg__video" id="fullscreen-bg-video">';
        } else {
          $bg_video_body_html .= '<video loop autoplay muted class="fullscreen-bg__video">';
        }

        // background video and background image
        if ($frontpage_video_url != "") {
          $bg_video_body_html .= '<source src="' . $frontpage_video_url . '" type="video/mp4">';
        }

        $bg_video_body_html .= '</video>';
        $bg_video_body_html .= '</div>';

        // detect hls url
        $is_hls_url = 0;
        if (strpos($frontpage_video_url, "m3u8") !== false) {
          $is_hls_url = 1;
        }
      }
    } // end if front page
  ?>

  <body class="<?php print $classes; ?>" <?php print $attributes;?>>
    
  <?php 
    if ($is_front and ($frontpage_image != "")) {
      print($bg_video_body_html); // <video> embed
      if ($frontpage_video_url != "") { // show video controls
  ?>
    <div id="tm-frontpage-video-controls" style="display:none;">
      <div id="tm-frontpage-video-buttons"><span class="tm-frontpage-video-pause"></span><span class="tm-frontpage-video-play"></span></div>
      <div id="tm-frontpage-video-info">Now playing: <a href="<?php echo tm_branding_get_element("frontpage_video_link"); ?>"><?php echo tm_branding_get_element("frontpage_video_text");?></a></div>
    </div>

<script>
  // Add HTTP Live Streaming support for Chrome and Firefox
  // https://github.com/dailymotion/hls.js
  // note: safari already supports hls
  jQuery(document).ready(function() {
    var is_chrome = navigator.userAgent.indexOf('Chrome') > -1;
    var is_explorer = navigator.userAgent.indexOf('MSIE') > -1;
    var is_firefox = navigator.userAgent.indexOf('Firefox') > -1;
    var is_safari = navigator.userAgent.indexOf("Safari") > -1;
    var is_hls_url = <?php print($is_hls_url); ?>;
    if (is_hls_url && (is_chrome || is_firefox)) {
      if(Hls.isSupported()) {
        var video = document.getElementById('fullscreen-bg-video');
        var hls = new Hls();
        hls.loadSource('<?php print($frontpage_video_url);?>');
        hls.attachMedia(video);
        hls.on(Hls.Events.MANIFEST_PARSED,function() {
          video.play();
        });
      }
    }
  });
</script>

  <?php 
      } // end show video controls
    } // end if front page video
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
    <?php print $page_bottom; ?>
    <?php print (drupal_get_js('tm_after_footer')); ?>
    <?php if($_SERVER["REQUEST_URI"] == "/chapters/map") { ?><script>L.Icon.Default.imagePath = "/sites/all/libraries/leaflet/images/";</script><?php } ?>
    
    <?php
      // include brand js
      $tm_branding_js = tm_branding_get_element("include_js");
      if ($tm_branding_js) {
        echo "<script type='text/javascript'>" . $tm_branding_js . "</script>";
      }
    ?>

    <!-- http://writing.colin-gourlay.com/safely-using-ready-before-including-jquery/ -->
    <script type="text/javascript">(function($,d){$.each(readyQ,function(i,f){$(f)});$.each(bindReadyQ,function(i,f){$(d).bind("ready",f)})})(jQuery,document)</script>

  </body>
</html>

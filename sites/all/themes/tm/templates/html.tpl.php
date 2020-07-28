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
  $sidebar_class = "";
  if (isset($conf["tm_theme_custom_sidebar_template"])) {
    if ($conf["tm_theme_custom_sidebar_template"] != "") {
      $sidebar_class = "tm_sidebar";
    }
  }
  ?>
  <body class="<?php print $classes; ?> <?php print $sidebar_class; ?>" <?php print $attributes;?>>
    
    <?php 
      $frontpage_image = tm_branding_get_element("frontpage_image");
      $frontpage_opacity = tm_branding_get_element("frontpage_opacity");
      if ($is_front and ($frontpage_image != "")) {
    ?>
        <style>
        #bg {
          position: fixed; 
          top: -50%; 
          left: -50%; 
          width: 200%; 
          height: 200%;
        }
        #bg img {
          position: absolute; 
          top: 0; 
          left: 0; 
          right: 0; 
          bottom: 0; 
          margin: auto; 
          min-width: 50%;
          min-height: 50%;
          opacity: <?php print $frontpage_opacity; ?>;
        }
        </style>
        <div id="bg">
          <img src="<?php print ($frontpage_image);?>" alt="background image">
        </div>
    <?php
      } // end if front page with image
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

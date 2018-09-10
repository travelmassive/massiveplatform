<?php

// tm_fields.menu_sharing.inc - menu methods

/**
 * Process menu items
 * Called from _tm_more_actions_process()
 */
function _tm_more_actions_process_sharing_menu($field, $title, &$links, &$menu_item, $currentuser, $targetuser, $nid) {

  global $conf;
  global $user;
  global $base_root;
  
  $base = $base_root . request_uri();

  // START sharing links
  $links['share'][] = array(
    'wrapper_class' => array('share_facebook'),
    'content' => l(t('Share on Facebook'), 'https://www.facebook.com/sharer/sharer.php', array(
      'absolute' => TRUE,
      'query' => array('u' => $base),
      'attributes' => array(
        'target' => '_blank',
        'class' => array('facebook'),
        'title' => t('Share this on Facebook')
      )
    ))
  );
  
  $links['share'][] = array(
    'wrapper_class' => array('share_twitter'),
    'content' => l(t('Share on Twitter'), 'https://twitter.com/share', array(
      'absolute' => TRUE,
      'query' => array('url' => $base),
      'attributes' => array(
        'target' => '_blank',
        'class' => array('twitter'),
        'title' => t('Share this on Twitter')
      )
    ))
  );

  // https://developer.linkedin.com/docs/share-on-linkedin#!
  $links['share'][] = array(
    'wrapper_class' => array('share_linkedin'),
    'content' => l(t('Share on LinkedIn'), 'https://www.linkedin.com/shareArticle', array(
      'absolute' => TRUE,
      'query' => array('url' => $base, 'mini' => true, 'source' => $conf["tm_site_name"]),
      'attributes' => array(
        'target' => '_blank',
        'class' => array('linkedin'),
        'title' => t('Share this on LinkedIn')
      )
    ))
  );
  // END sharing links

}
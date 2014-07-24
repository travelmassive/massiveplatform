<?php
/**
 * @file
 * Display Suite Card template.
 *
 * Available variables:
 *
 * Layout:
 * - $classes: String of classes that can be used to style this layout.
 * - $contextual_links: Renderable array of contextual links.
 * - $layout_wrapper: wrapper surrounding the layout.
 *
 * Regions:
 *
 * - $li_content: Rendered content for the "Content" region.
 *
 */
?>
<li>
  <<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes;?> clearfix">
      <!-- Needed to activate contextual links -->
      <?php if (isset($title_suffix['contextual_links'])): ?>
        <?php print render($title_suffix['contextual_links']); ?>
      <?php endif; ?>
      <?php if (isset($nid)): ?>
        <a href="<?php print url('node/' . $nid); ?>">
      <?php else: ?>
        <?php $uid = $elements['#account']->uid; ?>
        <a href="<?php print url('user/' . $uid); ?>">
      <?php endif; ?>
        <?php print $li_content; ?>
        </a>
  </<?php print $layout_wrapper ?>>
</li>
<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
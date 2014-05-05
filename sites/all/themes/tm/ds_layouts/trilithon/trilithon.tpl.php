<?php
/**
 * @file
 * Display Suite Trilithon template.
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
 * - $banner: Rendered content for the "Banner" region.
 * - $banner_classes: String of classes that can be used to style the "Banner" region.
 *
 * - $first_column: Rendered content for the "First column" region.
 * - $first_column_classes: String of classes that can be used to style the "First column" region.
 *
 * - $second_column: Rendered content for the "Second column" region.
 * - $second_column_classes: String of classes that can be used to style the "Second column" region.
 */
?>
<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes;?> clearfix">

  <!-- Needed to activate contextual links -->
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

    <<?php print $banner_wrapper; ?> class="banner<?php print $banner_classes; ?>">
      <?php print $banner; ?>
    </<?php print $banner_wrapper; ?>>

    <<?php print $first_column_wrapper; ?> class="column first<?php print $first_column_classes; ?>">
      <?php print $first_column; ?>
    </<?php print $first_column_wrapper; ?>>

    <<?php print $second_column_wrapper; ?> class="column second<?php print $second_column_classes; ?>">
      <?php print $second_column; ?>
    </<?php print $second_column_wrapper; ?>>

</<?php print $layout_wrapper ?>>

<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>

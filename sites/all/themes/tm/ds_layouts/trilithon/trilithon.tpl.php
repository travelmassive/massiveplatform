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
<<?php print $layout_wrapper; print $layout_attributes; ?> class="trilithon <?php print $classes;?> clearfix">

  <!-- Needed to activate contextual links -->
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

    <header class="trilithon-header contained">

      <<?php print $header_media_wrapper; ?> class="media <?php print $header_media_classes; ?>">
        <?php print $header_media; ?>
      </<?php print $header_media_wrapper; ?>>

      <<?php print $header_body_wrapper; ?> class="bd <?php print $header_body_classes; ?>">
        <?php print $header_body; ?>
      </<?php print $header_body_wrapper; ?>>

      <<?php print $header_extra_wrapper; ?> class="extra <?php print $header_extra_classes; ?>">
        <?php print $header_extra; ?>
      </<?php print $header_extra_wrapper; ?>>
    </header>

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

<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 * 
 * Customizations:
 * - add a UL with LI
 * 
 */
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<ul>
<?php foreach ($rows as $id => $row): ?>
  <li<?php if ($classes_array[$id]) { print ' class="' . $classes_array[$id] .'"';  } ?>>
    <?php print $row; ?>
  </li>
<?php endforeach; ?>
</ul>
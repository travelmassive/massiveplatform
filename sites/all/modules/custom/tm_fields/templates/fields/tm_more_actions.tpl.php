<?php
/**
 * @file
 * Template for the more_actions block
 */
?>

<?php //<li data-dropd-wrapper> - Set with display suite ?>
  <a class="toggle settings bttn bttn-tertiary bttn-m hide-txt" href="#more-actions" data-dropd-toggle><span>More actions</span></a>
  <div id="more-actions" class="inner dropd dropd-s dropd-right" data-dropd>
    <?php foreach ($links as $section) : ?>
      <?php if (empty($section)) continue; ?>
      <ul class="dropd-menu">
        <?php foreach ($section as $link) : ?>
        <li class="<?php if (isset($link['wrapper_class'])) { print implode(' ', $link['wrapper_class']); } ?>">
          <?php print $link['content']; ?>
        </li>
        <?php endforeach; ?>
        
      </ul>
    <?php endforeach; ?>
  </div>
<?php //</li> - Set with display suite ?>

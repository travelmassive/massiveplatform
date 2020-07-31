<script>
jQuery(document).ready(function(){

	function load_marketplace_feedme() {

			feedme_base_url = "<?php global $conf; print($conf["tm_marketplace_frontpage_url"]);?>";
			feedme_limit = <?php print($conf["tm_marketplace_frontpage_limit"]);?>;
			feedme_url = feedme_base_url + "?limit=" + feedme_limit;
			
			jQuery.get(feedme_url, function(data) {

				jQuery("#frontpage_marketplace_feed").replaceWith(data);
				jQuery("#frontpage_marketplace_feed #feedme-placeholder").remove();

				// if marketplace_feedme_loaded() function defined, call it
				// this can be used to display a div when the feed loads
				if (typeof(marketplace_frontpage_feedme_loaded) == "function") {
					marketplace_frontpage_feedme_loaded();
				}
			});
	}

	load_marketplace_feedme(); // This will run on page load

});
</script>

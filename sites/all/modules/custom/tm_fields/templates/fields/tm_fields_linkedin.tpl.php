<?php

// take a "linkedin" URL and format it
// can take a variety of human entered formats

// fix user typos
$url = trim($url);
$url = str_replace(" ", "+", $url); // strip spaces
$url = str_replace(",", ".", $url); // replace , with .
$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url

$linkedin_url = "";
$display_url = "";
$found_match = false;

// linked in country codes
$country_codes = array("af","al","dz","ar","au","at","bh","bd","be","bo","ba","br","bg","ca","cl","cn","co","cr","hr","cy","cz","dk","do","ec","eg","sv","ee","fi","fr","de","gh","gr","gt","hk","hu","is","in","id","ir","ie","il","it","jm","jp","jo","kz","ke","kr","kw","lv","lb","lt","lu","mk","my","mt","mu","mx","ma","np","nl","nz","ng","no","om","pk","pa","pe","ph","pl","pt","pr","qa","ro","ru","sa","sg","sk","si","za","es","lk","se","ch","tw","tz","th","tt","tn","tr","ug","ua","ae","uk","uy","ve","vn","zw");

// Case 1
// linkedin url
if (!$found_match) {
	if ((strpos(strtolower($url), "https://linkedin.com/") !== FALSE) or (strpos($url, "http://linkedin.com/") !== FALSE)) {
		$linkedin_url = str_ireplace("https://linkedin.com/", "https://linkedin.com/", $url);
		$linkedin_url = str_ireplace("http://linkedin.com/", "https://linkedin.com/", $url);
		$found_match = true;
	}
	if ((strpos(strtolower($url), "https://www.linkedin.com/") !== FALSE) or (strpos($url, "http://www.linkedin.com/") !== FALSE)) {
		$linkedin_url = str_ireplace("https://www.linkedin.com/", "https://linkedin.com/", $url);
		$linkedin_url = str_ireplace("http://www.linkedin.com/", "https://linkedin.com/", $url);
		$found_match = true;
	}
	foreach ($country_codes as $country_code) {
		if ((strpos(strtolower($url), "https://" . $country_code . ".linkedin.com/") !== FALSE) or (strpos($url, "http://" . ".linkedin.com/") !== FALSE)) {
			$linkedin_url = str_ireplace("https://" . $country_code . ".linkedin.com/", "https://linkedin.com/", $url);
			$linkedin_url = str_ireplace("http://" . $country_code . ".linkedin.com/", "https://linkedin.com/", $url);
			$found_match = true;
		}
	}
	
}


// Case 2
// linkedin username
// Anything not starting with http or www
if (!$found_match) {
	if (($linkedin_url == "") and (strpos(strtolower($url), "http") === FALSE) and (strpos(strtolower($url), "www") === FALSE)) {
		$linkedin_url = "https://www.linkedin.com/in/" . $url;
		$found_match = true;
	}
}


// remove http and https
if ($display_url == "") {
	$display_url = $linkedin_url;
	$display_url = str_ireplace("http://", "", $display_url);
	$display_url = str_ireplace("https://", "", $display_url);
}

// sanitize
$linkedin_url = check_url($linkedin_url);
$display_url = check_plain($display_url);

if (($linkedin_url != "") && ($url != "")) { ?>
<a href="<?php print $linkedin_url; ?>" rel="nofollow" target="_blank"><?php print $display_url; ?></a>
<?php } else { ?>
<script>try { document.getElementsByClassName("field-link-linkedin")[0].style.display = 'none'; } catch(err) {};</script>
<?php } ?>
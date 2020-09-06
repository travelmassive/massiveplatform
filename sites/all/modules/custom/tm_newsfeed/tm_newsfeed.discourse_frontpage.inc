<?php

// tm_newsfeed.discourse_frontpage.inc

function tm_newsfeed_discourse_fetch_site() {

	global $conf;

	// check cache
	$cache_key = 'tm-newsfeed-discourse-fetch-site';
	if ($conf["tm_newsfeed_discourse_cache"] > 0) {
		$cache = cache_get($cache_key, 'cache');
		if (!empty($cache)) {
			try {
				return json_decode($cache->data);
			} catch (Exception $e) {
				return array();
			}
		}
	}

	//open connection
	$ch = curl_init();

	// timeout
	$curl_timeout = $conf["tm_newsfeed_curl_timeout"];

	// url
	// use /latest.json api
	$url = $conf["tm_newsfeed_discourse_base_url"] . "/site.json";

	//set the url, number of POST vars, POST data
	$CURLOPT_SSL_VERIFYHOST = 0; 
	if ($conf["tm_newsfeed_curl_check_certificate"]) {
		$CURLOPT_SSL_VERIFYHOST = 2;
	}
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

	// basic http auth
	if (isset($conf["tm_newsfeed_discourse_api_userpass"])) {
		curl_setopt($ch, CURLOPT_USERPWD, $conf["tm_newsfeed_discourse_api_userpass"]);
	}
	curl_setopt($ch, CURLOPT_VERBOSE, false);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
	curl_setopt($ch, CURLOPT_TIMEOUT, $curl_timeout); // timeout in seconds

	//execute post
	$result = curl_exec($ch);

	$curl_error = "";
	if ($result === false) {
		$curl_error = "CURL error: " . curl_error($ch);
	}

	//close connection
	curl_close($ch);

	// return api data in json format
	if ($conf["tm_newsfeed_discourse_cache"] > 0) {
		cache_set($cache_key, $result, 'cache', time() + $conf["tm_newsfeed_discourse_cache"]);
	}
	
	// return result
	try {
		return json_decode($result);
	} catch (Exception $e) {
		return array();
	}
	
}

// render front page feed of latest discussions
function tm_newsfeed_discourse_render_frontpage_feed($max_topics = 5, $use_cache = true) {

	global $conf;

	if ($use_cache) {
		// check cache
		$cache = cache_get('tm-newsfeed-discourse-frontpage-feed', 'cache');
		if (!empty($cache)) {
			return $cache->data;
		}
	}

	$base_url = $conf["tm_newsfeed_discourse_base_url"];
	$topic_count = 0;
	$item_html = "";

	// get latest posts
	$results = tm_newsfeed_discourse_fetch();

	// create avatar and usernames lookup
	$user_avatars = array();
	$user_names = array();
	foreach($results->users as $result_user) {
		$user_avatars[$result_user->id] = $result_user->avatar_template;
		$user_names[$result_user->id] = $result_user->name;
	}

	// get categories
	$site_results = tm_newsfeed_discourse_fetch_site();
	$categories = array();
	foreach ($site_results->categories as $category) {
		$categories[$category->id] = $category;

	}

	// map parent categories
	$parent_categories = array();
	foreach ($site_results->categories as $category) {
		if (isset($category->parent_category_id)) {
			$parent_categories[$category->id] = $categories[$category->parent_category_id];
		} else {
			$parent_categories[$category->id] = $category;
		}
	}

	try {

		// get topics
		$topics = $results->topic_list->topics;
		if (count($topics) < 1) {
			echo "";
		}

		// get topics with at least one reply, or at least one like
		foreach($topics as $topic) {
			if ($topic_count >= $max_topics) {
				continue;
			}
			// check if ignoring topic
			if (isset($conf['tm_newsfeed_discourse_ignore_topics'])) {
				if (in_array($topic->id, $conf['tm_newsfeed_discourse_ignore_topics'])) {
					continue;
				}	
			}
			if (($topic->posts_count > 1) or ($topic->like_count > 0)) {

				// post count
				if ($topic->posts_count == 2) {
					$replies_label = "1 reply";
				} elseif ($topic->posts_count > 2) {
					$replies_label = "" . ($topic->posts_count - 1) . " replies";
				} else {
					$replies_label = "Be the first to reply";
				}

				// category
				$category_name = strip_tags($parent_categories[$topic->category_id]->name);
				$category_color = $parent_categories[$topic->category_id]->color;
				$category_text_color = $parent_categories[$topic->category_id]->text_color;
				$category_url = '/discourse/redirect_to_forum?forum_destination=' . $parent_categories[$topic->category_id]->topic_url;
				$category_html = '<span class="feedme-discussions-category">' . $category_name . '</span>'; 

				$href = '/discourse/redirect_to_forum?forum_destination=/t/' . $topic->slug . "/" . $topic->id;
				$title = strip_tags($topic->title);
				$logo = "";
				$clean_description = "";

				$post_image_url = $topic->image_url;
				$post_image_alt = "Topic thumbnail";
				$image_class = '';
				if ($post_image_url == null) {
					$post_image_url = $base_url . $user_avatars[$topic->posters[0]->user_id];
					$post_image_url = str_replace("{size}", 135, $post_image_url);
					$post_image_alt = strip_tags($user_names[$topic->posters[0]->user_id]);
					$post_image_alt = str_replace("\"", "", $post_image_alt);
					$image_class = 'feedme-image-avatar';
				}

				$item_html .= '<li class="feedme">
               <article class="card contained feedme view-mode-grid clearfix">
                    <a href="'. $href .'" class="feedme">
                        <div class="media">
                            <div class="avatar">
                                <span class="badge-discussions-feedme">
                                    <img class="feedme-image-discussions ' . $image_class . '" typeof="foaf:Image" alt="' . $post_image_alt . '" src="'. $post_image_url .'">
                                </span>
                            </div>
                        </div>
                        <div class="teaser">
                            <h1 class="prime-title feedme">'. $title .'</h1>
                            <p class="meta feedme">' . $category_html . '<span class="role">'. $replies_label . '</span></p>
                        </div>
                    </a>
                </article>
            </li>';

				$topic_count ++;
			}
		}
	} catch (Exception $e) {
		// do nothing
	}

	// template
	$template = <<<TEMPLATE

<section class="contained contained-block feedme-discussions">
    <header class="contained-head">
        <h1 class="prime-title feedme top">Community Discussions</h1>
    </header>
    <div class="contained-body">
        <ul class="discussions-feedme-list related-list">
            {$item_html}
        </ul>
    </div>
    <div class="more-link" style="font-size: 14px;">
    <a href="/discourse/redirect_to_forum" class="feedme more">View all topics</a>
    </div>
</section>
TEMPLATE;

	// check for empty results
	if ($item_html == '') {
		$template = "";
	}

	// store cache
	cache_set('tm-newsfeed-discourse-frontpage-feed', $template, 'cache', time() + 300); // 5 mins

	// return result
	echo $template;
}


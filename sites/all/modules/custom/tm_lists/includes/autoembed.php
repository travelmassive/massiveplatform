<?php
/**
 * Class to parse URLs & embed remote content vie oEmbed
 * Borrowed heavily from WordPress. Thanks!
 *
 * @link 	http://oembed.com
 * @link	https://gist.github.com/joshhartman/5380593
 * @link	http://www.warpconduit.net/2013/04/13/automatically-embedding-video-using-only-the-url-with-the-help-of-oembed
 * @link	https://github.com/MarQuisKnox/AutoEmbed
 * 
 * @example
 * 
 * $AutoEmbed	= new AutoEmbed(); 
 * $content	= $Autoembed->parse($content);
 * echo $content;
 * 
 * @package AutoEmbed
*/

class AutoEmbed 
{
	public $embedHeight	= 320;
	public $embedWidth	= 568; // 16:9
	public $useCache = true;
	public $listId = 0; // list id for caching
	
    public $providers = array(
            '#http://(www\.)?youtube\.com/watch.*#i'				=> array( 'https://www.youtube.com/oembed', true ),
            '#https://(www\.)?youtube\.com/watch.*#i'				=> array( 'https://www.youtube.com/oembed?scheme=https', true ),
            '#http://(www\.)?youtube\.com/playlist.*#i'				=> array( 'https://www.youtube.com/oembed', true ),
            '#https://(www\.)?youtube\.com/playlist.*#i'			=> array( 'https://www.youtube.com/oembed?scheme=https', true ),
            '#http://youtu\.be/.*#i'								=> array( 'https://www.youtube.com/oembed', true ),
            '#https://youtu\.be/.*#i'								=> array( 'https://www.youtube.com/oembed?scheme=https', true ),
            '#https?://(.+\.)?vimeo\.com/.*#i'						=> array( 'http://vimeo.com/api/oembed.{format}', true ),
            '#https?://(www\.)?soundcloud\.com/.*#i'				=> array( 'http://soundcloud.com/oembed', true ),
            '#https?://(www\.)?instagr(\.am|am\.com)/p/.*#i'        => array( 'http://api.instagram.com/oembed?hidecaption=true&url=', true ),
            '#https?://www\.facebook\.com/.*/posts/.*#i'            => array( 'https://www.facebook.com/plugins/post/oembed.json/', true  ),
            '#https?://www\.facebook\.com/.*/activity/.*#i'         => array( 'https://www.facebook.com/plugins/post/oembed.json/', true  ),
            '#https?://www\.facebook\.com/.*/photos/.*#i'           => array( 'https://www.facebook.com/plugins/post/oembed.json/', true  ),
            '#https?://www\.facebook\.com/photo(s/|\.php).*#i'      => array( 'https://www.facebook.com/plugins/post/oembed.json/', true  ),
            '#https?://www\.facebook\.com/permalink\.php.*#i'       => array( 'https://www.facebook.com/plugins/post/oembed.json/', true  ),
            '#https?://www\.facebook\.com/media/.*#i'               => array( 'https://www.facebook.com/plugins/post/oembed.json/', true  ),
            '#https?://www\.facebook\.com/questions/.*#i'           => array( 'https://www.facebook.com/plugins/post/oembed.json/', true  ),
            '#https?://www\.facebook\.com/notes/.*#i'               => array( 'https://www.facebook.com/plugins/post/oembed.json/', true  ),
            '#https?://www\.facebook\.com/.*/videos/.*#i'           => array( 'https://www.facebook.com/plugins/video/oembed.json/', true  ),
            '#https?://www\.facebook\.com/video\.php.*#i'           => array( 'https://www.facebook.com/plugins/video/oembed.json/', true  ),
            '#https://(www.)tiktok.com/@.*/video/.*#'				=> array( 'https://www.tiktok.com/oembed', true),
            '#https?://open.spotify.com/(track|episode|show)/.*#'		=> array( 'https://embed.spotify.com/oembed?url=', true),
            '#https://directory.libsyn.com/episode/index/show/.*/id/.*#'	=> array( 'https://oembed.libsyn.com/?item_id=', true),
            '#https://html5-player.libsyn.com/embed/episode/id/.*#' => array( 'https://oembed.libsyn.com/?item_id=', true),
            '#https?://(.*)\.podbean\.com/e/.*#i' => array( 'https://www.podbean.com/media/oembed?url=', true ),
    );
    
    public function getEmbedHeight()
    {
    	return $this->embedHeight;
    }
    
    public function setEmbedHeight( $height )
    {
    	$this->embedHeight = $height;
    }    
    
    public function getEmbedWidth()
    {
    	return $this->embedWidth;	
    }
    
    public function setEmbedWidth( $width )
    {
    	$this->embedWidth = $width;
    }    

    public function getUseCache()
    {
    	return $this->useCache;
    }    

    public function setUseCache( $useCache )
    {
    	$this->useCache = $useCache;
    } 

 	public function getListId()
    {
    	return $this->listId;
    }    

    public function setListId( $listId )
    {
    	$this->listId = $listId;
    } 


    
    /**
     * Passes on any unlinked URLs that are on their own line for potential embedding.
     * @param string $content The content to be searched.
     * @return string Potentially modified $content.
    */
    public function parse( $content ) 
    {
        return preg_replace_callback( '|^\s*(https?://[^\s"]+)\s*$|im', array( $this, 'autoembed_callback' ), $content );
    }

    /**
     * Callback function for @link AutoEmbed::parse()
     * 
     * @param	array	$match	a regex match array
     * @param	boolean	$autoDiscover
     * @return	string	The embed HTML on success, otherwise the original URL.
    */
    public function autoembed_callback( $match, $autoDiscover = false )
    {
    	$params				= array();
        $params['discover']	= $autoDiscover;
        $return				= $this->get_html( $match[1], $params );
        
        return "\n".$return."\n";
    }


    /**
     * The do-it-all function that takes a URL and attempts to return the HTML.
     * 
     * @param	string	$url	The URL to the content that should be attempted to be embedded.
     * @param	array	$args	Optional arguments.
     * @return	string	the source URL on failure, otherwise the UNSANITIZED (and potentially unsafe) HTML that should be used to embed.
    */
    public function get_html( $url, $args = array() ) 
    {
		$provider = false;

		if ( !isset( $args['discover'] ) ) {
        	$args['discover'] = false;
        }                    

        $i = 0;

        foreach ( $this->providers AS $matchmask => $data ) {
            $i++;

        	list( $providerurl, $regex ) = $data;

            // Turn the asterisk-type provider URLs into regex
            if ( !$regex ) {
            	$matchmask = '#' . str_replace( '___wildcard___', '(.+)', preg_quote( str_replace( '*', '___wildcard___', $matchmask ), '#' ) ) . '#i';
                $matchmask = preg_replace( '|^#http\\\://|', '#https?\://', $matchmask );
            }

			if ( preg_match( $matchmask, $url ) ) {
            	$provider = str_replace( '{format}', 'json', $providerurl ); // JSON is easier to deal with than XML

                break;
            }
		}

        if ( !$provider && $args['discover'] ) {
        	$provider = $this->discover( $url );
        }

        // libsyn
		// ie: https://directory.libsyn.com/episode/index/show/countingcountries/id/12668033
        if ((strpos($url, "directory.libsyn.com")) !== false) {
        	$parts = explode("/", $url);
        	$last = $parts[sizeof($parts) -1];
        	$args['libsyn_item_id'] = $last;
        }

        // https://html5-player.libsyn.com/embed/episode/id/12764438/etc..
		if ((strpos($url, "html5-player.libsyn.com")) !== false) {
        	$parts = explode("/", $url);
        	if (isset($parts[6])) {
				$args['libsyn_item_id'] = $parts[6];
        	}        	
        }

        if ( !$provider || false === $data = $this->fetch( $provider, $url, $args ) ) {
        	return $url;     	
        }

        return $this->data2html( $data, $url );
    }

    /**
     * Attempts to find oEmbed provider discovery <link> tags at the given URL.
     *
     * @param string $url The URL that should be inspected for discovery <link> tags.
     * @return bool|string False on failure, otherwise the oEmbed provider URL.
    */
    public function discover( $url ) 
    {
            $providers = array();

            // Fetch URL content
            if ( $html = $this->my_remote_get( $url ) ) {

                    // <link> types that contain oEmbed provider URLs
                    $linktypes = array(
                        'application/json+oembed' => 'json',
                        'text/xml+oembed' => 'xml',
                        'application/xml+oembed' => 'xml', // Incorrect, but used by at least Vimeo
                    );

                    // Strip <body>
                    $html = substr( $html, 0, stripos( $html, '</head>' ) );

                    // Do a quick check
                    $tagfound = false;
                    foreach ( $linktypes as $linktype => $format ) {
                            if ( stripos($html, $linktype) ) {
                                    $tagfound = true;
                                    break;
                            }
                    }

                    if ( $tagfound && preg_match_all( '/<link([^<>]+)>/i', $html, $links ) ) {
                            foreach ( $links[1] as $link ) {
                                    $atts = $this->shortcode_parse_atts( $link );

                                    if ( !empty($atts['type']) && !empty($linktypes[$atts['type']]) && !empty($atts['href']) ) {
                                            $providers[$linktypes[$atts['type']]] = $atts['href'];

                                            // Stop here if it's JSON (that's all we need)
                                            if ( 'json' == $linktypes[$atts['type']] )
                                                    break;
                                    }
                            }
                    }
            }

            // JSON is preferred to XML
            if ( !empty($providers['json']) )
                    return $providers['json'];
            elseif ( !empty($providers['xml']) )
                    return $providers['xml'];
            else
                    return false;
    }

    /**
     * Connects to a oEmbed provider and returns the result.
     *
     * @param	string	$provider The URL to the oEmbed provider.
     * @param	string	$url The URL to the content that is desired to be embedded.
     * @param	array	$args Optional arguments.
     * @return	bool|object False on failure, otherwise the result in the form of an object.
    */
    public function fetch( $provider, $url, $args = array() ) 
    {
		$width	= $this->embedWidth;
		$height = $this->embedHeight;
		$args	= array_merge( compact('width', 'height'), $args );

		$provider = $this->add_query_arg( 'maxwidth', (int) $args['width'], $provider );
		$provider = $this->add_query_arg( 'maxheight', (int) $args['height'], $provider );
		$provider = $this->add_query_arg( 'url', $url, $provider );
		
		// set ?item_id with libsyn
		if (isset($args['libsyn_item_id'])) {
			$provider = $this->add_query_arg( 'item_id', $args['libsyn_item_id'], $provider );
		}

		foreach( array( 'json', 'xml' ) AS $format ) {
			$result = $this->_fetch_with_format( $provider, $format );

			return $result;
		}
            
		return false;
    }

    /**
     * Fetches result from an oEmbed provider for a specific format and complete provider URL
     * @access private
     * @param string $provider_url_with_args URL to the provider with full arguments list (url, maxheight, etc.)
     * @param string $format Format to use
     * @return bool|object False on failure, otherwise the result in the form of an object.
    */
    private function _fetch_with_format( $provider_url_with_args, $format ) 
    {
        $provider_url_with_args = $this->add_query_arg( 'format', $format, $provider_url_with_args );

        if ( !$body = $this->my_remote_get( $provider_url_with_args ) ) {
            return false;
        }

        $parse_method = "_parse_$format";

        return $this->$parse_method( $body );
    }

    /**
     * Parses a json response body.
     * @access private
    */
    private function _parse_json( $response_body ) 
    {
        return ( ( $data = json_decode( trim( $response_body ) ) ) && is_object( $data ) ) ? $data : false;
    }

    /**
     * Parses an XML response body.
     * @access private
    */
    private function _parse_xml( $response_body ) 
    {
            if ( !function_exists('simplexml_load_string') ) {
                    return false;
            }

            if ( ! class_exists( 'DOMDocument' ) )
                    return false;

            $errors = libxml_use_internal_errors( true );
            $old_value = null;
            if ( function_exists( 'libxml_disable_entity_loader' ) ) {
                    $old_value = libxml_disable_entity_loader( true );
            }

            $dom = new DOMDocument;
            $success = $dom->loadXML( $response_body );

            if ( ! is_null( $old_value ) ) {
                    libxml_disable_entity_loader( $old_value );
            }
            libxml_use_internal_errors( $errors );

            if ( ! $success || isset( $dom->doctype ) ) {
                    return false;
            }

            $data = simplexml_import_dom( $dom );
            if ( ! is_object( $data ) )
                    return false;

            $return = new stdClass;
            foreach ( $data as $key => $value )
                    $return->$key = (string) $value;
            return $return;
    }

    /**
     * Converts a data object and returns the HTML.
     *
     * @param object $data A data object result from an oEmbed provider.
     * @param string $url The URL to the content that is desired to be embedded.
     * @return bool|string False on error, otherwise the HTML needed to embed.
    */
    public function data2html( $data, $url, $responsive = true ) 
    {
            if ( !is_object( $data ) || empty( $data->type ) ) {
                return false;
            }

            $return = false;

            switch ( $data->type ) {
                    case 'photo':
                            if ( empty( $data->url ) || empty( $data->width ) || empty( $data->height ) )
                                    break;
                            if ( ! is_string( $data->url ) || ! is_numeric( $data->width ) || ! is_numeric( $data->height ) )
                                    break;

                            $title = ! empty( $data->title ) && is_string( $data->title ) ? $data->title : '';
                            $return = '<a href="' . $this->esc_url( $url ) . '"><img src="' . htmlspecialchars( $data->url, ENT_QUOTES, 'UTF-8' ) . '" alt="' . htmlspecialchars($title, ENT_QUOTES, 'UTF-8') . '" width="' . htmlspecialchars($data->width, ENT_QUOTES, 'UTF-8') . '" height="' . htmlspecialchars($data->height, ENT_QUOTES, 'UTF-8') . '" /></a>';
                            break;

                    case 'video':
                    case 'rich':
                            if ( ! empty( $data->html ) && is_string( $data->html ) ) 

                                // no lazyloading for Facebook, TikTok
                                if ((strpos($url, 'facebook.com/') !== false) or
                                   (strpos($url, 'tiktok.com/') !== false)) {
                                    return $data->html;
                                }

                                // don't lazyload instagram
                                // run instagram embed js
                                // https://developers.facebook.com/docs/instagram/embedding/
                                if (strpos($url, 'instagram.com/') !== false) {
                                    return $data->html . "<script>if (typeof(instgrm) !== 'undefined') { instgrm.Embeds.process(); }</script>";
                                }
                                
                                // lazy loading for everything else
                            	return str_replace("src=", "class=\"lazyload\" data-src=", $data->html);
                            break;

                    case 'link':
                            if ( ! empty( $data->title ) && is_string( $data->title ) )
                                    $return = '<a href="' . $this->esc_url( $url ) . '">' . htmlspecialchars( $data->title, ENT_QUOTES, 'UTF-8') . '</a>';
                            break;

                    default:
                            $return = false;
            }

            // Strip any new lines from the HTML.
            if ( false !== strpos( $return, "\n" ) )
                    $return = str_replace( array( "\r\n", "\n" ), '', $return );

            if( $responsive ) {
                $return = $this->makeEmbedResponsive( $return );
            }
            
            return $return;
    }
    
    /**
     * Grabs the response from a remote URL.
     *
     * @param   string  $url            The remote URL.
     * @param   boolean $followLocation CURL_FOLLOWLOCATION
     * @return  bool|string False on error, otherwise the response body.
    */
    public function my_remote_get( $url, $followLocation = true)
    {

        // check cache
        $cache_key = "tm-lists-oembed-" . $this->listId . "_" . md5($url);
        $cache = cache_get($cache_key, 'cache');
        if ($this->useCache) {
                if (!empty($cache)) {
                        return $cache->data;
                }
        }

        // set browser
        $browser = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.16 (KHTML, like Gecko) \Chrome/24.0.1304.0 Safari/537.16';

        // Facebook wants a different browser
        if (strpos($url, "facebook.com/plugins") !== false) {
            $browser = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36';
        }

        $handle = curl_init();

        curl_setopt( $handle, CURLOPT_CONNECTTIMEOUT, 20 );
        curl_setopt( $handle, CURLOPT_TIMEOUT, 20 );
        curl_setopt( $handle, CURLOPT_URL, $url);
        curl_setopt( $handle, CURLOPT_RETURNTRANSFER, true );
        curl_setopt( $handle, CURLOPT_SSL_VERIFYHOST, false );
        curl_setopt( $handle, CURLOPT_SSL_VERIFYPEER, false );
        curl_setopt( $handle, CURLOPT_FOLLOWLOCATION, $followLocation );
        curl_setopt( $handle, CURLOPT_HEADER, false );
        // curl_setopt( $handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0 );
        curl_setopt( $handle, CURLOPT_USERAGENT, $browser);

        $response = curl_exec( $handle );

        curl_close( $handle );

        // set cache
        cache_set($cache_key, $response, 'cache', time() + (60 * 60 * 24)); // cache for 1 day

        return $response;
    }

    /**
     * Add HTTP query arguments.
    */
    public function add_query_arg() 
    {
            $ret = '';
            $args = func_get_args();
            if ( is_array( $args[0] ) ) {
                    if ( count( $args ) < 2 || false === $args[1] )
                            $uri = $_SERVER['REQUEST_URI'];
                    else
                            $uri = $args[1];
            } else {
                    if ( count( $args ) < 3 || false === $args[2] )
                            $uri = $_SERVER['REQUEST_URI'];
                    else
                            $uri = $args[2];
            }

            if ( $frag = strstr( $uri, '#' ) )
                    $uri = substr( $uri, 0, -strlen( $frag ) );
            else
                    $frag = '';

            if ( 0 === stripos( 'http://', $uri ) ) {
                    $protocol = 'http://';
                    $uri = substr( $uri, 7 );
            } elseif ( 0 === stripos( 'https://', $uri ) ) {
                    $protocol = 'https://';
                    $uri = substr( $uri, 8 );
            } else {
                    $protocol = '';
            }

            if ( strpos( $uri, '?' ) !== false ) {
                    $parts = explode( '?', $uri, 2 );
                    if ( 1 == count( $parts ) ) {
                            $base = '?';
                            $query = $parts[0];
                    } else {
                            $base = $parts[0] . '?';
                            $query = $parts[1];
                    }
            } elseif ( $protocol || strpos( $uri, '=' ) === false ) {
                    $base = $uri . '?';
                    $query = '';
            } else {
                    $base = '';
                    $query = $uri;
            }

            parse_str( $query, $qs );
            if ( is_array( $args[0] ) ) {
                    $kayvees = $args[0];
                    $qs = array_merge( $qs, $kayvees );
            } else {
                    $qs[ $args[0] ] = $args[1];
            }

            foreach ( $qs as $k => $v ) {
                    if ( $v === false )
                            unset( $qs[$k] );
            }
            $ret = http_build_query( $qs, null, '&' );
            $ret = trim( $ret, '?' );
            $ret = preg_replace( '#=(&|$)#', '$1', $ret );
            $ret = $protocol . $base . $ret . $frag;
            $ret = rtrim( $ret, '?' );
            return $ret;
    }
    
    /**
     * Checks and cleans a URL.
     * 
     * @param string $url The URL to be cleaned.
     * @return string The cleaned $url.
    */
    public function esc_url( $url ) 
    {

            if ( '' == $url )
                    return $url;
            $url = preg_replace('|[^a-z0-9-~+_.?#=!&;,/:%@$\|*\'()\\x80-\\xff]|i', '', $url);
            $strip = array('%0d', '%0a', '%0D', '%0A');
            $url = $this->_deep_replace($strip, $url);
            $url = str_replace(';//', '://', $url);
            /* If the URL doesn't appear to contain a scheme, we
             * presume it needs http:// appended (unless a relative
             * link starting with /, # or ? or a php file).
             */
            if ( strpos($url, ':') === false && ! in_array( $url[0], array( '/', '#', '?' ) ) &&
                    ! preg_match('/^[a-z0-9-]+?\.php/i', $url) )
                    $url = 'http://' . $url;
            
            $url = str_replace( '&amp;', '&#038;', $url );
            $url = str_replace( "'", '&#039;', $url );

            return $url;
    }
    
    /**
     * Perform a deep string replace operation to ensure the values in $search are no longer present
     *
     * Repeats the replacement operation until it no longer replaces anything so as to remove "nested" values
     * e.g. $subject = '%0%0%0DDD', $search ='%0D', $result ='' rather than the '%0%0DD' that
     * str_replace would return
     *
     * @access private
     *
     * @param string|array $search
     * @param string $subject
     * @return string The processed string
    */
    private function _deep_replace( $search, $subject ) 
    {
            $found = true;
            $subject = (string) $subject;
            while ( $found ) {
                    $found = false;
                    foreach ( (array) $search as $val ) {
                            while ( strpos( $subject, $val ) !== false ) {
                                    $found = true;
                                    $subject = str_replace( $val, '', $subject );
                            }
                    }
            }

            return $subject;
    }
    
    /**
     * Retrieve all attributes from the tag.
     *
     * @param string $text
     * @return array List of attributes and their value.
    */
    public function shortcode_parse_atts( $text ) 
    {
            $atts = array();
            $pattern = '/(\w+)\s*=\s*"([^"]*)"(?:\s|$)|(\w+)\s*=\s*\'([^\']*)\'(?:\s|$)|(\w+)\s*=\s*([^\s\'"]+)(?:\s|$)|"([^"]*)"(?:\s|$)|(\S+)(?:\s|$)/';
            $text = preg_replace("/[\x{00a0}\x{200b}]+/u", " ", $text);
            if ( preg_match_all($pattern, $text, $match, PREG_SET_ORDER) ) {
                    foreach ($match as $m) {
                            if (!empty($m[1]))
                                    $atts[strtolower($m[1])] = stripcslashes($m[2]);
                            elseif (!empty($m[3]))
                                    $atts[strtolower($m[3])] = stripcslashes($m[4]);
                            elseif (!empty($m[5]))
                                    $atts[strtolower($m[5])] = stripcslashes($m[6]);
                            elseif (isset($m[7]) and strlen($m[7]))
                                    $atts[] = stripcslashes($m[7]);
                            elseif (isset($m[8]))
                                    $atts[] = stripcslashes($m[8]);
                    }
            } else {
                    $atts = ltrim($text);
            }
            return $atts;
    }

    
    /**
     * Make an embed responsive
     *
     * @param   string  $string
     * @param   string  $targetWidth
     * @return  string
    */
    public function makeEmbedResponsive( $string, $targetWidth = '100%' )
    {
        $string = preg_replace(
            array( '/width="\d+"/i' ),
            array( sprintf( 'width="%s"', $targetWidth ) ),
            $string
        );
    
        return $string;
    }    
}
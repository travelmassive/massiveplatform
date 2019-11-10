<div class="search box">
	<div class="container-inline form-wrapper" id="edit-basic">
		<h1>Search our community.</h1>
		<div class="form-item form-type-textfield form-item-keys">
			<input style="margin: 0 auto; max-width: 315px;" type="text" id="search-query" name="query" placeholder="Enter a name, city, event or company ..." value="" size="40" maxlength="255" class="form-text">
		</div>
		<div>
			<button class="post-button-blue" id="search-submit" name="search-submit" value="Search">Search</button>
		</div>
	</div>
</div>

<div class="search login_message">
	<p>Please <a href='javascript:void(0);' onClick='javascript:jq_login_signup_box();'>log in</a> or <a href='javascript:void(0);' onClick='jq_login_signup_box();'>sign up</a>.</p>
</div>

<div class="search welcome" style="display: none;">
	<p>__SEARCH_WELCOME_MESSAGE__</p>
</div>

<div class="search empty_query" style="display: none;">
	<p>Please enter keywords to search. Example: <a href='#' class='search-example' data-search-example='__EXAMPLE_SEARCH__'>__EXAMPLE_SEARCH__</a></p>
</div>

<div class="search spinner" style="display: none;">
	<p><img src='/sites/all/themes/tm/images/load-more-ajax-loader-2.gif'></p>
</div>

<div class="search tips">
	__SEARCH_TIPS__
</div>

<div class="search timeout" style="display: none;">
	<p>Hang on, still working...</p>
</div>

<div class="search error-message" style="display: none;">
	<p>Sorry, this search encountered a problem. Try again later.</p>
</div>

<div class="search info">
	<p id="search-results-text" style="display:none;">Found 5 results</p>
	<p id="search-results-loading-more" style="display:none;"><img align="left" style="margin-top: 6px; padding-right: 8px;" src='/sites/all/themes/tm/images/load-more-ajax-loader-2.gif'> Updating results...
	</p>
</div>

<div class="search container">

	<div class="search filters">
		<section style="padding-left: 1.5rem;" class="contained contained-block">
			<p><strong>Filter results</strong></p>
			<ul class="filter">
				
				<li class="filter">
					<input type="checkbox" id="search-filter-people" name="search-filter-people">
					<label class="label-filter" for="search-filter-people">People</label>
				</li>
				<li class="filter">
					<input type="checkbox" id="search-filter-companies" name="search-filter-companies">
					<label class="label-filter" for="search-filter-companies">Companies</label>
				</li>
				<li class="filter">
					<input type="checkbox" id="search-filter-chapters" name="search-filter-chapters">
					<label class="label-filter" for="search-filter-chapters">Chapters</label>
				</li>
				<li class="filter">
					<input type="checkbox" id="search-filter-events" name="search-filter-events">
					<label class="label-filter" for="search-filter-events">Events</label>
				</li>
				<li class="filter">
					<input type="checkbox" id="search-filter-past-events" name="search-filter-past-events">
					<label class="label-filter" for="search-filter-past-events">Past Events</label>
				</li>
				
			</ul>

		</section>

		<section style="padding-left: 1.5rem;" class="contained contained-block search-help">
			__SEARCH_HELP_TEXT__
		</section>

		<section style="padding-left: 1.5rem;" class="contained contained-block search-help">
			__SEARCH_HELP_TIPS__
		</section>

	</div>

	<div class="search placeholder">
		<div class="search results">
			<section class="contained contained-block search-result">
				<div class="contained-body search-result" style="margin-top: 16px; margin-bottom: -16px;">
					<ul class="user-list related-list" id="search-results">
					</ul>
				</div>
			</section>
		</div>

		<div class="search noresults">
			<p>
				__NO_RESULTS_TEXT__
			</p>
		</div>

	</div>

</div>

<div style="clear:both;">

<div class="item-list">
	<ul class="pager pager-load-more" style="display:none;">
		<li class="pager-next first last">
			<a href="javascript:void(0);" id="search-load-more">Show more</a>
			<div style="min-height: 20px; margin-top: 4px;">
				<img class="search load-more-spinner" style="display: none;"src='/sites/all/themes/tm/images/load-more-ajax-loader-2.gif'>
			</span>
		</li>
	</ul>
</div>



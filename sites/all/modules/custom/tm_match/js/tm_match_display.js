(function ($, Drupal, window, document, undefined) {

	jQuery(document).ready(function() {

	// some globals to keep track of stuff
	var tm_match_base_url = "/match";
	var tm_match_fetching_new_cards = false;
	var tm_match_current_uid = jQuery("#match-user-uid").val(); // Drupal.settings.tm_users.current_user_uid;
	var tm_match_load_more = jQuery("#match-load-more").val();
	var tm_its_a_match = false;

	// cards
	var animating = false;
	var cardsCounter = 0;
	var numOfCards = 5;
	var decisionVal = 80;
	var pullDeltaX = 0;
	var deg = 0;
	var $card, $cardReject, $cardLike;

	function pullChange() {
		
		animating = true;
		deg = pullDeltaX / 10;
		$card.css("transform", "translateX("+ pullDeltaX +"px) rotate("+ deg +"deg)");

		var opacity = pullDeltaX / 100;
		var rejectOpacity = (opacity >= 0) ? 0 : Math.abs(opacity);
		var likeOpacity = (opacity <= 0) ? 0 : opacity;
		$cardReject.css("opacity", rejectOpacity);
		$cardLike.css("opacity", likeOpacity);
		jQuery(".tm_match__card__more_link").hide();

	};

	function release() {

		jQuery("#match-debug-message").val(Math.abs(pullDeltaX) + " >= " + decisionVal);
		
		var card_uid = $card.data("card-uid");

		if (pullDeltaX >= decisionVal) {
			$card.addClass("to-right");
			var card_first_name = $card.data("card-first-name");
			swipeCallback("right", tm_match_current_uid, card_uid);
			$card.addClass("tm_match_disable_move");
			
			var follows_you_back = $card.data("follows-you-back");
			if (follows_you_back) {
				itsAMatch(card_uid, card_first_name);
			} else {
				confetti.start();
				setTimeout(function() { 
					confetti.stop();
				}, 1000);
			}

		} else if (pullDeltaX <= -decisionVal) {
			$card.addClass("to-left");
			swipeCallback("left", tm_match_current_uid, card_uid);
			$card.addClass("tm_match_disable_move");
		}

		if (Math.abs(pullDeltaX) >= decisionVal) {
			$card.addClass("inactive");

			// remove more link
			$card.children(".tm_match__card__more_link").remove();

			setTimeout(function() {
				$card.addClass("below").removeClass("inactive to-left to-right");
				cardsCounter++;
				var card_load_more = $card.data("card-load-more");
				var is_last_card = $card.data("is-last-card");
				if ((is_last_card == true) && (tm_match_load_more == "1")) {
					fetchNewCards();
					//jQuery(".tm_match__card").removeClass("below");
				}
			}, 300);
		}

		if (Math.abs(pullDeltaX) < decisionVal) {
			$card.addClass("reset");
		}

		setTimeout(function() {
		  $card.attr("style", "").removeClass("reset").find(".tm_match__card__choice").attr("style", "");
		  pullDeltaX = 0;
		  animating = false;
		  if (!tm_match_fetching_new_cards) {
		  	jQuery(".tm_match__card__more_link").delay(500).fadeIn();
		  }
		}, 300);

	};

	jQuery(document).on("mousedown touchstart", ".tm_match__card:not(.inactive)", function(e) {
		
		if (animating) {
			return;
		}

		$card = jQuery(this);

		// card must have swipe enabled
		if (!$card.data("swipe-enabled")) {
			return;
		}

		$cardReject = jQuery(".tm_match__card__choice.m--reject", $card);
		$cardLike = jQuery(".tm_match__card__choice.m--like", $card);
		var startX =  e.pageX || e.originalEvent.touches[0].pageX;

		jQuery(document).on("mousemove touchmove", function(e) {
		  var x = e.pageX || e.originalEvent.touches[0].pageX;
		  pullDeltaX = (x - startX);
		  if (!pullDeltaX) return;
		  pullChange();
		});

		jQuery(document).on("mouseup touchend", function() {
		  jQuery(document).off("mousemove touchmove mouseup touchend");
		  if (!pullDeltaX) return; // prevents from rapid click events
		  release();
		});

	});

	// Register swipe callback
	function swipeCallback(left_or_right, uid, left_uid) {

		xhr = jQuery.ajax({
			url: tm_match_base_url + "/callback/" + left_or_right + "/" + uid + "/" + left_uid,
			type: "GET",
			success: function(response) {
			},
			error: function(xhr) {
				// This is also reached when we cancel the xhr request
			}
		});
	}

	// Its a match
	function itsAMatch(card_uid, first_name) {
		
		tm_its_a_match = true;

		// scroll to top
		window.scroll({
			top: 0, 
			left: 0, 
			behavior: 'auto'
		});

		// set fields
		jQuery(".tm_its_a_match_first_name").text(first_name);
		jQuery(".tm_its_a_match_url").attr("href", "/user/" + card_uid);
		jQuery(".tm_its_a_match_container").show();

		// celebrate
		confetti.start();
	}

	// Continue matching
	continueMatching = function() {
		tm_its_a_match = false;
		jQuery(".tm_its_a_match_container").fadeOut();
		confetti.stop();
	}

	// Fetch new cards
	function fetchNewCards() {

		// Check not already fetching
		if (tm_match_fetching_new_cards) {
			return;
		}

		// Reset its a match
		tm_its_a_match = false;
		// confetti.stop();

		// Fetching
		tm_match_fetching_new_cards = true;
		
		// Perform ajax query to list api	
		xhr = jQuery.ajax({
			url: tm_match_base_url + "/ajax/cards",
			type: "GET",
			success: function(response) {
				tm_match_fetching_new_cards = false;
				jQuery(".tm_match__card-cont").empty().append(response);
				jQuery('.tm_match__card__btm').textfill({maxFontPixels: 32});
				jQuery(".tm_match__card__more_link").fadeIn();
			},
			error: function(xhr) {
				// This is also reached when we cancel the xhr request
			}
		});
	};

	// text fill cards on initial load
	jQuery('.tm_match__card__btm').textfill({maxFontPixels: 32});

});
})(jQuery, Drupal, this, this.document);

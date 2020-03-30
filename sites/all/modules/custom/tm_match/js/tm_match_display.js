//(function ($, Drupal, window, document, undefined) {

	jQuery(document).ready(function() {

	// some globals to keep track of stuff
	var tm_match_base_url = "/match";
	var tm_match_fetching_new_cards = false;
	var tm_current_uid = 10452; // Drupal.settings.tm_users.current_user_uid;

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
	};

	function release() {

		if (pullDeltaX >= decisionVal) {
			$card.addClass("to-right");
			console.log("to right");
			var card_uid = $card.data("card-uid");
			swipeCallback("right", tm_current_uid, card_uid);
			$card.addClass("demo_disable_move");
			confetti.start();
			setTimeout(function() { confetti.stop() }, 5000);

		} else if (pullDeltaX <= -decisionVal) {
			$card.addClass("to-left");
			console.log("to left");
			var card_uid = $card.data("card-uid");
			swipeCallback("left", tm_current_uid, card_uid);
			$card.addClass("demo_disable_move");
		}

		if (Math.abs(pullDeltaX) >= decisionVal) {
			$card.addClass("inactive");

			setTimeout(function() {
				$card.addClass("below").removeClass("inactive to-left to-right");
				cardsCounter++;
				var card_load_more = $card.data("card-load-more");
				var is_last_card = $card.data("is-last-card");
				console.log(is_last_card);
				if (is_last_card == true) {
					console.log("fetch new cards");
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
		}, 300);

	};

	jQuery(document).on("mousedown touchstart", ".tm_match__card:not(.inactive)", function(e) {
		
		if (animating) return;

		$card = jQuery(this);
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
				console.log("logged " + left_or_right);
			},
			error: function(xhr) {
				// This is also reached when we cancel the xhr request
			}
		});
	}

	// Fetch new cards
	function fetchNewCards() {

		console.log("ajax...");

		// Check not already fetching
		if (tm_match_fetching_new_cards) {
			return;
		}

		// Fetching
		tm_match_fetching_new_cards = true;
		
		// Perform ajax query to list api	
		xhr = jQuery.ajax({
			url: tm_match_base_url + "/ajax/cards",
			type: "GET",
			success: function(response) {
				tm_match_fetching_new_cards = false;
				jQuery(".tm_match__card-cont").empty().append(response);
			},
			error: function(xhr) {
				// This is also reached when we cancel the xhr request
			}
		});
	};

});

//})(jQuery, Drupal, this, this.document);

<div class="match-index-container">
    <div class="match-index-intro">__INTRO__</div>
</div>

<div class="match-debug-container" style="__DEBUG_CSS__">
	<input type="text" id="match-user-uid" value="__UID__"/>
  <input type="text" id="match-load-more" value="__LOAD_MORE__"/>
	<span class="match-debug-message">Debug message</span>
</div>

<div class="tm_match_container">
  <!--<header class="tm_match__header"></header>-->
  <div class="tm_match__content">
    <div class="tm_match__card-cont">
      __RESULTS__
    </div>
    <p class="tm_match__tip">__MATCH_TIP__</p>
    <div class="tm_match_footer">
      __MATCH_FOOTER__
    </div>
  </div>
 
</div>

<div class="tm_its_a_match_container">
  <div class="tm_its_a_match_content">
    <h1>It's a match! 🎉</h1>
    <p>Congratulations &mdash; you've matched with <span class="tm_its_a_match_first_name"></span>.</p>
    <p>Visit <a class="tm_its_a_match_url" target="_blank" style="text-decoration: underline;" href="">their profile</a> to send a message.</p>
    <p><br><a href="javascript:continueMatching();">Continue matching</a></p>
  </div>
</div>

<style>
  .tm_its_a_match_container {
    display: none;
    position: absolute;
    width: 100%;
    height: 100%;
    top: 0;
    left: 0;
    z-index: 9999;
    background-color: #ffffff;
  }

  .tm_its_a_match_content {
    padding-top: 4rem;
    text-align: center;
  }

  .tm_its_a_match_content h1 {
    font-size: 38pt;
  }

  .tm_its_a_match_content p {
    font-size: 16pt;
  }

</style>


/* Commissions reports methods */
(function ($, Drupal, window, document, undefined) {jQuery(document).ready(function(){
  
  // Chosen
  $(".insightsSelect").chosen({placeholder_text_multiple: "Filter by region, country, or chapter"});

  // Logic
  $(".insightsSelect").change(function(){
    doinsights();
  });

  // Call Insights
  function doinsights() {

    $.ajax({
      url: "/insights/callbacks/reach",
      data: {
        chapters: $('.insightsSelect').val()
      },
      type: "GET",
      dataType: "html",
      success: function (data) {
        $('#insightsResults').html(data);
      },
      error: function (xhr, status) {
        $('#insightsResults').html("<center>Sorry, there was a problem fetching the results. Try again later.</center>");
      },
      complete: function (xhr, status) {
        //$('#showresults').slideDown('slow')
      }
    });

    // Loading image
    $("#insightsResults").html('<center><img class="tm-status-update-pager-loading-image" src="/sites/all/themes/tm/images/load-more-ajax-loader.gif"></center>');
  }

  // First load
  doinsights();

});})(jQuery, Drupal, this, this.document);

// load google charts
google.load('visualization', '1.1', {packages:['corechart', 'table', 'bar']});
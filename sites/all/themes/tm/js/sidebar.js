// keep track of sidenav state
var tm_sidenav_hidden = 1;

// toggle the side nav
function toggleSideBar() {
    if (tm_sidenav_hidden) {

        tm_sidenav_hidden = 0;
        document.documentElement.scrollTop = 0;

        var element = document.getElementById("tm_sidenav");
        element.className+=" min";
        element.style.opacity = "1";
        element.style.display = "block";

        var menu = document.getElementById("tm_sidenav_menu");
        element.style.height = menu.offsetHeight + 280 + "px";

        var element = document.getElementById("tm_sidenav_toggle");
        element.className+=" tm_sidenav_toggle_fixed";
       
    } else {

        tm_sidenav_hidden = 1;

        var element = document.getElementById("tm_sidenav");
        element.className = element.className.replace("min", "");
        element.style.opacity = "0";
        element.style.display = "none";

        var element = document.getElementById("tm_sidenav_toggle");
        element.className = element.className.replace("tm_sidenav_toggle_fixed", "");
    }
}
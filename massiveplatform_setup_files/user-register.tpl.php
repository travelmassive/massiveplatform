<!-- example user-register.tpl.php -->
<!-- installation: -->
<!-- 1. copy to sites/all/themes/tm/templates -->
<!-- 2. set on $conf["tm_users_custom_signup_page"] = true -->
<!-- 3. drush cc all -->
<!-- 4. /user/register -->

<style>

/* hide some form elements */
h1.page__title.title {
	display: none;
}

.bttn.bttn-tertiary.bttn-m.cancel { 
	display: none;
}

.confirm-parent {
	display: none;
}

.description {
	display: none;
}

.form-item-pass {
	margin-bottom: 2rem;
}

/* custom layout */
.custom-login-container {
	display: flex;
	width: 100%;
}

.custom-login-lhs { 
	display: inline-block;
	margin-right: 1rem;
	width: 350px;
	text-align: center;
	border-right: 1px solid #eee;
}

.custom-login-rhs { 
	display: inline-block;
	margin-left: 1rem;
	border-radius: 1rem;
}
</style>

<script>

jQuery(document).ready(function () { 

	// if an error occurs, the form is renamed to user-register-form--2 and loses its css
	jQuery("#user-register-form--2").attr("id", "user-register-form");

	// Auto-type the verify password, so it can be hidden
	function autoConfirmPassword() {
        var box1 = jQuery('.password-field');
        var box2 = jQuery('.password-confirm');
        box2.val(box1.val());
    }
    jQuery('.password-field').on('change', autoConfirmPassword);
    jQuery('.password-field').on('keyup', autoConfirmPassword);
});

</script>

<div class='custom-login-container'>

<div class='custom-login-lhs'>
	Sign Up Faster With<br>
	<a href="/user/simple-fb-connect" class="facebook-login" style="margin-left: -16px; width: 220px; text-decoration: none; display: inline-block; margin-top: 0.25rem; margin-bottom: 0.25rem;">Facebook</a><br>
	<span style='font-size: smaller'>We will never post without your permission.</span>
</div>

<div class='custom-login-rhs'>
<?php

// custom user--register.tpl.php

// first and last name
print render($form['field_user_first_name']);
print render($form['field_user_last_name']);

// get a list of chapters
$chapters = node_load_multiple(array(), array('type' => "chapter"));
$chapter_list = array();
foreach ($chapters as $chapter) { // See this line in your original code
	$chapter_list[$chapter->nid] = $chapter->title; 
}
asort($chapter_list);
$chapter_list = array("" => "Select a chapter", "-1" => "Choose later") + $chapter_list;

// add chapter list to registraton form
$form['chapter_list']=array(
	'#attributes'=>array('name'=>'chapter_list'),
	'#type'=>'select',
	'#title' => t('Home Chapter'),
	'#options' => $chapter_list,
	'#multiple' => false,
	'#description'=>'Please select a home chapter to join.',
	'#weight'=>9,
);
print render($form['chapter_list']);

// email
print render($form['account']['mail']);

// password
print render($form['account']['pass']);

// form stuff
print render($form['form_build_id']);
print render($form['form_id']);
print render($form['actions']);

?>

</div>
</div>

<div style="clear:both;"></div>


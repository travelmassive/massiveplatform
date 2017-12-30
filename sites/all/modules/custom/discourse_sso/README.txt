# Drupal - Discourse SSO (Single Sign On)
# Modified by Ian C for Massive Platform

This module provides a Single Sign On integration for a Discourse forum.

It means that users that are logged in to your Drupal site are automatically
logged in to your Discourse site/forum. If the user is not logged in to your
Drupal site (s)he will be redirected to the Drupal login page.

The SSO login settings do not allow the normal login with Discourse (user name
and password) to be used with SSO login. It's a limitation caused by Discourse.

## Installation

* Download and enable this module.
* Set the Discourse site/forum url and SSO secret (will be used for safe
  communication, the same must be set in Discourse site/forum administration)
  at /admin/config/services/discourse-sso .
* Log into the Discourse site/forum as an admin and enable SSO, set the SSO url
  (sso_url) and SSO secret (sso_secret) according to Official Single-Sign-On
  for Discourse guide
  https://meta.discourse.org/t/official-single-sign-on-for-discourse/13045 ,
  all necessary instructions and illustration images are present there.
  sso_url: the offsite URL users will be sent to when attempting to log on.
  sso_secret: a secret string used to hash SSO payloads. Ensures payloads are
  authentic.
* Move the "Discourse forum link" block to some region or create your own link
  to Discourse site/forum. If the link is in format:
  "[discourse url]/session/sso" then it will automatically auto-login the user
  or redirect them to the Drupal login page. [The link is of course just
  OPTIONAL.]
* Your Single Sign On authentication integration between Discourse and Drupal
  should be configured and functional now.

## Discourse version

Tested with:
* Discourse Version: 1.9.0

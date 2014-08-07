<?php

/**
 * @file
 * Documentation for Queue Mail API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter a queued email when it is dequeued and about to be sent.
 *
 * This hook is largely equivalent to hook_mail_alter(), which will have already
 * been called for this email, but as sending can be delayed for some time, if
 * you need to do specific things just before the email is actually sent, you
 * can use this hook.
 *
 * @param $message
 *   An array containing the message data. Keys in this array include:
 *  - 'id':
 *     The drupal_mail() id of the message. Look at module source code or
 *     drupal_mail() for possible id values.
 *  - 'to':
 *     The address or addresses the message will be sent to. The
 *     formatting of this string must comply with RFC 2822.
 *  - 'from':
 *     The address the message will be marked as being from, which is
 *     either a custom address or the site-wide default email address.
 *  - 'subject':
 *     Subject of the email to be sent. This must not contain any newline
 *     characters, or the email may not be sent properly.
 *  - 'body':
 *     An array of strings containing the message text. The message body is
 *     created by concatenating the individual array strings into a single text
 *     string using "\n\n" as a separator.
 *  - 'headers':
 *     Associative array containing mail headers, such as From, Sender,
 *     MIME-Version, Content-Type, etc.
 *  - 'params':
 *     An array of optional parameters supplied by the caller of drupal_mail()
 *     that is used to build the message before hook_mail_alter() is invoked.
 *  - 'language':
 *     The language object used to build the message before hook_mail_alter()
 *     is invoked.
 *  - 'send':
 *     Set to FALSE to abort sending this email message.
 *
 * @see hook_mail_alter()
 */
function hook_queue_mail_send_alter(&$message) {
  if ($message['id'] == 'modulename_messagekey') {
    if (!example_notifications_optin($message['to'], $message['id'])) {
      // If the recipient has opted to not receive such messages, cancel
      // sending.
      $message['send'] = FALSE;
      return;
    }
    $message['body'][] = "--\nMail sent out from " . variable_get('site_name', t('Drupal'));
  }
}

/**
 * @} End of "addtogroup hooks".
 */

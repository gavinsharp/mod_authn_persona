/* Copyright 1999-2004 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Based in part, on mod_auth_memcookie, made by Mathieu CARBONNEAUX.
 *
 * See http://authmemcookie.sourceforge.net/ for details;
 * licensed under Apache License, Version 2.0.
 *
 * SHA-1 implementation by Steve Reid, steve@edmweb.com, in
 * public domain.
 */

#ifndef __VERIFY_H__
#define __VERIFY_H__

#include "defines.h"
#include "cookie.h"

#include <stdio.h>
#include <string.h>
#define APR_WANT_STRFUNC
#include "apr_want.h"
#include "apr_strings.h"
#include "apr_uuid.h"
#include "apr_tables.h"

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"   /* for ap_hook_(check_user_id | auth_checker)*/
#include "apr_base64.h"

typedef struct _VerifyResult {
  const char * verifiedEmail; // email that was verified
  const char * identityIssuer; // domain that issued the identity
  const char * errorResponse;
} * VerifyResult;

/**
 * process an assertion:
 *   verify an assertion, either locally or using mozilla's verification
 *   service.  Upon success, extract an email address, upon failure,
 *   generate a json formatted error message that can be returned to
 *   front end javascript.
 *
 * RETURN VALUE:
 *   VerifyResult structure.
 *    - Upon success has non-NULL verifiedEmail and identityIssuer fields.
 *    - Upon failure, has non-NULL errorResponse.
 */
VerifyResult processAssertion(request_rec *r, const char *assertion);

#endif

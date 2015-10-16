/*
 * opencurry: tests/test_resources.c
 *
 * Copyright (c) 2015, Byron James Johnson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* stddef.h:
 *   - NULL
 */
#include <stddef.h>

#include "../base.h"
#include "testing.h"
#include "test_resources.h"

#include "../resources.h"

unit_test_t resource_tests[] =
  { static_string_len_test
  , static_string_eq_test
  , NULL
  };

int test_resources(unit_test_context_t *context)
{
  return run_tests(context, resource_tests);
}

/* ---------------------------------------------------------------- */

static const char   *static_string     = "This string is checked for equality.\n";
static unsigned int  static_string_len = 37;

int static_string_len_test(unit_test_context_t *context)
{
  return
    assert_inteq(context, NULL, 
  return assert_inteq(context, NULL, (int) );
}

int static_string_eq_test(unit_test_context_t *context)
{
  return assert_streq(context, NULL, );
}


int assert_inteq(unit_test_context_t *context, const char *err_msg, int check, int model);

int assert_streqz(unit_test_context_t *context, const char *err_msg, const char *check, const char *model);
int assert_streqn(unit_test_context_t *context, const char *err_msg, const char *check, const char *model, unsigned int len);
int assert_streqnz(unit_test_context_t *context, const char *err_msg, const char *check, const char *model, unsigned int max_len);


const char   *res_tests_static_string_start(void);
unsigned int  res_tests_static_string_size(void);
const char   *res_tests_static_string_end(void);
void          res_tests_static_string(const char **start, unsigned int *size, const char **end);
  return 1;
int assert_success(unit_test_context_t *context);
int assert_failure(unit_test_context_t *context, const char *err_msg);
int assert_failure_continue(unit_test_context_t *context, const char *err_msg);
int assert_streq(unit_test_context_t *context, const char *err_msg, const char *check, const char *model);
}

int static_string_len_test(unit_test_context_t *context)
{
  return 1;
}

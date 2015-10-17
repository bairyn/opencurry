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

/* string.h:
 *   - strlen
 */
#include <string.h>

#include "../base.h"
#include "testing.h"
#include "test_resources.h"

#include "resources.h"

#include "../resources.h"

int test_resources_cli(int argc, char **argv)
{
  return run_test_suite(resources_test);
}

/* ---------------------------------------------------------------- */

/* Resource tests. */
unit_test_t resources_test =
  {  test_resources_run
  , "test_resources"
  , "Resource tests."
  };

/* Array of resources tests. */
unit_test_t *resources_tests[] =
  { &static_string_len_test
  , &static_string_eq_test
  , NULL
  };

unit_test_result_t test_resources_run(unit_test_context_t *context)
{
  return run_tests(context, resources_tests);
}

/* ---------------------------------------------------------------- */

static const char   *static_string     = "This string is checked for equality.\n";
static unsigned int  static_string_len = 37;

/* ---------------------------------------------------------------- */

unit_test_t static_string_len_test =
  {  static_string_len_test_run
  , "static_string_len_test"
  , "Ensure the resource data has the correct size."
  };

unit_test_result_t static_string_len_test_run(unit_test_context_t *context)
{
  return
    assert_inteq(context, NULL, (int) strlen(static_string),          (int) static_string_len)
  | assert_inteq(context, NULL, (int) res_tests_static_string_size(), (int) static_string_len)
  ;
}

/* ---------------------------------------------------------------- */

unit_test_t static_string_eq_test =
  {  static_string_eq_test_run
  , "static_string_eq_test"
  , "Ensure resource data is equal to the correct string."
  };

unit_test_result_t static_string_eq_test_run(unit_test_context_t *context)
{
  return
    assert_streqn(context, NULL, (const char *) res_tests_static_string_start(), (const char *) static_string, (size_t) res_tests_static_string_size());
}

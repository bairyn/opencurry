/*
 * opencurry: tests/test_util.c
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

#include "../base.h"
#include "testing.h"
#include "test_util.h"

#include "../util.h"

int test_util_cli(int argc, char **argv)
{
  return run_test_suite(util_test);
}

/* ---------------------------------------------------------------- */

/* util tests. */
unit_test_t util_test =
  {  test_util_run
  , "test_util"
  , "util tests."
  };

/* Array of util tests. */
unit_test_t *util_tests[] =
  { &util_equalities_test

  , NULL
  };

unit_test_result_t test_util_run(unit_test_context_t *context)
{
  return run_tests(context, util_tests);
}

/* ---------------------------------------------------------------- */

unit_test_t util_equalities_test =
  {  util_equalities_test_run
  , "util_equalities_test"
  , "Aptly assorted equality tests for \"util\"."
  };

unit_test_result_t util_equalities_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    void *context;

    ASSERT2( inteq, min_int( 3,  7),  3 );
    ASSERT2( inteq, max_int( 3,  7),  7 );
    ASSERT2( inteq, min_int(-3, -7), -3 );
    ASSERT2( inteq, max_int(-3, -7), -7 );

    ASSERT2( sizeeq, min_size(3,  7),  3 );
    ASSERT2( sizeeq, max_size(3,  7),  7 );

    ASSERT2( sizeeq, size_less_null(0), 0 );
    ASSERT2( sizeeq, size_less_null(1), 0 );
    ASSERT2( sizeeq, size_less_null(2), 1 );

    ASSERT2( inteq, proc_true(),  1 );
    ASSERT2( inteq, proc_false(), 0 );

    ASSERT2( inteq, proc_cond(1)(), 1 );
    ASSERT2( inteq, proc_cond(0)(), 1 );

    ASSERT2( objpeq, proc_context(&context), &context );

    ASSERT2( inteq, proc_true_context(&context),  1 );
    ASSERT2( inteq, proc_false_context(&context), 0 );

    ASSERT2( inteq, is_big_endian(), !are_bytes_reversed() );
  }

  return result;
}

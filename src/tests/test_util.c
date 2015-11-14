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
  { &util_equalities_num_test
  , &util_equalities_nullterm_test
  , &util_equalities_str_test

  , NULL
  };

unit_test_result_t test_util_run(unit_test_context_t *context)
{
  return run_tests(context, util_tests);
}

/* ---------------------------------------------------------------- */

unit_test_t util_equalities_num_test =
  {  util_equalities_num_test_run
  , "util_equalities_num_test"
  , "Aptly assorted equality tests for numeric \"util\" utilities."
  };

unit_test_result_t util_equalities_num_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    void *object;

    ASSERT2( inteq, min_int( 3,  7),  3 );
    ASSERT2( inteq, max_int( 3,  7),  7 );
    ASSERT2( inteq, min_int(-3, -7), -7 );
    ASSERT2( inteq, max_int(-3, -7), -3 );

    ASSERT2( sizeeq, min_size(3,  7),  3 );
    ASSERT2( sizeeq, max_size(3,  7),  7 );

    ASSERT2( sizeeq, size_less_null(0), 0 );
    ASSERT2( sizeeq, size_less_null(1), 0 );
    ASSERT2( sizeeq, size_less_null(2), 1 );

    ASSERT2( inteq, proc_true(),  1 );
    ASSERT2( inteq, proc_false(), 0 );

    ASSERT2( inteq, proc_cond(0)(), 0 );
    ASSERT2( inteq, proc_cond(1)(), 1 );

    ASSERT2( objpeq, proc_context(&object), &object );

    ASSERT2( inteq, proc_true_context(&object),  1 );
    ASSERT2( inteq, proc_false_context(&object), 0 );

    ASSERT2( inteq, is_big_endian(), !are_bytes_reversed() );
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t util_equalities_nullterm_test =
  {  util_equalities_nullterm_test_run
  , "util_equalities_nullterm_test"
  , "set_null_terminator equality tests."
  };

unit_test_result_t util_equalities_nullterm_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    char buf_empty[4] = "";
    char buf_full [4] = "123";
    char buf_max  [4] = { 'l', 'o', 'v', 'e' };

    ASSERT3( streqn,     buf_empty, "",            sizeof(buf_empty) );
    ASSERT3( streqn,     buf_full,  "123",         sizeof(buf_full)  );
    ASSERT3( streqn,     buf_max,   "love",        sizeof(buf_max)   );
    ASSERT3( streqn,     buf_max,   "lovely love", sizeof(buf_max)   );
    ASSERT3( not_streqn, buf_max,   "123",         sizeof(buf_max)   );

    ASSERT3( streqn, buf_empty, "",     sizeof(buf_empty) );
    ASSERT3( streqn, buf_full,  "123",  sizeof(buf_full)  );
    ASSERT3( streqn, buf_max,   "love", sizeof(buf_max)   );

    ASSERT2( inteq, set_null_terminator(buf_empty, 0, sizeof(buf_empty)), 0);
    ASSERT2( inteq, set_null_terminator(buf_full,  3, sizeof(buf_full)),  0);
    ASSERT2( inteq, set_null_terminator(buf_max,   4, sizeof(buf_max)),   1);

    ASSERT3( streqn, buf_empty, "",    sizeof(buf_empty) );
    ASSERT3( streqn, buf_full,  "123", sizeof(buf_full)  );
    ASSERT3( streqn, buf_max,   "lov", sizeof(buf_max)   );

    ASSERT2( inteq, set_null_terminator(buf_empty, 0, sizeof(buf_empty)), 0);
    ASSERT2( inteq, set_null_terminator(buf_full,  3, sizeof(buf_full)),  0);
    ASSERT2( inteq, set_null_terminator(buf_max,   4, sizeof(buf_max)),   1);

    ASSERT3( streqn, buf_empty, "",    sizeof(buf_empty) );
    ASSERT3( streqn, buf_full,  "123", sizeof(buf_full)  );
    ASSERT3( streqn, buf_max,   "lov", sizeof(buf_max)   );

    ASSERT2( inteq, set_null_terminator(buf_empty, 2, 3), 0);
    ASSERT2( inteq, set_null_terminator(buf_full,  2, 3), 0);
    ASSERT2( inteq, set_null_terminator(buf_max,   2, 3), 0);

    ASSERT3( streqn, buf_empty, "",   sizeof(buf_empty) );
    ASSERT3( streqn, buf_full,  "12", sizeof(buf_full)  );
    ASSERT3( streqn, buf_max,   "lo", sizeof(buf_max)   );

    ASSERT2( inteq, set_null_terminator(buf_empty, 2, 2), 1);
    ASSERT2( inteq, set_null_terminator(buf_full,  2, 2), 1);
    ASSERT2( inteq, set_null_terminator(buf_max,   2, 2), 1);

    ASSERT3( streqn, buf_empty, "",  sizeof(buf_empty) );
    ASSERT3( streqn, buf_full,  "1", sizeof(buf_full)  );
    ASSERT3( streqn, buf_max,   "l", sizeof(buf_max)   );

    ASSERT2( inteq, set_null_terminator(buf_empty, 0, 9), 0);
    ASSERT2( inteq, set_null_terminator(buf_full,  0, 9), 0);
    ASSERT2( inteq, set_null_terminator(buf_max,   0, 9), 0);

    ASSERT3( streqn, buf_empty, "", sizeof(buf_empty) );
    ASSERT3( streqn, buf_full,  "", sizeof(buf_full)  );
    ASSERT3( streqn, buf_max,   "", sizeof(buf_max)   );

    ASSERT2( inteq, set_null_terminator(buf_empty, 0, 0), -1);
    ASSERT2( inteq, set_null_terminator(buf_full,  0, 0), -1);
    ASSERT2( inteq, set_null_terminator(buf_max,   0, 0), -1);

    ASSERT3( streqn, buf_empty, "", sizeof(buf_empty) );
    ASSERT3( streqn, buf_full,  "", sizeof(buf_full)  );
    ASSERT3( streqn, buf_max,   "", sizeof(buf_max)   );

    ASSERT2( inteq, set_null_terminator(NULL, 0, 0), -2);
    ASSERT2( inteq, set_null_terminator(NULL, 0, 0), -2);
    ASSERT2( inteq, set_null_terminator(NULL, 0, 0), -2);

    ASSERT3( streqn, buf_empty, "", sizeof(buf_empty) );
    ASSERT3( streqn, buf_full,  "", sizeof(buf_full)  );
    ASSERT3( streqn, buf_max,   "", sizeof(buf_max)   );
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t util_equalities_str_test =
  {  util_equalities_str_test_run
  , "util_equalities_str_test"
  , "Aptly assorted equality tests for \"util\"'s string utilities."
  };

unit_test_result_t util_equalities_str_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    char dest[1024];
    char smalldest[8];
    const char love[] = "love";
    const char peace[] = "peace";

    /* ---------------------------------------------------------------- */

    MASSERT2( inteq,  "strlcpy0", strlcpy(dest, love, 5), 4);
    MASSERT3( streqn, "strlcpy1", dest, "love", sizeof(dest) );

    MASSERT2( inteq,  "strlcpy2", strlcpy(dest, love, 4), 3);
    MASSERT3( streqn, "strlcpy3", dest, "lov",  sizeof(dest) );

    MASSERT2( inteq,  "strlcpy4", strlcpy(dest, love, 1), 0);
    MASSERT3( streqn, "strlcpy5", dest, "",     sizeof(dest) );

    MASSERT2( inteq,  "strlcpy6", strlcpy(dest, peace, sizeof(dest)), 5);
    MASSERT2( inteq,  "strlcpy7", strlcpy(dest, love,  0),            0);
    MASSERT3( streqn, "strlcpy8", dest, "peace", sizeof(dest) );

    /* ---------------------------------------------------------------- */

    MASSERT2( inteq,  "strlcpy_srcmax00", strlcpy_srcmax(smalldest, peace, 8, 5), 5);
    MASSERT3( streqn, "strlcpy_srcmax01", smalldest, "peace", sizeof(smalldest) );

    MASSERT2( inteq,  "strlcpy_srcmax02", strlcpy_srcmax(smalldest, peace, 8, 4), 4);
    MASSERT3( streqn, "strlcpy_srcmax03", smalldest, "peac",  sizeof(smalldest) );


    MASSERT2( inteq,  "strlcpy_srcmax04", strlcpy_srcmax(smalldest, peace, 5, 5), 4);
    MASSERT3( streqn, "strlcpy_srcmax05", smalldest, "peac",  sizeof(smalldest) );

    MASSERT2( inteq,  "strlcpy_srcmax06", strlcpy_srcmax(smalldest, peace, 5, 4), 4);
    MASSERT3( streqn, "strlcpy_srcmax07", smalldest, "peac",  sizeof(smalldest) );

    MASSERT2( inteq,  "strlcpy_srcmax08", strlcpy_srcmax(smalldest, peace, 5, 3), 3);
    MASSERT3( streqn, "strlcpy_srcmax09", smalldest, "pea",   sizeof(smalldest) );

    MASSERT2( inteq,  "strlcpy_srcmax10", strlcpy_srcmax(smalldest, peace, 5, 2), 2);
    MASSERT3( streqn, "strlcpy_srcmax11", smalldest, "pe",    sizeof(smalldest) );


    MASSERT2( inteq,  "strlcpy_srcmax12", strlcpy_srcmax(smalldest, peace, 4, 5), 3);
    MASSERT3( streqn, "strlcpy_srcmax13", smalldest, "pea",   sizeof(smalldest) );

    MASSERT2( inteq,  "strlcpy_srcmax14", strlcpy_srcmax(smalldest, peace, 4, 4), 3);
    MASSERT3( streqn, "strlcpy_srcmax15", smalldest, "pea",   sizeof(smalldest) );

    MASSERT2( inteq,  "strlcpy_srcmax16", strlcpy_srcmax(smalldest, peace, 4, 3), 3);
    MASSERT3( streqn, "strlcpy_srcmax17", smalldest, "pea",   sizeof(smalldest) );

    MASSERT2( inteq,  "strlcpy_srcmax18", strlcpy_srcmax(smalldest, peace, 4, 2), 2);
    MASSERT3( streqn, "strlcpy_srcmax19", smalldest, "pe",    sizeof(smalldest) );


    MASSERT2( inteq,  "strlcpy_srcmax20", strlcpy_srcmax(smalldest, peace, 8, 5), 5);
    MASSERT2( inteq,  "strlcpy_srcmax21", strlcpy_srcmax(smalldest, peace, 8, 0), 0);
    MASSERT3( streqn, "strlcpy_srcmax22", smalldest, "",      sizeof(smalldest) );


    MASSERT2( inteq,  "strlcpy_srcmax23", strlcpy_srcmax(smalldest, peace, 8, 5), 5);
    MASSERT2( inteq,  "strlcpy_srcmax24", strlcpy_srcmax(smalldest, peace, 0, 5), 0);
    MASSERT3( streqn, "strlcpy_srcmax25", smalldest, "peace", sizeof(smalldest) );
  }

  return result;
}

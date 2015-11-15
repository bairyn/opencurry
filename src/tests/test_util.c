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
  , &util_equalities_basic_str_test
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

    /* ---------------------------------------------------------------- */

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

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, size_minus(8, 0),  8  );
    ASSERT2( sizeeq, size_minus(8, 1),  7  );
    ASSERT2( sizeeq, size_minus(8, 7),  1  );
    ASSERT2( sizeeq, size_minus(8, 8),  0  );
    ASSERT2( sizeeq, size_minus(8, 9),  0  );
    ASSERT2( sizeeq, size_minus(8, 10), 0  );
    ASSERT2( sizeeq, size_minus(8, 99), 0  );

    ASSERT2( sizeeq, size_minus(1, 0),  1  );
    ASSERT2( sizeeq, size_minus(1, 1),  0  );
    ASSERT2( sizeeq, size_minus(1, 2),  0  );
    ASSERT2( sizeeq, size_minus(1, 4),  0  );
    ASSERT2( sizeeq, size_minus(1, 8),  0  );
    ASSERT2( sizeeq, size_minus(1, 99), 0  );

    ASSERT2( sizeeq, size_minus(0, 0),  0  );
    ASSERT2( sizeeq, size_minus(0, 1),  0  );
    ASSERT2( sizeeq, size_minus(0, 2),  0  );
    ASSERT2( sizeeq, size_minus(0, 4),  0  );
    ASSERT2( sizeeq, size_minus(0, 8),  0  );
    ASSERT2( sizeeq, size_minus(0, 99), 0  );


    ASSERT2( sizeeq, size_minus(0,  8), 0  );
    ASSERT2( sizeeq, size_minus(1,  8), 0  );
    ASSERT2( sizeeq, size_minus(7,  8), 0  );
    ASSERT2( sizeeq, size_minus(8,  8), 0  );
    ASSERT2( sizeeq, size_minus(9,  8), 1  );
    ASSERT2( sizeeq, size_minus(10, 8), 2  );
    ASSERT2( sizeeq, size_minus(99, 8), 91 );

    ASSERT2( sizeeq, size_minus(0,  1), 0  );
    ASSERT2( sizeeq, size_minus(1,  1), 0  );
    ASSERT2( sizeeq, size_minus(2,  1), 1  );
    ASSERT2( sizeeq, size_minus(4,  1), 3  );
    ASSERT2( sizeeq, size_minus(8,  1), 7  );
    ASSERT2( sizeeq, size_minus(99, 1), 98 );

    ASSERT2( sizeeq, size_minus(0,  0), 0  );
    ASSERT2( sizeeq, size_minus(1,  0), 1  );
    ASSERT2( sizeeq, size_minus(2,  0), 2  );
    ASSERT2( sizeeq, size_minus(4,  0), 4  );
    ASSERT2( sizeeq, size_minus(8,  0), 8  );
    ASSERT2( sizeeq, size_minus(99, 0), 99 );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, size_distance(8, 0),  8  );
    ASSERT2( sizeeq, size_distance(8, 1),  7  );
    ASSERT2( sizeeq, size_distance(8, 7),  1  );
    ASSERT2( sizeeq, size_distance(8, 8),  0  );
    ASSERT2( sizeeq, size_distance(8, 9),  1  );
    ASSERT2( sizeeq, size_distance(8, 10), 2  );
    ASSERT2( sizeeq, size_distance(8, 99), 91 );

    ASSERT2( sizeeq, size_distance(1, 0),  1  );
    ASSERT2( sizeeq, size_distance(1, 1),  0  );
    ASSERT2( sizeeq, size_distance(1, 2),  1  );
    ASSERT2( sizeeq, size_distance(1, 4),  3  );
    ASSERT2( sizeeq, size_distance(1, 8),  7  );
    ASSERT2( sizeeq, size_distance(1, 99), 98 );

    ASSERT2( sizeeq, size_distance(0, 0),  0  );
    ASSERT2( sizeeq, size_distance(0, 1),  1  );
    ASSERT2( sizeeq, size_distance(0, 2),  2  );
    ASSERT2( sizeeq, size_distance(0, 4),  4  );
    ASSERT2( sizeeq, size_distance(0, 8),  8  );
    ASSERT2( sizeeq, size_distance(0, 99), 99 );


    ASSERT2( sizeeq, size_distance(0,  8), 8  );
    ASSERT2( sizeeq, size_distance(1,  8), 7  );
    ASSERT2( sizeeq, size_distance(7,  8), 1  );
    ASSERT2( sizeeq, size_distance(8,  8), 0  );
    ASSERT2( sizeeq, size_distance(9,  8), 1  );
    ASSERT2( sizeeq, size_distance(10, 8), 2  );
    ASSERT2( sizeeq, size_distance(99, 8), 91 );

    ASSERT2( sizeeq, size_distance(0,  1), 1  );
    ASSERT2( sizeeq, size_distance(1,  1), 0  );
    ASSERT2( sizeeq, size_distance(2,  1), 1  );
    ASSERT2( sizeeq, size_distance(4,  1), 3  );
    ASSERT2( sizeeq, size_distance(8,  1), 7  );
    ASSERT2( sizeeq, size_distance(99, 1), 98 );

    ASSERT2( sizeeq, size_distance(0,  0), 0  );
    ASSERT2( sizeeq, size_distance(1,  0), 1  );
    ASSERT2( sizeeq, size_distance(2,  0), 2  );
    ASSERT2( sizeeq, size_distance(4,  0), 4  );
    ASSERT2( sizeeq, size_distance(8,  0), 8  );
    ASSERT2( sizeeq, size_distance(99, 0), 99 );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, terminator_size(99), 98 );
    ASSERT2( sizeeq, terminator_size(8),  7  );
    ASSERT2( sizeeq, terminator_size(4),  3 );
    ASSERT2( sizeeq, terminator_size(3),  2 );
    ASSERT2( sizeeq, terminator_size(2),  1 );
    ASSERT2( sizeeq, terminator_size(1),  0 );
    ASSERT2( sizeeq, terminator_size(0),  0 );
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

unit_test_t util_equalities_basic_str_test =
  {  util_equalities_basic_str_test_run
  , "util_equalities_basic_str_test"
  , "Aptly assorted equality tests for \"util\"'s basic string utilities."
  };

unit_test_result_t util_equalities_basic_str_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    char dest[1024];
    char smalldest[8];
    const char love[] = "love";
    const char peace[] = "peace";

    /* ---------------------------------------------------------------- */

    MASSERT2( sizeeq, "0strlcpy", strlcpy(dest,  love, 5), 4 );
    MASSERT3( streqn, "1strlcpy", dest, "love",  sizeof(dest) );

    MASSERT2( sizeeq, "2strlcpy", strlcpy(dest,  love, 4), 3);
    MASSERT3( streqn, "3strlcpy", dest, "lov",   sizeof(dest) );

    MASSERT2( sizeeq, "4strlcpy", strlcpy(dest,  love, 1), 0);
    MASSERT3( streqn, "5strlcpy", dest, "",      sizeof(dest) );


    MASSERT2( sizeeq, "6strlcpy", strlcpy(dest,  peace, sizeof(dest)), 5 );
    MASSERT2( sizeeq, "7strlcpy", strlcpy(dest,  love,  0),            0 );
    MASSERT3( streqn, "8strlcpy", dest, "peace", sizeof(dest) );

    /* ---------------------------------------------------------------- */

    MASSERT2( sizeeq, "00strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    MASSERT3( streqn, "01strlcpy_srcmax", smalldest, "peace", sizeof(smalldest) );

    MASSERT2( sizeeq, "02strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 8, 4), 4 );
    MASSERT3( streqn, "03strlcpy_srcmax", smalldest, "peac",  sizeof(smalldest) );


    MASSERT2( sizeeq, "04strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 5, 5), 4 );
    MASSERT3( streqn, "05strlcpy_srcmax", smalldest, "peac",  sizeof(smalldest) );

    MASSERT2( sizeeq, "06strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 5, 4), 4 );
    MASSERT3( streqn, "07strlcpy_srcmax", smalldest, "peac",  sizeof(smalldest) );

    MASSERT2( sizeeq, "08strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 5, 3), 3 );
    MASSERT3( streqn, "09strlcpy_srcmax", smalldest, "pea",   sizeof(smalldest) );

    MASSERT2( sizeeq, "10strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 5, 2), 2 );
    MASSERT3( streqn, "11strlcpy_srcmax", smalldest, "pe",    sizeof(smalldest) );


    MASSERT2( sizeeq, "12strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 4, 5), 3 );
    MASSERT3( streqn, "13strlcpy_srcmax", smalldest, "pea",   sizeof(smalldest) );

    MASSERT2( sizeeq, "14strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 4, 4), 3 );
    MASSERT3( streqn, "15strlcpy_srcmax", smalldest, "pea",   sizeof(smalldest) );

    MASSERT2( sizeeq, "16strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 4, 3), 3 );
    MASSERT3( streqn, "17strlcpy_srcmax", smalldest, "pea",   sizeof(smalldest) );

    MASSERT2( sizeeq, "18strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 4, 2), 2 );
    MASSERT3( streqn, "19strlcpy_srcmax", smalldest, "pe",    sizeof(smalldest) );


    MASSERT2( sizeeq, "20strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    MASSERT2( sizeeq, "21strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 8, 0), 0 );
    MASSERT3( streqn, "22strlcpy_srcmax", smalldest, "",      sizeof(smalldest) );


    MASSERT2( sizeeq, "23strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    MASSERT2( sizeeq, "24strlcpy_srcmax", strlcpy_srcmax(smalldest, peace, 0, 5), 0 );
    MASSERT3( streqn, "25strlcpy_srcmax", smalldest, "peace", sizeof(smalldest) );

    /* ---------------------------------------------------------------- */

    MASSERT2( sizeeq, "00strlcpy_with_max", strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    MASSERT3( streqn, "01strlcpy_with_max", dest, "love", sizeof(dest) );


    MASSERT2( sizeeq, "02strlcpy_with_max", strlcpy_with_max(dest, love, 8, 4, 3), 3 );
    MASSERT3( streqn, "03strlcpy_with_max", dest, "lov",  sizeof(dest) );

    MASSERT2( sizeeq, "02strlcpy_with_max", strlcpy_with_max(dest, love, 8, 3, 4), 3 );
    MASSERT3( streqn, "03strlcpy_with_max", dest, "lov",  sizeof(dest) );

    MASSERT2( sizeeq, "04strlcpy_with_max", strlcpy_with_max(dest, love, 8, 3, 3), 3 );
    MASSERT3( streqn, "05strlcpy_with_max", dest, "lov",  sizeof(dest) );


    MASSERT2( sizeeq, "06strlcpy_with_max", strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    MASSERT2( sizeeq, "07strlcpy_with_max", strlcpy_with_max(dest, love, 8, 0, 4), 0 );
    MASSERT3( streqn, "08strlcpy_with_max", dest, "",     sizeof(dest) );

    MASSERT2( sizeeq, "09strlcpy_with_max", strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    MASSERT2( sizeeq, "10strlcpy_with_max", strlcpy_with_max(dest, love, 8, 4, 0), 0 );
    MASSERT3( streqn, "11strlcpy_with_max", dest, "",     sizeof(dest) );

    MASSERT2( sizeeq, "12strlcpy_with_max", strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    MASSERT2( sizeeq, "13strlcpy_with_max", strlcpy_with_max(dest, love, 8, 0, 0), 0 );
    MASSERT3( streqn, "14strlcpy_with_max", dest, "",     sizeof(dest) );

    MASSERT2( sizeeq, "15strlcpy_with_max", strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    MASSERT2( sizeeq, "16strlcpy_with_max", strlcpy_with_max(dest, love, 1, 4, 4), 0 );
    MASSERT3( streqn, "17strlcpy_with_max", dest, "",     sizeof(dest) );

    MASSERT2( sizeeq, "18strlcpy_with_max", strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    MASSERT2( sizeeq, "19strlcpy_with_max", strlcpy_with_max(dest, love, 0, 4, 4), 0 );
    MASSERT3( streqn, "20strlcpy_with_max", dest, "love", sizeof(dest) );

    /* ---------------------------------------------------------------- */

    MASSERT2( sizeeq, "str?len strlcpy", strlcpy(dest, love, sizeof(dest)), 4 );

    MASSERT2( sizeeq, "0strnlen", strnlen(dest, 7), 4 );
    MASSERT2( sizeeq, "1strnlen", strnlen(dest, 6), 4 );
    MASSERT2( sizeeq, "2strnlen", strnlen(dest, 5), 4 );
    MASSERT2( sizeeq, "3strnlen", strnlen(dest, 4), 4 );
    MASSERT2( sizeeq, "4strnlen", strnlen(dest, 3), 3 );
    MASSERT2( sizeeq, "5strnlen", strnlen(dest, 2), 2 );
    MASSERT2( sizeeq, "6strnlen", strnlen(dest, 1), 1 );
    MASSERT2( sizeeq, "7strnlen", strnlen(dest, 0), 0 );

    MASSERT2( sizeeq, "0strllen", strllen(dest, 7), 4 );
    MASSERT2( sizeeq, "1strllen", strllen(dest, 6), 4 );
    MASSERT2( sizeeq, "2strllen", strllen(dest, 5), 4 );
    MASSERT2( sizeeq, "3strllen", strllen(dest, 4), 3 );
    MASSERT2( sizeeq, "4strllen", strllen(dest, 3), 2 );
    MASSERT2( sizeeq, "5strllen", strllen(dest, 2), 1 );
    MASSERT2( sizeeq, "6strllen", strllen(dest, 1), 0 );
    MASSERT2( sizeeq, "7strllen", strllen(dest, 0), 0 );

    /* ---------------------------------------------------------------- */

    MASSERT2( sizeeq, "has_null strlcpy", strlcpy(dest, love, sizeof(dest)), 4 );

    MASSERT1( true,  "0has_null", strn_has_null_terminator(dest, sizeof(dest)) );

    MASSERT1( true,  "1has_null", strn_has_null_terminator(dest, 5) );
    MASSERT1( false, "2has_null", strn_has_null_terminator(dest, 4) );
    MASSERT1( false, "2has_null", strn_has_null_terminator(dest, 3) );

    /* ---------------------------------------------------------------- */

    MASSERT2( sizeeq, "00strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT3( nstreq, "01strlcp_cycle eq", sizeof(dest), dest, "love" );

    MASSERT2( sizeeq, "02strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 1),  1 );
    MASSERT3( nstreq, "03strlcp_cycle eq", sizeof(dest), dest, "l" );

    MASSERT2( sizeeq, "04strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 0),  0 );
    MASSERT3( nstreq, "05strlcp_cycle eq", sizeof(dest), dest, "" );

    MASSERT2( sizeeq, "06strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 5),  5 );
    MASSERT3( nstreq, "07strlcp_cycle eq", sizeof(dest), dest, "lovel" );

    MASSERT2( sizeeq, "08strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 8),  8 );
    MASSERT3( nstreq, "09strlcp_cycle eq", sizeof(dest), dest, "lovelove" );

    MASSERT2( sizeeq, "10strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 9),  9 );
    MASSERT3( nstreq, "11strlcp_cycle eq", sizeof(dest), dest, "lovelovel" );

    MASSERT2( sizeeq, "12strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 12), 12 );
    MASSERT3( nstreq, "13strlcp_cycle eq", sizeof(dest), dest, "lovelovelove" );


    MASSERT2( sizeeq, "14strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "15strlcp_cycle",    strlcpy_cycle(dest, love, 5,            4),  4 );
    MASSERT3( nstreq, "16strlcp_cycle eq", sizeof(dest), dest, "love" );

    MASSERT2( sizeeq, "17strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "18strlcp_cycle",    strlcpy_cycle(dest, love, 2,            4),  1 );
    MASSERT3( nstreq, "19strlcp_cycle eq", sizeof(dest), dest, "l" );

    MASSERT2( sizeeq, "20strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "21strlcp_cycle",    strlcpy_cycle(dest, love, 1,            4),  0 );
    MASSERT3( nstreq, "22strlcp_cycle eq", sizeof(dest), dest, "" );

    MASSERT2( sizeeq, "23strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "24strlcp_cycle",    strlcpy_cycle(dest, love, 0,            4),  0 );
    MASSERT3( nstreq, "25strlcp_cycle eq", sizeof(dest), dest, "love" );


    MASSERT2( sizeeq, "26strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "27strlcp_cycle",    strlcpy_cycle(dest, "",   5,            4),  0 );
    MASSERT3( nstreq, "28strlcp_cycle eq", sizeof(dest), dest, "" );

    MASSERT2( sizeeq, "29strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "30strlcp_cycle",    strlcpy_cycle(dest, "",   2,            4),  0 );
    MASSERT3( nstreq, "31strlcp_cycle eq", sizeof(dest), dest, "" );

    MASSERT2( sizeeq, "32strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "33strlcp_cycle",    strlcpy_cycle(dest, "",   1,            4),  0 );
    MASSERT3( nstreq, "34strlcp_cycle eq", sizeof(dest), dest, "" );

    MASSERT2( sizeeq, "35strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "36strlcp_cycle",    strlcpy_cycle(dest, "",   0,            4),  0 );
    MASSERT3( nstreq, "37strlcp_cycle eq", sizeof(dest), dest, "love" );


    MASSERT2( sizeeq, "38strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "39strlcp_cycle",    strlcpy_cycle(dest, love, 3,            8),  2 );
    MASSERT3( nstreq, "40strlcp_cycle eq", sizeof(dest), dest, "lo" );

    MASSERT2( sizeeq, "41strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "42strlcp_cycle",    strlcpy_cycle(dest, love, 5,            8),  4 );
    MASSERT3( nstreq, "43strlcp_cycle eq", sizeof(dest), dest, "love" );

    MASSERT2( sizeeq, "44strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "45strlcp_cycle",    strlcpy_cycle(dest, love, 0,            8),  0 );
    MASSERT3( nstreq, "46strlcp_cycle eq", sizeof(dest), dest, "love" );


    MASSERT2( sizeeq, "47strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "38strlcp_cycle",    strlcpy_cycle(dest, love, 3,            10), 2 );
    MASSERT3( nstreq, "49strlcp_cycle eq", sizeof(dest), dest, "lo" );

    MASSERT2( sizeeq, "50strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "51strlcp_cycle",    strlcpy_cycle(dest, love, 5,            10), 4 );
    MASSERT3( nstreq, "53strlcp_cycle eq", sizeof(dest), dest, "love" );

    MASSERT2( sizeeq, "54strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "55strlcp_cycle",    strlcpy_cycle(dest, love, 9,            10), 8 );
    MASSERT3( nstreq, "56strlcp_cycle eq", sizeof(dest), dest, "lovelove" );

    MASSERT2( sizeeq, "57strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "58strlcp_cycle",    strlcpy_cycle(dest, love, 0,            10), 0 );
    MASSERT3( nstreq, "59strlcp_cycle eq", sizeof(dest), dest, "love" );


    MASSERT2( sizeeq, "60strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "61strlcp_cycle",    strlcpy_cycle(dest, love, 1,            1),  0 );
    MASSERT3( nstreq, "62strlcp_cycle eq", sizeof(dest), dest, "" );

    MASSERT2( sizeeq, "63strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "64strlcp_cycle",    strlcpy_cycle(dest, love, 1,            0),  0 );
    MASSERT3( nstreq, "65strlcp_cycle eq", sizeof(dest), dest, "" );

    MASSERT2( sizeeq, "66strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "67strlcp_cycle",    strlcpy_cycle(dest, love, 0,            1),  0 );
    MASSERT3( nstreq, "68strlcp_cycle eq", sizeof(dest), dest, "love" );

    MASSERT2( sizeeq, "69strlcp_cycle",    strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    MASSERT2( sizeeq, "70strlcp_cycle",    strlcpy_cycle(dest, love, 0,            0),  0 );
    MASSERT3( nstreq, "71strlcp_cycle eq", sizeof(dest), dest, "love" );

    /* ---------------------------------------------------------------- */

    MASSERT2( sizeeq, "0 0strl_constrain_index", strl_constrain_index(32, 0, 8),  8  );
    MASSERT2( sizeeq, "0 1strl_constrain_index", strl_constrain_index(32, 0, 29), 29 );
    MASSERT2( sizeeq, "0 2strl_constrain_index", strl_constrain_index(32, 0, 30), 30 );
    MASSERT2( sizeeq, "0 3strl_constrain_index", strl_constrain_index(32, 0, 31), 31 );
    MASSERT2( sizeeq, "0 4strl_constrain_index", strl_constrain_index(32, 0, 32), 31 );
    MASSERT2( sizeeq, "0 5strl_constrain_index", strl_constrain_index(32, 0, 32), 31 );
    MASSERT2( sizeeq, "0 6strl_constrain_index", strl_constrain_index(32, 0, 33), 31 );
    MASSERT2( sizeeq, "0 7strl_constrain_index", strl_constrain_index(32, 0, 34), 31 );
    MASSERT2( sizeeq, "0 8strl_constrain_index", strl_constrain_index(32, 0, 35), 31 );
    MASSERT2( sizeeq, "0 9strl_constrain_index", strl_constrain_index(32, 0, 36), 31 );
    MASSERT2( sizeeq, "0 Astrl_constrain_index", strl_constrain_index(32, 0, 64), 31 );

    MASSERT2( sizeeq, "1 0strl_constrain_index", strl_constrain_index(32, 1, 8),  8  );
    MASSERT2( sizeeq, "1 1strl_constrain_index", strl_constrain_index(32, 1, 29), 29 );
    MASSERT2( sizeeq, "1 2strl_constrain_index", strl_constrain_index(32, 1, 30), 30 );
    MASSERT2( sizeeq, "1 3strl_constrain_index", strl_constrain_index(32, 1, 31), 30 );
    MASSERT2( sizeeq, "1 4strl_constrain_index", strl_constrain_index(32, 1, 32), 30 );
    MASSERT2( sizeeq, "1 5strl_constrain_index", strl_constrain_index(32, 1, 32), 30 );
    MASSERT2( sizeeq, "1 6strl_constrain_index", strl_constrain_index(32, 1, 33), 30 );
    MASSERT2( sizeeq, "1 7strl_constrain_index", strl_constrain_index(32, 1, 34), 30 );
    MASSERT2( sizeeq, "1 8strl_constrain_index", strl_constrain_index(32, 1, 35), 30 );
    MASSERT2( sizeeq, "1 9strl_constrain_index", strl_constrain_index(32, 1, 36), 30 );
    MASSERT2( sizeeq, "1 Astrl_constrain_index", strl_constrain_index(32, 1, 64), 30 );

    /* ---------------------------------------------------------------- */

    MASSERT2( sizeeq, "strl_len_constrain_index strlcpy", strlcpy(dest, love, sizeof(dest)), 4 );
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t util_equalities_str_test =
  {  util_equalities_str_test_run
  , "util_equalities_str_test"
  , "Aptly assorted equality tests for \"util\"'s string utility procedures."
  };

unit_test_result_t util_equalities_str_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
#ifdef TODO /* TODO */
#error "test_util: More unit tests!  Tests for strinsert_fixedwidth onwards."
#else /* #ifdef TODO /-* TODO *-/ */
    return UNIT_TEST_SKIPPED_CONTINUE;
#endif /* #ifdef TODO /-* TODO *-/ */
  }

  return result;
}

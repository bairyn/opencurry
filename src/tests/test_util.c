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

    LASSERT2( inteq, min_int( 3,  7),  3 );
    LASSERT2( inteq, max_int( 3,  7),  7 );
    LASSERT2( inteq, min_int(-3, -7), -7 );
    LASSERT2( inteq, max_int(-3, -7), -3 );

    LASSERT2( sizeeq, min_size(3,  7),  3 );
    LASSERT2( sizeeq, max_size(3,  7),  7 );

    LASSERT2( sizeeq, size_less_null(0), 0 );
    LASSERT2( sizeeq, size_less_null(1), 0 );
    LASSERT2( sizeeq, size_less_null(2), 1 );

    LASSERT2( inteq, proc_true(),  1 );
    LASSERT2( inteq, proc_false(), 0 );

    LASSERT2( inteq, proc_cond(0)(), 0 );
    LASSERT2( inteq, proc_cond(1)(), 1 );

    LASSERT2( objpeq, proc_context(&object), &object );

    LASSERT2( inteq, proc_true_context(&object),  1 );
    LASSERT2( inteq, proc_false_context(&object), 0 );

    LASSERT2( inteq, is_big_endian(), !are_bytes_reversed() );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, size_minus(8, 0),  8  );
    LASSERT2( sizeeq, size_minus(8, 1),  7  );
    LASSERT2( sizeeq, size_minus(8, 7),  1  );
    LASSERT2( sizeeq, size_minus(8, 8),  0  );
    LASSERT2( sizeeq, size_minus(8, 9),  0  );
    LASSERT2( sizeeq, size_minus(8, 10), 0  );
    LASSERT2( sizeeq, size_minus(8, 99), 0  );

    LASSERT2( sizeeq, size_minus(1, 0),  1  );
    LASSERT2( sizeeq, size_minus(1, 1),  0  );
    LASSERT2( sizeeq, size_minus(1, 2),  0  );
    LASSERT2( sizeeq, size_minus(1, 4),  0  );
    LASSERT2( sizeeq, size_minus(1, 8),  0  );
    LASSERT2( sizeeq, size_minus(1, 99), 0  );

    LASSERT2( sizeeq, size_minus(0, 0),  0  );
    LASSERT2( sizeeq, size_minus(0, 1),  0  );
    LASSERT2( sizeeq, size_minus(0, 2),  0  );
    LASSERT2( sizeeq, size_minus(0, 4),  0  );
    LASSERT2( sizeeq, size_minus(0, 8),  0  );
    LASSERT2( sizeeq, size_minus(0, 99), 0  );


    LASSERT2( sizeeq, size_minus(0,  8), 0  );
    LASSERT2( sizeeq, size_minus(1,  8), 0  );
    LASSERT2( sizeeq, size_minus(7,  8), 0  );
    LASSERT2( sizeeq, size_minus(8,  8), 0  );
    LASSERT2( sizeeq, size_minus(9,  8), 1  );
    LASSERT2( sizeeq, size_minus(10, 8), 2  );
    LASSERT2( sizeeq, size_minus(99, 8), 91 );

    LASSERT2( sizeeq, size_minus(0,  1), 0  );
    LASSERT2( sizeeq, size_minus(1,  1), 0  );
    LASSERT2( sizeeq, size_minus(2,  1), 1  );
    LASSERT2( sizeeq, size_minus(4,  1), 3  );
    LASSERT2( sizeeq, size_minus(8,  1), 7  );
    LASSERT2( sizeeq, size_minus(99, 1), 98 );

    LASSERT2( sizeeq, size_minus(0,  0), 0  );
    LASSERT2( sizeeq, size_minus(1,  0), 1  );
    LASSERT2( sizeeq, size_minus(2,  0), 2  );
    LASSERT2( sizeeq, size_minus(4,  0), 4  );
    LASSERT2( sizeeq, size_minus(8,  0), 8  );
    LASSERT2( sizeeq, size_minus(99, 0), 99 );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, size_distance(8, 0),  8  );
    LASSERT2( sizeeq, size_distance(8, 1),  7  );
    LASSERT2( sizeeq, size_distance(8, 7),  1  );
    LASSERT2( sizeeq, size_distance(8, 8),  0  );
    LASSERT2( sizeeq, size_distance(8, 9),  1  );
    LASSERT2( sizeeq, size_distance(8, 10), 2  );
    LASSERT2( sizeeq, size_distance(8, 99), 91 );

    LASSERT2( sizeeq, size_distance(1, 0),  1  );
    LASSERT2( sizeeq, size_distance(1, 1),  0  );
    LASSERT2( sizeeq, size_distance(1, 2),  1  );
    LASSERT2( sizeeq, size_distance(1, 4),  3  );
    LASSERT2( sizeeq, size_distance(1, 8),  7  );
    LASSERT2( sizeeq, size_distance(1, 99), 98 );

    LASSERT2( sizeeq, size_distance(0, 0),  0  );
    LASSERT2( sizeeq, size_distance(0, 1),  1  );
    LASSERT2( sizeeq, size_distance(0, 2),  2  );
    LASSERT2( sizeeq, size_distance(0, 4),  4  );
    LASSERT2( sizeeq, size_distance(0, 8),  8  );
    LASSERT2( sizeeq, size_distance(0, 99), 99 );


    LASSERT2( sizeeq, size_distance(0,  8), 8  );
    LASSERT2( sizeeq, size_distance(1,  8), 7  );
    LASSERT2( sizeeq, size_distance(7,  8), 1  );
    LASSERT2( sizeeq, size_distance(8,  8), 0  );
    LASSERT2( sizeeq, size_distance(9,  8), 1  );
    LASSERT2( sizeeq, size_distance(10, 8), 2  );
    LASSERT2( sizeeq, size_distance(99, 8), 91 );

    LASSERT2( sizeeq, size_distance(0,  1), 1  );
    LASSERT2( sizeeq, size_distance(1,  1), 0  );
    LASSERT2( sizeeq, size_distance(2,  1), 1  );
    LASSERT2( sizeeq, size_distance(4,  1), 3  );
    LASSERT2( sizeeq, size_distance(8,  1), 7  );
    LASSERT2( sizeeq, size_distance(99, 1), 98 );

    LASSERT2( sizeeq, size_distance(0,  0), 0  );
    LASSERT2( sizeeq, size_distance(1,  0), 1  );
    LASSERT2( sizeeq, size_distance(2,  0), 2  );
    LASSERT2( sizeeq, size_distance(4,  0), 4  );
    LASSERT2( sizeeq, size_distance(8,  0), 8  );
    LASSERT2( sizeeq, size_distance(99, 0), 99 );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, terminator_size(99), 98 );
    LASSERT2( sizeeq, terminator_size(8),  7  );
    LASSERT2( sizeeq, terminator_size(4),  3 );
    LASSERT2( sizeeq, terminator_size(3),  2 );
    LASSERT2( sizeeq, terminator_size(2),  1 );
    LASSERT2( sizeeq, terminator_size(1),  0 );
    LASSERT2( sizeeq, terminator_size(0),  0 );
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

    LASSERT3( streqn,     buf_empty, "",            sizeof(buf_empty) );
    LASSERT3( streqn,     buf_full,  "123",         sizeof(buf_full)  );
    LASSERT3( streqn,     buf_max,   "love",        sizeof(buf_max)   );
    LASSERT3( streqn,     buf_max,   "lovely love", sizeof(buf_max)   );
    LASSERT3( not_streqn, buf_max,   "123",         sizeof(buf_max)   );

    LASSERT3( streqn, buf_empty, "",     sizeof(buf_empty) );
    LASSERT3( streqn, buf_full,  "123",  sizeof(buf_full)  );
    LASSERT3( streqn, buf_max,   "love", sizeof(buf_max)   );

    LASSERT2( inteq, set_null_terminator(buf_empty, 0, sizeof(buf_empty)), 0);
    LASSERT2( inteq, set_null_terminator(buf_full,  3, sizeof(buf_full)),  0);
    LASSERT2( inteq, set_null_terminator(buf_max,   4, sizeof(buf_max)),   1);

    LASSERT3( streqn, buf_empty, "",    sizeof(buf_empty) );
    LASSERT3( streqn, buf_full,  "123", sizeof(buf_full)  );
    LASSERT3( streqn, buf_max,   "lov", sizeof(buf_max)   );

    LASSERT2( inteq, set_null_terminator(buf_empty, 0, sizeof(buf_empty)), 0);
    LASSERT2( inteq, set_null_terminator(buf_full,  3, sizeof(buf_full)),  0);
    LASSERT2( inteq, set_null_terminator(buf_max,   4, sizeof(buf_max)),   1);

    LASSERT3( streqn, buf_empty, "",    sizeof(buf_empty) );
    LASSERT3( streqn, buf_full,  "123", sizeof(buf_full)  );
    LASSERT3( streqn, buf_max,   "lov", sizeof(buf_max)   );

    LASSERT2( inteq, set_null_terminator(buf_empty, 2, 3), 0);
    LASSERT2( inteq, set_null_terminator(buf_full,  2, 3), 0);
    LASSERT2( inteq, set_null_terminator(buf_max,   2, 3), 0);

    LASSERT3( streqn, buf_empty, "",   sizeof(buf_empty) );
    LASSERT3( streqn, buf_full,  "12", sizeof(buf_full)  );
    LASSERT3( streqn, buf_max,   "lo", sizeof(buf_max)   );

    LASSERT2( inteq, set_null_terminator(buf_empty, 2, 2), 1);
    LASSERT2( inteq, set_null_terminator(buf_full,  2, 2), 1);
    LASSERT2( inteq, set_null_terminator(buf_max,   2, 2), 1);

    LASSERT3( streqn, buf_empty, "",  sizeof(buf_empty) );
    LASSERT3( streqn, buf_full,  "1", sizeof(buf_full)  );
    LASSERT3( streqn, buf_max,   "l", sizeof(buf_max)   );

    LASSERT2( inteq, set_null_terminator(buf_empty, 0, 9), 0);
    LASSERT2( inteq, set_null_terminator(buf_full,  0, 9), 0);
    LASSERT2( inteq, set_null_terminator(buf_max,   0, 9), 0);

    LASSERT3( streqn, buf_empty, "", sizeof(buf_empty) );
    LASSERT3( streqn, buf_full,  "", sizeof(buf_full)  );
    LASSERT3( streqn, buf_max,   "", sizeof(buf_max)   );

    LASSERT2( inteq, set_null_terminator(buf_empty, 0, 0), -1);
    LASSERT2( inteq, set_null_terminator(buf_full,  0, 0), -1);
    LASSERT2( inteq, set_null_terminator(buf_max,   0, 0), -1);

    LASSERT3( streqn, buf_empty, "", sizeof(buf_empty) );
    LASSERT3( streqn, buf_full,  "", sizeof(buf_full)  );
    LASSERT3( streqn, buf_max,   "", sizeof(buf_max)   );

    LASSERT2( inteq, set_null_terminator(NULL, 0, 0), -2);
    LASSERT2( inteq, set_null_terminator(NULL, 0, 0), -2);
    LASSERT2( inteq, set_null_terminator(NULL, 0, 0), -2);

    LASSERT3( streqn, buf_empty, "", sizeof(buf_empty) );
    LASSERT3( streqn, buf_full,  "", sizeof(buf_full)  );
    LASSERT3( streqn, buf_max,   "", sizeof(buf_max)   );
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

    LASSERT2( sizeeq, strlcpy(dest, peace, sizeof(dest)), 5 );

    LASSERT2( sizeeq, strn_reverse(dest, sizeof(dest)), 5 );
    LASSERT3( streqn, dest, "ecaep", sizeof(dest) );
    LASSERT2( sizeeq, strn_reverse(dest, sizeof(dest)), 5 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );

    LASSERT2( sizeeq, strn_reverse(dest, 5), 4 );
    LASSERT3( streqn, dest, "caepe", sizeof(dest) );
    LASSERT2( sizeeq, strn_reverse(dest, 5), 4 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );

    LASSERT2( sizeeq, strn_reverse(dest, 4), 3 );
    LASSERT3( streqn, dest, "aepce", sizeof(dest) );
    LASSERT2( sizeeq, strn_reverse(dest, 4), 3 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );

    LASSERT2( sizeeq, strn_reverse(dest, 3), 2 );
    LASSERT3( streqn, dest, "epace", sizeof(dest) );
    LASSERT2( sizeeq, strn_reverse(dest, 3), 2 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );

    LASSERT2( sizeeq, strn_reverse(dest, 2), 1 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );
    LASSERT2( sizeeq, strn_reverse(dest, 2), 1 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );

    LASSERT2( sizeeq, strn_reverse(dest, 1), 0 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );
    LASSERT2( sizeeq, strn_reverse(dest, 1), 0 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );

    LASSERT2( sizeeq, strn_reverse(dest, 0), 0 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );
    LASSERT2( sizeeq, strn_reverse(dest, 0), 0 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );

    /* ---------------------------------------------------------------- */

#ifdef TODO
    #error "TODO: Test (mem|endianness)_*_reverse."
#endif /* #ifdef TODO */

    /* ---------------------------------------------------------------- */

    LASSERT2( inteq, BASE_BINARY,      2  );
    LASSERT2( inteq, BASE_OCTAL,       8  );
    LASSERT2( inteq, BASE_DECIMAL,     10 );
    LASSERT2( inteq, BASE_HEXADECIMAL, 16 );
    LASSERT2( inteq, DEFAULT_BASE,     10 );
    LASSERT2( inteq, DEFAULT_BASE,     BASE_DECIMAL );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, unum_digits(0,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, unum_digits(1,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, unum_digits(8,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, unum_digits(512,      BASE_DECIMAL),                3 );
    LASSERT2( sizeeq, unum_digits(1024,     BASE_DECIMAL),                4 );
    LASSERT2( sizeeq, unum_digits(7777,     BASE_DECIMAL),                4 );
    LASSERT2( sizeeq, unum_digits(7777777,  BASE_DECIMAL),                7 );


    LASSERT2( sizeeq, num_digits (0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, num_digits (1,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, num_digits (8,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, num_digits (512,      BASE_DECIMAL, OPTIONAL_SIGN), 3 );
    LASSERT2( sizeeq, num_digits (1024,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, num_digits (7777,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, num_digits (7777777,  BASE_DECIMAL, OPTIONAL_SIGN), 7 );

    LASSERT2( sizeeq, num_digits (0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, num_digits (-1,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    LASSERT2( sizeeq, num_digits (-8,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    LASSERT2( sizeeq, num_digits (-512,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, num_digits (-1024,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    LASSERT2( sizeeq, num_digits (-7777,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    LASSERT2( sizeeq, num_digits (-7777777, BASE_DECIMAL, OPTIONAL_SIGN), 8 );


    LASSERT2( sizeeq, num_digits (0,        BASE_DECIMAL, FORCE_SIGN), 2 );
    LASSERT2( sizeeq, num_digits (1,        BASE_DECIMAL, FORCE_SIGN), 2 );
    LASSERT2( sizeeq, num_digits (8,        BASE_DECIMAL, FORCE_SIGN), 2 );
    LASSERT2( sizeeq, num_digits (512,      BASE_DECIMAL, FORCE_SIGN), 4 );
    LASSERT2( sizeeq, num_digits (1024,     BASE_DECIMAL, FORCE_SIGN), 5 );
    LASSERT2( sizeeq, num_digits (7777,     BASE_DECIMAL, FORCE_SIGN), 5 );
    LASSERT2( sizeeq, num_digits (7777777,  BASE_DECIMAL, FORCE_SIGN), 8 );

    LASSERT2( sizeeq, num_digits (0,        BASE_DECIMAL, FORCE_SIGN), 2 );
    LASSERT2( sizeeq, num_digits (-1,       BASE_DECIMAL, FORCE_SIGN), 2 );
    LASSERT2( sizeeq, num_digits (-8,       BASE_DECIMAL, FORCE_SIGN), 2 );
    LASSERT2( sizeeq, num_digits (-512,     BASE_DECIMAL, FORCE_SIGN), 4 );
    LASSERT2( sizeeq, num_digits (-1024,    BASE_DECIMAL, FORCE_SIGN), 5 );
    LASSERT2( sizeeq, num_digits (-7777,    BASE_DECIMAL, FORCE_SIGN), 5 );
    LASSERT2( sizeeq, num_digits (-7777777, BASE_DECIMAL, FORCE_SIGN), 8 );


    LASSERT2( sizeeq, unum_digits(0x00,        BASE_HEXADECIMAL),      1 );
    LASSERT2( sizeeq, unum_digits(0x01,        BASE_HEXADECIMAL),      1 );
    LASSERT2( sizeeq, unum_digits(0x08,        BASE_HEXADECIMAL),      1 );
    LASSERT2( sizeeq, unum_digits(0x78,        BASE_HEXADECIMAL),      2 );
    LASSERT2( sizeeq, unum_digits(0x80,        BASE_HEXADECIMAL),      2 );
    LASSERT2( sizeeq, unum_digits(0x88,        BASE_HEXADECIMAL),      2 );
    LASSERT2( sizeeq, unum_digits(0x10F,       BASE_HEXADECIMAL),      3 );
    LASSERT2( sizeeq, unum_digits(0x00000000U, BASE_HEXADECIMAL),      1 );
    LASSERT2( sizeeq, unum_digits(0x00000001U, BASE_HEXADECIMAL),      1 );
    LASSERT2( sizeeq, unum_digits(0x00000010U, BASE_HEXADECIMAL),      2 );
    LASSERT2( sizeeq, unum_digits(0x00000100U, BASE_HEXADECIMAL),      3 );
    LASSERT2( sizeeq, unum_digits(0x00001000U, BASE_HEXADECIMAL),      4 );
    LASSERT2( sizeeq, unum_digits(0x00010000U, BASE_HEXADECIMAL),      5 );
    LASSERT2( sizeeq, unum_digits(0x00100000U, BASE_HEXADECIMAL),      6 );
    LASSERT2( sizeeq, unum_digits(0x01000000U, BASE_HEXADECIMAL),      7 );
    LASSERT2( sizeeq, unum_digits(0x10000000U, BASE_HEXADECIMAL),      8 );
    LASSERT2( sizeeq, unum_digits(0x7777777,   BASE_HEXADECIMAL),      7 );

    /* ---------------------------------------------------------------- */

    LASSERT2( inteq, (int) isign(-7777777), (int) ISIGN(-7777777) );
    LASSERT2( inteq, (int) isign(-8      ), (int) ISIGN(-8      ) );
    LASSERT2( inteq, (int) isign(-1      ), (int) ISIGN(-1      ) );
    LASSERT2( inteq, (int) isign( 0      ), (int) ISIGN( 0      ) );
    LASSERT2( inteq, (int) isign( 1      ), (int) ISIGN( 1      ) );
    LASSERT2( inteq, (int) isign( 8      ), (int) ISIGN( 8      ) );
    LASSERT2( inteq, (int) isign( 7777777), (int) ISIGN( 7777777) );

    LASSERT2( inteq, (int) isign(-7777777), (int) '-' );
    LASSERT2( inteq, (int) isign(-8      ), (int) '-' );
    LASSERT2( inteq, (int) isign(-1      ), (int) '-' );
    LASSERT2( inteq, (int) isign( 0      ), (int) ' ' );
    LASSERT2( inteq, (int) isign( 1      ), (int) '+' );
    LASSERT2( inteq, (int) isign( 8      ), (int) '+' );
    LASSERT2( inteq, (int) isign( 7777777), (int) '+' );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 0,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 1,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 8,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 512,      BASE_DECIMAL),                3 );
    LASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 1024,     BASE_DECIMAL),                4 );
    LASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 7777,     BASE_DECIMAL),                4 );
    LASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 7777777,  BASE_DECIMAL),                7 );

    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 1,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 8,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 512,      BASE_DECIMAL, OPTIONAL_SIGN), 3 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 1024,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 7777,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 7777777,  BASE_DECIMAL, OPTIONAL_SIGN), 7 );

    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -1,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -8,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -512,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -1024,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -7777,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -7777777, BASE_DECIMAL, OPTIONAL_SIGN), 8 );

    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 0,        BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 1,        BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 8,        BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 512,      BASE_DECIMAL, FORCE_SIGN),    4 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 1024,     BASE_DECIMAL, FORCE_SIGN),    5 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 7777,     BASE_DECIMAL, FORCE_SIGN),    5 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 7777777,  BASE_DECIMAL, FORCE_SIGN),    8 );

    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), 0,        BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -1,       BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -8,       BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -512,     BASE_DECIMAL, FORCE_SIGN),    4 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -1024,    BASE_DECIMAL, FORCE_SIGN),    5 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -7777,    BASE_DECIMAL, FORCE_SIGN),    5 );
    LASSERT2( sizeeq, itoa (NULL, sizeof(dest), -7777777, BASE_DECIMAL, FORCE_SIGN),    8 );


    LASSERT2( sizeeq, uitoa(NULL, 0,            0,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, uitoa(NULL, 0,            1,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, uitoa(NULL, 0,            8,        BASE_DECIMAL),                1 );
    LASSERT2( sizeeq, uitoa(NULL, 0,            512,      BASE_DECIMAL),                3 );
    LASSERT2( sizeeq, uitoa(NULL, 0,            1024,     BASE_DECIMAL),                4 );
    LASSERT2( sizeeq, uitoa(NULL, 0,            7777,     BASE_DECIMAL),                4 );
    LASSERT2( sizeeq, uitoa(NULL, 0,            7777777,  BASE_DECIMAL),                7 );

    LASSERT2( sizeeq, itoa (NULL, 0,            0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, itoa (NULL, 0,            1,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, itoa (NULL, 0,            8,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, itoa (NULL, 0,            512,      BASE_DECIMAL, OPTIONAL_SIGN), 3 );
    LASSERT2( sizeeq, itoa (NULL, 0,            1024,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, itoa (NULL, 0,            7777,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, itoa (NULL, 0,            7777777,  BASE_DECIMAL, OPTIONAL_SIGN), 7 );

    LASSERT2( sizeeq, itoa (NULL, 0,            0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -1,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -8,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -512,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -1024,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -7777,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -7777777, BASE_DECIMAL, OPTIONAL_SIGN), 8 );

    LASSERT2( sizeeq, itoa (NULL, 0,            0,        BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, 0,            1,        BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, 0,            8,        BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, 0,            512,      BASE_DECIMAL, FORCE_SIGN),    4 );
    LASSERT2( sizeeq, itoa (NULL, 0,            1024,     BASE_DECIMAL, FORCE_SIGN),    5 );
    LASSERT2( sizeeq, itoa (NULL, 0,            7777,     BASE_DECIMAL, FORCE_SIGN),    5 );
    LASSERT2( sizeeq, itoa (NULL, 0,            7777777,  BASE_DECIMAL, FORCE_SIGN),    8 );

    LASSERT2( sizeeq, itoa (NULL, 0,            0,        BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -1,       BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -8,       BASE_DECIMAL, FORCE_SIGN),    2 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -512,     BASE_DECIMAL, FORCE_SIGN),    4 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -1024,    BASE_DECIMAL, FORCE_SIGN),    5 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -7777,    BASE_DECIMAL, FORCE_SIGN),    5 );
    LASSERT2( sizeeq, itoa (NULL, 0,            -7777777, BASE_DECIMAL, FORCE_SIGN),    8 );


    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0,        BASE_DECIMAL), 1 );
      LASSERT3( nstreq, sizeof(dest), dest,    "0" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 1,        BASE_DECIMAL), 1 );
      LASSERT3( nstreq, sizeof(dest), dest,    "1" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 8,        BASE_DECIMAL), 1 );
      LASSERT3( nstreq, sizeof(dest), dest,    "8" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 512,      BASE_DECIMAL), 3 );
      LASSERT3( nstreq, sizeof(dest), dest,    "512" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 1024,     BASE_DECIMAL), 4 );
      LASSERT3( nstreq, sizeof(dest), dest,    "1024" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 7777,     BASE_DECIMAL), 4 );
      LASSERT3( nstreq, sizeof(dest), dest,    "7777" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 7777777,  BASE_DECIMAL), 7 );
      LASSERT3( nstreq, sizeof(dest), dest,    "7777777" );


    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 0,         BASE_DECIMAL, OPTIONAL_SIGN), 1 );
      LASSERT3( nstreq, sizeof(dest), dest,    "0" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 1,         BASE_DECIMAL, OPTIONAL_SIGN), 1 );
      LASSERT3( nstreq, sizeof(dest), dest,    "1" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 8,         BASE_DECIMAL, OPTIONAL_SIGN), 1 );
      LASSERT3( nstreq, sizeof(dest), dest,    "8" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 512,       BASE_DECIMAL, OPTIONAL_SIGN), 3 );
      LASSERT3( nstreq, sizeof(dest), dest,    "512" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 1024,      BASE_DECIMAL, OPTIONAL_SIGN), 4 );
      LASSERT3( nstreq, sizeof(dest), dest,    "1024" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 7777,      BASE_DECIMAL, OPTIONAL_SIGN), 4 );
      LASSERT3( nstreq, sizeof(dest), dest,    "7777" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 7777777,   BASE_DECIMAL, OPTIONAL_SIGN), 7 );
      LASSERT3( nstreq, sizeof(dest), dest,    "7777777" );

    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 0,         BASE_DECIMAL, OPTIONAL_SIGN), 1 );
      LASSERT3( nstreq, sizeof(dest), dest,    "0" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -1,        BASE_DECIMAL, OPTIONAL_SIGN), 2 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-1" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -8,        BASE_DECIMAL, OPTIONAL_SIGN), 2 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-8" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -512,      BASE_DECIMAL, OPTIONAL_SIGN), 4 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-512" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -1024,     BASE_DECIMAL, OPTIONAL_SIGN), 5 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-1024" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -7777,     BASE_DECIMAL, OPTIONAL_SIGN), 5 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-7777" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -7777777,  BASE_DECIMAL, OPTIONAL_SIGN), 8 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-7777777" );


    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 0,         BASE_DECIMAL, FORCE_SIGN), 2 );
      LASSERT3( nstreq, sizeof(dest), dest,    " 0" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 1,         BASE_DECIMAL, FORCE_SIGN), 2 );
      LASSERT3( nstreq, sizeof(dest), dest,    "+1" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 8,         BASE_DECIMAL, FORCE_SIGN), 2 );
      LASSERT3( nstreq, sizeof(dest), dest,    "+8" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 512,       BASE_DECIMAL, FORCE_SIGN), 4 );
      LASSERT3( nstreq, sizeof(dest), dest,    "+512" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 1024,      BASE_DECIMAL, FORCE_SIGN), 5 );
      LASSERT3( nstreq, sizeof(dest), dest,    "+1024" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 7777,      BASE_DECIMAL, FORCE_SIGN), 5 );
      LASSERT3( nstreq, sizeof(dest), dest,    "+7777" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 7777777,   BASE_DECIMAL, FORCE_SIGN), 8 );
      LASSERT3( nstreq, sizeof(dest), dest,    "+7777777" );

    LASSERT2( sizeeq, itoa(dest, sizeof(dest), 0,         BASE_DECIMAL, FORCE_SIGN), 2 );
      LASSERT3( nstreq, sizeof(dest), dest,    " 0" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -1,        BASE_DECIMAL, FORCE_SIGN), 2 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-1" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -8,        BASE_DECIMAL, FORCE_SIGN), 2 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-8" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -512,      BASE_DECIMAL, FORCE_SIGN), 4 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-512" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -1024,     BASE_DECIMAL, FORCE_SIGN), 5 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-1024" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -7777,     BASE_DECIMAL, FORCE_SIGN), 5 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-7777" );
    LASSERT2( sizeeq, itoa(dest, sizeof(dest), -7777777,  BASE_DECIMAL, FORCE_SIGN), 8 );
      LASSERT3( nstreq, sizeof(dest), dest,    "-7777777" );


    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00,        BASE_HEXADECIMAL),      1 );
      LASSERT3( nstreq, sizeof(dest), dest, "0" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x01,        BASE_HEXADECIMAL),      1 );
      LASSERT3( nstreq, sizeof(dest), dest, "1" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x08,        BASE_HEXADECIMAL),      1 );
      LASSERT3( nstreq, sizeof(dest), dest, "8" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x78,        BASE_HEXADECIMAL),      2 );
      LASSERT3( nstreq, sizeof(dest), dest, "78" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x80,        BASE_HEXADECIMAL),      2 );
      LASSERT3( nstreq, sizeof(dest), dest, "80" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x88,        BASE_HEXADECIMAL),      2 );
      LASSERT3( nstreq, sizeof(dest), dest, "88" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x10F,       BASE_HEXADECIMAL),      3 );
      LASSERT3( nstreq, sizeof(dest), dest, "10F" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00000000U, BASE_HEXADECIMAL),      1 );
      LASSERT3( nstreq, sizeof(dest), dest, "0" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00000001U, BASE_HEXADECIMAL),      1 );
      LASSERT3( nstreq, sizeof(dest), dest, "1" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00000010U, BASE_HEXADECIMAL),      2 );
      LASSERT3( nstreq, sizeof(dest), dest, "10" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00000100U, BASE_HEXADECIMAL),      3 );
      LASSERT3( nstreq, sizeof(dest), dest, "100" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00001000U, BASE_HEXADECIMAL),      4 );
      LASSERT3( nstreq, sizeof(dest), dest, "1000" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00010000U, BASE_HEXADECIMAL),      5 );
      LASSERT3( nstreq, sizeof(dest), dest, "10000" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00100000U, BASE_HEXADECIMAL),      6 );
      LASSERT3( nstreq, sizeof(dest), dest, "100000" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x01000000U, BASE_HEXADECIMAL),      7 );
      LASSERT3( nstreq, sizeof(dest), dest, "1000000" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x10000000U, BASE_HEXADECIMAL),      8 );
      LASSERT3( nstreq, sizeof(dest), dest, "10000000" );
    LASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x7777777,   BASE_HEXADECIMAL),      7 );
      LASSERT3( nstreq, sizeof(dest), dest, "7777777" );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, strlcpy(dest,  love, 5), 4 );
    LASSERT3( streqn, dest, "love",  sizeof(dest) );

    LASSERT2( sizeeq, strlcpy(dest,  love, 4), 3);
    LASSERT3( streqn, dest, "lov",   sizeof(dest) );

    LASSERT2( sizeeq, strlcpy(dest,  love, 1), 0);
    LASSERT3( streqn, dest, "",      sizeof(dest) );


    LASSERT2( sizeeq, strlcpy(dest,  peace, sizeof(dest)), 5 );
    LASSERT2( sizeeq, strlcpy(dest,  love,  0),            0 );
    LASSERT3( streqn, dest, "peace", sizeof(dest) );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    LASSERT3( streqn, smalldest, "peace", sizeof(smalldest) );

    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 4), 4 );
    LASSERT3( streqn, smalldest, "peac",  sizeof(smalldest) );


    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 5, 5), 4 );
    LASSERT3( streqn, smalldest, "peac",  sizeof(smalldest) );

    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 5, 4), 4 );
    LASSERT3( streqn, smalldest, "peac",  sizeof(smalldest) );

    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 5, 3), 3 );
    LASSERT3( streqn, smalldest, "pea",   sizeof(smalldest) );

    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 5, 2), 2 );
    LASSERT3( streqn, smalldest, "pe",    sizeof(smalldest) );


    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 4, 5), 3 );
    LASSERT3( streqn, smalldest, "pea",   sizeof(smalldest) );

    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 4, 4), 3 );
    LASSERT3( streqn, smalldest, "pea",   sizeof(smalldest) );

    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 4, 3), 3 );
    LASSERT3( streqn, smalldest, "pea",   sizeof(smalldest) );

    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 4, 2), 2 );
    LASSERT3( streqn, smalldest, "pe",    sizeof(smalldest) );


    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 0), 0 );
    LASSERT3( streqn, smalldest, "",      sizeof(smalldest) );


    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    LASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 0, 5), 0 );
    LASSERT3( streqn, smalldest, "peace", sizeof(smalldest) );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    LASSERT3( streqn, dest, "love", sizeof(dest) );


    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 3), 3 );
    LASSERT3( streqn, dest, "lov",  sizeof(dest) );

    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 3, 4), 3 );
    LASSERT3( streqn, dest, "lov",  sizeof(dest) );

    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 3, 3), 3 );
    LASSERT3( streqn, dest, "lov",  sizeof(dest) );


    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 0, 4), 0 );
    LASSERT3( streqn, dest, "",     sizeof(dest) );

    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 0), 0 );
    LASSERT3( streqn, dest, "",     sizeof(dest) );

    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 0, 0), 0 );
    LASSERT3( streqn, dest, "",     sizeof(dest) );

    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 1, 4, 4), 0 );
    LASSERT3( streqn, dest, "",     sizeof(dest) );

    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    LASSERT2( sizeeq, strlcpy_with_max(dest, love, 0, 4, 4), 0 );
    LASSERT3( streqn, dest, "love", sizeof(dest) );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, strlcpy(dest, love, sizeof(dest)), 4 );

    LASSERT2( sizeeq, strnlen(dest, 7), 4 );
    LASSERT2( sizeeq, strnlen(dest, 6), 4 );
    LASSERT2( sizeeq, strnlen(dest, 5), 4 );
    LASSERT2( sizeeq, strnlen(dest, 4), 4 );
    LASSERT2( sizeeq, strnlen(dest, 3), 3 );
    LASSERT2( sizeeq, strnlen(dest, 2), 2 );
    LASSERT2( sizeeq, strnlen(dest, 1), 1 );
    LASSERT2( sizeeq, strnlen(dest, 0), 0 );

    LASSERT2( sizeeq, strllen(dest, 7), 4 );
    LASSERT2( sizeeq, strllen(dest, 6), 4 );
    LASSERT2( sizeeq, strllen(dest, 5), 4 );
    LASSERT2( sizeeq, strllen(dest, 4), 3 );
    LASSERT2( sizeeq, strllen(dest, 3), 2 );
    LASSERT2( sizeeq, strllen(dest, 2), 1 );
    LASSERT2( sizeeq, strllen(dest, 1), 0 );
    LASSERT2( sizeeq, strllen(dest, 0), 0 );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, strlcpy(dest, love, sizeof(dest)), 4 );

    LASSERT1( true,  strn_has_null_terminator(dest, sizeof(dest)) );

    LASSERT1( true,  strn_has_null_terminator(dest, 5) );
    LASSERT1( false, strn_has_null_terminator(dest, 4) );
    LASSERT1( false, strn_has_null_terminator(dest, 3) );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 1),  1 );
    LASSERT3( nstreq, sizeof(dest), dest, "l" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 5),  5 );
    LASSERT3( nstreq, sizeof(dest), dest, "lovel" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 8),  8 );
    LASSERT3( nstreq, sizeof(dest), dest, "lovelove" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 9),  9 );
    LASSERT3( nstreq, sizeof(dest), dest, "lovelovel" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 12), 12 );
    LASSERT3( nstreq, sizeof(dest), dest, "lovelovelove" );


    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 5,            4),  4 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 2,            4),  1 );
    LASSERT3( nstreq, sizeof(dest), dest, "l" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 1,            4),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            4),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );


    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, "",   5,            4),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, "",   2,            4),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, "",   1,            4),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, "",   0,            4),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );


    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 3,            8),  2 );
    LASSERT3( nstreq, sizeof(dest), dest, "lo" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 5,            8),  4 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            8),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );


    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 3,            10), 2 );
    LASSERT3( nstreq, sizeof(dest), dest, "lo" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 5,            10), 4 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 9,            10), 8 );
    LASSERT3( nstreq, sizeof(dest), dest, "lovelove" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            10), 0 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );


    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 1,            1),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 1,            0),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            1),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );

    LASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    LASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            0),  0 );
    LASSERT3( nstreq, sizeof(dest), dest, "love" );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, index_bounds(32, 0, 8),  8  );
    LASSERT2( sizeeq, index_bounds(32, 0, 29), 29 );
    LASSERT2( sizeeq, index_bounds(32, 0, 30), 30 );
    LASSERT2( sizeeq, index_bounds(32, 0, 31), 31 );
    LASSERT2( sizeeq, index_bounds(32, 0, 32), 31 );
    LASSERT2( sizeeq, index_bounds(32, 0, 32), 31 );
    LASSERT2( sizeeq, index_bounds(32, 0, 33), 31 );
    LASSERT2( sizeeq, index_bounds(32, 0, 34), 31 );
    LASSERT2( sizeeq, index_bounds(32, 0, 35), 31 );
    LASSERT2( sizeeq, index_bounds(32, 0, 36), 31 );
    LASSERT2( sizeeq, index_bounds(32, 0, 64), 31 );

    LASSERT2( sizeeq, index_bounds(32, 1, 8),  8  );
    LASSERT2( sizeeq, index_bounds(32, 1, 29), 29 );
    LASSERT2( sizeeq, index_bounds(32, 1, 30), 30 );
    LASSERT2( sizeeq, index_bounds(32, 1, 31), 30 );
    LASSERT2( sizeeq, index_bounds(32, 1, 32), 30 );
    LASSERT2( sizeeq, index_bounds(32, 1, 32), 30 );
    LASSERT2( sizeeq, index_bounds(32, 1, 33), 30 );
    LASSERT2( sizeeq, index_bounds(32, 1, 34), 30 );
    LASSERT2( sizeeq, index_bounds(32, 1, 35), 30 );
    LASSERT2( sizeeq, index_bounds(32, 1, 36), 30 );
    LASSERT2( sizeeq, index_bounds(32, 1, 64), 30 );

    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, strlcpy(dest, love, sizeof(dest)), 4 );

    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 8), 4 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 7), 4 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 4), 4 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 3), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 2), 2 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 1), 1 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 0), 0 );

    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 8), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 7), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 4), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 3), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 2), 2 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 1), 1 );
    LASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 0), 0 );


    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 8), 4 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 7), 4 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 4), 4 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 3), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 2), 2 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 1), 1 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 0), 0 );

    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 8), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 7), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 4), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 3), 3 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 2), 2 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 1), 1 );
    LASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 0), 0 );


    /* ---------------------------------------------------------------- */

    LASSERT2( sizeeq, str_index_bounds(love, 0, 8), 4 );
    LASSERT2( sizeeq, str_index_bounds(love, 0, 7), 4 );
    LASSERT2( sizeeq, str_index_bounds(love, 0, 4), 4 );
    LASSERT2( sizeeq, str_index_bounds(love, 0, 3), 3 );
    LASSERT2( sizeeq, str_index_bounds(love, 0, 2), 2 );
    LASSERT2( sizeeq, str_index_bounds(love, 0, 1), 1 );
    LASSERT2( sizeeq, str_index_bounds(love, 0, 0), 0 );

    LASSERT2( sizeeq, str_index_bounds(love, 1, 8), 3 );
    LASSERT2( sizeeq, str_index_bounds(love, 1, 7), 3 );
    LASSERT2( sizeeq, str_index_bounds(love, 1, 4), 3 );
    LASSERT2( sizeeq, str_index_bounds(love, 1, 3), 3 );
    LASSERT2( sizeeq, str_index_bounds(love, 1, 2), 2 );
    LASSERT2( sizeeq, str_index_bounds(love, 1, 1), 1 );
    LASSERT2( sizeeq, str_index_bounds(love, 1, 0), 0 );
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

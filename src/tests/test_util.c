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

    ASSERT2( sizeeq, strlcpy(dest, peace, sizeof(dest)), 5 );

    ASSERT2( sizeeq, strn_reverse(dest, sizeof(dest)), 5 );
    ASSERT3( streqn, dest, "ecaep", sizeof(dest) );
    ASSERT2( sizeeq, strn_reverse(dest, sizeof(dest)), 5 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );

    ASSERT2( sizeeq, strn_reverse(dest, 5), 4 );
    ASSERT3( streqn, dest, "caepe", sizeof(dest) );
    ASSERT2( sizeeq, strn_reverse(dest, 5), 4 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );

    ASSERT2( sizeeq, strn_reverse(dest, 4), 3 );
    ASSERT3( streqn, dest, "aepce", sizeof(dest) );
    ASSERT2( sizeeq, strn_reverse(dest, 4), 3 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );

    ASSERT2( sizeeq, strn_reverse(dest, 3), 2 );
    ASSERT3( streqn, dest, "epace", sizeof(dest) );
    ASSERT2( sizeeq, strn_reverse(dest, 3), 2 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );

    ASSERT2( sizeeq, strn_reverse(dest, 2), 1 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );
    ASSERT2( sizeeq, strn_reverse(dest, 2), 1 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );

    ASSERT2( sizeeq, strn_reverse(dest, 1), 0 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );
    ASSERT2( sizeeq, strn_reverse(dest, 1), 0 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );

    ASSERT2( sizeeq, strn_reverse(dest, 0), 0 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );
    ASSERT2( sizeeq, strn_reverse(dest, 0), 0 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );

    /* ---------------------------------------------------------------- */

#ifdef TODO
    #error "TODO: Test (mem|endianness)_*_reverse."
#endif /* #ifdef TODO */

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, BASE_BINARY,      2  );
    ASSERT2( inteq, BASE_OCTAL,       8  );
    ASSERT2( inteq, BASE_DECIMAL,     10 );
    ASSERT2( inteq, BASE_HEXADECIMAL, 16 );
    ASSERT2( inteq, DEFAULT_BASE,     10 );
    ASSERT2( inteq, DEFAULT_BASE,     BASE_DECIMAL );

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, digit(0,    BASE_DECIMAL),     (int) '0' );
    ASSERT2( inteq, digit(1,    BASE_DECIMAL),     (int) '1' );
    ASSERT2( inteq, digit(2,    BASE_DECIMAL),     (int) '2' );
    ASSERT2( inteq, digit(3,    BASE_DECIMAL),     (int) '3' );
    ASSERT2( inteq, digit(4,    BASE_DECIMAL),     (int) '4' );
    ASSERT2( inteq, digit(5,    BASE_DECIMAL),     (int) '5' );
    ASSERT2( inteq, digit(6,    BASE_DECIMAL),     (int) '6' );
    ASSERT2( inteq, digit(7,    BASE_DECIMAL),     (int) '7' );
    ASSERT2( inteq, digit(8,    BASE_DECIMAL),     (int) '8' );
    ASSERT2( inteq, digit(9,    BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0xA,  BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0xB,  BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0xC,  BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0xD,  BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0xE,  BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0xF,  BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0x10, BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0x11, BASE_DECIMAL),     (int) '9' );
    ASSERT2( inteq, digit(0x12, BASE_DECIMAL),     (int) '9' );

    ASSERT2( inteq, digit(0,    BASE_HEXADECIMAL), (int) '0' );
    ASSERT2( inteq, digit(1,    BASE_HEXADECIMAL), (int) '1' );
    ASSERT2( inteq, digit(2,    BASE_HEXADECIMAL), (int) '2' );
    ASSERT2( inteq, digit(3,    BASE_HEXADECIMAL), (int) '3' );
    ASSERT2( inteq, digit(4,    BASE_HEXADECIMAL), (int) '4' );
    ASSERT2( inteq, digit(5,    BASE_HEXADECIMAL), (int) '5' );
    ASSERT2( inteq, digit(6,    BASE_HEXADECIMAL), (int) '6' );
    ASSERT2( inteq, digit(7,    BASE_HEXADECIMAL), (int) '7' );
    ASSERT2( inteq, digit(8,    BASE_HEXADECIMAL), (int) '8' );
    ASSERT2( inteq, digit(9,    BASE_HEXADECIMAL), (int) '9' );
    ASSERT2( inteq, digit(0xA,  BASE_HEXADECIMAL), (int) 'A' );
    ASSERT2( inteq, digit(0xB,  BASE_HEXADECIMAL), (int) 'B' );
    ASSERT2( inteq, digit(0xC,  BASE_HEXADECIMAL), (int) 'C' );
    ASSERT2( inteq, digit(0xD,  BASE_HEXADECIMAL), (int) 'D' );
    ASSERT2( inteq, digit(0xE,  BASE_HEXADECIMAL), (int) 'E' );
    ASSERT2( inteq, digit(0xF,  BASE_HEXADECIMAL), (int) 'F' );
    ASSERT2( inteq, digit(0x10, BASE_HEXADECIMAL), (int) 'F' );
    ASSERT2( inteq, digit(0x11, BASE_HEXADECIMAL), (int) 'F' );
    ASSERT2( inteq, digit(0x12, BASE_HEXADECIMAL), (int) 'F' );

    ASSERT2( inteq, digit(0,    BASE_OCTAL),       (int) '0' );
    ASSERT2( inteq, digit(1,    BASE_OCTAL),       (int) '1' );
    ASSERT2( inteq, digit(2,    BASE_OCTAL),       (int) '2' );
    ASSERT2( inteq, digit(3,    BASE_OCTAL),       (int) '3' );
    ASSERT2( inteq, digit(4,    BASE_OCTAL),       (int) '4' );
    ASSERT2( inteq, digit(5,    BASE_OCTAL),       (int) '5' );
    ASSERT2( inteq, digit(6,    BASE_OCTAL),       (int) '6' );
    ASSERT2( inteq, digit(7,    BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(8,    BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(9,    BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0xA,  BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0xB,  BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0xC,  BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0xD,  BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0xE,  BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0xF,  BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0x10, BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0x11, BASE_OCTAL),       (int) '7' );
    ASSERT2( inteq, digit(0x12, BASE_OCTAL),       (int) '7' );

    ASSERT2( inteq, digit(0,    BASE_BINARY),      (int) '0' );
    ASSERT2( inteq, digit(1,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(2,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(3,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(4,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(5,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(6,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(7,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(8,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(9,    BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0xA,  BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0xB,  BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0xC,  BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0xD,  BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0xE,  BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0xF,  BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0x10, BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0x11, BASE_BINARY),      (int) '1' );
    ASSERT2( inteq, digit(0x12, BASE_BINARY),      (int) '1' );

    ASSERT2( inteq, digit(0,    3),                (int) '0' );
    ASSERT2( inteq, digit(1,    3),                (int) '1' );
    ASSERT2( inteq, digit(2,    3),                (int) '2' );
    ASSERT2( inteq, digit(3,    3),                (int) '2' );
    ASSERT2( inteq, digit(4,    3),                (int) '2' );
    ASSERT2( inteq, digit(5,    3),                (int) '2' );
    ASSERT2( inteq, digit(6,    3),                (int) '2' );
    ASSERT2( inteq, digit(7,    3),                (int) '2' );
    ASSERT2( inteq, digit(8,    3),                (int) '2' );
    ASSERT2( inteq, digit(9,    3),                (int) '2' );
    ASSERT2( inteq, digit(0xA,  3),                (int) '2' );
    ASSERT2( inteq, digit(0xB,  3),                (int) '2' );
    ASSERT2( inteq, digit(0xC,  3),                (int) '2' );
    ASSERT2( inteq, digit(0xD,  3),                (int) '2' );
    ASSERT2( inteq, digit(0xE,  3),                (int) '2' );
    ASSERT2( inteq, digit(0xF,  3),                (int) '2' );
    ASSERT2( inteq, digit(0x10, 3),                (int) '2' );
    ASSERT2( inteq, digit(0x11, 3),                (int) '2' );
    ASSERT2( inteq, digit(0x12, 3),                (int) '2' );

    ASSERT2( inteq, digit(0,    17),               (int) '0' );
    ASSERT2( inteq, digit(1,    17),               (int) '1' );
    ASSERT2( inteq, digit(2,    17),               (int) '2' );
    ASSERT2( inteq, digit(3,    17),               (int) '3' );
    ASSERT2( inteq, digit(4,    17),               (int) '4' );
    ASSERT2( inteq, digit(5,    17),               (int) '5' );
    ASSERT2( inteq, digit(6,    17),               (int) '6' );
    ASSERT2( inteq, digit(7,    17),               (int) '7' );
    ASSERT2( inteq, digit(8,    17),               (int) '8' );
    ASSERT2( inteq, digit(9,    17),               (int) '9' );
    ASSERT2( inteq, digit(0xA,  17),               (int) 'A' );
    ASSERT2( inteq, digit(0xB,  17),               (int) 'B' );
    ASSERT2( inteq, digit(0xC,  17),               (int) 'C' );
    ASSERT2( inteq, digit(0xD,  17),               (int) 'D' );
    ASSERT2( inteq, digit(0xE,  17),               (int) 'E' );
    ASSERT2( inteq, digit(0xF,  17),               (int) 'F' );
    ASSERT2( inteq, digit(0x10, 17),               (int) 'G' );
    ASSERT2( inteq, digit(0x11, 17),               (int) 'G' );
    ASSERT2( inteq, digit(0x12, 17),               (int) 'G' );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, unum_digits(0,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, unum_digits(1,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, unum_digits(8,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, unum_digits(512,      BASE_DECIMAL),                3 );
    ASSERT2( sizeeq, unum_digits(1024,     BASE_DECIMAL),                4 );
    ASSERT2( sizeeq, unum_digits(7777,     BASE_DECIMAL),                4 );
    ASSERT2( sizeeq, unum_digits(7777777,  BASE_DECIMAL),                7 );


    ASSERT2( sizeeq, num_digits (0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, num_digits (1,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, num_digits (8,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, num_digits (512,      BASE_DECIMAL, OPTIONAL_SIGN), 3 );
    ASSERT2( sizeeq, num_digits (1024,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, num_digits (7777,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, num_digits (7777777,  BASE_DECIMAL, OPTIONAL_SIGN), 7 );

    ASSERT2( sizeeq, num_digits (0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, num_digits (-1,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    ASSERT2( sizeeq, num_digits (-8,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    ASSERT2( sizeeq, num_digits (-512,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, num_digits (-1024,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    ASSERT2( sizeeq, num_digits (-7777,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    ASSERT2( sizeeq, num_digits (-7777777, BASE_DECIMAL, OPTIONAL_SIGN), 8 );


    ASSERT2( sizeeq, num_digits (0,        BASE_DECIMAL, FORCE_SIGN), 2 );
    ASSERT2( sizeeq, num_digits (1,        BASE_DECIMAL, FORCE_SIGN), 2 );
    ASSERT2( sizeeq, num_digits (8,        BASE_DECIMAL, FORCE_SIGN), 2 );
    ASSERT2( sizeeq, num_digits (512,      BASE_DECIMAL, FORCE_SIGN), 4 );
    ASSERT2( sizeeq, num_digits (1024,     BASE_DECIMAL, FORCE_SIGN), 5 );
    ASSERT2( sizeeq, num_digits (7777,     BASE_DECIMAL, FORCE_SIGN), 5 );
    ASSERT2( sizeeq, num_digits (7777777,  BASE_DECIMAL, FORCE_SIGN), 8 );

    ASSERT2( sizeeq, num_digits (0,        BASE_DECIMAL, FORCE_SIGN), 2 );
    ASSERT2( sizeeq, num_digits (-1,       BASE_DECIMAL, FORCE_SIGN), 2 );
    ASSERT2( sizeeq, num_digits (-8,       BASE_DECIMAL, FORCE_SIGN), 2 );
    ASSERT2( sizeeq, num_digits (-512,     BASE_DECIMAL, FORCE_SIGN), 4 );
    ASSERT2( sizeeq, num_digits (-1024,    BASE_DECIMAL, FORCE_SIGN), 5 );
    ASSERT2( sizeeq, num_digits (-7777,    BASE_DECIMAL, FORCE_SIGN), 5 );
    ASSERT2( sizeeq, num_digits (-7777777, BASE_DECIMAL, FORCE_SIGN), 8 );


    ASSERT2( sizeeq, unum_digits(0x00,        BASE_HEXADECIMAL),      1 );
    ASSERT2( sizeeq, unum_digits(0x01,        BASE_HEXADECIMAL),      1 );
    ASSERT2( sizeeq, unum_digits(0x08,        BASE_HEXADECIMAL),      1 );
    ASSERT2( sizeeq, unum_digits(0x78,        BASE_HEXADECIMAL),      2 );
    ASSERT2( sizeeq, unum_digits(0x80,        BASE_HEXADECIMAL),      2 );
    ASSERT2( sizeeq, unum_digits(0x88,        BASE_HEXADECIMAL),      2 );
    ASSERT2( sizeeq, unum_digits(0x10F,       BASE_HEXADECIMAL),      3 );
    ASSERT2( sizeeq, unum_digits(0x00000000U, BASE_HEXADECIMAL),      1 );
    ASSERT2( sizeeq, unum_digits(0x00000001U, BASE_HEXADECIMAL),      1 );
    ASSERT2( sizeeq, unum_digits(0x00000010U, BASE_HEXADECIMAL),      2 );
    ASSERT2( sizeeq, unum_digits(0x00000100U, BASE_HEXADECIMAL),      3 );
    ASSERT2( sizeeq, unum_digits(0x00001000U, BASE_HEXADECIMAL),      4 );
    ASSERT2( sizeeq, unum_digits(0x00010000U, BASE_HEXADECIMAL),      5 );
    ASSERT2( sizeeq, unum_digits(0x00100000U, BASE_HEXADECIMAL),      6 );
    ASSERT2( sizeeq, unum_digits(0x01000000U, BASE_HEXADECIMAL),      7 );
    ASSERT2( sizeeq, unum_digits(0x10000000U, BASE_HEXADECIMAL),      8 );
    ASSERT2( sizeeq, unum_digits(0x7777777,   BASE_HEXADECIMAL),      7 );

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, (int) isign(-7777777), (int) ISIGN(-7777777) );
    ASSERT2( inteq, (int) isign(-8      ), (int) ISIGN(-8      ) );
    ASSERT2( inteq, (int) isign(-1      ), (int) ISIGN(-1      ) );
    ASSERT2( inteq, (int) isign( 0      ), (int) ISIGN( 0      ) );
    ASSERT2( inteq, (int) isign( 1      ), (int) ISIGN( 1      ) );
    ASSERT2( inteq, (int) isign( 8      ), (int) ISIGN( 8      ) );
    ASSERT2( inteq, (int) isign( 7777777), (int) ISIGN( 7777777) );

    ASSERT2( inteq, (int) isign(-7777777), (int) '-' );
    ASSERT2( inteq, (int) isign(-8      ), (int) '-' );
    ASSERT2( inteq, (int) isign(-1      ), (int) '-' );
    ASSERT2( inteq, (int) isign( 0      ), (int) ' ' );
    ASSERT2( inteq, (int) isign( 1      ), (int) '+' );
    ASSERT2( inteq, (int) isign( 8      ), (int) '+' );
    ASSERT2( inteq, (int) isign( 7777777), (int) '+' );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 0,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 1,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 8,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 512,      BASE_DECIMAL),                3 );
    ASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 1024,     BASE_DECIMAL),                4 );
    ASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 7777,     BASE_DECIMAL),                4 );
    ASSERT2( sizeeq, uitoa(NULL, sizeof(dest), 7777777,  BASE_DECIMAL),                7 );

    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 1,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 8,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 512,      BASE_DECIMAL, OPTIONAL_SIGN), 3 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 1024,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 7777,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 7777777,  BASE_DECIMAL, OPTIONAL_SIGN), 7 );

    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -1,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -8,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -512,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -1024,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -7777,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -7777777, BASE_DECIMAL, OPTIONAL_SIGN), 8 );

    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 0,        BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 1,        BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 8,        BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 512,      BASE_DECIMAL, FORCE_SIGN),    4 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 1024,     BASE_DECIMAL, FORCE_SIGN),    5 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 7777,     BASE_DECIMAL, FORCE_SIGN),    5 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 7777777,  BASE_DECIMAL, FORCE_SIGN),    8 );

    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), 0,        BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -1,       BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -8,       BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -512,     BASE_DECIMAL, FORCE_SIGN),    4 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -1024,    BASE_DECIMAL, FORCE_SIGN),    5 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -7777,    BASE_DECIMAL, FORCE_SIGN),    5 );
    ASSERT2( sizeeq, itoa (NULL, sizeof(dest), -7777777, BASE_DECIMAL, FORCE_SIGN),    8 );


    ASSERT2( sizeeq, uitoa(NULL, 0,            0,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, uitoa(NULL, 0,            1,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, uitoa(NULL, 0,            8,        BASE_DECIMAL),                1 );
    ASSERT2( sizeeq, uitoa(NULL, 0,            512,      BASE_DECIMAL),                3 );
    ASSERT2( sizeeq, uitoa(NULL, 0,            1024,     BASE_DECIMAL),                4 );
    ASSERT2( sizeeq, uitoa(NULL, 0,            7777,     BASE_DECIMAL),                4 );
    ASSERT2( sizeeq, uitoa(NULL, 0,            7777777,  BASE_DECIMAL),                7 );

    ASSERT2( sizeeq, itoa (NULL, 0,            0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, itoa (NULL, 0,            1,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, itoa (NULL, 0,            8,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, itoa (NULL, 0,            512,      BASE_DECIMAL, OPTIONAL_SIGN), 3 );
    ASSERT2( sizeeq, itoa (NULL, 0,            1024,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, itoa (NULL, 0,            7777,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, itoa (NULL, 0,            7777777,  BASE_DECIMAL, OPTIONAL_SIGN), 7 );

    ASSERT2( sizeeq, itoa (NULL, 0,            0,        BASE_DECIMAL, OPTIONAL_SIGN), 1 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -1,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -8,       BASE_DECIMAL, OPTIONAL_SIGN), 2 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -512,     BASE_DECIMAL, OPTIONAL_SIGN), 4 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -1024,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -7777,    BASE_DECIMAL, OPTIONAL_SIGN), 5 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -7777777, BASE_DECIMAL, OPTIONAL_SIGN), 8 );

    ASSERT2( sizeeq, itoa (NULL, 0,            0,        BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, 0,            1,        BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, 0,            8,        BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, 0,            512,      BASE_DECIMAL, FORCE_SIGN),    4 );
    ASSERT2( sizeeq, itoa (NULL, 0,            1024,     BASE_DECIMAL, FORCE_SIGN),    5 );
    ASSERT2( sizeeq, itoa (NULL, 0,            7777,     BASE_DECIMAL, FORCE_SIGN),    5 );
    ASSERT2( sizeeq, itoa (NULL, 0,            7777777,  BASE_DECIMAL, FORCE_SIGN),    8 );

    ASSERT2( sizeeq, itoa (NULL, 0,            0,        BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -1,       BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -8,       BASE_DECIMAL, FORCE_SIGN),    2 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -512,     BASE_DECIMAL, FORCE_SIGN),    4 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -1024,    BASE_DECIMAL, FORCE_SIGN),    5 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -7777,    BASE_DECIMAL, FORCE_SIGN),    5 );
    ASSERT2( sizeeq, itoa (NULL, 0,            -7777777, BASE_DECIMAL, FORCE_SIGN),    8 );


    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0,        BASE_DECIMAL), 1 );
      ASSERT3( nstreq, sizeof(dest), dest,    "0" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 1,        BASE_DECIMAL), 1 );
      ASSERT3( nstreq, sizeof(dest), dest,    "1" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 8,        BASE_DECIMAL), 1 );
      ASSERT3( nstreq, sizeof(dest), dest,    "8" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 512,      BASE_DECIMAL), 3 );
      ASSERT3( nstreq, sizeof(dest), dest,    "512" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 1024,     BASE_DECIMAL), 4 );
      ASSERT3( nstreq, sizeof(dest), dest,    "1024" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 7777,     BASE_DECIMAL), 4 );
      ASSERT3( nstreq, sizeof(dest), dest,    "7777" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 7777777,  BASE_DECIMAL), 7 );
      ASSERT3( nstreq, sizeof(dest), dest,    "7777777" );


    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 0,         BASE_DECIMAL, OPTIONAL_SIGN), 1 );
      ASSERT3( nstreq, sizeof(dest), dest,    "0" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 1,         BASE_DECIMAL, OPTIONAL_SIGN), 1 );
      ASSERT3( nstreq, sizeof(dest), dest,    "1" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 8,         BASE_DECIMAL, OPTIONAL_SIGN), 1 );
      ASSERT3( nstreq, sizeof(dest), dest,    "8" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 512,       BASE_DECIMAL, OPTIONAL_SIGN), 3 );
      ASSERT3( nstreq, sizeof(dest), dest,    "512" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 1024,      BASE_DECIMAL, OPTIONAL_SIGN), 4 );
      ASSERT3( nstreq, sizeof(dest), dest,    "1024" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 7777,      BASE_DECIMAL, OPTIONAL_SIGN), 4 );
      ASSERT3( nstreq, sizeof(dest), dest,    "7777" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 7777777,   BASE_DECIMAL, OPTIONAL_SIGN), 7 );
      ASSERT3( nstreq, sizeof(dest), dest,    "7777777" );

    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 0,         BASE_DECIMAL, OPTIONAL_SIGN), 1 );
      ASSERT3( nstreq, sizeof(dest), dest,    "0" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -1,        BASE_DECIMAL, OPTIONAL_SIGN), 2 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-1" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -8,        BASE_DECIMAL, OPTIONAL_SIGN), 2 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-8" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -512,      BASE_DECIMAL, OPTIONAL_SIGN), 4 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-512" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -1024,     BASE_DECIMAL, OPTIONAL_SIGN), 5 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-1024" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -7777,     BASE_DECIMAL, OPTIONAL_SIGN), 5 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-7777" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -7777777,  BASE_DECIMAL, OPTIONAL_SIGN), 8 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-7777777" );


    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 0,         BASE_DECIMAL, FORCE_SIGN), 2 );
      ASSERT3( nstreq, sizeof(dest), dest,    " 0" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 1,         BASE_DECIMAL, FORCE_SIGN), 2 );
      ASSERT3( nstreq, sizeof(dest), dest,    "+1" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 8,         BASE_DECIMAL, FORCE_SIGN), 2 );
      ASSERT3( nstreq, sizeof(dest), dest,    "+8" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 512,       BASE_DECIMAL, FORCE_SIGN), 4 );
      ASSERT3( nstreq, sizeof(dest), dest,    "+512" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 1024,      BASE_DECIMAL, FORCE_SIGN), 5 );
      ASSERT3( nstreq, sizeof(dest), dest,    "+1024" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 7777,      BASE_DECIMAL, FORCE_SIGN), 5 );
      ASSERT3( nstreq, sizeof(dest), dest,    "+7777" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 7777777,   BASE_DECIMAL, FORCE_SIGN), 8 );
      ASSERT3( nstreq, sizeof(dest), dest,    "+7777777" );

    ASSERT2( sizeeq, itoa(dest, sizeof(dest), 0,         BASE_DECIMAL, FORCE_SIGN), 2 );
      ASSERT3( nstreq, sizeof(dest), dest,    " 0" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -1,        BASE_DECIMAL, FORCE_SIGN), 2 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-1" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -8,        BASE_DECIMAL, FORCE_SIGN), 2 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-8" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -512,      BASE_DECIMAL, FORCE_SIGN), 4 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-512" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -1024,     BASE_DECIMAL, FORCE_SIGN), 5 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-1024" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -7777,     BASE_DECIMAL, FORCE_SIGN), 5 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-7777" );
    ASSERT2( sizeeq, itoa(dest, sizeof(dest), -7777777,  BASE_DECIMAL, FORCE_SIGN), 8 );
      ASSERT3( nstreq, sizeof(dest), dest,    "-7777777" );


    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00,        BASE_HEXADECIMAL),      1 );
      ASSERT3( nstreq, sizeof(dest), dest, "0" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x01,        BASE_HEXADECIMAL),      1 );
      ASSERT3( nstreq, sizeof(dest), dest, "1" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x08,        BASE_HEXADECIMAL),      1 );
      ASSERT3( nstreq, sizeof(dest), dest, "8" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x78,        BASE_HEXADECIMAL),      2 );
      ASSERT3( nstreq, sizeof(dest), dest, "78" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x80,        BASE_HEXADECIMAL),      2 );
      ASSERT3( nstreq, sizeof(dest), dest, "80" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x88,        BASE_HEXADECIMAL),      2 );
      ASSERT3( nstreq, sizeof(dest), dest, "88" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x10F,       BASE_HEXADECIMAL),      3 );
      ASSERT3( nstreq, sizeof(dest), dest, "10F" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00000000U, BASE_HEXADECIMAL),      1 );
      ASSERT3( nstreq, sizeof(dest), dest, "0" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00000001U, BASE_HEXADECIMAL),      1 );
      ASSERT3( nstreq, sizeof(dest), dest, "1" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00000010U, BASE_HEXADECIMAL),      2 );
      ASSERT3( nstreq, sizeof(dest), dest, "10" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00000100U, BASE_HEXADECIMAL),      3 );
      ASSERT3( nstreq, sizeof(dest), dest, "100" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00001000U, BASE_HEXADECIMAL),      4 );
      ASSERT3( nstreq, sizeof(dest), dest, "1000" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00010000U, BASE_HEXADECIMAL),      5 );
      ASSERT3( nstreq, sizeof(dest), dest, "10000" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x00100000U, BASE_HEXADECIMAL),      6 );
      ASSERT3( nstreq, sizeof(dest), dest, "100000" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x01000000U, BASE_HEXADECIMAL),      7 );
      ASSERT3( nstreq, sizeof(dest), dest, "1000000" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x10000000U, BASE_HEXADECIMAL),      8 );
      ASSERT3( nstreq, sizeof(dest), dest, "10000000" );
    ASSERT2( sizeeq, uitoa(dest, sizeof(dest), 0x7777777,   BASE_HEXADECIMAL),      7 );
      ASSERT3( nstreq, sizeof(dest), dest, "7777777" );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy(dest,  love, 5), 4 );
    ASSERT3( streqn, dest, "love",  sizeof(dest) );

    ASSERT2( sizeeq, strlcpy(dest,  love, 4), 3);
    ASSERT3( streqn, dest, "lov",   sizeof(dest) );

    ASSERT2( sizeeq, strlcpy(dest,  love, 1), 0);
    ASSERT3( streqn, dest, "",      sizeof(dest) );


    ASSERT2( sizeeq, strlcpy(dest,  peace, sizeof(dest)), 5 );
    ASSERT2( sizeeq, strlcpy(dest,  love,  0),            0 );
    ASSERT3( streqn, dest, "peace", sizeof(dest) );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    ASSERT3( streqn, smalldest, "peace", sizeof(smalldest) );

    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 4), 4 );
    ASSERT3( streqn, smalldest, "peac",  sizeof(smalldest) );


    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 5, 5), 4 );
    ASSERT3( streqn, smalldest, "peac",  sizeof(smalldest) );

    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 5, 4), 4 );
    ASSERT3( streqn, smalldest, "peac",  sizeof(smalldest) );

    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 5, 3), 3 );
    ASSERT3( streqn, smalldest, "pea",   sizeof(smalldest) );

    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 5, 2), 2 );
    ASSERT3( streqn, smalldest, "pe",    sizeof(smalldest) );


    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 4, 5), 3 );
    ASSERT3( streqn, smalldest, "pea",   sizeof(smalldest) );

    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 4, 4), 3 );
    ASSERT3( streqn, smalldest, "pea",   sizeof(smalldest) );

    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 4, 3), 3 );
    ASSERT3( streqn, smalldest, "pea",   sizeof(smalldest) );

    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 4, 2), 2 );
    ASSERT3( streqn, smalldest, "pe",    sizeof(smalldest) );


    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 0), 0 );
    ASSERT3( streqn, smalldest, "",      sizeof(smalldest) );


    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 8, 5), 5 );
    ASSERT2( sizeeq, strlcpy_srcmax(smalldest, peace, 0, 5), 0 );
    ASSERT3( streqn, smalldest, "peace", sizeof(smalldest) );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    ASSERT3( streqn, dest, "love", sizeof(dest) );


    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 3), 3 );
    ASSERT3( streqn, dest, "lov",  sizeof(dest) );

    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 3, 4), 3 );
    ASSERT3( streqn, dest, "lov",  sizeof(dest) );

    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 3, 3), 3 );
    ASSERT3( streqn, dest, "lov",  sizeof(dest) );


    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 0, 4), 0 );
    ASSERT3( streqn, dest, "",     sizeof(dest) );

    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 0), 0 );
    ASSERT3( streqn, dest, "",     sizeof(dest) );

    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 0, 0), 0 );
    ASSERT3( streqn, dest, "",     sizeof(dest) );

    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 1, 4, 4), 0 );
    ASSERT3( streqn, dest, "",     sizeof(dest) );

    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 8, 4, 4), 4 );
    ASSERT2( sizeeq, strlcpy_with_max(dest, love, 0, 4, 4), 0 );
    ASSERT3( streqn, dest, "love", sizeof(dest) );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy(dest, love, sizeof(dest)), 4 );

    ASSERT2( sizeeq, strnlen(dest, 7), 4 );
    ASSERT2( sizeeq, strnlen(dest, 6), 4 );
    ASSERT2( sizeeq, strnlen(dest, 5), 4 );
    ASSERT2( sizeeq, strnlen(dest, 4), 4 );
    ASSERT2( sizeeq, strnlen(dest, 3), 3 );
    ASSERT2( sizeeq, strnlen(dest, 2), 2 );
    ASSERT2( sizeeq, strnlen(dest, 1), 1 );
    ASSERT2( sizeeq, strnlen(dest, 0), 0 );

    ASSERT2( sizeeq, strllen(dest, 7), 4 );
    ASSERT2( sizeeq, strllen(dest, 6), 4 );
    ASSERT2( sizeeq, strllen(dest, 5), 4 );
    ASSERT2( sizeeq, strllen(dest, 4), 3 );
    ASSERT2( sizeeq, strllen(dest, 3), 2 );
    ASSERT2( sizeeq, strllen(dest, 2), 1 );
    ASSERT2( sizeeq, strllen(dest, 1), 0 );
    ASSERT2( sizeeq, strllen(dest, 0), 0 );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strzlen(dest),    4 );
    ASSERT2( sizeeq, strzlen(love),    4 );
    ASSERT2( sizeeq, strzlen(peace),   5 );
    ASSERT2( sizeeq, strzlen(""),      0 );
    ASSERT2( sizeeq, strzlen("love!"), 5 );
    ASSERT2( sizeeq, strzlen(NULL),    0 );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy(dest, love, sizeof(dest)), 4 );

    ASSERT1( true,  strn_has_null_terminator(dest, sizeof(dest)) );

    ASSERT1( true,  strn_has_null_terminator(dest, 5) );
    ASSERT1( false, strn_has_null_terminator(dest, 4) );
    ASSERT1( false, strn_has_null_terminator(dest, 3) );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 1),  1 );
    ASSERT3( nstreq, sizeof(dest), dest, "l" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 5),  5 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovel" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 8),  8 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 9),  9 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovel" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 12), 12 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovelove" );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 5,            4),  4 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 2,            4),  1 );
    ASSERT3( nstreq, sizeof(dest), dest, "l" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 1,            4),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            4),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, "",   5,            4),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, "",   2,            4),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, "",   1,            4),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, "",   0,            4),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 3,            8),  2 );
    ASSERT3( nstreq, sizeof(dest), dest, "lo" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 5,            8),  4 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            8),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 3,            10), 2 );
    ASSERT3( nstreq, sizeof(dest), dest, "lo" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 5,            10), 4 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 9,            10), 8 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            10), 0 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 1,            1),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 1,            0),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            1),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),  4 );
    ASSERT2( sizeeq, strlcpy_cycle(dest, love, 0,            0),  0 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)),   8  );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)),   8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)),   12 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 13 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  17 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 13 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  3),             16 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelov" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 13 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  17 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  21 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 13 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  17 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  21 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),                  22 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 13 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  17 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  21 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), "!",   0),             21 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 13 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  17 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  21 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), "!",   1),             22 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 13 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  17 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  21 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), "!",   2),             22 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  8  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 13 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  17 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  21 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),                  22 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),                  23 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove!!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),            4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  5  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  9  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  10 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 15 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  16 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  20 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  21 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  25 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),                  26 );
    ASSERT3( nstreq, sizeof(dest), dest, "love love peace love love!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0),            0  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  5  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 10 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  11 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  15 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  16 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  20 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),                  21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),          4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                5  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)), 9  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                10 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)), 14 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                15 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)), 19 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                20 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)), 24 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),                25 );
    ASSERT3( nstreq, sizeof(dest), dest, "love love love love love!" );



    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0),            0  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  5  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 10 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  11 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  15 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  16 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  20 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),                  21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );

    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), "",    sizeof(dest)),  21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), NULL,  sizeof(dest)),  21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  0),             21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );


    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
      );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest, sizeof(dest)),  42  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
        "love peace love love!"
      );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest, sizeof(dest)),  84  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
        "love peace love love!"
        "love peace love love!"
        "love peace love love!"
      );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest, sizeof(dest)),  168 );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
        "love peace love love!"
        "love peace love love!"
        "love peace love love!"

        "love peace love love!"
        "love peace love love!"
        "love peace love love!"
        "love peace love love!"
      );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0),            0  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  5  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 10 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  11 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  15 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),                  16 );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  20 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),                  21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );

    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
      );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest + 1, size_minus(sizeof(dest), 1)),  41  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
         "ove peace love love!"
      );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest + 5, size_minus(sizeof(dest), 5)),  77  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
         "ove peace love love!"

             "peace love love!"
         "ove peace love love!"
      );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)),                     81  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
         "ove peace love love!"

             "peace love love!"
         "ove peace love love!"

         "love"
      );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0),            0  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love,  sizeof(love)),  4  );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, " ",  sizeof(dest), 2),            2  );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), peace, sizeof(peace)), 7  );
    ASSERT3( nstreq, sizeof(dest), dest, "  peace" );

    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest + 2, size_minus(sizeof(dest), 2)), 12 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace" );

    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'), 13  );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!" );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'), 14  );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!!" );

    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest + 2, size_minus(sizeof(dest), 2)), 26 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!!peacepeace!!" );

    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest + 2, size_minus(sizeof(dest), 2)), 50 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!!peacepeace!!peacepeace!!peacepeace!!" );

    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), dest + 2, size_minus(sizeof(dest), 2)), 98 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!!peacepeace!!peacepeace!!peacepeace!!peacepeace!!peacepeace!!peacepeace!!peacepeace!!" );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)), 8 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelove" );
    ASSERT2( sizeeq, strlappendn(dest + 3, size_minus(sizeof(dest), 3), love, sizeof(love)), 12 - 3 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );
    ASSERT2( sizeeq, strlcpy_cycle(dest + 3, love, size_minus(sizeof(dest), 3), 0), 0 );
    ASSERT3( nstreq, sizeof(dest), dest, "lov" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );
    ASSERT2( sizeeq, strlcpy_cycle(dest + 3, love, size_minus(sizeof(dest), 3), 4), 7 - 3 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovlove" );
    ASSERT2( sizeeq, strlappendn(dest, sizeof(dest), love, sizeof(love)), 11 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovlovelove" );
    ASSERT2( sizeeq, strlappendn(dest + 3, size_minus(sizeof(dest), 3), love, sizeof(love)), 15 - 3 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovlovelovelove" );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      8  );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      8  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      12 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      8  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace),     13 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      17 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      8  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace),     13 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      17 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      21 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      8  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace),     13 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      17 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      21 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),       22 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      8  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace),     13 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      17 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      21 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), "!"),       22 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      8  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace),     13 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      17 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      21 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),       22 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),       23 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovepeacelovelove!!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   5  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  9  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   10  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace), 15 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   16 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  20 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   21 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  25 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),   26 );
    ASSERT3( nstreq, sizeof(dest), dest, "love love peace love love!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0), 0  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   5  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace), 10 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   11 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  15 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   16 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  20 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),   21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4), 4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),       5  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      9  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),       10 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      14 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),       15 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      19 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),       20 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      24 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),       25 );
    ASSERT3( nstreq, sizeof(dest), dest, "love love love love love!" );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0), 0  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   5  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace), 10 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   11 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  15 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),   16 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),  20 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),   21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );

    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), ""),   21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), NULL), 21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );


    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
      );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest),  42  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
        "love peace love love!"
      );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest),  84  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
        "love peace love love!"
        "love peace love love!"
        "love peace love love!"
      );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest),  168 );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
        "love peace love love!"
        "love peace love love!"
        "love peace love love!"

        "love peace love love!"
        "love peace love love!"
        "love peace love love!"
        "love peace love love!"
      );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0), 0 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      4  );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),       5  );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace),     10 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),       11 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      15 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), ' '),       16 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      20 );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),       21 );
    ASSERT3( nstreq, sizeof(dest), dest, "love peace love love!" );

    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
      );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest + 1),  41  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
         "ove peace love love!"
      );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest + 5),  77  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
         "ove peace love love!"

             "peace love love!"
         "ove peace love love!"
      );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      81  );
    ASSERT3
      ( nstreq, sizeof(dest), dest
      , "love peace love love!"
         "ove peace love love!"

             "peace love love!"
         "ove peace love love!"

         "love"
      );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 0), 0 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),      4 );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, " ",  sizeof(dest), 2), 2 );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), peace),     7 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peace" );

    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest + 2),  12 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace" );

    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),       13 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!" );
    ASSERT2( sizeeq, strlappendc(dest, sizeof(dest), '!'),       14 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!!" );

    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest + 2),  26 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!!peacepeace!!" );

    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest + 2),  50 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!!peacepeace!!peacepeace!!peacepeace!!" );

    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), dest + 2),  98 );
    ASSERT3( nstreq, sizeof(dest), dest, "  peacepeace!!peacepeace!!peacepeace!!peacepeace!!peacepeace!!peacepeace!!peacepeace!!peacepeace!!" );


    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),                    4  );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),                         8  );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelove" );
    ASSERT2( sizeeq, strlappendz(dest + 3, size_minus(sizeof(dest), 3), love),      12 - 3 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovelovelove" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),                    4  );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );
    ASSERT2( sizeeq, strlcpy_cycle(dest + 3, love, size_minus(sizeof(dest), 3), 0), 0  );
    ASSERT3( nstreq, sizeof(dest), dest, "lov" );

    ASSERT2( sizeeq, strlcpy_cycle(dest, love, sizeof(dest), 4),                    4  );
    ASSERT3( nstreq, sizeof(dest), dest, "love" );
    ASSERT2( sizeeq, strlcpy_cycle(dest + 3, love, size_minus(sizeof(dest), 3), 4), 7 - 3  );
    ASSERT3( nstreq, sizeof(dest), dest, "lovlove" );
    ASSERT2( sizeeq, strlappendz(dest, sizeof(dest), love),                         11 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovlovelove" );
    ASSERT2( sizeeq, strlappendz(dest + 3, size_minus(sizeof(dest), 3), love),      15 - 3 );
    ASSERT3( nstreq, sizeof(dest), dest, "lovlovelovelove" );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, index_bounds(32, 0, 8),  8  );
    ASSERT2( sizeeq, index_bounds(32, 0, 29), 29 );
    ASSERT2( sizeeq, index_bounds(32, 0, 30), 30 );
    ASSERT2( sizeeq, index_bounds(32, 0, 31), 31 );
    ASSERT2( sizeeq, index_bounds(32, 0, 32), 31 );
    ASSERT2( sizeeq, index_bounds(32, 0, 32), 31 );
    ASSERT2( sizeeq, index_bounds(32, 0, 33), 31 );
    ASSERT2( sizeeq, index_bounds(32, 0, 34), 31 );
    ASSERT2( sizeeq, index_bounds(32, 0, 35), 31 );
    ASSERT2( sizeeq, index_bounds(32, 0, 36), 31 );
    ASSERT2( sizeeq, index_bounds(32, 0, 64), 31 );

    ASSERT2( sizeeq, index_bounds(32, 1, 8),  8  );
    ASSERT2( sizeeq, index_bounds(32, 1, 29), 29 );
    ASSERT2( sizeeq, index_bounds(32, 1, 30), 30 );
    ASSERT2( sizeeq, index_bounds(32, 1, 31), 30 );
    ASSERT2( sizeeq, index_bounds(32, 1, 32), 30 );
    ASSERT2( sizeeq, index_bounds(32, 1, 32), 30 );
    ASSERT2( sizeeq, index_bounds(32, 1, 33), 30 );
    ASSERT2( sizeeq, index_bounds(32, 1, 34), 30 );
    ASSERT2( sizeeq, index_bounds(32, 1, 35), 30 );
    ASSERT2( sizeeq, index_bounds(32, 1, 36), 30 );
    ASSERT2( sizeeq, index_bounds(32, 1, 64), 30 );

    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, strlcpy(dest, love, sizeof(dest)), 4 );

    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 8), 4 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 7), 4 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 4), 4 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 3), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 2), 2 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 1), 1 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 0, 0), 0 );

    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 8), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 7), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 4), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 3), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 2), 2 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 1), 1 );
    ASSERT2( sizeeq, strn_index_bounds(dest, sizeof(dest), 1, 0), 0 );


    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 8), 4 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 7), 4 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 4), 4 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 3), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 2), 2 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 1), 1 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 0, 0), 0 );

    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 8), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 7), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 4), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 3), 3 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 2), 2 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 1), 1 );
    ASSERT2( sizeeq, strn_index_bounds(dest, 5, 1, 0), 0 );


    /* ---------------------------------------------------------------- */

    ASSERT2( sizeeq, str_index_bounds(love, 0, 8), 4 );
    ASSERT2( sizeeq, str_index_bounds(love, 0, 7), 4 );
    ASSERT2( sizeeq, str_index_bounds(love, 0, 4), 4 );
    ASSERT2( sizeeq, str_index_bounds(love, 0, 3), 3 );
    ASSERT2( sizeeq, str_index_bounds(love, 0, 2), 2 );
    ASSERT2( sizeeq, str_index_bounds(love, 0, 1), 1 );
    ASSERT2( sizeeq, str_index_bounds(love, 0, 0), 0 );

    ASSERT2( sizeeq, str_index_bounds(love, 1, 8), 3 );
    ASSERT2( sizeeq, str_index_bounds(love, 1, 7), 3 );
    ASSERT2( sizeeq, str_index_bounds(love, 1, 4), 3 );
    ASSERT2( sizeeq, str_index_bounds(love, 1, 3), 3 );
    ASSERT2( sizeeq, str_index_bounds(love, 1, 2), 2 );
    ASSERT2( sizeeq, str_index_bounds(love, 1, 1), 1 );
    ASSERT2( sizeeq, str_index_bounds(love, 1, 0), 0 );
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

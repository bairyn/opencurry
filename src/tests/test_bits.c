/*
 * opencurry: tests/test_bits.c
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
#include "test_bits.h"

#include "../bits.h"

int test_bits_cli(int argc, char **argv)
{
  return run_test_suite(bits_test);
}

/* ---------------------------------------------------------------- */

/* bits tests. */
unit_test_t bits_test =
  {  test_bits_run
  , "test_bits"
  , "bits tests."
  };

/* Array of bits tests. */
unit_test_t *bits_tests[] =
  { &bits_consistency_test
  , &bits_equalities_test

  , NULL
  };

unit_test_result_t test_bits_run(unit_test_context_t *context)
{
  return run_tests(context, bits_tests);
}

/* ---------------------------------------------------------------- */

unit_test_t bits_consistency_test =
  {  bits_consistency_test_run
  , "bits_consistency_test"
  , "Testing consistency between macros and procedures provided by the \"bits\" module."
  };

unit_test_result_t bits_consistency_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    unsigned int val;

    typedef unsigned int  uint;
    typedef unsigned long ulong;

    ASSERT2( uinteq, "on",       (uint) on_uint(),       (uint) ON()  );
    ASSERT2( uinteq, "off",      (uint) off_uint(),      (uint) OFF() );

    ASSERT2( uinteq, "true",     (uint) true_uint(),     (uint) TRUE()  );
    ASSERT2( uinteq, "false",    (uint) false_uint(),    (uint) FALSE() );

    ASSERT2( uinteq, "yes",      (uint) yes_uint(),      (uint) YES()  );
    ASSERT2( uinteq, "no",       (uint) no_uint(),       (uint) NO() );

    ASSERT2( uinteq, "enabled",  (uint) enabled_uint(),  (uint) ENABLED()  );
    ASSERT2( uinteq, "disabled", (uint) disabled_uint(), (uint) DISABLED() );

    ASSERT2( uinteq, "enable",   (uint) enable_uint(),   (uint) ENABLE()  );
    ASSERT2( uinteq, "disable",  (uint) disable_uint(),  (uint) DISABLE() );

    /* ---------------------------------------------------------------- */

    /* 0111 1100 */
    val = 0x7C;
    ASSERT2( uinteq, "get_bit0",   (uint) get_bit_uint (0,    val), (uint) GET_BIT (0,    val));
    ASSERT2( uinteq, "get_bit1",   (uint) get_bit_uint (1,    val), (uint) GET_BIT (1,    val));
    ASSERT2( uinteq, "get_bit2",   (uint) get_bit_uint (2,    val), (uint) GET_BIT (2,    val));
    ASSERT2( uinteq, "get_bit3",   (uint) get_bit_uint (3,    val), (uint) GET_BIT (3,    val));
    ASSERT2( uinteq, "get_bit4",   (uint) get_bit_uint (4,    val), (uint) GET_BIT (4,    val));
    ASSERT2( uinteq, "get_bit5",   (uint) get_bit_uint (5,    val), (uint) GET_BIT (5,    val));
    ASSERT2( uinteq, "get_bit6",   (uint) get_bit_uint (6,    val), (uint) GET_BIT (6,    val));
    ASSERT2( uinteq, "get_bit7",   (uint) get_bit_uint (7,    val), (uint) GET_BIT (7,    val));

    ASSERT2( uinteq, "set_bit0_0", (uint) set_bit_uint (0, 0, val), (uint) SET_BIT (0, 0, val));
    ASSERT2( uinteq, "set_bit1_0", (uint) set_bit_uint (1, 0, val), (uint) SET_BIT (1, 0, val));
    ASSERT2( uinteq, "set_bit2_0", (uint) set_bit_uint (2, 0, val), (uint) SET_BIT (2, 0, val));
    ASSERT2( uinteq, "set_bit3_0", (uint) set_bit_uint (3, 0, val), (uint) SET_BIT (3, 0, val));
    ASSERT2( uinteq, "set_bit4_0", (uint) set_bit_uint (4, 0, val), (uint) SET_BIT (4, 0, val));
    ASSERT2( uinteq, "set_bit5_0", (uint) set_bit_uint (5, 0, val), (uint) SET_BIT (5, 0, val));
    ASSERT2( uinteq, "set_bit6_0", (uint) set_bit_uint (6, 0, val), (uint) SET_BIT (6, 0, val));
    ASSERT2( uinteq, "set_bit7_0", (uint) set_bit_uint (7, 0, val), (uint) SET_BIT (7, 0, val));

    ASSERT2( uinteq, "set_bit0_1", (uint) set_bit_uint (0, 1, val), (uint) SET_BIT (0, 1, val));
    ASSERT2( uinteq, "set_bit1_1", (uint) set_bit_uint (1, 1, val), (uint) SET_BIT (1, 1, val));
    ASSERT2( uinteq, "set_bit2_1", (uint) set_bit_uint (2, 1, val), (uint) SET_BIT (2, 1, val));
    ASSERT2( uinteq, "set_bit3_1", (uint) set_bit_uint (3, 1, val), (uint) SET_BIT (3, 1, val));
    ASSERT2( uinteq, "set_bit4_1", (uint) set_bit_uint (4, 1, val), (uint) SET_BIT (4, 1, val));
    ASSERT2( uinteq, "set_bit5_1", (uint) set_bit_uint (5, 1, val), (uint) SET_BIT (5, 1, val));
    ASSERT2( uinteq, "set_bit6_1", (uint) set_bit_uint (6, 1, val), (uint) SET_BIT (6, 1, val));
    ASSERT2( uinteq, "set_bit7_1", (uint) set_bit_uint (7, 1, val), (uint) SET_BIT (7, 1, val));

    ASSERT2( uinteq, "flip_bit0",  (uint) flip_bit_uint(0,    val), (uint) FLIP_BIT(0,    val));
    ASSERT2( uinteq, "flip_bit1",  (uint) flip_bit_uint(1,    val), (uint) FLIP_BIT(1,    val));
    ASSERT2( uinteq, "flip_bit2",  (uint) flip_bit_uint(2,    val), (uint) FLIP_BIT(2,    val));
    ASSERT2( uinteq, "flip_bit3",  (uint) flip_bit_uint(3,    val), (uint) FLIP_BIT(3,    val));
    ASSERT2( uinteq, "flip_bit4",  (uint) flip_bit_uint(4,    val), (uint) FLIP_BIT(4,    val));
    ASSERT2( uinteq, "flip_bit5",  (uint) flip_bit_uint(5,    val), (uint) FLIP_BIT(5,    val));
    ASSERT2( uinteq, "flip_bit6",  (uint) flip_bit_uint(6,    val), (uint) FLIP_BIT(6,    val));
    ASSERT2( uinteq, "flip_bit7",  (uint) flip_bit_uint(7,    val), (uint) FLIP_BIT(7,    val));

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "starts",   (uint) bit_group_starts_uint   (val), (uint) BIT_GROUP_STARTS   (val));
    ASSERT2( uinteq, "ends",     (uint) bit_group_ends_uint     (val), (uint) BIT_GROUP_ENDS     (val));

    ASSERT2( uinteq, "nostarts", (uint) bit_group_no_starts_uint(val), (uint) BIT_GROUP_NO_STARTS(val));
    ASSERT2( uinteq, "noends",   (uint) bit_group_no_ends_uint  (val), (uint) BIT_GROUP_NO_ENDS  (val));

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "1contagious", (uint) bit_one_contagious_uint (val), (uint) BIT_ONE_CONTAGIOUS (val));
    ASSERT2( uinteq, "0contagious", (uint) bit_zero_contagious_uint(val), (uint) BIT_ZERO_CONTAGIOUS(val));

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "odd",  (uint) bits_is_odd_uint (val), (uint) BITS_IS_ODD (val));
    ASSERT2( uinteq, "even", (uint) bits_is_even_uint(val), (uint) BITS_IS_EVEN(val));

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "nonzero val", (uint) is_nonzero_uint(val), (uint) IS_NONZERO(val));
    ASSERT2( uinteq, "zero val",    (uint) is_zero_uint   (val), (uint) IS_ZERO   (val));

    ASSERT2( uinteq, "nonzero 0",   (uint) is_nonzero_uint(0),   (uint) IS_NONZERO(0));
    ASSERT2( uinteq, "zero 0",      (uint) is_zero_uint   (0),   (uint) IS_ZERO   (0));

    ASSERT2( uinteq, "nonzero 1",   (uint) is_nonzero_uint(1),   (uint) IS_NONZERO(1));
    ASSERT2( uinteq, "zero 1",      (uint) is_zero_uint   (1),   (uint) IS_ZERO   (1));

    ASSERT2( uinteq, "nonzero 2",   (uint) is_nonzero_uint(2),   (uint) IS_NONZERO(2));
    ASSERT2( uinteq, "zero 2",      (uint) is_zero_uint   (2),   (uint) IS_ZERO   (2));

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "predecessor val", (uint) bit_nat_pred_uint(val),  (uint) BIT_NAT_PRED(val));
    ASSERT2( uinteq, "predecessor 2"  , (uint) bit_nat_pred_uint(0x02), (uint) BIT_NAT_PRED(0x02));
    ASSERT2( uinteq, "predecessor 1"  , (uint) bit_nat_pred_uint(0x01), (uint) BIT_NAT_PRED(0x01));
    ASSERT2( uinteq, "predecessor 0"  , (uint) bit_nat_pred_uint(0x00), (uint) BIT_NAT_PRED(0x00));

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "0  repeating 1 bits", (uint) one_bit_repeat_uint(0),  (uint) ONE_BIT_REPEAT(0));
    ASSERT2( uinteq, "1  repeating 1 bits", (uint) one_bit_repeat_uint(1),  (uint) ONE_BIT_REPEAT(1));
    ASSERT2( uinteq, "2  repeating 1 bits", (uint) one_bit_repeat_uint(2),  (uint) ONE_BIT_REPEAT(2));
    ASSERT2( uinteq, "3  repeating 1 bits", (uint) one_bit_repeat_uint(3),  (uint) ONE_BIT_REPEAT(3));
    ASSERT2( uinteq, "4  repeating 1 bits", (uint) one_bit_repeat_uint(4),  (uint) ONE_BIT_REPEAT(4));
    ASSERT2( uinteq, "5  repeating 1 bits", (uint) one_bit_repeat_uint(5),  (uint) ONE_BIT_REPEAT(5));
    ASSERT2( uinteq, "6  repeating 1 bits", (uint) one_bit_repeat_uint(6),  (uint) ONE_BIT_REPEAT(6));
    ASSERT2( uinteq, "7  repeating 1 bits", (uint) one_bit_repeat_uint(7),  (uint) ONE_BIT_REPEAT(7));
    ASSERT2( uinteq, "8  repeating 1 bits", (uint) one_bit_repeat_uint(8),  (uint) ONE_BIT_REPEAT(8));

    ASSERT2( uinteq, "31 repeating 1 bits", (uint) one_bit_repeat_uint(31), (uint) ONE_BIT_REPEAT(31));
    ASSERT2( uinteq, "32 repeating 1 bits", (uint) one_bit_repeat_uint(32), (uint) ONE_BIT_REPEAT(32U));

    /* ---------------------------------------------------------------- */

    ASSERT2( ulongeq, "1  repeating 1 bits", (ulong) one_bit_repeat_ulong(1LU),  (ulong) ONE_BIT_REPEAT(1LU));
    ASSERT2( ulongeq, "2  repeating 1 bits", (ulong) one_bit_repeat_ulong(2LU),  (ulong) ONE_BIT_REPEAT(2LU));
    ASSERT2( ulongeq, "3  repeating 1 bits", (ulong) one_bit_repeat_ulong(3LU),  (ulong) ONE_BIT_REPEAT(3LU));

    ASSERT2( ulongeq, "61 repeating 1 bits", (ulong) one_bit_repeat_ulong(61LU), (ulong) ONE_BIT_REPEAT(61LU));
    ASSERT2( ulongeq, "62 repeating 1 bits", (ulong) one_bit_repeat_ulong(62LU), (ulong) ONE_BIT_REPEAT(62LU));
    ASSERT2( ulongeq, "63 repeating 1 bits", (ulong) one_bit_repeat_ulong(63LU), (ulong) ONE_BIT_REPEAT(63LU));
    ASSERT2( ulongeq, "64 repeating 1 bits", (ulong) one_bit_repeat_ulong(64LU), (ulong) ONE_BIT_REPEAT(64LU));
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t bits_equalities_test =
  {  bits_equalities_test_run
  , "bits_equalities_test"
  , "Equality tests for macros and procedures provided by the \"bits\" module."
  };

unit_test_result_t bits_equalities_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    unsigned int val;

    typedef unsigned int  uint;
    typedef unsigned long ulong;

    ASSERT2( uinteq, "on",       (uint) on_uint(),       (uint) 1 );
    ASSERT2( uinteq, "off",      (uint) off_uint(),      (uint) 0 );

    ASSERT2( uinteq, "true",     (uint) true_uint(),     (uint) 1 );
    ASSERT2( uinteq, "false",    (uint) false_uint(),    (uint) 0 );

    ASSERT2( uinteq, "yes",      (uint) yes_uint(),      (uint) 1 );
    ASSERT2( uinteq, "no",       (uint) no_uint(),       (uint) 0 );

    ASSERT2( uinteq, "enabled",  (uint) enabled_uint(),  (uint) 1 );
    ASSERT2( uinteq, "disabled", (uint) disabled_uint(), (uint) 0 );

    ASSERT2( uinteq, "enable",   (uint) enable_uint(),   (uint) 1 );
    ASSERT2( uinteq, "disable",  (uint) disable_uint(),  (uint) 0 );

    /* ---------------------------------------------------------------- */

    /* 0111 1100 */
    val = 0x7C;
    ASSERT2( uinteq, "get_bit0",   (uint) get_bit_uint (0,    val), (uint) 0);
    ASSERT2( uinteq, "get_bit1",   (uint) get_bit_uint (1,    val), (uint) 0);
    ASSERT2( uinteq, "get_bit2",   (uint) get_bit_uint (2,    val), (uint) 1);
    ASSERT2( uinteq, "get_bit3",   (uint) get_bit_uint (3,    val), (uint) 1);
    ASSERT2( uinteq, "get_bit4",   (uint) get_bit_uint (4,    val), (uint) 1);
    ASSERT2( uinteq, "get_bit5",   (uint) get_bit_uint (5,    val), (uint) 1);
    ASSERT2( uinteq, "get_bit6",   (uint) get_bit_uint (6,    val), (uint) 1);
    ASSERT2( uinteq, "get_bit7",   (uint) get_bit_uint (7,    val), (uint) 0);

    ASSERT2( uinteq, "set_bit0_0", (uint) set_bit_uint (0, 0, val), (uint) 0x7C);
    ASSERT2( uinteq, "set_bit1_0", (uint) set_bit_uint (1, 0, val), (uint) 0x7C);
    ASSERT2( uinteq, "set_bit2_0", (uint) set_bit_uint (2, 0, val), (uint) 0x78);
    ASSERT2( uinteq, "set_bit3_0", (uint) set_bit_uint (3, 0, val), (uint) 0x74);
    ASSERT2( uinteq, "set_bit4_0", (uint) set_bit_uint (4, 0, val), (uint) 0x6C);
    ASSERT2( uinteq, "set_bit5_0", (uint) set_bit_uint (5, 0, val), (uint) 0x5C);
    ASSERT2( uinteq, "set_bit6_0", (uint) set_bit_uint (6, 0, val), (uint) 0x3C);
    ASSERT2( uinteq, "set_bit7_0", (uint) set_bit_uint (7, 0, val), (uint) 0x7C);

    ASSERT2( uinteq, "set_bit0_1", (uint) set_bit_uint (0, 1, val), (uint) 0x7D);
    ASSERT2( uinteq, "set_bit1_1", (uint) set_bit_uint (1, 1, val), (uint) 0x7E);
    ASSERT2( uinteq, "set_bit2_1", (uint) set_bit_uint (2, 1, val), (uint) 0x7C);
    ASSERT2( uinteq, "set_bit3_1", (uint) set_bit_uint (3, 1, val), (uint) 0x7C);
    ASSERT2( uinteq, "set_bit4_1", (uint) set_bit_uint (4, 1, val), (uint) 0x7C);
    ASSERT2( uinteq, "set_bit5_1", (uint) set_bit_uint (5, 1, val), (uint) 0x7C);
    ASSERT2( uinteq, "set_bit6_1", (uint) set_bit_uint (6, 1, val), (uint) 0x7C);
    ASSERT2( uinteq, "set_bit7_1", (uint) set_bit_uint (7, 1, val), (uint) 0xFC);

    ASSERT2( uinteq, "flip_bit0",  (uint) flip_bit_uint(0,    val), (uint) 0x7D);
    ASSERT2( uinteq, "flip_bit1",  (uint) flip_bit_uint(1,    val), (uint) 0x7E);
    ASSERT2( uinteq, "flip_bit2",  (uint) flip_bit_uint(2,    val), (uint) 0x78);
    ASSERT2( uinteq, "flip_bit3",  (uint) flip_bit_uint(3,    val), (uint) 0x74);
    ASSERT2( uinteq, "flip_bit4",  (uint) flip_bit_uint(4,    val), (uint) 0x6C);
    ASSERT2( uinteq, "flip_bit5",  (uint) flip_bit_uint(5,    val), (uint) 0x5C);
    ASSERT2( uinteq, "flip_bit6",  (uint) flip_bit_uint(6,    val), (uint) 0x3C);
    ASSERT2( uinteq, "flip_bit7",  (uint) flip_bit_uint(7,    val), (uint) 0xFC);

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "starts",   (uint) bit_group_starts_uint   (val), (uint) 0x40);
    ASSERT2( uinteq, "ends",     (uint) bit_group_ends_uint     (val), (uint) 0x04);

    ASSERT2( uinteq, "nostarts", (uint) bit_group_no_starts_uint(val), (uint) 0x3C);
    ASSERT2( uinteq, "noends",   (uint) bit_group_no_ends_uint  (val), (uint) 0x78);

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "1contagious", (uint) bit_one_contagious_uint (val), (uint) 0xFE);
    ASSERT2( uinteq, "0contagious", (uint) bit_zero_contagious_uint(val), (uint) 0x38);

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "odd",  (uint) bits_is_odd_uint (val), (uint) 0);
    ASSERT2( uinteq, "even", (uint) bits_is_even_uint(val), (uint) 1);

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "nonzero val", (uint) is_nonzero_uint(val), (uint) 1);
    ASSERT2( uinteq, "zero val",    (uint) is_zero_uint   (val), (uint) 0);

    ASSERT2( uinteq, "nonzero 0",   (uint) is_nonzero_uint(0),   (uint) 0);
    ASSERT2( uinteq, "zero 0",      (uint) is_zero_uint   (0),   (uint) 1);

    ASSERT2( uinteq, "nonzero 1",   (uint) is_nonzero_uint(1),   (uint) 1);
    ASSERT2( uinteq, "zero 1",      (uint) is_zero_uint   (1),   (uint) 0);

    ASSERT2( uinteq, "nonzero 2",   (uint) is_nonzero_uint(2),   (uint) 1);
    ASSERT2( uinteq, "zero 2",      (uint) is_zero_uint   (2),   (uint) 0);

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "predecessor val", (uint) bit_nat_pred_uint(val),  (uint) 0x7B);
    ASSERT2( uinteq, "predecessor 2"  , (uint) bit_nat_pred_uint(0x02), (uint) 0x01);
    ASSERT2( uinteq, "predecessor 1"  , (uint) bit_nat_pred_uint(0x01), (uint) 0x00);
    ASSERT2( uinteq, "predecessor 0"  , (uint) bit_nat_pred_uint(0x00), (uint) 0x00);

    /* ---------------------------------------------------------------- */

    ASSERT2( uinteq, "0  repeating 1 bits", (uint) one_bit_repeat_uint(0),  (uint) 0x00);
    ASSERT2( uinteq, "1  repeating 1 bits", (uint) one_bit_repeat_uint(1),  (uint) 0x01);
    ASSERT2( uinteq, "2  repeating 1 bits", (uint) one_bit_repeat_uint(2),  (uint) 0x03);
    ASSERT2( uinteq, "3  repeating 1 bits", (uint) one_bit_repeat_uint(3),  (uint) 0x07);
    ASSERT2( uinteq, "4  repeating 1 bits", (uint) one_bit_repeat_uint(4),  (uint) 0x0F);
    ASSERT2( uinteq, "5  repeating 1 bits", (uint) one_bit_repeat_uint(5),  (uint) 0x1F);
    ASSERT2( uinteq, "6  repeating 1 bits", (uint) one_bit_repeat_uint(6),  (uint) 0x3F);
    ASSERT2( uinteq, "7  repeating 1 bits", (uint) one_bit_repeat_uint(7),  (uint) 0x7F);
    ASSERT2( uinteq, "8  repeating 1 bits", (uint) one_bit_repeat_uint(8),  (uint) 0xFF);

    ASSERT2( uinteq, "31 repeating 1 bits", (uint) one_bit_repeat_uint(31), (uint) 0x7FFFFFFF);
    ASSERT2( uinteq, "32 repeating 1 bits", (uint) one_bit_repeat_uint(32), (uint) 0xFFFFFFFF);

    /* ---------------------------------------------------------------- */

    ASSERT2( ulongeq, "1  repeating 1 bits", (ulong) one_bit_repeat_ulong(1LU),  (ulong) 0x01LU);
    ASSERT2( ulongeq, "2  repeating 1 bits", (ulong) one_bit_repeat_ulong(2LU),  (ulong) 0x03LU);
    ASSERT2( ulongeq, "3  repeating 1 bits", (ulong) one_bit_repeat_ulong(3LU),  (ulong) 0x07LU);

    ASSERT2( ulongeq, "61 repeating 1 bits", (ulong) one_bit_repeat_ulong(61LU), (ulong) 0x1FFFFFFFFFFFACEFUL | (ulong) 0x0FABFFFF);
    ASSERT2( ulongeq, "62 repeating 1 bits", (ulong) one_bit_repeat_ulong(62LU), (ulong) 0x3FFFFFFFFFFFACEFUL | (ulong) 0x0FABFFFF);
    ASSERT2( ulongeq, "63 repeating 1 bits", (ulong) one_bit_repeat_ulong(63LU), (ulong) 0x7FFFFFFFFFFFACEFUL | (ulong) 0x0FABFFFF);
    ASSERT2( ulongeq, "64 repeating 1 bits", (ulong) one_bit_repeat_ulong(64LU), (ulong) 0xFFFFFFFFFFFFACEFUL | (ulong) 0x0FABFFFF);
  }

  return result;
}

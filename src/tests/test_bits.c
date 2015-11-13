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
  , "Testing consistency between macros and procedures provided by the \"bits\" module.."
  };

unit_test_result_t bits_consistency_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    unsigned int val;

    ASSERT2( inteq, "on",       (int) on_uint(),       (int) ON()  );
    ASSERT2( inteq, "off",      (int) off_uint(),      (int) OFF() );

    ASSERT2( inteq, "true",     (int) true_uint(),     (int) TRUE()  );
    ASSERT2( inteq, "false",    (int) false_uint(),    (int) FALSE() );

    ASSERT2( inteq, "yes",      (int) yes_uint(),      (int) YES()  );
    ASSERT2( inteq, "no",       (int) no_uint(),       (int) NO() );

    ASSERT2( inteq, "enabled",  (int) enabled_uint(),  (int) ENABLED()  );
    ASSERT2( inteq, "disabled", (int) disabled_uint(), (int) DISABLED() );

    ASSERT2( inteq, "enable",   (int) enable_uint(),   (int) ENABLE()  );
    ASSERT2( inteq, "disable",  (int) disable_uint(),  (int) DISABLE() );

    /* ---------------------------------------------------------------- */

    /* 0111 1100 */
    val = 0x7C;
    ASSERT2( inteq, "get_bit0",   (int) get_bit_uint (0,    val), (int) GET_BIT (0,    val));
    ASSERT2( inteq, "get_bit1",   (int) get_bit_uint (1,    val), (int) GET_BIT (1,    val));
    ASSERT2( inteq, "get_bit2",   (int) get_bit_uint (2,    val), (int) GET_BIT (2,    val));
    ASSERT2( inteq, "get_bit3",   (int) get_bit_uint (3,    val), (int) GET_BIT (3,    val));
    ASSERT2( inteq, "get_bit4",   (int) get_bit_uint (4,    val), (int) GET_BIT (4,    val));
    ASSERT2( inteq, "get_bit5",   (int) get_bit_uint (5,    val), (int) GET_BIT (5,    val));
    ASSERT2( inteq, "get_bit6",   (int) get_bit_uint (6,    val), (int) GET_BIT (6,    val));
    ASSERT2( inteq, "get_bit7",   (int) get_bit_uint (7,    val), (int) GET_BIT (7,    val));

    ASSERT2( inteq, "set_bit0_0", (int) set_bit_uint (0, 0, val), (int) SET_BIT (0, 0, val));
    ASSERT2( inteq, "set_bit1_0", (int) set_bit_uint (1, 0, val), (int) SET_BIT (1, 0, val));
    ASSERT2( inteq, "set_bit2_0", (int) set_bit_uint (2, 0, val), (int) SET_BIT (2, 0, val));
    ASSERT2( inteq, "set_bit3_0", (int) set_bit_uint (3, 0, val), (int) SET_BIT (3, 0, val));
    ASSERT2( inteq, "set_bit4_0", (int) set_bit_uint (4, 0, val), (int) SET_BIT (4, 0, val));
    ASSERT2( inteq, "set_bit5_0", (int) set_bit_uint (5, 0, val), (int) SET_BIT (5, 0, val));
    ASSERT2( inteq, "set_bit6_0", (int) set_bit_uint (6, 0, val), (int) SET_BIT (6, 0, val));
    ASSERT2( inteq, "set_bit7_0", (int) set_bit_uint (7, 0, val), (int) SET_BIT (7, 0, val));

    ASSERT2( inteq, "set_bit0_1", (int) set_bit_uint (0, 1, val), (int) SET_BIT (0, 1, val));
    ASSERT2( inteq, "set_bit1_1", (int) set_bit_uint (1, 1, val), (int) SET_BIT (1, 1, val));
    ASSERT2( inteq, "set_bit2_1", (int) set_bit_uint (2, 1, val), (int) SET_BIT (2, 1, val));
    ASSERT2( inteq, "set_bit3_1", (int) set_bit_uint (3, 1, val), (int) SET_BIT (3, 1, val));
    ASSERT2( inteq, "set_bit4_1", (int) set_bit_uint (4, 1, val), (int) SET_BIT (4, 1, val));
    ASSERT2( inteq, "set_bit5_1", (int) set_bit_uint (5, 1, val), (int) SET_BIT (5, 1, val));
    ASSERT2( inteq, "set_bit6_1", (int) set_bit_uint (6, 1, val), (int) SET_BIT (6, 1, val));
    ASSERT2( inteq, "set_bit7_1", (int) set_bit_uint (7, 1, val), (int) SET_BIT (7, 1, val));

    ASSERT2( inteq, "flip_bit0",  (int) flip_bit_uint(0,    val), (int) FLIP_BIT(0,    val));
    ASSERT2( inteq, "flip_bit1",  (int) flip_bit_uint(1,    val), (int) FLIP_BIT(1,    val));
    ASSERT2( inteq, "flip_bit2",  (int) flip_bit_uint(2,    val), (int) FLIP_BIT(2,    val));
    ASSERT2( inteq, "flip_bit3",  (int) flip_bit_uint(3,    val), (int) FLIP_BIT(3,    val));
    ASSERT2( inteq, "flip_bit4",  (int) flip_bit_uint(4,    val), (int) FLIP_BIT(4,    val));
    ASSERT2( inteq, "flip_bit5",  (int) flip_bit_uint(5,    val), (int) FLIP_BIT(5,    val));
    ASSERT2( inteq, "flip_bit6",  (int) flip_bit_uint(6,    val), (int) FLIP_BIT(6,    val));
    ASSERT2( inteq, "flip_bit7",  (int) flip_bit_uint(7,    val), (int) FLIP_BIT(7,    val));

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "starts",   (int) bit_group_starts_uint   (val), (int) BIT_GROUP_STARTS   (val));
    ASSERT2( inteq, "ends",     (int) bit_group_ends_uint     (val), (int) BIT_GROUP_ENDS     (val));

    ASSERT2( inteq, "nostarts", (int) bit_group_no_starts_uint(val), (int) BIT_GROUP_NO_STARTS(val));
    ASSERT2( inteq, "noends",   (int) bit_group_no_ends_uint  (val), (int) BIT_GROUP_NO_ENDS  (val));

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "1contagious", (int) bit_one_contagious_uint (val), (int) BIT_ONE_CONTAGIOUS (val));
    ASSERT2( inteq, "0contagious", (int) bit_zero_contagious_uint(val), (int) BIT_ZERO_CONTAGIOUS(val));

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "odd",  (int) bits_is_odd_uint (val), (int) BITS_IS_ODD (val));
    ASSERT2( inteq, "even", (int) bits_is_even_uint(val), (int) BITS_IS_EVEN(val));

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "nonzero", (int) is_nonzero_uint(val), (int) IS_NONZERO(val));
    ASSERT2( inteq, "zero",    (int) is_zero_uint   (val), (int) IS_ZERO   (val));

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "predecessor val", (int) bit_nat_pred_uint(val),  (int) BIT_NAT_PRED(val));
    ASSERT2( inteq, "predecessor 2"  , (int) bit_nat_pred_uint(0x02), (int) BIT_NAT_PRED(0x02));
    ASSERT2( inteq, "predecessor 1"  , (int) bit_nat_pred_uint(0x01), (int) BIT_NAT_PRED(0x01));
    ASSERT2( inteq, "predecessor 0"  , (int) bit_nat_pred_uint(0x00), (int) BIT_NAT_PRED(0x00));

    /* ---------------------------------------------------------------- */

    /* 0000 0111 */
    ASSERT2( inteq, "0  repeating 1 bits", (int) one_bit_repeat_uint(0),  (int) ONE_BIT_REPEAT(0));
    ASSERT2( inteq, "1  repeating 1 bits", (int) one_bit_repeat_uint(1),  (int) ONE_BIT_REPEAT(1));
    ASSERT2( inteq, "2  repeating 1 bits", (int) one_bit_repeat_uint(2),  (int) ONE_BIT_REPEAT(2));
    ASSERT2( inteq, "3  repeating 1 bits", (int) one_bit_repeat_uint(3),  (int) ONE_BIT_REPEAT(3));
    ASSERT2( inteq, "4  repeating 1 bits", (int) one_bit_repeat_uint(4),  (int) ONE_BIT_REPEAT(4));
    ASSERT2( inteq, "5  repeating 1 bits", (int) one_bit_repeat_uint(5),  (int) ONE_BIT_REPEAT(5));
    ASSERT2( inteq, "6  repeating 1 bits", (int) one_bit_repeat_uint(6),  (int) ONE_BIT_REPEAT(6));
    ASSERT2( inteq, "7  repeating 1 bits", (int) one_bit_repeat_uint(7),  (int) ONE_BIT_REPEAT(7));
    ASSERT2( inteq, "8  repeating 1 bits", (int) one_bit_repeat_uint(8),  (int) ONE_BIT_REPEAT(8));

    ASSERT2( inteq, "31 repeating 1 bits", (int) one_bit_repeat_uint(31), (int) ONE_BIT_REPEAT(31));
    /* Handle overflow. */
    ASSERT2( inteq, "32 repeating 1 bits", (int) ( ((long) ONE_BIT_REPEAT(32)) - ((long) one_bit_repeat_uint(32)) ), (int) 0);
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

    ASSERT2( inteq, "on",       (int) on_uint(),       (int) 1 );
    ASSERT2( inteq, "off",      (int) off_uint(),      (int) 0 );

    ASSERT2( inteq, "true",     (int) true_uint(),     (int) 1 );
    ASSERT2( inteq, "false",    (int) false_uint(),    (int) 0 );

    ASSERT2( inteq, "yes",      (int) yes_uint(),      (int) 1 );
    ASSERT2( inteq, "no",       (int) no_uint(),       (int) 0 );

    ASSERT2( inteq, "enabled",  (int) enabled_uint(),  (int) 1 );
    ASSERT2( inteq, "disabled", (int) disabled_uint(), (int) 0 );

    ASSERT2( inteq, "enable",   (int) enable_uint(),   (int) 1 );
    ASSERT2( inteq, "disable",  (int) disable_uint(),  (int) 0 );

    /* ---------------------------------------------------------------- */

    /* 0111 1100 */
    val = 0x7C;
    ASSERT2( inteq, "get_bit0",   (int) get_bit_uint (0,    val), (int) 0);
    ASSERT2( inteq, "get_bit1",   (int) get_bit_uint (1,    val), (int) 0);
    ASSERT2( inteq, "get_bit2",   (int) get_bit_uint (2,    val), (int) 1);
    ASSERT2( inteq, "get_bit3",   (int) get_bit_uint (3,    val), (int) 1);
    ASSERT2( inteq, "get_bit4",   (int) get_bit_uint (4,    val), (int) 1);
    ASSERT2( inteq, "get_bit5",   (int) get_bit_uint (5,    val), (int) 1);
    ASSERT2( inteq, "get_bit6",   (int) get_bit_uint (6,    val), (int) 1);
    ASSERT2( inteq, "get_bit7",   (int) get_bit_uint (7,    val), (int) 0);

    ASSERT2( inteq, "set_bit0_0", (int) set_bit_uint (0, 0, val), (int) 0x7C);
    ASSERT2( inteq, "set_bit1_0", (int) set_bit_uint (1, 0, val), (int) 0x7C);
    ASSERT2( inteq, "set_bit2_0", (int) set_bit_uint (2, 0, val), (int) 0x78);
    ASSERT2( inteq, "set_bit3_0", (int) set_bit_uint (3, 0, val), (int) 0x74);
    ASSERT2( inteq, "set_bit4_0", (int) set_bit_uint (4, 0, val), (int) 0x6C);
    ASSERT2( inteq, "set_bit5_0", (int) set_bit_uint (5, 0, val), (int) 0x5C);
    ASSERT2( inteq, "set_bit6_0", (int) set_bit_uint (6, 0, val), (int) 0x3C);
    ASSERT2( inteq, "set_bit7_0", (int) set_bit_uint (7, 0, val), (int) 0x7C);

    ASSERT2( inteq, "set_bit0_1", (int) set_bit_uint (0, 1, val), (int) 0x7D);
    ASSERT2( inteq, "set_bit1_1", (int) set_bit_uint (1, 1, val), (int) 0x7E);
    ASSERT2( inteq, "set_bit2_1", (int) set_bit_uint (2, 1, val), (int) 0x7C);
    ASSERT2( inteq, "set_bit3_1", (int) set_bit_uint (3, 1, val), (int) 0x7C);
    ASSERT2( inteq, "set_bit4_1", (int) set_bit_uint (4, 1, val), (int) 0x7C);
    ASSERT2( inteq, "set_bit5_1", (int) set_bit_uint (5, 1, val), (int) 0x7C);
    ASSERT2( inteq, "set_bit6_1", (int) set_bit_uint (6, 1, val), (int) 0x7C);
    ASSERT2( inteq, "set_bit7_1", (int) set_bit_uint (7, 1, val), (int) 0xFC);

    ASSERT2( inteq, "flip_bit0",  (int) flip_bit_uint(0,    val), (int) 0x7D);
    ASSERT2( inteq, "flip_bit1",  (int) flip_bit_uint(1,    val), (int) 0x7E);
    ASSERT2( inteq, "flip_bit2",  (int) flip_bit_uint(2,    val), (int) 0x78);
    ASSERT2( inteq, "flip_bit3",  (int) flip_bit_uint(3,    val), (int) 0x74);
    ASSERT2( inteq, "flip_bit4",  (int) flip_bit_uint(4,    val), (int) 0x6C);
    ASSERT2( inteq, "flip_bit5",  (int) flip_bit_uint(5,    val), (int) 0x5C);
    ASSERT2( inteq, "flip_bit6",  (int) flip_bit_uint(6,    val), (int) 0x3C);
    ASSERT2( inteq, "flip_bit7",  (int) flip_bit_uint(7,    val), (int) 0xFC);

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "starts",   (int) bit_group_starts_uint   (val), (int) 0x40);
    ASSERT2( inteq, "ends",     (int) bit_group_ends_uint     (val), (int) 0x04);

    ASSERT2( inteq, "nostarts", (int) bit_group_no_starts_uint(val), (int) 0x3C);
    ASSERT2( inteq, "noends",   (int) bit_group_no_ends_uint  (val), (int) 0x78);

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "1contagious", (int) bit_one_contagious_uint (val), (int) 0xFE);
    ASSERT2( inteq, "0contagious", (int) bit_zero_contagious_uint(val), (int) 0x38);

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "odd",  (int) bits_is_odd_uint (val), (int) 0);
    ASSERT2( inteq, "even", (int) bits_is_even_uint(val), (int) 1);

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "nonzero", (int) is_nonzero_uint(val), (int) 1);
    ASSERT2( inteq, "zero",    (int) is_zero_uint   (val), (int) 0);

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, "predecessor val", (int) bit_nat_pred_uint(val),  (int) 0x7B);
    ASSERT2( inteq, "predecessor 2"  , (int) bit_nat_pred_uint(0x02), (int) 0x01);
    ASSERT2( inteq, "predecessor 1"  , (int) bit_nat_pred_uint(0x01), (int) 0x00);
    ASSERT2( inteq, "predecessor 0"  , (int) bit_nat_pred_uint(0x00), (int) 0x00);

    /* ---------------------------------------------------------------- */

    /* 0000 0111 */
    ASSERT2( inteq, "0  repeating 1 bits", (int) one_bit_repeat_uint(0),  (int) 0x00);
    ASSERT2( inteq, "1  repeating 1 bits", (int) one_bit_repeat_uint(1),  (int) 0x01);
    ASSERT2( inteq, "2  repeating 1 bits", (int) one_bit_repeat_uint(2),  (int) 0x03);
    ASSERT2( inteq, "3  repeating 1 bits", (int) one_bit_repeat_uint(3),  (int) 0x07);
    ASSERT2( inteq, "4  repeating 1 bits", (int) one_bit_repeat_uint(4),  (int) 0x0F);
    ASSERT2( inteq, "5  repeating 1 bits", (int) one_bit_repeat_uint(5),  (int) 0x1F);
    ASSERT2( inteq, "6  repeating 1 bits", (int) one_bit_repeat_uint(6),  (int) 0x3F);
    ASSERT2( inteq, "7  repeating 1 bits", (int) one_bit_repeat_uint(7),  (int) 0x7F);
    ASSERT2( inteq, "8  repeating 1 bits", (int) one_bit_repeat_uint(8),  (int) 0xFF);

    ASSERT2( inteq, "31 repeating 1 bits", (int) one_bit_repeat_uint(31), (int) 0x7FFFFFFF);
    /* Handle overflow. */
    ASSERT2( inteq, "32 repeating 1 bits", (int) ( ((long) 0xFFFFFFFF) - ((long) one_bit_repeat_uint(32)) ), (int) 0);
  }

  return result;
}

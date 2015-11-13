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
    ASSERT2( inteq, "get_bit0", (int) get_bit_uint(0, val), (int) 0);
    ASSERT2( inteq, "get_bit1", (int) get_bit_uint(1, val), (int) 0);
    ASSERT2( inteq, "get_bit2", (int) get_bit_uint(2, val), (int) 1);
    ASSERT2( inteq, "get_bit3", (int) get_bit_uint(3, val), (int) 1);
    ASSERT2( inteq, "get_bit4", (int) get_bit_uint(4, val), (int) 1);
    ASSERT2( inteq, "get_bit5", (int) get_bit_uint(5, val), (int) 1);
    ASSERT2( inteq, "get_bit6", (int) get_bit_uint(6, val), (int) 1);
    ASSERT2( inteq, "get_bit7", (int) get_bit_uint(7, val), (int) 0);
  }

  return result;
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
  }

  return result;
}

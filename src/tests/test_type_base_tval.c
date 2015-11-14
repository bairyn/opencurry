/*
 * opencurry: tests/test_type_base_tval.c
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
#include "test_type_base_tval.h"

#include "../type_base_tval.h"
#include "../type_base_type.h"
#ifdef TODO
#include "../type_base_memory_manager.h"
#endif /* #ifdef TODO */
#ifndef TODO
#include "../type_base.h"
#endif /* #ifndef TODO */

#error TODO

int test_type_base_tval_cli(int argc, char **argv)
{
  return run_test_suite(type_base_tval_test);
}

/* ---------------------------------------------------------------- */

/* type_base_tval tests. */
unit_test_t type_base_tval_test =
  {  test_type_base_tval_run
  , "test_type_base_tval"
  , "type_base_tval tests."
  };

/* Array of type_base_tval tests. */
unit_test_t *type_base_tval_tests[] =
  { &tval_typeof_variant_equalities_test
  , &tval_typeof_equivalences_test

  , NULL
  };

unit_test_result_t test_type_base_tval_run(unit_test_context_t *context)
{
  return run_tests(context, type_base_tval_tests);
}

/* ---------------------------------------------------------------- */

unit_test_t tval_typeof_variant_equalities_test =
  {  tval_typeof_variant_equalities_test_run
  , "tval_typeof_variant_equalities_test"
  , "typeof() calls should return expected values."
  };

unit_test_result_t tval_typeof_variant_equalities_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
#ifdef TODO
#error "TODO: write test!"
#endif /* #ifdef TODO */
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t tval_typeof_equivalences_test =
  {  tval_typeof_equivalences_test_run
  , "tval_typeof_equivalences_test"
  , "typeof() calls should return expected values."
  };

unit_test_result_t tval_typeof_equivalences_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    const type_t           *type_type_def = type_type();
    const memory_manager_t *manager       = &malloc_manager;

    /* ---------------------------------------------------------------- */
    /* ---------------------------------------------------------------- */

    MASSERT1
      ( true,  "typeof(type_type())     equivalent to   type_type()"
      , is_type_equivalent(typeof(type_type_def), type_type())
      );

    MASSERT1
      ( true,  "typeof(&malloc_manager) equivalent to   memory_manager_type()"
      , is_type_equivalent(typeof(manager),       memory_manager_type())
      );

    /* ---------------------------------------------------------------- */

    MASSERT1
      ( false, "typeof(type_type())     different from memory_manager_type()"
      , is_type_equivalent(typeof(type_type_def), memory_manager_type())
      );

    MASSERT1
      ( false, "typeof(&malloc_manager) different from type_type()"
      , is_type_equivalent(typeof(manager),       type_type())
      );

    /* ---------------------------------------------------------------- */
    /* ---------------------------------------------------------------- */

    MASSERT1
      ( true,  "typeof(typeof(type_type()))     equivalent to   type_type()"
      , is_type_equivalent(typeof(typeof(type_type_def)), type_type())
      );

    MASSERT1
      ( true,  "typeof(typeof(&malloc_manager)) equivalent to   type_type()"
      , is_type_equivalent(typeof(typeof(manager)),       type_type())
      );

    /* ---------------------------------------------------------------- */

    MASSERT1
      ( true,  "typeof(typeof(type_type()))     different from memory_manager_type()"
      , is_type_equivalent(typeof(typeof(type_type_def)), memory_manager_type())
      );

    MASSERT1
      ( true,  "typeof(typeof(&malloc_manager)) different from memory_manager_type()"
      , is_type_equivalent(typeof(typeof(manager)),       memory_manager_type())
      );
  }

  return result;
}

/*
 * opencurry: tests/test_type_base_prim.c
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
 *   - size_t
 */
#include <stddef.h>

#include "../base.h"
#include "testing.h"
#include "test_type_base_prim.h"

#include "../type_base_prim.h"
#ifndef TODO
#include "../type_base_type.h"
#endif /* #ifndef TODO */

int test_type_base_prim_cli(int argc, char **argv)
{
  return run_test_suite(type_base_prim_test);
}

/* ---------------------------------------------------------------- */

/* type_base_prim tests. */
unit_test_t type_base_prim_test =
  {  test_type_base_prim_run
  , "test_type_base_prim"
  , "type_base_prim tests."
  };

/* Array of type_base_prim tests. */
unit_test_t *type_base_prim_tests[] =
  { &prim_type_size_correspondence_test

  , NULL
  };

unit_test_result_t test_type_base_prim_run(unit_test_context_t *context)
{
  return run_tests(context, type_base_prim_tests);
}

/* ---------------------------------------------------------------- */

unit_test_t prim_type_size_correspondence_test =
  {  prim_type_size_correspondence_test_run
  , "prim_type_size_correspondence_test"
  , "Testing sizeof equivalence for type_t and its defining struct type."
  };

unit_test_result_t prim_type_size_correspondence_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    TASSERT2( sizeeq, "type_t sizeof equivalence", sizeof(type_t), sizeof(struct type_s));
  }

  return result;
}

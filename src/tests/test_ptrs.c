/*
 * opencurry: tests/test_ptrs.c
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
 *   - ptrdiff_t
 *   - NULL
 */
#include <stddef.h>

#include "../base.h"
#include "testing.h"
#include "test_ptrs.h"

#include "../ptrs.h"

int test_ptrs_cli(int argc, char **argv)
{
  return run_test_suite(ptrs_test);
}

/* TODO: use sizeof to test large values, and to test size equality! */
/* TODO: size equality should be checked somehow in "ptrs" anyway, I think. */

/* ---------------------------------------------------------------- */

/* ptrs tests. */
unit_test_t ptrs_test =
  {  test_ptrs_run
  , "test_ptrs"
  , "ptrs tests."
  };

/* Array of ptrs tests. */
unit_test_t *ptrs_tests[] =
  { &ptrs_identities_test

  , NULL
  };

unit_test_result_t test_ptrs_run(unit_test_context_t *context)
{
  return run_tests(context, ptrs_tests);
}

/* ---------------------------------------------------------------- */

typedef int (*add3_t)(int a);
static int add3(int a)
{
  return a + 3;
}

static const int num42 = 42;

/* ---------------------------------------------------------------- */

unit_test_t ptrs_identities_test =
  {  ptrs_identities_test_run
  , "ptrs_identities_test"
  , "Casting from one type to another should be identity when the sizes are equal."
  };

unit_test_result_t ptrs_identities_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    void        *objp = NULL;
    funp_cast_t  funp = NULL;
    size_t       size = 0;
    ptrdiff_t    diff = 0;

    /* ---------------------------------------------------------------- */
    /* funp <-> objp                                                    */
    /* ---------------------------------------------------------------- */

    funp = (funp_cast_t) add3;
    objp = funp_to_objp(funp);
    funp = objp_to_funp(objp);

    MASSERT2
      ( funpeq, "funp_to_objp -> objp_to_funp = 1"
      , (tests_funp_t) funp, (tests_funp_t) &add3
      );

    MASSERT2
      ( inteq,  "funp_to_objp -> objp_to_funp validity"
      , ((add3_t) funp)(7), 10
      );

    /* ---------------------------------------------------------------- */

    objp = (void *) &num42;
    funp = objp_to_funp(objp);
    objp = funp_to_objp(funp);

    MASSERT2
      ( objpeq, "objp_to_funp -> funp_to_objp = 1"
      , (const void *) objp, (const void *) &num42
      );

    MASSERT2
      ( inteq,  "objp_to_funp -> funp_to_objp validity"
      , *((const int *) objp), 42
      );

    /* ---------------------------------------------------------------- */
    /* objp <-> size                                                    */
    /* ---------------------------------------------------------------- */

    objp = (void *) &num42;
    size = objp_to_size(objp);
    objp = size_to_objp(size);

    MASSERT2
      ( objpeq, "objp_to_size -> size_to_objp = 1"
      , (const void *) objp, (const void *) &num42
      );

    MASSERT2
      ( inteq,  "objp_to_size -> size_to_objp validity"
      , *((const int *) objp), 42
      );

    /* ---------------------------------------------------------------- */

    size = 0;
    objp = size_to_objp(size);
    size = objp_to_size(objp);

    MASSERT2
      ( inteq,  "size_to_objp -> objp_to_size = 1"
      , (int) size, 0
      );

    /* ---------------------------------------------------------------- */
    /* objp <-> ptrdiff                                                 */
    /* ---------------------------------------------------------------- */

    objp = (void *) &num42;
    diff = objp_to_ptrdiff(objp);
    objp = ptrdiff_to_objp(diff);

    MASSERT2
      ( objpeq, "objp_to_ptrdiff -> ptrdiff_to_objp = 1"
      , (const void *) objp, (const void *) &num42
      );

    MASSERT2
      ( inteq,  "objp_to_ptrdiff -> ptrdiff_to_objp validity"
      , *((const int *) objp), 42
      );

    /* ---------------------------------------------------------------- */

    diff = 0;
    objp = ptrdiff_to_objp(diff);
    diff = objp_to_ptrdiff(objp);

    MASSERT2
      ( inteq,  "ptrdiff_to_objp -> objp_to_ptrdiff = 1"
      , (int) diff, 0
      );
  }

  return result;
}

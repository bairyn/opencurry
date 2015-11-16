/*
 * opencurry: tests/test_type_base_compare.c
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
#include "test_type_base_compare.h"

#include "../type_base_compare.h"

#include "../bits.h"

#include "../util.h"

int test_type_base_compare_cli(int argc, char **argv)
{
  return run_test_suite(type_base_compare_test);
}

/* ---------------------------------------------------------------- */

/* type_base_compare tests. */
unit_test_t type_base_compare_test =
  {  test_type_base_compare_run
  , "test_type_base_compare"
  , "type_base_compare tests."
  };

/* Array of type_base_compare tests. */
unit_test_t *type_base_compare_tests[] =
  { &ordering_equalities_test
  , &comparers_test

  , NULL
  };

unit_test_result_t test_type_base_compare_run(unit_test_context_t *context)
{
  return run_tests(context, type_base_compare_tests);
}

/* ---------------------------------------------------------------- */

unit_test_t ordering_equalities_test =
  {  ordering_equalities_test_run
  , "ordering_equalities_test"
  , "ordering functions return expected values."
  };

#define IS_SUCCESS(ordering) checked_is_ordering_success(context, &result, ordering)
#define IS_ERROR(ordering)   checked_is_ordering_error  (context, &result, ordering)
#define IS_LOSSY(ordering)   checked_is_ordering_lossy  (context, &result, ordering)
#define SUCCESS(ordering)    checked_ordering_success   (context, &result, ordering)
#define INVERT(ordering)     checked_ordering_invert    (context, &result, ordering)
#define ORDERING(ordering)   checked_ordering_relation  (context, &result, ordering)

static int checked_is_ordering_success(unit_test_context_t *context, unit_test_result_t *out_result, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    ASSERT2( inteq, (call = is_ordering_success(ordering)), IS_ORDERING_SUCCESS(ordering) );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static int checked_is_ordering_error(unit_test_context_t *context, unit_test_result_t *out_result, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    ASSERT2( inteq, (call = is_ordering_error(ordering)), IS_ORDERING_ERROR(ordering) );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static int checked_is_ordering_lossy(unit_test_context_t *context, unit_test_result_t *out_result, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    ASSERT2( inteq, (call = is_ordering_lossy(ordering)), IS_ORDERING_LOSSY(ordering) );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static int checked_ordering_success(unit_test_context_t *context, unit_test_result_t *out_result, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    ASSERT2( inteq, (call = ordering_success(ordering)), ORDERING_SUCCESS(ordering) );
    ASSERT2( inteq, IS_SUCCESS(call),                    1 );
    ASSERT2( inteq, IS_ERROR  (call),                    0 );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static int checked_ordering_invert(unit_test_context_t *context, unit_test_result_t *out_result, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    ASSERT2( inteq, (call = ordering_invert(ordering)), ORDERING_INVERT(ordering) );
    if (!IS_LOSSY(ordering))
      ASSERT2( inteq, ordering_invert(ordering_invert(ordering)), ordering );
    ASSERT2( inteq, IS_SUCCESS(call),                   IS_SUCCESS     (ordering) );
    ASSERT2( inteq, IS_ERROR  (call),                   IS_ERROR       (ordering) );
    ASSERT2( inteq, IS_LOSSY  (call),                   IS_LOSSY       (ordering) );

    if (IS_SUCCESS(ordering) && !IS_LOSSY(ordering))
    {
      ASSERT2( inteq, call, -ordering);
    }
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static order_relation_t checked_ordering_relation(unit_test_context_t *context, unit_test_result_t *out_result, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    ASSERT2( inteq, (call = ordering_relation(ordering)), ORDERING_RELATION(ordering) );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

unit_test_result_t ordering_equalities_test(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    ASSERT2( inteq, ordering_err_1(),      ORDERING_ERR_1 );
    ASSERT2( inteq, ordering_err_2(),      ORDERING_ERR_2 );
    ASSERT2( inteq, ordering_lossy_lt(),   ORDERING_LOSSY_LT );
    ASSERT2( inteq, ordering_lossy_gt_1(), ORDERING_LOSSY_GT_1 );
    ASSERT2( inteq, ordering_lossy_gt_2(), ORDERING_LOSSY_GT_2 );
    ASSERT2( inteq, ordering_lossy_gt_3(), ORDERING_LOSSY_GT_3 );

    ASSERT2( inteq, IS_SUCCESS(0),                     1 );
    ASSERT2( inteq, IS_SUCCESS(1),                     1 );
    ASSERT2( inteq, IS_SUCCESS(-1),                    1 );
    ASSERT2( inteq, IS_SUCCESS(8),                     1 );
    ASSERT2( inteq, IS_SUCCESS(-16),                   1 );
    ASSERT2( inteq, IS_SUCCESS(ordering_err_1()),      0 );
    ASSERT2( inteq, IS_SUCCESS(ordering_err_2()),      0 );
    ASSERT2( inteq, IS_SUCCESS(ordering_lossy_lt()),   1 );
    ASSERT2( inteq, IS_SUCCESS(ordering_lossy_gt_1()), 1 );
    ASSERT2( inteq, IS_SUCCESS(ordering_lossy_gt_2()), 1 );
    ASSERT2( inteq, IS_SUCCESS(ordering_lossy_gt_3()), 1 );

    ASSERT2( inteq, IS_ERROR(0),                       0 );
    ASSERT2( inteq, IS_ERROR(1),                       0 );
    ASSERT2( inteq, IS_ERROR(-1),                      0 );
    ASSERT2( inteq, IS_ERROR(8),                       0 );
    ASSERT2( inteq, IS_ERROR(-16),                     0 );
    ASSERT2( inteq, IS_ERROR(ordering_err_1()),        1 );
    ASSERT2( inteq, IS_ERROR(ordering_err_2()),        1 );
    ASSERT2( inteq, IS_ERROR(ordering_lossy_lt()),     0 );
    ASSERT2( inteq, IS_ERROR(ordering_lossy_gt_1()),   0 );
    ASSERT2( inteq, IS_ERROR(ordering_lossy_gt_2()),   0 );
    ASSERT2( inteq, IS_ERROR(ordering_lossy_gt_3()),   0 );

    ASSERT2( inteq, IS_LOSSY(0),                       0 );
    ASSERT2( inteq, IS_LOSSY(1),                       0 );
    ASSERT2( inteq, IS_LOSSY(-1),                      0 );
    ASSERT2( inteq, IS_LOSSY(8),                       0 );
    ASSERT2( inteq, IS_LOSSY(-16),                     0 );
    ASSERT2( inteq, IS_LOSSY(ordering_err_1()),        0 );
    ASSERT2( inteq, IS_LOSSY(ordering_err_2()),        0 );
    ASSERT2( inteq, IS_LOSSY(ordering_lossy_lt()),     1 );
    ASSERT2( inteq, IS_LOSSY(ordering_lossy_gt_1()),   1 );
    ASSERT2( inteq, IS_LOSSY(ordering_lossy_gt_2()),   1 );
    ASSERT2( inteq, IS_LOSSY(ordering_lossy_gt_3()),   1 );

    ASSERT2( inteq, SUCCESS(0),                         0 );
    ASSERT2( inteq, SUCCESS(1),                         1 );
    ASSERT2( inteq, SUCCESS(-1),                       -1 );
    ASSERT2( inteq, SUCCESS(8),                         8 );
    ASSERT2( inteq, SUCCESS(-16),                      -16 );
    ASSERT2( inteq, SUCCESS(ordering_err_1()),          ordering_lossy_lt()   );
    ASSERT2( inteq, SUCCESS(ordering_err_2()),          ordering_lossy_lt()   );
    ASSERT2( inteq, SUCCESS(ordering_lossy_lt()),       ordering_lossy_lt()   );
    ASSERT2( inteq, SUCCESS(ordering_lossy_gt_1()),     ordering_lossy_gt_1() );
    ASSERT2( inteq, SUCCESS(ordering_lossy_gt_2()),     ordering_lossy_gt_2() );
    ASSERT2( inteq, SUCCESS(ordering_lossy_gt_3()),     ordering_lossy_gt_3() );

    ASSERT2( inteq, ordering_lt(), ORDERING_LT );
    ASSERT2( inteq, ordering_eq(), ORDERING_EQ );
    ASSERT2( inteq, ordering_gt(), ORDERING_GT );

    ASSERT2( inteq, sign_int(ordering_lt()), -1 );
    ASSERT2( inteq, sign_int(ordering_eq()),  0 );
    ASSERT2( inteq, sign_int(ordering_gt()),  1 );

    ASSERT2( inteq, INVERT(0),                       0 );
    ASSERT2( inteq, INVERT(1),                       1 );
    ASSERT2( inteq, INVERT(-1),                     -1 );
    ASSERT2( inteq, INVERT(8),                       8 );
    ASSERT2( inteq, INVERT(-16),                    -16 );
    ASSERT2( inteq, INVERT(ordering_err_1()),        ordering_err_1() );
    ASSERT2( inteq, INVERT(ordering_err_2()),        ordering_err_2() );
    ASSERT2( inteq, INVERT(ordering_lossy_lt()),     ordering_lossy_gt_1() );
    ASSERT2( inteq, INVERT(ordering_lossy_gt_1()),   ordering_lossy_lt() );
    ASSERT2( inteq, INVERT(ordering_lossy_gt_2()),   ordering_lossy_lt() );
    ASSERT2( inteq, INVERT(ordering_lossy_gt_3()),   ordering_lossy_lt() );

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, ORDERING(0),                     ordering_rel_eq );
    ASSERT2( inteq, ORDERING(1),                     ordering_rel_gt );
    ASSERT2( inteq, ORDERING(-1),                    ordering_rel_lt );
    ASSERT2( inteq, ORDERING(8),                     ordering_rel_gt );
    ASSERT2( inteq, ORDERING(-16),                   ordering_rel_lt );
    ASSERT2( inteq, ORDERING(ordering_err_1()),      ordering_rel_err_1 );
    ASSERT2( inteq, ORDERING(ordering_err_2()),      ordering_rel_err_2 );
    ASSERT2( inteq, ORDERING(ordering_lossy_lt()),   ordering_rel_lt );

    ASSERT2( inteq, ordering_rel_lt,                 SIGN(ordering_rel_lt) );
    ASSERT2( inteq, ordering_rel_eq,                 SIGN(ordering_rel_eq) );
    ASSERT2( inteq, ordering_rel_gt,                 SIGN(ordering_rel_gt) );

    ASSERT2( true, ordering_rel_lt <  0 );
    ASSERT2( true, ordering_rel_eq == 0 );
    ASSERT2( true, ordering_rel_gt >  0 );

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, SUCCESFULL_CMP(-7, -7), ordering_rel_eq );
    ASSERT2( inteq, SUCCESFULL_CMP(-7, -2), ordering_rel_lt );
    ASSERT2( inteq, SUCCESFULL_CMP(-7,  0), ordering_rel_lt );
    ASSERT2( inteq, SUCCESFULL_CMP(-7,  2), ordering_rel_lt );
    ASSERT2( inteq, SUCCESFULL_CMP(-7,  7), ordering_rel_lt );

    ASSERT2( inteq, SUCCESFULL_CMP(-2, -7), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP(-2, -2), ordering_rel_eq );
    ASSERT2( inteq, SUCCESFULL_CMP(-2,  0), ordering_rel_lt );
    ASSERT2( inteq, SUCCESFULL_CMP(-2,  2), ordering_rel_lt );
    ASSERT2( inteq, SUCCESFULL_CMP(-2,  7), ordering_rel_lt );

    ASSERT2( inteq, SUCCESFULL_CMP( 0, -7), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 0, -2), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 0,  0), ordering_rel_eq );
    ASSERT2( inteq, SUCCESFULL_CMP( 0,  2), ordering_rel_lt );
    ASSERT2( inteq, SUCCESFULL_CMP( 0,  7), ordering_rel_lt );

    ASSERT2( inteq, SUCCESFULL_CMP( 2, -7), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 2, -2), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 2,  0), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 2,  2), ordering_rel_eq );
    ASSERT2( inteq, SUCCESFULL_CMP( 2,  7), ordering_rel_lt );

    ASSERT2( inteq, SUCCESFULL_CMP( 7, -7), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 7, -2), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 7,  0), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 7,  2), ordering_rel_gt );
    ASSERT2( inteq, SUCCESFULL_CMP( 7,  7), ordering_rel_eq );

    ASSERT2( inteq, SUCCESFULL_CMP(ordering_err_1(),      0), SIGN(ordering_err_1())      );
    ASSERT2( inteq, SUCCESFULL_CMP(ordering_err_2(),      0), SIGN(ordering_err_2())      );
    ASSERT2( inteq, SUCCESFULL_CMP(ordering_lossy_lt(),   0), SIGN(ordering_lossy_lt())   );
    ASSERT2( inteq, SUCCESFULL_CMP(ordering_lossy_gt_1(), 0), SIGN(ordering_lossy_gt_1()) );
    ASSERT2( inteq, SUCCESFULL_CMP(ordering_lossy_gt_2(), 0), SIGN(ordering_lossy_gt_2()) );
    ASSERT2( inteq, SUCCESFULL_CMP(ordering_lossy_gt_3(), 0), SIGN(ordering_lossy_gt_3()) );

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-7, -7)), ordering_rel_eq );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-7, -2)), ordering_rel_lt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-7,  0)), ordering_rel_lt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-7,  2)), ordering_rel_lt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-7,  7)), ordering_rel_lt );

    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-2, -7)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-2, -2)), ordering_rel_eq );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-2,  0)), ordering_rel_lt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-2,  2)), ordering_rel_lt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-2,  7)), ordering_rel_lt );

    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 0, -7)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 0, -2)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 0,  0)), ordering_rel_eq );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 0,  2)), ordering_rel_lt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 0,  7)), ordering_rel_lt );

    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 2, -7)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 2, -2)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 2,  0)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 2,  2)), ordering_rel_eq );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 2,  7)), ordering_rel_lt );

    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 7, -7)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 7, -2)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 7,  0)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 7,  2)), ordering_rel_gt );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP( 7,  7)), ordering_rel_eq );

    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_err_1(),      0)), SIGN(ordering_err_1())      );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_err_2(),      0)), SIGN(ordering_err_2())      );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_lossy_lt(),   0)), SIGN(ordering_lossy_lt())   );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_lossy_gt_1(), 0)), SIGN(ordering_lossy_gt_1()) );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_lossy_gt_2(), 0)), SIGN(ordering_lossy_gt_2()) );
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_lossy_gt_3(), 0)), SIGN(ordering_lossy_gt_3()) );

    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_err_1(),      0)), ORDERING(SIGN(ordering_err_1())      ));
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_err_2(),      0)), ORDERING(SIGN(ordering_err_2())      ));
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_lossy_lt(),   0)), ORDERING(SIGN(ordering_lossy_lt())   ));
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_lossy_gt_1(), 0)), ORDERING(SIGN(ordering_lossy_gt_1()) ));
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_lossy_gt_2(), 0)), ORDERING(SIGN(ordering_lossy_gt_2()) ));
    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(ordering_lossy_gt_3(), 0)), ORDERING(SIGN(ordering_lossy_gt_3()) ));

    /* ---------------------------------------------------------------- */

    ASSERT2( inteq, ORDERING(SUCCESFULL_CMP(-7, -7)), ordering_rel_eq );
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t comparers_test =
  {  comparers_test_run
  , "comparers_test"
  , "comparers return expected results on test input."
  };

unit_test_result_t comparers_test(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    comparer_t  comparer;
    void       *context;

    #define CHECKED_COMPARE0(comparer_base_name, check, baseline, ordering)     \
    if (1)                                                                      \
    {                                                                           \
      comparer = CAT(comparer_, comparer_base_name);                            \
      context  = CAT(comparer_, CAT(comparer_base_name, context))();            \
      ASSERT2( inteq, ORDERING(comparer(context, check, baseline)), ordering ); \
      ASSERT2( inteq, ORDERING(comparer(context, baseline, check)), INVERT(ordering) ); \
    } else do { } while(0)

    /* ---------------------------------------------------------------- */

    int a;
    int b;

    /* ---------------------------------------------------------------- */

    a = 3;
    b = 7;
    CHECKED_COMPARE0(int, &a, &b, ordering_rel_lt);
    /* TODO */
  }

  return result;
}

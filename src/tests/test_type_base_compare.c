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

#define IS_SUCCESS(ordering) checked_is_ordering_success(context, &result, LINETAG(), ordering)
#define IS_ERROR(ordering)   checked_is_ordering_error  (context, &result, LINETAG(), ordering)
#define IS_LOSSY(ordering)   checked_is_ordering_lossy  (context, &result, LINETAG(), ordering)
#define SUCCESS(ordering)    checked_ordering_success   (context, &result, LINETAG(), ordering)
#define INVERT(ordering)     checked_ordering_invert    (context, &result, LINETAG(), ordering)
#define ORDERING(ordering)   checked_ordering_relation  (context, &result, LINETAG(), ordering)

static int checked_is_ordering_success(unit_test_context_t *context, unit_test_result_t *out_result, const char *tag, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    TASSERT2( inteq, tag, (call = is_ordering_success(ordering)), IS_ORDERING_SUCCESS(ordering) );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static int checked_is_ordering_error(unit_test_context_t *context, unit_test_result_t *out_result, const char *tag, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    TASSERT2( inteq, tag, (call = is_ordering_error(ordering)), IS_ORDERING_ERROR(ordering) );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static int checked_is_ordering_lossy(unit_test_context_t *context, unit_test_result_t *out_result, const char *tag, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    TASSERT2( inteq, tag, (call = is_ordering_lossy(ordering)), IS_ORDERING_LOSSY(ordering) );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static int checked_ordering_success(unit_test_context_t *context, unit_test_result_t *out_result, const char *tag, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    TASSERT2( inteq, tag, (call = ordering_success(ordering)), ORDERING_SUCCESS(ordering) );
    TASSERT2( inteq, tag, IS_SUCCESS(call),                    1 );
    TASSERT2( inteq, tag, IS_ERROR  (call),                    0 );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static int checked_ordering_invert(unit_test_context_t *context, unit_test_result_t *out_result, const char *tag, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    TASSERT2( inteq, tag, (call = ordering_invert(ordering)), ORDERING_INVERT(ordering) );
    if (!IS_LOSSY(ordering))
    {
      TASSERT2( inteq, tag, ordering_invert(ordering_invert(ordering)), ordering );
    }
    TASSERT2( inteq, tag, IS_SUCCESS(call),                   IS_SUCCESS     (ordering) );
    TASSERT2( inteq, tag, IS_ERROR  (call),                   IS_ERROR       (ordering) );
    TASSERT2( inteq, tag, IS_LOSSY  (call),                   IS_LOSSY       (ordering) );

    if (IS_SUCCESS(ordering) && !IS_LOSSY(ordering))
    {
      TASSERT2( inteq, tag, call, -ordering);
    }
  }

  if (out_result)
    *out_result |= result;

  return call;
}

static ordering_relation_t checked_ordering_relation(unit_test_context_t *context, unit_test_result_t *out_result, const char *tag, int ordering)
{
  int call = -1;
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    TASSERT2( inteq, tag, (call = ordering_relation(ordering)), ORDERING_RELATION(ordering) );
  }

  if (out_result)
    *out_result |= result;

  return call;
}

unit_test_result_t ordering_equalities_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    LASSERT2( inteq, ordering_err_1(),      ORDERING_ERR_1 );
    LASSERT2( inteq, ordering_err_2(),      ORDERING_ERR_2 );
    LASSERT2( inteq, ordering_lossy_lt(),   ORDERING_LOSSY_LT );
    LASSERT2( inteq, ordering_lossy_gt_1(), ORDERING_LOSSY_GT_1 );
    LASSERT2( inteq, ordering_lossy_gt_2(), ORDERING_LOSSY_GT_2 );
    LASSERT2( inteq, ordering_lossy_gt_3(), ORDERING_LOSSY_GT_3 );

    LASSERT2( inteq, IS_SUCCESS(0),                     1 );
    LASSERT2( inteq, IS_SUCCESS(1),                     1 );
    LASSERT2( inteq, IS_SUCCESS(-1),                    1 );
    LASSERT2( inteq, IS_SUCCESS(8),                     1 );
    LASSERT2( inteq, IS_SUCCESS(-16),                   1 );
    LASSERT2( inteq, IS_SUCCESS(ordering_err_1()),      0 );
    LASSERT2( inteq, IS_SUCCESS(ordering_err_2()),      0 );
    LASSERT2( inteq, IS_SUCCESS(ordering_lossy_lt()),   1 );
    LASSERT2( inteq, IS_SUCCESS(ordering_lossy_gt_1()), 1 );
    LASSERT2( inteq, IS_SUCCESS(ordering_lossy_gt_2()), 1 );
    LASSERT2( inteq, IS_SUCCESS(ordering_lossy_gt_3()), 1 );

    LASSERT2( inteq, IS_ERROR(0),                       0 );
    LASSERT2( inteq, IS_ERROR(1),                       0 );
    LASSERT2( inteq, IS_ERROR(-1),                      0 );
    LASSERT2( inteq, IS_ERROR(8),                       0 );
    LASSERT2( inteq, IS_ERROR(-16),                     0 );
    LASSERT2( inteq, IS_ERROR(ordering_err_1()),        1 );
    LASSERT2( inteq, IS_ERROR(ordering_err_2()),        1 );
    LASSERT2( inteq, IS_ERROR(ordering_lossy_lt()),     0 );
    LASSERT2( inteq, IS_ERROR(ordering_lossy_gt_1()),   0 );
    LASSERT2( inteq, IS_ERROR(ordering_lossy_gt_2()),   0 );
    LASSERT2( inteq, IS_ERROR(ordering_lossy_gt_3()),   0 );

    LASSERT2( inteq, IS_LOSSY(0),                       0 );
    LASSERT2( inteq, IS_LOSSY(1),                       0 );
    LASSERT2( inteq, IS_LOSSY(-1),                      0 );
    LASSERT2( inteq, IS_LOSSY(8),                       0 );
    LASSERT2( inteq, IS_LOSSY(-16),                     0 );
    LASSERT2( inteq, IS_LOSSY(ordering_err_1()),        0 );
    LASSERT2( inteq, IS_LOSSY(ordering_err_2()),        0 );
    LASSERT2( inteq, IS_LOSSY(ordering_lossy_lt()),     1 );
    LASSERT2( inteq, IS_LOSSY(ordering_lossy_gt_1()),   1 );
    LASSERT2( inteq, IS_LOSSY(ordering_lossy_gt_2()),   1 );
    LASSERT2( inteq, IS_LOSSY(ordering_lossy_gt_3()),   1 );

    LASSERT2( inteq, SUCCESS(0),                         0 );
    LASSERT2( inteq, SUCCESS(1),                         1 );
    LASSERT2( inteq, SUCCESS(-1),                       -1 );
    LASSERT2( inteq, SUCCESS(8),                         8 );
    LASSERT2( inteq, SUCCESS(-16),                      -16 );
    LASSERT2( inteq, SUCCESS(ordering_err_1()),          ordering_lossy_lt()   );
    LASSERT2( inteq, SUCCESS(ordering_err_2()),          ordering_lossy_lt()   );
    LASSERT2( inteq, SUCCESS(ordering_lossy_lt()),       ordering_lossy_lt()   );
    LASSERT2( inteq, SUCCESS(ordering_lossy_gt_1()),     ordering_lossy_gt_1() );
    LASSERT2( inteq, SUCCESS(ordering_lossy_gt_2()),     ordering_lossy_gt_2() );
    LASSERT2( inteq, SUCCESS(ordering_lossy_gt_3()),     ordering_lossy_gt_3() );

    LASSERT2( inteq, ordering_lt(), ORDERING_LT );
    LASSERT2( inteq, ordering_eq(), ORDERING_EQ );
    LASSERT2( inteq, ordering_gt(), ORDERING_GT );

    LASSERT2( inteq, sign_int(ordering_lt()), -1 );
    LASSERT2( inteq, sign_int(ordering_eq()),  0 );
    LASSERT2( inteq, sign_int(ordering_gt()),  1 );

    LASSERT2( inteq, INVERT(0),                        0                    );
    LASSERT2( inteq, INVERT(1),                       -1                    );
    LASSERT2( inteq, INVERT(-1),                       1                    );
    LASSERT2( inteq, INVERT(8),                       -8                    );
    LASSERT2( inteq, INVERT(-16),                      16                    );
    LASSERT2( inteq, INVERT(ordering_err_1()),        ordering_err_1()      );
    LASSERT2( inteq, INVERT(ordering_err_2()),        ordering_err_2()      );
    LASSERT2( inteq, INVERT(ordering_lossy_lt()),     ordering_lossy_gt_1() );
    LASSERT2( inteq, INVERT(ordering_lossy_gt_1()),   ordering_lossy_lt()   );
    LASSERT2( inteq, INVERT(ordering_lossy_gt_2()),   ordering_lossy_lt()   );
    LASSERT2( inteq, INVERT(ordering_lossy_gt_3()),   ordering_lossy_lt()   );

    /* ---------------------------------------------------------------- */

    LASSERT2( inteq, ORDERING(0),                     ordering_rel_eq );
    LASSERT2( inteq, ORDERING(1),                     ordering_rel_gt );
    LASSERT2( inteq, ORDERING(-1),                    ordering_rel_lt );
    LASSERT2( inteq, ORDERING(8),                     ordering_rel_gt );
    LASSERT2( inteq, ORDERING(-16),                   ordering_rel_lt );
    LASSERT2( inteq, ORDERING(ordering_err_1()),      ordering_rel_err_1 );
    LASSERT2( inteq, ORDERING(ordering_err_2()),      ordering_rel_err_2 );
    LASSERT2( inteq, ORDERING(ordering_lossy_lt()),   ordering_rel_lt );

    LASSERT2( inteq, ordering_rel_lt,                 SIGN(ordering_rel_lt) );
    LASSERT2( inteq, ordering_rel_eq,                 SIGN(ordering_rel_eq) );
    LASSERT2( inteq, ordering_rel_gt,                 SIGN(ordering_rel_gt) );

    LASSERT1( true, ordering_rel_lt <  0 );
    LASSERT1( true, ordering_rel_eq == 0 );
    LASSERT1( true, ordering_rel_gt >  0 );

    /* ---------------------------------------------------------------- */

    LASSERT2( inteq, CMP_SUCCESS(-7, -7),  0  );
    LASSERT2( inteq, CMP_SUCCESS(-7, -2), -5  );
    LASSERT2( inteq, CMP_SUCCESS(-7,  0), -7  );
    LASSERT2( inteq, CMP_SUCCESS(-7,  2), -9  );
    LASSERT2( inteq, CMP_SUCCESS(-7,  7), -14 );

    LASSERT2( inteq, CMP_SUCCESS(-2, -7),  5  );
    LASSERT2( inteq, CMP_SUCCESS(-2, -2),  0  );
    LASSERT2( inteq, CMP_SUCCESS(-2,  0), -2  );
    LASSERT2( inteq, CMP_SUCCESS(-2,  2), -4  );
    LASSERT2( inteq, CMP_SUCCESS(-2,  7), -9  );

    LASSERT2( inteq, CMP_SUCCESS( 0, -7),  7  );
    LASSERT2( inteq, CMP_SUCCESS( 0, -2),  2  );
    LASSERT2( inteq, CMP_SUCCESS( 0,  0),  0  );
    LASSERT2( inteq, CMP_SUCCESS( 0,  2), -2  );
    LASSERT2( inteq, CMP_SUCCESS( 0,  7), -7  );

    LASSERT2( inteq, CMP_SUCCESS( 2, -7),  9  );
    LASSERT2( inteq, CMP_SUCCESS( 2, -2),  4  );
    LASSERT2( inteq, CMP_SUCCESS( 2,  0),  2  );
    LASSERT2( inteq, CMP_SUCCESS( 2,  2),  0  );
    LASSERT2( inteq, CMP_SUCCESS( 2,  7), -5  );

    LASSERT2( inteq, CMP_SUCCESS( 7, -7),  14 );
    LASSERT2( inteq, CMP_SUCCESS( 7, -2),  9  );
    LASSERT2( inteq, CMP_SUCCESS( 7,  0),  7  );
    LASSERT2( inteq, CMP_SUCCESS( 7,  2),  5  );
    LASSERT2( inteq, CMP_SUCCESS( 7,  7),  0  );

    LASSERT2( inteq, CMP_SUCCESS(ordering_err_1(),      0), SIGN(ordering_err_1())      );
    LASSERT2( inteq, CMP_SUCCESS(ordering_err_2(),      0), SIGN(ordering_err_2())      );
    LASSERT2( inteq, CMP_SUCCESS(ordering_lossy_lt(),   0), SIGN(ordering_lossy_lt())   );
    LASSERT2( inteq, CMP_SUCCESS(ordering_lossy_gt_1(), 0), SIGN(ordering_lossy_gt_1()) );
    LASSERT2( inteq, CMP_SUCCESS(ordering_lossy_gt_2(), 0), SIGN(ordering_lossy_gt_2()) );
    LASSERT2( inteq, CMP_SUCCESS(ordering_lossy_gt_3(), 0), SIGN(ordering_lossy_gt_3()) );

    /* ---------------------------------------------------------------- */

    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-7, -7)), ordering_rel_eq );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-7, -2)), ordering_rel_lt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-7,  0)), ordering_rel_lt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-7,  2)), ordering_rel_lt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-7,  7)), ordering_rel_lt );

    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-2, -7)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-2, -2)), ordering_rel_eq );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-2,  0)), ordering_rel_lt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-2,  2)), ordering_rel_lt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(-2,  7)), ordering_rel_lt );

    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 0, -7)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 0, -2)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 0,  0)), ordering_rel_eq );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 0,  2)), ordering_rel_lt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 0,  7)), ordering_rel_lt );

    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 2, -7)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 2, -2)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 2,  0)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 2,  2)), ordering_rel_eq );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 2,  7)), ordering_rel_lt );

    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 7, -7)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 7, -2)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 7,  0)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 7,  2)), ordering_rel_gt );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS( 7,  7)), ordering_rel_eq );

    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_err_1(),      0)), SIGN(ordering_err_1())      );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_err_2(),      0)), SIGN(ordering_err_2())      );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_lossy_lt(),   0)), SIGN(ordering_lossy_lt())   );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_lossy_gt_1(), 0)), SIGN(ordering_lossy_gt_1()) );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_lossy_gt_2(), 0)), SIGN(ordering_lossy_gt_2()) );
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_lossy_gt_3(), 0)), SIGN(ordering_lossy_gt_3()) );

    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_err_1(),      0)), ORDERING(SIGN(ordering_err_1())      ));
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_err_2(),      0)), ORDERING(SIGN(ordering_err_2())      ));
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_lossy_lt(),   0)), ORDERING(SIGN(ordering_lossy_lt())   ));
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_lossy_gt_1(), 0)), ORDERING(SIGN(ordering_lossy_gt_1()) ));
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_lossy_gt_2(), 0)), ORDERING(SIGN(ordering_lossy_gt_2()) ));
    LASSERT2( inteq, ORDERING(CMP_SUCCESS(ordering_lossy_gt_3(), 0)), ORDERING(SIGN(ordering_lossy_gt_3()) ));
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t comparers_test =
  {  comparers_test_run
  , "comparers_test"
  , "comparers return expected results on test input."
  };

unit_test_result_t comparers_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  ENCLOSE()
  {
    #define COMPARE_ORD(callback_compare_suffix, check, baseline, ordering)                                                                  \
    if (1)                                                                                                                                   \
    {                                                                                                                                        \
      TASSERT2( inteq, LINETAG_SUF("0"), ORDERING(call_callback_compare(CAT(callback_compare_, callback_compare_suffix), check, baseline)), ordering );         \
      TASSERT2( inteq, LINETAG_SUF("1"), ORDERING(call_callback_compare(CAT(callback_compare_, callback_compare_suffix), baseline, check)), INVERT(ordering) ); \
    } else do { } while(0)

    #define COMPARE(   callback_compare_suffix, check, baseline, ordering)                                                         \
    if (1)                                                                                                                         \
    {                                                                                                                              \
      COMPARE_ORD(callback_compare_suffix, check, baseline, ORDERING(ordering));                                                   \
      TASSERT2( inteq, LINETAG_SUF("2"), call_callback_compare(CAT(callback_compare_, callback_compare_suffix), check, baseline), ordering );         \
      TASSERT2( inteq, LINETAG_SUF("3"), call_callback_compare(CAT(callback_compare_, callback_compare_suffix), baseline, check), INVERT(ordering) ); \
    } else do { } while(0)

    /* ---------------------------------------------------------------- */

    int a;
    int b;

    /* ---------------------------------------------------------------- */

    a = 3;
    b = 7;
    COMPARE(int(), &a, &b, -4);
    /* TODO */
  }

  return result;
}

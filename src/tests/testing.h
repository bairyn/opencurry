/*
 * opencurry: tests/testing.h
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


/*
 * tests/testing.h
 * ------
 *
 * Included by each C file under `testing/`.
 */

#ifndef TESTS_TESTING_H
#define TESTS_TESTING_H

/* stddef.h:
 *   - size_t
 */
#include <stddef.h>

/*
 * stdio.h:
 *  - FILE
 *  - stdout
 *  - stderr
 */
#include <stdio.h>

#include "../base.h"
#include "testing.h"

/* ---------------------------------------------------------------- */
/* Unit test context type. */

/* struct unit_test_context_s:
 *
 * This structure is threaded through each unit test function during testing.
 */
struct unit_test_context_s
{
  /* Environment. */
  char   *err_buf;
  size_t err_buf_len;
  int    seed;

  FILE   *out;
  FILE   *err;

  void   (*free)(struct unit_test_context_s *self);

  /* State. */
  int    aborting;       /* Silence output, and don't update last_fail and last_pass.
                            Automatically set on aborting failures. */

  int    group_depth;  /* Group depth. */

  int    next_test_id; /* Number of the id of the next test that would be run.   */
                       /* This is incremented immediately before a test is run.  */
                       /* The current test id is equal to this minus 1.          */

  int    num_pass;     /* Number of tests passed so far. */
  int    num_fail;     /* Number of tests failed so far. */

  int    last_fail;    /* Number of the last test that failed; default -1. */
  int    last_pass;    /* Number of the last test that passed; default -1. */
};
typedef struct unit_test_context_s unit_test_context_t;

#define DEFAULT_TEST_CONTEXT_ERR_BUF_SIZE (65536)
#define DEFAULT_TEST_CONTEXT_SEED         (42)
#define DEFAULT_TEST_CONTEXT_OUT stdout
#define DEFAULT_TEST_CONTEXT_ERR stderr
unit_test_context_t *new_unit_test_context( int override_err_buf_len, int override_seed, int  override_out, int  override_err
                                          , int err_buf_len,          int seed,          FILE *out,         FILE *err);
void                free_unit_test_context(unit_test_context_t *context);

/* ---------------------------------------------------------------- */
/* Unit test type. */

/* Unit test result codes. */
#define UNIT_TEST_PASS          ( 0)
#define UNIT_TEST_FAIL          (-1)
#define UNIT_TEST_FAIL_CONTINUE ( 1)
typedef int unit_test_result_t;

/* A unit test is a function, coupled with a name and description:
 *
 * Input: unit_test_context_t *context
 * Output:
 *    = 0  if the test passes
 *   <= -1 if the test fails.
 *   >= 1  if the test fails, but testing can continue.
 */
typedef int (*unit_test_fun_t)(unit_test_context_t *context);
struct unit_test_s
{
  unit_test_fun_t      run;
  const char          *name;
  const char          *description;
};
typedef struct unit_test_s unit_test_t;


/* ---------------------------------------------------------------- */
/* Running tests. */

int run_test_suite(unit_test_t test);
int run_test_suite_with_context(unit_test_context_t *context, unit_test_t test);

void print_test_suite_result(unit_test_context_t *context, unit_test_result_t result);

int test_result_success(unit_test_result_t result);
int test_result_failure(unit_test_result_t result);
int test_result_can_continue(unit_test_result_t result);
int test_result_need_abort(unit_test_result_t result);

int run_test(unit_test_context_t *context, unit_test_t test);

void print_test_indent(unit_test_context_t *context);
void print_test_prefix(unit_test_context_t *context, unit_test_t test, int id);
void print_passed_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result);
void print_failed_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result);

int run_tests_num(unit_test_context_t *context, unit_test_t **tests, size_t num_tests);
int run_tests(unit_test_context_t *context, unit_test_t **tests);

/* ---------------------------------------------------------------- */
/* Default error messages for assertion failures. */

void assert_failure_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len);

void assert_true_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, int condition);

void assert_inteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, int check, int model);

void assert_streqz_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *check, const char *model);
void assert_streqn_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *check, const char *model, size_t max_len);


void assert_false_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, int condition);

void assert_not_inteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, int check, int model);

void assert_not_streqz_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *check, const char *model);
void assert_not_streqn_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *check, const char *model, size_t max_len);

/* ---------------------------------------------------------------- */
/* Assertions with default error messages; non-zero on failure. */

unit_test_result_t assert_success(unit_test_context_t *context);
unit_test_result_t assert_failure(unit_test_context_t *context, const char *err_msg);
unit_test_result_t assert_failure_continue(unit_test_context_t *context, const char *err_msg);

unit_test_result_t assert_true(unit_test_context_t *context, const char *err_msg, int condition);
unit_test_result_t assert_true_continue(unit_test_context_t *context, const char *err_msg, int condition);

unit_test_result_t assert_inteq(unit_test_context_t *context, const char *err_msg, int check, int model);
unit_test_result_t assert_inteq_continue(unit_test_context_t *context, const char *err_msg, int check, int model);

unit_test_result_t assert_streqz(unit_test_context_t *context, const char *err_msg, const char *check, const char *model);
unit_test_result_t assert_streqz_continue(unit_test_context_t *context, const char *err_msg, const char *check, const char *model);
unit_test_result_t assert_streqn(unit_test_context_t *context, const char *err_msg, const char *check, const char *model, size_t max_len);
unit_test_result_t assert_streqn_continue(unit_test_context_t *context, const char *err_msg, const char *check, const char *model, size_t max_len);


unit_test_result_t assert_false(unit_test_context_t *context, const char *err_msg, int condition);
unit_test_result_t assert_false_continue(unit_test_context_t *context, const char *err_msg, int condition);

unit_test_result_t assert_not_inteq(unit_test_context_t *context, const char *err_msg, int check, int model);
unit_test_result_t assert_not_inteq_continue(unit_test_context_t *context, const char *err_msg, int check, int model);

unit_test_result_t assert_not_streqz(unit_test_context_t *context, const char *err_msg, const char *check, const char *model);
unit_test_result_t assert_not_streqz_continue(unit_test_context_t *context, const char *err_msg, const char *check, const char *model);
unit_test_result_t assert_not_streqn(unit_test_context_t *context, const char *err_msg, const char *check, const char *model, size_t max_len);
unit_test_result_t assert_not_streqn_continue(unit_test_context_t *context, const char *err_msg, const char *check, const char *model, size_t max_len);

#endif /* ifndef TESTS_TESTING_H */

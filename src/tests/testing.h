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
#include "../base.h"
#include "testing.h"

/* Types. */
struct unit_test_context_s
{
  char          *err_buf;
  unsigned int  err_buf_len;
  int           seed;

  /* State. */
  int           num_pass;    /* Number of tests passed so far. */
  int           num_fail;    /* Number of tests failed so far. */

  int           last_test;   /* Number of the last test that ran; default -1. */
  int           last_fail;   /* Number of the last test that failed; default -1. */
  int           last_pass;   /* Number of the last test that passed; default -1. */
};
typedef struct unit_test_context_s unit_test_context_t;

/* A unit test is a function:
 *
 * Input: unit_test_context_t *context
 * Output:
 *    = 0  if the test passes
 *   >= 1  if the test fails
 *   <= -1 if the test fails, but testing can continue.
 */
#define UNIT_TEST_PASS          ( 0)
#define UNIT_TEST_FAIL          ( 1)
#define UNIT_TEST_FAIL_CONTINUE (-1)
typedef int (*unit_test_t)(unit_test_context_t *context);

#define DEFAULT_TEST_ERR_BUF_SIZE (65536)
#define DEFAULT_TEST_SEED         (42)
unit_test_context_t *new_unit_test_context(int override_err_buf_len, int override_seed, int err_buf_len, int seed);
void                free_unit_test_context(unit_test_context_t *context);

/* Running tests. */
#define DEFAULT_TEST_GROUP_NAME "(unnamed test group)"
extern char default_test_group_name[];
#define DEFAULT_TEST_NAME "(unnamed test)"
extern char default_test_name[];

int  run_test_suite_num(unit_test_t *tests, unsigned int num_tests, const char *test_group_name);
int  run_test_suite(unit_test_t *tests, const char *test_group_name);
void print_test_suite_result(unit_test_context_t *context, int result, const char *test_group_name);
int  run_tests(unit_test_context_t *context, unit_test_t *tests, const char *test_group_name);
int  run_tests_num(unit_test_context_t *context, unit_test_t *tests, unsigned int num_tests, const char *test_group_name);
int  run_test(unit_test_context_t *context, unit_test_t tests, const char *test_name);
void print_passed_test_result(unit_test_context_t *context, int result, const char *test_name);
void print_failed_test_result(unit_test_context_t *context, int result, const char *test_name);

/* Default error messages for assertion failures. */
void assert_failure_msg(unit_test_context_t *context, const char *name, char *msg_out, unsigned int msg_out_len);
void assert_streq_msg(unit_test_context_t *context, const char *name, char *check_out, unsigned int check_out_len, char *model_out, unsigned int model_out_len);

/* Assertions with default error messages; non-zero positive on failure. */
int assert_success(unit_test_context_t *context, const char *name);
int assert_failure(unit_test_context_t *context, const char *name, const char *err_msg);
int assert_failure_continue(unit_test_context_t *context, const char *name, const char *err_msg);

int assert_inteq(unit_test_context_t *context, const char *name, const char *err_msg, int check, int model);

int assert_streqz(unit_test_context_t *context, const char *name, const char *err_msg, const char *check, const char *model);
int assert_streqn(unit_test_context_t *context, const char *name, const char *err_msg, const char *check, const char *model, unsigned int len);
int assert_streqnz(unit_test_context_t *context, const char *name, const char *err_msg, const char *check, const char *model, unsigned int max_len);

#endif /* ifndef TESTS_TESTING_H */

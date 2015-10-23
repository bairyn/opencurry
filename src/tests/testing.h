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
  /* Whether the value has been initialized. */
  int      is_initialized;
  /* Whether the value was initialized on the heap.
   * free_unit_test_context_t checks this value to determine whether to call
   * "free".
   */
  int      is_heap_allocated;

  /* Environment. */

  /* How to emit output. */
  FILE    *out;
  FILE    *err;

  /* Error message buffer. */
  /* Twice "halfsize" is allocated to this buffer.  The second half is reserved
   * to record the error buffer for the first failing unit test. */
  char    *err_buf;
  size_t   err_buf_halfsize;
  size_t   err_buf_len;
  size_t   err_buf_first_err_len;
  int      err_buf_is_heap;

  /* Internal error message buffer. */
  char    *int_err_buf;
  size_t   int_err_buf_size;
  size_t   int_err_buf_len;
  int      is_snprintf_err;
  int      int_err_buf_is_heap;

  /*
   * Buffer to record extra information in case a test fails.
   * The assertion message generator procedures use this when an assertion
   * fails to append to the error buffer, before resetting this one.
   */
  char    *details_buf;
  size_t   details_buf_size;
  size_t   details_buf_len;
  int      details_buf_is_heap;

  /*
   * Extra buffer for individual unit tests, not used by "testing".
   *
   * This may be reset at the beginning of any "unit_test_t".
   */
  void    *user_buf;
  size_t   user_buf_size;
  int      user_buf_is_heap;

  /*
   * Extra data buffer.
   */
  void    *misc_buf;
  size_t   misc_buf_size;
  int      misc_buf_is_heap;

  /*
   * Data for pseudo-random number generation.
   */
  unsigned int seed;
  unsigned int seed_initial;
  unsigned int seed_first_err_start;
  unsigned int seed_first_err_end;

  void   (*free)(struct unit_test_context_s *self);

  /* State. */
  int      aborting;     /* Silence output, and don't update last_fail and         *
                          * last_pass.  Automatically set on aborting failures.    */

  int      group_depth;  /* Group depth. */

  int      next_test_id; /* Number of the id of the next test that would be run.   */
                         /* This is incremented immediately before a test is run.  */
                         /* The current test id is equal to this minus 1.          */

  int      num_pass;     /* Number of tests passed so far.                         */
  int      num_fail;     /* Number of tests failed so far.                         */
  int      num_skip;     /* Number of tests skipped so far.                        */

  int      last_fail;    /* Number of the last test that failed; default -1.       */
  int      last_pass;    /* Number of the last test that passed; default -1.       */
  int      last_skip;    /* Number of the last test that was skipped; default -1.  */

  int      first_fail;   /* ID of the first test that failed; default -1.          */
};
typedef struct unit_test_context_s unit_test_context_t;

#define CONTEXT_UNINITIALIZED         0
#define CONTEXT_FULLY_INITIALIZED     1
#define CONTEXT_PARTIALLY_INITIALIZED 2

#define DEFAULT_TEST_CONTEXT_ERR_BUF_HALFSIZE (65536)
#define DEFAULT_TEST_CONTEXT_INT_ERR_BUF_SIZE (65536)
#define DEFAULT_TEST_CONTEXT_DETAILS_BUF_SIZE (16 * 65536)
#define DEFAULT_TEST_CONTEXT_USER_BUF_SIZE    (32 * 65536)
#define DEFAULT_TEST_CONTEXT_MISC_BUF_SIZE    (4  * 65536)
#define DEFAULT_TEST_CONTEXT_SEED             (42)
#define DEFAULT_TEST_CONTEXT_OUT              stdout
#define DEFAULT_TEST_CONTEXT_ERR              stderr
unit_test_context_t *new_unit_test_context
  ( unit_test_context_t *initialize_context_noheap

  , int override_err_buf_halfsize, int override_int_err_buf_size, int override_details_buf_size
  , int override_user_buf_size,    int override_misc_buf_size
  , int override_seed,             int override_out,              int override_err

  , size_t       err_buf_halfsize, size_t       int_err_buf_size, size_t       details_buf_size
  , size_t       user_buf_size,    size_t       misc_buf_size
  , unsigned int seed,             FILE        *out,              FILE        *err
  );
unit_test_context_t *new_unit_test_context_defaults(unit_test_context_t *initialize_context_noheap);
void                free_unit_test_context(unit_test_context_t *context);

/* ---------------------------------------------------------------- */
/* Unit test type. */

/* Unit test result codes. */
#define UNIT_TEST_PASS             ( 0)
#define UNIT_TEST_FAIL             (-1)
#define UNIT_TEST_FAIL_CONTINUE    ( 1)
#define UNIT_TEST_INTERNAL_ERROR   (-3)
#define UNIT_TEST_SKIPPED          (-4)
#define UNIT_TEST_SKIPPED_CONTINUE ( 4)
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

  /* (Internal API procedures, unlikely to be useful to users.) */
  void print_test_suite_result(unit_test_context_t *context, unit_test_result_t result);

int is_test_result_success(unit_test_result_t result);
int is_test_result_failure(unit_test_result_t result);
int is_test_result_skip(unit_test_result_t result);
int is_test_result_can_continue(unit_test_result_t result);
int is_test_result_aborting(unit_test_result_t result);

unit_test_result_t run_test(unit_test_context_t *context, unit_test_t test);

  /* (Internal API procedures, unlikely to be useful to users.) */
  void process_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start);
  void process_first_test_failure(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start);

  void print_test_indent(unit_test_context_t *context);
  void print_test_prefix(unit_test_context_t *context, unit_test_t test, int id);
  void print_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start);
  void print_passed_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start);
  void print_failed_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start);
  void print_skipped_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start);

  void ensure_buf_limits(unit_test_context_t *context, char *buf);

  size_t test_add_internal_error_msg(unit_test_context_t *context, const char *msg);
  void context_internal_error(unit_test_context_t *context, const char *msg);

unit_test_result_t run_tests_num(unit_test_context_t *context, unit_test_t **tests, size_t num_tests);
unit_test_result_t run_tests(unit_test_context_t *context, unit_test_t **tests);

/* ---------------------------------------------------------------- */
/* Default error messages for assertion failures. */

#define DEFAULT_TAG_SIZE 4096  /* (2 << 12) */

#define ASSERT_MSG_WIDTH 80

void reset_err_msg_details(unit_test_context_t *context);
size_t test_add_details_msg(unit_test_context_t *context, const char *msg);

size_t assert_snprintf_error_msg(unit_test_context_t *context, int snprintf_error_code, char *msg_out, size_t msg_out_size, const char *tag);
size_t assert_msg_check_snprintf(unit_test_context_t *context, int snprintf_result, char *msg_out, size_t msg_out_size, const char *tag, int *out_is_err);


size_t assert_msg_append_details(unit_test_context_t *context, size_t len, char *msg_out, size_t msg_out_size, const char *tag);

size_t assert_failure_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag);

size_t assert_true_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int condition);

size_t assert_inteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int check, int model);

size_t assert_streqz_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char *check, const char *model);
size_t assert_streqn_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char *check, const char *model, size_t max_len);

size_t assert_memeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, void *check, void *model, size_t n);


size_t assert_false_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int condition);

size_t assert_not_inteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int check, int model);

size_t assert_not_streqz_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char *check, const char *model);
size_t assert_not_streqn_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char *check, const char *model, size_t max_len);

size_t assert_not_memeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, void *check, void *model, size_t n);

/* ---------------------------------------------------------------- */
/* Assertions with default error messages; non-zero on failure. */

unit_test_result_t assert_success(unit_test_context_t *context);
unit_test_result_t assert_failure(unit_test_context_t *context, const char *err_msg, const char *tag);
unit_test_result_t assert_failure_continue(unit_test_context_t *context, const char *err_msg, const char *tag);

unit_test_result_t assert_true(unit_test_context_t *context, const char *err_msg, const char *tag, int condition);
unit_test_result_t assert_true_continue(unit_test_context_t *context, const char *err_msg, const char *tag, int condition);

unit_test_result_t assert_inteq(unit_test_context_t *context, const char *err_msg, const char *tag, int check, int model);
unit_test_result_t assert_inteq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, int check, int model);

unit_test_result_t assert_streqz(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model);
unit_test_result_t assert_streqz_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model);
unit_test_result_t assert_streqn(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model, size_t max_len);
unit_test_result_t assert_streqn_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model, size_t max_len);

unit_test_result_t assert_memeq(unit_test_context_t *context, const char *err_msg, const char *tag, void *check, void *model, size_t n);
unit_test_result_t assert_memeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, void *check, void *model, size_t n);


unit_test_result_t assert_false(unit_test_context_t *context, const char *err_msg, const char *tag, int condition);
unit_test_result_t assert_false_continue(unit_test_context_t *context, const char *err_msg, const char *tag, int condition);

unit_test_result_t assert_not_inteq(unit_test_context_t *context, const char *err_msg, const char *tag, int check, int model);
unit_test_result_t assert_not_inteq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, int check, int model);

unit_test_result_t assert_not_streqz(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model);
unit_test_result_t assert_not_streqz_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model);
unit_test_result_t assert_not_streqn(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model, size_t max_len);
unit_test_result_t assert_not_streqn_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model, size_t max_len);

unit_test_result_t assert_not_memeq(unit_test_context_t *context, const char *err_msg, const char *tag, void *check, void *model, size_t n);
unit_test_result_t assert_not_memeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, void *check, void *model, size_t n);

#endif /* ifndef TESTS_TESTING_H */

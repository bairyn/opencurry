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

#include "../util.h"

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
#define UNIT_TEST_SKIP             (-4)
#define UNIT_TEST_SKIP_CONTINUE    ( 4)

#define UNIT_TEST_SKIPPED          UNIT_TEST_SKIP
#define UNIT_TEST_SKIPPED_CONTINUE UNIT_TEST_SKIP_CONTINUE
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

unit_test_t unit_test(unit_test_fun_t run, const char *name, const char *description);

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

#define TEST_RESULT_PASS_PRINT_SAME_LINE          ENABLE()
#define TEST_RESULT_PASS_PRINT_DESCRIPTION        DISABLE() || IS_FALSE(TEST_RESULT_PASS_PRINT_SAME_LINE)

#define TEST_RESULT_SKIP_PRINT_DESCRIPTION        ENABLE()

#define TEST_RESULT_GROUP_PRINT_INTERSPERSED_LINE DISABLE()

#define TEST_RESULT_GROUP_REPEAT_ID               ENABLE()

#define TEST_RESULT_PRINT_GROUP_PASS_RESULT       DISABLE()
#define TEST_RESULT_PRINT_GROUP_FAIL_RESULT       DISABLE()

unit_test_result_t run_test(unit_test_context_t *context, unit_test_t test);

  /* (Internal API procedures, unlikely to be useful to users.) */
  void process_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start);
  void process_first_test_failure(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start);

#define TEST_INDENT_NO_ALERT 0
#define TEST_INDENT_ALERT    1
#define TEST_INDENT_NO_PENDING_TEXT 1
#define TEST_INDENT_PENDING_TEXT    0
  void print_test_indent(unit_test_context_t *context, int alert, int no_pending_text);
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

#define TEST_LEVEL(context) ((context)->group_depth - 1)
int test_level(unit_test_context_t *context);

/* ---------------------------------------------------------------- */
/* Trivial unit tests.                                              */

unit_test_result_t passing_test(unit_test_context_t *context);
unit_test_result_t failing_test(unit_test_context_t *context);
unit_test_result_t skipped_test(unit_test_context_t *context);
unit_test_result_t failing_continue_test(unit_test_context_t *context);
unit_test_result_t skipped_continue_test(unit_test_context_t *context);
unit_test_result_t internal_error_test(unit_test_context_t *context);

extern const unit_test_fun_t passing_test_fun;
extern const unit_test_fun_t failing_test_fun;
extern const unit_test_fun_t skipped_test_fun;
extern const unit_test_fun_t failing_continue_test_fun;
extern const unit_test_fun_t skipped_continue_test_fun;
extern const unit_test_fun_t internal_error_test_fun;

unit_test_fun_t trivial_test(unit_test_context_t *context, unit_test_result_t result);
unit_test_t trivial_unit_test(unit_test_context_t *context, unit_test_result_t result, const char *name, const char *description);
unit_test_result_t run_trivial_test(unit_test_context_t *context, unit_test_result_t result, const char *name, const char *description);

unit_test_t anonymous_skip_continue_test(unit_test_context_t *context);

/* ---------------------------------------------------------------- */
/* Default error messages for assertion failures.                   */

typedef void *(*tests_funp_t)(void *, ...);

#define DEFAULT_TAG_SIZE 4096  /* (2 << 12) */

#define ASSERT_MSG_WIDTH 80

void reset_err_buf(unit_test_context_t *context);

void reset_err_msg_details(unit_test_context_t *context);
size_t test_add_details_msg(unit_test_context_t *context, const char *msg);
size_t test_set_details_msg(unit_test_context_t *context, const char *msg);

size_t assert_snprintf_error_msg(unit_test_context_t *context, int snprintf_error_code, char *msg_out, size_t msg_out_size, const char *tag);
size_t assert_msg_check_snprintf(unit_test_context_t *context, int snprintf_result, char *msg_out, size_t msg_out_size, const char *tag, int *out_is_err);

size_t assert_msg_append_details(unit_test_context_t *context, size_t len, char *msg_out, size_t msg_out_size, const char *tag);

/* ---------------------------------------------------------------- */

size_t assert_failure_msg       (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag);

size_t assert_true_msg          (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int            condition);

size_t assert_inteq_msg         (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int            check,     int            model);
size_t assert_uinteq_msg        (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, unsigned int   check,     unsigned int   model);
size_t assert_ulongeq_msg       (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, unsigned long  check,     unsigned long  model);
size_t assert_sizeeq_msg        (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, size_t         check,     size_t         model);

size_t assert_objpeq_msg        (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const void    *check,     const void    *model);
size_t assert_funpeq_msg        (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, tests_funp_t   check,     tests_funp_t   model);

size_t assert_streqz_msg        (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char    *check,     const char    *model);
size_t assert_streqn_msg        (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char    *check,     const char    *model, size_t max_len);

size_t assert_memeq_msg         (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, void          *check,     void          *model, size_t n);


size_t assert_false_msg         (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int            condition);

size_t assert_not_inteq_msg     (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int            check,     int            model);
size_t assert_not_uinteq_msg    (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, unsigned int   check,     unsigned int   model);
size_t assert_not_ulongeq_msg   (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, unsigned long  check,     unsigned long  model);
size_t assert_not_sizeeq_msg    (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, size_t         check,     size_t         model);

size_t assert_not_objpeq_msg    (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const void    *check,     const void    *model);
size_t assert_not_funpeq_msg    (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, tests_funp_t   check,     tests_funp_t   model);

size_t assert_not_streqz_msg    (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char    *check,     const char    *model);
size_t assert_not_streqn_msg    (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char    *check,     const char    *model, size_t max_len);

size_t assert_not_memeq_msg     (unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, void          *check,     void          *model, size_t n);

/* ---------------------------------------------------------------- */
/* Assertions with default error messages; non-zero on failure. */

#define ASSERT_PARAMS unit_test_context_t *context, const char *err_msg, const char *tag
#define ASSERT_PARAMSC 3

#define ASSERT_ARGS context, err_msg, tag
#define ASSERT_ARGC ASSERT_PARAMSC

#define ASSERT_ARGS_TAGGED(  tag)                   ASSERT_ARGS_CONTEXT (context, tag)
#define ASSERT_ARGS_CONTEXT( context, tag)          ASSERT_ARGS_COMPLETE(context, NULL, tag)
#define ASSERT_ARGS_COMPLETE(context, err_msg, tag) context, err_msg, tag

unit_test_result_t assert_success             (unit_test_context_t *context);
unit_test_result_t assert_successful          (ASSERT_PARAMS);
unit_test_result_t assert_failure             (ASSERT_PARAMS);
unit_test_result_t assert_failure_continue    (ASSERT_PARAMS);

unit_test_result_t assert_true                (ASSERT_PARAMS, int           condition);
unit_test_result_t assert_true_continue       (ASSERT_PARAMS, int           condition);

unit_test_result_t assert_inteq               (ASSERT_PARAMS, int           check,     int            model);
unit_test_result_t assert_inteq_continue      (ASSERT_PARAMS, int           check,     int            model);
unit_test_result_t assert_uinteq              (ASSERT_PARAMS, unsigned int  check,     unsigned int   model);
unit_test_result_t assert_uinteq_continue     (ASSERT_PARAMS, unsigned int  check,     unsigned int   model);
unit_test_result_t assert_ulongeq             (ASSERT_PARAMS, unsigned long check,     unsigned long  model);
unit_test_result_t assert_ulongeq_continue    (ASSERT_PARAMS, unsigned long check,     unsigned long  model);
unit_test_result_t assert_sizeeq              (ASSERT_PARAMS, size_t        check,     size_t         model);
unit_test_result_t assert_sizeeq_continue     (ASSERT_PARAMS, size_t        check,     size_t         model);

unit_test_result_t assert_objpeq              (ASSERT_PARAMS, const void   *check,     const void    *model);
unit_test_result_t assert_objpeq_continue     (ASSERT_PARAMS, const void   *check,     const void    *model);
unit_test_result_t assert_funpeq              (ASSERT_PARAMS, tests_funp_t  check,     tests_funp_t   model);
unit_test_result_t assert_funpeq_continue     (ASSERT_PARAMS, tests_funp_t  check,     tests_funp_t   model);

unit_test_result_t assert_streqz              (ASSERT_PARAMS, const char   *check,     const char    *model);
unit_test_result_t assert_streqz_continue     (ASSERT_PARAMS, const char   *check,     const char    *model);
unit_test_result_t assert_streqn              (ASSERT_PARAMS, const char   *check,     const char    *model, size_t max_len);
unit_test_result_t assert_streqn_continue     (ASSERT_PARAMS, const char   *check,     const char    *model, size_t max_len);
unit_test_result_t assert_nstreq              (ASSERT_PARAMS, size_t max_len, const char   *check,     const char    *model);
unit_test_result_t assert_nstreq_continue     (ASSERT_PARAMS, size_t max_len, const char   *check,     const char    *model);

unit_test_result_t assert_memeq               (ASSERT_PARAMS, void         *check,     void          *model, size_t n);
unit_test_result_t assert_memeq_continue      (ASSERT_PARAMS, void         *check,     void          *model, size_t n);


unit_test_result_t assert_false               (ASSERT_PARAMS, int           condition);
unit_test_result_t assert_false_continue      (ASSERT_PARAMS, int           condition);

unit_test_result_t assert_not_inteq           (ASSERT_PARAMS, int           check,     int            model);
unit_test_result_t assert_not_inteq_continue  (ASSERT_PARAMS, int           check,     int            model);
unit_test_result_t assert_not_uinteq          (ASSERT_PARAMS, unsigned int  check,     unsigned int   model);
unit_test_result_t assert_not_uinteq_continue (ASSERT_PARAMS, unsigned int  check,     unsigned int   model);
unit_test_result_t assert_not_ulongeq         (ASSERT_PARAMS, unsigned long check,     unsigned long  model);
unit_test_result_t assert_not_ulongeq_continue(ASSERT_PARAMS, unsigned long check,     unsigned long  model);
unit_test_result_t assert_not_sizeeq          (ASSERT_PARAMS, size_t        check,     size_t         model);
unit_test_result_t assert_not_sizeeq_continue (ASSERT_PARAMS, size_t        check,     size_t         model);

unit_test_result_t assert_not_objpeq          (ASSERT_PARAMS, const void   *check,     const void    *model);
unit_test_result_t assert_not_objpeq_continue (ASSERT_PARAMS, const void   *check,     const void    *model);
unit_test_result_t assert_not_funpeq          (ASSERT_PARAMS, tests_funp_t  check,     tests_funp_t   model);
unit_test_result_t assert_not_funpeq_continue (ASSERT_PARAMS, tests_funp_t  check,     tests_funp_t   model);

unit_test_result_t assert_not_streqz          (ASSERT_PARAMS, const char   *check,     const char    *model);
unit_test_result_t assert_not_streqz_continue (ASSERT_PARAMS, const char   *check,     const char    *model);
unit_test_result_t assert_not_streqn          (ASSERT_PARAMS, const char   *check,     const char    *model, size_t max_len);
unit_test_result_t assert_not_streqn_continue (ASSERT_PARAMS, const char   *check,     const char    *model, size_t max_len);
unit_test_result_t assert_not_nstreq          (ASSERT_PARAMS, size_t max_len, const char   *check,     const char    *model);
unit_test_result_t assert_not_nstreq_continue (ASSERT_PARAMS, size_t max_len, const char   *check,     const char    *model);

unit_test_result_t assert_not_memeq           (ASSERT_PARAMS, void         *check,     void          *model, size_t n);
unit_test_result_t assert_not_memeq_continue  (ASSERT_PARAMS, void         *check,     void          *model, size_t n);

/* ---------------------------------------------------------------- */
/* Utilities.                                                       */
/* ---------------------------------------------------------------- */

/* Adds an individual result value to the composite "result" value. */
#define ADD_RESULT(result_to_add) ADD_RESULT_COMPLETE(result, result_to_add)
#define OUT_RESULT(result_to_add)                        \
  do                                                     \
  {                                                      \
    if (out_result)                                      \
      ADD_RESULT_COMPLETE(*out_result, (result_to_add)); \
  } while(0)
#define ADD_RESULT_COMPLETE(result, result_to_add) (result) |= (result_to_add)

#define NEST_TEST(name, description, assertion) NEST_TEST_CONTEXT(context, name, description, assertion)
#define NEST_TEST_CONTEXT(context, name, description, assertion) run_trivial_test((context), (assertion), (name), (description))

/* If "result" contains an error code indicating non-continuable failure,
 * return it.
 */
#define ABORTABLE(result)                \
  do                                     \
  {                                      \
    if (is_test_result_aborting(result)) \
      return result;                     \
  } while(0)

/* Break when "result" contains an error code indicating non-continuable
 * failure.
 *
 * Can be combined with "ENCLOSE" for code that requires cleanup, for example:
 *
 * > unit_test_result_t my_test_run(unit_test_context_t *context)
 * > {
 * >   unit_test_result_t result = assert_success(context);
 * >
 * >   void *foo = malloc(...);
 * >
 * >   ENCLOSE()
 * >   {
 * >     result |= assert_inteq(context, NULL, "1 = 1", 1, 1);
 * >     BREAKABLE(result);
 * >
 * >     result |= assert_not_inteq(context, NULL, "2 + 2 != 5", 2 + 2, 5);
 * >     BREAKABLE(result);
 * >   }
 * >
 * >   free(foo);
 * >
 * >   return result;
 * > }
 */
#define BREAKABLE_ABORTING(result)     \
  if (is_test_result_aborting(result)) \
    break;                             \
  else                                 \
    do {} while(0)

/* Break on any non-successful result, whether or not testing can continue. */
#define BREAKABLE_ANY(result)                \
  if (  (  is_test_result_aborting(result) ) \
     || ( !is_test_result_success (result) ) \
     )                                       \
    break;                                   \
  else                                       \
    do {} while(0)

/* Break on failure, whether or not testing can continue. */
#define BREAKABLE(result)                   \
  if (  ( is_test_result_aborting(result) ) \
     || ( is_test_result_failure (result) ) \
     )                                      \
    break;                                  \
  else                                      \
    do {} while(0)

/* COMPOUND: add the result of an assertion, breaking in an enclosure if
 * unsuccessful.
 */
#define COMPOUND_TRIVIAL(assertion) BREAKABLE((ADD_RESULT((assertion))))

/* Allows "result" to be set in the assertion". */
#define COMPOUND(assertion)              \
  if (1)                                 \
  {                                      \
    unit_test_result_t _COMPOUND_result; \
                                         \
    _COMPOUND_result = (assertion);      \
                                         \
    COMPOUND_TRIVIAL(_COMPOUND_result);  \
  }                                      \
  else                                   \
    do {} while(0)

/* Run the assertion as a child test. */
#define COMPOUND_TEST(name, description, assertion) \
  COMPOUND_TEST_CONTEXT(context, name, description, assertion)
#define COMPOUND_TEST_CONTEXT(context, name, description, assertion) \
  COMPOUND(NEST_TEST_CONTEXT(context, name, description, assertion))

/* ASSERT_NEST: Run an assertion as a child test.
 * This can be composed with the *ASSERTn macros.
 */
#define ASSERT_NEST(name, description, assertion) \
  ASSERT_NEST_CONTEXT(context, name, description, assertion)
#define ASSERT_NEST_CONTEXT(context, name, description, assertion)           \
  if (1)                                                                     \
  {                                                                          \
    ENCLOSE()                                                                \
    {                                                                        \
      COMPOUND((assertion));                                                 \
    }                                                                        \
                                                                             \
    COMPOUND((NEST_TEST_CONTEXT((context), (name), (description), result))); \
  }                                                                          \
  else                                                                       \
    do {} while(0)

#define LNEST_TEST(assertion) \
  NEST_TEST    (("child test: " LINETAG()), ("LNEST_TEST(assertion): child test: " LINETAG()),     (assertion))

#define LCOMPOUND_TEST(assertion) \
  COMPOUND_TEST(("child test: " LINETAG()), ("LCOMPOUND_TEST(assertion): child test: " LINETAG()), (assertion))

#define LASSERT_NEST(assertion) \
  ASSERT_NEST  (("child test: " LINETAG()), ("LASSERT_NEST(assertion): child test: " LINETAG()),   (assertion))

/*
 * n-ary COMPOUND-based assertions.
 *
 * Example:
 * > unit_test_result_t my_unit_test_run(unit_test_context_t *context)
 * > {
 * >   unit_test_result_t result = assert_success(context);
 * >
 * >   ENCLOSE()
 * >   {
 * >     ASSERT2( inteq, "1=1", 1, 1 );
 * >     ASSERT2( inteq, "2=2", 2, 2 );
 * >   }
 * >
 * >   return result;
 * > }
 */

#define TASSERT0( assert_method, tag) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag))

#define TASSERT1( assert_method, tag, arg1) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1))

#define TASSERT2( assert_method, tag, arg1, arg2) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2))

#define TASSERT3( assert_method, tag, arg1, arg2, arg3) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2, arg3))

#define TASSERT4( assert_method, tag, arg1, arg2, arg3, arg4) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2, arg3, arg4))

#define TASSERT5( assert_method, tag, arg1, arg2, arg3, arg4, arg5) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2, arg3, arg4, arg5))

#define TASSERT6( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2, arg3, arg4, arg5, arg6))

#define TASSERT7( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7))

#define TASSERT8( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8))

#define TASSERT9( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9))

#define TASSERT10(assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
  COMPOUND(CAT(assert_, assert_method)(context, NULL, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10))

/* LASSERT: The line number is the tag.
 */

#define LINETAG() "line #" STR(__LINE__)
#define LINETAG_SUF(suffix) "line #" STR(__LINE__) "_" suffix

#define LASSERT0( assert_method) \
  TASSERT0 (assert_method, LINETAG())

#define LASSERT1( assert_method, arg1) \
  TASSERT1 (assert_method, LINETAG(), arg1)

#define LASSERT2( assert_method, arg1, arg2) \
  TASSERT2 (assert_method, LINETAG(), arg1, arg2)

#define LASSERT3( assert_method, arg1, arg2, arg3) \
  TASSERT3 (assert_method, LINETAG(), arg1, arg2, arg3)

#define LASSERT4( assert_method, arg1, arg2, arg3, arg4) \
  TASSERT4 (assert_method, LINETAG(), arg1, arg2, arg3, arg4)

#define LASSERT5( assert_method, arg1, arg2, arg3, arg4, arg5) \
  TASSERT5 (assert_method, LINETAG(), arg1, arg2, arg3, arg4, arg5)

#define LASSERT6( assert_method, arg1, arg2, arg3, arg4, arg5, arg6) \
  TASSERT6 (assert_method, LINETAG(), arg1, arg2, arg3, arg4, arg5, arg6)

#define LASSERT7( assert_method, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
  TASSERT7 (assert_method, LINETAG(), arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define LASSERT8( assert_method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
  TASSERT8 (assert_method, LINETAG(), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#define LASSERT9( assert_method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
  TASSERT9 (assert_method, LINETAG(), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define LASSERT10(assert_method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
  TASSERT10(assert_method, LINETAG(), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)


#define LTASSERT0( assert_method, tag) \
  TASSERT0 (assert_method, LINETAG() ": " tag)

#define LTASSERT1( assert_method, tag, arg1) \
  TASSERT1 (assert_method, LINETAG() ": " tag, arg1)

#define LTASSERT2( assert_method, tag, arg1, arg2) \
  TASSERT2 (assert_method, LINETAG() ": " tag, arg1, arg2)

#define LTASSERT3( assert_method, tag, arg1, arg2, arg3) \
  TASSERT3 (assert_method, LINETAG() ": " tag, arg1, arg2, arg3)

#define LTASSERT4( assert_method, tag, arg1, arg2, arg3, arg4) \
  TASSERT4 (assert_method, LINETAG() ": " tag, arg1, arg2, arg3, arg4)

#define LTASSERT5( assert_method, tag, arg1, arg2, arg3, arg4, arg5) \
  TASSERT5 (assert_method, LINETAG() ": " tag, arg1, arg2, arg3, arg4, arg5)

#define LTASSERT6( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6) \
  TASSERT6 (assert_method, LINETAG() ": " tag, arg1, arg2, arg3, arg4, arg5, arg6)

#define LTASSERT7( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
  TASSERT7 (assert_method, LINETAG() ": " tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define LTASSERT8( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
  TASSERT8 (assert_method, LINETAG() ": " tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#define LTASSERT9( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
  TASSERT9 (assert_method, LINETAG() ": " tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define LTASSERT10(assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
  TASSERT10(assert_method, LINETAG() ": " tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)

/* n-ary assertions where the tag passed is a stringification of the assertion
 * method and select arguments.
 */

#define ASSERT0( assert_method) \
  LTASSERT0 (assert_method, STR(assert_method) STR((...)                                                             ))

#define ASSERT1( assert_method, arg1) \
  LTASSERT1 (assert_method, STR(assert_method) STR((..., arg1)                                                       ), arg1)

#define ASSERT2( assert_method, arg1, arg2) \
  LTASSERT2 (assert_method, STR(assert_method) STR((..., arg1, arg2)                                                 ), arg1, arg2)

#define ASSERT3( assert_method, arg1, arg2, arg3) \
  LTASSERT3 (assert_method, STR(assert_method) STR((..., arg1, arg2, arg3)                                           ), arg1, arg2, arg3)

#define ASSERT4( assert_method, arg1, arg2, arg3, arg4) \
  LTASSERT4 (assert_method, STR(assert_method) STR((..., arg1, arg2, arg3, arg4)                                     ), arg1, arg2, arg3, arg4)

#define ASSERT5( assert_method, arg1, arg2, arg3, arg4, arg5) \
  LTASSERT5 (assert_method, STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5)                               ), arg1, arg2, arg3, arg4, arg5)

#define ASSERT6( assert_method, arg1, arg2, arg3, arg4, arg5, arg6) \
  LTASSERT6 (assert_method, STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6)                         ), arg1, arg2, arg3, arg4, arg5, arg6)

#define ASSERT7( assert_method, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
  LTASSERT7 (assert_method, STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6, arg7)                   ), arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define ASSERT8( assert_method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
  LTASSERT8 (assert_method, STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)             ), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#define ASSERT9( assert_method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
  LTASSERT9 (assert_method, STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)       ), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define ASSERT10(assert_method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
  LTASSERT10(assert_method, STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)

/* n-ary assertions with a tag prefixed to the call.  Effectively combines
 * ASSERT(...) and TASSERT(...).
 */

#define MASSERT0( assert_method, tag) \
  TASSERT0 (assert_method, tag ": " STR(assert_method) STR((...)                                                             ))

#define MASSERT1( assert_method, tag, arg1) \
  TASSERT1 (assert_method, tag ": " STR(assert_method) STR((..., arg1)                                                       ), arg1)

#define MASSERT2( assert_method, tag, arg1, arg2) \
  TASSERT2 (assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2)                                                 ), arg1, arg2)

#define MASSERT3( assert_method, tag, arg1, arg2, arg3) \
  TASSERT3 (assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2, arg3)                                           ), arg1, arg2, arg3)

#define MASSERT4( assert_method, tag, arg1, arg2, arg3, arg4) \
  TASSERT4 (assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2, arg3, arg4)                                     ), arg1, arg2, arg3, arg4)

#define MASSERT5( assert_method, tag, arg1, arg2, arg3, arg4, arg5) \
  TASSERT5 (assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5)                               ), arg1, arg2, arg3, arg4, arg5)

#define MASSERT6( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6) \
  TASSERT6 (assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6)                         ), arg1, arg2, arg3, arg4, arg5, arg6)

#define MASSERT7( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
  TASSERT7 (assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6, arg7)                   ), arg1, arg2, arg3, arg4, arg5, arg6, arg7)

#define MASSERT8( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
  TASSERT8 (assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)             ), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

#define MASSERT9( assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
  TASSERT9 (assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)       ), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

#define MASSERT10(assert_method, tag, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
  TASSERT10(assert_method, tag ": " STR(assert_method) STR((..., arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)), arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)

#endif /* ifndef TESTS_TESTING_H */

/*
 * opencurry: tests/testing.c
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
 *   - NULL
 */
#include <stddef.h>

/*
 * stdio.h:
 *  - FILE
 *  - fprintf
 *  - stdout
 *  - stderr
 */
#include <stdio.h>

/* stdlib.h:
 *   - malloc
 *   - calloc
 *   - free
 */
#include <stdlib.h>

#include "../base.h"
#include "testing.h"

/* ---------------------------------------------------------------- */
/* Types. */

unit_test_context_t *new_unit_test_context(int override_err_buf_len, int override_seed, int err_buf_len, int seed)
{
  unit_test_context_t *context;

  if(!override_err_buf_len) err_buf_len = DEFAULT_TEST_ERR_BUF_SIZE;
  if(!override_seed)        seed        = DEFAULT_TEST_SEED;


  context = calloc(1, sizeof(unit_test_context_t));

  context->err_buf_len = err_buf_len;
  context->err_buf = malloc(context->err_buf_len);
  context->err_buf[0] = 0;

  context->seed = seed;


  context->num_pass = 0;
  context->num_fail = 0;

  context->last_test = -1;
  context->last_pass = -1;
  context->last_fail = -1;

  return context;
}

void free_unit_test_context(unit_test_context_t *context)
{
  if(context)
  {
    if(context->err_buf)
    {
      free(context->err_buf);
      context->err_buf = (char *) NULL;
    }

    free(context);
  }
}

/* ---------------------------------------------------------------- */
/* Running tests. */

char default_test_group_name[] = DEFAULT_TEST_GROUP_NAME;
char default_test_name[]       = DEFAULT_TEST_NAME;

/* Run a test suite. */
int run_test_suite_num(const char *test_group_name, unit_test_t *tests, unsigned int num_tests)
{
  int result;
  unit_test_context_t *context;

  context = new_unit_test_context(0, 0, 0, 0);
  {
    result = run_tests_num(context, tests, num_tests, test_group_name);
    print_test_suite_result(context, result);
  } free_unit_test_context(context);

  return result;
}

/* Run a test suite. */
int run_test_suite(const char *test_group_name, unit_test_t *tests)
{
  int result;
  unit_test_context_t *context;

  context = new_unit_test_context(0, 0, 0, 0);
  {
    result = run_tests(context, tests, test_group_name);
    print_test_suite_result(context, result);
  } free_unit_test_context(context);

  return result;
}

void print_test_suite_result(const char *test_group_name, unit_test_context_t *context, int result)
{
  FILE *out;

  if(!(result == 0))
  {
    out = stdout;

    fprintf(out, "pass: %d/%d test groups.\n", (int) (context->num_pass), (int) (context->num_pass));
  }
  else
  {
    out = stderr;

    context->err_buf[context->err_buf_len-1] = 0;
    fprintf
      ( out
      , "Error: %d tests failed:\n  last failed test #: %d\n  number of tests run: %d\n  can continue testing after last failure?: %s\n\nLast error message:\n%s"
      , (int) (context->num_fail)
      , (int) (context->last_fail)
      , (int) (context->last_test)
      , (result >= 0) ? "yes" : "no (aborted)"
      , (const char *) context->err_buf
      );
  }
}

int run_tests(const char *test_group_name, unit_test_context_t *context, unit_test_t *tests)
{
  int result = 0;

  unit_test_t *test;

  for(test = tests; *test; ++test)
  {
    result |= run_test(context, *test);

    if(!(result >= 0))
    {
      break;
    }
  }

  return result;
}

int run_tests_num(const char *test_group_name, unit_test_context_t *context, unit_test_t *tests, unsigned int num_tests)
{
  int i;
  int result = 0;

  for(i = 0; i < num_tests; ++i)
  {
    result |= run_test(context, tests[i]);

    if(!(result >= 0))
    {
      break;
    }
  }

  return result;
}

int run_test(unit_test_context_t *context, unit_test_t test, const char *test_name)
{
  int result;

  result = (*test)(context);
  ++context->last_test;

  if(result == 0)
  {
    ++context->num_pass;
    context->last_pass = context->last_test;

    print_passed_test_result(context, result);
  }
  else
  {
    ++context->num_fail;
    context->last_fail = context->last_test;

    print_failed_test_result(context, result);
  }

  return result;
}

void print_passed_test_result(unit_test_context_t *context, int result, const char *test_name)
{
}

void print_failed_test_result(unit_test_context_t *context, int result, const char *test_name)
{
  context->err_buf[context->err_buf_len-1] = 0;
  fprintf(stderr, "FAIL: test group #%d failed:\n%s", (int) (context->last_fail), (const char *) context->err_buf);
}





/*
int run_tests(unit_test_t *tests)
{
  int result;

  unit_test_t *test;
  unit_test_context_t *context;

  context = new_unit_test_context(0, 0, 0, 0);
  {
    for(test = tests; *test; ++test)
    {
      result |= (*test)(err_buf, TEST_ERR_BUF_SIZE);

      if(!(result == 0))
      {
        err_buf[TEST_ERR_BUF_SIZE-1] = 0;
        break;

        fprintf(stderr, "FAIL: test group #%d failed:\n%s", (int) (test-tests), (const char *) err_buf);
        return result;
      }
    }

    if(!(result == 0))
    {
      fprintf(stderr, "FAIL: test group #%d failed:\n%s", (int) (test-tests), (const char *) err_buf);
    }
    else
    {
      fprintf(stdout, "pass: %d/%d test groups.\n", (int) (test-tests), (int) (test-tests));
    }

  } free_unit_test_context(context);

  return result;
}
*/

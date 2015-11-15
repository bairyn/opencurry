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
 *   - size_t
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
 *   - calloc
 *   - free
 *   - malloc
 */
#include <stdlib.h>

/* string.h:
 *   - memcmp
 *   - memcpy
 *   - strcmp
 *   - strncmp
 */
#include <string.h>

#include "../base.h"
#include "testing.h"

#include "../util.h"

/* ---------------------------------------------------------------- */
/* Types. */

/*
 * new_unit_test_context:
 *
 * Initialize a new "unit_test_context_t" structure.
 *
 * "new_unit_test_context_defaults" can
 * be used to initialize a "unit_test_context_t" value with default values.
 *
 *
 * This procedure prepares a "unit_test_context_t" for use, allocating memory
 * and setting values in the structure.
 *
 * Input:
 *   unit_test_context_t *initialize_context_noheap:
 *
 *     NULL:
 *       By default, a new "unit_test_context_t" will be allocated on the heap.
 *       Pass "NULL" for this behaviour.
 *
 *     non-NULL:
 *       This procedure can initialize an existing "unit_test_context_t".  If
 *       this argument is non-NULL, the procedure will behave the same, except
 *       it will use the existing structure rather than "malloc"ing a new one,
 *       (and "is_heap_allocated" will be set accordingly).
 *
 *       "free_unit_test_context" will check the value of "is_heap_allocated"
 *       to determine whether to call "free" on a "unit_test_context_t".
 *
 *       Note: re-initializing an existing "unit_test_context_t" results in
 *       undefined behaviour.  "initialize_context_noheap" should not already
 *       be initialized.
 *
 *   ...
 */
unit_test_context_t *new_unit_test_context
  ( unit_test_context_t *initialize_context_noheap

  , int override_err_buf_halfsize, int override_int_err_buf_size, int override_details_buf_size
  , int override_user_buf_size,    int override_misc_buf_size
  , int override_seed,             int override_out,              int override_err

  , size_t       err_buf_halfsize, size_t       int_err_buf_size, size_t       details_buf_size
  , size_t       user_buf_size,    size_t       misc_buf_size
  , unsigned int seed,             FILE        *out,              FILE        *err
  )
{
  size_t               size;
  unit_test_context_t *context;

  /* Set default arguments. */
  if (!override_err_buf_halfsize) err_buf_halfsize = DEFAULT_TEST_CONTEXT_ERR_BUF_HALFSIZE;
  if (!override_int_err_buf_size) int_err_buf_size = DEFAULT_TEST_CONTEXT_INT_ERR_BUF_SIZE;
  if (!override_details_buf_size) details_buf_size = DEFAULT_TEST_CONTEXT_DETAILS_BUF_SIZE;
  if (!override_user_buf_size)    user_buf_size    = DEFAULT_TEST_CONTEXT_USER_BUF_SIZE;
  if (!override_misc_buf_size)    misc_buf_size    = DEFAULT_TEST_CONTEXT_MISC_BUF_SIZE;
  if (!override_seed)             seed             = DEFAULT_TEST_CONTEXT_SEED;
  if (!override_out)              out              = DEFAULT_TEST_CONTEXT_OUT;
  if (!override_err)              err              = DEFAULT_TEST_CONTEXT_ERR;

  /* Allocate memory. */
  if (initialize_context_noheap)
  {
    context = initialize_context_noheap;
    context->is_heap_allocated = 0;
  }
  else
  {
    const size_t nmemb = 1;

    size  = sizeof(unit_test_context_t);

    context = calloc(nmemb, size);
    if (!context)
    {
      fprintf
        ( err
        , "new_unit_test_context:\n"
          "  Failed to allocate heap memory for new context.\n"
          "  Requested '%d'*'%d' bytes.\n"
        , (int) nmemb, (int) size);
      return NULL;
    }

    context->is_heap_allocated = 1;
  }

  context->is_initialized = CONTEXT_PARTIALLY_INITIALIZED;


  /* Allocate buffers. */
  context->err_buf_halfsize = err_buf_halfsize;
  context->int_err_buf_size = int_err_buf_size;
  context->details_buf_size = details_buf_size;
  context->user_buf_size    = user_buf_size;
  context->misc_buf_size    = misc_buf_size;

  size = 2 * context->err_buf_halfsize;
  context->err_buf = malloc(size);
  if(!context->err_buf)
  {
    if (context->is_heap_allocated) free(context);

    fprintf
      ( err
      , "new_unit_test_context:\n"
        "  Failed to allocate heap memory for err_buf.\n"
        "  Requested '%d' bytes.\n"
      , (int) size);
    return NULL;
  }
  else
  {
    context->err_buf_is_heap = 1;
  }

  size = context->int_err_buf_size;
  context->int_err_buf = malloc(size);
  if(!context->int_err_buf)
  {
    free(context->err_buf);

    if (context->is_heap_allocated) free(context);

    fprintf
      ( err
      , "new_unit_test_context:\n"
        "  Failed to allocate heap memory for int_err_buf.\n"
        "  Requested '%d' bytes.\n"
      , (int) size);
    return NULL;
  }
  else
  {
    context->int_err_buf_is_heap = 1;
  }

  size = context->details_buf_size;
  context->details_buf = malloc(size);
  if(!context->details_buf)
  {
    free(context->err_buf);
    free(context->int_err_buf);

    if (context->is_heap_allocated) free(context);

    fprintf
      ( err
      , "new_unit_test_context:\n"
        "  Failed to allocate heap memory for details_buf.\n"
        "  Requested '%d' bytes.\n"
      , (int) size);
    return NULL;
  }
  else
  {
    context->details_buf_is_heap = 1;
  }

  size = context->user_buf_size;
  context->user_buf = malloc(size);
  if(!context->user_buf)
  {
    free(context->err_buf);
    free(context->int_err_buf);
    free(context->details_buf);

    if (context->is_heap_allocated) free(context);

    fprintf
      ( err
      , "new_unit_test_context:\n"
        "  Failed to allocate heap memory for user_buf.\n"
        "  Requested '%d' bytes.\n"
      , (int) size);
    return NULL;
  }
  else
  {
    context->user_buf_is_heap = 1;
  }

  size = context->misc_buf_size;
  context->misc_buf = malloc(size);
  if(!context->misc_buf)
  {
    free(context->err_buf);
    free(context->int_err_buf);
    free(context->details_buf);
    free(context->user_buf);

    if (context->is_heap_allocated) free(context);

    fprintf
      ( err
      , "new_unit_test_context:\n"
        "  Failed to allocate heap memory for misc_buf.\n"
        "  Requested '%d' bytes.\n"
      , (int) size);
    return NULL;
  }
  else
  {
    context->misc_buf_is_heap = 1;
  }

  /* Set up environment. */
  context->err_buf    [0]                             = 0;
    context->err_buf    [context->err_buf_halfsize + 0] = 0;
  context->int_err_buf[0]                             = 0;
  context->details_buf[0]                             = 0;
  /*
  (void *) context->user_buf
  (void *) context->misc_buf
  */

  context->err_buf_len           = 0;
  context->err_buf_first_err_len = 0;
  context->int_err_buf_len       = 0;
  context->details_buf_len       = 0;

  context->is_snprintf_err = 0;


  context->seed           = seed;
  context->seed_initial   = seed;
  context->seed_first_err_start = -1;
  context->seed_first_err_end   = -1;

  context->out            = out;
  context->err            = err;

  context->free           = free_unit_test_context;

  /* Set up initial state. */
  context->aborting       = 0;

  context->group_depth    = 0;

  context->next_test_id   = 1;

  context->num_pass       = 0;
  context->num_fail       = 0;
  context->num_skip       = 0;

  context->last_pass      = -1;
  context->last_fail      = -1;
  context->last_skip      = -1;

  context->first_fail     = -1;

  /* Set metadata. */
  context->is_initialized = CONTEXT_FULLY_INITIALIZED;

  /* Insert precautionary NULL terminators at the end of each buffer. */
  if (context->err_buf_halfsize > 0)
  {
    context->err_buf[context->err_buf_halfsize - 1] = 0;
      context->err_buf[context->err_buf_halfsize + context->err_buf_halfsize - 1] = 0;
  }
  if (context->int_err_buf_size > 0)
  {
    context->int_err_buf[context->int_err_buf_size - 1] = 0;
  }
  if (context->details_buf_size > 0)
  {
    context->details_buf[context->details_buf_size - 1] = 0;
  }

  /* Return object. */
  return context;
}

unit_test_context_t *new_unit_test_context_defaults(unit_test_context_t *initialize_context_noheap)
{
  return
    new_unit_test_context
      ( initialize_context_noheap

      , 0,                                     0,                                     0
      , 0,                                     0
      , 0,                                     0,                                     0

      , DEFAULT_TEST_CONTEXT_ERR_BUF_HALFSIZE, DEFAULT_TEST_CONTEXT_INT_ERR_BUF_SIZE, DEFAULT_TEST_CONTEXT_DETAILS_BUF_SIZE
      , DEFAULT_TEST_CONTEXT_USER_BUF_SIZE,    DEFAULT_TEST_CONTEXT_MISC_BUF_SIZE
      , DEFAULT_TEST_CONTEXT_SEED,             DEFAULT_TEST_CONTEXT_OUT,              DEFAULT_TEST_CONTEXT_ERR
      );
}

void free_unit_test_context(unit_test_context_t *context)
{
  if (context)
  {
    /* Already uninitialized? */
    if (context->is_initialized)
    {
      const int partially_initialized = context->is_initialized == CONTEXT_PARTIALLY_INITIALIZED;

      /* Does this context have a deinitialization procedure that differs from
       * this one?
       */
      if (partially_initialized || !context->free || context->free == free_unit_test_context)
      {
        if (context->err_buf)
        {
          free(context->err_buf);
          context->err_buf = (char *) NULL;
        }

        if (context->int_err_buf)
        {
          free(context->int_err_buf);
          context->err_buf = (char *) NULL;
        }

        if (context->details_buf)
        {
          free(context->details_buf);
          context->details_buf = (char *) NULL;
        }

        if (context->user_buf)
        {
          free(context->user_buf);
          context->user_buf = (void *) NULL;
        }

        if (context->misc_buf)
        {
          free(context->misc_buf);
          context->misc_buf = (void *) NULL;
        }

        context->is_initialized = 0;

        if (context->is_heap_allocated)
        {
          context->is_heap_allocated = 0;

          free(context);
        }
      }
      else
      {
        context->free(context);
      }
    }
  }
}

/* ---------------------------------------------------------------- */
/* Running tests. */

/* Run a unit test independently, with a new unit test context state. */
int run_test_suite(unit_test_t test)
{
  unit_test_context_t  test_suite_context;

  unit_test_result_t   result;
  unit_test_context_t *context;

  context = new_unit_test_context_defaults(&test_suite_context);
  {
    if (!context)
    {
      fprintf
        ( stderr
        , "run_test_suite: Error initializing new unit test context.\n"
        );

      return UNIT_TEST_INTERNAL_ERROR;
    }

    result = run_test_suite_with_context(context, test);
  } free_unit_test_context(context);

  return (int) result;
}

/* Run a unit test independently, with its unit test context already provided.
 *
 * The context is not freed upon exit!
 */
int run_test_suite_with_context(unit_test_context_t *context, unit_test_t test)
{
  unit_test_result_t result;

  result = run_test(context, test);
  print_test_suite_result(context, result);

  return result;
}

void print_test_suite_result(unit_test_context_t *context, unit_test_result_t result)
{
  const static int print_last_instead_of_first_error = 0;

  FILE *out;

  if
    (  is_test_result_success(result)
    && context->num_skip == 0
    && context->num_fail == 0
    )
  {
    out = context->out;

    fprintf
      ( out
      , "\n"
        "pass: %d/%d tests and test groups.\n"

      , (int) (context->num_pass)
      , (int) (context->num_pass + context->num_skip)
      );
  }
  else if
    (  is_test_result_success(result)
    || is_test_result_skip   (result)
    )
  {
    if (is_test_result_can_continue(result))
    {
      out = context->out;

      fprintf
        ( out
        , "\n"
          "pass: %d/%d tests and test groups.\n"
          "*** skipped: %d/%d tests and test groups.\n"

        , (int) (context->num_pass)
        , (int) (context->num_pass + context->num_skip)

        , (int) (context->num_skip)
        , (int) (context->num_pass + context->num_skip)
        );
    }
    else
    {
      out = context->out;

      fprintf
        ( out
        , "\n"
          "pass: %d tests and test groups.\n"
          "*** skipped: %d tests and test groups.\n"
          "*** Error: skipped test signaled testing can't continue.\n"

        , (int) (context->num_pass)

        , (int) (context->num_skip)
        );
    }
  }
  else
  {
    out = context->err;


    ensure_buf_limits(context, context->err_buf);

    context->err_buf[context->err_buf_halfsize - 1] = 0;
    if (print_last_instead_of_first_error)
    {
      fprintf
        ( out
        , "\n"
          "*** Error: %d tests failed:\n  last failed test #: %d\n  number of tests run: %d\n  can continue testing after last failure?: %s\n\nLast error message%s:\n\n%s%s"

        , (int) (context->num_fail)
        , (int) (context->last_fail)
        , (int) (context->next_test_id - context->num_skip)
        , (result >= 0) ? "yes" : "no (aborted)"
        ,   (!(context->err_buf_len >= context->err_buf_halfsize - 1))
          ? ""
          : " (error message buffer maxed out; additional text might be truncated)"
        , (const char *) context->err_buf
        , context->err_buf_len >= 1 && context->err_buf[context->err_buf_len - 1] == '\n' ? "" : "\n"
        );
    }
    else
    {
      fprintf
        ( out
        , "\n"
          "*** Error: %d tests failed:\n"
          "  can continue testing after last failure?: %s\n"
          "  first failed test #: %d\n"
          "  number of tests run: %d\n"
          "\n"
          "First error message%s: %s%s"

        , (int) (context->num_fail)
        , (result >= 0) ? "yes" : "no (aborted)"

        , (int) (context->first_fail)
        , (int) (context->next_test_id - context->num_skip)
        ,   (!(context->err_buf_len >= context->err_buf_halfsize - 1))
          ? ""
          : " (error message buffer maxed out; additional text might be truncated)"
        , (const char *) context->err_buf + context->err_buf_halfsize
        , context->err_buf_first_err_len >= 1 && context->err_buf[context->err_buf_halfsize + context->err_buf_first_err_len - 1] == '\n' ? "" : "\n"
        );
    }
  }
}


int is_test_result_success(unit_test_result_t result)
{
  return result == UNIT_TEST_PASS;
}

int is_test_result_failure(unit_test_result_t result)
{
  return result == UNIT_TEST_FAIL || result == UNIT_TEST_FAIL_CONTINUE;
}

int is_test_result_skip(unit_test_result_t result)
{
  return result == UNIT_TEST_SKIPPED || result == UNIT_TEST_SKIPPED_CONTINUE;
}

int is_test_result_can_continue(unit_test_result_t result)
{
  return result >= 0;
}

int is_test_result_aborting(unit_test_result_t result)
{
  return result <  0;
}


/* Run a "unit_test_t", updating the context state and emitting output. */
unit_test_result_t run_test(unit_test_context_t *context, unit_test_t test)
{
  int                id;
  unit_test_result_t result;

  unsigned int       seed_start;

  /* Set the "id" of this test. */
  id = context->next_test_id++;

  seed_start = context->seed;

  /* Print the id and name of the test. */
  print_test_prefix(context, test, id);

  /* Reset error and details buffers, run, and obtain result. */
  reset_err_buf(context);
  reset_err_msg_details(context);
  /* Call the test procedure. */

  /* This might recursively call child tests via "run_test*" procedures
   * provided by this API.
   */
  result = test.run(context);

  /* ---------------------------------------------------------------- */

  /* This test, and all child tests, are done. */

  /* Update context state. */
  process_test_result(context, test, id, result, seed_start);

  /* Print the result. */
  print_test_result(context, test, id, result, seed_start);
  reset_err_buf(context);

  /* ---------------------------------------------------------------- */

  /* If a unit test is skipped, reset the result. */

  if (is_test_result_can_continue(result))
  {
    if (!is_test_result_success(result) && !is_test_result_failure(result))
    {
      if (is_test_result_skip(result))
      {
        result = UNIT_TEST_PASS;
      }
    }
  }

  return result;
}

/* Update "context" state reflect the result of the unit test. */
void process_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start)
{
  if (result == UNIT_TEST_INTERNAL_ERROR)
  {
    context_internal_error(context, "process_test_result: result is UNIT_TEST_INTERNAL_ERROR.");
  }

  /* Update result-type state. */
  if      (is_test_result_success(result))
  {
    /* Test passed. */

    ++context->num_pass;
    context->last_pass = id;
  }
  else if (is_test_result_skip(result))
  {
    /* Test skipped. */

    ++context->num_skip;
    context->last_skip = id;
  }
  else
  {
    /* Test failed. */

    int is_first_failure = context->num_fail == 0;

    ++context->num_fail;
    context->last_fail = id;

    if (is_first_failure)
    {
      process_first_test_failure(context, test, id, result, seed_start);
    }
  }
}

void process_first_test_failure(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start)
{
  /* ID. */
  context->first_fail            = id;

  /* seed. */
  context->seed_first_err_start  = seed_start;
  context->seed_first_err_end    = context->seed;

  /* error buf. */
  ensure_buf_limits(context, context->err_buf);
  memcpy(context->err_buf + context->err_buf_halfsize, context->err_buf, context->err_buf_len);
  context->err_buf_first_err_len = context->err_buf_len;
}

/*
 * Prints context->group_depth * "| ".
 *
 * When no_pending_text, omits final whitespace character.
 *
 * When alert, omit the first 2 '|' characters, and always prints first 3
 * characters as "***".
 */
void print_test_indent(unit_test_context_t *context, int alert, int no_pending_text)
{
  int i;

  if (!no_pending_text)
  {
    if (!alert)
    {
      /* Case 1: basic. */

      /* group_depth times print "| " */

      for (i = 0; i < context->group_depth; ++i)
      {
        fprintf(context->out, "| ");
      }
    }

    else
    {
      /* Case 2: alert. */

      /* 1) group_depth times print "| "    */
      /*                                    */
      /* 2) Overwrite beginning with "*** " */

      fprintf(context->out, "*** ");

      for (i = 2; i < context->group_depth; ++i)
      {
        fprintf(context->out, "| ");
      }
    }
  }

  else
  {
    if (!alert)
    {
      /* Case 3: no trailing whitespace. */

      /* 1) group_depth times print "| "    */
      /*                                    */
      /* 2) Remove trailing whitespace.     */

      for (i = 0; i < context->group_depth; ++i)
      {
        if (i < 1)
          fprintf(context->out, "|");
        else
          fprintf(context->out, " |");
      }
    }

    else
    {
      /* Case 4: alert, no trailing whitespace. */

      /* 1) group_depth times print "| "    */
      /*                                    */
      /* 2) Overwrite beginning with "*** " */
      /*                                    */
      /* 3) Remove trailing whitespace.     */

      fprintf(context->out, "***");

      for (i = 2; i < context->group_depth; ++i)
      {
        fprintf(context->out, " |");
      }
    }
  }
}

void print_test_prefix(unit_test_context_t *context, unit_test_t test, int id)
{
  if (context->aborting)
  {
    /* We've already printed a failure message for a test that aborted, so omit
     * printing the output for parent tests.
     */
    return;
  }

  print_test_indent(context, TEST_INDENT_NO_ALERT, TEST_INDENT_PENDING_TEXT);

  #if IS_FALSE(TEST_RESULT_PASS_PRINT_SAME_LINE)
    fprintf(context->out, "- %d: %s:\n", id, test.name);
  #else
    fprintf(context->out, "- %d: %s:",   id, test.name);
  #endif
}

void print_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start)
{
  if (context->aborting)
  {
    /* We've already printed a failure message for a test that aborted, so omit
     * printing the output for parent tests.
     */
    return;
  }

  if (is_test_result_aborting(result))
  {
    /* We'll abort after this test, so set "aborting" so we know not to print
     * more test results, for the parents that will probably also fail.
     */
    context->aborting = 1;
  }

  if      (is_test_result_success(result))
  {
    print_passed_test_result (context, test, id, result, seed_start);
  }
  else if (is_test_result_skip(result))
  {
    print_skipped_test_result(context, test, id, result, seed_start);
  }
  else
  {
    print_failed_test_result (context, test, id, result, seed_start);
  }
}

void print_passed_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start)
{
  int ran_test_group;

  ran_test_group = context->next_test_id != (id + 1);

  /* #if IS_FALSE(TEST_RESULT_PASS_PRINT_SAME_LINE) */
  if
    (  IS_FALSE(TEST_RESULT_PASS_PRINT_SAME_LINE)
    || ran_test_group
    )
  {
    print_test_indent(context, TEST_INDENT_NO_ALERT, TEST_INDENT_PENDING_TEXT);

    #if IS_FALSE(TEST_RESULT_GROUP_REPEAT_ID)
    {
      #if IS_FALSE(TEST_RESULT_PASS_PRINT_DESCRIPTION)
        fprintf(context->out, "  %*.s   =)\n", 2 * context->group_depth, "");
      #else
        fprintf(context->out, "  %*.s   =): pass: %s\n", 2 * context->group_depth, "", test.description);
      #endif
    }

    #else
    {
      #if IS_FALSE(TEST_RESULT_PASS_PRINT_DESCRIPTION)
        fprintf(context->out, "  %*.s   =) #%d\n", 2 * context->group_depth, "", (int) id);
      #else
        fprintf(context->out, "  %*.s   =) #%d: pass: %s\n", 2 * context->group_depth, "", (int) id, test.description);
      #endif
    }
    #endif
  }
  else
  {
    #if IS_FALSE(TEST_RESULT_PASS_PRINT_DESCRIPTION)
      fprintf(context->out, " =)\n");
    #else
      fprintf(context->out, " =): pass: %s\n", test.description);
    #endif
  }
}

void print_failed_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start)
{
  int can_continue;

  ensure_buf_limits(context, context->err_buf);
  context->err_buf[context->err_buf_halfsize - 1] = 0;

  can_continue = is_test_result_can_continue(result);

  #if IS_TRUE(TEST_RESULT_PASS_PRINT_SAME_LINE)
    fprintf(context->out, "\n");
  #endif

  print_test_indent(context, TEST_INDENT_ALERT, TEST_INDENT_PENDING_TEXT);
  fprintf(context->out, "  %*.s   FAILURE - %s\n", 2 * context->group_depth, "", test.description);

  fprintf(context->err, "/----------------------------------------------------------------\n");
  fprintf(context->err, "FAILURE:\n");
  fprintf(context->err, "  test number:        %d\n", id);
  fprintf(context->err, "  test name:          %s\n", test.name);
  fprintf(context->err, "  test description:   %s\n", test.description);
  fprintf(context->err, "  test result code:   %d\n", (int) result);
  fprintf(context->err, "  seed at test start: %d\n", (int) seed_start);
  fprintf(context->err, "  can continue?:      %s\n", (can_continue) ? "yes" : "no (aborting!)");
  fprintf(context->err, "\n");
  if (!(context->err_buf_len >= context->err_buf_halfsize - 1))
    fprintf(context->err, "Error message:\n");
  else
    fprintf(context->err, "Error message(buffer maxed out; additional text might be truncated):\n");
  fprintf(context->err, "\n");
  fprintf(context->err, "%s", (const char *) context->err_buf);
  if (!(context->err_buf_len >= 1 && context->err_buf[context->err_buf_len - 1] == '\n'))
    fprintf(context->err, "\n");
  fprintf(context->err, "\\----------------------------------------------------------------\n\n");
}

void print_skipped_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result, unsigned int seed_start)
{
  #if IS_TRUE(TEST_RESULT_PASS_PRINT_SAME_LINE)
    fprintf(context->out, "\n");
  #endif

  print_test_indent(context, TEST_INDENT_ALERT, TEST_INDENT_PENDING_TEXT);

  #if IS_FALSE(TEST_RESULT_SKIP_PRINT_DESCRIPTION)
    fprintf(context->out, "  %*.s   SKIPPED\n", 2 * context->group_depth, "");
  #else
    fprintf(context->out, "  %*.s   SKIPPED: %s\n", 2 * context->group_depth, "", test.description);
  #endif
}

void ensure_buf_limits(unit_test_context_t *context, char *buf)
{
  size_t  size;
  size_t *len;

  if      (!buf)
  {
    ensure_buf_limits(context, context->err_buf);
    ensure_buf_limits(context, context->err_buf + context->err_buf_halfsize);
    ensure_buf_limits(context, context->int_err_buf);
    ensure_buf_limits(context, context->details_buf);

    ensure_buf_limits(context, context->user_buf);
    ensure_buf_limits(context, context->misc_buf);

    return;
  }
  else if (buf == context->err_buf)
  {
    buf  =  context->err_buf;
    size =  context->err_buf_halfsize;
    len  = &context->err_buf_len;
  }
  else if (buf == context->err_buf + context->err_buf_halfsize)
  {
    buf  =  context->err_buf + context->err_buf_halfsize;
    size =  context->err_buf_halfsize;
    len  = &context->err_buf_first_err_len;
  }
  else if (buf == context->int_err_buf)
  {
    buf  =  context->details_buf;
    size =  context->details_buf_size;
    len  = &context->details_buf_len;
  }
  else if (buf == context->details_buf)
  {
    buf  =  context->details_buf;
    size =  context->details_buf_size;
    len  = &context->details_buf_len;
  }
  else if (buf == context->user_buf)
  {
    return;
  }
  else if (buf == context->misc_buf)
  {
    return;
  }
  else
  {
    context_internal_error
      ( context
      , "ERROR: ensure_buf_limits called with non-NULL \"buf\" argument that isn't one of \"context\"'s buffers!"
      );

    return;
  }

  if (*len >= size)
  {
    *len = size - 1;
  }
  buf[*len] = 0;
}

size_t test_add_internal_error_msg(unit_test_context_t *context, const char *msg)
{
  int    l;

  ensure_buf_limits(context, context->int_err_buf);

  l = snprintf
    ( (char *) (context->int_err_buf + context->int_err_buf_len), (size_t) terminator_size(size_minus(context->int_err_buf_size, context->int_err_buf_len))
    , "%s"
    , msg
    );
  if (l < 0) return (context->int_err_buf_len = assert_msg_check_snprintf(context, l, context->int_err_buf, context->int_err_buf_size, "(context_internal_error)", &context->is_snprintf_err));

  return (context->int_err_buf_len = (size_t) l);
}

void context_internal_error(unit_test_context_t *context, const char *msg)
{
  test_add_internal_error_msg(context, msg);

  context->aborting = 1;

  fprintf
    ( context->err
    , "\n"
      "\n"
      "****************************************************************\n"
      "context_internal_error: an internal error occurred:\n"
      "\n"
      "%s\n"
      "****************************************************************\n"
    , msg
    );
}


unit_test_result_t run_tests_num(unit_test_context_t *context, unit_test_t **tests, size_t num_tests)
{
  int                i;
  int                aborting = 0;
  unit_test_result_t result_sum = UNIT_TEST_PASS;

  ++context->group_depth;
  for (i = 0; i < num_tests; ++i)
  {
    int individual_result;

#if IS_TRUE(TEST_RESULT_PASS_PRINT_SAME_LINE)
    if (i == 0)
    {
      fprintf(context->out, "\n");
    }
#endif

#if IS_TRUE(TEST_RESULT_GROUP_PRINT_INTERSPERSED_LINE)
    if (i >= 1)
    {
      /* Add extra line. */
      print_test_indent(context, TEST_INDENT_NO_ALERT, TEST_INDENT_NO_PENDING_TEXT);
      fprintf(context->out, "\n");
    }
#endif

    individual_result = run_test(context, *tests[i]);

    aborting = is_test_result_aborting(individual_result) || is_test_result_aborting(result_sum);

    result_sum |= individual_result;

    if (aborting)
    {
      break;
    }
  }
  --context->group_depth;

  return result_sum;
}

unit_test_result_t run_tests(unit_test_context_t *context, unit_test_t **tests)
{
  unit_test_t **end;

  if (!tests)
    return UNIT_TEST_INTERNAL_ERROR;

  for (end = tests; *end; ++end);

  return run_tests_num(context, tests, end - tests);
}

/* ---------------------------------------------------------------- */
/* Default error messages for assertion failures. */

/* Clear the error buffer.
 *
 * This is called on each invocation of "run_test", immediately before the test
 * procedure is invoked.
 */
void reset_err_buf(unit_test_context_t *context)
{
  context->err_buf[0] = 0;
  context->err_buf_len = 0;
}

/* Clear the details buffer.
 *
 * This is called on each invocation of "run_test", immediately before the test
 * procedure is invoked.
 */
void reset_err_msg_details(unit_test_context_t *context)
{
  context->details_buf[0] = 0;
  context->details_buf_len = 0;
}

/* Append to the details buffer. */
/* TODO: varags */
size_t test_add_details_msg(unit_test_context_t *context, const char *msg)
{
  int    l;

  if (context->details_buf_len >= context->details_buf_size)
  {
    context->details_buf_len = context->details_buf_size - 1;
  }

  l = snprintf
    ( (char *) (context->details_buf + context->details_buf_len), (size_t) terminator_size(size_minus(context->details_buf_size, context->details_buf_len))
    , "%s"
    , msg
    );
  if (l < 0) return (context->details_buf_len = assert_msg_check_snprintf(context, l, context->details_buf, context->details_buf_size, "(test_add_details_msg)", &context->is_snprintf_err));

  return (context->details_buf_len = (size_t) l);
}

/*
 * These procedures return the number of characters written, excluding the NULL
 * terminating character.
 *
 * In some cases of errors with "snprintf", more bytes than the 
 */

/*
 * Error handling for "snprintf".
 *
 * Call with the return value of "snprintf" to handle "snprintf" errors.
 *
 * On success, the original "snprintf_result" result is simply returned.  This
 * value represents the number of bytes written, excluding the NULL terminator.
 * "out_is_err" is set to 0 if a reference provided.
 *
 * On failure, a new descriptive error message is written instead, with the
 * number of bytes written returned, and "out_is_err" if provided as a non-NULL
 * pointer is set to 1.
 */
size_t assert_msg_check_snprintf(unit_test_context_t *context, int snprintf_result, char *msg_out, size_t msg_out_size, const char *tag, int *out_is_err)
{
  if (!(snprintf_result < 0))
  {
    if (out_is_err) *out_is_err = 0;
    return (size_t) snprintf_result;
  }
  else
  {
    char   *buf;

    int     len;
    size_t  size_terminator;

    if (out_is_err) *out_is_err = 1;

    size_terminator = msg_out_size - 1;

    buf = malloc(msg_out_size);

    if (!buf)
    {
      const char *err_msg =
        "ERROR: assert_msg_check_snprintf: out of memory: failed to allocate memory to generate message.\n";

      strncpy
        ( msg_out
        , err_msg
        , size_terminator
        );

      return (size_t) strlen(err_msg);
    }

    strncpy(buf, msg_out, size_terminator);

    len = snprintf
      ( (char *) msg_out, (size_t) size_terminator
      , "assert_msg_check_snprintf:\n"
        "  Error generating error message!\n"
        "  A call to \"snprintf\" failed with error code '%d'.\n"
        "\n"
        "  %s%s.\n"
        "\n"
        "Original error buffer, with trailing newline:\n"
        "%s\n"
      , snprintf_result
      , tag ? "tag: " : "(no tag provided)"
      , tag ? tag     : ""
      , buf
      );

    if (len < 0)
    {
      const char *err_msg =
        "ERROR: assert_msg_check_snprintf: failed to generate message explaining a failure to generate a message.\n";

      strncpy
        ( msg_out
        , err_msg
        , size_terminator
        );

      return (size_t) strlen(err_msg);
    }

    return (size_t) len;
  }
}

/*
 * Append the details buffer if it's not empty with a preceding newline
 * character to the context's error buffer.
 *
 * Keeps the first "len" bytes in the error buffer.
 *
 * Returns the new total number of bytes written.
 *
 * (If the error message buffer lacks a trailing newline character, one is
 * added automatically.)
 */
size_t assert_msg_append_details(unit_test_context_t *context, size_t len, char *msg_out, size_t msg_out_size, const char *tag)
{
  int    l;

  if (!context->details_buf_len)
  {
    return len;
  }

  /* Ensure buffer is null terminated. */
  context->details_buf[context->details_buf_size - 1] = 0;

  /* Ensure len is no out of bounds.               */
  /* Also ensure 1 extra byte for NULL terminator. */
  if (context->details_buf_len >= context->details_buf_size)
  {
    context->details_buf_len = context->details_buf_size - 1;
  }
  context->details_buf[context->details_buf_len] = 0;

  l = snprintf
    ( (char *) (msg_out + len), (size_t) (terminator_size(size_minus(msg_out_size, len)))
    , "\n\n%s"
    , context->err_buf_len >= 1 && context->err_buf[context->err_buf_len - 1] == '\n' ? "" : "\n"
    , (const char *) context->details_buf
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  len += l;

  return (size_t) len;
}

size_t assert_failure_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Assertion '%s' failed - no details provided.\n"
    , (const char *) tag
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_true_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int condition)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Boolean assertion '%s' failed - the condition be true, but it is false: %d (bool: %d)\n"
    , (const char *) tag
    , (int) condition
    , (int) !!condition
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_inteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int check, int model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Assertion '%s' failed - integers must be equal, but differ:\n  should be:   % d\n  actually is: % d\n"
    , (const char *) tag
    , (int) model
    , (int) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_uinteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, unsigned int check, unsigned int model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Assertion '%s' failed - unsigned integers must be equal, but differ:\n  should be:   % u\n  actually is: % u\n"
    , (const char *) tag
    , (unsigned int) model
    , (unsigned int) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_ulongeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, unsigned long check, unsigned long model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Assertion '%s' failed - unsigned long integers must be equal, but differ:\n  should be:   %lu\n  actually is: %lu\n"
    , (const char *) tag
    , (unsigned long) model
    , (unsigned long) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_sizeeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, size_t check, size_t model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Assertion '%s' failed - size_t integers must be equal, but differ:\n  should be:   %zu\n  actually is: %zu\n"
    , (const char *) tag
    , (size_t) model
    , (size_t) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_objpeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const void *check, const void *model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Assertion '%s' failed - object pointers must be equal, but differ:\n  should be:   %p\n  actually is: %p\n"
    , (const char *) tag
    , (void *) model
    , (void *) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_funpeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, tests_funp_t check, tests_funp_t model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Assertion '%s' failed - function pointers must be equal, but differ:\n  should be:   %p\n  actually is: %p\n"
    , (const char *) tag
    , (void *) *((void **) &model)
    , (void *) *((void **) &check)
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_streqz_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char *check, const char *model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Assertion '%s' failed - strings must be equal, but differ:\n  should be:   %s\n  actually is: %s\n"
    , (const char *) tag
    , (const char*) model
    , (const char*) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_streqn_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char *check, const char *model, size_t max_len)
{
  int     l;

  char   *checkz, *modelz;

  checkz = malloc(2 * (max_len+1));
  {
    modelz = checkz + (max_len+1);

    strncpy(checkz, check, max_len);
    strncpy(modelz, model, max_len);
    checkz[max_len] = 0;
    modelz[max_len] = 0;

    l = snprintf
      ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
      , "Assertion '%s' failed - strings must be equal, but differ:\n  should be:   %s\n  actually is: %s\n"
      , (const char *) tag
      , (const char *) modelz
      , (const char *) checkz
      );
  } free(checkz);
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_memeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, void *check, void *model, size_t n)
{
  int              l;

  int              i, j;
  size_t           written = 0;

  int              cols_printed;
  int              hexdump_indent_spaces;
  int              need_multiple_lines = 0;

  static const int byte_print_width    = sizeof(" 0x00") - 1;
  static const int width               = ASSERT_MSG_WIDTH;

  /* ---------------------------------------------------------------- */
  /* Start writing error message. */
  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "Assertion '%s' failed - '%d' bytes of memory must be equal, but differ:"
    , (const char *) tag
    , (int) n
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  /* ---------------------------------------------------------------- */
  /* Hex dump of model: write what the memory should be equal to. */

  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "\n"
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  l = hexdump_indent_spaces = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "  should be:"  /* A: Print two extra spaces below only without a newline. */
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  hexdump_indent_spaces += 2;

  /* Here we also calculate whether this will span multiple lines, in which
   * case the hex dump starts on its own line for readability.
   */
  if (hexdump_indent_spaces + byte_print_width * n >= width)
  {
    /* The hexdump will span multiple lines, so start it on its own line. */
    /* It'll be indented by two extra spaces, one of which we write here. */
    need_multiple_lines = 1;

    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "\n"
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;

    l = hexdump_indent_spaces = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "    "
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }
  else
  {
    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "  "  /* A: No newline, so print two extra spaces for alignment. */
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }


  /* Hex dump: print each byte. */
  cols_printed = hexdump_indent_spaces;
  for (i = 0; i < n; ++i)
  {
    cols_printed += byte_print_width;
    if (cols_printed >= width)
    {
      /* Start a new line. */
      cols_printed = hexdump_indent_spaces + byte_print_width;

      l = snprintf
        ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
        , "\n"
        );
      if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
      written += l;

      for (j = 0; j < hexdump_indent_spaces; ++j)
      {
        l = snprintf
          ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
          , " "
          );
        if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
        written += l;
      }
    }

    /* Print the byte. */
    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , " 0x%.2X"
      , (unsigned int) (((unsigned char *) model)[i])
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }

  /* ---------------------------------------------------------------- */
  /* Hex dump of check: write what the memory should be equal to. */

  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "\n"
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "  actually is:"
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  /* Should we start a new line? */
  if (need_multiple_lines)
  {
    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "\n"
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;

    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "    "
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }

  /* Hex dump: print each byte. */
  cols_printed = hexdump_indent_spaces;
  for (i = 0; i < n; ++i)
  {
    cols_printed += byte_print_width;
    if (cols_printed >= width)
    {
      /* Start a new line. */
      cols_printed = hexdump_indent_spaces + byte_print_width;

      l = snprintf
        ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
        , "\n"
        );
      if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
      written += l;

      for (j = 0; j < hexdump_indent_spaces; ++j)
      {
        l = snprintf
          ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
          , " "
          );
        if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
        written += l;
      }
    }

    /* Print the byte. */
    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , " 0x%.2X"
      , (unsigned int) (((unsigned char *) check)[i])
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }

  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "\n"
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  return assert_msg_append_details(context, (size_t) written, msg_out, msg_out_size, tag);
}


size_t assert_false_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int condition)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Inverse boolean assertion '%s' failed - the condition be false, but it is true: %d (bool: %d)\n"
    , (const char *) tag
    , (int) condition
    , (int) !!condition
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_inteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, int check, int model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Inverse assertion '%s' failed - integers must differ, but they are the same:\n  should differ from:  % d\n  but still is:        % d\n"
    , (const char *) tag
    , (int) model
    , (int) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_uinteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, unsigned int check, unsigned int model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Inverse assertion '%s' failed - unsigned integers must differ, but they are the same:\n  should differ from:  % d\n  but still is:        % d\n"
    , (const char *) tag
    , (unsigned int) model
    , (unsigned int) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_ulongeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, unsigned long check, unsigned long model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Inverse assertion '%s' failed - unsigned long integers must differ, but they are the same:\n  should differ from:  %lu\n  but still is:        %lu\n"
    , (const char *) tag
    , (unsigned long) model
    , (unsigned long) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_sizeeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, size_t check, size_t model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Inverse assertion '%s' failed - size_t integers must differ, but they are the same:\n  should differ from:  %zu\n  but still is:        %zu\n"
    , (const char *) tag
    , (size_t) model
    , (size_t) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_objpeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const void *check, const void *model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Inverse assertion '%s' failed - object pointers must differ, but they are the same:\n  should differ from:  %p\n  but still is:        %p\n"
    , (const char *) tag
    , (void *) model
    , (void *) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_funpeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, tests_funp_t check, tests_funp_t model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Inverse assertion '%s' failed - function pointers must differ, but they are the same:\n  should differ from:  %p\n  but still is:        %p\n"
    , (const char *) tag
    , (void *) *((void **) &model)
    , (void *) *((void **) &check)
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_streqz_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char *check, const char *model)
{
  int    l;

  l = snprintf
    ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
    , "Inverse assertion '%s' failed - strings must duffer, but they are the same:\n  should differ from:  %s\n  but still is:        %s\n"
    , (const char *) tag
    , (const char*) model
    , (const char*) check
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_streqn_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, const char *check, const char *model, size_t max_len)
{
  int     l;

  char   *checkz, *modelz;

  checkz = malloc(2 * (max_len+1));
  {
    modelz = checkz + (max_len+1);

    strncpy(checkz, check, max_len);
    strncpy(modelz, model, max_len);
    checkz[max_len] = 0;
    modelz[max_len] = 0;

    l = snprintf
      ( (char *) msg_out, (size_t) terminator_size(msg_out_size)
      , "Inverse assertion '%s' failed - strings must duffer, but they are the same:\n  should differ from:  %s\n  but still is:        %s\n"
      , (const char *) tag
      , (const char *) modelz
      , (const char *) checkz
      );
  } free(checkz);
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);

  return assert_msg_append_details(context, (size_t) l, msg_out, msg_out_size, tag);
}

size_t assert_not_memeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_size, const char *tag, void *check, void *model, size_t n)
{
  int              l;

  int              i, j;
  size_t           written = 0;

  int              cols_printed;
  int              hexdump_indent_spaces;
  int              need_multiple_lines = 0;

  static const int byte_print_width    = sizeof(" 0x00") - 1;
  static const int width               = ASSERT_MSG_WIDTH;

  /* ---------------------------------------------------------------- */
  /* Start writing error message. */
  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "Inverse assertion '%s' failed - '%d' bytes of memory must differ, but they are the same:"
    , (const char *) tag
    , (int) n
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  /* ---------------------------------------------------------------- */
  /* Hex dump of model: write what the memory should be equal to. */

  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "\n"
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  l = hexdump_indent_spaces = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "  should differ from:"
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  /* Here we also calculate whether this will span multiple lines, in which
   * case the hex dump starts on its own line for readability.
   */
  if (hexdump_indent_spaces + byte_print_width * n >= width)
  {
    /* The hexdump will span multiple lines, so start it on its own line. */
    /* It'll be indented by two extra spaces, one of which we write here. */
    need_multiple_lines = 1;

    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "\n"
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;

    l = hexdump_indent_spaces = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "    "
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }


  /* Hex dump: print each byte. */
  cols_printed = hexdump_indent_spaces;
  for (i = 0; i < n; ++i)
  {
    cols_printed += byte_print_width;
    if (cols_printed >= width)
    {
      /* Start a new line. */
      cols_printed = hexdump_indent_spaces + byte_print_width;

      l = snprintf
        ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
        , "\n"
        );
      if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
      written += l;

      for (j = 0; j < hexdump_indent_spaces; ++j)
      {
        l = snprintf
          ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
          , " "
          );
        if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
        written += l;
      }
    }

    /* Print the byte. */
    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , " 0x%.2X"
      , (unsigned int) (((unsigned char *) model)[i])
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }

  /* ---------------------------------------------------------------- */
  /* Hex dump of check: write what the memory should be equal to. */

  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "\n"
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "  but still is:"  /* A: Print six extra spaces below only without a newline. */
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  /* Should we start a new line? */
  if (need_multiple_lines)
  {
    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "\n"
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;

    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "    "
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }
  else
  {
    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , "      "  /* A: No newline, so print six extra spaces for alignment. */
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }

  /* Hex dump: print each byte. */
  cols_printed = hexdump_indent_spaces;
  for (i = 0; i < n; ++i)
  {
    cols_printed += byte_print_width;
    if (cols_printed >= width)
    {
      /* Start a new line. */
      cols_printed = hexdump_indent_spaces + byte_print_width;

      l = snprintf
        ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
        , "\n"
        );
      if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
      written += l;

      for (j = 0; j < hexdump_indent_spaces; ++j)
      {
        l = snprintf
          ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
          , " "
          );
        if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
        written += l;
      }
    }

    /* Print the byte. */
    l = snprintf
      ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
      , " 0x%.2X"
      , (unsigned int) (((unsigned char *) check)[i])
      );
    if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
    written += l;
  }

  l = snprintf
    ( (char *) (msg_out + written), (size_t) terminator_size(size_minus(msg_out_size, written))
    , "\n"
    );
  if (l < 0) return assert_msg_check_snprintf(context, l, msg_out, msg_out_size, tag, &context->is_snprintf_err);
  written += l;

  return assert_msg_append_details(context, (size_t) written, msg_out, msg_out_size, tag);
}

/* ---------------------------------------------------------------- */
/* Assertions with default error messages; non-zero on failure. */

unit_test_result_t assert_success(unit_test_context_t *context)
{
  return UNIT_TEST_PASS;
}

unit_test_result_t assert_failure(unit_test_context_t *context, const char *err_msg, const char *tag)
{
  if (err_msg)
    strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
  else
    context->err_buf_len = assert_failure_msg(context, context->err_buf, context->err_buf_halfsize, tag);

  return UNIT_TEST_FAIL;
}

unit_test_result_t assert_failure_continue(unit_test_context_t *context, const char *err_msg, const char *tag)
{
  if (err_msg)
    strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
  else
    context->err_buf_len = assert_failure_msg(context, context->err_buf, context->err_buf_halfsize, tag);

  return UNIT_TEST_FAIL_CONTINUE;
}

/* ---------------------------------------------------------------- */

unit_test_result_t assert_true(unit_test_context_t *context, const char *err_msg, const char *tag, int condition)
{
  if (condition)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_true_msg(context, context->err_buf, context->err_buf_halfsize, tag, condition);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_true_continue(unit_test_context_t *context, const char *err_msg, const char *tag, int condition)
{
  if (condition)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_true_msg(context, context->err_buf, context->err_buf_halfsize, tag, condition);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_inteq(unit_test_context_t *context, const char *err_msg, const char *tag, int check, int model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_inteq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_inteq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, int check, int model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_inteq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_uinteq(unit_test_context_t *context, const char *err_msg, const char *tag, unsigned int check, unsigned int model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_uinteq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_uinteq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, unsigned int check, unsigned int model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_uinteq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_ulongeq(unit_test_context_t *context, const char *err_msg, const char *tag, unsigned long check, unsigned long model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_ulongeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_ulongeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, unsigned long check, unsigned long model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_ulongeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_sizeeq(unit_test_context_t *context, const char *err_msg, const char *tag, size_t check, size_t model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_sizeeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_sizeeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, size_t check, size_t model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_sizeeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_objpeq(unit_test_context_t *context, const char *err_msg, const char *tag, const void *check, const void *model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_objpeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_objpeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const void *check, const void *model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_objpeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_funpeq(unit_test_context_t *context, const char *err_msg, const char *tag, tests_funp_t check, tests_funp_t model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_funpeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_funpeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, tests_funp_t check, tests_funp_t model)
{
  if (check == model)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_funpeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_streqz(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model)
{
  if (strcmp(check, model) == 0)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_streqz_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_streqz_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model)
{
  if (strcmp(check, model) == 0)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_streqz_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_streqn(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model, size_t max_len)
{
  if (strncmp(check, model, max_len) == 0)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_streqn_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model, max_len);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_streqn_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model, size_t max_len)
{
  if (strncmp(check, model, max_len) == 0)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_streqn_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model, max_len);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_nstreq(unit_test_context_t *context, const char *err_msg, const char *tag, size_t max_len, const char *check, const char *model)
{
  return assert_streqn(context, err_msg, tag, check, model, max_len);
}

unit_test_result_t assert_nstreq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, size_t max_len, const char *check, const char *model)
{
  return assert_streqn_continue(context, err_msg, tag, check, model, max_len);
}

unit_test_result_t assert_memeq(unit_test_context_t *context, const char *err_msg, const char *tag, void *check, void *model, size_t n)
{
  if (memcmp(check, model, n) == 0)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_memeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model, n);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_memeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, void *check, void *model, size_t n)
{
  if (memcmp(check, model, n) == 0)
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_memeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model, n);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

/* ---------------------------------------------------------------- */

unit_test_result_t assert_false(unit_test_context_t *context, const char *err_msg, const char *tag, int condition)
{
  if (!(condition))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_false_msg(context, context->err_buf, context->err_buf_halfsize, tag, condition);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_false_continue(unit_test_context_t *context, const char *err_msg, const char *tag, int condition)
{
  if (!(condition))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_false_msg(context, context->err_buf, context->err_buf_halfsize, tag, condition);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_inteq(unit_test_context_t *context, const char *err_msg, const char *tag, int check, int model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_inteq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_inteq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, int check, int model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_inteq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_uinteq(unit_test_context_t *context, const char *err_msg, const char *tag, unsigned int check, unsigned int model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_uinteq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_uinteq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, unsigned int check, unsigned int model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_uinteq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_ulongeq(unit_test_context_t *context, const char *err_msg, const char *tag, unsigned long check, unsigned long model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_ulongeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_ulongeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, unsigned long check, unsigned long model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_ulongeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_objpeq(unit_test_context_t *context, const char *err_msg, const char *tag, const void *check, const void *model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_objpeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_objpeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const void *check, const void *model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_objpeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_funpeq(unit_test_context_t *context, const char *err_msg, const char *tag, tests_funp_t check, tests_funp_t model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_funpeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_funpeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, tests_funp_t check, tests_funp_t model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_funpeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_sizeeq(unit_test_context_t *context, const char *err_msg, const char *tag, size_t check, size_t model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_sizeeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_sizeeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, size_t check, size_t model)
{
  if (!(check == model))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_sizeeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_streqz(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model)
{
  if (!(strcmp(check, model) == 0))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_streqz_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_streqz_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model)
{
  if (!(strcmp(check, model) == 0))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_streqz_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_streqn(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model, size_t max_len)
{
  if (!(strncmp(check, model, max_len) == 0))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_streqn_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model, max_len);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_streqn_continue(unit_test_context_t *context, const char *err_msg, const char *tag, const char *check, const char *model, size_t max_len)
{
  if (!(strncmp(check, model, max_len) == 0))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_streqn_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model, max_len);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

unit_test_result_t assert_not_nstreq(unit_test_context_t *context, const char *err_msg, const char *tag, size_t max_len, const char *check, const char *model)
{
  return assert_not_streqn(context, err_msg, tag, check, model, max_len);
}

unit_test_result_t assert_not_nstreq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, size_t max_len, const char *check, const char *model)
{
  return assert_not_streqn_continue(context, err_msg, tag, check, model, max_len);
}

unit_test_result_t assert_not_memeq(unit_test_context_t *context, const char *err_msg, const char *tag, void *check, void *model, size_t n)
{
  if (!(memcmp(check, model, n) == 0))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_memeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model, n);

    return UNIT_TEST_FAIL;
  }
}

unit_test_result_t assert_not_memeq_continue(unit_test_context_t *context, const char *err_msg, const char *tag, void *check, void *model, size_t n)
{
  if (!(memcmp(check, model, n) == 0))
  {
    return UNIT_TEST_PASS;
  }
  else
  {
    if (err_msg)
      strncpy(context->err_buf, err_msg, context->err_buf_halfsize);
    else
      context->err_buf_len = assert_not_memeq_msg(context, context->err_buf, context->err_buf_halfsize, tag, check, model, n);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

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
 *  - snprintf
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

/* string.h:
 *   - memcmp
 *   - strcmp
 *   - strncmp
 */
#include <string.h>

#include "../base.h"
#include "testing.h"

#include "../util.h"

/* ---------------------------------------------------------------- */
/* Types. */

unit_test_context_t *new_unit_test_context( int override_err_buf_len, int override_seed, int  override_out, int  override_err
                                          , int err_buf_len,          int seed,          FILE *out,         FILE *err)
{
  unit_test_context_t *context;

  /* Set default arguments. */
  if (!override_err_buf_len) err_buf_len = DEFAULT_TEST_CONTEXT_ERR_BUF_SIZE;
  if (!override_seed)        seed        = DEFAULT_TEST_CONTEXT_SEED;
  if (!override_out)         out         = DEFAULT_TEST_CONTEXT_OUT;
  if (!override_err)         err         = DEFAULT_TEST_CONTEXT_ERR;

  /* Allocate memory. */
  context = calloc(1, sizeof(unit_test_context_t));


  /* Set up environment. */
  context->err_buf_len = err_buf_len;
  context->err_buf     = malloc(context->err_buf_len);
  context->err_buf[0]  = 0;

  context->seed = seed;

  context->out  = out;
  context->err  = err;

  context->free = free_unit_test_context;

  /* Set up initial state. */
  context->group_depth  = 0;

  context->next_test_id = 1;

  context->num_pass     = 0;
  context->num_fail     = 0;

  context->last_pass    = -1;
  context->last_fail    = -1;

  /* Return object. */
  return context;
}

void free_unit_test_context(unit_test_context_t *context)
{
  if (context)
  {
    if (!context->free || context->free == free_unit_test_context)
    {
      if (context->err_buf)
      {
        free(context->err_buf);
        context->err_buf = (char *) NULL;
      }

      free(context);
    }
    else
    {
      context->free(context);
    }
  }
}

/* ---------------------------------------------------------------- */
/* Running tests. */

/* Run a unit test independently, with a new unit test context state. */
int run_test_suite(unit_test_t test)
{
  unit_test_result_t result;
  unit_test_context_t *context;

  context =
    new_unit_test_context
      ( 0, 0, 0,    0
      , 0, 0, NULL, NULL
      );
  {
    result = run_test_suite_with_context(context, test);
  } free_unit_test_context(context);

  return result;
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
  FILE *out;

  if (test_result_success(result))
  {
    out = context->out;

    fprintf(out, "pass: %d/%d test groups.\n", (int) (context->num_pass), (int) (context->num_pass));
  }
  else
  {
    out = context->err;

    context->err_buf[context->err_buf_len-1] = 0;
    fprintf
      ( out
      , "Error: %d tests failed:\n  last failed test #: %d\n  number of tests run: %d\n  can continue testing after last failure?: %s\n\nLast error message:\n\n%s\n"
      , (int) (context->num_fail)
      , (int) (context->last_fail)
      , (int) (context->next_test_id)
      , (result >= 0) ? "yes" : "no (aborted)"
      , (const char *) context->err_buf
      );
  }
}


int test_result_success(unit_test_result_t result)
{
  return result == 0;
}

int test_result_failure(unit_test_result_t result)
{
  return result != 0;
}

int test_result_can_continue(unit_test_result_t result)
{
  return result >= 0;
}

int test_result_need_abort(unit_test_result_t result)
{
  return result <  0;
}


int run_test(unit_test_context_t *context, unit_test_t test)
{
  int id;
  unit_test_result_t result;

  id = context->next_test_id++;

  print_test_prefix(context, test, id);

  result = test.run(context);

  if (!context->aborting)
  {
    if (test_result_success(result))
    {
      ++context->num_pass;
      context->last_pass = id;

      print_passed_test_result(context, test, id, result);
    }
    else
    {
      ++context->num_fail;
      context->last_fail = id;

      print_failed_test_result(context, test, id, result);
    }
  }

  if (test_result_need_abort(result))
  {
    context->aborting = 1;
  }

  return result;
}


void print_test_indent(unit_test_context_t *context)
{
  int i;

  for (i = 0; i < context->group_depth; ++i)
  {
    fprintf(context->out, "| ");
  }
}

void print_test_prefix(unit_test_context_t *context, unit_test_t test, int id)
{
  print_test_indent(context);

  fprintf(context->out, "- %d: %s:\n", id, test.name);
}

void print_passed_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result)
{
  print_test_indent(context);

  /* fprintf(context->out, "  %*.s   =)\n", 2 * context->group_depth, ""); */
  fprintf(context->out, "  %*.s   =): pass: %s\n", 2 * context->group_depth, "", test.description);
}

void print_failed_test_result(unit_test_context_t *context, unit_test_t test, int id, unit_test_result_t result)
{
  int can_continue;

  context->err_buf[context->err_buf_len-1] = 0;

  can_continue = test_result_can_continue(result);

  print_test_indent(context);
  fprintf(context->out, "  %*.s   FAILURE - %s\n", 2 * context->group_depth, "", test.description);

  fprintf(context->err, "/----------------------------------------------------------------\n");
  fprintf(context->err, "FAILURE:\n");
  fprintf(context->err, "  test number:      %d\n", id);
  fprintf(context->err, "  test name:        %s\n", test.name);
  fprintf(context->err, "  test description: %s\n", test.description);
  fprintf(context->err, "  test result code: %d\n", (int) result);
  fprintf(context->err, "  can continue?:    %s\n", (can_continue) ? "yes" : "no (aborting!)");
  fprintf(context->err, "\n");
  fprintf(context->err, "Error message:\n");
  fprintf(context->err, "\n");
  fprintf(context->err, "%s\n", (const char *) context->err_buf);
  fprintf(context->err, "\\----------------------------------------------------------------\n\n");
}


int run_tests_num(unit_test_context_t *context, unit_test_t **tests, size_t num_tests)
{
  int                i;
  int                abort = 0;
  unit_test_result_t result = UNIT_TEST_PASS;

  ++context->group_depth;
  for (i = 0; i < num_tests; ++i)
  {
    int individual_result;
    
    individual_result = run_test(context, *tests[i]);

    abort = test_result_need_abort(individual_result) || test_result_need_abort(result);

    result |= individual_result;

    if (abort)
    {
      break;
    }
  }
  --context->group_depth;

  return result;
}

int run_tests(unit_test_context_t *context, unit_test_t **tests)
{
  int                abort = 0;
  unit_test_t        **test;
  unit_test_result_t result = UNIT_TEST_PASS;

  ++context->group_depth;
  for (test = tests; *test; ++test)
  {
    int individual_result = run_test(context, **test);

    abort = test_result_need_abort(individual_result) || test_result_need_abort(result);

    result |= individual_result;

    if (abort)
    {
      break;
    }
  }
  --context->group_depth;

  return result;
}

/* ---------------------------------------------------------------- */
/* Default error messages for assertion failures. */

void assert_failure_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag)
{
  snprintf
    ( (char *) msg_out, (size_t) msg_out_len
    , "Assertion '%s' failed - no details provided."
    , (const char *) tag
    );
}

void assert_true_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, int condition)
{
  snprintf
    ( (char *) msg_out, (size_t) msg_out_len
    , "Boolean assertion '%s' failed - the condition be true, but it is false: %d (bool: %d)"
    , (const char *) tag
    , (int) condition
    , (int) !!condition
    );
}

void assert_inteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, int check, int model)
{
  snprintf
    ( (char *) msg_out, (size_t) msg_out_len
    , "Assertion '%s' failed - integers must be equal, but differ:\n  should be:   % d\n  actually is: % d"
    , (const char *) tag
    , (int) model
    , (int) check
    );
}

void assert_streqz_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, const char *check, const char *model)
{
  snprintf
    ( (char *) msg_out, (size_t) msg_out_len
    , "Assertion '%s' failed - strings must be equal, but differ:\n  should be:   %s\n  actually is: %s"
    , (const char *) tag
    , (const char*) model
    , (const char*) check
    );
}

void assert_streqn_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, const char *check, const char *model, size_t max_len)
{
  char *checkz, *modelz;

  checkz = malloc(2 * (max_len+1));
  {
    modelz = checkz + (max_len+1);

    strncpy(checkz, check, max_len);
    strncpy(modelz, model, max_len);
    checkz[max_len] = 0;
    modelz[max_len] = 0;

    snprintf
      ( (char *) msg_out, (size_t) msg_out_len
      , "Assertion '%s' failed - strings must be equal, but differ:\n  should be:   %s\n  actually is: %s"
      , (const char *) tag
      , (const char *) modelz
      , (const char *) checkz
      );
  } free(checkz);
}

void assert_memeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, void *check, void *model, size_t n)
{
  int              i, j;
  size_t           written = 0;

  int              cols_printed;
  int              hexdump_indent_spaces;
  int              need_multiple_lines = 0;

  static const int byte_print_width    = sizeof(" 0x00") - 1;
  static const int width               = ASSERT_MSG_WIDTH;

  /* ---------------------------------------------------------------- */
  /* Start writing error message. */
  written += snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "Assertion '%s' failed - '%d' bytes of memory must be equal, but differ:"
    , (const char *) tag
    , (int) n
    );

  /* ---------------------------------------------------------------- */
  /* Hex dump of model: write what the memory should be equal to. */

  written += snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "\n"
    );
  written += (hexdump_indent_spaces = snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "  should be:"  /* A: Print two extra spaces below only without a newline. */
    ));
  hexdump_indent_spaces += 2;

  /* Here we also calculate whether this will span multiple lines, in which
   * case the hex dump starts on its own line for readability.
   */
  if (hexdump_indent_spaces + byte_print_width * n >= width)
  {
    /* The hexdump will span multiple lines, so start it on its own line. */
    /* It'll be indented by two extra spaces, one of which we write here. */
    need_multiple_lines = 1;

    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "\n"
      );
    written += (hexdump_indent_spaces = snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "    "
      ));
  }
  else
  {
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "  "  /* A: No newline, so print two extra spaces for alignment. */
      );
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

      written += snprintf
        ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
        , "\n"
        );

      for (j = 0; j < hexdump_indent_spaces; ++j)
      {
        written += snprintf
          ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
          , " "
          );
      }
    }

    /* Print the byte. */
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , " 0x%.2X"
      , (unsigned int) (((unsigned char *) model)[i])
      );
  }

  /* ---------------------------------------------------------------- */
  /* Hex dump of check: write what the memory should be equal to. */

  written += snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "\n"
    );
  written += snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "  actually is:"
    );

  /* Should we start a new line? */
  if (need_multiple_lines)
  {
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "\n"
      );
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "    "
      );
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

      written += snprintf
        ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
        , "\n"
        );

      for (j = 0; j < hexdump_indent_spaces; ++j)
      {
        written += snprintf
          ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
          , " "
          );
      }
    }

    /* Print the byte. */
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , " 0x%.2X"
      , (unsigned int) (((unsigned char *) check)[i])
      );
  }
}


void assert_false_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, int condition)
{
  snprintf
    ( (char *) msg_out, (size_t) msg_out_len
    , "Inverse boolean assertion '%s' failed - the condition be false, but it is true: %d (bool: %d)"
    , (const char *) tag
    , (int) condition
    , (int) !!condition
    );
}

void assert_not_inteq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, int check, int model)
{
  snprintf
    ( (char *) msg_out, (size_t) msg_out_len
    , "Inverse assertion '%s' failed - integers must differ, but they are the same:\n  should differ from:  %d\n  but still is:        %d"
    , (const char *) tag
    , (int) model
    , (int) check
    );
}

void assert_not_streqz_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, const char *check, const char *model)
{
  snprintf
    ( (char *) msg_out, (size_t) msg_out_len
    , "Inverse assertion '%s' failed - strings must duffer, but they are the same:\n  should differ from:  %s\n  but still is:        %s"
    , (const char *) tag
    , (const char*) model
    , (const char*) check
    );
}

void assert_not_streqn_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, const char *check, const char *model, size_t max_len)
{
  char *checkz, *modelz;

  checkz = malloc(2 * (max_len+1));
  {
    modelz = checkz + (max_len+1);

    strncpy(checkz, check, max_len);
    strncpy(modelz, model, max_len);
    checkz[max_len] = 0;
    modelz[max_len] = 0;

    snprintf
      ( (char *) msg_out, (size_t) msg_out_len
      , "Inverse assertion '%s' failed - strings must duffer, but they are the same:\n  should differ from:  %s\n  but still is:        %s"
      , (const char *) tag
      , (const char *) modelz
      , (const char *) checkz
      );
  } free(checkz);
}

void assert_not_memeq_msg(unit_test_context_t *context, char *msg_out, size_t msg_out_len, const char *tag, void *check, void *model, size_t n)
{
  int              i, j;
  size_t           written = 0;

  int              cols_printed;
  int              hexdump_indent_spaces;
  int              need_multiple_lines = 0;

  static const int byte_print_width    = sizeof(" 0x00") - 1;
  static const int width               = ASSERT_MSG_WIDTH;

  /* ---------------------------------------------------------------- */
  /* Start writing error message. */
  written += snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "Inverse assertion '%s' failed - '%d' bytes of memory must differ, but they are the same:"
    , (const char *) tag
    , (int) n
    );

  /* ---------------------------------------------------------------- */
  /* Hex dump of model: write what the memory should be equal to. */

  written += snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "\n"
    );
  written += (hexdump_indent_spaces = snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "  should differ from:"
    ));

  /* Here we also calculate whether this will span multiple lines, in which
   * case the hex dump starts on its own line for readability.
   */
  if (hexdump_indent_spaces + byte_print_width * n >= width)
  {
    /* The hexdump will span multiple lines, so start it on its own line. */
    /* It'll be indented by two extra spaces, one of which we write here. */
    need_multiple_lines = 1;

    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "\n"
      );
    written += (hexdump_indent_spaces = snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "    "
      ));
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

      written += snprintf
        ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
        , "\n"
        );

      for (j = 0; j < hexdump_indent_spaces; ++j)
      {
        written += snprintf
          ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
          , " "
          );
      }
    }

    /* Print the byte. */
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , " 0x%.2X"
      , (unsigned int) (((unsigned char *) model)[i])
      );
  }

  /* ---------------------------------------------------------------- */
  /* Hex dump of check: write what the memory should be equal to. */

  written += snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "\n"
    );
  written += snprintf
    ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
    , "  but still is:"  /* A: Print six extra spaces below only without a newline. */
    );

  /* Should we start a new line? */
  if (need_multiple_lines)
  {
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "\n"
      );
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "    "
      );
  }
  else
  {
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , "      "  /* A: No newline, so print six extra spaces for alignment. */
      );
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

      written += snprintf
        ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
        , "\n"
        );

      for (j = 0; j < hexdump_indent_spaces; ++j)
      {
        written += snprintf
          ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
          , " "
          );
      }
    }

    /* Print the byte. */
    written += snprintf
      ( (char *) (msg_out + written), (size_t) (msg_out_len + written)
      , " 0x%.2X"
      , (unsigned int) (((unsigned char *) check)[i])
      );
  }
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
    strncpy(context->err_buf, err_msg, context->err_buf_len);
  else
    assert_failure_msg(context, context->err_buf, context->err_buf_len, tag);

  return UNIT_TEST_FAIL;
}

unit_test_result_t assert_failure_continue(unit_test_context_t *context, const char *err_msg, const char *tag)
{
  if (err_msg)
    strncpy(context->err_buf, err_msg, context->err_buf_len);
  else
    assert_failure_msg(context, context->err_buf, context->err_buf_len, tag);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_true_msg(context, context->err_buf, context->err_buf_len, tag, condition);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_true_msg(context, context->err_buf, context->err_buf_len, tag, condition);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_inteq_msg(context, context->err_buf, context->err_buf_len, tag, check, model);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_inteq_msg(context, context->err_buf, context->err_buf_len, tag, check, model);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_streqz_msg(context, context->err_buf, context->err_buf_len, tag, check, model);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_streqz_msg(context, context->err_buf, context->err_buf_len, tag, check, model);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_streqn_msg(context, context->err_buf, context->err_buf_len, tag, check, model, max_len);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_streqn_msg(context, context->err_buf, context->err_buf_len, tag, check, model, max_len);

    return UNIT_TEST_FAIL_CONTINUE;
  }
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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_memeq_msg(context, context->err_buf, context->err_buf_len, tag, check, model, n);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_memeq_msg(context, context->err_buf, context->err_buf_len, tag, check, model, n);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_false_msg(context, context->err_buf, context->err_buf_len, tag, condition);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_false_msg(context, context->err_buf, context->err_buf_len, tag, condition);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_not_inteq_msg(context, context->err_buf, context->err_buf_len, tag, check, model);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_not_inteq_msg(context, context->err_buf, context->err_buf_len, tag, check, model);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_not_streqz_msg(context, context->err_buf, context->err_buf_len, tag, check, model);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_not_streqz_msg(context, context->err_buf, context->err_buf_len, tag, check, model);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_not_streqn_msg(context, context->err_buf, context->err_buf_len, tag, check, model, max_len);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_not_streqn_msg(context, context->err_buf, context->err_buf_len, tag, check, model, max_len);

    return UNIT_TEST_FAIL_CONTINUE;
  }
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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_not_memeq_msg(context, context->err_buf, context->err_buf_len, tag, check, model, n);

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
      strncpy(context->err_buf, err_msg, context->err_buf_len);
    else
      assert_not_memeq_msg(context, context->err_buf, context->err_buf_len, tag, check, model, n);

    return UNIT_TEST_FAIL_CONTINUE;
  }
}

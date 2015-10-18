/*
 * opencurry: tests/test_utf8.c
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
#include "test_utf8.h"

#include "../utf8.h"

#include "../unicode.h"

int test_utf8_cli(int argc, char **argv)
{
  return run_test_suite(utf8_test);
}

/* ---------------------------------------------------------------- */

/* utf8 tests. */
unit_test_t utf8_test =
  {  test_utf8_run
  , "test_utf8"
  , "utf8 tests."
  };

/* Array of utf8 tests. */
unit_test_t *utf8_tests[] =
  { &utf8_encode_one_equalities_test
  , NULL
  };

unit_test_result_t test_utf8_run(unit_test_context_t *context)
{
  return run_tests(context, utf8_tests);
}

/* ---------------------------------------------------------------- */

static const struct utf8_codepoint_pair_s
{
  codepoint_t codepoint;

  unsigned char buf[4];
  size_t        utf8_size;
} utf8_codepoint_pairs[] =
  { { 0x0037, {0x37, 0x00, 0x00, 0x00}, 1}  /*  U+0037: DIGIT SEVEN       */
  , { 0x02DA, {0xCB, 0x9A, 0x00, 0x00}, 2}  /*  U+02DA: RING ABOVE        */
  , { 0x2665, {0xE2, 0x99, 0xA5, 0x00}, 3}  /*  U+2665: BLACK HEART SUIT  */
  , {0x1D13D, {0xF0, 0x9D, 0x8C, 0x9D}, 4}  /* U+1D31D: TETRAGRAM FOR JOY */
  , {0x1F48C, {0xF0, 0x9F, 0x92, 0x8C}, 4}  /* U+1F48C: LOVE LETTER       */
  };
static const size_t utf8_codepoint_pairs_size = sizeof(utf8_codepoint_pairs) / sizeof(utf8_codepoint_pairs[0]);

/* ---------------------------------------------------------------- */

unit_test_t utf8_encode_one_equalities_test =
  {  utf8_encode_one_equalities_test_run 
  , "utf8_encode_one_equalities_test"
  , "utf8_encode_one: equality tests."
  };

unit_test_result_t utf8_encode_one_equalities_test_run(unit_test_context_t *context)
{
  int i;
  unit_test_result_t result;

  const struct utf8_codepoint_pair_s *pair;

  result = assert_success(context);

  for(i = 0; i < utf8_codepoint_pairs_size; ++i)
  {
    unsigned char buf[4] = { 0x00, 0x00, 0x00, 0x00 };
    size_t        utf8_size;

    pair = &utf8_codepoint_pairs[i];

    utf8_size = utf8_encode_one(buf, pair->codepoint);

    result |=
      assert_inteq (context, NULL, (int)          utf8_size, (int)          pair->utf8_size);
    if (test_result_need_abort(result)) break;

    result |=
      assert_memeq (context, NULL, (void *)       buf,       (void *)       pair->buf,       4);
    if (test_result_need_abort(result)) break;

    result |=
      assert_streqn(context, NULL, (const char *) buf,       (const char *) pair->buf,       4);
    if (test_result_need_abort(result)) break;
  }

  return result;
}

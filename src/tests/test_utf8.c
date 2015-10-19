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
#include "../util.h"

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
  , &utf8_encode_one_edge_cases_test

  , &utf8_encode_equalities_test
  , &utf8_encode_edge_cases_test


  , &utf8_decode_one_equalities_test
  , &utf8_decode_one_edge_cases_test

  , &utf8_decode_equalities_test
  /* TODO
  , &utf8_decode_edge_cases_test
  */

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
  , {0x1D31D, {0xF0, 0x9D, 0x8C, 0x9D}, 4}  /* U+1D31D: TETRAGRAM FOR JOY */
  , {0x1F48C, {0xF0, 0x9F, 0x92, 0x8C}, 4}  /* U+1F48C: LOVE LETTER       */
  };
static const size_t utf8_codepoint_pairs_size = sizeof(utf8_codepoint_pairs) / sizeof(utf8_codepoint_pairs[0]);

/* ---------------------------------------------------------------- */
/* utf8_encode_one tests                                            */
/* ---------------------------------------------------------------- */

unit_test_t utf8_encode_one_equalities_test =
  {  utf8_encode_one_equalities_test_run 
  , "utf8_encode_one_equalities_test"
  , "utf8_encode_one: equality tests."
  };

unit_test_result_t utf8_encode_one_equalities_test_run(unit_test_context_t *context)
{
  char tag[DEFAULT_MSG_SIZE];

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

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "width %d", i);
    result |=
      assert_inteq (context, NULL, tag, (int)          utf8_size, (int)          pair->utf8_size);
    if (test_result_need_abort(result)) break;

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "buf mem %d", i);
    result |=
      assert_memeq (context, NULL, tag, (void *)       buf,       (void *)       pair->buf,       4);
    /* if (test_result_need_abort(result)) break; */

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "buf str %d", i);
    result |=
      assert_streqn(context, NULL, tag, (const char *) buf,       (const char *) pair->buf,       4);
    if (test_result_need_abort(result)) break;
  }

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t utf8_encode_one_edge_cases_test =
  {  utf8_encode_one_edge_cases_test_run 
  , "utf8_encode_one_edge_cases_test"
  , "utf8_encode_one: edge cases tests."
  };

unit_test_result_t utf8_encode_one_edge_cases_test_run(unit_test_context_t *context)
{
  size_t                              utf8_size;
  unit_test_result_t                  result;

  const struct utf8_codepoint_pair_s *pair;


  result = assert_success(context);

  pair = &utf8_codepoint_pairs[0];

  /* Test NULL arguments for optional parameters. */
  utf8_size = utf8_encode_one(NULL, pair->codepoint);

  result |=
    assert_inteq (context, NULL, "width", (int) utf8_size, (int) pair->utf8_size);
  if (test_result_need_abort(result)) return result;

  return result;
}

/* ---------------------------------------------------------------- */
/* utf8_encode tests                                                */
/* ---------------------------------------------------------------- */

unit_test_t utf8_encode_equalities_test =
  {  utf8_encode_equalities_test_run 
  , "utf8_encode_equalities_test"
  , "utf8_encode: equality tests."
  };

static const codepoint_t utf8_codepoints[] =
  { 0x2665  /* BLACK HEART SUIT                    */
  , 0x2665  /* BLACK HEART SUIT                    */
  , 0x2665  /* BLACK HEART SUIT                    */
  , 0x03B1  /* GREEK SMALL LETTER ALPHA            */
  , 0x03B3  /* GREEK SMALL LETTER GAMMA            */
  , 0x03B1  /* GREEK SMALL LETTER ALPHA            */
  , 0x03C0  /* GREEK SMALL LETTER PI               */
  , 0x03CE  /* GREEK SMALL LETTER OMEGA WITH TONOS */
  , 0x2665  /* BLACK HEART SUIT                    */
  };
static const size_t utf8_codepoints_size = sizeof(utf8_codepoints) / sizeof(utf8_codepoints[0]);

static const unsigned char utf8_codepoints_encoding[] =
  { 0xE2, 0x99, 0xA5  /* U+2665: BLACK HEART SUIT                    */
  , 0xE2, 0x99, 0xA5  /* U+2665: BLACK HEART SUIT                    */
  , 0xE2, 0x99, 0xA5  /* U+2665: BLACK HEART SUIT                    */
  , 0xCE, 0xB1        /* U+03B1: GREEK SMALL LETTER ALPHA            */
  , 0xCE, 0xB3        /* U+03B1: GREEK SMALL LETTER GAMMA            */
  , 0xCE, 0xB1        /* U+03B1: GREEK SMALL LETTER ALPHA            */
  , 0xCF, 0x80        /* U+03B1: GREEK SMALL LETTER PI               */
  , 0xCF, 0x8E        /* U+03B1: GREEK SMALL LETTER OMEGA WITH TONOS */
  , 0xE2, 0x99, 0xA5  /* U+2665: BLACK HEART SUIT                    */
  };
static const size_t utf8_codepoints_encoding_size = sizeof(utf8_codepoints_encoding) / sizeof(utf8_codepoints_encoding[0]);

/* Number of bytes to encode last symbol in utf8_codepoints_encoding.
 * Used in the unit test "".
 */
static const size_t utf8_encoding_last_width = 3;

unit_test_result_t utf8_encode_equalities_test_run(unit_test_context_t *context)
{
  unit_test_result_t result;

  unsigned char      buf[sizeof(utf8_codepoints_encoding) / sizeof(utf8_codepoints_encoding[0])];
  size_t             buf_written;
  size_t             num_encoded;

  result = assert_success(context);

  buf_written = utf8_encode(buf, utf8_codepoints_encoding_size, utf8_codepoints, utf8_codepoints_size, &num_encoded);

  result |=
    assert_inteq (context, NULL, "num_written", (int)          buf_written, (int)          utf8_codepoints_encoding_size);
  if (test_result_need_abort(result)) return result;

  result |=
    assert_inteq (context, NULL, "num_encoded", (int)          num_encoded, (int)          utf8_codepoints_size);
  if (test_result_need_abort(result)) return result;

  result |=
    assert_memeq (context, NULL, "buf mem",     (void *)       buf,         (void *)       utf8_codepoints_encoding,      utf8_codepoints_encoding_size);
  /* if (test_result_need_abort(result)) return result; */

  result |=
    assert_streqn(context, NULL, "buf str",     (const char *) buf,         (const char *) utf8_codepoints_encoding,      utf8_codepoints_encoding_size);
  if (test_result_need_abort(result)) return result;

  return result;
}

/* ---------------------------------------------------------------- */

unit_test_t utf8_encode_edge_cases_test =
  {  utf8_encode_edge_cases_test_run 
  , "utf8_encode_edge_cases_test"
  , "utf8_encode: edge cases tests."
  };

unit_test_result_t utf8_encode_edge_cases_test_run(unit_test_context_t *context)
{
  unit_test_result_t result;

  unsigned char      buf[sizeof(utf8_codepoints_encoding) / sizeof(utf8_codepoints_encoding[0])];
  size_t             buf_written;
  size_t             num_encoded;

  result = assert_success(context);

  /* Test NULL arguments for optional parameters. */
  buf_written = utf8_encode(buf, utf8_codepoints_encoding_size, utf8_codepoints, utf8_codepoints_size, NULL);

  result |=
    assert_inteq (context, NULL, "num_written",       (int)          buf_written, (int)          utf8_codepoints_encoding_size);
  if (test_result_need_abort(result)) return result;

  /* Test smaller buf limit. */
  buf_written = utf8_encode(buf, utf8_codepoints_encoding_size - 1, utf8_codepoints, utf8_codepoints_size, &num_encoded);

  result |=
    assert_inteq (context, NULL, "small num_written", (int)          buf_written, (int)          utf8_codepoints_encoding_size - utf8_encoding_last_width);
  if (test_result_need_abort(result)) return result;

  result |=
    assert_inteq (context, NULL, "small num_encoded", (int)          num_encoded, (int)          utf8_codepoints_size - 1);
  if (test_result_need_abort(result)) return result;

  return result;
}

/* ---------------------------------------------------------------- */
/* utf8_decode_one tests                                            */
/* ---------------------------------------------------------------- */

unit_test_t utf8_decode_one_equalities_test =
  {  utf8_decode_one_equalities_test_run 
  , "utf8_decode_one_equalities_test"
  , "utf8_decode_one: equality tests."
  };

unit_test_result_t utf8_decode_one_equalities_test_run(unit_test_context_t *context)
{
  char tag[DEFAULT_MSG_SIZE];

  int i;
  unit_test_result_t result;

  const struct utf8_codepoint_pair_s *pair;

  result = assert_success(context);

  for(i = 0; i < utf8_codepoint_pairs_size; ++i)
  {
    codepoint_t                codepoint;
    size_t                     width;
    utf8_decode_error_status_t error_status;

    pair = &utf8_codepoint_pairs[i];

    codepoint = utf8_decode_one(pair->buf, pair->utf8_size, utf8_default_decode_error_behaviour, &width, &error_status);

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "codepoint %d", i);
    result |=
      assert_inteq (context, NULL, tag, (int) codepoint,      (int) pair->codepoint);
    if (test_result_need_abort(result)) break;

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "width %d", i);
    result |=
      assert_inteq (context, NULL, tag, (int) width,          (int) pair->utf8_size);
    if (test_result_need_abort(result)) break;

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "error_status %d", i);
    result |=
      assert_inteq (context, NULL, tag, (int) error_status,   (int) utf8_decode_no_error);
    if (test_result_need_abort(result)) break;
  }

  return result;
}

/* ---------------------------------------------------------------- */

static const struct utf8_decode_one_pair_s
{
  /* Input. */
  unsigned char                 buf[4];
  size_t                        utf8_size;

  utf8_decode_error_behaviour_t error_behaviour;

  /* Output. */
  codepoint_t                   codepoint;

  size_t                        width;
  size_t                        error_status;
} utf8_decode_one_pairs[] =
  {
    /* Valid input. */
    { {0x37, 0x00, 0x00, 0x00}, 1,  utf8_default_decode_error_behaviour  /*  U+0037: DIGIT SEVEN       */
    ,  0x0037, 1, utf8_decode_no_error
    }
  , { {0xCB, 0x9A, 0x00, 0x00}, 2,  utf8_default_decode_error_behaviour  /*  U+02DA: RING ABOVE        */
    ,  0x02DA, 2, utf8_decode_no_error
    }
  , { {0xE2, 0x99, 0xA5, 0x00}, 3,  utf8_default_decode_error_behaviour  /*  U+2665: BLACK HEART SUIT  */
    ,  0x2665, 3, utf8_decode_no_error
    }
  , { {0xF0, 0x9D, 0x8C, 0x9D}, 4,  utf8_default_decode_error_behaviour  /* U+1D31D: TETRAGRAM FOR JOY */
    , 0x1D31D, 4, utf8_decode_no_error
    }
  , { {0xF0, 0x9F, 0x92, 0x8C}, 4,  utf8_default_decode_error_behaviour  /* U+1F48C: LOVE LETTER       */
    , 0x1F48C, 4, utf8_decode_no_error
    }

    /* Testing different behaviours on unexpected continuation bytes. */
  , { {0x8C, 0x9F, 0x92, 0x8C}, 4,  utf8_replacement_character_uFFFD_behaviour
    ,  0xFFFD, 1, utf8_decode_unexpected_continuation_byte
    }
  , { {0x8C, 0x9F, 0x92, 0x8C}, 4,  utf8_invalid_character_uDCxx_behaviour
    ,  0xDC8C, 1, utf8_decode_unexpected_continuation_byte
    }
  , { {0x8C, 0x9F, 0x92, 0x8C}, 4,  utf8_codepoint_u00xx_behaviour
    ,  0x008C, 1, utf8_decode_unexpected_continuation_byte
    }

    /* Testing other errors. */
  , { {0xF0, 0x9F, 0x92, 0x00}, 3,  utf8_replacement_character_uFFFD_behaviour
    ,  0xFFFD, 1, utf8_decode_insufficient_continuation_bytes_eof
    }
  , { {0xF0, 0x9F, 0x92, 0x37}, 4,  utf8_replacement_character_uFFFD_behaviour
    ,  0xFFFD, 1, utf8_decode_insufficient_continuation_bytes_sufficient_input
    }
  , { {0xF8, 0x9F, 0x92, 0x8C}, 4,  utf8_replacement_character_uFFFD_behaviour
    ,  0xFFFD, 1, utf8_decode_out_of_bounds
    }

    /* Testing overlong encoding errors. */

    /* '7' in 2 bytes */
    /* xxxx xxxx = 0011 0111 = 0x37 */
    /* 110x xxxx  10xx xxxx */
    /* 1100 0000  1011 0111 */
    /* 0xC0      0xB7      */
  , { {0xC0, 0xB7, 0x00, 0x00}, 2,  utf8_replacement_character_uFFFD_behaviour
    ,  0xFFFD, 1, utf8_decode_overlong_encoding
    }
  , { {0xC0, 0xB7, 0x00, 0x00}, 2,  utf8_po_replacement_character_uFFFD_behaviour
    ,  0x0037, 2, utf8_decode_overlong_encoding
    }

    /* 'G' in 2 bytes */
    /* xxxx xxxx = 0100 0111 = 0x47 */
    /* 110x xxxx  10xx xxxx */
    /* 1100 0001  1000 0111 */
    /* 0xC1       0x87      */
  , { {0xC1, 0x87, 0x00, 0x00}, 2,  utf8_replacement_character_uFFFD_behaviour
    ,  0xFFFD, 1, utf8_decode_overlong_encoding
    }
  , { {0xC1, 0x87, 0x00, 0x00}, 2,  utf8_po_replacement_character_uFFFD_behaviour
    ,  0x0047, 2, utf8_decode_overlong_encoding
    }
  };
static const size_t utf8_decode_one_pairs_size = sizeof(utf8_decode_one_pairs) / sizeof(utf8_decode_one_pairs[0]);

/* ---------------------------------------------------------------- */

unit_test_t utf8_decode_one_edge_cases_test =
  {  utf8_decode_one_edge_cases_test_run 
  , "utf8_decode_one_edge_cases_test"
  , "utf8_decode_one: edge cases tests."
  };

unit_test_result_t utf8_decode_one_edge_cases_test_run(unit_test_context_t *context)
{
  char tag[DEFAULT_MSG_SIZE];

  int i;
  unit_test_result_t result;

  const struct utf8_decode_one_pair_s *pair;

  result = assert_success(context);

  for(i = 0; i < utf8_decode_one_pairs_size; ++i)
  {
    codepoint_t                codepoint;
    size_t                     width;
    utf8_decode_error_status_t error_status;

    pair = &utf8_decode_one_pairs[i];

    codepoint = utf8_decode_one(pair->buf, pair->utf8_size, pair->error_behaviour, &width, &error_status);

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "error_status %d", i);
    result |=
      assert_inteq (context, NULL, tag, (int) error_status,   (int) pair->error_status);
    if (test_result_need_abort(result)) break;

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "codepoint %d", i);
    result |=
      assert_inteq (context, NULL, tag, (int) codepoint,      (int) pair->codepoint);
    if (test_result_need_abort(result)) break;

    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "width %d", i);
    result |=
      assert_inteq (context, NULL, tag, (int) width,          (int) pair->width);
    if (test_result_need_abort(result)) break;
  }

  return result;
}

/* ---------------------------------------------------------------- */
/* utf8_decode tests                                                */
/* ---------------------------------------------------------------- */

unit_test_t utf8_decode_equalities_test =
  {  utf8_decode_equalities_test_run 
  , "utf8_decode_equalities_test"
  , "utf8_decode: equality tests."
  };

unit_test_result_t utf8_decode_equalities_test_run(unit_test_context_t *context)
{
  unit_test_result_t result;

  size_t                     num_codepoints;
  codepoint_t                codepoints[sizeof(utf8_codepoints) / sizeof(utf8_codepoints[0])];
  size_t                     bytes_read;
  size_t                     num_trailing_bytes;
  utf8_decode_error_status_t error_status;

  result = assert_success(context);

  num_codepoints =
    utf8_decode
      ( codepoints, utf8_codepoints_size, utf8_codepoints_encoding
      , utf8_codepoints_encoding_size, 0, utf8_default_decode_error_behaviour
      , &bytes_read, &num_trailing_bytes, &error_status
      );

  result |=
    assert_inteq (context, NULL, "num_codepoints",     (int)    num_codepoints,     (int)    utf8_codepoints_size);
  if (test_result_need_abort(result)) return result;

  result |=
    assert_inteq (context, NULL, "bytes_read",         (int)    bytes_read,         (int)    utf8_codepoints_encoding_size);
  if (test_result_need_abort(result)) return result;

  result |=
    assert_inteq (context, NULL, "num_trailing_bytes", (int)    num_trailing_bytes, (int)    0);
  if (test_result_need_abort(result)) return result;

  result |=
    assert_inteq (context, NULL, "error_status",       (int)    error_status,       (int)    utf8_decode_no_error);
  if (test_result_need_abort(result)) return result;

  result |=
    assert_memeq (context, NULL, "buf mem",            (void *) codepoints,         (void *) utf8_codepoints, sizeof(utf8_codepoints));
  if (test_result_need_abort(result)) return result;

  return result;
}

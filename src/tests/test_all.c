/*
 * opencurry: tests/test_all.c
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

#include "../base.h"
#include "testing.h"
#include "test_all.h"

#include "test_testing.h"
#include "test_resources.h"
#include "test_opencurry.h"
#include "test_cli.h"
#include "test_util.h"
#include "test_buffers.h"
#include "test_mempool.h"
#include "test_type_base.h"
#include "test_type_base_prim.h"
#include "test_type_base_tval.h"
#include "test_type_tags.h"
#include "test_type_structs.h"
#include "test_type_fun.h"
#include "test_type_adts.h"
#include "test_type_util.h"
#include "test_bits.h"
#include "test_global.h"
#include "test_bnf.h"
#include "test_lex.h"
#include "test_unicode.h"
#include "test_utf8.h"
#include "test_integer.h"
#include "test_ansi_c_ast.h"
#include "test_ansi_c_ast_base.h"
#include "test_ansi_c_ast_gen.h"
#include "test_ansi_c_ast_render.h"

int test_all(int argc, char **argv)
{
  return run_test_suite(test_all_test);
}

/* ---------------------------------------------------------------- */

/* Root unit test. */
unit_test_t test_all_test =
  {  test_all_run
  , "test_all"
  , "All tests."
  };

/* Array of all test groups. */
unit_test_t *all_tests[] =
  { &testing_test
  , &resources_test
  , &opencurry_test
  , &cli_test
  , &util_test
  , &buffers_test
  , &mempool_test
  , &type_base_test
  , &type_base_prim_test
  , &type_base_tval_test
  , &type_tags_test
  , &type_structs_test
  , &type_fun_test
  , &type_adts_test
  , &type_util_test
  , &bits_test
  , &global_test
  , &bnf_test
  , &lex_test
  , &unicode_test
  , &utf8_test
  , &integer_test
  , &ansi_c_ast_test
  , &ansi_c_ast_base_test
  , &ansi_c_ast_gen_test
  , &ansi_c_ast_render_test
  , NULL
  };

unit_test_result_t test_all_run(unit_test_context_t *context)
{
  return run_tests(context, all_tests);
}

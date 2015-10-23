/*
 * opencurry: tests/test_ansi_c_ast_gen.c
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
#include "test_ansi_c_ast_gen.h"

#include "ansi_c_ast_base.h"

#include "../ansi_c_ast_gen.h"

int test_ansi_c_ast_gen_cli(int argc, char **argv)
{
  return run_test_suite(ansi_c_ast_gen_test);
}

/* ---------------------------------------------------------------- */

/* ansi_c_ast_gen tests. */
unit_test_t ansi_c_ast_gen_test =
  {  test_ansi_c_ast_gen_run
  , "test_ansi_c_ast_gen"
  , "ansi_c_ast_gen tests."
  };

/* Array of ansi_c_ast_gen tests. */
unit_test_t *ansi_c_ast_gen_tests[] =
  { &ansi_c_ast_gen_0_equalities_test

  , NULL
  };

unit_test_result_t test_ansi_c_ast_gen_run(unit_test_context_t *context)
{
  return run_tests(context, ansi_c_ast_gen_tests);
}

/* ---------------------------------------------------------------- */
/* Testing helpers.                                                 */
/* ---------------------------------------------------------------- */

/* ---------------------------------------------------------------- */
/* Static test data for all local tests.                            */
/* ---------------------------------------------------------------- */

/* ---------------------------------------------------------------- */
/* ansi_c_ast_gen_0_equalities_test:                                */
/* ---------------------------------------------------------------- */

/* Testing generated C AST through ansi_c_ast_gen API results in    */
/* expected AST.                                                    */

/* ---------------------------------------------------------------- */
/* gen_0 test: Expected AST model, hand-coded.                      */
/* ---------------------------------------------------------------- */

/*
static const ac_ast_t gen_0_model =
  {
  };
*/

/* ---------------------------------------------------------------- */
/* gen_0 test: Generate AST to test through API.                    */
/* ---------------------------------------------------------------- */

/*
static ac_ast_bufs_t *gen_0_check_gen(ac_ast_bufs_t *ast)
{
  /8 TODO 8/

  return ast;
}
*/

/* ---------------------------------------------------------------- */

unit_test_t ansi_c_ast_gen_0_equalities_test =
  {  ansi_c_ast_gen_0_equalities_test_run
  , "ansi_c_ast_gen_0_equalities_test"
  , "ansi_c_ast_gen API equality check: ansi_c_ast_gen API generated AST is correct."
  };

unit_test_result_t ansi_c_ast_gen_0_equalities_test_run(unit_test_context_t *context)
{
  return assert_success(context);
/*
  char                 tag[DEFAULT_TAG_SIZE];

  const ac_ast_bufs_t *model = &gen_0_model;
  ac_ast_bufs_t       *check;

  unit_test_result_t   result;

  result = assert_success(context);

  check = new_ac_ast_bufs();
  {
    snprintf(tag, sizeof(tag) / sizeof(tag[0]), "gen_0");
    result |=
      assert_eq_c_ast(context, NULL, tag, (const ac_ast_bufs_t *) (check->elems->slots), (const ac_ast_bufs_t *) model);
    if (is_test_result_aborting(result)) return result;
  } free_ac_ast_bufs_recursive(check);

  return result;
*/
}

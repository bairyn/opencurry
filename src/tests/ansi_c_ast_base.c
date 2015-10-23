/*
 * opencurry: tests/ansi_c_ast_base.c
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
#include "ansi_c_ast_base.h"

#include "../ansi_c_ast_base.h"

unit_test_result_t assert_eq_c_ast(unit_test_context_t *context, const char *tag, const c_ast_t *check, const c_ast_t *model)
{
  int i;
  char alltag[DEFAULT_TAG_SIZE];
  void *refs[2];

  unit_test_result_t result;

  result = assert_success(context);


  tag ||= "";


  i = 0;
  refs[i++] = (void *) check;
  refs[i++] = (void *) model;

  /* Add our location in the AST. */
  test_add_details_msg("c_ast:\n");

  /* Check references are not NULL. */
  for (i = 0; i < sizeof(refs) / sizeof(refs[0]); ++i)
  {
    snprintf(alltag, sizeof(alltag) / sizeof(alltag[0]), "assert_eq_c_ast non-NULL %s (%s)", !i ? "check" : "model", tag);
    if (!refs[i])
      result |=
        assert_failure(context, NULL, alltag);
    if (test_result_need_abort(result)) break;
  }

  /* Check types are equal. */
  snprintf(alltag, sizeof(alltag) / sizeof(alltag[0]), "type (%s)", tag);
  result |=
    assert_inteq (context, NULL, alltag, (int) check->type, (int) model->type);
  if (test_result_need_abort(result)) return result;

  switch (check->type)
  {
    default:
    ac_ast_empt:
      break;

    ac_ast_anno:
      assert_eq_ac_anno(context, tag, (const ac_anno_t *) check->values.anno.anno, (const ac_anno_t *) check->values.ast.ast);
      break;

    ac_ast_anno:
      assert_eq_ac_item(context, tag, (const ac_anno_t *) check->values.anno.anno, (const ac_anno_t *) check->values.ast.ast);
      break;
  };

  return result;
}

unit_test_result_t assert_eq_ac_anno(unit_test_context_t *context, const char *tag, const c_ast_t *check, const c_ast_t *model, int depth)
{
  /* TODO */
  return assert_success(context);
}

unit_test_result_t assert_eq_ac_item(unit_test_context_t *context, const char *tag, const c_ast_t *check, const c_ast_t *model, int depth)
{
  /* TODO */
  return assert_success(context);
}

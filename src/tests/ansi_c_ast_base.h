/*
 * opencurry: tests/ansi_c_ast_base.h
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
 * tests/ansi_c_base.h
 * -------------------
 *
 * Helper methods for testing with the "ansi_c_base" module.
 */

#ifndef TESTS_ANSI_C_AST_BASE_H
#define TESTS_ANSI_C_AST_BASE_H
#include "../base.h"
#include "testing.h"

#include "../ansi_c_ast_base.h"

unit_test_result_t assert_eq_c_ast(unit_test_context_t *context, const char *tag, const c_ast_t *check, const c_ast_t *model);

unit_test_result_t assert_eq_ac_anno(unit_test_context_t *context, const char *tag, const ac_anno_t *check, const ac_anno_t *model, int depth);
unit_test_result_t assert_eq_ac_item(unit_test_context_t *context, const char *tag, const ac_item_t *check, const ac_item_t *model, int depth);

#endif /* ifndef TESTS_ANSI_C_AST_BASE_H */

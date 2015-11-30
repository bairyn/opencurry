/*
 * opencurry: tests/test_type_base_lookup.h
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
 * tests/test_type_base_lookup.h
 * ------
 */

#ifndef TESTS_TEST_TYPE_BASE_LOOKUP_H
#define TESTS_TEST_TYPE_BASE_LOOKUP_H
#include "../base.h"
#include "testing.h"

#include "../type_base_lookup.h"

#include "../util.h"

int test_type_base_lookup_cli(int argc, char **argv);

extern unit_test_t type_base_lookup_test;
extern unit_test_t *type_base_lookup_tests[];

unit_test_result_t test_type_base_lookup_run(unit_test_context_t *context);

/* ---------------------------------------------------------------- */

#define CHECKED_LOOKUP_NUM_USED_VALUES(lookup) \
  checked_lookup_num_used_values(context, &result, lookup)

#define CHECKED_LOOKUP_NUM_USED_NODES(lookup) \
  checked_lookup_num_used_nodes(context, &result, lookup)

#define CHECKED_LOOKUP_INT_LEN(lookup) \
  checked_lookup_int_len(context, &result, lookup)

size_t checked_lookup_num_used_values(unit_test_context_t *context, unit_test_result_t *out_result, const lookup_t *lookup);
size_t checked_lookup_num_used_nodes (unit_test_context_t *context, unit_test_result_t *out_result, const lookup_t *lookup);

unit_test_result_t assert_lookup_int_ordered_from          (ASSERT_PARAMS, const lookup_t *lookup, const bnode_t *bnode);
unit_test_result_t assert_lookup_int_ordered               (ASSERT_PARAMS, const lookup_t *lookup);

unit_test_result_t assert_lookup_sufficiently_balanced_from(ASSERT_PARAMS, const lookup_t *lookup, const bnode_t *bnode, int *out_height, size_t *out_num);
unit_test_result_t assert_lookup_sufficiently_balanced     (ASSERT_PARAMS, const lookup_t *lookup, int *out_height, size_t *out_num);

size_t checked_lookup_int_len(unit_test_context_t *context, unit_test_result_t *out_result, const lookup_t *lookup);

/* ---------------------------------------------------------------- */

extern unit_test_t lookup_memory_management_test;
unit_test_result_t lookup_memory_management_test_run(unit_test_context_t *context);

extern unit_test_t lookup_insert_test;
unit_test_result_t lookup_insert_test_run(unit_test_context_t *context);

extern unit_test_t lookup_insert_delete_test;
unit_test_result_t lookup_insert_delete_test_run(unit_test_context_t *context);

extern unit_test_t lookup_minsert_test;
unit_test_result_t lookup_minsert_test_run(unit_test_context_t *context);

extern unit_test_t lookup_accessing_test;
unit_test_result_t lookup_accessing_test_run(unit_test_context_t *context);

#endif /* ifndef TESTS_TEST_TYPE_BASE_LOOKUP_H */

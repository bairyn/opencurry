/*
 * opencurry: tests/test_util.h
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
 * tests/test_util.h
 * ------
 */

#ifndef TESTS_TEST_UTIL_H
#define TESTS_TEST_UTIL_H
#include "../base.h"
#include "testing.h"

#include "../util.h"

int test_util_cli(int argc, char **argv);

extern unit_test_t util_test;
extern unit_test_t *util_tests[];

unit_test_result_t test_util_run(unit_test_context_t *context);

/* ---------------------------------------------------------------- */

extern unit_test_t util_equalities_num_test;
unit_test_result_t util_equalities_num_test_run(unit_test_context_t *context);

extern unit_test_t util_equalities_nullterm_test;
unit_test_result_t util_equalities_nullterm_test_run(unit_test_context_t *context);

extern unit_test_t util_equalities_str_test;
unit_test_result_t util_equalities_str_test_run(unit_test_context_t *context);

#endif /* ifndef TESTS_TEST_UTIL_H */

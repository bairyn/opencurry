/*
 * opencurry: tests/test_type_base_memory_tracker.c
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
#include "test_type_base_memory_tracker.h"

#include "../type_base_memory_tracker.h"

int test_type_base_memory_tracker_cli(int argc, char **argv)
{
  return run_test_suite(type_base_memory_tracker_test);
}

/* ---------------------------------------------------------------- */

/* type_base_memory_tracker tests. */
unit_test_t type_base_memory_tracker_test =
  {  test_type_base_memory_tracker_run
  , "test_type_base_memory_tracker"
  , "type_base_memory_tracker tests."
  };

/* Array of type_base_memory_tracker tests. */
unit_test_t *type_base_memory_tracker_tests[] =
  { &memory_tracking_test

  , NULL
  };

unit_test_result_t test_type_base_memory_tracker_run(unit_test_context_t *context)
{
  return run_tests(context, type_base_memory_tracker_tests);
}

/* ---------------------------------------------------------------- */

unit_test_t memory_tracking_test =
  {  memory_tracking_test_run
  , "memory_tracking_test"
  , "Testing memory tracking."
  };

unit_test_result_t memory_tracking_test_run(unit_test_context_t *context)
{
  unit_test_result_t result = assert_success(context);

  memory_tracker_t  memory_tracker;
  memory_tracker_t *tracker;

  tracker = memory_tracker_init(&memory_tracker, NULL, NULL);

  ENCLOSE()
  {
    ASSERT1( true, IS_TRUE(tracker) );

    COMPOUND_SKIP_CONT();
  }

  ENCLOSE()
  {
    ASSERT2( not_inteq, memory_tracker_free(tracker), 0 );
  }

  return result;
}

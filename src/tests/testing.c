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

/*
 * stdio.h:
 *  - fprintf
 *  - stdout
 *  - stderr
 */
#include <stdio.h>

#include "../base.h"
#include "testing.h"

#define TEST_ERR_BUF_SIZE 4096

int run_tests(unit_test_t *tests)
{
  unit_test_t *test;
  char err_buf[TEST_ERR_BUF_SIZE];

  for(test = tests; *test; ++test)
  {
    int result = (*test)(err_buf, TEST_ERR_BUF_SIZE);

    if(!(result == 0))
    {
      err_buf[TEST_ERR_BUF_SIZE-1] = 0;

      fprintf(stderr, "FAIL: test group #%d failed:\n%s", (int) (test-tests), (const char *) err_buf);
      return result;
    }
  }

  fprintf(stdout, "pass: %d/%d test groups.\n", (int) (test-tests), (int) (test-tests));
  return 0;
}

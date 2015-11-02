/*
 * opencurry: util.h
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
 * util.h
 * ------
 */


#ifndef UTIL_H
#define UTIL_H
#include "base.h"

/* stddef.h:
 *   - size_t
 */
#include <stddef.h>

#define DEFAULT_ERR_BUF_SIZE 8096

/* ---------------------------------------------------------------- */

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define MAX(a, b) (((a) >  (b)) ? (a) : (b))

int min_int(int a, int b);
int max_int(int a, int b);

size_t min_size(size_t a, size_t b);
size_t max_size(size_t a, size_t b);

/* ---------------------------------------------------------------- */

int snprintf(char *str, size_t size, const char *format, ...);
/* TODO */
/*
int snprintf_prepend(char *str, size_t size, const char *format, ...);
int snprintf_append(char *str, size_t size, const char *format, ...);
int snprintf_insert(char *str, size_t size, size_t offset, const char *format, ...);
*/

/* Should be called only from locations that shouldn't be reached. */
void report_bug(const char *msg);

size_t size_minus(size_t from, size_t subtract_by);

size_t terminator_size(size_t total_size);

void ensure_ascii_null_terminated(char *buf, size_t buf_size);

#define ASCII_ENDS_IN_CHAR_DEFAULT_ENDING ('\n')
int ascii_ends_in_char(const char *null_terminated_str, char ending);
int ensure_ascii_ends_in_char(char *buf, size_t buf_size, char ending);

#endif /* ifndef UTIL_H */

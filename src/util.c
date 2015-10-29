/*
 * opencurry: util.c
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
 *  - stderr
 */
#include <stdio.h>

/* string.h:
 *   - strlen
 */
#include <string.h>

#include "base.h"
#include "util.h"

int min_int(int a, int b)
{
  return MIN(a, b);
}

int max_int(int a, int b)
{
  return MAX(a, b);
}

size_t min_size(size_t a, size_t b)
{
  return MIN(a, b);
}

size_t max_size(size_t a, size_t b)
{
  return MAX(a, b);
}

/* ---------------------------------------------------------------- */

/* ANSI C implementation of snprintf. */
int snprintf_TODO(char *str, size_t size, const char *format, ...)
{
  /* TODO! */
  return 0;
}

/* Should be called only from locations that shouldn't be reached. */
void report_bug(const char *msg)
{
  fprintf(stderr, "**BUG: %s\n", msg);
}

/*
 * Return "total_size" - 1, unless "total_size" is 0, in which case 0 is
 * returned.
 *
 * Makes room for a terminator or other character.
 */
size_t terminator_size(size_t total_size)
{
  if (total_size == 0)
    return 0;
  else
    return total_size - 1;
}

/*
 * Subtract from a "size_t", resulting in "0" if the amount to subtract by is
 * greater than the value, without overflowing.
 */
size_t size_minus(size_t from, size_t subtract_by)
{
  if (subtract_by >= from)
    return 0;
  else
    return from - subtract_by;
}

/* If the buf is at max capacity, write a NULL byte if it doesn't end in one,
 * even if it overwrites another byte.
 */
void ensure_ascii_null_terminated(char *buf, size_t buf_size)
{
  if (buf_size <= 0)
    return;

  buf[buf_size - 1] = 0;
}

/* Set "ending" to 0 to default to ASCII_ENDS_IN_CHAR_DEFAULT_ENDING. */
/* 1 if true, 0 if false. */
int ascii_ends_in_char(const char *null_terminated_str, char ending)
{
  int         match;
  const char *str;

  if (!null_terminated_str)
    return 0;

  if (!ending)
    ending = ASCII_ENDS_IN_CHAR_DEFAULT_ENDING;

  match = 0;
  for (str = null_terminated_str; *str; ++str)
  {
    match = *str == ending;
  }

  return match;
}

/*
 * Add a trailing newline or other character (followed by a new terminating
 * NULL byte) to a null-terminated string if it doesn't end in one, if there is
 * space available for the extra byte and the new null terminating byte.
 *
 * Returns:
 *   1:  Already ends with "ending" || '\n'.
 *   0:  An ending character was written.
 *   -1: There is no room to insert a null-terminated "ending" || '\n' char.
 *   -2: "buf" is NULL.
 */
int ensure_ascii_ends_in_char(char *buf, size_t buf_size, char ending)
{
  size_t i;

  int    match;

  if (!buf)
    return -2;

  if (!ending)
    ending = ASCII_ENDS_IN_CHAR_DEFAULT_ENDING;

  /* Loop until we reach a NULL terminator so long as there is room to insert
   * another byte.
   */
  match = 0;
  for (i = 0; i < terminator_size(buf_size); ++i)
  {
    char c = buf[i];

    if (!c)
    {
      /* We've reached the end of the string, and we have room to insert
       * another char if we need to.
       */
      if (match)
      {
        /* Already ends with char. */
        return 1;
      }
      else
      {
        /* Add char. */
        buf[i]     = ending;
        buf[i + 1] = 0;

        return 0;
      }
    }

    match = c == ending;
  }

  /* There is no room to insert another character. */

  if (match)
  {
    /* Already ends with char. */
    return 1;
  }
  else
  {
    /* No room to insert char. */
    return -1;
  }
}

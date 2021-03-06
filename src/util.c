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

/* stddef.h:
 *   - NULL
 *   - ptrdiff_t
 *   - size_t
 */
#include <stddef.h>

/*
 * stdio.h:
 *  - fprintf
 *  - stderr
 */
#include <stdio.h>

/*
 * stdlib.h:
 *  - malloc
 *  - free
 */
#include <stdlib.h>

/* string.h:
 *   - strlen
 *   - strncpy
 *   - memmove
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

void *offset_ref(void *base, ptrdiff_t offset)
{
  return OFFSET_REF(base, offset);
}

const void *offset_cref(const void *base, ptrdiff_t offset)
{
  return OFFSET_CREF(base, offset);
}

void *deref_ptr(void **ptr)
{
  return DEREF_PTR(ptr);
}

void *deref_default(void **ptr, void *when_default)
{
  return DEREF_DEFAULT(ptr, when_default);
}

/* ---------------------------------------------------------------- */

/* xxyyzz000000 */
/* =>           */
/* xx00yy00zz00 */
void *pad(void *buf, size_t num, size_t from_elem_size, size_t to_elem_size)
{
  size_t i;
  size_t distance;

  unsigned char *bytes = buf;

  if (!buf)
    return NULL;

  if (from_elem_size > to_elem_size)
    return NULL;

  distance = size_minus(to_elem_size, from_elem_size);

  if (num <= 0 || distance <= 0)
    return buf;

  for (i = 0; i < num; ++i)
  {
    size_t r = size_minus(size_less_null(num), i);

    memmove(bytes + to_elem_size * r, bytes + from_elem_size * r, from_elem_size);
    memset (bytes + to_elem_size * r + from_elem_size, 0x00, distance);
  }

  return buf;
}

/* ---------------------------------------------------------------- */

size_t size_less_null(size_t size)
{
  return SIZE_LESS_NULL(size);
}

/* ---------------------------------------------------------------- */

void suppress_uninitialized(void *a)
{
}

/* ---------------------------------------------------------------- */

int proc_true(void)
{
  return 1;
}

int proc_false(void)
{
  return 0;
}

int (*proc_cond(int cond))(void)
{
  if (cond)
    return proc_true;
  else
    return proc_false;
}

void *proc_context(void *context)
{
  return context;
}

int proc_true_context(void *context)
{
  return proc_true();
}

int proc_false_context(void *context)
{
  return proc_false();
}

int are_bytes_reversed(void)
{
  typedef unsigned long multi_t;

  unsigned char  multibyte_integral_bytes[sizeof(multi_t)];
  multi_t       *multibyte_integral;

  multibyte_integral = (multi_t *) (&multibyte_integral_bytes[0]);

  *multibyte_integral = 0x01;
  if (multibyte_integral_bytes[0] == 0)
    return 1;
  else
    return 0;
}

static int is_big_endian_lazy_eval(void);
static int (*is_big_endian_lazy)(void) = is_big_endian_lazy_eval;
static int is_big_endian_lazy_eval(void)
{
  return (is_big_endian_lazy = proc_cond(!are_bytes_reversed()))();
}

int is_big_endian(void)
{
  return is_big_endian_lazy();
}

/* ---------------------------------------------------------------- */

void *tail_index(size_t chunk_num, void **root, size_t index)
{
  void *base = *root;

  if (!root || chunk_num <= 0)
    return NULL;

  while (index >= chunk_num)
  {
    index -= chunk_num;
    base = ((void **) base)[SIZE_LESS_NULL(chunk_num)];
  }

  return ((void **) base)[index];
}

/* ---------------------------------------------------------------- */

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

/* Return the quantity by which the two sizes differ.
 */
size_t size_distance(size_t from, size_t to)
{
  if (to >= from)
    return to - from;
  else
    return from - to;
}

/*
 * Return "total_size" - 1, unless "total_size" is 0, in which case 0 is
 * returned.
 *
 * Makes room for a terminator or other character.
 */
size_t terminator_size(size_t total_size)
{
  if (total_size <= 0)
    return 0;
  else
    return total_size - 1;
}

/* ---------------------------------------------------------------- */

/* Returns length. */
size_t strn_reverse(char *buf, size_t n)
{
  size_t i;
  size_t len;

#ifdef ERROR_CHECKING
  if (!buf)
    return 0;
#endif /* #ifdef ERROR_CHECKING */

  len = strllen(buf, n);

  for (i = 0; i < len / 2; ++i)
  {
    SWAP_NO_OVERLAP(buf[i], buf[(len-i) - 1]);
  }

  return len;
}

/* Reverses an array of fixed-width elements. */
/*                                            */
/* Returns length (elem_num).                 */
size_t mem_reverse(void *mem, size_t elem_size, size_t elem_num)
{
  size_t i;
  unsigned char *bytes;

#ifdef ERROR_CHECKING
  if (!mem)
    return 0;
#endif /* #ifdef ERROR_CHECKING */

  bytes = mem;

  for (i = 0; i < elem_size; ++i)
  {
    size_t j;

    for (j = 0; j < elem_num / 2; ++j)
    {
      size_t elem = elem_size * j;

      SWAP_WITH_TMP(unsigned char, bytes[elem + i], bytes[(elem_num-1) - elem + i]);
    }
  }

  return elem_num;
}

/* For each element in an array, reverse the bytes. */
/*                                                  */
/* Returns length (elem_num).                       */
size_t endianness_reverse(void *mem, size_t elem_size, size_t elem_num)
{
  size_t i;
  unsigned char *bytes;

#ifdef ERROR_CHECKING
  if (!mem)
    return 0;
#endif /* #ifdef ERROR_CHECKING */

  for (i = 0, bytes = mem; i < elem_num; ++i, bytes += elem_size)
  {
    size_t j;

    for (j = 0; j < elem_size / 2; ++j)
    {
      SWAP_WITH_TMP(unsigned char, bytes[j], bytes[(elem_size-1) - j]);
    }
  }

  return elem_num;
}

/* Reverse each element in an array,           */
/* and also reverse the bytes in each element. */
/*                                             */
/* Returns length (elem_num).                  */
size_t mem_endianness_reverse(void *mem, size_t elem_size, size_t elem_num)
{
  if (!mem_reverse(mem, 1, elem_size * elem_num))
    return 0;

  return elem_num;
}

size_t unum_digits(unsigned long num, int base)
{
  size_t len;

#ifdef ERROR_CHECKING
  if (base <= 0)
    return 0;
#endif /* #ifdef ERROR_CHECKING */

  for (len = 1; num >= base; num /= base, ++len)
    ;

  return len;
}

size_t num_digits(signed long num, int base, int force_sign_symbol)
{
  if (num < 0)
    return unum_digits((unsigned long) -num, base) + 1;
  else
    return unum_digits((unsigned long)  num, base) + IS_TRUE(force_sign_symbol);
}

const char *digits_base62(void)
  { return digits_base62_def; }

size_t      num_digits_base62 (void)
  { return num_digits_base62_def; }

size_t      max_base_base62   (void)
  { return max_base_base62_def; }

size_t      min_base_base62   (void)
  { return min_base_base62_def; }

size_t      max_digit_base62  (void)
  { return max_digit_base62_def; }

size_t      digits_base62_size(void)
  { return digits_base62_size_def; }

size_t      digits_base62_num (void)
  { return digits_base62_num_def; }

size_t      digits_base62_len (void)
  { return digits_base62_len_def; }

const char digits_base62_def[] =
  DIGITS_BASE62;
const size_t num_digits_base62_def  = NUM_DIGITS_BASE62;
const size_t max_base_base62_def    = MAX_BASE_BASE62;
const size_t min_base_base62_def    = MIN_BASE_BASE62;
const size_t max_digit_base62_def   = MAX_DIGIT_BASE62;
const size_t digits_base62_size_def = ARRAY_SIZE   (digits_base62_def);
const size_t digits_base62_num_def  = ARRAY_NUM    (digits_base62_def);
const size_t digits_base62_len_def  = ARRAY_LEN_ALL(digits_base62_def);

size_t udigits_decimal_buf_size(void)
  { return udigits_decimal_buf_size_def; }
size_t digits_decimal_buf_size(void)
  { return digits_decimal_buf_size_def; }
const size_t udigits_decimal_buf_size_def = UDIGITS_DECIMAL_BUF_SIZE;
const size_t digits_decimal_buf_size_def  = DIGITS_DECIMAL_BUF_SIZE;

size_t udigits_buf_size(void)
  { return udigits_buf_size_def; }
size_t digits_buf_size(void)
  { return digits_buf_size_def; }
const size_t udigits_buf_size_def = UDIGITS_BUF_SIZE;
const size_t digits_buf_size_def  = DIGITS_BUF_SIZE;

char digit(int digit, int base)
{
  base  = CLAMP_INTERVAL(base, MIN_BASE_BASE62, MAX_BASE_BASE62);
  digit = CLAMP_INTERVAL(digit, 0, SIZE_LESS_NULL(base));
  digit = CLAMP_INTERVAL(digit, 0, MAX_DIGIT_BASE62);

  return digits_base62_def[digit];
}

size_t uitoa(char *dest, size_t n, unsigned long num, int base)
{
  size_t i;
  size_t digits_written;

  base = CLAMP_INTERVAL(base, MIN_BASE_BASE62, MAX_BASE_BASE62);
#ifdef ERROR_CHECKING
  if (base < 2)
    return 0;
  if (base >= strlen(digits_base62_def))
    return 0;
#endif /* #ifdef ERROR_CHECKING */

  if (!dest)
    return unum_digits(num, base);

  if (n <= 0)
    return 0;

  digits_written = 0;

  for (i = 0; i < SIZE_LESS_NULL(n); ++i)
  {
    dest[i] = digits_base62_def[num % base];
    num /= base;
    ++digits_written;

    if (num <= 0)
      break;
  }

  dest[digits_written] = 0;

  if (!mem_reverse(dest, 1, digits_written))
    return 0;

  return digits_written;
}

char isign(signed long num)
{
  return ISIGN(num);
}

size_t itoa(char *dest, size_t n, signed long num, int base, int force_sign_symbol)
{
  base = CLAMP_INTERVAL(base, MIN_BASE_BASE62, MAX_BASE_BASE62);
#ifdef ERROR_CHECKING
  if (base < 2)
    return 0;
  if (base >= strlen(digits_base62_def))
    return 0;
#endif /* #ifdef ERROR_CHECKING */

  if (!dest)
    return num_digits(num, base, force_sign_symbol);

  if (n <= 0)
    return 0;

  if (n <= 1)
  {
    *dest = 0;
    return 0;
  }

  if (num < 0)
  {
    *dest++ = ISIGN(num);
    --n;

    return uitoa(dest, n, (unsigned long) -num, base) + 1;
  }
  else if(force_sign_symbol)
  {
    *dest++ = ISIGN(num);
    --n;

    return uitoa(dest, n, (unsigned long)  num, base) + 1;
  }
  else
  {
    return uitoa(dest, n, (unsigned long)  num, base);
  }
}

/* ---------------------------------------------------------------- */

/* Returns <= -1 on error, such as when "buf_size" is too small to set a null
 * terminator (this happens when "buf_size" is 0).
 *
 * Returns >= 0 on success when a NULL terminator was set, >= 1 in case of
 * truncation.
 *
 * Return value:
 *   -2: "buf" is NULL.
 *   -1: "buf_size" is 0: no room to set NULL terminator.
 *    0: NULL terminator set without truncation.
 *    1: "len_before_terminator" is greater than or equal to "buf_size",
 *       so setting the final byte in the buffer to 0 truncated the final
 *       byte.
 *    2: "len_before_terminator" is greater than or equal to "buf_size",
 *       so setting the final byte in the buffer to 0 truncated the final
 *       byte.
 */
int set_null_terminator(char *buf, size_t len_before_terminator, size_t buf_size)
{
  int    truncated = 0;
  size_t buf_terminator_size;

  if (!buf)
    return -2;

  if (buf_size <= 0)
    return -1;

  buf_terminator_size = size_less_null(buf_size);

  if (len_before_terminator > buf_terminator_size)
  {
    truncated = (int) (((int) (buf_terminator_size - len_before_terminator)) + 1);
    if (!(truncated >= 1))
      truncated = 1;

    len_before_terminator = buf_terminator_size;
  }

  if (len_before_terminator <= 0)
    len_before_terminator = 0;

  buf[len_before_terminator] = 0;

  if (!truncated)
    return 0;
  else
    return truncated;
}

/* Like strncpy, except writes a NULL terminator at max capacity. */
/* Returns number of bytes written, excluding the NULL terminator. */
size_t strlcpy(char *dest, const char *src, size_t dest_size)
{
  size_t len;

  if (dest_size <= 0)
    return 0;

  len = 0;
  for (; dest_size >= 2; ++dest, ++src, --dest_size)
  {
    if (!*src)
      break;

    *dest = *src;
    ++len;
  }

  *dest = 0;

  return len;
}

/* strlcpy with a limit on bytes to copy from "src". */
size_t strlcpy_srcmax(char *dest, const char *src, size_t dest_size, size_t src_max_bytes)
{
  size_t len;

  /* Check "dest_size" before adding "src_max_bytes" constraint, because
   * the latter could be 0 while the former could be positive, in which case we
   * still need to write the null terminator.
   */
  if (dest_size     <= 0)
    return 0;

  len = 0;
  for (; dest_size >= 2 && src_max_bytes >= 1; ++dest, ++src, --dest_size, --src_max_bytes)
  {
    if (!*src)
      break;

    *dest = *src;
    ++len;
  }

  *dest = 0;

  return len;
}

/* strlcpy with 2 limits on number of bytes to copy from "src". */
size_t strlcpy_with_max(char *dest, const char *src, size_t dest_size, size_t src_size, size_t src_max_bytes)
{
  return strlcpy_srcmax(dest, src, dest_size, min_size(src_size, src_max_bytes));
}

/* Returns the length of a NULL-terminated string. */
size_t strzlen(const char *src)
{
  const char *p;

  if (!src)
    return 0;

  for (p = src; *p; ++p)
    ;

  return p - src;
}

/* Get the length of the string limited by "size".
 *
 * The string can be at maximum capacity without a null terminator.
 */
size_t strnlen(const char *src, size_t size)
{
  size_t len;

  if (!src)
    return 0;

  len = 0;
  for (; size >= 1; --size)
  {
    if (!*src++)
      break;

    ++len;
  }

  return len;
}

/* Ensure the last byte is reserved for a NULL terminator, treating
 * max-capacity strings as null-terminated regardless of the actual contents of
 * the final byte.
 *
 * Like "strnlen" but assumes max-capacity strings are null-terminated.
 */
size_t strllen(const char *src, size_t size)
{
  size_t len;

  if (!src)
    return 0;

  len = 0;
  for (; size >= 2; --size)
  {
    if (!*src++)
      break;

    ++len;
  }

  return len;
}

int strn_has_null_terminator(const char *src, size_t size)
{
  if (!src)
    return 0;

  return strnlen(src, size) < size;
}

/* Copy "num_bytes" from an infinitely repeated "src" string.
 *
 * Always writes NULL terminator when dest_size is non-zero.
 *
 * Returns number of bytes written, excluding the NULL terminator.
 *
 * If "src" is empty, write an empty, NULL-terminated string.
 */
size_t strlcpy_cycle(char *dest, const char *src, size_t dest_size, size_t num_bytes)
{
  size_t len;
  const char *src_pos;

  if (!dest || dest_size <= 0)
    return 0;

  if (!src || !*src)
  {
    *dest = 0;
    return 0;
  }

  src_pos = src;
  for (len = 0; dest_size >= 2 && num_bytes >= 1; ++len, --dest_size, --num_bytes)
  {
    if (!*src_pos)
      src_pos = src;

    *dest++ = *src_pos++;
  }

  *dest = 0;

  return len;
}

size_t strlappendc(char *dest, size_t dest_size, char byte)
{
  size_t i;

  if (!dest)
    return 0;

  if (dest_size <= 0)
    return 0;

  for (i = 0; i < size_less_null(size_less_null(dest_size)); ++i)
  {
    if (!dest[i])
    {
      dest[i++] = byte;
      break;
    }
  }

  dest[i] = 0;

  return i;
}

size_t strlappendn(char *dest, size_t dest_size, const char *src, size_t src_size)
{
  size_t i;

  if (!dest)
    return 0;

  if (dest_size <= 0)
    return 0;

  for (i = 0; i < size_less_null(dest_size); ++i)
    if (!dest[i])
      break;

  if ((src) && (src_size >= 1))
  {
    /* Handle overlapping memory. */
    if (  ( dest <= src + size_less_null(src_size)                    )
       && (         src                            < dest + dest_size )
       )
    {
      src_size = strllen(src, src_size);
    }

    while (i < size_less_null(dest_size))
    {
      if (!*src)
        break;

      dest[i++] = *src++;

      if (--src_size <= 0)
        break;
    }
  }

  dest[i] = 0;

  return i;
}

size_t strlappendz(char *dest, size_t dest_size, const char *src)
{
  size_t i;

  if (!dest)
    return 0;

  if (dest_size <= 0)
    return 0;

  for (i = 0; i < size_less_null(dest_size); ++i)
    if (!dest[i])
      break;

  if (src)
  {
    size_t size = size_less_null(dest_size);

    /* Handle overlapping memory. */
    if (  ( dest <= src + dest_size                    )
       && (         src             < dest + dest_size )
       )
    {
      size = min_size(size, i + strlen(src));
    }

    while (i < size)
    {
      if (!*src)
        break;

      dest[i++] = *src++;
    }
  }

  dest[i] = 0;

  return i;
}

/*
 * Given "size", return an in-bounds "index" (or an out-of-bounds index of 0
 * when size is 0, or size is 1 and reserve_final_byte is enabled).
 *
 * If "index" is in-bounds, return it; else return the closest in-bounds index
 * (i.e., the maximum index).
 *
 * Return a new index value within [0, size), such that 0 <= new_index < size.
 *
 * E.g. if "size" is 8, returns an index [0, 7].
 *
 * Example:
 *   size  = 8:
 *   Return index constrained to [0, 7].
 *   > if (index <= 7)
 *   >   return index;
 *   > else
 *   >   return 7;
 *
 * If "reserve_final_byte" is enabled, the final index is considered
 * out-of-bounds, so the maximum in-bounds index is one less.
 *
 * Example:
 *   size  = 8:
 *   reserve_final_byte: 1
 *   Return index constrained to [0, 6].
 *   > if (index <= 6)
 *   >   return index;
 *   > else
 *   >   return 6;
 *
 * (In an interval, brackets, "[]", are inclusive, and parentheses, "()" are
 * exclusive.)
 */
size_t index_bounds(size_t size, int is_final_byte_out_of_bounds, size_t index)
{
  size_t max_index;

  if (!is_final_byte_out_of_bounds)
    max_index = size_minus(               size,  1);
  else
    max_index = size_minus(size_less_null(size), 1);

  return min_size(max_index, index);
}

/*
 * Behaves like "index_bounds" with 2 bounds: the string length and "size" (for
 * str).
 */
size_t strn_index_bounds(const char *str, size_t size, int is_terminator_out_of_bounds, size_t index)
{
  index = index_bounds(strnlen(str, size) + 1, is_terminator_out_of_bounds, index);
  index = index_bounds(size,                   is_terminator_out_of_bounds, index);

  return index;
}

/* Like "strn_index_bounds", but assume "str" is NULL terminated.
 */
size_t str_index_bounds(const char *str, int is_terminator_out_of_bounds, size_t index)
{
  return index_bounds(strlen(str) + 1, is_terminator_out_of_bounds, index);
}

/*
 * strinsert_fixedwidth:
 *
 * Prepend or insert a buffer to or inside another.
 *
 * dest_size and src_size specify the maximum size available for each buffer.
 *
 * dest_num and src_num specify the number of bytes used in each buffer.
 *
 * To prepend "src" to "dest", set "dest_insert_offset" to 0 and
 * "insert_relative_to" to "strpos_beginning".
 *
 * To append "src" to "dest", set "dest_insert_offset" to 0 and
 * "insert_relative_to" to "strpos_after_end".
 *
 * To insert "src" at an arbitrary position in "dest", set "dest_insert_offset"
 * to 0 and "insert_relative_to" accordingly.  If "insert_relative_to" is set
 * to "strpos_after_end", then "dest_insert_offset" refers to the number
 * of bytes before the "end" of "dest": where, more specifically, the "end" of
 * "dest" refers to the position in "dest" immediately after the last byte in
 * "dest" when "dest" is not empty, and the first byte otherwise.
 *
 * This procedure cleanly handles out-of-bounds input, and will never access
 * either buffer outside their respective bounds specified by "dest_size" or
 * "src_size" (even if "dest_num" or "src_num" are greater, in which case they will be
 * constrained according to the size, with each being set to the maximum
 * possible value.)
 *
 * Thus only the buffer sizes dest_size and src_size need to correctly
 * represent the maximum buffer sizes for dest and src, or a size less than the
 * maximum buffer sizes.
 *
 * The number of bytes from the original "dest" buffer lost due to truncation
 * is written to "out_num_truncated_orig_dest" when an output reference is
 * provided (i.e. it is non-NULL).  Likewise for the "src" buffer and
 * "out_num_truncated_src".
 *
 * Thus the total number of bytes lost can be obtained from the sum of these
 * two values.
 *
 * "dest" and "src" can overlap.
 *
 * "src" can be NULL, in which case it is treated as an empty buffer.  This is
 * particularly useful when only removing a substring in "dest".
 *
 * When "dest" is NULL, 0 is returned.
 *
 * "dest_replace_num" can be non-zero to specify the number of characters to
 * effectively remove from "dest" at the specified position before inserting.
 * (Note: If more characters are removed than inserted, trailing characters are
 * not necessarily overwritten or cleared with 0-bytes; instead the new length
 * is represented by the return value.)
 *
 * Returns the new "length" / number of bytes now present in "dest".
 */
/* Returns the new length of "dest", which is "src_len" + "dest_len"
 * constrained by the size available in the destination buffer. */
size_t meminsert
  ( void              *dest
  , const void        *src
  , size_t             dest_size
  , size_t             src_size
  , size_t             dest_num
  , size_t             src_num
  , size_t             dest_replace_num
  , size_t             dest_insert_offset
  , strpos_baseline_t  insert_relative_to
  , size_t            *out_num_truncated_orig_dest
  , size_t            *out_num_truncated_src
  )
{
  size_t new_dest_num;
  size_t dest_remainder_size;
  size_t dest_remainder_num;

  if (!dest)
    return 0;

  if (!src)
  {
    src_size = 0;
    src_num  = 0;
    src      = (const void *) ((const char *) "");
  }

  if (insert_relative_to == strpos_after_end)
  {
    insert_relative_to = strpos_beginning;

    dest_insert_offset = size_minus(dest_num, dest_insert_offset);
  }

  src_num             = index_bounds(src_size,            0, src_num);
  dest_num            = index_bounds(dest_size,           0, dest_num);

  dest_insert_offset  = index_bounds(dest_size,           0, dest_insert_offset);

  if (dest_insert_offset > 0)
  {
    /*
    return meminsert
      ( (void *) (((char *) dest) + dest_insert_offset)
      , src
      , size_minus(dest_size, dest_insert_offset)
      , src_size
      , size_minus(dest_num
      , dest_insert_offset)
      , src_num
      , dest_replace_num
      , 0
      , strpos_beginning
      , out_num_truncated
      );
    */

    dest                = (void *) (((char *) dest) + dest_insert_offset);
    dest_size           = size_minus(dest_size, dest_insert_offset);
    dest_num            = size_minus(dest_num,  dest_insert_offset);
    dest_insert_offset  = 0;
    insert_relative_to  = strpos_beginning;
  }

  dest_num            = size_minus(dest_num, dest_replace_num);

  new_dest_num        = index_bounds(dest_size,           0, src_num + dest_num);

  dest_remainder_size = size_minus(dest_size, max_size(src_num, dest_replace_num));
  dest_remainder_num  = index_bounds(dest_remainder_size, 0, dest_num);

  /* Calculate the number of truncated bytes from the original "dest" buffer. */
  if (out_num_truncated_orig_dest)
  {
    if (dest_num <= dest_remainder_num)
      *out_num_truncated_orig_dest = (size_t) 0;
    else
      *out_num_truncated_orig_dest = (size_t) size_minus(dest_num, dest_remainder_num);
  }

  /* Calculate the number of bytes from "src" truncated (lost due to there not
   * being enough room). */
  if (out_num_truncated_src)
  {
    if (src_num <= dest_size)
      *out_num_truncated_src = (size_t) 0;
    else
      *out_num_truncated_src = (size_t) size_minus(src_num, dest_size);
  }

  /* Copy existing contents of "dest" forwards, or backwards, in "dest" to make room, or remove characters. */
  if (size_distance(dest_replace_num, src_num) != 0)
  {
    memmove((void *) (((char *) dest) + src_num), (const void *) (((char *) dest) + dest_replace_num), (size_t) dest_remainder_num);
  }

  /* Prepend "src". */
  if (src_num >= 1)
  {
    memmove((void *) dest, (const void *) src, (size_t) src_num);
  }

  return new_dest_num;
}

/*
 * strlinsert:
 *
 * Insert one NULL-terminated string into another.
 *
 * This adds NULL-terminator handling to "meminsert".
 */
size_t strlinsert
  ( char              *dest
  , const char        *src
  , size_t             dest_size
  , size_t             src_size
  , size_t             dest_max_len
  , size_t             src_max_len
  , size_t             dest_replace_num
  , size_t             dest_insert_offset
  , strpos_baseline_t  insert_relative_to
  , size_t            *out_num_truncated_orig_dest
  , size_t            *out_num_truncated_src
  )
{
  size_t dest_num;
  size_t src_num;

  size_t inserted_dest_len;

  if (dest)
    dest_num = min_size(dest_max_len, strllen(dest, dest_size));
  else
    dest_num = 0;

  if (src)
    src_num  = min_size(src_max_len,  strnlen(src,  src_size));
  else
    src_num  = 0;

  inserted_dest_len =
    meminsert
      ( dest
      , src
      , dest_size
      , src_size
      , dest_num
      , src_num
      , dest_replace_num
      , dest_insert_offset
      , insert_relative_to
      , out_num_truncated_orig_dest
      , out_num_truncated_src
      );

  if (inserted_dest_len < dest_size)
    dest[inserted_dest_len] = 0;

  return inserted_dest_len;
}

/*
 * "strlinsert" with lengths obtained from null-terminated string lengths.
 *
 * "src" must be a null-terminated string if non-NULL.
 */
size_t strlinsert_len
  ( char              *dest
  , const char        *src
  , size_t             dest_size
  , size_t             dest_replace_num
  , size_t             dest_insert_offset
  , strpos_baseline_t  insert_relative_to
  , size_t            *out_num_truncated_orig_dest
  , size_t            *out_num_truncated_src
  )
{
  size_t inserted_dest_len;

  size_t src_len  = 0;
  size_t dest_len = 0;

  if (src)
    src_len = strlen(src);
  if (dest)
    dest_len = strnlen(dest, dest_size);

  inserted_dest_len =
    strlinsert
      ( dest
      , src
      , dest_size
      , src_len
      , dest_len
      , src_len
      , dest_replace_num
      , dest_insert_offset
      , insert_relative_to
      , out_num_truncated_orig_dest
      , out_num_truncated_src
      );

  if (inserted_dest_len < dest_size)
    dest[inserted_dest_len] = 0;

  return inserted_dest_len;
}

/* Returns number of bytes written. */
size_t strl_replications(char *dest, const char *src, size_t dest_size, size_t num_repetitions)
{
  size_t i;
  size_t len;

  if (!dest)
    return 0;

  *dest = 0;
  len = 0;

  if (!src)
    return len;

  for (i = 0; i < num_repetitions; ++i)
  {
    len =
      strlinsert_len
        ( dest
        , src
        , dest_size
        , 0
        , 0
        , strpos_after_end
        , NULL
        , NULL
        );
  }

  return len;
}

/* Return a pointer to the last num_bytes number of bytes of the
 * NULL-terminated "str".
 *
 * Will not return a string out of "str"'s null-terminated bounds.
 */
const char *last_bytes(const char *str, size_t num_bytes)
{
  size_t len;

  if (!str)
    return NULL;

  len = strlen(str);

  return str + size_minus(len, num_bytes);
}

/* ---------------------------------------------------------------- */

const char   indentation_spaces_buf[] =
  /* 80 * 80 = 6400 */
  /* String literal length is limited to 509 characters. */
  /*
  REPLICATE_80(
    "                                                                                "
  );
  */
  {
    /* REPLICATE_COMMA_80(REPLICATE_COMMA_80(' ')), '\x00' */
    REPLICATE_COMMA_6000(' '), REPLICATE_COMMA_400(' '), '\x00'
  };
const char   indentation_tabs_buf[] =
  /* 80 * 40 = 3200 */
  /*
  REPLICATE_80(
    "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
  );
  */
  {
    REPLICATE_COMMA_3000(' '), REPLICATE_COMMA_200(' '), '\x00'
  };

const size_t indentation_spaces_buf_size =
  sizeof(indentation_spaces_buf);
const size_t indentation_spaces_buf_len  =
  (SIZE_LESS_NULL(sizeof(indentation_spaces_buf)));

const size_t indentation_tabs_buf_size =
  sizeof(indentation_tabs_buf);
const size_t indentation_tabs_buf_len  =
  (SIZE_LESS_NULL(sizeof(indentation_tabs_buf)));

const size_t max_indentation_spaces           = MAX_INDENTATION_SPACES;
const size_t max_indentation_tabs             = MAX_INDENTATION_TABS;
const size_t default_indentation_step_spaces  = DEFAULT_INDENTATION_STEP_SPACES;
const size_t default_indentation_step_tabs    = DEFAULT_INDENTATION_STEP_TABS;

const char *indentation_spaces(size_t steps, size_t step_size)
{
  size_t spaces;

  if (!step_size)
    step_size = default_indentation_step_spaces;

  spaces = index_bounds(indentation_spaces_buf_len, 0, step_size * steps);

  return last_bytes(indentation_spaces_buf, spaces);
}

const char *indentation_tabs(size_t steps, size_t step_size)
{
  size_t tabs;

  if (!step_size)
    step_size = default_indentation_step_tabs;

  tabs = index_bounds(indentation_tabs_buf_len, 0, step_size * steps);

  return last_bytes(indentation_spaces_buf, tabs);
}

/* ---------------------------------------------------------------- */

/* ANSI C implementation of snprintf. */
int snprintf_TODO(char *str, size_t size, const char *format, ...)
{
  /* TODO! */
  return 0;
}

/* TODO */
/*
int snprintf_prepend(char *str, size_t size, const char *format, ...)
{
}

int snprintf_append(char *str, size_t size, const char *format, ...)
{
}

int snprintf_insert(char *str, size_t size, const char *format, ...)
{
}
*/

/* Should be called only from locations that shouldn't be reached. */
void bug(const char *msg)
{
  fprintf(stderr, "**BUG: %s\n", msg);
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

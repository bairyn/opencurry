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

#include "bits.h"

/* TODO: move CPP-specific bits to new module "cpp"! */

/* ---------------------------------------------------------------- */

/*
 * Add a trivial block of code that can contain "break" to skip to the end.
 */
#define ENCLOSE() switch(0) default:

/* ---------------------------------------------------------------- */

/* These correspond to the behaviour of "if". */
#define IS_TRUE(a) (!!(a))
#define IS_FALSE(a) (!(a))

/* ---------------------------------------------------------------- */

#define DEFAULT_ERR_BUF_SIZE 8096

/* ---------------------------------------------------------------- */

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define MAX(a, b) (((a) >  (b)) ? (a) : (b))

int min_int(int a, int b);
int max_int(int a, int b);

size_t min_size(size_t a, size_t b);
size_t max_size(size_t a, size_t b);

/* ---------------------------------------------------------------- */

/* Wrap code inside a single statement to avoid potential pitfalls. */
#define BLOCK_STATEMENT(body) \
  do                          \
  {                           \
    body                      \
  } while(0)

/* ---------------------------------------------------------------- */

#define EXPAND1(a) a

#define EXPAND_PARENS(a) EXPAND1 a
#define CALL_EXPAND_PARENS(f, args) f args

/* ---------------------------------------------------------------- */

/* Thanks https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms ! */

#define BLANK

#define EMPTY()
#define DEFER(a) a EMPTY()

/* ---------------------------------------------------------------- */

/* Macros to quote an argument to make a string literal and macros to combine
 * two arguments into one via concatenation.
 */

/* "LITERAL_STRINGIFY" quotes the argument before expansion.  To allow a macro
 * to be expanded by the C preprocessor before being quoted, use
 * "EXPAND_STRINGIFY".
 */
#define EXPAND_STRINGIFY(a) LITERAL_STRINGIFY(a)
#define LITERAL_STRINGIFY(a) #a

#define EXPAND_CAT(a, b) LITERAL_CAT(a, b)
#define LITERAL_CAT(a, b) a##b

/* Shorter names. */

#define XSTR(a) EXPAND_STRINGIFY(a)
#define LSTR(a) #a /* LITERAL_STRINGIFY(a) */

#define XCAT(a, b) EXPAND_CAT(a, b)
#define LCAT(a, b) a##b /* LITERAL_CAT(a, b) */

#define STR(a)    XSTR(a)
#define CAT(a, b) XCAT(a, b)

#define CAT0()
#define CAT1( a)                                                 a
#define CAT2( a, b)                                              CAT(a, b)
#define CAT3( a, b, c)                                           CAT(a, CAT2 (b, c))
#define CAT4( a, b, c, d)                                        CAT(a, CAT3 (b, c, d))
#define CAT5( a, b, c, d, e)                                     CAT(a, CAT4 (b, c, d, e))
#define CAT6( a, b, c, d, e, f)                                  CAT(a, CAT5 (b, c, d, e, f))
#define CAT7( a, b, c, d, e, f, g)                               CAT(a, CAT6 (b, c, d, e, f, g))
#define CAT8( a, b, c, d, e, f, g, h)                            CAT(a, CAT7 (b, c, d, e, f, g, h))
#define CAT9( a, b, c, d, e, f, g, h, i)                         CAT(a, CAT8 (b, c, d, e, f, g, h, i))
#define CAT10(a, b, c, d, e, f, g, h, i, j)                      CAT(a, CAT9 (b, c, d, e, f, g, h, i, j))
#define CAT11(a, b, c, d, e, f, g, h, i, j, k)                   CAT(a, CAT10(b, c, d, e, f, g, h, i, j, k))
#define CAT12(a, b, c, d, e, f, g, h, i, j, k, l)                CAT(a, CAT11(b, c, d, e, f, g, h, i, j, k, l))
#define CAT13(a, b, c, d, e, f, g, h, i, j, k, l, m)             CAT(a, CAT12(b, c, d, e, f, g, h, i, j, k, l, m))
#define CAT14(a, b, c, d, e, f, g, h, i, j, k, l, m, n)          CAT(a, CAT13(b, c, d, e, f, g, h, i, j, k, l, m, n))
#define CAT15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o)       CAT(a, CAT14(b, c, d, e, f, g, h, i, j, k, l, m, n, o))
#define CAT16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p)    CAT(a, CAT15(b, c, d, e, f, g, h, i, j, k, l, m, n, o, p))
#define CAT17(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q) CAT(a, CAT16(b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q))


#define DUPLICATE(a) CAT(a, a)

/* ---------------------------------------------------------------- */

#define HASH   #
#define COMMA  ,
#define LPAREN (
#define RPAREN )

#define JUX2(a, b) a b
#define RJUX2(a, b) b a

#define APPEND_COMMA( a) a,
#define APPEND_LPAREN(a) a(
#define APPEND_RPAREN(a) a)

#define PREPEND_COMMA( a) ,a
#define PREPEND_LPAREN(a) (a
#define PREPEND_RPAREN(a) )a

#define APPEND_COMMA_0( a) a
#define APPEND_COMMA_1( a) a,
#define APPEND_COMMA_2( a) a,,
#define APPEND_COMMA_3( a) a,,,
#define APPEND_COMMA_4( a) a,,,,
#define APPEND_COMMA_5( a) a,,,,,
#define APPEND_COMMA_6( a) a,,,,,,
#define APPEND_COMMA_7( a) a,,,,,,,
#define APPEND_COMMA_8( a) a,,,,,,,,
#define APPEND_COMMA_9( a) a,,,,,,,,,
#define APPEND_COMMA_10(a) a,,,,,,,,,,
#define APPEND_COMMA_D(d, a) CAT(APPEND_COMMA_, d)(a)

#define APPEND_LPAREN_0( a) a
#define APPEND_LPAREN_1( a) a(
#define APPEND_LPAREN_2( a) a((
#define APPEND_LPAREN_3( a) a(((
#define APPEND_LPAREN_4( a) a((((
#define APPEND_LPAREN_5( a) a(((((
#define APPEND_LPAREN_6( a) a((((((
#define APPEND_LPAREN_7( a) a(((((((
#define APPEND_LPAREN_8( a) a((((((((
#define APPEND_LPAREN_9( a) a(((((((((
#define APPEND_LPAREN_10(a) a((((((((((
#define APPEND_LPAREN_D(d, a) CAT(APPEND_LPAREN_, d)(a)

#define APPEND_RPAREN_0( a) a
#define APPEND_RPAREN_1( a) a)
#define APPEND_RPAREN_2( a) a))
#define APPEND_RPAREN_3( a) a)))
#define APPEND_RPAREN_4( a) a))))
#define APPEND_RPAREN_5( a) a)))))
#define APPEND_RPAREN_6( a) a))))))
#define APPEND_RPAREN_7( a) a)))))))
#define APPEND_RPAREN_8( a) a))))))))
#define APPEND_RPAREN_9( a) a)))))))))
#define APPEND_RPAREN_10(a) a))))))))))
#define APPEND_RPAREN_D(d, a) CAT(APPEND_RPAREN_, d)(a)

#define PREPEND_COMMA_0( a) a
#define PREPEND_COMMA_1( a) ,a
#define PREPEND_COMMA_2( a) ,,a
#define PREPEND_COMMA_3( a) ,,,a
#define PREPEND_COMMA_4( a) ,,,,a
#define PREPEND_COMMA_5( a) ,,,,,a
#define PREPEND_COMMA_6( a) ,,,,,,a
#define PREPEND_COMMA_7( a) ,,,,,,,a
#define PREPEND_COMMA_8( a) ,,,,,,,,a
#define PREPEND_COMMA_9( a) ,,,,,,,,,a
#define PREPEND_COMMA_10(a) ,,,,,,,,,,a
#define PREPEND_COMMA_D(d, a) CAT(PREPEND_COMMA_, d)(a)

#define PREPEND_LPAREN_0( a) a
#define PREPEND_LPAREN_1( a) (a
#define PREPEND_LPAREN_2( a) ((a
#define PREPEND_LPAREN_3( a) (((a
#define PREPEND_LPAREN_4( a) ((((a
#define PREPEND_LPAREN_5( a) (((((a
#define PREPEND_LPAREN_6( a) ((((((a
#define PREPEND_LPAREN_7( a) (((((((a
#define PREPEND_LPAREN_8( a) ((((((((a
#define PREPEND_LPAREN_9( a) (((((((((a
#define PREPEND_LPAREN_10(a) ((((((((((a
#define PREPEND_LPAREN_D(d, a) CAT(PREPEND_LPAREN_, d)(a)

#define PREPEND_RPAREN_0( a) a
#define PREPEND_RPAREN_1( a) )a
#define PREPEND_RPAREN_2( a) ))a
#define PREPEND_RPAREN_3( a) )))a
#define PREPEND_RPAREN_4( a) ))))a
#define PREPEND_RPAREN_5( a) )))))a
#define PREPEND_RPAREN_6( a) ))))))a
#define PREPEND_RPAREN_7( a) )))))))a
#define PREPEND_RPAREN_8( a) ))))))))a
#define PREPEND_RPAREN_9( a) )))))))))a
#define PREPEND_RPAREN_10(a) ))))))))))a
#define PREPEND_RPAREN_D(d, a) CAT(PREPEND_RPAREN_, d)(a)

/* ---------------------------------------------------------------- */

/* ((size >= 1) ? (size - 1) : 0) */
/* max(1, size) - 1               */
/* TODO: FIX BIT_NAT_PRED! */
#define TODO_SIZE_LESS_NULL(size) ((BIT_NAT_PRED((size))))
#define SIZE_LESS_NULL(size) ((!!(size)) * ((size) - 1))
size_t size_less_null(size_t size);

#define ARRAY_SIZE(array)    (sizeof((array)))
#define ARRAY_NUM(array)     ((sizeof((array))) / (sizeof((array)[0])))
/* ARRAY_LEN_ALL treats the last element as a terminator and all other elements as values used in the array even when they are NULL or zero. */
/* Thus this is just the number of elements in the array minus 1. */
#define ARRAY_LEN_ALL(array) ((SIZE_LESS_NULL((ARRAY_NUM(array)))))
#define ARRAY_FINAL_ELEM(array) array[ARRAY_LEN_ALL(array)]
#define ARRAY_FINAL_REF(array) &array[ARRAY_LEN_ALL(array)]

/* ---------------------------------------------------------------- */

int proc_true(void);
int proc_false(void);
int (*
    proc_cond(int cond)
  )(void);

void *proc_context(void *context);

int proc_true_context(void *context);
int proc_false_context(void *context);

int are_bytes_reversed(void);
int is_big_endian(void);

/* ---------------------------------------------------------------- */

void *if_then_else(int condition, void *when_true, void *when_false);

size_t size_minus(size_t from, size_t subtract_by);
size_t size_distance(size_t from, size_t to);

size_t terminator_size(size_t total_size);

/* ---------------------------------------------------------------- */

#ifdef TODO
#error "TODO: move to new "string" OSLT module."
#endif /* #ifdef TODO */

/* https://en.wikipedia.org/wiki/XOR_swap_algorithm */
#define SWAP(a, b) \
  do               \
  {                \
    if (a != b)    \
    {              \
      a ^= b;      \
      b ^= a;      \
      a ^= b;      \
    }              \
  } while(0)

#define SWAP_NO_OVERLAP(a, b) \
  do                          \
  {                           \
    a ^= b;                   \
    b ^= a;                   \
    a ^= b;                   \
  } while(0)

#define SWAP_WITH_TMP(type, a, b)                 \
  do                                              \
  {                                               \
    type _SWAP_WITH_TMP_tmp = a;                  \
    a                       = b;                  \
    b                       = _SWAP_WITH_TMP_tmp; \
  } while(0)

size_t strn_reverse(char *buf, size_t n);

size_t mem_reverse           (void *mem, size_t elem_size, size_t elem_num);
size_t endianness_reverse    (void *mem, size_t elem_size, size_t elem_num);
size_t mem_endianness_reverse(void *mem, size_t elem_size, size_t elem_num);

#define FORCE_SIGN    ENABLED()
#define OPTIONAL_SIGN DISABLED()

size_t unum_digits(unsigned long num, int base);
size_t num_digits (signed   long num, int base, int force_sign_symbol);

#define DIGITS_BASE62 \
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define NUM_DIGITS_BASE62 \
  ((SIZE_LESS_NULL((sizeof((DIGITS_BASE62))))))
#define MAX_BASE_BASE62 \
  (NUM_DIGITS_BASE62)
#define MIN_BASE_BASE62 \
  (MIN((MAX_BASE_BASE62), 2))
#define MAX_DIGIT_BASE62 \
  ((SIZE_LESS_NULL((MAX_BASE_BASE62))))
const char *digits_base62     (void);
size_t      num_digits_base62 (void);
size_t      max_base_base62   (void);
size_t      min_base_base62   (void);
size_t      max_digit_base62  (void);
size_t      digits_base62_size(void);
size_t      digits_base62_num (void);
size_t      digits_base62_len (void);

#define BASE_BINARY      2
#define BASE_OCTAL       8
#define BASE_DECIMAL     10
#define BASE_HEXADECIMAL 16
#define DEFAULT_BASE     BASE_DECIMAL

extern const char digits_base62_def[];
extern const size_t num_digits_base62_def;
extern const size_t max_base_base62_def;
extern const size_t min_base_base62_def;
extern const size_t max_digit_base62_def;
extern const size_t digits_base62_size_def;
extern const size_t digits_base62_num_def;
extern const size_t digits_base62_len_def;

#define ISIGN(num) \
  (SIGN_CASE((num), '-', ' ', '+'))
char isign(signed long num);

#define UDIGITS_DECIMAL_BUF_SIZE ( (MAX(1, (3 * sizeof(unsigned long))))      + 1)
#define DIGITS_DECIMAL_BUF_SIZE  (((MAX(1, (3 * sizeof(unsigned long)))) + 1) + 1)
size_t udigits_decimal_buf_size(void);
size_t digits_decimal_buf_size(void);
extern const size_t udigits_decimal_buf_size_def;
extern const size_t digits_decimal_buf_size_def;

/* (binary) */
#define UDIGITS_BUF_SIZE ( (MAX(1, (8 * sizeof(unsigned long))))      + 1)
#define DIGITS_BUF_SIZE  (((MAX(1, (8 * sizeof(unsigned long)))) + 1) + 1)
size_t udigits_buf_size(void);
size_t digits_buf_size(void);
extern const size_t udigits_buf_size_def;
extern const size_t digits_buf_size_def;

size_t uitoa(char *dest, size_t n, unsigned long num, int base);
size_t itoa (char *dest, size_t n, signed   long num, int base, int force_sign_symbol);

/* ---------------------------------------------------------------- */

int set_null_terminator(char *buf, size_t len_before_terminator, size_t buf_size);

size_t strlcpy(char *dest, const char *src, size_t dest_size);
size_t strlcpy_srcmax(char *dest, const char *src, size_t dest_size, size_t src_max_bytes);
size_t strlcpy_with_max(char *dest, const char *src, size_t dest_size, size_t src_size, size_t src_max_bytes);

size_t strnlen(const char *src, size_t size);
size_t strllen(const char *src, size_t size);

int strn_has_null_terminator(const char *src, size_t size);

size_t strlcpy_cycle(char *dest, const char *src, size_t dest_size, size_t num_bytes);

size_t index_bounds(size_t size, int is_final_byte_out_of_bounds, size_t index);
size_t strn_index_bounds(const char *str, size_t size, int is_terminator_out_of_bounds, size_t index);
size_t str_index_bounds(const char *str, int is_terminator_out_of_bounds, size_t index);

enum strpos_baseline_e
{
  strpos_beginning = 0,
  strpos_after_end = 1,

  /* Number of enumerated "strpos_baseline_t" values. */
  strpos_relative_to_num
};
typedef enum strpos_baseline_e strpos_baseline_t;
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
  );
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
  );
size_t strlinsert_len
  ( char              *dest
  , const char        *src
  , size_t             dest_size
  , size_t             dest_replace_num
  , size_t             dest_insert_offset
  , strpos_baseline_t  insert_relative_to
  , size_t            *out_num_truncated_orig_dest
  , size_t            *out_num_truncated_src
  );

size_t strl_replications(char *dest, const char *src, size_t dest_size, size_t num_repetitions);
const char *last_bytes(const char *str, size_t num_bytes);

/* ---------------------------------------------------------------- */

/* TODO: int / size_t / double functions! */

#define ABS(val) \
  ( ((val) >= 0) \
  ? (val)        \
  : (-(val))     \
  )

/* Assumes a <= b. */
#define DISTANCE_MONOTONIC(from, to) ((to) - (from))
#define DISTANCE(from, to)              \
  ( ((from) <= (to))                             \
  ? (DISTANCE_MONOTONIC((from), (to))   \
  : (DISTANCE_MONOTONIC((to),   (from)) \
  )

#define IS_EVEN(val) (((val) % 2) == 0)
#define IS_ODD(val)  (((val) % 2) != 0)

/* ((integral) + (!!(IS_ODD((integral))))) */
#define INTEGRAL_ROUND_UP_EVEN(integral) \
  ( (IS_EVEN((integral)))                \
  ? (integral)                           \
  : ((integral) + 1)                     \
  )

#define WHEN_LT_EQ_GT(value, baseline, when_lt, when_eq, when_gt) \
  ( ((value) <= (baseline))                                       \
  ? ( (!((value) == (baseline)))                                  \
    ? (when_lt)                                                   \
    : (when_eq)                                                   \
    )                                                             \
  : (when_gt)                                                     \
  )

/* These functions only use the binary "/" operator on a dividend such that the
 * divisor is a multiple of the dividend.
 */
#define DIV_FLOOR(dividend, divisor) (((dividend) - ((dividend) % (divisor))) / (divisor))
#define DIV_CEIL(dividend, divisor)                                        \
  ( (DIV_FLOOR((dividend), (divisor)))                                     \
  + ( ( (((dividend) / (divisor)) <= ((DIV_FLOOR((dividend), (divisor))))) \
      ? 0                                                                  \
      : 1                                                                  \
      )                                                                    \
    )                                                                      \
  )
#define INTEGRAL_DIV_CEIL(dividend, divisor)  ((((dividend) % (divisor)) == 0) ? ((dividend) / (divisor)) : ((DIV_FLOOR((dividend), (divisor))) + 1))

/* If 2 * the remainder equals the divisor, then the mantissa is .5, in which
 * case we return the nearest even integral value. */

/* Examples:
 *    X/4: 2*remainder == 4: result
 *
 *    8/4: 2*0         <  4: 2
 *    9/4: 2*1         <  4: 2
 *   10/4: 2*2         == 4: 2.5 => 2
 *   11/4: 2*3         >  4: 3
 *
 *   12/4: 2*0         <  4: 3
 *   13/4: 2*1         <  4: 3
 *   14/4: 2*2         == 4: 3.5 => 4
 *   15/4: 2*3         >  4: 4
 */
/* Divide and round to the nearest even integer. */
#define DIV_ROUND(dividend, divisor)                                          \
  ( WHEN_LT_EQ_GT(                                                            \
      ( (2 * ((dividend) % (divisor))) )                                      \
      , (divisor)                                                             \
                                                                              \
      ,                         (INTEGRAL_DIV_FLOOR((dividend), (divisor)))   \
      , (INTEGRAL_ROUND_UP_EVEN((INTEGRAL_DIV_FLOOR((dividend), (divisor))))) \
      ,                         (INTEGRAL_DIV_CEIL((dividend),  (divisor)))   \
      )                                                                       \
  )

#define BASE_OFFSET(base, offset) \
  ( base   \
  + offset \
  )

/* Assumes min <= max. */
#define CLAMP_INTERVAL_MONOTONIC(min, max, value) \
  ( ((value) <= (max))                            \
  ? ( ((value) >= (min))                          \
    ? ((value))                                   \
    : ((min))                                     \
    )                                             \
  : ((max))                                       \
  )
#define CLAMP_INTERVAL(value, a, b)               \
  ( ((a) <= (b))                                  \
  ? (CLAMP_INTERVAL_MONOTONIC((a), (b), (value))) \
  : (CLAMP_INTERVAL_MONOTONIC((b), (a), (value))) \
  )
#define CLAMP(value) ((CLAMP_INTERVAL((0), (1), (value))))

#define RANGE_MIDPOINT(distance) \
  ( DIV_ROUND(                   \
      ( distance )               \
      , 2                        \
      )                          \
  )

#define RANGE_LERP(distance, proportion) \
  ((distance) * (proportion))
#define RANGE_LERP_CLAMP_ALT(distance, proportion)  \
  ( RANGE_LERP((distance), (CLAMP((proportion)))) )
#define RANGE_LERP_CLAMP(distance, proportion)      \
  ( CLAMP_INTERVAL(                                 \
      ( (0) )                                       \
      , (distance)                                  \
      , (RANGE_LERP((distance), (proportion)))     \
      )                                             \
  )

#define INTEGRAL_RANGE_LERP(distance, proportion_numerator, proportion_denominator) \
  ( DIV_ROUND(                                                                      \
      ( ((distance) * (proportion_numerator)) )                                     \
      , ((proportion_denominator))                                                  \
      )                                                                             \
  )
#define INTEGRAL_RANGE_LERP_CLAMP(distance, proportion_numerator, proportion_denominator)   \
  ( CLAMP_INTERVAL(                                                                         \
      ( (0) )                                                                               \
      , (distance)                                                                          \
      , (INTEGRAL_RANGE_LERP((distance), (proportion_numerator), (proportion_denominator))) \
      )                                                                                     \
  )

#define MIDPOINT(from, to)             \
 ( (from)                              \
 + ( RANGE_MIDPOINT(                   \
       ( DISTANCE(from, to) )          \
       )                               \
   )                                   \
 )

/* ---------------------------------------------------------------- */

#define REPLICATE_0(a)
#define REPLICATE_1(a)     a
#define REPLICATE_2(a)     a a

#define REPLICATE_3(a)     a REPLICATE_2 (a)
#define REPLICATE_4(a)     a REPLICATE_3 (a)
#define REPLICATE_5(a)     a REPLICATE_4 (a)
#define REPLICATE_6(a)     a REPLICATE_5 (a)
#define REPLICATE_7(a)     a REPLICATE_6 (a)
#define REPLICATE_8(a)     a REPLICATE_7 (a)
#define REPLICATE_9(a)     a REPLICATE_8 (a)
#define REPLICATE_10(a)    a REPLICATE_9 (a)
#define REPLICATE_11(a)    a REPLICATE_10(a)
#define REPLICATE_12(a)    a REPLICATE_11(a)
#define REPLICATE_13(a)    a REPLICATE_12(a)
#define REPLICATE_14(a)    a REPLICATE_13(a)
#define REPLICATE_15(a)    a REPLICATE_14(a)

#define REPLICATE_16(a)    REPLICATE_2(REPLICATE_8   (a))
#define REPLICATE_32(a)    REPLICATE_2(REPLICATE_16  (a))
#define REPLICATE_64(a)    REPLICATE_2(REPLICATE_32  (a))
#define REPLICATE_128(a)   REPLICATE_2(REPLICATE_64  (a))
#define REPLICATE_256(a)   REPLICATE_2(REPLICATE_128 (a))
#define REPLICATE_512(a)   REPLICATE_2(REPLICATE_256 (a))
#define REPLICATE_1024(a)  REPLICATE_2(REPLICATE_512 (a))
#define REPLICATE_2048(a)  REPLICATE_2(REPLICATE_1024(a))
#define REPLICATE_4096(a)  REPLICATE_2(REPLICATE_2048(a))
#define REPLICATE_8192(a)  REPLICATE_2(REPLICATE_4096(a))
#define REPLICATE_16384(a) REPLICATE_2(REPLICATE_8192(a))

#define REPLICATE_20(a)    REPLICATE_10(REPLICATE_2(a))
#define REPLICATE_30(a)    REPLICATE_10(REPLICATE_3(a))
#define REPLICATE_40(a)    REPLICATE_10(REPLICATE_4(a))
#define REPLICATE_50(a)    REPLICATE_10(REPLICATE_5(a))
#define REPLICATE_60(a)    REPLICATE_10(REPLICATE_6(a))
#define REPLICATE_70(a)    REPLICATE_10(REPLICATE_7(a))
#define REPLICATE_80(a)    REPLICATE_10(REPLICATE_8(a))
#define REPLICATE_90(a)    REPLICATE_10(REPLICATE_9(a))

#define REPLICATE_100(a)   REPLICATE_10(REPLICATE_10  (a))
#define REPLICATE_1000(a)  REPLICATE_10(REPLICATE_100 (a))
#define REPLICATE_10000(a) REPLICATE_10(REPLICATE_1000(a))

#define REPLICATE_19(a)    REPLICATE_10(REPLICATE_1(a)) REPLICATE_9(a)
#define REPLICATE_29(a)    REPLICATE_10(REPLICATE_2(a)) REPLICATE_9(a)
#define REPLICATE_39(a)    REPLICATE_10(REPLICATE_3(a)) REPLICATE_9(a)
#define REPLICATE_49(a)    REPLICATE_10(REPLICATE_4(a)) REPLICATE_9(a)
#define REPLICATE_59(a)    REPLICATE_10(REPLICATE_5(a)) REPLICATE_9(a)
#define REPLICATE_69(a)    REPLICATE_10(REPLICATE_6(a)) REPLICATE_9(a)
#define REPLICATE_79(a)    REPLICATE_10(REPLICATE_7(a)) REPLICATE_9(a)
#define REPLICATE_89(a)    REPLICATE_10(REPLICATE_8(a)) REPLICATE_9(a)
#define REPLICATE_99(a)    REPLICATE_10(REPLICATE_9(a)) REPLICATE_9(a)

#define REPLICATE_21(a)                                  REPLICATE_10(REPLICATE_2(a)) REPLICATE_1(a)
#define REPLICATE_31(a)                                  REPLICATE_10(REPLICATE_3(a)) REPLICATE_1(a)
#define REPLICATE_41(a)                                  REPLICATE_10(REPLICATE_4(a)) REPLICATE_1(a)
#define REPLICATE_51(a)                                  REPLICATE_10(REPLICATE_5(a)) REPLICATE_1(a)
#define REPLICATE_61(a)                                  REPLICATE_10(REPLICATE_6(a)) REPLICATE_1(a)
#define REPLICATE_71(a)                                  REPLICATE_10(REPLICATE_7(a)) REPLICATE_1(a)
#define REPLICATE_81(a)                                  REPLICATE_10(REPLICATE_8(a)) REPLICATE_1(a)
#define REPLICATE_91(a)                                  REPLICATE_10(REPLICATE_9(a)) REPLICATE_1(a)
#define REPLICATE_101(a)   REPLICATE_100(REPLICATE_1(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(a)

/* ANSI C conformance require support for string literals of sizes of at least
 * a minimum of 509.
 */
#define REPLICATE_500(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_0(a))
#define REPLICATE_501(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_1(a))
#define REPLICATE_502(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_2(a))
#define REPLICATE_503(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_3(a))
#define REPLICATE_504(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_4(a))
#define REPLICATE_505(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_5(a))
#define REPLICATE_506(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_6(a))
#define REPLICATE_507(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_7(a))
#define REPLICATE_508(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_8(a))
#define REPLICATE_509(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_0(a)) REPLICATE_1(REPLICATE_9(a))
#define REPLICATE_510(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_1(a)) REPLICATE_1(REPLICATE_0(a))
#define REPLICATE_511(a)   REPLICATE_100(REPLICATE_5(a)) REPLICATE_10(REPLICATE_1(a)) REPLICATE_1(REPLICATE_1(a))

/*
#define REPLICATE_DIGITS_1(            d1, a) REPLICATE_##d1(a)
#define REPLICATE_DIGITS_2(        d2, d1, a) REPLICATE_##d1(a) REPLICATE_10(REPLICATE_##d2(a))
#define REPLICATE_DIGITS_3(    d3, d2, d1, a) REPLICATE_##d1(a) REPLICATE_10(REPLICATE_##d2(a)) REPLICATE_100(REPLICATE_##d3(a))
#define REPLICATE_DIGITS_4(d4, d3, d2, d1, a) REPLICATE_##d1(a) REPLICATE_10(REPLICATE_##d2(a)) REPLICATE_100(REPLICATE_##d3(a)) REPLICATE_1000(REPLICATE_##d4(a))
*/

#define REPLICATE_PLUS( num, a, acc) REPLICATE_##num(a) acc
#define REPLICATE_TIMES(num, a, acc) REPLICATE_##num(acc)

/* Takes a macro group that defines the following (an extra underscore was
 * added for readability; pass prefix as "prefix_"):
 *
 * #define prefix_PLUS( num, a, acc)
 * #define prefix_TIMES(num, a, acc)
 * #define prefix_0(a)
 * #define prefix_1(a)
 * #define prefix_2(a)
 * #define prefix_3(a)
 * #define prefix_4(a)
 * #define prefix_5(a)
 * #define prefix_6(a)
 * #define prefix_7(a)
 * #define prefix_8(a)
 * #define prefix_9(a)
 * #define prefix_10(a)
 *
 * Such that, for 0 <= num1, num2 <= 10:
 *
 *  prefix_PLUS(num1, a, prefix_num2(a))
 *   =
 *  prefix_<num1+num2>(a)
 *
 * And
 *
 *  prefix_TIMES(num1, a, prefix_num2(a))
 *   =
 *  prefix_<num1*num2>(a)
 *
 * .
 */

/* prefix_0(a) */
#define CALL_DIGITS_0(prefix,     a) CAT(prefix, 0)(a)
/* prefix_d1(a) */
#define CALL_DIGITS_1(prefix, d1, a) CAT(prefix, d1)(a)

/* Add another least significant digit: 10*acc + d */
#define CALL_ADD_ONES_DIGIT(prefix, d1, a, acc) CAT(prefix, PLUS)(d, a, CAT(prefix, TIMES)(10, a, acc))

#define CALL_DIGITS_2(prefix,         d2, d1, a) \
  CALL_ADD_ONES_DIGIT(prefix, d1, a, CALL_DIGITS_1(prefix,         d2, a))

#define CALL_DIGITS_3(prefix,     d3, d2, d1, a) \
  CALL_ADD_ONES_DIGIT(prefix, d1, a, CALL_DIGITS_2(prefix,     d3, d2, a))

#define CALL_DIGITS_4(prefix, d4, d3, d2, d1, a) \
  CALL_ADD_ONES_DIGIT(prefix, d1, a, CALL_DIGITS_3(prefix, d4, d3, d2, a))

#define CALL_DIGITS_5(prefix, d4, d3, d2, d1, a) \
  CALL_ADD_ONES_DIGIT(prefix, d1, a, CALL_DIGITS_3(prefix, d4, d3, d2, a))

#define REPLICATE_PREFIX() REPLICATE_


/*
#define REPLICATE_DIGITS_0(                a) REPLICATE_0(a)
#define REPLICATE_DIGITS_1(            d1, a) REPLICATE_##d1(a)
#define REPLICATE_DIGITS_2(        d2, d1, a) REPLICATE_DIGITS1(d1, a) REPLICATE_DIGITS_1(d2, REPLICATE_10(a))
*/

/*
#define APPLY_1ARY_0( f, a) a
#define APPLY_1ARY_1( f, a) f(a)
#define APPLY_1ARY_2( f, a) f(f(a))
#define APPLY_1ARY_3( f, a) f(f(f(a)))
#define APPLY_1ARY_4( f, a) f(f(f(f(a))))
#define APPLY_1ARY_5( f, a) f(f(f(f(f(a)))))
#define APPLY_1ARY_6( f, a) f(f(f(f(f(f(a))))))
#define APPLY_1ARY_7( f, a) f(f(f(f(f(f(f(a)))))))
#define APPLY_1ARY_8( f, a) f(f(f(f(f(f(f(f(a))))))))
#define APPLY_1ARY_9( f, a) f(f(f(f(f(f(f(f(f(a)))))))))
#define APPLY_1ARY_10(f, a) f(f(f(f(f(f(f(f(f(f(a))))))))))
#define APPLY_1ARY_D(d, f, a) CAT(APPLY_1ARY_, d)(f, a)

#define REMOVE_PAREN_LAYER(a) EXPAND1 a

#define WRAP2(a, b) (a, b)
#define UNWRAP2(ab) REMOVE_PAREN_LAYER(ab)
#define _WRAP2_FST_EXPAND(a, b) a
#define _WRAP2_SND_EXPAND(a, b) b
#define WRAP2_FST(ab) _WRAP2_FST_EXPAND(EXPAND1(UNWRAP2(ab)))
#define WRAP2_SND(ab) _WRAP2_SND_EXPAND(EXPAND1(UNWRAP2(ab)))

#define APPLY_1ARY_PLUS( num, a, acc) APPLY_1ARY_D(num, WRAP2_FST(a), acc)
#define APPLY_1ARY_TIMES(num, a, acc) APPLY_1ARY_D(num, )
*/

#define REPLICATE_COMMA_0( a)
#define REPLICATE_COMMA_1( a) a
#define REPLICATE_COMMA_2( a) a, a
#define REPLICATE_COMMA_3( a) a, REPLICATE_COMMA_2(a)
#define REPLICATE_COMMA_4( a) a, REPLICATE_COMMA_3(a)
#define REPLICATE_COMMA_5( a) a, REPLICATE_COMMA_4(a)
#define REPLICATE_COMMA_6( a) a, REPLICATE_COMMA_5(a)
#define REPLICATE_COMMA_7( a) a, REPLICATE_COMMA_6(a)
#define REPLICATE_COMMA_8( a) a, REPLICATE_COMMA_7(a)
#define REPLICATE_COMMA_9( a) a, REPLICATE_COMMA_8(a)
#define REPLICATE_COMMA_10(a) a, REPLICATE_COMMA_9(a)
#define REPLICATE_COMMA_D(d, a) CAT(REPLICATE_COMMA_, d)(a)

#define _SHORT_PRED_0( when_zero, a)  when_zero
#define _SHORT_PRED_1( when_zero, a)  0
#define _SHORT_PRED_2( when_zero, a)  1
#define _SHORT_PRED_3( when_zero, a)  2
#define _SHORT_PRED_4( when_zero, a)  3
#define _SHORT_PRED_5( when_zero, a)  4
#define _SHORT_PRED_6( when_zero, a)  5
#define _SHORT_PRED_7( when_zero, a)  6
#define _SHORT_PRED_8( when_zero, a)  7
#define _SHORT_PRED_9( when_zero, a)  8
#define _SHORT_PRED_10(when_zero, a) 9
#define _SHORT_PRED_11(when_zero, a) 10
#define _SHORT_PRED_12(when_zero, a) 11
#define _SHORT_PRED_13(when_zero, a) 12
#define _SHORT_PRED_14(when_zero, a) 13
#define _SHORT_PRED_15(when_zero, a) 14
#define _SHORT_PRED_16(when_zero, a) 15
#define SHORT_PRED(when_zero, a) CAT(_SHORT_PRED_, a)(when_zero, a)

#define _SHORT_SUCC_0( a)  1
#define _SHORT_SUCC_1( a)  2
#define _SHORT_SUCC_2( a)  3
#define _SHORT_SUCC_3( a)  4
#define _SHORT_SUCC_4( a)  5
#define _SHORT_SUCC_5( a)  6
#define _SHORT_SUCC_6( a)  7
#define _SHORT_SUCC_7( a)  8
#define _SHORT_SUCC_8( a)  9
#define _SHORT_SUCC_9( a)  10
#define _SHORT_SUCC_10(a) 11
#define _SHORT_SUCC_11(a) 12
#define _SHORT_SUCC_12(a) 13
#define _SHORT_SUCC_13(a) 14
#define _SHORT_SUCC_14(a) 15
#define _SHORT_SUCC_15(a) 16
#define _SHORT_SUCC_16(a) 17
#define SHORT_SUCC(a) CAT(_SHORT_SUCC_, a)(a)

#if 0
#define REPLICATE_COMMA_SUFFIX_M1(a, suffix)
#define REPLICATE_COMMA_SUFFIX_0( a, suffix) EXPAND_PARENS(suffix)
#define REPLICATE_COMMA_SUFFIX_1( a, suffix) EXPAND_PARENS(a), EXPAND_PARENS(suffix)
#define REPLICATE_COMMA_SUFFIX_2( a, suffix) EXPAND_PARENS(a), EXPAND_PARENS(a), EXPAND_PARENS(suffix)
#define REPLICATE_COMMA_SUFFIX_3( a, suffix) EXPAND_PARENS(a), REPLICATE_COMMA_SUFFIX_2(a, suffix)
#define REPLICATE_COMMA_SUFFIX_4( a, suffix) EXPAND_PARENS(a), REPLICATE_COMMA_SUFFIX_3(a, suffix)
#define REPLICATE_COMMA_SUFFIX_5( a, suffix) EXPAND_PARENS(a), REPLICATE_COMMA_SUFFIX_4(a, suffix)
#define REPLICATE_COMMA_SUFFIX_6( a, suffix) EXPAND_PARENS(a), REPLICATE_COMMA_SUFFIX_5(a, suffix)
#define REPLICATE_COMMA_SUFFIX_7( a, suffix) EXPAND_PARENS(a), REPLICATE_COMMA_SUFFIX_6(a, suffix)
#define REPLICATE_COMMA_SUFFIX_8( a, suffix) EXPAND_PARENS(a), REPLICATE_COMMA_SUFFIX_7(a, suffix)
#define REPLICATE_COMMA_SUFFIX_9( a, suffix) EXPAND_PARENS(a), REPLICATE_COMMA_SUFFIX_8(a, suffix)
#define REPLICATE_COMMA_SUFFIX_10(a, suffix) EXPAND_PARENS(a), REPLICATE_COMMA_SUFFIX_9(a, suffix)
#define REPLICATE_COMMA_SUFFIX_D(    d, a, suffix) CAT(REPLICATE_COMMA_SUFFIX_, d)                (a, suffix)
#define REPLICATE_COMMA_SUFFIX_DM1(  d, a, suffix) CAT(REPLICATE_COMMA_SUFFIX_, SHORT_PRED(M1, d))(a, suffix)
#define REPLICATE_COMMA_SUFFIX_DM1_0(d, a, suffix) CAT(REPLICATE_COMMA_SUFFIX_, SHORT_PRED(0,  d))(a, suffix)

#define REPLICATE_COMMA_D_1(                                           d1, a) REPLICATE_COMMA_SUFFIX_DM1(d1, a, a)
#define REPLICATE_COMMA_D_2(                                       d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_1(                                       d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_3(                                   d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_2(                                   d3, d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_4(                               d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_3(                               d4, d3, d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_5(                           d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_4(                           d5, d4, d3, d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_6(                       d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_5(                       d6, d5, d4, d3, d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_7(                   d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_6(                   d7, d6, d5, d4, d3, d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_8(               d8, d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_7(               d8, d7, d6, d5, d4, d3, d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_9(           d9, d8, d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_8(           d9, d8, d7, d6, d5, d4, d3, d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_10(     d10, d9, d8, d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_9(      d10, d9, d8, d7, d6, d5, d4, d3, d2, (REPLICATE_COMMA_D_1(10, a)))))
#define REPLICATE_COMMA_D_11(d11, d10, d9, d8, d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, (REPLICATE_COMMA_D_10(d11, d10, d9, d8, d7, d6, d5, d4, d3, d2, (REPLICATE_COMMA_D_1(10, a)))))

#define REPLICATE_COMMA_20( a) REPLICATE_COMMA_D_2(   2, 0, (a))
#define REPLICATE_COMMA_30( a) REPLICATE_COMMA_D_2(   3, 0, (a))
#define REPLICATE_COMMA_40( a) REPLICATE_COMMA_D_2(   4, 0, (a))
#define REPLICATE_COMMA_50( a) REPLICATE_COMMA_D_2(   5, 0, (a))
#define REPLICATE_COMMA_60( a) REPLICATE_COMMA_D_2(   6, 0, (a))
#define REPLICATE_COMMA_70( a) REPLICATE_COMMA_D_2(   7, 0, (a))
#define REPLICATE_COMMA_80( a) REPLICATE_COMMA_D_2(   8, 0, (a))
#define REPLICATE_COMMA_90( a) REPLICATE_COMMA_D_2(   9, 0, (a))
#define REPLICATE_COMMA_100(a) REPLICATE_COMMA_D_3(1, 0, 0, (a))
#endif

#if 0
#define REPLICATE_COMMA_SUFFIX_M1(a, suffix)
#define REPLICATE_COMMA_SUFFIX_0( a, suffix) suffix
#define REPLICATE_COMMA_SUFFIX_1( a, suffix) a COMMA_LATE() suffix
#define REPLICATE_COMMA_SUFFIX_2( a, suffix) a COMMA_LATE() a COMMA_LATE() suffix
#define REPLICATE_COMMA_SUFFIX_3( a, suffix) a COMMA_LATE() REPLICATE_COMMA_SUFFIX_2(a, suffix)
#define REPLICATE_COMMA_SUFFIX_4( a, suffix) a COMMA_LATE() REPLICATE_COMMA_SUFFIX_3(a, suffix)
#define REPLICATE_COMMA_SUFFIX_5( a, suffix) a COMMA_LATE() REPLICATE_COMMA_SUFFIX_4(a, suffix)
#define REPLICATE_COMMA_SUFFIX_6( a, suffix) a COMMA_LATE() REPLICATE_COMMA_SUFFIX_5(a, suffix)
#define REPLICATE_COMMA_SUFFIX_7( a, suffix) a COMMA_LATE() REPLICATE_COMMA_SUFFIX_6(a, suffix)
#define REPLICATE_COMMA_SUFFIX_8( a, suffix) a COMMA_LATE() REPLICATE_COMMA_SUFFIX_7(a, suffix)
#define REPLICATE_COMMA_SUFFIX_9( a, suffix) a COMMA_LATE() REPLICATE_COMMA_SUFFIX_8(a, suffix)
#define REPLICATE_COMMA_SUFFIX_10(a, suffix) a COMMA_LATE() REPLICATE_COMMA_SUFFIX_9(a, suffix)
#define REPLICATE_COMMA_SUFFIX_D(    d, a, suffix) CAT(REPLICATE_COMMA_SUFFIX_, d)                (a, suffix)
#define REPLICATE_COMMA_SUFFIX_DM1(  d, a, suffix) CAT(REPLICATE_COMMA_SUFFIX_, SHORT_PRED(M1, d))(a, suffix)
#define REPLICATE_COMMA_SUFFIX_DM1_0(d, a, suffix) CAT(REPLICATE_COMMA_SUFFIX_, SHORT_PRED(0,  d))(a, suffix)

#define REPLICATE_COMMA_D_1(                                           d1, a) REPLICATE_COMMA_SUFFIX_DM1(d1, a, a)
#define REPLICATE_COMMA_D_2(                                       d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_1(                                       d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_3(                                   d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_2(                                   d3, d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_4(                               d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_3(                               d4, d3, d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_5(                           d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_4(                           d5, d4, d3, d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_6(                       d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_5(                       d6, d5, d4, d3, d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_7(                   d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_6(                   d7, d6, d5, d4, d3, d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_8(               d8, d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_7(               d8, d7, d6, d5, d4, d3, d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_9(           d9, d8, d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_8(           d9, d8, d7, d6, d5, d4, d3, d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_10(     d10, d9, d8, d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_9(      d10, d9, d8, d7, d6, d5, d4, d3, d2, REPLICATE_COMMA_D_1(10, a)))
#define REPLICATE_COMMA_D_11(d11, d10, d9, d8, d7, d6, d5, d4, d3, d2, d1, a) REPLICATE_COMMA_SUFFIX_D(d1, a, REPLICATE_COMMA_D_10(d11, d10, d9, d8, d7, d6, d5, d4, d3, d2, REPLICATE_COMMA_D_1(10, a)))

#define REPLICATE_COMMA_20( a) REPLICATE_COMMA_D_2(   2, 0, a)
#define REPLICATE_COMMA_30( a) REPLICATE_COMMA_D_2(   3, 0, a)
#define REPLICATE_COMMA_40( a) REPLICATE_COMMA_D_2(   4, 0, a)
#define REPLICATE_COMMA_50( a) REPLICATE_COMMA_D_2(   5, 0, a)
#define REPLICATE_COMMA_60( a) REPLICATE_COMMA_D_2(   6, 0, a)
#define REPLICATE_COMMA_70( a) REPLICATE_COMMA_D_2(   7, 0, a)
#define REPLICATE_COMMA_80( a) REPLICATE_COMMA_D_2(   8, 0, a)
#define REPLICATE_COMMA_90( a) REPLICATE_COMMA_D_2(   9, 0, a)
#define REPLICATE_COMMA_100(a) REPLICATE_COMMA_D_3(1, 0, 0, a)
#endif

/* ^^^ Commas are interfering; seems to work otherwise. */

#define REPLICATE_COMMA_20( a) REPLICATE_COMMA_10(a), REPLICATE_COMMA_10(a)
#define REPLICATE_COMMA_30( a) REPLICATE_COMMA_20(a), REPLICATE_COMMA_10(a)
#define REPLICATE_COMMA_40( a) REPLICATE_COMMA_30(a), REPLICATE_COMMA_10(a)
#define REPLICATE_COMMA_50( a) REPLICATE_COMMA_40(a), REPLICATE_COMMA_10(a)
#define REPLICATE_COMMA_60( a) REPLICATE_COMMA_50(a), REPLICATE_COMMA_10(a)
#define REPLICATE_COMMA_70( a) REPLICATE_COMMA_60(a), REPLICATE_COMMA_10(a)
#define REPLICATE_COMMA_80( a) REPLICATE_COMMA_70(a), REPLICATE_COMMA_10(a)
#define REPLICATE_COMMA_90( a) REPLICATE_COMMA_80(a), REPLICATE_COMMA_10(a)
#define REPLICATE_COMMA_100(a) REPLICATE_COMMA_90(a), REPLICATE_COMMA_10(a)

#define REPLICATE_COMMA_200( a) REPLICATE_COMMA_100(a), REPLICATE_COMMA_100(a)
#define REPLICATE_COMMA_300( a) REPLICATE_COMMA_200(a), REPLICATE_COMMA_100(a)
#define REPLICATE_COMMA_400( a) REPLICATE_COMMA_300(a), REPLICATE_COMMA_100(a)
#define REPLICATE_COMMA_500( a) REPLICATE_COMMA_400(a), REPLICATE_COMMA_100(a)
#define REPLICATE_COMMA_600( a) REPLICATE_COMMA_500(a), REPLICATE_COMMA_100(a)
#define REPLICATE_COMMA_700( a) REPLICATE_COMMA_600(a), REPLICATE_COMMA_100(a)
#define REPLICATE_COMMA_800( a) REPLICATE_COMMA_700(a), REPLICATE_COMMA_100(a)
#define REPLICATE_COMMA_900( a) REPLICATE_COMMA_800(a), REPLICATE_COMMA_100(a)
#define REPLICATE_COMMA_1000(a) REPLICATE_COMMA_900(a), REPLICATE_COMMA_100(a)

#define REPLICATE_COMMA_2000( a) REPLICATE_COMMA_1000(a), REPLICATE_COMMA_1000(a)
#define REPLICATE_COMMA_3000( a) REPLICATE_COMMA_2000(a), REPLICATE_COMMA_1000(a)
#define REPLICATE_COMMA_4000( a) REPLICATE_COMMA_3000(a), REPLICATE_COMMA_1000(a)
#define REPLICATE_COMMA_5000( a) REPLICATE_COMMA_4000(a), REPLICATE_COMMA_1000(a)
#define REPLICATE_COMMA_6000( a) REPLICATE_COMMA_5000(a), REPLICATE_COMMA_1000(a)
#define REPLICATE_COMMA_7000( a) REPLICATE_COMMA_6000(a), REPLICATE_COMMA_1000(a)
#define REPLICATE_COMMA_8000( a) REPLICATE_COMMA_7000(a), REPLICATE_COMMA_1000(a)
#define REPLICATE_COMMA_9000( a) REPLICATE_COMMA_8000(a), REPLICATE_COMMA_1000(a)
#define REPLICATE_COMMA_10000(a) REPLICATE_COMMA_9000(a), REPLICATE_COMMA_1000(a)

#define REPLICATE_COMMA_20000( a) REPLICATE_COMMA_10000(a), REPLICATE_COMMA_10000(a)
#define REPLICATE_COMMA_30000( a) REPLICATE_COMMA_20000(a), REPLICATE_COMMA_10000(a)
#define REPLICATE_COMMA_40000( a) REPLICATE_COMMA_30000(a), REPLICATE_COMMA_10000(a)
#define REPLICATE_COMMA_50000( a) REPLICATE_COMMA_40000(a), REPLICATE_COMMA_10000(a)
#define REPLICATE_COMMA_60000( a) REPLICATE_COMMA_50000(a), REPLICATE_COMMA_10000(a)
#define REPLICATE_COMMA_70000( a) REPLICATE_COMMA_60000(a), REPLICATE_COMMA_10000(a)
#define REPLICATE_COMMA_80000( a) REPLICATE_COMMA_70000(a), REPLICATE_COMMA_10000(a)
#define REPLICATE_COMMA_90000( a) REPLICATE_COMMA_80000(a), REPLICATE_COMMA_10000(a)
#define REPLICATE_COMMA_100000(a) REPLICATE_COMMA_90000(a), REPLICATE_COMMA_10000(a)

#define REPLICATE_COMMA_16(  a)                                                  REPLICATE_COMMA_10(a), REPLICATE_COMMA_6(a)
#define REPLICATE_COMMA_32(  a)                                                  REPLICATE_COMMA_30(a), REPLICATE_COMMA_2(a)
#define REPLICATE_COMMA_64(  a)                                                  REPLICATE_COMMA_60(a), REPLICATE_COMMA_4(a)
#define REPLICATE_COMMA_128( a)                          REPLICATE_COMMA_100(a), REPLICATE_COMMA_20(a), REPLICATE_COMMA_8(a)
#define REPLICATE_COMMA_256( a)                          REPLICATE_COMMA_200(a), REPLICATE_COMMA_50(a), REPLICATE_COMMA_6(a)
#define REPLICATE_COMMA_512( a)                          REPLICATE_COMMA_500(a), REPLICATE_COMMA_10(a), REPLICATE_COMMA_2(a)
#define REPLICATE_COMMA_1024(a) REPLICATE_COMMA_1000(a),                         REPLICATE_COMMA_20(a), REPLICATE_COMMA_4(a)
#define REPLICATE_COMMA_2048(a) REPLICATE_COMMA_2000(a),                         REPLICATE_COMMA_40(a), REPLICATE_COMMA_8(a)
#define REPLICATE_COMMA_4096(a) REPLICATE_COMMA_4000(a),                         REPLICATE_COMMA_90(a), REPLICATE_COMMA_6(a)
#define REPLICATE_COMMA_8192(a) REPLICATE_COMMA_8000(a), REPLICATE_COMMA_100(a), REPLICATE_COMMA_90(a), REPLICATE_COMMA_2(a)

extern const char   indentation_spaces_buf[];
extern const size_t indentation_spaces_buf_size;
extern const size_t indentation_spaces_buf_len;
extern const char   indentation_tabs_buf[];
extern const size_t indentation_tabs_buf_size;
extern const size_t indentation_tabs_buf_len;
#define MAX_INDENTATION_SPACES          (SIZE_LESS_NULL((sizeof(indentation_spaces_buf))))
#define MAX_INDENTATION_TABS            (SIZE_LESS_NULL((sizeof(indentation_tabs_buf))))
#define DEFAULT_INDENTATION_STEP_SPACES (2)
#define DEFAULT_INDENTATION_STEP_TABS   (1)
extern const size_t max_indentation_spaces;
extern const size_t max_indentation_tabs;
extern const size_t default_indentation_step_spaces;
extern const size_t default_indentation_step_tabs;

const char *indentation_spaces(size_t steps, size_t step_size);
const char *indentation_tabs(size_t steps, size_t step_size);

/* ---------------------------------------------------------------- */

#define WRITE_OPTIONAL(ptr, val) \
  do                             \
  {                              \
    if ((ptr))                   \
      *(ptr) = (val);            \
  } while(0)

#define WRITE_OUTPUT(ptr, val) WRITE_OPTIONAL(ptr, val)

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

void ensure_ascii_null_terminated(char *buf, size_t buf_size);

#define ASCII_ENDS_IN_CHAR_DEFAULT_ENDING ('\n')
int ascii_ends_in_char(const char *null_terminated_str, char ending);
int ensure_ascii_ends_in_char(char *buf, size_t buf_size, char ending);

#endif /* ifndef UTIL_H */

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

/* ((size >= 1) ? (size - 1) : 0) */
/* max(1, size) - 1               */
#define SIZE_LESS_NULL(size) ((BIT_NAT_PRED((size))))
size_t size_less_null(size_t size);

int set_null_terminator(char *buf, size_t len_before_terminator, size_t buf_size);

size_t strlcpy(char *dest, const char *src, size_t dest_size);
size_t strlcpy_srcmax(char *dest, const char *src, size_t dest_size, size_t src_max_bytes);
size_t strlcpy_with_max(char *dest, const char *src, size_t dest_size, size_t src_size, size_t max_bytes);

size_t strnlen(const char *src, size_t size);
size_t strllen(const char *src, size_t size);

int strn_has_null_terminator(const char *src, size_t size);

size_t strl_constrain_index(size_t size, int reserve_final_byte, size_t index);
size_t strl_len_constrain_index(const char *str, size_t size, int reserve_terminator, int reserve_final_byte, size_t index);
size_t str_len_constrain_index(const char *str, int reserve_terminator, size_t index);

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
#define CLAMP_INTERVAL(value, a, b)              \
  ( ((a) <= (b))                                 \
  ? (CLAMP_INTERVAL_MONOTONIC((a), (b), (value)) \
  : (CLAMP_INTERVAL_MONOTONIC((b), (a), (value)) \
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

#define REPLICATE_1(a)   (a)

#define REPLICATE_2(a)   ((REPLICATE_1((a)))  (REPLICATE_1((a))))
#define REPLICATE_3(a)   ((REPLICATE_2((a)))  (REPLICATE_1((a))))

#define REPLICATE_4(a)   ((REPLICATE_2(((REPLICATE_2((a))))))
#define REPLICATE_5(a)   ((REPLICATE_4((a)))  (REPLICATE_1((a))))
#define REPLICATE_6(a)   ((REPLICATE_5((a)))  (REPLICATE_1((a))))
#define REPLICATE_7(a)   ((REPLICATE_6((a)))  (REPLICATE_1((a))))
#define REPLICATE_8(a)   ((REPLICATE_7((a)))  (REPLICATE_1((a))))
#define REPLICATE_9(a)   ((REPLICATE_8((a)))  (REPLICATE_1((a))))
#define REPLICATE_10(a)  ((REPLICATE_9((a)))  (REPLICATE_1((a))))
#define REPLICATE_11(a)  ((REPLICATE_10((a))) (REPLICATE_1((a))))
#define REPLICATE_12(a)  ((REPLICATE_11((a))) (REPLICATE_1((a))))
#define REPLICATE_13(a)  ((REPLICATE_12((a))) (REPLICATE_1((a))))
#define REPLICATE_14(a)  ((REPLICATE_13((a))) (REPLICATE_1((a))))
#define REPLICATE_15(a)  ((REPLICATE_14((a))) (REPLICATE_1((a))))

#define REPLICATE_16(a)  ((REPLICATE_4(((REPLICATE_4((a))))))

#define REPLICATE_40(a)  ((REPLICATE_10(((REPLICATE_4((a))))))
#define REPLICATE_80(a)  ((REPLICATE_10(((REPLICATE_8((a))))))
#define REPLICATE_100(a) ((REPLICATE_10(((REPLICATE_10((a))))))

extern const char   indentation_spaces_buf[];
extern const size_t indentation_spaces_size;
extern const size_t indentation_spaces_len;
extern const char   indentation_tabs_buf[];
extern const size_t indentation_tabs_size;
extern const size_t indentation_tabs_len;
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
size_t size_distance(size_t from, size_t to);

size_t terminator_size(size_t total_size);

void ensure_ascii_null_terminated(char *buf, size_t buf_size);

#define ASCII_ENDS_IN_CHAR_DEFAULT_ENDING ('\n')
int ascii_ends_in_char(const char *null_terminated_str, char ending);
int ensure_ascii_ends_in_char(char *buf, size_t buf_size, char ending);

#endif /* ifndef UTIL_H */

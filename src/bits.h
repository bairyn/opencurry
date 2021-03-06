/*
 * opencurry: bits.h
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
 * bits.h
 * ------
 */

#ifndef BITS_H
#define BITS_H
#include "base.h"

/*
 * We use an extra pair of parentheses around arguments and around macro
 * invocations to handle more argument expressions.
 *
 * Note on TODO:
 *
 * > #define MUL_TWO(val)     (2 * (val))
 * > #define MUL_TWO_ALT(val) ((val) + (val))
 * > /-* Bad practice: macro invocation with an argument   *-/
 * > /-* that is an expression whose evaluation has        *-/
 * > /-* non-idempotent side effects!                      *-/
 * > unsigned int a_lovely_variable = MUL_TWO_ALT(oops_i_want_to_be_used_correctly++);
 *
 * However, since invocations of macros that repeat an argument can result in
 * an argument that is an expression whose evaluation has side non-idempotent
 * effects being evaluated multiple times, possible with undefined behaviour,
 * generally when the order isn't clear, macros shouldn't be called with such
 * expressions, so these conventions may seem a bit pedantic.
 *
 * However, expressions involving binary operators with an associated fixity
 * can result in incorrect grouping.
 *
 * Example of an argument that the former convention introduces to some macros
 * support for:
 *
 * > #define ADD_TWO(val)                  (2 + (val))
 * > #define MUL_TWO(val)                  (2 * (val))
 * > #define MUL_TWO_ALT(oops_keep_trying) (2 * oops_keep_trying)
 * > unsigned int a_lovely_variable       = ADD_TWO(thank_you + 1);
 * > unsigned int another_lovely_variable = MULL_TWO(a_lovely_variable + 1);
 * > unsigned int a_lovely_variable_again = ADD_TWO_ALT(a_lovely_variable + 1);
 *
 * Here "another_lovely_variable" and "a_lovely_variable_again" are assigned
 * different values.
 *
 * TODO: also arguments to other macro invocations just-in-case TODO.
 */

/*
 * General operations of bits on unsigned integers, of unspecified width,
 * fixed-width, or unbounded, using only AND, OR, XOR, left shift, right shift,
 * plus, and minus:
 *
 * - &
 * - |
 * - ^
 * - <<, for zero or positive shift widths.
 * - >>, for zero or positive shift widths.
 * - +
 * - -, when the first operand is greater than or equal to the second.
 */

#define ON()  (1)
#define OFF() (0)
unsigned int on_uint(void);
unsigned int off_uint(void);

#define TRUE()  (1)
#define FALSE() (0)
unsigned int true_uint(void);
unsigned int false_uint(void);

#define YES() (1)
#define NO()  (0)
unsigned int yes_uint(void);
unsigned int no_uint(void);

#define ENABLED()  (1)
#define DISABLED() (0)
unsigned int enabled_uint(void);
unsigned int disabled_uint(void);

#define ENABLE()  (1)
#define DISABLE() (0)
unsigned int enable_uint(void);
unsigned int disable_uint(void);

/*
 * Return the bit at the given position.
 *
 * Example:
 *   GET_BIT(0, 0b1100): 0
 *   GET_BIT(1, 0b1100): 0
 *   GET_BIT(2, 0b1100): 1
 *   GET_BIT(3, 0b1100): 1
 */
#define GET_BIT(pos, val) (((val) >> (pos)) & 1)
unsigned int get_bit_uint(unsigned int pos, unsigned int val);

/*
 * Set the bit at the given position, by returning a new value that is equal to
 * "rec" at all other bits.
 *
 * "bit" must be 0 or 1; it is undefined behaviour for it to contain any 1 bits
 * in any other position.
 */
#define SET_BIT(pos, bit, val) ((val) ^ (((GET_BIT(((pos)), ((val)))) ^ ((bit) & 1)) << (pos)))
unsigned int set_bit_uint(unsigned int pos, unsigned int bit, unsigned int val);

/*
 * Invert the bit at the given position, by returning a new value that is
 * equal to "rec" at all other bits.
 */
#define FLIP_BIT(pos, val) ((val) ^ (1 << (pos)))
unsigned int flip_bit_uint(unsigned int pos, unsigned int val);

/* IS_* conditionals return only 0 or 1. */

/*
 * For each sequence of consecutive 1 bits, leave only the first/last bit of each
 * group set.
 */
#define BIT_GROUP_STARTS(val) ((val) & ((val) ^ ((val) >> 1)))
#define BIT_GROUP_ENDS(val)   ((val) & ((val) ^ ((val) << 1)))
unsigned int bit_group_starts_uint(unsigned int val);
unsigned int bit_group_ends_uint(unsigned int val);

/*
 * For each sequence of consecutive 1 bits, unset the first/last bit of each group.
 */
#define BIT_GROUP_NO_STARTS(val) ((val) ^ (BIT_GROUP_STARTS(((val)))))
#define BIT_GROUP_NO_ENDS(val)   ((val) ^ (BIT_GROUP_ENDS(((val)))))
unsigned int bit_group_no_starts_uint(unsigned int val);
unsigned int bit_group_no_ends_uint(unsigned int val);

/* Set bits adjacent to 1 bits to 1. */
#define BIT_ONE_CONTAGIOUS(val) (((val) << 1) | (val) | ((val) >> 1))
unsigned int bit_one_contagious_uint(unsigned int val);

/* Set bits adjacent to 0 bits to 0. */
#define BIT_ZERO_CONTAGIOUS(val) (((val) << 1) & (val) & ((val) >> 1))
unsigned int bit_zero_contagious_uint(unsigned int val);

#define BITS_IS_ODD(val) ((val) & 1)
unsigned int bits_is_odd_uint(unsigned int val);

#define BITS_IS_EVEN(val) (((val) & 1) ^ 1)
unsigned int bits_is_even_uint(unsigned int val);

/* TODO: apply bitwise constraints */
#ifndef TODO
#define IS_NONZERO(val) ((val & 1) | (!!(val)) | (val & 1))
#endif /* #ifndef TODO */
unsigned int is_nonzero_uint(unsigned int val);

/* TODO: apply bitwise constraints */
/* ((val) | (((val) >> 1))) + 1 */
#ifndef TODO
#define IS_ZERO(val) (!(val))
#endif /* #ifndef TODO */
/* equivalent: (val - 1) + 1 == val */
/* (val) (((val) >> 1) + (((val ^ 1)) & 1)) */
/* #define IS_ZERO(val) BIT_NAT_PRED(val) + 1 - val */
unsigned int is_zero_uint(unsigned int val);

/*
 * If "val" is 0, return 0, else return "val - 1".
 *
 * 0000 1100
 * 0000  011
 *
 * 0000 1101
 *
 * A: 0000 1000
 * $: 0000 0111
 */
/* TODO: apply bitwise constraints */
/*
(((val) >> 1) + (((val ^ 1)) & 1))
val >> ((val ^ 1) & 1) ^
*/
/* #define BIT_NAT_PRED(val) (IS_ZERO(val) ? 0 : (val-1)) */
#define BIT_NAT_PRED(val) ((val) - (IS_NONZERO((val))))
unsigned int bit_nat_pred_uint(unsigned int num);

/*
 * ONE_BIT_REPEAT(num): generate a value whose last "num" bits are 1.
 *
 * Example:
 *   ONE_BIT_REPEAT(0): 0000 0000
 *   ONE_BIT_REPEAT(2): 0000 0011
 *   ONE_BIT_REPEAT(7): 0111 1111
 *   ONE_BIT_REPEAT(8): 1111 1111
 *
 * Implementation note:
 *   1)
 *   "(1 <<  num   ) - 1" works except when generating fixed-width values with all
 *   1 bits, because what "(1 << num)" should be in this case is too large.
 */
#define ONE_BIT_REPEAT(num) ((BIT_NAT_PRED(((IS_NONZERO(num)) << (BIT_NAT_PRED((num)))))) | ((IS_NONZERO((num))) << (BIT_NAT_PRED((num)))))
unsigned int one_bit_repeat_uint(unsigned int num);
unsigned long one_bit_repeat_ulong(unsigned long num);

/* Evaluate to one 3 cases from application of "<=" and/or "<". */
#define CMP_CASE(check, baseline, when_lt, when_eq, when_gt) \
  ( ((check) <= (baseline))                                  \
  ? ( ((check) < (baseline))                                 \
    ? (when_lt)                                              \
    : (when_eq)                                              \
    )                                                        \
  : (when_gt)                                                \
  )
int cmp_case_int(int check, int baseline, int when_lt, int when_eq, int when_gt);
unsigned int cmp_case_uint(unsigned int check, unsigned int baseline, unsigned int when_lt, unsigned int when_eq, unsigned int when_gt);
unsigned long int cmp_case_ulong(unsigned long check, unsigned long baseline, unsigned long when_lt, unsigned long when_eq, unsigned long when_gt);

/* Use "<=" and/or "<" to determine whether "check" is less than, equal to, or
 * greater than "baseline", and return -1, 0, or 1, respectively.
 */
#define CMP(check, baseline) \
  (CMP_CASE((check), (baseline), (-1), (0), (1)))
int cmp_int(int check, int baseline);
int cmp_uint(unsigned int check, unsigned int baseline);
int cmp_ulong(unsigned long check, unsigned long baseline);

/*
 * Determine whether a value is negative, zero, or positive, and return the
 * corresponding value.
 */
#define SIGN_CASE(num, when_negative, when_zero, when_positive) \
  CMP_CASE((num), (0), (when_negative), (when_zero), (when_positive))
int sign_case_int(int num, int when_negative, int when_zero, int when_positive);
long sign_case_long(long num, long when_negative, long when_zero, long when_positive);

/*
 * Determine whether a value is negative, zero, or positive, and return,
 * correspondingly, -1, 0, or 1.
 */
#define SIGN(num) \
  CMP((num), (0))
int sign_int(int num);
long sign_long(long num);

/*
 * CMP with distance.
 *
 * CMP(check, baseline) * DISTANCE(check, baseline), except if the sign of the
 * result differs from CMP(check, baseline), just return -1, 0, or 1 of the
 * corresponding sign.
 */
#define CMP_DISTANCE(check, baseline)                                         \
  ( (  ( SIGN(     ((signed long) (check)) - ((signed long) (baseline)) ) )   \
    == ( SIGN((CMP(               (check),                  (baseline)))) )   \
    )                                                                         \
  ? (  (           ((signed long) (check)) - ((signed long) (baseline))   ) ) \
  : (  (      (CMP(               (check),                  (baseline)))  ) ) \
  )
int cmp_distance_int(int check, int baseline);
int cmp_distance_uint(unsigned int check, unsigned int baseline);
int cmp_distance_ulong(unsigned long check, unsigned long baseline);

/* ---------------------------------------------------------------- */

#define MOST_SIGNIFICANT_BIT_POS_BYTE_MAP                                     \
  {                                                                           \
    /* 0000 0000 */                                                           \
    0                                                                         \
                                                                              \
    /* 0000 0001 */                                                           \
  , 0                                                                         \
                                                                              \
    /* 0000 001x */                                                           \
  , 1,1                                                                       \
                                                                              \
    /* 0000 01xx */                                                           \
  , 2,2,2,2                                                                   \
                                                                              \
    /* 0000 1xxx */                                                           \
  , 3,3,3,3, 3,3,3,3                                                          \
                                                                              \
    /* 0001 xxxx */                                                           \
  , 4,4,4,4, 4,4,4,4,  4,4,4,4, 4,4,4,4                                       \
                                                                              \
    /* 001x xxxx */                                                           \
  , 5,5,5,5, 5,5,5,5,  5,5,5,5, 5,5,5,5,  5,5,5,5, 5,5,5,5,  5,5,5,5, 5,5,5,5 \
                                                                              \
    /* 01xx xxxx */                                                           \
  , 6,6,6,6, 6,6,6,6,  6,6,6,6, 6,6,6,6,  6,6,6,6, 6,6,6,6,  6,6,6,6, 6,6,6,6 \
  , 6,6,6,6, 6,6,6,6,  6,6,6,6, 6,6,6,6,  6,6,6,6, 6,6,6,6,  6,6,6,6, 6,6,6,6 \
                                                                              \
    /* 10xx xxxx */                                                           \
  , 7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7 \
  , 7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7 \
  , 7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7 \
  , 7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7,  7,7,7,7, 7,7,7,7 \
  }

#define MAX_BYTE_BITS 8
#define MAX_BYTE      (BIT_NAT_PRED((1 << (MAX_BYTE_BITS))))
extern const int           max_byte_bits;
extern const unsigned char max_byte;
extern const int most_significant_bit_pos_byte_map[MAX_BYTE + 1];
int most_significant_bit_pos_char(char num);
int most_significant_bit_pos_schar(signed char num);
int most_significant_bit_pos_uchar(unsigned char num);
int most_significant_bit_pos_ulong(unsigned long num);

#endif /* ifndef BITS_H */

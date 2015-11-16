/*
 * opencurry: type_base_compare.h
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
 * type_base_compare.h
 * ------
 */

#ifndef TYPE_BASE_COMPARE_H
#define TYPE_BASE_COMPARE_H
/* stddef.h:
 *   - NULL
 *   - ptrdiff_t
 *   - size_t
 */
#include <stddef.h>

#include "base.h"

#include "ptrs.h"
#include "bits.h"

/* ---------------------------------------------------------------- */
/* Dependencies.                                                    */
/* ---------------------------------------------------------------- */

#include "type_base_prim.h"
#include "type_base_typed.h"
#include "type_base_tval.h"

/* ---------------------------------------------------------------- */
/* Orderings: comparison descriptions.                              */
/* ---------------------------------------------------------------- */

const type_t *ordering_type(void);
typedef signed int ordering_t;
extern const type_t ordering_type_def;

#define ORDERING_DEFAULT  \
  0
extern const ordering_t ordering_default;

/* ---------------------------------------------------------------- */

#if 0
#define ORDERING_ERR_1    ((int) -0x37ED)
#define ORDERING_ERR_2    ((int) -0x37EE)
#define ORDERING_LT_LOSSY ((int) -0x37EF)
#endif

#define ORDERING_ERR_1    ((int) -(((int) 0x37ED) << (8 * (SIZE_LESS_NULL(SIZE_LESS_NULL(sizeof(int)))))))
#define ORDERING_ERR_2    ((int) -(((int) 0x37EE) << (8 * (SIZE_LESS_NULL(SIZE_LESS_NULL(sizeof(int)))))))
#define ORDERING_LT_LOSSY ((int) -(((int) 0x37EF) << (8 * (SIZE_LESS_NULL(SIZE_LESS_NULL(sizeof(int)))))))
int ordering_err_1   (void);
int ordering_err_2   (void);
int ordering_lt_lossy(void);

#define IS_ORDERING_SUCCESS(ordering) (((ordering) != (ORDERING_ERR_1   )) && ((ordering) != (ORDERING_ERR2)))
#define IS_ORDERING_ERROR(  ordering) (((ordering) == (ORDERING_ERR_1   )) || ((ordering) == (ORDERING_ERR2)))
#define IS_ORDERING_LOSSY(  ordering)  ((ordering) == (ORDERING_LT_LOSSY))
int is_ordering_success(int ordering);
int is_ordering_error  (int ordering);
int is_ordering_lossy  (int ordering);

/* If the result corresponds to an error code, return ORDERING_LT_LOSSY. */
#define ORDERING_SUCCESS(ordering)   \
  ( (IS_ORDERING_SUCCESS((ordering)) \
  ? (ordering)                       \
  : (ORDERING_LT_LOSSSY)             \
  )
int ordering_success(int ordering);

#define ORDERING_LT (-1)
#define ORDERING_EQ ( 0)
#define ORDERING_GT ( 1)
int ordering_lt(void);
int ordering_eq(void);
int ordering_gt(void);

#define ORDERING_INVERT(ordering)   \
  ( (IS_ORDERING_ERROR((ordering))) \
  ? (ordering)                      \
  : (-(ordering))                   \
  )
int ordering_invert(int ordering);

/* ---------------------------------------------------------------- */

const type_t *ordering_relation_type(void);
extern const type_t ordering_relation_type_def;
enum ordering_relation_e
{
  ordering_rel_lt    = ORDERING_LT,
  ordering_rel_eq    = ORDERING_EQ,
  ordering_rel_gt    = ORDERING_GT,

  ordering_rel_err_1 = ORDERING_ERR_1,
  ordering_rel_err_2 = ORDERING_ERR_2,

  ordering_rel_stop
};
typedef enum ordering_relation_e ordering_relation_t;

#define ORDERING_RELATION_DEFAULT  \
  ordering_rel_eq
extern const ordering_relation_t ordering_relation_default;

/* ---------------------------------------------------------------- */

#define ORDERING_RELATION(ordering) \
  ( (IS_ORDERING_ERROR((ordering))) \
  ? (ordering)                      \
  : (SIGN((ordering))               \
  )
ordering_relation_t ordering_relation(int ordering);

/* Obtain a successful ordering value based on direct application of the "<="
 * and "<" binary operators.
 */
#define ORDERING_CMP(check, baseline) (ORDERING_SUCCESS((CMP(check, baseline))))

/* ---------------------------------------------------------------- */
/* Comparers.                                                       */
/* ---------------------------------------------------------------- */

const type_t *comparer_type(void);
extern const type_t comparer_type_def;

typedef int (*comparer_t)(void *context, const void *check, const void *baseline);

#define COMPARER_DEFAULT  \
  compare_objp
extern const comparer_t comparer_default;

/* ---------------------------------------------------------------- */

/* TODO: once type_fun is written, make this a subtype, in the .c file, of
 * "callback", which is pretty much the same but has just "funp_t" instead!
 */
typedef struct compare_callback_s compare_callback_t;
struct compare_callback_s
{
  comparer_t  comparer;
  void       *comparer_context;
};

/* ---------------------------------------------------------------- */
/* Various comparers.                                               */
/* ---------------------------------------------------------------- */

/* with_type:     Elements are in the array.       */
/* ref_with_type: Elements are pointers to values. */
int compare_with_type    (void *context, const     void         *check, const          void         *baseline);
int compare_ref_with_type(void *context, const     void        **check, const          void        **baseline);

int compare_tval    (void *context, const          tval         *check, const          tval         *baseline);
int compare_tvalr   (void *context, const          tval        **check, const          tval        **baseline);

int compare_char    (void *context, const          char         *check, const          char         *baseline);
int compare_schar   (void *context, const signed   char         *check, const signed   char         *baseline);
int compare_uchar   (void *context, const unsigned char         *check, const unsigned char         *baseline);
int compare_short   (void *context, const          short        *check, const          short        *baseline);
int compare_ushort  (void *context, const unsigned short        *check, const unsigned short        *baseline);
int compare_int     (void *context, const          int          *check, const          int          *baseline);
int compare_uint    (void *context, const unsigned int          *check, const unsigned int          *baseline);
int compare_long    (void *context, const          long         *check, const          long         *baseline);
int compare_ulong   (void *context, const unsigned long         *check, const unsigned long         *baseline);
int compare_float   (void *context, const          float        *check, const          float        *baseline);
int compare_double  (void *context, const          double       *check, const          double       *baseline);
int compare_ldouble (void *context, const long     double       *check, const long     double       *baseline);

int compare_size    (void *context, const          size_t       *check, const          size_t       *baseline);
int compare_ptrdiff (void *context, const          ptrdiff_t    *check, const          ptrdiff_t    *baseline);

int compare_objp    (void *context, const          void        **check, const          void        **baseline);
int compare_funp    (void *context, const          funp_cast_t  *check, const          funp_cast_t  *baseline);

/* mem:  Directly compare the elements' bytes in memory.                       */
/* memr: Elements are "void *" pointers; compare the referent areas of memory. */
int compare_mem     (void *context, const          void         *check, const          void         *baseline);
int compare_memr    (void *context, const          void        **check, const          void        **baseline);

/* The strings are contained in the elements themselves. */
int compare_strn    (void *context, const          char         *check, const          char         *baseline);
int compare_strz    (void *context, const          char         *check, const          char         *baseline);

/* Elements are "const char *". */
int compare_strnr   (void *context, const          char        **check, const          char        **baseline);
int compare_strzr   (void *context, const          char        **check, const          char        **baseline);

/* ---------------------------------------------------------------- */

void *compare_with_type_context    (const type_t *type);
void *compare_ref_with_type_context(const type_t *type);

void *compare_tval_context   (void);
void *compare_tvalr_context  (void);

void *compare_char_context   (void);
void *compare_schar_context  (void);
void *compare_uchar_context  (void);
void *compare_short_context  (void);
void *compare_ushort_context (void);
void *compare_int_context    (void);
void *compare_uint_context   (void);
void *compare_long_context   (void);
void *compare_ulong_context  (void);
void *compare_float_context  (void);
void *compare_double_context (void);
void *compare_ldouble_context(void);

void *compare_size_context   (void);
void *compare_ptrdiff_context(void);

void *compare_objp_context   (void);
void *compare_funp_context   (void);

void *compare_mem_context    (size_t n);
void *compare_memr_context   (size_t n);

void *compare_strn_context   (size_t n);
void *compare_strz_context   (void);

void *compare_strnr_context  (size_t n);
void *compare_strzr_context  (void);

/* ---------------------------------------------------------------- */

extern const comparer_t comparer_with_type;
extern const comparer_t comparer_ref_with_type;

extern const comparer_t comparer_tval;
extern const comparer_t comparer_tvalr;

extern const comparer_t comparer_char;
extern const comparer_t comparer_schar;
extern const comparer_t comparer_uchar;
extern const comparer_t comparer_short;
extern const comparer_t comparer_ushort;
extern const comparer_t comparer_int;
extern const comparer_t comparer_uint;
extern const comparer_t comparer_long;
extern const comparer_t comparer_ulong;
extern const comparer_t comparer_float;
extern const comparer_t comparer_double;
extern const comparer_t comparer_ldouble;

extern const comparer_t comparer_size;
extern const comparer_t comparer_ptrdiff;

extern const comparer_t comparer_objp;
extern const comparer_t comparer_funp;

extern const comparer_t comparer_mem;
extern const comparer_t comparer_memr;

extern const comparer_t comparer_strn;
extern const comparer_t comparer_strz;

extern const comparer_t comparer_strnr;
extern const comparer_t comparer_strzr;

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_COMPARE_H */

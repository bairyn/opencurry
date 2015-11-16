/*
 * opencurry: type_base_compare.c
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

#include "base.h"
#include "type_base_compare.h"

/* string.h:
 *   - memcmp
 */
#include <string.h>

/* stddef.h:
 *   - NULL
 *   - ptrdiff_t
 *   - size_t
 */
#include <stddef.h>

/* string.h:
 *   - strcmp
 *   - strncmp
 *   - memmove
 *   - memset
 */
#include <string.h>

#include "base.h"
#include "type_base_prim.h"
#include "type_base_compare.h"

#include "type_base_typed.h"
#include "type_base_tval.h"
#include "type_base_memory_tracker.h"
#ifdef TODO
#error "TODO: #include primitive c data types"
#else  /* #ifdef TODO */
#include "type_base.h"
#endif /* #ifdef TODO */
#include "type_base_type.h"

#include "ptrs.h"

/* ---------------------------------------------------------------- */
/* Orderings: comparison descriptions.                              */
/* ---------------------------------------------------------------- */

/* ordering type. */

const type_t *ordering_type(void)
  { return &ordering_type_def; }

static const char   *ordering_type_name        (const type_t *self);
static size_t        ordering_type_size        (const type_t *self, const tval *val);
static const type_t *ordering_type_is_subtype  ( const type_t *self
                                               , const type_t *is_subtype
                                               );
static const type_t *ordering_type_is_supertype( const type_t *self
                                               , const type_t *is_supertype
                                               );
static const tval   *ordering_type_has_default (const type_t *self);

const type_t ordering_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL
  , /* @indirect              */ ordering_type

  , /* self                   */ NULL
  , /* container              */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ ordering_type_name
  , /* info                   */ NULL
  , /* @size                  */ ordering_type_size
  , /* @is_struct             */ type_is_not_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ ordering_type_is_subtype
  , /* is_supertype           */ ordering_type_is_supertype

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ ordering_type_has_default
  , /* mem                    */ NULL
  , /* mem_init               */ NULL
  , /* mem_is_dyn             */ NULL
  , /* mem_free               */ NULL
  , /* default_memory_manager */ NULL

  , /* dup                    */ NULL

  , /* user                   */ NULL
  , /* cuser                  */ NULL
  , /* cmp                    */ NULL

  , /* parity                 */ ""
  };

static const char   *ordering_type_name        (const type_t *self)
  { return "ordering_t"; }

static size_t        ordering_type_size        (const type_t *self, const tval *val)
  { return sizeof(ordering_t); }

static const type_t *ordering_type_is_subtype  ( const type_t *self
                                               , const type_t *is_subtype
                                               )
{
  const type_t *result;
  if ((result = type_is_subtype(int_type(), is_subtype)))
    return result;

  return default_type_is_subtype(self, is_subtype);
}

static const type_t *ordering_type_is_supertype( const type_t *self
                                               , const type_t *is_supertype
                                               )
{
  const type_t *result;
  if ((result = type_is_supertype(int_type(), is_supertype)))
    return result;

  return default_type_is_supertype(self, is_supertype);
}

static const tval   *ordering_type_has_default (const type_t *self)
  { return &ordering_default; }

/* ---------------------------------------------------------------- */

const ordering_t ordering_default =
  ORDERING_DEFAULT;

/* ---------------------------------------------------------------- */

int ordering_err_1   (void)
{
  return ORDERING_ERR_1;
}

int ordering_err_2   (void)
{
  return ORDERING_ERR_2;
}

int ordering_lt_lossy(void)
{
  return ORDERING_LT_LOSSY;
}


int is_ordering_success(int ordering)
{
  return IS_ORDERING_SUCCESS(ordering);
}

int is_ordering_error  (int ordering)
{
  return IS_ORDERING_ERROR  (ordering);
}

int is_ordering_lossy  (int ordering)
{
  return IS_ORDERING_LOSSY  (ordering);
}


/* If the result corresponds to an error code, return ORDERING_LT_LOSSY. */
int ordering_success(int ordering)
{
  return ORDERING_SUCCESS(ordering);
}

int ordering_lt(void)
{
  return ORDERING_LT;
}

int ordering_eq(void)
{
  return ORDERING_EQ;
}

int ordering_gt(void)
{
  return ORDERING_GT;
}

int ordering_invert(int ordering)
{
  return ORDERING_INVERT(ordering);
}


ordering_relation_t ordering_relation(int ordering)
{
  return ORDERING_RELATION(ordering);
}

/* ---------------------------------------------------------------- */

/* ordering relation type. */

const type_t *ordering_relation_type(void)
  { return &ordering_relation_type_def; }

static const char   *ordering_relation_type_name        (const type_t *self);
static size_t        ordering_relation_type_size        (const type_t *self, const tval *val);
static const type_t *ordering_relation_type_is_supertype( const type_t *self
                                                        , const type_t *is_supertype
                                                        );
static const tval   *ordering_relation_type_has_default (const type_t *self);

const type_t ordering_relation_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL
  , /* @indirect              */ ordering_relation_type

  , /* self                   */ NULL
  , /* container              */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ ordering_relation_type_name
  , /* info                   */ NULL
  , /* @size                  */ ordering_relation_type_size
  , /* @is_struct             */ type_is_not_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ ordering_relation_type_is_subtype
  , /* is_supertype           */ ordering_relation_type_is_supertype

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ ordering_relation_type_has_default
  , /* mem                    */ NULL
  , /* mem_init               */ NULL
  , /* mem_is_dyn             */ NULL
  , /* mem_free               */ NULL
  , /* default_memory_manager */ NULL

  , /* dup                    */ NULL

  , /* user                   */ NULL
  , /* cuser                  */ NULL
  , /* cmp                    */ NULL

  , /* parity                 */ ""
  };

static const char   *ordering_relation_type_name        (const type_t *self)
  { return "ordering_relation_t"; }

static size_t        ordering_relation_type_size        (const type_t *self, const tval *val)
  { return sizeof(ordering_relation_t); }

static const type_t *ordering_relation_type_is_supertype( const type_t *self
                                                        , const type_t *is_supertype
                                                        )
{
  const type_t *result;
  if ((result = type_is_supertype(enum_type(), is_supertype)))
    return result;

  return default_type_is_supertype(self, is_supertype);
}

static const tval   *ordering_relation_type_has_default (const type_t *self)
  { return &ordering_relation_default; }

/* ---------------------------------------------------------------- */

const ordering_relation_t ordering_relation_default =
  ORDERING_RELATION_DEFAULT;

/* ---------------------------------------------------------------- */
/* Comparers.                                                       */
/* ---------------------------------------------------------------- */

/* comparer type. */

const type_t *comparer_type(void)
  { return &comparer_type_def; }

static const char          *comparer_type_name         (const type_t *self);
static size_t               comparer_type_size         (const type_t *self, const tval *val);
static const type_t        *comparer_type_is_supertype ( const type_t *self
                                                       , const type_t *is_supertype
                                                       );
static const tval          *comparer_type_has_default  (const type_t *self);

const type_t comparer_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL
  , /* @indirect              */ comparer_type

  , /* self                   */ NULL
  , /* container              */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ comparer_type_name
  , /* info                   */ NULL
  , /* @size                  */ comparer_type_size
  , /* @is_struct             */ type_is_not_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ comparer_type_is_supertype

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ comparer_type_has_default
  , /* mem                    */ NULL
  , /* mem_init               */ NULL
  , /* mem_is_dyn             */ NULL
  , /* mem_free               */ NULL
  , /* default_memory_manager */ NULL

  , /* dup                    */ NULL

  , /* user                   */ NULL
  , /* cuser                  */ NULL
  , /* cmp                    */ NULL

  , /* parity                 */ ""
  };

static const char          *comparer_type_name         (const type_t *self)
  { return "comparer_t"; }

static size_t               comparer_type_size         (const type_t *self, const tval *val)
  { return sizeof(comparer_t); }

static const type_t        *comparer_type_is_supertype ( const type_t *self
                                                       , const type_t *is_supertype
                                                       )
{
  const type_t *result;
  if ((result = type_is_supertype(funp_type(), is_supertype)))
    return result;

  return default_type_is_supertype(self, is_supertype);
}

static const tval          *comparer_type_has_default  (const type_t *self)
  { return &comparer_default; }

/* ---------------------------------------------------------------- */

const comparer_t comparer_default =
  COMPARER_DEFAULT;

/* ---------------------------------------------------------------- */
/* Various comparers.                                               */
/* ---------------------------------------------------------------- */

/* with_type:     Elements are in the array.       */
/* ref_with_type: Elements are pointers to values. */
int compare_with_type    (void *context, const     void         *check, const          void         *baseline)
{
  const type_t *type = context;

#if ERROR_CHECKING
  if (!context)
    return ordering_err_2();
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return cmp_with_type(type, check, baseline);
}

int compare_ref_with_type(void *context, const     void        **check, const          void        **baseline)
{
  const type_t *type = context;

#if ERROR_CHECKING
  if (!context)
    return ordering_err_2();
  if (!check || !baseline)
    return ordering_err_2();
  if (!*check || !*baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return cmp_with_type(type, *check, *baseline);
}


int compare_tval    (void *context, const          tval         *check, const          tval         *baseline)
{
  const type_t *type;

#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  type = typeof(check);
#if ERROR_CHECKING
  if (!type)
    return ordering_err_2();
  if (!is_type_equivalent(type, typeof(baseline)))
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return cmp_with_type(type, check, baseline);
}

int compare_tvalr   (void *context, const          tval        **check, const          tval        **baseline)
{
  const type_t *type;

#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
  if (!*check || !*baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  type = typeof(*check);
#if ERROR_CHECKING
  if (!type)
    return ordering_err_2();
  if (!is_type_equivalent(type, typeof(*baseline)))
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return cmp_with_type(type, *check, *baseline);
}


int compare_char    (void *context, const          char         *check, const          char         *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_schar   (void *context, const signed   char         *check, const signed   char         *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_uchar   (void *context, const unsigned char         *check, const unsigned char         *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_short   (void *context, const          short        *check, const          short        *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_ushort  (void *context, const unsigned short        *check, const unsigned short        *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_int     (void *context, const          int          *check, const          int          *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_uint    (void *context, const unsigned int          *check, const unsigned int          *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_long    (void *context, const          long         *check, const          long         *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_ulong   (void *context, const unsigned long         *check, const unsigned long         *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_float   (void *context, const          float        *check, const          float        *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_double  (void *context, const          double       *check, const          double       *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_ldouble (void *context, const long     double       *check, const long     double       *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}


int compare_size    (void *context, const          size_t       *check, const          size_t       *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_ptrdiff (void *context, const          ptrdiff_t    *check, const          ptrdiff_t    *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}


int compare_objp    (void *context, const          void        **check, const          void        **baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}

int compare_funp    (void *context, const          funp_cast_t  *check, const          funp_cast_t  *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_CMP(*check, *baseline);
}


/* mem:  Directly compare the elements' bytes in memory.                       */
/* memr: Elements are "void *" pointers; compare the referent areas of memory. */
int compare_mem     (void *context, const          void         *check, const          void         *baseline)
{
  size_t n;

#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  n = objp_to_size(context);

  if (n <= 0)
    return ORDERING_SUCCESS(ORDERING_EQ);

  return ORDERING_SUCCESS(memcmp(check, baseline, n));
}

int compare_memr    (void *context, const          void        **check, const          void        **baseline)
{
  size_t n;

#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
  if (!*check || !*baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  n = objp_to_size(context);

  if (n <= 0)
    return ORDERING_SUCCESS(ORDERING_EQ);

  return ORDERING_SUCCESS(memcmp(*check, *baseline, n));
}


/* The strings are contained in the elements themselves. */
int compare_strn    (void *context, const          char         *check, const          char         *baseline)
{
  size_t n;

#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  n = objp_to_size(context);

  if (n <= 0)
    return ORDERING_SUCCESS(ORDERING_EQ);

  return ORDERING_SUCCESS(strncmp(check, baseline, n));
}

int compare_strz    (void *context, const          char         *check, const          char         *baseline)
{
#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_SUCCESS(strcmp(check, baseline));
}


/* Elements are "const char *". */
int compare_strnr   (void *context, const          char        **check, const          char        **baseline)
{
  size_t n;

#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
  if (!*check || !*baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  n = objp_to_size(context);

  if (n <= 0)
    return ORDERING_SUCCESS(ORDERING_EQ);

  return ORDERING_SUCCESS(strncmp(*check, *baseline, n));
}

int compare_strzr   (void *context, const          char        **check, const          char        **baseline)
{

#if ERROR_CHECKING
  if (!check || !baseline)
    return ordering_err_2();
  if (!*check || !*baseline)
    return ordering_err_2();
#endif /* #if ERROR_CHECKING  */

  return ORDERING_SUCCESS(strcmp(*check, *baseline));
}

/* ---------------------------------------------------------------- */

void *compare_with_type_context    (const type_t *type)
{
  return (void *) type;
}

void *compare_ref_with_type_context(const type_t *type)
{
  return (void *) type;
}


void *compare_tval_context   (void)
{
  return NULL;
}

void *compare_tvalr_context  (void)
{
  return NULL;
}


void *compare_char_context   (void)
{
  return NULL;
}

void *compare_schar_context  (void)
{
  return NULL;
}

void *compare_uchar_context  (void)
{
  return NULL;
}

void *compare_short_context  (void)
{
  return NULL;
}

void *compare_ushort_context (void)
{
  return NULL;
}

void *compare_int_context    (void)
{
  return NULL;
}

void *compare_uint_context   (void)
{
  return NULL;
}

void *compare_long_context   (void)
{
  return NULL;
}

void *compare_ulong_context  (void)
{
  return NULL;
}

void *compare_float_context  (void)
{
  return NULL;
}

void *compare_double_context (void)
{
  return NULL;
}

void *compare_ldouble_context(void)
{
  return NULL;
}


void *compare_size_context   (void)
{
  return NULL;
}

void *compare_ptrdiff_context(void)
{
  return NULL;
}


void *compare_objp_context   (void)
{
  return NULL;
}

void *compare_funp_context   (void)
{
  return NULL;
}


void *compare_mem_context    (size_t n)
{
  return (void *) size_to_objp(n);
}

void *compare_memr_context   (size_t n)
{
  return (void *) size_to_objp(n);
}


void *compare_strn_context   (size_t n)
{
  return (void *) size_to_objp(n);
}

void *compare_strz_context   (void)
{
  return NULL;
}


void *compare_strnr_context  (size_t n)
{
  return (void *) size_to_objp(n);
}

void *compare_strzr_context  (void)
{
  return NULL;
}

/* ---------------------------------------------------------------- */

const comparer_t comparer_with_type     = (comparer_t) &compare_with_type;
const comparer_t comparer_ref_with_type = (comparer_t) &compare_ref_with_type;

const comparer_t comparer_tval          = (comparer_t) &compare_tval;
const comparer_t comparer_tvalr         = (comparer_t) &compare_tvalr;

const comparer_t comparer_char          = (comparer_t) &compare_char;
const comparer_t comparer_schar         = (comparer_t) &compare_schar;
const comparer_t comparer_uchar         = (comparer_t) &compare_uchar;
const comparer_t comparer_short         = (comparer_t) &compare_short;
const comparer_t comparer_ushort        = (comparer_t) &compare_ushort;
const comparer_t comparer_int           = (comparer_t) &compare_int;
const comparer_t comparer_uint          = (comparer_t) &compare_uint;
const comparer_t comparer_long          = (comparer_t) &compare_long;
const comparer_t comparer_ulong         = (comparer_t) &compare_ulong;
const comparer_t comparer_float         = (comparer_t) &compare_float;
const comparer_t comparer_double        = (comparer_t) &compare_double;
const comparer_t comparer_ldouble       = (comparer_t) &compare_ldouble;

const comparer_t comparer_size          = (comparer_t) &compare_size;
const comparer_t comparer_ptrdiff       = (comparer_t) &compare_ptrdiff;

const comparer_t comparer_objp          = (comparer_t) &compare_objp;
const comparer_t comparer_funp          = (comparer_t) &compare_funp;

const comparer_t comparer_mem           = (comparer_t) &compare_mem;
const comparer_t comparer_memr          = (comparer_t) &compare_memr;

const comparer_t comparer_strn          = (comparer_t) &compare_strn;
const comparer_t comparer_strz          = (comparer_t) &compare_strz;

const comparer_t comparer_strnr         = (comparer_t) &compare_strnr;
const comparer_t comparer_strzr         = (comparer_t) &compare_strnz;

/*
 * opencurry: type_base_lookup.c
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
 *   - size_t
 */
#include <stddef.h>

/* string.h:
 *   - memset
 */
#include <string.h>

#include "base.h"
#include "type_base_prim.h"
#include "type_base_lookup.h"

#include "type_base_typed.h"
#include "type_base_memory_tracker.h"
#ifdef TODO
#error "TODO: #include primitive c data types"
#else  /* #ifdef TODO */
#include "type_base.h"
#endif /* #ifdef TODO */
#include "type_base_type.h"

#include "util.h"

/* ---------------------------------------------------------------- */
/* lookup_t and children.                                           */
/* ---------------------------------------------------------------- */

/* bnode type. */

const type_t *bnode_type(void)
  { return &bnode_type_def; }

static const char          *bnode_type_name       (const type_t *self);
static size_t               bnode_type_size       (const type_t *self, const tval *val);
static const struct_info_t *bnode_type_is_struct  (const type_t *self);
static const tval          *bnode_type_has_default(const type_t *self);

const type_t bnode_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL
  , /* @indirect              */ bnode_type

  , /* self                   */ NULL
  , /* container              */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ bnode_type_name
  , /* info                   */ NULL
  , /* @size                  */ bnode_type_size
  , /* @is_struct             */ bnode_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ bnode_type_has_default
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

static const char          *bnode_type_name       (const type_t *self)
  { return "bnode_t"; }

static size_t               bnode_type_size       (const type_t *self, const tval *val)
  { return sizeof(bnode_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(bnode)
static const struct_info_t *bnode_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(bnode);

    /* size_t value; */
    STRUCT_INFO_RADD(size_type(), value);

    /* size_t left;  */
    /* size_t right; */
    STRUCT_INFO_RADD(size_type(), left);
    STRUCT_INFO_RADD(size_type(), right);

    STRUCT_INFO_DONE();
  }

static const tval          *bnode_type_has_default(const type_t *self)
  { return type_has_default_value(self, &bnode_defaults); }

/* ---------------------------------------------------------------- */

const bnode_t bnode_defaults =
  BNODE_DEFAULTS;

/* ---------------------------------------------------------------- */

/* lookup type. */

const type_t *lookup_type(void)
  { return &lookup_type_def; }

static const char          *lookup_type_name       (const type_t *self);
static size_t               lookup_type_size       (const type_t *self, const tval *val);
static const struct_info_t *lookup_type_is_struct  (const type_t *self);
static const tval          *lookup_type_has_default(const type_t *self);

const type_t lookup_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL
  , /* @indirect              */ lookup_type

  , /* self                   */ NULL
  , /* container              */ NULL

  , /* typed                  */ NULL

  , /* @name                  */ lookup_type_name
  , /* info                   */ NULL
  , /* @size                  */ lookup_type_size
  , /* @is_struct             */ lookup_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ lookup_type_has_default
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

static const char          *lookup_type_name       (const type_t *self)
  { return "lookup_t"; }

static size_t               lookup_type_size       (const type_t *self, const tval *val)
  { return sizeof(lookup_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(lookup)
static const struct_info_t *lookup_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(lookup);

    /* typed_t type; */
    STRUCT_INFO_RADD(typed_type(), type);

    /* size_t   num; */
    STRUCT_INFO_RADD(size_type(),  num);

    /* void    *values;     */
    /* size_t   value_size; */
    STRUCT_INFO_RADD(objp_type(),  values);
    STRUCT_INFO_RADD(size_type(),  value_size);

    /* bnode_t *order; */
    STRUCT_INFO_RADD(objp_type(),  order);

    /* size_t   len; */
    STRUCT_INFO_RADD(size_type(),  len);

    STRUCT_INFO_DONE();
  }

static const tval          *lookup_type_has_default(const type_t *self)
  { return type_has_default_value(self, &lookup_defaults); }

/* ---------------------------------------------------------------- */

const lookup_t lookup_defaults =
  LOOKUP_DEFAULTS;

/* ---------------------------------------------------------------- */
/* bnode_t methods.                                                 */
/* ---------------------------------------------------------------- */

void bnode_init(bnode_t *bnode)
{
#if ERROR_CHECKING
  if (!bnode)
    return;
#endif /* #if ERROR_CHECKING  */

  bnode->value = 0;

  bnode->left  = 0;
  bnode->right = 0;
}

void bnode_init_array(bnode_t *bnode, size_t num)
{
  if (!bnode)
    return;

  memset(bnode, 0x00, num * sizeof(bnode_t));
}

/* ---------------------------------------------------------------- */
/* lookup_t methods.                                                */
/* ---------------------------------------------------------------- */

/*
 * Initialize a lookup value with zero element slots.
 */
void lookup_init_empty(lookup_t *lookup, size_t value_size)
{
#if ERROR_CHECKING
  if (!lookup)
    return;
#endif /* #if ERROR_CHECKING  */

  lookup->type       = lookup_type;

  lookup->num        = 0;

  lookup->values     = NULL;
  lookup->value_size = value_size;

  lookup->order      = NULL;

  lookup->len        = 0;
}

/* Free memory allocated inside the lookup value. */
void lookup_deinit
  ( lookup_t *lookup

  , void  (*free)(void *context, void *area)
  , void   *free_context
  )
{
#if ERROR_CHECKING
  if (!lookup)
    return;
#endif /* #if ERROR_CHECKING  */

  if (lookup->num <= 0)
    return;

#if ERROR_CHECKING
  if (!lookup->values)
    return;
  if (!lookup->order)
    return;
  if (!free)
    return;
#endif /* #if ERROR_CHECKING  */

  free(free_context, lookup->values);
  free(free_context, lookup->order);

  lookup->num = 0;
}

/* Does lookup not have any used values? */
int lookup_empty(const lookup_t *lookup)
{
  return lookup_len(lookup) <= 0;
}

/* Get the number of element slots. */
size_t lookup_num(const lookup_t *lookup)
{
  return lookup->num;
}

/* Get the number of used element slots. */
size_t lookup_len(const lookup_t *lookup)
{
  return lookup->len;
}

/* Allocate more memory for additional element slots. */
/*                                                    */
/* Does nothing with a "num" argument smaller than    */
/* the lookup value's element-based size.             */
lookup_t *lookup_expand
  ( lookup_t *lookup
  , size_t    num

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context
  )
{
  size_t old_num;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  old_num = lookup_num(lookup);
  if (num <= old_num)
    return lookup;

  if (old_num <= 0)
  {
#if ERROR_CHECKING
    if (!calloc)
      return NULL;
    if (lookup->values)
      return NULL;
    if (lookup->order)
      return NULL;
#endif /* #if ERROR_CHECKING  */

    lookup->values = calloc(calloc_context, num, lookup->value_size);
    if (!lookup->values)
    {
      lookup->order = NULL;
      lookup->num   = 0;

      return NULL;
    }

    lookup->order  = calloc(calloc_context, num, sizeof(bnode_t));
    if (!lookup->order)
    {
      lookup->values = NULL;
      lookup->num    = 0;

      return NULL;
    }

    /* ---------------------------------------------------------------- */

    lookup->num = num;

    bnode_init_array(lookup->order, num);

    return lookup;
  }
  else
  {
#if ERROR_CHECKING
    if (!realloc)
      return NULL;
    if (!lookup->values)
      return NULL;
    if (!lookup->order)
      return NULL;
#endif /* #if ERROR_CHECKING  */

    lookup->values = realloc(realloc_context, lookup->values, num * lookup->value_size);
    if (!lookup->values)
    {
      lookup->order = NULL;
      lookup->num   = 0;

      return NULL;
    }

    lookup->order  = realloc(realloc_context, lookup->order,  num * sizeof(bnode_t));
    if (!lookup->order)
    {
      lookup->values = NULL;
      lookup->num    = 0;

      return NULL;
    }

    lookup->num = num;

    bnode_init_array(lookup->order + old_num, size_minus(num, old_num));

    return lookup;
  }
}

#ifdef TODO /* TODO */
/* Move elements to remove gaps of free element slots. */
void lookup_defragment(lookup_t *lookup);

/* Reallocate less memory for fewer element slots. */
/*                                                 */
/* "num" is bounded by the last used element slot. */
/* This does not defragment.                       */
/*                                                 */
/* Does nothing with a "num" argument larger than  */
/* the lookup value's element-based size.          */
lookup_t *lookup_shrink
  ( lookup_t *lookup
  , size_t    num

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , void  (*free)(void *context, void *area)
  , void   *free_context
  );

/* Reallocate more or less memory for free element slots. */
/*                                                        */
/* When shrinking, defragmentation is performed.          */
/*                                                        */
/* When shrinking, the new number is bounded by           */
/* the number of elements in use, so no data loss for     */
/* existing elements will occur.                          */
lookup_t *lookup_resize
  ( lookup_t *lookup
  , size_t    num

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , void  (*free)(void *context, void *area)
  , void   *free_context
  )
{
  size_t old_num;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  old_num = lookup_num(lookup);

  if (num < old_num)
  {
    lookup_defragment(lookup);

    return lookup_shrink
      (lookup, num, realloc, realloc_context, free,    free_context);
  }
  else if (num > old_num)
  {
    return lookup_expand
      (lookup, num, calloc,  calloc_context,  realloc, realloc_context);
  }
  else
  {
    return lookup;
  }
}

/* ---------------------------------------------------------------- */

lookup_t *lookup_insert_controlled
  ( lookup_t  *lookup
  , void      *val
  , int        add_when_exists

  , compare_t  cmp
  , void      *cmp_context

  , int       *out_already_exists
  , int       *out_no_space
  )
{
}

#endif /* #ifdef TODO /-* TODO *-/ */

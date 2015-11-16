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
 *   - ptrdiff_t
 *   - size_t
 */
#include <stddef.h>

/* string.h:
 *   - memmove
 *   - memset
 */
#include <string.h>

#include "base.h"
#include "type_base_prim.h"
#include "type_base_lookup.h"

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

    /* size_t   capacity; */
    STRUCT_INFO_RADD(size_type(),  capacity);

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

/* bnode_t field constructors. */

/* Construct values for the "value" field. */

size_t bnode_black_value(size_t index)
{
  return BNODE_BLACK_VALUE  (index);
}

size_t bnode_red_value  (size_t index)
{
  return BNODE_RED_VALUE    (index);
}

size_t bnode_colored_value(size_t index, size_t color)
{
  return BNODE_COLORED_VALUE(index, color);
}

/* Construct values for the "left" and "right" fields. */

size_t bnode_leaf (void)
{
  return BNODE_LEAF         ();
}

size_t bnode_child(size_t index)
{
  return BNODE_CHILD        (index);
}

/* ---------------------------------------------------------------- */

/* bnode_t field interpretors. */

size_t bnode_get_color(size_t value)
{
  return BNODE_GET_COLOR(value);
}

int    bnode_is_black (size_t value)
{
  return BNODE_IS_BLACK (value);
}

int    bnode_is_red   (size_t value)
{
  return BNODE_IS_RED   (value);
}


size_t bnode_get_value(size_t value)
{
  return BNODE_GET_VALUE(value);
}


int    bnode_is_leaf  (size_t ref)
{
  return BNODE_IS_LEAF  (ref);
}

size_t bnode_get_child(size_t ref)
{
  return BNODE_GET_CHILD(ref);
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

  lookup->capacity   = 0;

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

  if (lookup->capacity <= 0)
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
  lookup->values = 0;
  lookup->order  = 0;

  lookup->num = 0;

  lookup->len = 0;
}

/* ---------------------------------------------------------------- */

/* How many elements can this lookup tree contain? */
size_t lookup_capacity(const lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_CAPACITY(lookup);
}

/* Does this lookup container have no memory allocated for elements? */
int    lookup_null    (const lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return -1;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_CAPACITY(lookup) <= 0;
}

/* ---------------------------------------------------------------- */

/* How many elements does "lookup" contain? */
size_t lookup_len     (const lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_LEN(lookup);
}

/* Does "lookup" contain no elements? */
int    lookup_empty   (const lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return -1;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_LEN(lookup) <= 0;
}

/* ---------------------------------------------------------------- */

int    lookup_max_capacity(const lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return -1;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_LEN(lookup) >= LOOKUP_CAPACITY(lookup);
}

/* ---------------------------------------------------------------- */

size_t lookup_value_size(const lookup_t *lookup)
{
  return LOOKUP_VALUE_SIZE(lookup);
}

/* ---------------------------------------------------------------- */

/* Allocate more memory for additional element slots. */
/*                                                    */
/* Does nothing with a "num" argument smaller than    */
/* the lookup value's element-based size.             */
lookup_t *lookup_expand
  ( lookup_t *lookup
  , size_t    capacity

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context
  )
{
  size_t old_capacity;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  old_capacity = LOOKUP_CAPACITY(lookup);
  if (capacity <= old_capacity)
    return lookup;

  if (old_capacity <= 0)
  {
#if ERROR_CHECKING
    if (!calloc)
      return NULL;
    if (lookup->values)
      return NULL;
    if (lookup->order)
      return NULL;
#endif /* #if ERROR_CHECKING  */

    lookup->values = calloc(calloc_context, capacity, lookup->value_size);
    if (!lookup->values)
    {
      lookup->order    = NULL;
      lookup->capacity = 0;

      return NULL;
    }

    lookup->order  = calloc(calloc_context, capacity, sizeof(bnode_t));
    if (!lookup->order)
    {
      lookup->values   = NULL;
      lookup->capacity = 0;

      return NULL;
    }

    /* ---------------------------------------------------------------- */

    lookup->capacity = capacity;

    bnode_init_array(lookup->order, capacity);

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

    lookup->values = realloc(realloc_context, lookup->values, capacity * lookup->value_size);
    if (!lookup->values)
    {
      lookup->order    = NULL;
      lookup->capacity = 0;

      return NULL;
    }

    lookup->order  = realloc(realloc_context, lookup->order,  capacity * sizeof(bnode_t));
    if (!lookup->order)
    {
      lookup->values   = NULL;
      lookup->capacity = 0;

      return NULL;
    }

    lookup->capacity = capacity;

    bnode_init_array(lookup->order + old_capacity, size_minus(capacity, old_capacity));

    return lookup;
  }
}

#ifdef TODO /* TODO */
/* Move elements to remove gaps of free element slots. */
void lookup_defragment(lookup_t *lookup);

/* Reallocate less memory for fewer element slots.      */
/*                                                      */
/* "capacity" is bounded by the last used element slot. */
/* This does not defragment.                            */
/*                                                      */
/* Does nothing with a "num" argument larger than       */
/* the lookup value's element-based size.               */
lookup_t *lookup_shrink
  ( lookup_t *lookup
  , size_t    capacity

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
  , size_t    capacity

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , void  (*free)(void *context, void *area)
  , void   *free_context
  )
{
  size_t old_capacity;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  old_capacity = LOOKUP_CAPACITY(lookup);

  if (capacity < old_capacity)
  {
    lookup_defragment(lookup);

    return lookup_shrink
      (lookup, capacity, realloc, realloc_context, free,    free_context);
  }
  else if (capacity > old_capacity)
  {
    return lookup_expand
      (lookup, capacity, calloc,  calloc_context,  realloc, realloc_context);
  }
  else
  {
    return lookup;
  }
}

#endif /* #ifdef TODO /-* TODO *-/ */

/* ---------------------------------------------------------------- */

void    *lookup_index_value(lookup_t *lookup, size_t index)
{
  return LOOKUP_INDEX_VALUE(lookup, index);
}

bnode_t *lookup_index_order(lookup_t *lookup, size_t index)
{
  return LOOKUP_INDEX_ORDER(lookup, index);
}

/* ---------------------------------------------------------------- */

/*
 * TODO
 *
 * If the tree reaches maximum capacity, enable out_max_capacity.
 *
 * If there is no space to insert a value, enable out_max_capacity and return
 * NULL.
 */
lookup_t *lookup_insert_controlled
  ( lookup_t   *lookup
  , const void *val
  , int         add_when_exists

  , compare_t   cmp
  , void       *cmp_context

  , int        *out_already_exists
  , int        *out_max_capacity
  )
{
  size_t   capacity;

  size_t   value;
  size_t   node;

  void    *dest;
  bnode_t *cur;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  /* Is a value provided? */
  if (!val)
  {
    WRITE_OUTPUT(out_already_exists, 0);
    WRITE_OUTPUT(out_max_capacity,   lookup_max_capacity(lookup));

    return NULL;
  }

  /* Are we inserting the first element? */
  if (lookup_empty(lookup))
  {
    capacity = lookup_capacity(lookup);

    /* Do we have space? */
    if (capacity <= 0)
    {
      WRITE_OUTPUT(out_already_exists, 0);
      WRITE_OUTPUT(out_max_capacity,   1);
      return NULL;
    }

    /* ---------------------------------------------------------------- */

    /* Set indices. */
    value = 0;
    node  = 0;
    dest = LOOKUP_INDEX_VALUE(lookup, value);
    cur  = LOOKUP_INDEX_ORDER(lookup, value);

    /* Write the value. */
    memmove(dest, val, LOOKUP_VALUE_SIZE(lookup));

    /* Write the node. */
    cur->value = BNODE_BLACK_VALUE(value);
    cur->left  = BNODE_LEAF();
    cur->right = BNODE_LEAF();

    /* Update len. */
    ++lookup->len;

    /* Write output. */
    WRITE_OUTPUT(out_already_exists, 0);
    WRITE_OUTPUT(out_max_capacity,   lookup_max_capacity(lookup));

    /* Success. */
    return lookup;
  }
  else
  {
#if ERROR_CHECKING
    if (!cmp)
      return NULL;
#endif /* #if ERROR_CHECKING  */

    /* Traverse our binary search tree: */
    /*                                  */
    /* Find the first equivalent node,  */
    /* or the first in-order leaf.      */
    value = 0;
    node  = 0;
    dest  = LOOKUP_INDEX_VALUE(lookup, value);
    cur   = LOOKUP_INDEX_ORDER(lookup, value);
    for (;;)
    {
      int ordering;

      ordering = cmp(cmp_context, val, dest);

      if ()
      {
      }
    }
  }

  /* TODO */
  *out_already_exists = 0;
  *out_max_capacity   = 0;

  return lookup;
}

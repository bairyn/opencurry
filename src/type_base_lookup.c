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
#include "type_base_compare.h"
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
    /* size_t   next_value; */
    STRUCT_INFO_RADD(objp_type(),  values);
    STRUCT_INFO_RADD(size_type(),  value_size);
    STRUCT_INFO_RADD(size_type(),  next_value);

    /* bnode_t *order;      */
    /* size_t   next_order; */
    STRUCT_INFO_RADD(objp_type(),  order);
    STRUCT_INFO_RADD(size_type(),  next_order);

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

size_t bnode_red_value(size_t index)
{
  return BNODE_RED_VALUE    (index);
}

size_t bnode_colored_value(size_t index, size_t color)
{
  return BNODE_COLORED_VALUE(index, color);
}

/* Construct values for the "left" and "right" fields. */

size_t bnode_leaf(void)
{
  return BNODE_LEAF         ();
}

size_t bnode_ref(size_t index)
{
  return BNODE_REF          (index);
}

size_t bnode_set_order_in_use_bit(bnode_t *bnode, size_t bit)
{
#if ERROR_CHECKING
  if (!bnode)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return (BNODE_SET_ORDER_IN_USE_BIT(bnode, bit));
}

size_t bnode_set_value_in_use_bit(bnode_t *bnode, size_t bit)
{
#if ERROR_CHECKING
  if (!bnode)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return (BNODE_SET_VALUE_IN_USE_BIT(bnode, bit));
}

size_t *bnode_get_child(bnode_t *bnode, int ordering)
{
#if ERROR_CHECKING
  if (!bnode)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  return BNODE_GET_CHILD(bnode, ordering);
}

/* ---------------------------------------------------------------- */

/* bnode_t field interpretors. */

size_t bnode_get_color(size_t value)
{
  return BNODE_GET_COLOR(value);
}

int    bnode_is_black(size_t value)
{
  return BNODE_IS_BLACK (value);
}

int    bnode_is_red(size_t value)
{
  return BNODE_IS_RED   (value);
}


size_t bnode_get_value(size_t value)
{
  return BNODE_GET_VALUE(value);
}


int    bnode_is_leaf(size_t ref)
{
  return BNODE_IS_LEAF  (ref);
}

size_t bnode_get_ref(size_t ref)
{
  return BNODE_GET_REF(ref);
}

size_t bnode_get_order_in_use_bit(const bnode_t *bnode)
{
#if ERROR_CHECKING
  if (!bnode)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return BNODE_GET_ORDER_IN_USE_BIT(bnode);
}

size_t bnode_get_value_in_use_bit(const bnode_t *bnode)
{
#if ERROR_CHECKING
  if (!bnode)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return BNODE_GET_VALUE_IN_USE_BIT(bnode);
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
  lookup->next_value = 0;

  lookup->order      = NULL;
  lookup->next_order = 0;

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
  lookup->values   = NULL;
  lookup->order    = NULL;

  lookup->capacity = 0;

  lookup->len      = 0;
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

int lookup_is_recycling(const lookup_t *lookup)
{
  return LOOKUP_IS_RECYCLING(lookup);
}

int lookup_is_value_recycling(const lookup_t *lookup)
{
  return LOOKUP_IS_VALUE_RECYCLING(lookup);
}

int lookup_is_order_recycling(const lookup_t *lookup)
{
  return LOOKUP_IS_ORDER_RECYCLING(lookup);
}

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

int lookup_is_value_free(const lookup_t *lookup, size_t value)
{
#if ERROR_CHECKING
  if (!lookup)
    return -1;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_IS_VALUE_FREE(lookup, value);
}

int lookup_is_order_free(const lookup_t *lookup, size_t order)
{
#if ERROR_CHECKING
  if (!lookup)
    return -1;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_IS_ORDER_FREE(lookup, order);
}

size_t lookup_next_value(lookup_t *lookup)
{
  size_t current_value;

#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  current_value = lookup->next_value++;

  for (; lookup->next_value < lookup->capacity; ++lookup->next_value)
    if (!LOOKUP_IS_VALUE_FREE(lookup, lookup->next_value))
      return current_value;

  for (lookup->next_value = 0; lookup->next_value < current_value; ++lookup->next_value)
    if (!LOOKUP_IS_VALUE_FREE(lookup, lookup->next_value))
      return current_value;

  return 0;
}

size_t lookup_next_order(lookup_t *lookup)
{
  size_t current_order;

#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  current_order = lookup->next_order++;

  for (; lookup->next_order < lookup->capacity; ++lookup->next_order)
    if (!LOOKUP_IS_ORDER_FREE(lookup, lookup->next_order))
      return current_order;

  for (lookup->next_order = 0; lookup->next_order < current_order; ++lookup->next_order)
    if (!LOOKUP_IS_ORDER_FREE(lookup, lookup->next_order))
      return current_order;

  return 0;
}

size_t lookup_set_is_value_free(lookup_t *lookup, size_t index, size_t bit)
{
  return (LOOKUP_SET_IS_VALUE_FREE(lookup, index, bit));
}

size_t lookup_set_is_order_free(lookup_t *lookup, size_t index, size_t bit)
{
  return (LOOKUP_SET_IS_ORDER_FREE(lookup, index, bit));
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
lookup_t *lookup_insert
  ( lookup_t           *lookup
  , const void         *val
  , int                 add_when_exists

  , callback_compare_t  cmp

  , int                *out_already_exists
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
    return NULL;
  }

  capacity = lookup_capacity(lookup);

  /* Are we inserting the first element? */
  if (lookup_empty(lookup))
  {
    WRITE_OUTPUT(out_already_exists, 0);

    /* Do we have space? */
    if (capacity <= 0)
      return NULL;

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
    lookup_next_value(lookup);
    lookup_next_order(lookup);
    ++lookup->len;

    BNODE_SET_ORDER_IN_USE_BIT(cur, 1);
    BNODE_SET_VALUE_IN_USE_BIT(cur, 1);

    /* Success. */
    return lookup;
  }
  else
  {
    size_t child;

    int ordering;

    /* Traverse our binary search tree until we reach a leaf. */
    node = 0;
    for (;;)
    {
      cur   = LOOKUP_INDEX_ORDER(lookup, node);
      dest  = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(cur->value));

      ordering = call_callback_compare(cmp, val, dest);

#if ERROR_CHECKING
      if (IS_ORDERING_ERROR(ordering))
        return NULL;
#endif /* #if ERROR_CHECKING  */

      if (ordering <= 0)
        child = cur->left;
      else
        child = cur->right;

      if (BNODE_IS_LEAF(child))
        break;
      else
        node = BNODE_GET_REF(child);
    }

    /* "node" refers to the index of the parent of the leaf.   */
    /* "ordering" refers to the result of the last comparison. */

    if (ordering == 0)
    {
      WRITE_OUTPUT(out_already_exists, 1);

      if (!add_when_exists)
        return lookup;
    }
    else
    {
      WRITE_OUTPUT(out_already_exists, 0);
    }

    if (lookup->len >= lookup->capacity)
    {
      /* Out of space. */
      return NULL;
    }

    /* Get the next free index. */
    /*
    value = lookup_next_value(lookup);
    child = lookup_next_order(lookup);
    */
    value = lookup->next_value++;
    child = lookup->next_order++;
    ++lookup->len;

    BNODE_SET_REF(cur, ordering, child);

    cur   = LOOKUP_INDEX_ORDER(lookup, child);
    dest  = LOOKUP_INDEX_VALUE(lookup, value);

    /* Write the value. */
    memmove(dest, val, LOOKUP_VALUE_SIZE(lookup));
    cur->value = BNODE_BLACK_VALUE(value);
    cur->left  = BNODE_LEAF();
    cur->right = BNODE_LEAF();

    BNODE_SET_ORDER_IN_USE_BIT(cur, 1);
    BNODE_SET_VALUE_IN_USE_BIT(cur, 1);

    /* TODO: balance! */
    return lookup;
  }
}

void *lookup_retrieve
  ( lookup_t           *lookup
  , const void         *val

  , callback_compare_t  cmp
  )
{
  size_t   node;

  void    *dest;
  bnode_t *cur;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (!val)
    return NULL;

  if (lookup_empty(lookup))
    return NULL;

  node = 0;
  for (;;)
  {
    int ordering;

    cur   = LOOKUP_INDEX_ORDER(lookup, node);
    dest  = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(cur->value));

    ordering = call_callback_compare(cmp, val, dest);

#if ERROR_CHECKING
    if (IS_ORDERING_ERROR(ordering))
      return NULL;
#endif /* #if ERROR_CHECKING  */

    if      (ordering < 0)
      node = cur->left;
    else if (ordering > 0)
      node = cur->right;
    else
      return dest;

    if (BNODE_IS_LEAF(node))
      break;
    else
      node = BNODE_GET_REF(node);
  }

  return NULL;
}

lookup_t *lookup_remove
  ( lookup_t           *lookup
  , const void         *val

  , callback_compare_t  cmp

  , int                *out_missing
  )
{
  size_t   parent;
  size_t   node;

  void    *dest;
  bnode_t *par;
  bnode_t *cur;

  int      par_ordering = 0;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (!val)
  {
    WRITE_OUTPUT(out_missing, 0);
    return NULL;
  }

  if (lookup_empty(lookup))
  {
    WRITE_OUTPUT(out_missing, 1);
    return NULL;
  }

  parent = 0;
  node   = 0;
  for (;;)
  {
    int      ordering;
    size_t   child;

    cur   = LOOKUP_INDEX_ORDER(lookup, node);
    dest  = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(cur->value));

    ordering = call_callback_compare(cmp, val, dest);

#if ERROR_CHECKING
    if (IS_ORDERING_ERROR(ordering))
      return NULL;
#endif /* #if ERROR_CHECKING  */

    if      (ordering < 0)
      child = cur->left;
    else if (ordering > 0)
      child = cur->right;
    else
      break;

    parent       = node;
    par_ordering = ordering;

    if (BNODE_IS_LEAF(node))
    {
      WRITE_OUTPUT(out_missing, 1);
      return lookup;
    }
    else
    {
      parent = node;
      node   = BNODE_GET_REF(child);
    }
  }

  WRITE_OUTPUT(out_missing, 0);
  node = BNODE_GET_REF(node);

  /* cur => node (we're removing "node") */
  par = LOOKUP_INDEX_ORDER(lookup, parent);

  /* First, mark the element as free in our container. */
  LOOKUP_SET_IS_VALUE_FREE(lookup, BNODE_GET_VALUE(cur->value), 1);

  /* Second, update the tree. */

  /* Are we removing the root node? */
  if (parent == node)
  {
    /* TODO */
  }
  else
  {
    if (BNODE_IS_LEAF(cur->left) && BNODE_IS_LEAF(cur->right))
    {
      BNODE_SET_LEAF(par, par_ordering);
      LOOKUP_SET_IS_ORDER_FREE(lookup, node, 1);
    }
    else if (BNODE_IS_LEAF(cur->left))
    {
      BNODE_SET_REF(par, par_ordering, BNODE_GET_REF(cur->right));
      LOOKUP_SET_IS_ORDER_FREE(lookup, node, 1);
    }
    else if (BNODE_IS_LEAF(cur->right))
    {
      BNODE_SET_REF(par, par_ordering, BNODE_GET_REF(cur->left));
      LOOKUP_SET_IS_ORDER_FREE(lookup, node, 1);
    }
    else
    {
      /* TODO */
    }
  }

  --lookup->len;

  return lookup;
}

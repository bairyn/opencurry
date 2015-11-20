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

#include "bits.h"
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

void bnode_init(bnode_t *node)
{
#if ERROR_CHECKING
  if (!node)
    return;
#endif /* #if ERROR_CHECKING  */

  node->value = 0;

  node->left  = 0;
  node->right = 0;
}

void bnode_init_array(bnode_t *node, size_t num)
{
  if (!node)
    return;

  memset(node, 0x00, num * sizeof(bnode_t));
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

size_t bnode_set_order_in_use_bit(bnode_t *node, size_t bit)
{
#if ERROR_CHECKING
  if (!node)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return (BNODE_SET_ORDER_IN_USE_BIT(node, bit));
}

size_t bnode_set_value_in_use_bit(bnode_t *node, size_t bit)
{
#if ERROR_CHECKING
  if (!node)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return (BNODE_SET_VALUE_IN_USE_BIT(node, bit));
}

size_t *bnode_get_child(bnode_t *node, int ordering)
{
#if ERROR_CHECKING
  if (!node)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  return BNODE_GET_CHILD(node, ordering);
}

size_t bnode_link_set_leaf(size_t *link)
{
  return BNODE_LINK_SET_LEAF(link);
}

size_t bnode_link_set_ref (size_t *link, size_t index)
{
  return BNODE_LINK_SET_REF (link, index);
}

size_t bnode_set_leaf(bnode_t *bnode, int ordering)
{
  return BNODE_SET_LEAF(bnode, ordering);
}

size_t bnode_set_ref (bnode_t *bnode, int ordering, size_t index)
{
  return BNODE_SET_REF (bnode, ordering, index);
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

size_t bnode_get_order_in_use_bit(const bnode_t *node)
{
#if ERROR_CHECKING
  if (!node)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return BNODE_GET_ORDER_IN_USE_BIT(node);
}

size_t bnode_get_value_in_use_bit(const bnode_t *node)
{
#if ERROR_CHECKING
  if (!node)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return BNODE_GET_VALUE_IN_USE_BIT(node);
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

  return LOOKUP_NULL(lookup);
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

  return LOOKUP_EMPTY(lookup);
}

/* ---------------------------------------------------------------- */

int lookup_max_capacity(const lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return -1;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_MAX_CAPACITY(lookup);
}

/* ---------------------------------------------------------------- */

size_t lookup_value_size(const lookup_t *lookup)
{
  return LOOKUP_VALUE_SIZE(lookup);
}

/* ---------------------------------------------------------------- */

size_t lookup_set_order_in_use_bit(lookup_t *lookup, size_t index, size_t bit)
{
#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_SET_ORDER_IN_USE_BIT(lookup, index, bit);
}

size_t lookup_set_value_in_use_bit(lookup_t *lookup, size_t index, size_t bit)
{
#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_SET_VALUE_IN_USE_BIT(lookup, index, bit);
}


size_t lookup_get_order_in_use_bit(const lookup_t *lookup, size_t index)
{
#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_GET_ORDER_IN_USE_BIT(lookup, index);
}

size_t lookup_get_value_in_use_bit(const lookup_t *lookup, size_t index)
{
#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  return LOOKUP_GET_VALUE_IN_USE_BIT(lookup, index);
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

/* If we're either at max capacity or recycling, then double the capacity. */
/*                                                                         */
/* auto_defragment:                                                        */
/*   If recycling, first defragment and only continue if we're using more  */
/*   than half the capacity.                                               */
lookup_t *lookup_auto_expand
  ( lookup_t *lookup
  , int       auto_defragment

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , int *out_expanded
  , int *out_defragmented
  )
{
#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (!(LOOKUP_MAX_CAPACITY(lookup)) && !(LOOKUP_IS_RECYCLING(lookup)))
  {
    WRITE_OUTPUT(out_expanded,     0);
    WRITE_OUTPUT(out_defragmented, 0);

    return lookup;
  }

  if (auto_defragment && LOOKUP_IS_RECYCLING(lookup))
  {
    WRITE_OUTPUT(out_defragmented, 1);

    lookup_defragment(lookup);

    /* Are we not using more than half the capacity? */
    if ((LOOKUP_LEN(lookup)) <= ((LOOKUP_CAPACITY(lookup)) >> 1))
    {
      WRITE_OUTPUT(out_expanded, 0);
      return lookup;
    }
  }
  else
  {
    WRITE_OUTPUT(out_defragmented, 0);
  }

  WRITE_OUTPUT(out_expanded, 1);

  return
    lookup_expand
      ( lookup
      , LOOKUP_CAPACITY(lookup) << 1

      , calloc
      , calloc_context

      , realloc
      , realloc_context
      );
}

/* "lookup_auto_expand" except without the output parameters. */
lookup_t *lookup_auto_expand_simple
  ( lookup_t *lookup
  , int       auto_defragment

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context
  )
{
#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (!(LOOKUP_MAX_CAPACITY(lookup)) && !(LOOKUP_IS_RECYCLING(lookup)))
    return lookup;

  if (auto_defragment && LOOKUP_IS_RECYCLING(lookup))
  {
    lookup_defragment(lookup);

    /* Are we not using more than half the capacity? */
    if ((LOOKUP_LEN(lookup)) <= ((LOOKUP_CAPACITY(lookup)) >> 1))
      return lookup;
  }

  return
    lookup_expand
      ( lookup
      , LOOKUP_CAPACITY(lookup) << 1

      , calloc
      , calloc_context

      , realloc
      , realloc_context
      );
}

/* Move elements to remove gaps of free element slots. */
void lookup_defragment(lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return;
#endif /* #if ERROR_CHECKING  */

  /* TODO */
#ifdef TODO
#error "TODO: implement lookup_defragment."
#endif /* #ifdef TODO */
}

#ifdef TODO /* TODO */
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

const void *lookup_index_cvalue(const lookup_t *lookup, size_t index)
{
  return LOOKUP_INDEX_CVALUE(lookup, index);
}

const bnode_t *lookup_index_corder(const lookup_t *lookup, size_t index)
{
  return LOOKUP_INDEX_CORDER(lookup, index);
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

size_t balanced_btree_max_nodes(int height)
{
  /*
   * Max nodes from g to each level:
   *
   *                                         r                          '
   *                                     ___/ \___...                   '
   *                                    /                               '
   *                                   /                                '
   *                                  g                                 | 1
   *                 ________________/ \________________                |
   *                /                                   \               |
   *               /                                     \              |
   *              p                                       u             | 2 + 1
   *        _____/ \_____                           _____/ \_____       |
   *       /             \                         /             \      |
   *      /               \                       /               \     |
   *     n                 s                     A                 B    | 3 + 2 + 1
   *    / \               / \                   / \               / \   |
   *   /   \             /   \                 /   \             /   \  |
   *  l     r           C     D               E     F           G     H | 4 + 3 + 2 + 1
   *
   * r: Root node.
   * g: Grandparent node.
   * p: Parent node.
   * u: Aunt node.
   * n: Current node.
   * l: Left child.
   * r: Right child.
   *
   * ----------------------------------------------------------------
   *
   * f(height) = max nodes      | binary version
   *
   * f(0)      = 1              | 0000 0001
   * f(1)      = 2 + 1          | 0000 0011
   * f(2)      = 3 + 2 + 1      | 0000 0111
   * f(3)      = 4 + 3 + 2 + 1  | 0000 1111
   *
   * f(h)      = ONE_BIT_REPEAT(h + 1)
   */

  return BALANCED_BTREE_MAX_NODES(height);
}

int balanced_btree_max_height(size_t num_nodes)
{
  if (num_nodes == 0)
    return -1;

  /*
   * Max nodes from g to each level:
   *
   *                                         r                          '
   *                                     ___/ \___...                   '
   *                                    /                               '
   *                                   /                                '
   *                                  g                                 | 1
   *                 ________________/ \________________                |
   *                /                                   \               |
   *               /                                     \              |
   *              p                                       u             | 2 + 1
   *        _____/ \_____                           _____/ \_____       |
   *       /             \                         /             \      |
   *      /               \                       /               \     |
   *     n                 s                     A                 B    | 3 + 2 + 1
   *    / \               / \                   / \               / \   |
   *   /   \             /   \                 /   \             /   \  |
   *  l     r           C     D               E     F           G     H | 4 + 3 + 2 + 1
   *
   * r: Root node.
   * g: Grandparent node.
   * p: Parent node.
   * u: Aunt node.
   * n: Current node.
   * l: Left child.
   * r: Right child.
   *
   * ----------------------------------------------------------------
   *
   * f(num) | binary num = height
   *
   * f(1)   | 0000 0001  = 0
   *
   *
   * f(2)   | 0000 0010  = 1
   * f(3)   | 0000 0011  = 1
   *
   *
   * f(4)   | 0000 0100  = 2
   * f(5)   | 0000 0101  = 2
   * f(6)   | 0000 0110  = 2
   * f(7)   | 0000 0111  = 2
   *
   *
   * f(8)   | 0000 1000  = 3
   * f(9)   | 0000 1001  = 3
   * f(10)  | 0000 1010  = 3
   * f(11)  | 0000 1011  = 3
   *
   * f(12)  | 0000 1100  = 3
   * f(13)  | 0000 1101  = 3
   * f(14)  | 0000 1110  = 3
   * f(15)  | 0000 1111  = 3
   *
   * f(num >= 1)         = MOST_SIGNIFICANT_BIT_POS(num)
   */

  return most_significant_bit_pos_ulong((unsigned long) num_nodes);
}

int lookup_max_height(size_t num_nodes)
{
  if (num_nodes == 0)
    return -1;

  /* http://doctrina.org/maximum-height-of-red-black-tree.html */
  return most_significant_bit_pos_ulong((unsigned long) (num_nodes + 1)) << 1;
}

int lookup_height_from(const lookup_t *lookup, const bnode_t *node)
{
#if ERROR_CHECKING
  if (!lookup)
    return -2;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return -1;

  if (!node)
    node = &lookup->order[0];

  if      ( BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
    return 0;
  else if (!BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
    return lookup_height_from(lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->left ))) + 1;
  else if ( BNODE_IS_LEAF(node->left) && !BNODE_IS_LEAF(node->right))
    return lookup_height_from(lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right))) + 1;
  else
    return
      MAX
        ( lookup_height_from(lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->left )))
        , lookup_height_from(lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right)))
        ) + 1;
}

int lookup_height(const lookup_t *lookup)
{
  return lookup_height_from(lookup, NULL);
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

  , int                *out_is_duplicate
  )
{
  size_t   capacity;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  WRITE_OUTPUT(out_is_duplicate, -1);

  /* Is a value provided? */
  if (!val)
    return NULL;

  capacity = lookup_capacity(lookup);

  /* Are we inserting the first element? */
  if (lookup_empty(lookup))
  {
    size_t   value;
    size_t   node;

    void    *dest;
    bnode_t *cur;

    WRITE_OUTPUT(out_is_duplicate, 0);

    /* Do we have space? */
    if (capacity <= 0)
      return NULL;

    /* ---------------------------------------------------------------- */

    /* Set indices. */
    value = 0;
    node  = 0;
    dest = LOOKUP_INDEX_VALUE(lookup, value);
    cur  = LOOKUP_INDEX_ORDER(lookup, node);

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
    bnode_t    *parent;
    bnode_t    *node;
    size_t     *child_link;

    const void *node_val;

    int         ordering;

    UNUSED(parent);
    SUPPRESS_UNINITIALIZED(child_link);

    /* Ordered BST traversal to leaf or first duplicate. */
    parent = NULL;
    node   = &lookup->order[0];
    for(;;)
    {
      /* val <?= node value */
      node_val = LOOKUP_INDEX_VALUE(lookup, node->value);
      ordering = call_callback_compare(cmp, val, node_val);

#if ERROR_CHECKING
      if (IS_ORDERING_ERROR(ordering))
        return NULL;
#endif /* #if ERROR_CHECKING  */

      if      (ordering < 0)
        child_link = &node->left;
      else if (ordering > 0)
        child_link = &node->right;
      else
        break;

      if (BNODE_IS_LEAF(*child_link))
        break;

      parent = node;
      node   = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(*child_link));
    }

    /* Is duplicate? */
    if (ordering == 0)
    {
      WRITE_OUTPUT(out_is_duplicate, 1);

      if (!add_when_exists)
        return lookup;

      child_link = &node->left;
    }
    else
    {
      WRITE_OUTPUT(out_is_duplicate, 0);
    }

    /* Out of space? */
    if (LOOKUP_MAX_CAPACITY(lookup))
    {
      return NULL;
    }
    else
    {
      /* Add the node. */

      size_t   child_index;
      size_t   child_val_index;

      bnode_t *child;
      void    *child_val;

      child_index     = lookup_next_order(lookup);
      child_val_index = lookup_next_value(lookup);
      ++lookup->len;

      child     = LOOKUP_INDEX_ORDER(lookup, child_index);
      child_val = LOOKUP_INDEX_VALUE(lookup, child_val_index);

      child->value = BNODE_BLACK_VALUE(child_val_index);

      /* Write the value. */
      memmove(child_val, val, LOOKUP_VALUE_SIZE(lookup));

      /* Link the node. */
      if (ordering == 0)
      {
        /* Inserting a duplicate.  Make "child" node's (left) child. */

        /* Is "node's" (left) child currently a leaf? */
        if (BNODE_IS_LEAF(*child_link))
          BNODE_LINK_SET_LEAF(&child->left);
        else
          BNODE_LINK_SET_REF (&child->left, *child_link);
        BNODE_LINK_SET_LEAF(&child->right);
        BNODE_LINK_SET_REF (child_link, child_index);
      }
      else
      {
        BNODE_LINK_SET_LEAF(&child->left);
        BNODE_LINK_SET_LEAF(&child->right);
        BNODE_LINK_SET_REF (child_link, child_index);
      }

      /* Finalize the new node. */
      LOOKUP_SET_ORDER_IN_USE_BIT(lookup, child_index,     1);
      LOOKUP_SET_VALUE_IN_USE_BIT(lookup, child_val_index, 1);

      /* TODO: balance! */
      return lookup;
    }
  }
}

const void *lookup_retrieve
  ( const lookup_t     *lookup
  , const void         *val

  , callback_compare_t  cmp

  , const bnode_t     **out_node
  )
{
  size_t         node;

  const void    *dest;
  const bnode_t *cur;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  WRITE_OUTPUT(out_node, NULL);

  if (!val)
    return NULL;

  if (lookup_empty(lookup))
    return NULL;

  node = 0;
  for (;;)
  {
    int ordering;

    cur   = LOOKUP_INDEX_CORDER(lookup, node);
    dest  = LOOKUP_INDEX_CVALUE(lookup, BNODE_GET_VALUE(cur->value));

    ordering = call_callback_compare(cmp, val, dest);

#if ERROR_CHECKING
    if (IS_ORDERING_ERROR(ordering))
      return NULL;
#endif /* #if ERROR_CHECKING  */

    if      (ordering < 0)
    {
      node = cur->left;
    }
    else if (ordering > 0)
    {
      node = cur->right;
    }
    else
    {
      WRITE_OUTPUT(out_node, cur);
      return dest;
    }

    if (BNODE_IS_LEAF(node))
      break;
    else
      node = BNODE_GET_REF(node);
  }

  WRITE_OUTPUT(out_node, NULL);
  return NULL;
}

void *lookup_min(lookup_t *lookup)
{
  const bnode_t *cur;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return NULL;

  for
    ( cur = &lookup->order[0]
    ; !BNODE_IS_LEAF(cur->left)
    ; cur = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(cur->left))
    )
    ;

  return LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(cur->value));
}

void *lookup_max(lookup_t *lookup)
{
  const bnode_t *cur;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return NULL;

  for
    ( cur = &lookup->order[0]
    ; !BNODE_IS_LEAF(cur->right)
    ; cur = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(cur->right))
    )
    ;

  return LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(cur->value));
}

const void *lookup_cmin(const lookup_t *lookup)
{
  const bnode_t *cur;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return NULL;

  for
    ( cur = &lookup->order[0]
    ; !BNODE_IS_LEAF(cur->left)
    ; cur = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(cur->left))
    )
    ;

  return LOOKUP_INDEX_CVALUE(lookup, BNODE_GET_VALUE(cur->value));
}

const void *lookup_cmax(const lookup_t *lookup)
{
  const bnode_t *cur;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return NULL;

  for
    ( cur = &lookup->order[0]
    ; !BNODE_IS_LEAF(cur->right)
    ; cur = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(cur->right))
    )
    ;

  return LOOKUP_INDEX_CVALUE(lookup, BNODE_GET_VALUE(cur->value));
}

lookup_t *lookup_delete
  ( lookup_t           *lookup
  , const void         *val

  , callback_compare_t  cmp

  , int                *out_num_deleted
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
    WRITE_OUTPUT(out_num_deleted, 0);
    return NULL;
  }

  if (lookup_empty(lookup))
  {
    WRITE_OUTPUT(out_num_deleted, 0);
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
      WRITE_OUTPUT(out_num_deleted, 0);
      return lookup;
    }
    else
    {
      parent = node;
      node   = BNODE_GET_REF(child);
    }
  }

  WRITE_OUTPUT(out_num_deleted, 1);
  node = BNODE_GET_REF(node);

  /* cur => node (we're removing "node") */
  par = LOOKUP_INDEX_ORDER(lookup, parent);

  /* First, mark the element as free in our container. */
  LOOKUP_SET_IS_VALUE_FREE(lookup, BNODE_GET_VALUE(cur->value), 1);

  /* Second, update the tree. */

  /* TODO: duplicates! */

  /* Are we removing the root node? */
  if (parent == node)
  {
    if (BNODE_IS_LEAF(cur->left) && BNODE_IS_LEAF(cur->right))
    {
      LOOKUP_SET_ORDER_IN_USE_BIT(lookup, node, 0);
    }
    else
    {
      /* Replace "node" with the left-most node of "node"'s right-sub-tree. */
      size_t *child;

      for
        ( child =        &cur->right
        ; !BNODE_IS_LEAF( LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(*child))->left)
        ; child =       (&LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(*child))->left)
        );

      cur->value = BNODE_GET_VALUE( LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(*child))->value );

      /* Remove bottom node. */
      LOOKUP_SET_IS_ORDER_FREE(lookup, *child, 1);
      BNODE_LINK_SET_LEAF(child);
    }
  }
  else
  {
    if (BNODE_IS_LEAF(cur->left) && BNODE_IS_LEAF(cur->right))
    {
      /* Bottom node; make parent's child link a leaf, and free "node". */
      BNODE_SET_LEAF(par, par_ordering);
      LOOKUP_SET_IS_ORDER_FREE(lookup, node, 1);
    }
    else if (!BNODE_IS_LEAF(cur->left))
    {
      /* Has only a left-sub-tree; set parent's child link to it */
      /* and free "node".                                        */
      BNODE_SET_REF(par, par_ordering, BNODE_GET_REF(cur->left));
      LOOKUP_SET_IS_ORDER_FREE(lookup, node, 1);
    }
    else if (!BNODE_IS_LEAF(cur->right))
    {
      /* Has only a right-sub-tree; set parent's child link to it */
      /* and free "node".                                         */
      BNODE_SET_REF(par, par_ordering, BNODE_GET_REF(cur->right));
      LOOKUP_SET_IS_ORDER_FREE(lookup, node, 1);
    }
    else
    {
      /* Replace "node" with the left-most node of "node"'s right-sub-tree. */
      size_t *child;

      for
        ( child =        &cur->right
        ; !BNODE_IS_LEAF( LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(*child))->left)
        ; child =       (&LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(*child))->left)
        );

      cur->value = BNODE_GET_VALUE( LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(*child))->value );

      /* Remove bottom node. */
      LOOKUP_SET_IS_ORDER_FREE(lookup, *child, 1);
      BNODE_LINK_SET_LEAF(child);
    }
  }

  --lookup->len;

  return lookup;
}

/* ---------------------------------------------------------------- */

static void *lookup_iterate_node_from_step
  ( const lookup_t *lookup
  , const bnode_t  *node
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, const bnode_t *node, int *out_iteration_break)
  , void *context

  , void *initial_accumulation

  , int *iteration_break
  )
{
  void *value;

  int   iteration_break_working;


  int i;
  size_t children[2];

  void *accumulation = initial_accumulation;

#ifdef ERROR_CHECKING
  if (!lookup)
    return  NULL;
  if (!with_value)
    return  NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return initial_accumulation;

  if (!node)
    node = &lookup->order[0];

  value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));

  if (!iteration_break)
  {
    iteration_break = &iteration_break_working;
    *iteration_break = 0;
  }

  if (*iteration_break)
    return initial_accumulation;

  reverse_direction = IS_TRUE(reverse_direction);

  children[ reverse_direction] = node->left;
  children[!reverse_direction] = node->right;

  for (i = 0; i < ARRAY_NUM(children) && !*iteration_break; ++i)
  {
    size_t child = children[i];

    if (!BNODE_IS_LEAF(child))
      accumulation =
        lookup_iterate_node_from_step
          ( lookup
          , LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(child))
          , reverse_direction

          , with_value
          , context

          , accumulation

          , iteration_break
          );

    if (i == 0 && !*iteration_break)
      accumulation = with_value(context, accumulation, value, node, iteration_break);
  }

  return accumulation;
}

void *lookup_iterate_node_from
  ( const lookup_t *lookup
  , const bnode_t  *node
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, const bnode_t *node, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  )
{
  return
    lookup_iterate_node_from_step
      ( lookup
      , node
      , reverse_direction

      , with_value
      , context

      , initial_accumulation

      , NULL
      );
}

void *lookup_iterate_node
  ( const lookup_t *lookup
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, const bnode_t *node, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  )
{
  return
    lookup_iterate_node_from
      ( lookup
      , NULL
      , reverse_direction

      , with_value
      , context

      , initial_accumulation
      );
}

static void *lookup_iterate_from_step
  ( const lookup_t *lookup
  , const bnode_t  *node
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_iteration_break)
  , void *context

  , void *initial_accumulation

  , int *iteration_break
  )
{
  void *value;

  int   iteration_break_working;


  int i;
  size_t children[2];

  void *accumulation = initial_accumulation;

#ifdef ERROR_CHECKING
  if (!lookup)
    return  NULL;
  if (!with_value)
    return  NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return initial_accumulation;

  if (!node)
    node = &lookup->order[0];

  value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));

  if (!iteration_break)
  {
    iteration_break = &iteration_break_working;
    *iteration_break = 0;
  }

  if (*iteration_break)
    return initial_accumulation;

  reverse_direction = IS_TRUE(reverse_direction);

  children[ reverse_direction] = node->left;
  children[!reverse_direction] = node->right;

  for (i = 0; i < ARRAY_NUM(children) && !*iteration_break; ++i)
  {
    size_t child = children[i];

    if (!BNODE_IS_LEAF(child))
      accumulation =
        lookup_iterate_from_step
          ( lookup
          , LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(child))
          , reverse_direction

          , with_value
          , context

          , accumulation

          , iteration_break
          );

    if (i == 0 && !*iteration_break)
      accumulation = with_value(context, accumulation, value, iteration_break);
  }

  return accumulation;
}

void *lookup_iterate_from
  ( const lookup_t *lookup
  , const bnode_t  *node
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  )
{
  return
    lookup_iterate_from_step
      ( lookup
      , node
      , reverse_direction

      , with_value
      , context

      , initial_accumulation

      , NULL
      );
}

void *lookup_iterate
  ( const lookup_t *lookup
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  )
{
  return
    lookup_iterate_from
      ( lookup
      , NULL
      , reverse_direction

      , with_value
      , context

      , initial_accumulation
      );
}

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
#include "type_base_memory_manager.h"
#include "type_base_memory_tracker.h"
#ifdef TODO
#error "TODO: #include primitive c data types"
#else  /* #ifdef TODO */
#include "type_base.h"
#endif /* #ifdef TODO */
#include "type_base_type.h"

#include "bits.h"
#include "ptrs.h"

#include "cpp.h"

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
static tval                *lookup_type_dup        ( const type_t *self
                                                   , tval *dest
                                                   , const tval *src
                                                   , int defaults_src_unused
                                                   , int rec_copy
                                                   , int dup_metadata
                                                   , ref_traversal_t *ref_traversal
                                                   );

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

  , /* dup                    */ lookup_type_dup

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

static tval                *lookup_type_dup        ( const type_t *self
                                                   , tval *dest
                                                   , const tval *src
                                                   , int defaults_src_unused
                                                   , int rec_copy
                                                   , int dup_metadata
                                                   , ref_traversal_t *ref_traversal
                                                   )
{
  const lookup_t *from;
  lookup_t       *copy;

  copy =
    default_type_dup
      ( self
      , dest
      , src
      , defaults_src_unused
      , rec_copy
      , dup_metadata
      , ref_traversal
      );

  if (!copy)
    return copy;

  from = src;

  if (!from)
    return copy;

  if (rec_copy)
  {
    if (  copy->values == from->values
       && copy->order  == from->order
       )
    {
      const memory_manager_t *manager = type_default_memory_manager(self, copy);

      copy->values = memory_manager_mcalloc(manager, LOOKUP_CAPACITY(from), LOOKUP_VALUE_SIZE(from));
      if (!copy->values)
      {
        lookup_init_empty(copy, LOOKUP_VALUE_SIZE(from));
        return NULL;
      }
      copy->order  = memory_manager_mcalloc(manager, LOOKUP_CAPACITY(from), sizeof(*from->order));
      if (!copy->order)
      {
        lookup_init_empty(copy, LOOKUP_VALUE_SIZE(from));
        return NULL;
      }

      memmove(copy->values, from->values, LOOKUP_VALUE_SIZE(from) * LOOKUP_CAPACITY(from));
      memmove(copy->order,  from->order,  sizeof(*from->order)    * LOOKUP_CAPACITY(from));
    }
  }

  return copy;
}

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

bnode_t *bnode_copy(bnode_t *dest, const bnode_t *src)
{
#if ERROR_CHECKING
  if (!src)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (!dest)
    return NULL;

  dest->value = src->value;
  dest->left  = src->left;
  dest->right = src->right;

  return dest;
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

size_t bnode_colored_value(size_t index, size_t color_bit)
{
  return BNODE_COLORED_VALUE(index, color_bit);
}

size_t bnode_value(size_t index)
{
  return BNODE_VALUE(index);
}

size_t bnode_color(size_t color_bit)
{
  return BNODE_COLOR(color_bit);
}

size_t bnode_set_color(bnode_t *node, size_t color_bit)
{
  return (BNODE_SET_COLOR(node, color_bit));
}

size_t bnode_set_black(bnode_t *node)
{
  return (BNODE_SET_BLACK(node));
}

size_t bnode_set_red  (bnode_t *node)
{
  return (BNODE_SET_RED(node));
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

  , const memory_manager_t *memory_manager
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
#endif /* #if ERROR_CHECKING  */

  memory_manager_mfree(memory_manager, lookup->values);
  memory_manager_mfree(memory_manager, lookup->order);
  lookup->values   = NULL;
  lookup->order    = NULL;

  lookup->capacity = 0;

  lookup->len      = 0;
}

size_t lookup_copy_value_buffer(void *dest, size_t dest_size, const lookup_t *lookup, size_t start, size_t num_values)
{
  size_t capacity;
  size_t value_size;

#if ERROR_CHECKING
  if (!lookup)
    return 0;
  if (!dest)
    return 0;
#endif /* #if ERROR_CHECKING  */

  if (dest_size <= 0)
    return 0;

  if (LOOKUP_NULL(lookup))
    return 0;

  capacity   = LOOKUP_CAPACITY  (lookup);
  value_size = LOOKUP_VALUE_SIZE(lookup);

  start      = min_size(capacity,                         start);
  num_values = min_size(size_minus(capacity, start),      num_values);
  num_values = min_size(DIV_FLOOR(dest_size, value_size), num_values);

  memmove(dest, LOOKUP_INDEX_CVALUE(lookup, start), value_size * num_values);

  return num_values;
}

size_t lookup_copy_order_buffer(bnode_t *dest, size_t dest_num, const lookup_t *lookup, size_t start, size_t num_values)
{
  size_t capacity;

#if ERROR_CHECKING
  if (!lookup)
    return 0;
  if (!dest)
    return 0;
#endif /* #if ERROR_CHECKING  */

  if (dest_num <= 0)
    return 0;

  if (LOOKUP_NULL(lookup))
    return 0;

  capacity   = LOOKUP_CAPACITY  (lookup);

  start      = min_size(capacity,                    start);
  num_values = min_size(size_minus(capacity, start), num_values);
  num_values = min_size(dest_num,                    num_values);

  memmove(dest, LOOKUP_INDEX_CVALUE(lookup, start), sizeof(*lookup->order) * num_values);

  return num_values;
}

lookup_t *lookup_copy
  (       lookup_t *dest
  , const lookup_t *src

  , const memory_manager_t *memory_manager
  )
{
  int dest_dynamic;

  size_t capacity;
  size_t value_size;

#if ERROR_CHECKING
  if (!src)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  /* ---------------------------------------------------------------- */

  capacity   = LOOKUP_CAPACITY  (src);
  value_size = LOOKUP_VALUE_SIZE(src);

  /* ---------------------------------------------------------------- */
  /* Dynamically allocated a copy?                                    */

  if (dest)
  {
    dest_dynamic = 0;
  }
  else
  {
    dest_dynamic = 1;

    if (!(dest = memory_manager_mmalloc(memory_manager, sizeof(*dest))))
      return NULL;
  }

  /* ---------------------------------------------------------------- */
  /* Handle value and order buffer allocation first.                  */

  if (LOOKUP_NULL(src))
  {
    dest->values = NULL;
    dest->order  = NULL;
  }
  else
  {
    if (!(dest->values = memory_manager_mcalloc(memory_manager, capacity, value_size)))
    {
      if (dest_dynamic)
        memory_manager_mfree(memory_manager, dest);

      return NULL;
    }

    if (!(dest->order = memory_manager_mcalloc(memory_manager, capacity, sizeof(*dest->order))))
    {
      memory_manager_mfree(memory_manager, dest->values);
      dest->values = NULL;

      if (dest_dynamic)
        memory_manager_mfree(memory_manager, dest);

      return NULL;
    }

    if (!lookup_copy_value_buffer(dest->values, capacity * value_size, src, 0, capacity))
    {
      memory_manager_mfree(memory_manager, dest->values);
      dest->values = NULL;

      memory_manager_mfree(memory_manager, dest->order);
      dest->order  = NULL;

      if (dest_dynamic)
        memory_manager_mfree(memory_manager, dest);

      return NULL;
    }
  }

  /* ---------------------------------------------------------------- */
  /* Copy everything else.                                            */

  dest->type       = src->type;

  dest->capacity   = capacity;

  dest->value_size = value_size;
  dest->next_value = src->next_value;

  dest->next_order = src->next_order;

  dest->len        = src->len;

  /* ---------------------------------------------------------------- */
  /* Return "dest".                                                   */

  return dest;
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

  , const memory_manager_t *memory_manager
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
    if (lookup->values)
      return NULL;
    if (lookup->order)
      return NULL;
#endif /* #if ERROR_CHECKING  */

    lookup->values = memory_manager_mcalloc(memory_manager, capacity, lookup->value_size);
    if (!lookup->values)
    {
      lookup->order    = NULL;
      lookup->capacity = 0;

      return NULL;
    }

    lookup->order  = memory_manager_mcalloc(memory_manager, capacity, sizeof(bnode_t));
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
    if (!lookup->values)
      return NULL;
    if (!lookup->order)
      return NULL;
#endif /* #if ERROR_CHECKING  */

    lookup->values = memory_manager_mrealloc(memory_manager, lookup->values, capacity * lookup->value_size);
    if (!lookup->values)
    {
      lookup->order    = NULL;
      lookup->capacity = 0;

      return NULL;
    }

    lookup->order  = memory_manager_mrealloc(memory_manager, lookup->order,  capacity * sizeof(bnode_t));
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

  , const memory_manager_t *memory_manager

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

    lookup_defragment_simple(lookup, auto_defragment);

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

      , memory_manager
      );
}

/* "lookup_auto_expand" except without the output parameters. */
lookup_t *lookup_auto_expand_simple
  ( lookup_t *lookup
  , int       auto_defragment

  , const memory_manager_t *memory_manager
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
    lookup_defragment_simple(lookup, DEFRAGMENT_DEFAULT);

    /* Are we not using more than half the capacity? */
    if ((LOOKUP_LEN(lookup)) <= ((LOOKUP_CAPACITY(lookup)) >> 1))
      return lookup;
  }

  return
    lookup_expand
      ( lookup
      , LOOKUP_CAPACITY(lookup) << 1

      , memory_manager
      );
}

static lookup_t *lookup_defragment_step
  ( lookup_t *lookup
  , bnode_t  *node
  , int       defragment_which

  , void *(*on_new_node_index) (void *on_new_order_index_context, void *last_accumulation, lookup_t *lookup, size_t new_node_index,  size_t old_node_index,  int *out_break_iteration)
    , void *on_new_node_index_context
    , void *on_new_node_index_initial_accumulation

  , void *(*on_new_value_index)(void *on_new_value_index_context, void *last_accumulation, lookup_t *lookup, size_t new_value_index, size_t old_value_index, int *out_break_iteration)
    , void *on_new_value_index_context
    , void *on_new_value_index_initial_accumulation

  , void **out_on_new_node_index_final_accumulation
  , void **out_on_new_value_index_final_accumulation
  )
{
  if (LOOKUP_EMPTY(lookup))
    return lookup;

  LOOKUP_OPTIONAL_NODE(lookup, node);

  if (defragment_which & (DEFRAGMENT_ORDER))
  {
    if (LOOKUP_IS_ORDER_RECYCLING(lookup))
    {
      /* TODO */
    }
  }
  if (defragment_which & (DEFRAGMENT_VALUES))
  {
    if (LOOKUP_IS_VALUE_RECYCLING(lookup))
    {
      /* TODO */
    }
  }

  WRITE_OUTPUT(out_on_new_node_index_final_accumulation,  on_new_node_index_initial_accumulation);
  WRITE_OUTPUT(out_on_new_value_index_final_accumulation, on_new_value_index_initial_accumulation);

  return lookup;
}

/* Move elements to remove gaps of free element slots. */
lookup_t *lookup_defragment
  ( lookup_t *lookup
  , int       defragment_which

  , void *(*on_new_node_index) (void *on_new_order_index_context, void *last_accumulation, lookup_t *lookup, size_t new_node_index,  size_t old_node_index,  int *out_break_iteration)
    , void *on_new_node_index_context
    , void *on_new_node_index_initial_accumulation

  , void *(*on_new_value_index)(void *on_new_value_index_context, void *last_accumulation, lookup_t *lookup, size_t new_value_index, size_t old_value_index, int *out_break_iteration)
    , void *on_new_value_index_context
    , void *on_new_value_index_initial_accumulation

  , void **out_on_new_node_index_final_accumulation
  , void **out_on_new_value_index_final_accumulation
  )
{
  return
    lookup_defragment_step
      ( lookup
      , LOOKUP_ROOT_NODE(lookup)
      , defragment_which

      , on_new_node_index
        , on_new_node_index_context
        , on_new_node_index_initial_accumulation

      , on_new_value_index
        , on_new_value_index_context
        , on_new_value_index_initial_accumulation

      , out_on_new_node_index_final_accumulation
      , out_on_new_value_index_final_accumulation
      );
}

lookup_t *lookup_defragment_simple
  ( lookup_t *lookup
  , int       defragment_which
  )
{
  return
    lookup_defragment
      ( lookup
      , defragment_which

      , NULL
        , NULL
        , NULL

      , NULL
        , NULL
        , NULL

      , NULL
      , NULL
      );
}

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

  , const memory_manager_t *memory_manager
  )
{
  size_t old_capacity;
  size_t new_capacity;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
    return lookup;

  new_capacity = capacity;
  new_capacity = max_size(lookup->next_value, new_capacity);
  new_capacity = max_size(lookup->next_order, new_capacity);
  new_capacity = max_size(LOOKUP_LEN(lookup), new_capacity);

  old_capacity = LOOKUP_CAPACITY(lookup);
  if (new_capacity >= old_capacity)
    return lookup;

#if ERROR_CHECKING
  if (!lookup->values)
    return NULL;
  if (!lookup->order)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (new_capacity <= 0)
  {
    memory_manager_mfree(memory_manager, lookup->values);
    memory_manager_mfree(memory_manager, lookup->order);

    lookup->values = NULL;
    lookup->order  = NULL;

    lookup->capacity = 0;
    lookup->len      = 0;

    return lookup;
  }
  else
  {
#if ERROR_CHECKING
    if (lookup->values)
      return NULL;
    if (lookup->order)
      return NULL;
#endif /* #if ERROR_CHECKING  */

    lookup->values = memory_manager_mrealloc(memory_manager, lookup->values, LOOKUP_VALUE_SIZE(lookup) * new_capacity);
    if (lookup->values)
      lookup->order = memory_manager_mrealloc(memory_manager, lookup->order, sizeof(*lookup->order) * new_capacity);

    if (!lookup->values || !lookup->order)
    {
      if (lookup->values)
        memory_manager_mfree(memory_manager, lookup->values);
      if (lookup->order)
        memory_manager_mfree(memory_manager, lookup->order);

      lookup->values = NULL;
      lookup->order  = NULL;

      lookup->capacity = 0;
      lookup->len      = 0;

      return NULL;
    }

    lookup->capacity = new_capacity;
    lookup->len      = min_size(new_capacity, lookup->len);

    return lookup;
  }
}

/* Reallocate more or less memory for free element slots. */
/*                                                        */
/* When shrinking, defragmentation is performed when      */
/* recycling.                                             */
/*                                                        */
/* When shrinking, the new number is bounded by           */
/* the number of elements in use, so no data loss for     */
/* existing elements will occur.                          */
lookup_t *lookup_resize
  ( lookup_t *lookup
  , size_t    capacity

  , const memory_manager_t *memory_manager
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
    if (LOOKUP_IS_RECYCLING(lookup))
    {
      lookup = lookup_defragment_simple(lookup, DEFRAGMENT_DEFAULT);
      if (!lookup)
        return NULL;
    }

    return lookup_shrink(lookup, capacity, memory_manager);
  }
  else if (capacity > old_capacity)
  {
    return lookup_expand(lookup, capacity, memory_manager);
  }
  else
  {
    return lookup;
  }
}

lookup_t *lookup_auto_resize
  ( lookup_t *lookup

  , const memory_manager_t *memory_manager
  )
{
  return
    lookup_auto_resize_controlled
      ( /* lookup                     */ lookup

      , /* memory_manager             */ memory_manager

      , /* min_capacity               */ lookup_auto_min_capacity
        , /* min_capacity_context     */ lookup_auto_min_capacity_context
      , /* expand_threshold           */ lookup_auto_expand_threshold
        , /* expand_threshold_context */ lookup_auto_expand_threshold_context
      , /* expand_capacity            */ lookup_auto_expand_capacity
        , /* expand_capacity_context  */ lookup_auto_expand_capacity_context
      , /* shrink_threshold           */ lookup_auto_shrink_threshold
        , /* shrink_threshold_context */ lookup_auto_shrink_threshold_context
      , /* shrink_capacity            */ lookup_auto_shrink_capacity
        , /* shrink_capacity_context  */ lookup_auto_shrink_capacity_context
      , /* defragment_which           */ lookup_auto_defragment_which
        , /* defragment_which_context */ lookup_auto_defragment_which_context

      , /* on_new_node_index          */ NULL
      , /* on_new_node_index_context  */ NULL
      , /* on_new_node_index_initial_accumulation    */ NULL

      , /* on_new_value_index         */ NULL
      , /* on_new_value_index_context */ NULL
      , /* on_new_value_index_initial_accumulation   */ NULL

      , /* out_expanding              */ NULL
      , /* out_shrinking              */ NULL
      , /* out_defragmenting          */ NULL
      , /* out_new_capacity           */ NULL

      , /* out_on_new_node_index_final_accumulation  */ NULL
      , /* out_on_new_value_index_final_accumulation */ NULL
      );
}

size_t lookup_auto_min_capacity    (void *context)
  { return LOOKUP_AUTO_MIN_CAPACITY; }

size_t lookup_auto_expand_threshold(void *context, size_t capacity)
  { return LOOKUP_AUTO_EXPAND_THRESHOLD(capacity); }

size_t lookup_auto_expand_capacity (void *context, size_t capacity)
  { return LOOKUP_AUTO_SHRINK_CAPACITY (capacity); }

size_t lookup_auto_shrink_threshold(void *context, size_t capacity)
  { return LOOKUP_AUTO_EXPAND_THRESHOLD(capacity); }

size_t lookup_auto_shrink_capacity (void *context, size_t capacity)
  { return LOOKUP_AUTO_SHRINK_CAPACITY (capacity); }

int lookup_auto_defragment_which   (void *context)
  { return LOOKUP_AUTO_DEFRAGMENT_WHICH; }

void * const lookup_auto_min_capacity_context     = NULL;
void * const lookup_auto_expand_threshold_context = NULL;
void * const lookup_auto_expand_capacity_context  = NULL;
void * const lookup_auto_shrink_threshold_context = NULL;
void * const lookup_auto_shrink_capacity_context  = NULL;
void * const lookup_auto_defragment_which_context = NULL;

lookup_t *lookup_auto_resize_controlled
  ( lookup_t *lookup

  , const memory_manager_t *memory_manager

  , size_t (*min_capacity)(void *context)
    , void                  *min_capacity_context
  , lookup_capacity_fun_t  expand_threshold
    , void                  *expand_threshold_context
  , lookup_capacity_fun_t  expand_capacity
    , void                  *expand_capacity_context
  , lookup_capacity_fun_t  shrink_threshold
    , void                  *shrink_threshold_context
  , lookup_capacity_fun_t  shrink_capacity
    , void                  *shrink_capacity_context
  , int    (*defragment_which)(void *context)
    , void                  *defragment_which_context

  , void *(*on_new_node_index) (void *on_new_order_index_context, void *last_accumulation, lookup_t *lookup, size_t new_node_index,  size_t old_node_index,  int *out_break_iteration)
    , void *on_new_node_index_context
    , void *on_new_node_index_initial_accumulation

  , void *(*on_new_value_index)(void *on_new_value_index_context, void *last_accumulation, lookup_t *lookup, size_t new_value_index, size_t old_value_index, int *out_break_iteration)
    , void *on_new_value_index_context
    , void *on_new_value_index_initial_accumulation

  , int    *out_expanding
  , int    *out_shrinking
  , int    *out_defragmenting
  , size_t *out_new_capacity

  , void  **out_on_new_node_index_final_accumulation
  , void  **out_on_new_value_index_final_accumulation
  )
{
  size_t len;
  size_t capacity;

  int    expanding;
  int    shrinking;
  int    defragmenting;
  size_t new_capacity;

  size_t min_capacity_val;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  /* ---------------------------------------------------------------- */

  if (!min_capacity)
  {
    min_capacity         = lookup_auto_min_capacity;
    min_capacity_context = lookup_auto_min_capacity_context;
  }

  if (!expand_threshold)
  {
    expand_threshold         = lookup_auto_expand_threshold;
    expand_threshold_context = lookup_auto_expand_threshold_context;
  }

  if (!expand_capacity)
  {
    expand_capacity         = lookup_auto_expand_capacity;
    expand_capacity_context = lookup_auto_expand_capacity_context;
  }

  if (!shrink_threshold)
  {
    shrink_threshold         = lookup_auto_shrink_threshold;
    shrink_threshold_context = lookup_auto_shrink_threshold_context;
  }

  if (!shrink_capacity)
  {
    shrink_capacity         = lookup_auto_shrink_capacity;
    shrink_capacity_context = lookup_auto_shrink_capacity_context;
  }

  if (!defragment_which)
  {
    defragment_which         = lookup_auto_defragment_which;
    defragment_which_context = lookup_auto_defragment_which_context;
  }

  /* ---------------------------------------------------------------- */

  len       = LOOKUP_LEN     (lookup);
  capacity  = LOOKUP_CAPACITY(lookup);

  expanding     = 0;
  shrinking     = 0;
  defragmenting = 0;
  new_capacity  = capacity;

  /* ---------------------------------------------------------------- */

  /* 1) Ensure minimum capacity. */

  min_capacity_val = min_capacity(min_capacity_context);

  /* Should we expand to minimum capacity? */
  if (capacity < min_capacity_val)
  {
    expanding     = 1;
    shrinking     = 0;
    defragmenting = 0;
    new_capacity  = min_capacity_val;
  }

  /* Assume "len" is at least minimum capacity. */
  if (capacity <= min_capacity_val)
    len = min_capacity_val;

  /* 2) Maximum capacity => expand. */

  /* Are we at max capacity? */
  if (len >= capacity)
  {
    expanding     = 1;
    shrinking     = 0;
    defragmenting = 0;
    new_capacity  = expand_capacity(expand_capacity_context, new_capacity);
  }
  else
  {
    /* 3) Expanding and shrinking thresholds. */

    /* If we're recycling,
     *   1) check expand_threshold, and
     *   2) defragment if we're shrinking.
     */
    if (LOOKUP_IS_RECYCLING(lookup))
    {
      if      (len >= expand_threshold(expand_threshold_context, new_capacity))
      {
        expanding     = 1;
        shrinking     = 0;
        defragmenting = 0;
        new_capacity  = expand_capacity(expand_capacity_context, new_capacity);
      }
      else if (len <= shrink_threshold(shrink_threshold_context, new_capacity))
      {
        expanding     = 0;
        shrinking     = 1;
        defragmenting = 1;
        new_capacity  = shrink_capacity(shrink_capacity_context, new_capacity);
      }
    }
    else
    {
      /* Is there enough free space to warrant shrinking? */
      if (len <= shrink_threshold(shrink_threshold_context, new_capacity))
      {
        expanding     = 0;
        shrinking     = 1;
        defragmenting = 0;
        new_capacity  = shrink_capacity(shrink_capacity_context, new_capacity);
      }
    }
  }

  /* ---------------------------------------------------------------- */

  if (expanding)
  {
    expanding = max_int(1, new_capacity - capacity);
    shrinking = 0;
  }

  if (shrinking)
  {
    shrinking = max_int(1, capacity     - new_capacity);
    expanding = 0;
  }

  WRITE_OUTPUT(out_expanding,     expanding);
  WRITE_OUTPUT(out_shrinking,     shrinking);
  WRITE_OUTPUT(out_defragmenting, defragmenting);
  WRITE_OUTPUT(out_new_capacity,  new_capacity);

  /* ---------------------------------------------------------------- */

  if (expanding > 0 && shrinking <= 0)
  {
    if (lookup)
      lookup = lookup_expand(lookup, new_capacity, memory_manager);
  }

  if (defragmenting)
  {
    if (lookup)
      lookup = lookup_defragment
        ( lookup
        , defragment_which(defragment_which_context)

        , on_new_node_index
        , on_new_node_index_context
        , on_new_node_index_initial_accumulation

        , on_new_value_index
        , on_new_value_index_context
        , on_new_value_index_initial_accumulation

        , out_on_new_node_index_final_accumulation
        , out_on_new_value_index_final_accumulation
        );
  }

  if (shrinking > 0 && shrinking <= 0)
  {
    if (lookup)
      lookup = lookup_shrink(lookup, new_capacity, memory_manager);
  }

  return lookup;
}

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

void    *lookup_index_value(lookup_t *lookup, size_t        index)
{
  return LOOKUP_INDEX_VALUE(lookup, index);
}

bnode_t *lookup_index_order(lookup_t *lookup, size_t        index)
{
  return LOOKUP_INDEX_ORDER(lookup, index);
}

void    *lookup_node_value (lookup_t *lookup, const bnode_t *node)
{
  return LOOKUP_NODE_VALUE(lookup, node);
}


const void    *lookup_index_cvalue(const lookup_t *lookup, size_t        index)
{
  return LOOKUP_INDEX_CVALUE(lookup, index);
}

const bnode_t *lookup_index_corder(const lookup_t *lookup, size_t        index)
{
  return LOOKUP_INDEX_CORDER(lookup, index);
}

const void    *lookup_node_cvalue (const lookup_t *lookup, const bnode_t *node)
{
  return LOOKUP_NODE_CVALUE(lookup, node);
}


bnode_t *lookup_root_node(lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
    return NULL;

  return LOOKUP_ROOT_NODE(lookup);
}

const bnode_t *lookup_root_cnode(const lookup_t *lookup)
{
#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
    return NULL;

  return LOOKUP_ROOT_CNODE(lookup);
}


size_t lookup_get_value_index(const lookup_t *lookup, const void *value_ref)
{
#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
    return 0;

  if (!value_ref)
    return 0;

  return min_size(LOOKUP_CAPACITY(lookup), LOOKUP_GET_VALUE_INDEX(lookup, value_ref));
}

size_t lookup_get_node_index(const lookup_t *lookup, const bnode_t *node)
{
#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
    return 0;

  if (!node)
    return 0;

  return min_size(LOOKUP_CAPACITY(lookup), LOOKUP_GET_NODE_INDEX(lookup, node));
}


bnode_t *lookup_node_or_root(lookup_t *lookup, bnode_t *node, bnode_t **out_node)
{
#if ERROR_CHECKING
  if (!lookup)
  {
    WRITE_OUTPUT(out_node, NULL);
    return NULL;
  }
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
  {
    WRITE_OUTPUT(out_node, NULL);
    return NULL;
  }

  LOOKUP_NODE_OR_ROOT(lookup, node);

  WRITE_OUTPUT(out_node, node);
  return node;
}

const bnode_t *lookup_node_or_croot(const lookup_t *lookup, const bnode_t *node, const bnode_t **out_node)
{
#if ERROR_CHECKING
  if (!lookup)
  {
    WRITE_OUTPUT(out_node, NULL);
    return NULL;
  }
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
  {
    WRITE_OUTPUT(out_node, NULL);
    return NULL;
  }

  LOOKUP_NODE_OR_CROOT(lookup, node);

  WRITE_OUTPUT(out_node, node);
  return node;
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

  current_value = lookup->next_value;

  for (                      ; lookup->next_value < lookup->capacity; ++lookup->next_value)
    if (LOOKUP_IS_VALUE_FREE(lookup, lookup->next_value))
      return lookup->next_value++;

  for (lookup->next_value = 0; lookup->next_value < current_value   ; ++lookup->next_value)
    if (LOOKUP_IS_VALUE_FREE(lookup, lookup->next_value))
      return lookup->next_value++;

  return 0;
}

size_t lookup_next_order(lookup_t *lookup)
{
  size_t current_order;

#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  current_order = lookup->next_order;

  for (                      ; lookup->next_order < lookup->capacity; ++lookup->next_order)
    if (LOOKUP_IS_ORDER_FREE(lookup, lookup->next_order))
      return lookup->next_order++;

  for (lookup->next_order = 0; lookup->next_order < current_order   ; ++lookup->next_order)
    if (LOOKUP_IS_ORDER_FREE(lookup, lookup->next_order))
      return lookup->next_order++;

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

  LOOKUP_OPTIONAL_CNODE(lookup, node);

  if      ( BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
    return 0;
  else if (!BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
    return lookup_height_from(lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->left ))) + 1;
  else if ( BNODE_IS_LEAF(node->left) && !BNODE_IS_LEAF(node->right))
    return lookup_height_from(lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right))) + 1;
  else
    return
      max_int
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
 * Traverse the binary search tree, stopping at the given value or at a leaf.
 *
 * "node" when non-NULL determines the relative root:
 *
 * If the "node" parameter is NULL, traversal starts at lookup's root node;
 * otherwise traversal starts at the "node" parameter.
 *
 * If the value was found:
 * | parent
 * |   => Parent of the value node, or NULL if the value node is the relative
 * |      root (the "node" parameter).
 * |
 * | parent_link
 * |   => "left" or "right" reference in "parent" to the value node.
 * |
 * | node
 * |   => The value node, i.e. the node containing a value equal to "val".
 * |
 * | node_link
 * |   => &node->left
 * |
 * | node_val
 * |   => The value associated with the value node.
 * |
 * | parent_ordering
 * |   => The comparison of "val" with the parent node's value,
 * |      or 0 if "parent" is NULL.
 * |
 * | ordering
 * |   => The comparison of "val" with the value node's value; i.e. 0
 * |      (ordering_rel_eq).
 *
 * If the value was not found, then the output is the same, except the "value
 * node" is the parent of the leaf, and "node_link" is the "left" or "right"
 * reference to the leaf:
 * | parent
 * |   => Grandparent of the leaf, or NULL if the value node is the relative
 * |      root (the "node" parameter).
 * |
 * | parent_link
 * |   => "left" or "right" reference in "parent" to the parent of the leaf.
 * |
 * | node
 * |   => Parent of the leaf.
 * |
 * | node_link
 * |   => "left" or "right" reference in the parent to the leaf.
 * |
 * | node_val
 * |   => The value associated with the parent of the leaf.
 * |
 * | parent_ordering
 * |   => The comparison of "val"  with the leaf's grandparent's value,
 * |      or 0 if "parent" is NULL.
 * |
 * | ordering
 * |   => The comparison of "val" with the leaf's parent's value.
 *
 * Thus "node" is either the value node when found, else parent of the leaf.
 *
 * To check whether a leaf was reached, check whether "ordering" is not 0.
 * (Alternatively, check whether "node_link" is non-NULL.)
 *
 * ----------------------------------------------------------------
 *
 * If the tree is empty, outputs all NULL's and 0's.
 */
lookup_t *lookup_find_from
  ( lookup_t           *lookup
  , bnode_t            *root
  , const void         *val

  , callback_compare_t  cmp

  , bnode_t    **out_grandparent
  , size_t     **out_grandparent_link
  , bnode_t    **out_parent
  , size_t     **out_parent_link
  , bnode_t    **out_node
  , size_t     **out_node_link

  , const void **out_node_val

  , int         *out_grandparent_ordering
  , int         *out_parent_ordering
  , int         *out_ordering
  )
{
  LOOKUP_FIND_VARIABLE_DECLARATIONS;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
  if (!val)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
  {
    WRITE_OUTPUT(out_grandparent,          NULL);
    WRITE_OUTPUT(out_grandparent_link,     NULL);
    WRITE_OUTPUT(out_parent,               NULL);
    WRITE_OUTPUT(out_parent_link,          NULL);
    WRITE_OUTPUT(out_node,                 NULL);
    WRITE_OUTPUT(out_node_link,            NULL);

    WRITE_OUTPUT(out_node_val,             NULL);

    WRITE_OUTPUT(out_grandparent_ordering, 0);
    WRITE_OUTPUT(out_parent_ordering,      0);
    WRITE_OUTPUT(out_ordering,             0);

    return lookup;
  }

  if (LOOKUP_EMPTY(lookup))
  {
    WRITE_OUTPUT(out_grandparent,          NULL);
    WRITE_OUTPUT(out_grandparent_link,     NULL);
    WRITE_OUTPUT(out_parent,               NULL);
    WRITE_OUTPUT(out_parent_link,          NULL);
    WRITE_OUTPUT(out_node,                 (           LOOKUP_ROOT_NODE(lookup))        );
    WRITE_OUTPUT(out_node_link,            ( ARROW_REF(LOOKUP_ROOT_NODE(lookup), left)) );

    WRITE_OUTPUT(out_node_val,             NULL);

    WRITE_OUTPUT(out_grandparent_ordering, 0);
    WRITE_OUTPUT(out_parent_ordering,      0);
    WRITE_OUTPUT(out_ordering,             0);

    return lookup;
  }

  LOOKUP_OPTIONAL_NODE(lookup, root);

  grandparent          = NULL;
  grandparent_link     = NULL;
  parent               = NULL;
  parent_link          = NULL;
  node_link            = NULL;
  grandparent_ordering = 0;
  parent_ordering      = 0;
  ordering             = 0;
  node                 = root;
  for (;;)
  {
    grandparent_ordering = parent_ordering;
    parent_ordering      = ordering;

    /* val <?= node value */
    node_val = LOOKUP_NODE_CVALUE(lookup, node);
    ordering = call_callback_compare(cmp, val, node_val);

#if ERROR_CHECKING
    if (IS_ORDERING_ERROR(ordering))
    {
      WRITE_OUTPUT(out_grandparent,          NULL);
      WRITE_OUTPUT(out_grandparent_link,     NULL);
      WRITE_OUTPUT(out_parent,               NULL);
      WRITE_OUTPUT(out_parent_link,          NULL);
      WRITE_OUTPUT(out_node,                 NULL);
      WRITE_OUTPUT(out_node_link,            NULL);

      WRITE_OUTPUT(out_node_val,             NULL);

      WRITE_OUTPUT(out_grandparent_ordering, grandparent_ordering);
      WRITE_OUTPUT(out_parent_ordering,      parent_ordering);
      WRITE_OUTPUT(out_ordering,             ordering);

      return NULL;
    }
#endif /* #if ERROR_CHECKING  */

    if      (ordering < 0)
      node_link = &node->left;
    else if (ordering > 0)
      node_link = &node->right;
    else
      break;

    if (BNODE_IS_LEAF(*node_link))
      break;

    grandparent      = parent;
    grandparent_link = parent_link;

    parent      = node;
    parent_link = node_link;

    node = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(*node_link));
  }

  if (ordering == 0)
    node_link = &node->left;

  WRITE_OUTPUT(out_grandparent,          grandparent);
  WRITE_OUTPUT(out_grandparent_link,     grandparent_link);
  WRITE_OUTPUT(out_parent,               parent);
  WRITE_OUTPUT(out_parent_link,          parent_link);
  WRITE_OUTPUT(out_node,                 node);
  WRITE_OUTPUT(out_node_link,            node_link);

  WRITE_OUTPUT(out_node_val,             node_val);

  WRITE_OUTPUT(out_grandparent_ordering, grandparent_ordering);
  WRITE_OUTPUT(out_parent_ordering,      parent_ordering);
  WRITE_OUTPUT(out_ordering,             ordering);

  return lookup;
}

const lookup_t *lookup_cfind_from
  ( const lookup_t      *lookup
  , const bnode_t       *root
  , const void          *val

  , callback_compare_t   cmp

  , const bnode_t **out_grandparent
  , const size_t  **out_grandparent_link
  , const bnode_t **out_parent
  , const size_t  **out_parent_link
  , const bnode_t **out_node
  , const size_t  **out_node_link

  , const void    **out_node_val

  , int           *out_grandparent_ordering
  , int           *out_parent_ordering
  , int           *out_ordering
  )
{
  LOOKUP_CFIND_VARIABLE_DECLARATIONS;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
  if (!val)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_NULL(lookup))
  {
    WRITE_OUTPUT(out_grandparent,          NULL);
    WRITE_OUTPUT(out_grandparent_link,     NULL);
    WRITE_OUTPUT(out_parent,               NULL);
    WRITE_OUTPUT(out_parent_link,          NULL);
    WRITE_OUTPUT(out_node,                 NULL);
    WRITE_OUTPUT(out_node_link,            NULL);

    WRITE_OUTPUT(out_node_val,             NULL);

    WRITE_OUTPUT(out_grandparent_ordering, 0);
    WRITE_OUTPUT(out_parent_ordering,      0);
    WRITE_OUTPUT(out_ordering,             0);

    return lookup;
  }

  if (LOOKUP_EMPTY(lookup))
  {
    WRITE_OUTPUT(out_grandparent,          NULL);
    WRITE_OUTPUT(out_grandparent_link,     NULL);
    WRITE_OUTPUT(out_parent,               NULL);
    WRITE_OUTPUT(out_parent_link,          NULL);
    WRITE_OUTPUT(out_node,                 (           LOOKUP_ROOT_CNODE(lookup))        );
    WRITE_OUTPUT(out_node_link,            ( ARROW_REF(LOOKUP_ROOT_CNODE(lookup), left)) );

    WRITE_OUTPUT(out_node_val,             NULL);

    WRITE_OUTPUT(out_grandparent_ordering, 0);
    WRITE_OUTPUT(out_parent_ordering,      0);
    WRITE_OUTPUT(out_ordering,             0);

    return lookup;
  }

  LOOKUP_OPTIONAL_CNODE(lookup, root);

  grandparent          = NULL;
  grandparent_link     = NULL;
  parent               = NULL;
  parent_link          = NULL;
  node_link            = NULL;
  grandparent_ordering = 0;
  parent_ordering      = 0;
  ordering             = 0;
  node                 = root;
  for (;;)
  {
    grandparent_ordering = parent_ordering;
    parent_ordering      = ordering;

    /* val <?= node value */
    node_val = LOOKUP_NODE_CVALUE(lookup, node);
    ordering = call_callback_compare(cmp, val, node_val);

#if ERROR_CHECKING
    if (IS_ORDERING_ERROR(ordering))
    {
      WRITE_OUTPUT(out_grandparent,          NULL);
      WRITE_OUTPUT(out_grandparent_link,     NULL);
      WRITE_OUTPUT(out_parent,               NULL);
      WRITE_OUTPUT(out_parent_link,          NULL);
      WRITE_OUTPUT(out_node,                 NULL);
      WRITE_OUTPUT(out_node_link,            NULL);

      WRITE_OUTPUT(out_node_val,             NULL);

      WRITE_OUTPUT(out_grandparent_ordering, grandparent_ordering);
      WRITE_OUTPUT(out_parent_ordering,      parent_ordering);
      WRITE_OUTPUT(out_ordering,             ordering);

      return NULL;
    }
#endif /* #if ERROR_CHECKING  */

    if      (ordering < 0)
      node_link = &node->left;
    else if (ordering > 0)
      node_link = &node->right;
    else
      break;

    if (BNODE_IS_LEAF(*node_link))
      break;

    grandparent      = parent;
    grandparent_link = parent_link;

    parent      = node;
    parent_link = node_link;

    node = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(*node_link));
  }

  if (ordering == 0)
    node_link = &node->left;

  WRITE_OUTPUT(out_grandparent,          grandparent);
  WRITE_OUTPUT(out_grandparent_link,     grandparent_link);
  WRITE_OUTPUT(out_parent,               parent);
  WRITE_OUTPUT(out_parent_link,          parent_link);
  WRITE_OUTPUT(out_node,                 node);
  WRITE_OUTPUT(out_node_link,            node_link);

  WRITE_OUTPUT(out_node_val,             node_val);

  WRITE_OUTPUT(out_grandparent_ordering, grandparent_ordering);
  WRITE_OUTPUT(out_parent_ordering,      parent_ordering);
  WRITE_OUTPUT(out_ordering,             ordering);

  return lookup;
}

/* ---------------------------------------------------------------- */

/*
 * TODO
 *
 * If there is no space to insert a value,
 * then sets "out_is_duplicate" and returns NULL.
 */
lookup_t *lookup_insert
  ( lookup_t           *lookup
  , const void         *val
  , int                 add_when_exists

  , callback_compare_t  cmp

  , size_t             *out_value_index
  , int                *out_is_duplicate
  )
{
  int empty;

  LOOKUP_FIND_VARIABLE_DECLARATIONS;

  size_t   child_ref;
  bnode_t *child;

  size_t   value_ref;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  WRITE_OUTPUT(out_value_index,   0);
  WRITE_OUTPUT(out_is_duplicate, -1);

  /* Is a value provided? */
  if (!val)
    return NULL;

  /* ---------------------------------------------------------------- */

  empty = LOOKUP_EMPTY(lookup);

  /* Ordered BST traversal to leaf or first duplicate. */
  if (!LOOKUP_FIND_FROM_STD(lookup, NULL, val, cmp))
    return NULL;

  /* Is duplicate? */
  if (!empty && ordering == 0)
  {
    WRITE_OUTPUT(out_is_duplicate, 1);

    if (!add_when_exists)
    {
      WRITE_OUTPUT(out_value_index, LOOKUP_GET_VALUE_INDEX(lookup, node_val));
      return lookup;
    }
  }
  else
  {
    WRITE_OUTPUT(out_is_duplicate, 0);
  }

  /* Out of space? */
  if (LOOKUP_MAX_CAPACITY(lookup))
    return NULL;

  /* Add a value and node. */
  child_ref = lookup_next_value(lookup);
  value_ref = lookup_next_order(lookup);
  ++lookup->len;

  /* Link the value. */
  child = LOOKUP_INDEX_ORDER(lookup, child_ref);
  BNODE_SET_VALUE(child, value_ref);

  /* Write the value. */
  memmove(LOOKUP_INDEX_VALUE(lookup, value_ref), val, LOOKUP_VALUE_SIZE(lookup));

  /* Set in use. */
  LOOKUP_SET_ORDER_IN_USE_BIT(lookup, child_ref, 1);
  LOOKUP_SET_VALUE_IN_USE_BIT(lookup, value_ref, 1);

  /* Write out index. */
  WRITE_OUTPUT(out_value_index, value_ref);

  /* ---------------------------------------------------------------- */

  /* Insert the node into the tree. */

  /* Is this the first value? */
  if (empty)
  {
    BNODE_SET_BLACK(child);
    BNODE_LINK_SET_LEAF(&child->left);
    BNODE_LINK_SET_LEAF(&child->right);

    return lookup;
  }

  /* TODO: balance! */

  /* Are we inserting into a leaf? */
  if (ordering != 0)
  {
    BNODE_LINK_SET_LEAF(&child->left);
    BNODE_LINK_SET_LEAF(&child->right);

    BNODE_LINK_SET_REF (node_link, child_ref);

    return lookup;
  }
  else
  {
    /* Inserting a duplicate value. */
    if (BNODE_IS_LEAF(*node_link))
      BNODE_LINK_SET_LEAF(&child->left);
    else
      BNODE_LINK_SET_REF (&child->left, BNODE_GET_REF(*node_link));
    BNODE_LINK_SET_LEAF(&child->right);

    BNODE_LINK_SET_REF(node_link, child_ref);

    return lookup;
  }
}

const void *lookup_retrieve
  ( const lookup_t     *lookup
  , const void         *val

  , callback_compare_t  cmp
  )
{
  LOOKUP_CFIND_VARIABLE_DECLARATIONS;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  /* Is a value provided? */
  if (!val)
    return NULL;

  /* ---------------------------------------------------------------- */

  /* Ordered BST traversal to leaf or first duplicate. */
  if (!LOOKUP_CFIND_FROM_STD(lookup, NULL, val, cmp))
    return NULL;

  /* Was there no match? */
  if (ordering != 0)
    return NULL;

  /* Return the value. */
  return node_val;
}

/* Returns the number of matches, (even when it exceeds out_val_num_max). */
size_t lookup_retrieve_multiple
  ( const lookup_t     *lookup
  , const void         *val

  , callback_compare_t  cmp

  , void               *out_val
  , size_t              out_val_num_max
  )
{
  size_t num_child_matches;

  unsigned char *out_val_bytes = out_val;

  LOOKUP_CFIND_VARIABLE_DECLARATIONS;

#if ERROR_CHECKING
  if (!lookup)
    return 0;
#endif /* #if ERROR_CHECKING  */

  /* Is a value provided? */
  if (!val)
    return 0;

  /* ---------------------------------------------------------------- */

  /* Ordered BST traversal to leaf or first match. */
  if (!LOOKUP_CFIND_FROM_STD(lookup, NULL, val, cmp))
    return 0;

  /* Was there no match? */
  if (ordering != 0)
    return 0;

  /* Traverse each left-node while equivalent. */
  for (num_child_matches = 0; ; ++num_child_matches)
  {
    if (out_val_bytes)
    {
      /* Write out node_val? */
      if (num_child_matches < out_val_num_max)
      {
        memmove(out_val_bytes, node_val, LOOKUP_VALUE_SIZE(lookup));
        out_val_bytes += LOOKUP_VALUE_SIZE(lookup);
      }

      /* Leaf? */
      if (BNODE_IS_LEAF(node->left))
        break;

      /* Next node. */
      node = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(node->left));

      /* val <?= node value */
      node_val = LOOKUP_NODE_CVALUE(lookup, node);
      ordering = call_callback_compare(cmp, val, node_val);

      /* Non-matching value? */
      if (ordering != 0)
        break;
    }
  }

  return num_child_matches + 1;
}

#if DEBUG
  size_t debug_bnode(const lookup_t *lookup, const bnode_t *node, char *out_info, size_t info_size)
  {
    size_t written = 0;

    ENCLOSE()
    {
      const int *value;

      if (!lookup)
      {
        written += snprintf
          ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
            "lookup: NULL.\n"
          );

        break;
      }

      if (LOOKUP_VALUE_SIZE(lookup) != sizeof(int))
      {
        written += snprintf
          ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
            "lookup: size differs from ints!\n"
          );

        break;
      }

      if (LOOKUP_EMPTY(lookup))
      {
        written += snprintf
          ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
            "lookup: empty.\n"
          );

        break;
      }

      if (!node)
        node = LOOKUP_ROOT_CNODE(lookup);

      value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));

      written += snprintf
        ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
          "****************************************************************\n"
          "node #%d: %d:\n"
          "  value: %d\n"
          "  left:  %s\n"
          "  right: %s\n"
          "\n"

        , (int) (node - lookup->order), (int) *value
          , (int) *value
          , (const char *) (BNODE_IS_LEAF(node->left)  ? "<leaf>" : "<Child>")
          , (const char *) (BNODE_IS_LEAF(node->right) ? "<leaf>" : "<Child>")
        );

      /* ---------------------------------------------------------------- */

      if      ( BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
      {
        /*
         * written += snprintf
         *  ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
         *    "        %d                \n"
         *    "       /  \               \n"
         *    "      /    \              \n"
         *    "    LL      LL            \n"
         *    "   /  \    /  \           \n"
         *    "  *    *  *    *          \n"
         *
         *  , (int) *value
         *  );
         */

        written += snprintf
          ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
            "       % 03d\n"
            "       /  \\\n"
            "      /    \\\n"
            "    LL      LL\n"
            "   /  \\    /  \\\n"
            "  *    *  *    *\n"

          , (int) *value
          );
      }
      /* ---------------------------------------------------------------- */
      else if (!BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
      {
        const bnode_t *left       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (node->left));
        const int     *left_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(left->value));

        if      ( BNODE_IS_LEAF(left->left) &&  BNODE_IS_LEAF(left->right))
        {
          /*
           * written += snprintf
           *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
           *     "        %d                \n"
           *     "       /  \               \n"
           *     "      /    \              \n"
           *     "    %d      LL            \n"
           *     "   /  \    /  \           \n"
           *     "  L    L  *    *          \n"
           *
           *   , (int) *value
           *
           *   , (int) *left_value
           *   );
           */

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "       % 03d\n"
              "       /  \\\n"
              "      /    \\\n"
              "   % 03d      LL\n"
              "   /  \\    /  \\\n"
              "  L    L  *    *\n"

            , (int) *value

            , (int) *left_value
            );
        }
        else if (!BNODE_IS_LEAF(left->left) &&  BNODE_IS_LEAF(left->right))
        {
          const bnode_t *left_left       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (left->left));
          const int     *left_left_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(left_left->value));

          /*
           * written += snprintf
           *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
           *     "        %d                \n"
           *     "       /  \               \n"
           *     "      /    \              \n"
           *     "    %d      LL            \n"
           *     "   /  \    /  \           \n"
           *     "  %d   L  *    *          \n"
           *
           *   , (int) *value
           *
           *   , (int) *left_value
           *
           *   , (int) *left_left_value
           *   );
           */

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "       % 03d\n"
              "       /  \\\n"
              "      /    \\\n"
              "   % 03d      LL\n"
              "   /  \\    /  \\\n"
              " % 03d   L  *    *\n"

            , (int) *value

            , (int) *left_value

            , (int) *left_left_value
            );
        }
        else if ( BNODE_IS_LEAF(left->left) && !BNODE_IS_LEAF(left->right))
        {
          const bnode_t *left_right       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (left->right));
          const int     *left_right_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(left_right->value));

          /*
           * written += snprintf
           *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
           *     "        %d                \n"
           *     "       /  \               \n"
           *     "      /    \              \n"
           *     "    %d      LL            \n"
           *     "   /  \    /  \           \n"
           *     "  L   %d  *    *          \n"
           *
           *   , (int) *value
           *
           *   , (int) *left_value
           *
           *   , (int) *left_right_value
           *   );
           */

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "       % 03d\n"
              "       /  \\\n"
              "      /    \\\n"
              "   % 03d      LL\n"
              "   /  \\    /  \\\n"
              "  L  % 03d  *    *\n"

            , (int) *value

            , (int) *left_value

            , (int) *left_right_value
            );
        }
        else
        {
          const bnode_t *left_left       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (left->left));
          const int     *left_left_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(left_left->value));

          const bnode_t *left_right       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (left->right));
          const int     *left_right_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(left_right->value));

          /*
           * written += snprintf
           *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
           *     "        %d                \n"
           *     "       /  \               \n"
           *     "      /    \              \n"
           *     "    %d      LL            \n"
           *     "   /  \    /  \           \n"
           *     "  %d  %d  *    *          \n"
           *
           *   , (int) *value
           *
           *   , (int) *left_value
           *
           *   , (int) *left_left_value
           *   , (int) *left_right_value
           *  );
           */

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "       % 03d\n"
              "       /  \\\n"
              "      /    \\\n"
              "   % 03d      LL\n"
              "   /  \\    /  \\\n"
              " % 03d % 03d  *    *\n"

            , (int) *value

            , (int) *left_value

            , (int) *left_left_value
            , (int) *left_right_value
            );
        }
      }
      /* ---------------------------------------------------------------- */
      else if ( BNODE_IS_LEAF(node->left) && !BNODE_IS_LEAF(node->right))
      {
        const bnode_t *right       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (node->right));
        const int     *right_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(right->value));

        if      ( BNODE_IS_LEAF(right->left) &&  BNODE_IS_LEAF(right->right) )
        {
          /*
           * written += snprintf
           *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
           *     "        %d                \n"
           *     "       /  \               \n"
           *     "      /    \              \n"
           *     "    LL      %d            \n"
           *     "   /  \    /  \           \n"
           *     "  *    *  L    L          \n"
           *
           *   , (int) *value
           *
           *   , (int) *right_value
           *   );
           */

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "       % 03d\n"
              "       /  \\\n"
              "      /    \\\n"
              "    LL     % 03d\n"
              "   /  \\    /  \\\n"
              "  *    *  L    L\n"

            , (int) *value

            , (int) *right_value
            );
        }
        else if (!BNODE_IS_LEAF(right->left) &&  BNODE_IS_LEAF(right->right))
        {
          const bnode_t *right_left       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (right->left));
          const int     *right_left_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(right_left->value));

          /*
           * written += snprintf
           *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
           *     "        %d                \n"
           *     "       /  \               \n"
           *     "      /    \              \n"
           *     "    LL      %d            \n"
           *     "   /  \    /  \           \n"
           *     "  *    *  %d   L          \n"
           *
           *   , (int) *value
           *
           *   , (int) *right_value
           *
           *   , (int) *right_left_value
           *   );
           */

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "       % 03d\n"
              "       /  \\\n"
              "      /    \\\n"
              "    LL     % 03d\n"
              "   /  \\    /  \\\n"
              "  *    * % 03d   L\n"

            , (int) *value

            , (int) *right_value

            , (int) *right_left_value
            );
        }
        else if ( BNODE_IS_LEAF(right->left) && !BNODE_IS_LEAF(right->right))
        {
          const bnode_t *right_right       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (right->right));
          const int     *right_right_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(right_right->value));

          /*
           * written += snprintf
           *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
           *     "        %d                \n"
           *     "       /  \               \n"
           *     "      /    \              \n"
           *     "    LL      %d            \n"
           *     "   /  \    /  \           \n"
           *     "  *    *  L   %d          \n"
           *
           *   , (int) *value
           *
           *   , (int) *right_value
           *
           *   , (int) *right_right_value
           *   );
           */

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "       % 03d\n"
              "       /  \\\n"
              "      /    \\\n"
              "    LL     % 03d\n"
              "   /  \\    /  \\\n"
              "  *    *  L  % 03d\n"

            , (int) *value

            , (int) *right_value

            , (int) *right_right_value
            );
        }
        else
        {
          const bnode_t *right_left       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (right->left));
          const int     *right_left_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(right_left->value));

          const bnode_t *right_right       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (right->right));
          const int     *right_right_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(right_right->value));

          /*
           * written += snprintf
           *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
           *     "        %d                \n"
           *     "       /  \               \n"
           *     "      /    \              \n"
           *     "    LL      %d            \n"
           *     "   /  \    /  \           \n"
           *     "  *    *  %d  %d          \n"
           *
           *   , (int) *value
           *
           *   , (int) *right_value
           *
           *   , (int) *right_left_value
           *   , (int) *right_right_value
           *   );
           */

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "       % 03d\n"
              "       /  \\\n"
              "      /    \\\n"
              "    LL     % 03d\n"
              "   /  \\    /  \\\n"
              "  *    * % 03d % 03d\n"

            , (int) *value

            , (int) *right_value

            , (int) *right_left_value
            , (int) *right_right_value
            );
        }
      }
      /* ---------------------------------------------------------------- */
      else
      {
        const bnode_t *left       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (node->left));
        const int     *left_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(left->value));

        const bnode_t *right       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (node->right));
        const int     *right_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(right->value));

        /*
         * written += snprintf
         *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
         *     "        %d                \n"
         *     "       /  \               \n"
         *     "      /    \              \n"
         *     "    %d      %d            \n"
         *     "   /  \    /  \           \n"
         *     "  xx  xx  xx  xx          \n"
         *
         *   , (int) *value
         *
         *   , (int) *left_value
         *   , (int) *right_value
         *   );
         */

        /*
         * written += snprintf
         *   ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
         *     "        %d                \n"
         *     "       /  \               \n"
         *     "      /    \              \n"
         *     "    %d      %d            \n"
         *     "   /  \    /  \           \n"
         *
         *   , (int) *value
         *
         *   , (int) *left_value
         *   , (int) *right_value
         *   );
         */

        written += snprintf
          ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
            "       % 03d\n"
            "       /  \\\n"
            "      /    \\\n"
            "   % 03d     % 03d\n"
            "   /  \\    /  \\\n"

          , (int) *value

          , (int) *left_value
          , (int) *right_value
          );

        /* 4 vals */
        /*
        written += snprintf
          ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
            "  xx  xx  xx  xx\n"

          , (int) *value

          , (int) *left_value
          , (int) *right_value
          );
        */

        /* ---------------------------------------------------------------- */
        /* val #1.                                                          */

        if (BNODE_IS_LEAF(left->left))
        {
          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "  LL"
            );
        }
        else
        {
          const bnode_t *left_left       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (left->left));
          const int     *left_left_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(left_left->value));

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              " % 03d"

            , (int) *left_left_value
            );
        }

        /* ---------------------------------------------------------------- */
        /* val #2.                                                          */

        if (BNODE_IS_LEAF(left->right))
        {
          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "  LL"
            );
        }
        else
        {
          const bnode_t *left_right       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (left->right));
          const int     *left_right_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(left_right->value));

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              " % 03d"

            , (int) *left_right_value
            );
        }

        /* ---------------------------------------------------------------- */
        /* val #3.                                                          */

        if (BNODE_IS_LEAF(right->left))
        {
          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "  LL"
            );
        }
        else
        {
          const bnode_t *right_left       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (right->left));
          const int     *right_left_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(right_left->value));

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              " % 03d"

            , (int) *right_left_value
            );
        }

        /* ---------------------------------------------------------------- */
        /* val #4.                                                          */

        if (BNODE_IS_LEAF(right->right))
        {
          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              "  LL\n"
            );
        }
        else
        {
          const bnode_t *right_right       = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF  (right->right));
          const int     *right_right_value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(right_right->value));

          written += snprintf
            ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
              " % 03d\n"

            , (int) *right_right_value
            );
        }
      }

      written += snprintf
        ( (char *) (out_info + written), (size_t) terminator_size(size_minus(info_size, written)), ""
          "****************************************************************\n"
          "\n"
        );
    }

    return written;
  }

# include <stdio.h>  /* fwrite, stderr, fprintf */
  void debug_lookup_print(FILE *out, const char *str)
  {
    if (!str)
      return;

    if (!out)
      out = stderr;

    fwrite(str, sizeof(*str), strzlen(str), out);
  }

  void debug_print_bnode(const lookup_t *lookup, const bnode_t *node, FILE *out)
  {
    char buf[DEFAULT_BUF_SIZE];
    *buf = 0;

    if (!out)
      out = stderr;

    debug_bnode(lookup, node, buf, sizeof(buf));

    debug_lookup_print(out, buf);
  }
#endif /* #if DEBUG */

#if !DEBUG || !defined(DEBUG_DELETE)
#  define DELETE_DEBUG(a) do {} while(0)
#else
#  define DELETE_DEBUG(a) do { if (LOOKUP_VALUE_SIZE(lookup) == sizeof(int)) { a; } } while(0)
#endif

/* Delete all matches. */
lookup_t *lookup_delete
  ( lookup_t           *lookup
  , const void         *val

  , callback_compare_t  cmp

  , size_t             *out_num_deleted
  )
{
  size_t   num_deleted;
  bnode_t *next_root;

  LOOKUP_FIND_VARIABLE_DECLARATIONS;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (!val)
  {
    WRITE_OUTPUT(out_num_deleted, 0);
    return NULL;
  }

  WRITE_OUTPUT(out_num_deleted, 0);

  /* ---------------------------------------------------------------- */

  DELETE_DEBUG
    ( fprintf
        ( stderr, ""
          "\n"
          "/ ----------------------------------------------------------------\n"
          "|                                                                 \n"
          "| ****************************************************************\n"
          "|                                                                 \n"
          "| ----------------------------------------------------------------\n"
          "|\n"
          "| lookup_delete(lookup, <%d>, ...);\n"
          "|\n"
          "|\n"
          "*\n"
          "\n"

        , (int) *(const int *) val
        )
    );

  /* Is the lookup container empty? */
  if (LOOKUP_EMPTY(lookup))
  {
    WRITE_OUTPUT(out_num_deleted, 0);
    return lookup;
  }

  next_root   = LOOKUP_ROOT_NODE(lookup);

  num_deleted = 0;

  for (;;)
  {
    WRITE_OUTPUT(out_num_deleted, num_deleted);

    /* Is the lookup container empty? */
    if (LOOKUP_EMPTY(lookup))
    {
      WRITE_OUTPUT(out_num_deleted, num_deleted);
      return lookup;
    }

    /* Ordered BST traversal to leaf or first match. */
    if (!LOOKUP_FIND_FROM_STD(lookup, next_root, val, cmp))
      return NULL;

    /* Set next root. */
    if (parent)
      next_root = parent;
    else
      next_root = LOOKUP_ROOT_NODE(lookup);

    /* Did we find no match? */
    if (ordering != 0)
    {
      WRITE_OUTPUT(out_num_deleted, num_deleted);
      return lookup;
    }

    /* ---------------------------------------------------------------- */

    if (!LOOKUP_IS_RECYCLING(lookup))
    {
      --lookup->next_value;
      --lookup->next_order;
    }

    if      ( BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
    {
      if (parent_link)
        BNODE_LINK_SET_LEAF(parent_link);

      LOOKUP_SET_ORDER_IN_USE_BIT(lookup, node - lookup->order,         0);
      LOOKUP_SET_VALUE_IN_USE_BIT(lookup, BNODE_GET_VALUE(node->value), 0);
      --lookup->len;

      ++num_deleted;
      WRITE_OUTPUT(out_num_deleted, num_deleted);

      break;
    }
    else if (!BNODE_IS_LEAF(node->left) &&  BNODE_IS_LEAF(node->right))
    {
      if (parent_link)
        BNODE_LINK_SET_REF(parent_link, BNODE_GET_REF(node->left));

      LOOKUP_SET_ORDER_IN_USE_BIT(lookup, node - lookup->order,         0);
      LOOKUP_SET_VALUE_IN_USE_BIT(lookup, BNODE_GET_VALUE(node->value), 0);
      --lookup->len;

      ++num_deleted;
      WRITE_OUTPUT(out_num_deleted, num_deleted);

      /*
       *   n
       *  / \
       * l   r
       *
       * l <= n < r
       *
       * l may be a duplicate; continue.
       */
      continue;
    }
    else if ( BNODE_IS_LEAF(node->left) && !BNODE_IS_LEAF(node->right))
    {
      if (parent_link)
        BNODE_LINK_SET_REF(parent_link, BNODE_GET_REF(node->right));

      LOOKUP_SET_ORDER_IN_USE_BIT(lookup, node - lookup->order,         0);
      LOOKUP_SET_VALUE_IN_USE_BIT(lookup, BNODE_GET_VALUE(node->value), 0);

      --lookup->len;

      ++num_deleted;
      WRITE_OUTPUT(out_num_deleted, num_deleted);

      break;
    }
    else
    {
      /* We need to handle duplicates. */

      bnode_t    *end;              /* node->right->leftmost */
      const void *end_val;

      bnode_t    *begin;            /* node->right->leftmost_first_equal */
      bnode_t    *begin_parent;     /* begin->parent                     */

      bnode_t    *begin_rightmost;  /* node->right->rightmost */

      /* ---------------------------------------------------------------- */

      LOOKUP_SET_VALUE_IN_USE_BIT(lookup, BNODE_GET_VALUE(node->value), 0);

      /* ---------------------------------------------------------------- */

      /* Pivot nodes we'll relink. */
      /* 1) end:   First, find right->leftmost and its value (end_val).        */
      /* 2) begin: right->leftmost, stopping at the first equivalent value.    */
      /*    begin_parent:    Also track begin->parent.                         */
      /* 3) begin_rightmost: Find right->rightmost.                            */

      /* Relinking. */
      /* 4) end->left              = node->left                                */
      /* 5) begin_rightmost->right = node->right unless begin_parent == begin. */
      /* 6) begin_parent->left     = NULL        unless begin_parent == begin. */
      /* 7) parent_link            = begin                                     */

      /* Then we can free node.                                                */

      /* ---------------------------------------------------------------- */

      /* Example:                                                            * */
      /*                                                                     * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* *                      p                                            * */
      /* *                      |                                            * */
      /* *                      |                                            * */
      /* *              node-> 07                                            * */
      /* *                    /  \                                           * */
      /* *                   /    \                                          * */
      /* *                 04      10                                        * */
      /* *                        /  \                                       * */
      /* *                       /    \                                      * */
      /* *             begin-> 08      12                                    * */
      /* *                    /  \                                           * */
      /* *                   /    \                                          * */
      /* *                 08      09 <- begin_rightmost                     * */
      /* *                /                                                  * */
      /* *               /                                                   * */
      /* *             08                                                    * */
      /* *            /                                                      * */
      /* *           /                                                       * */
      /* *   end-> 08                                                        * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* * 4) end->left                = node->left                          * */
      /* * if (begin_parent != node):                                        * */
      /* *   5) begin_rightmost->right = node->right                         * */
      /* *   6) begin_parent->left     = NULL                                * */
      /* * 7) p->parent_link           = begin                               * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* *                     p                                             * */
      /* *                     |                                             * */
      /* *                     |                                             * */
      /* *            begin-> 08                                             * */
      /* *                   /  \                                            * */
      /* *                  /    \                                           * */
      /* *                08      09 <- begin_rightmost                      * */
      /* *               /          \                                        * */
      /* *              /            \                                       * */
      /* *            08              10                                     * */
      /* *           /                  \                                    * */
      /* *          /                    \                                   * */
      /* *  end-> 08                      12                                 * */
      /* *       /                                                           * */
      /* *      /                                                            * */
      /* *    04                                                             * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */

      /* Example:                                                            * */
      /*                                                                     * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* *               p                                                   * */
      /* *               |                                                   * */
      /* *               |                                                   * */
      /* *       node-> 07                                                   * */
      /* *             /  \                                                  * */
      /* *            /    \                                                 * */
      /* *           /      \                                                * */
      /* *         04        12                                              * */
      /* *        /  \      /  \                                             * */
      /* *       /    \    /    \                                            * */
      /* *      03    05  08    42                                           * */
      /* *                 ^                                                 * */
      /* *                 |                                                 * */
      /* *                 begin = end = begin_rightmost                     * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* * 4) end->left                = node->left                          * */
      /* * if (begin_parent != node): (true)                                 * */
      /* *   5) begin_rightmost->right = node->right                         * */
      /* *   6) begin_parent->left     = NULL                                * */
      /* * 7) p->parent_link           = begin                               * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* *               p                                                   * */
      /* *               |                                                   * */
      /* *               |                                                   * */
      /* *              08 <- begin = end = begin_rightmost                  * */
      /* *             /  \                                                  * */
      /* *            /    \                                                 * */
      /* *           /      \                                                * */
      /* *         04        12                                              * */
      /* *        /  \         \                                             * */
      /* *       /    \         \                                            * */
      /* *      03    05         42                                          * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */

      /* Example:                                                            * */
      /*                                                                     * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* *               p                                                   * */
      /* *               |                                                   * */
      /* *               |                                                   * */
      /* *       node-> 07                                                   * */
      /* *             /  \                                                  * */
      /* *            /    \                                                 * */
      /* *           /      \                                                * */
      /* *         04        12 <- begin                                     * */
      /* *        /  \      /  \                                             * */
      /* *       /    \    /    \                                            * */
      /* *      03    05  12    42 <- begin_rightmost                        * */
      /* *                 ^                                                 * */
      /* *                 |                                                 * */
      /* *                 end                                               * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* * 4) end->left                = node->left                          * */
      /* * if (begin_parent != node): (false)                                * */
      /* *   5) begin_rightmost->right = node->right                         * */
      /* *   6) begin_parent->left     = NULL                                * */
      /* * 7) p->parent_link           = begin                               * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */
      /* *                                                                   * */
      /* *              p                                                    * */
      /* *              |                                                    * */
      /* *              |                                                    * */
      /* *             12 <- begin                                           * */
      /* *            /  \                                                   * */
      /* *           /    \                                                  * */
      /* *   end-> 12     42 <- begin_rightmost                              * */
      /* *        /                                                          * */
      /* *       /                                                           * */
      /* *      04                                                           * */
      /* *     /  \                                                          * */
      /* *    /    \                                                         * */
      /* *   03    05                                                        * */
      /* *                                                                   * */
      /* * ----------------------------------------------------------------- * */

      /* ---------------------------------------------------------------- */
      /* Nodes of interest.                                               */

      /* 1) Get end node.  Sets "end" and "end_val".                      */
      if (!(end_val = lookup_min(lookup, LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right)), &end)))
        return NULL;

      /* 2) Get first end node with same value.                           */
      /*    Sets "begin" and "begin_parent".                              */
      if
        (
          !(
            lookup_find_from
              ( /* lookup                   */ lookup
              , /* root                     */ LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right))
              , /* val                      */ end_val

              , /* cmp                      */ cmp

              , /* out_grandparent          */ NULL
              , /* out_grandparent_link     */ NULL
              , /* out_parent               */ &begin_parent
              , /* out_parent_link          */ NULL
              , /* out_node                 */ &begin
              , /* out_node_link            */ NULL

              , /* out_node_val             */ NULL

              , /* out_grandparent_ordering */ NULL
              , /* out_parent_ordering      */ NULL
              , /* out_ordering             */ NULL
              )
          )
        )
        return NULL;

      /* 3) Get begin_rightmost node.                                     */
      if (begin_parent != node)
        if (!(lookup_max(lookup, begin, &begin_rightmost)))
          return NULL;

      /* ---------------------------------------------------------------- */

      DELETE_DEBUG(debug_lookup_print(NULL, "/** node:\n"));
      DELETE_DEBUG(debug_print_bnode(lookup, node, NULL));

      DELETE_DEBUG(debug_lookup_print(NULL, "/** begin:\n"));
      DELETE_DEBUG(debug_print_bnode(lookup, begin, NULL));

      DELETE_DEBUG(debug_lookup_print(NULL, "/** end:\n"));
      DELETE_DEBUG(debug_print_bnode(lookup, end, NULL));

      if (begin_parent != node)
      {
        DELETE_DEBUG(debug_lookup_print(NULL, "/** begin_rightmost:\n"));
        DELETE_DEBUG(debug_print_bnode(lookup, begin_rightmost, NULL));
      }

      /* ---------------------------------------------------------------- */
      /* Relinking.                                                       */

      /* 4) end->left = node->left                                        */
      BNODE_LINK_SET_REF(&end->left, BNODE_GET_REF(node->left));

      if (begin_parent != node)
      {
        /* 5) begin_rightmost->right = node->right */
        BNODE_LINK_SET_REF(&begin_rightmost->right, node->right);

        /* 6) begin_parent->left     = NULL        */
        BNODE_LINK_SET_LEAF(&begin_parent->left);
      }

      /* 7) parent_link            = begin                                */
      if (parent)
      {
        BNODE_LINK_SET_REF(parent_link, begin - lookup->order);
      }
      else
      {
        /* Root node: node = begin, then we'll free begin instead. */
        BNODE_SET_VALUE   (node,         BNODE_GET_VALUE(begin->value));
        BNODE_LINK_SET_REF(&node->left,  BNODE_GET_REF  (begin->left));
        BNODE_LINK_SET_REF(&node->right, BNODE_GET_REF  (begin->right));
      }

      /* ---------------------------------------------------------------- */
      /* Free node.                                                       */

      if (parent)
      {
        LOOKUP_SET_ORDER_IN_USE_BIT(lookup, node - lookup->order, 0);
      }
      else
      {
        /* Root node: we set node to begin, since we're freeing "begin" instead.
         */
        LOOKUP_SET_ORDER_IN_USE_BIT(lookup, begin - lookup->order, 0);
      }

      --lookup->len;

      /* ---------------------------------------------------------------- */

      if (parent)
      {
        DELETE_DEBUG(debug_lookup_print(NULL, "/** replacement:\n"));
        DELETE_DEBUG(debug_print_bnode(lookup, LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(*parent_link)), NULL));
      }

      /* ---------------------------------------------------------------- */

      /* For this node we're done! */

      ++num_deleted;
      WRITE_OUTPUT(out_num_deleted, num_deleted);

      continue;
    }
  }

  DELETE_DEBUG
    ( fprintf
        ( stderr, ""
          "\n"
          "*\n"
          "|\n"
          "|\n"
          "| Finished: lookup_delete(lookup, <%d>, ...);\n"
          "|\n"
          "| ----------------------------------------------------------------\n"
          "|                                                                 \n"
          "| ****************************************************************\n"
          "|                                                                 \n"
          "\\ ----------------------------------------------------------------\n"
          "\n"

        , (int) *(const int *) val
        )
    );

  WRITE_OUTPUT(out_num_deleted, num_deleted);

  return lookup;
}

#ifdef TODO
/* Returns the number of deletions, (even when it exceeds out_val_num_max). */
size_t lookup_delete_limit
  ( lookup_t           *lookup
  , const void         *val
  , size_t              limit

  , callback_compare_t  cmp

  , size_t             *out_num_deleted
  , void               *out_val
  , size_t              out_val_num_max
  );
#endif /* #ifdef TODO */

/* ---------------------------------------------------------------- */

const lookup_tree_traversal_direction_t lttd_current  = ltd_current;
const lookup_tree_traversal_direction_t lttd_break    = ltd_break;
const lookup_tree_traversal_direction_t lttd_parent   = ltd_parent;
const lookup_tree_traversal_direction_t lttd_left     = ltd_left;
const lookup_tree_traversal_direction_t lttd_right    = ltd_right;
const lookup_tree_traversal_direction_t lttd_end      = ltd_end;
const lookup_tree_traversal_direction_t lttd_bits     = ltd_bits;
const lookup_tree_traversal_direction_t lttd_end_mask = ltd_end_mask;

/* ---------------------------------------------------------------- */

static void *lookup_traverse_tree_from_step
  ( const lookup_t *lookup
  , const bnode_t  *node

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  , void                            *initial_accumulation

  , lookup_tree_traversal_direction_t *dir
  )
{
  for (;;)
  {
    const bnode_t *child;

    switch (*dir)
    {
      default:
      case ltd_break:
        return initial_accumulation;

      case ltd_current:
        initial_accumulation = with_value
          ( with_value_context
          , initial_accumulation

          , lookup
          , LOOKUP_NODE_CVALUE(lookup, node)
          , node

          , (int *) dir
          );
        break;

      case ltd_parent:
        *dir = ltd_current;
        return initial_accumulation;

      case ltd_left:
        if (BNODE_IS_LEAF(node->left))
        {
          *dir = ltd_break;
          return initial_accumulation;
        }
        else
        {
          *dir = ltd_current;

          child = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(node->left));

          initial_accumulation = lookup_traverse_tree_from_step
            ( lookup
            , child

            , with_value
            , with_value_context
            , initial_accumulation

            , dir
            );

          break;
        }

      case ltd_right:
        if (BNODE_IS_LEAF(node->right))
        {
          *dir = ltd_break;
          return initial_accumulation;
        }
        else
        {
          *dir = ltd_current;

          child = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(node->right));

          initial_accumulation = lookup_traverse_tree_from_step
            ( lookup
            , child

            , with_value
            , with_value_context
            , initial_accumulation

            , dir
            );

          break;
        }
    }
  }
}

/* out_break_iteration should be set to the next direction. */
void *lookup_traverse_tree_from
  ( const lookup_t *lookup
  , const bnode_t  *root

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  , void                            *initial_accumulation
  )
{
  lookup_tree_traversal_direction_t dir;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
  if (!with_value)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_EMPTY(lookup))
    return initial_accumulation;

  LOOKUP_OPTIONAL_CNODE(lookup, root);

  dir = ltd_current;

  initial_accumulation =
    lookup_traverse_tree_from_step
      ( lookup
      , root

      , with_value
      , with_value_context
      , initial_accumulation

      , &dir
      );

  return initial_accumulation;
}

static void *traverse_values
  ( void *context
  , void *last_accumulation

  , const lookup_t *lookup
  , const void     *value
  , const bnode_t  *node

  , int *out_dir
  )
{
  lookup_traverser_values_subtree_context_t *tcontext = context;

  if (tcontext->first)
  {
    if (tcontext->left_to_right)
    {
      if (!BNODE_IS_LEAF(node->left))
      {
        *out_dir = ltd_left;
        return last_accumulation;
      }
      else
      {
        tcontext->first = 0;
        *out_dir = ltd_current;
        return last_accumulation;
      }
    }
    else
    {
      if (!BNODE_IS_LEAF(node->right))
      {
        *out_dir = ltd_right;
        return last_accumulation;
      }
      else
      {
        tcontext->first = 0;
        *out_dir = ltd_current;
        return last_accumulation;
      }
    }
  }
  else
  {
    if (!tcontext->moving)
    {
      int break_iteration;

      break_iteration = 0;
      last_accumulation = tcontext->with_value
        ( tcontext->with_value_context
        , last_accumulation

        , lookup
        , value
        , node

        , &break_iteration
        );

      if (break_iteration)
      {
        *out_dir = ltd_break;
        return last_accumulation;
      }

      *out_dir = ltd_current;

      if (tcontext->left_to_right)
        tcontext->moving = 1;
      else
        tcontext->moving = -1;

      tcontext->last_value = value;

      return last_accumulation;
    }
    else
    {
      if (!tcontext->parent)
      {
        size_t child_ref;
        lookup_tree_traversal_direction_t child_dir;

        if (tcontext->moving < 0)
        {
          child_ref = node->left;
          child_dir = ltd_left;
        }
        else
        {
          child_ref = node->right;
          child_dir = ltd_right;
        }

        if (tcontext->moving == tcontext->left_to_right)
        {
          if (!BNODE_IS_LEAF(child_ref))
          {
            tcontext->moving = -tcontext->moving;
            *out_dir = child_dir;
            return last_accumulation;
          }
          else
          {
            /* Started retrieving next value, but no child node, so start going
             * upwards. */
            tcontext->parent = 1;
            *out_dir = ltd_parent;
            return last_accumulation;
          }
        }
        else
        {
          if (!BNODE_IS_LEAF(child_ref))
          {
            *out_dir = child_dir;
            return last_accumulation;
          }
          else
          {
            tcontext->moving = 0;
            *out_dir = ltd_current;
            return last_accumulation;
          }
        }
      }
      else
      {
        /* tcontext->parent */
        int ordering;

        /* last_value <?= value */
        ordering = call_callback_compare(tcontext->cmp, tcontext->last_value, value);

#if ERROR_CHECKING
        if (IS_ORDERING_ERROR(ordering))
        {
          *out_dir = ltd_break;
          return NULL;
        }
#endif /* #if ERROR_CHECKING */

        if (  ( tcontext->left_to_right && ordering < 0)
           || (!tcontext->left_to_right && ordering > 0)
           )
        {
          /* Keep going upwards. */
          *out_dir = ltd_parent;
          return last_accumulation;
        }
        else
        {
          /* Next node at either this one or a sub-node on the appropriate side. */
          size_t child_ref;
          lookup_tree_traversal_direction_t child_dir;

          if (tcontext->left_to_right)
          {
            tcontext->moving = 1;
            child_ref = node->right;
            child_dir = ltd_right;
          }
          else
          {
            tcontext->moving = -1;
            child_ref = node->left;
            child_dir = ltd_left;
          }

          if (BNODE_IS_LEAF(child_ref))
          {
            /* No child, but iterate and then keep going. */
            int break_iteration;

            break_iteration = 0;
            last_accumulation = tcontext->with_value
              ( tcontext->with_value_context
              , last_accumulation

              , lookup
              , value
              , node

              , &break_iteration
              );

            if (break_iteration)
            {
              *out_dir = ltd_break;
              return last_accumulation;
            }

            /* Keep going. */
            *out_dir = ltd_parent;
            return last_accumulation;
          }
          else
          {
            tcontext->parent = 0;

            *out_dir = child_dir;
            return last_accumulation;
          }
        }
      }
    }
  }
}

const lookup_iteration_callback_fun_t lookup_traverser_values_subtree =
  (lookup_iteration_callback_fun_t) traverse_values;

lookup_traverser_values_subtree_context_t lookup_traverser_values_subtree_initial_context
  ( const bnode_t *root

  , callback_compare_t cmp

  , int left_to_right

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  )
{
  lookup_traverser_values_subtree_context_t context;

  context.root               = root;
  context.cmp                = cmp;
  context.left_to_right      = left_to_right;
  context.with_value         = with_value;
  context.with_value_context = with_value_context;
  context.first              = 1;
  context.moving             = 0;
  context.parent             = 0;
  context.last_value         = NULL;

  return context;
}

void *lookup_traverse_values_subtree
  ( const lookup_t *lookup
  , const bnode_t  *root

  , callback_compare_t cmp

  , int right_to_left

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  , void                            *initial_accumulation
  )
{
  lookup_traverser_values_subtree_context_t context;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
  if (!with_value)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_EMPTY(lookup))
    return initial_accumulation;

  LOOKUP_OPTIONAL_CNODE(lookup, root);

  context = lookup_traverser_values_subtree_initial_context
    ( root
    , cmp
    , !right_to_left
    , with_value
    , with_value_context
    );

  initial_accumulation =
    lookup_traverse_tree_from
      ( lookup
      , root

      , lookup_traverser_values_subtree 
      , &context
      , initial_accumulation
      );

  return initial_accumulation;
}

void *lookup_traverse_values
  ( const lookup_t *lookup

  , callback_compare_t cmp

  , int right_to_left

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  , void                            *initial_accumulation
  )
{
  return
    lookup_traverse_values_subtree
      ( lookup
      , NULL

      , cmp

      , right_to_left

      , with_value
      , with_value_context
      , initial_accumulation
      );
}

/* ---------------------------------------------------------------- */

static void *lookup_iterate_tree_from_step
  ( const lookup_t *lookup
  , const bnode_t  *node

  , lookup_iteration_order_t order

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context

  , void *initial_accumulation

  , int    *break_iteration
  , size_t  breadth_level
  , size_t  building_queue
  , void   *parent_queue
  , void   *current_queue
  )
{
  size_t i;
  size_t children[2];

  if (node)
  {
    children[ !order.right_to_left] = node->left;
    children[!!order.right_to_left] = node->right;
  }
  else
  {
    children[ !order.right_to_left] = 0;
    children[!!order.right_to_left] = 0;
  }

  if (order.parent_first)
  {
    /* Parent. */
    initial_accumulation = with_value
      ( with_value_context
      , initial_accumulation

      , lookup
      , LOOKUP_NODE_CVALUE(lookup, node)
      , node

      , break_iteration
      );

    if (*break_iteration)
      return initial_accumulation;

    /* Children. */
    for (i = 0; i < ARRAY_NUM(children); ++i)
    {
      if (!BNODE_IS_LEAF(children[i]))
      {
        const bnode_t *child = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(children[i]));

        /* Child. */
        initial_accumulation = with_value
          ( with_value_context
          , initial_accumulation

          , lookup
          , LOOKUP_NODE_CVALUE(lookup, child)
          , child

          , break_iteration
          );

        if (*break_iteration)
          return initial_accumulation;

        /* Grandchildren. */
        initial_accumulation = lookup_iterate_tree_from_step
          ( lookup
          , child

          , order

          , with_value
          , with_value_context

          , initial_accumulation

          , break_iteration
          , 0, 0, NULL, NULL
          );

        if (*break_iteration)
          return initial_accumulation;
      }
    }

    /* Done. */
    return initial_accumulation;
  }
  /* ---------------------------------------------------------------- */
  else if (order.leaves_first)
  {
    /* Children. */
    for (i = 0; i < ARRAY_NUM(children); ++i)
    {
      if (!BNODE_IS_LEAF(children[i]))
      {
        const bnode_t *child = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(children[i]));

        /* Grandchildren. */
        initial_accumulation = lookup_iterate_tree_from_step
          ( lookup
          , child

          , order

          , with_value
          , with_value_context

          , initial_accumulation

          , break_iteration
          , 0, 0, NULL, NULL
          );

        if (*break_iteration)
          return initial_accumulation;

        /* Child. */
        initial_accumulation = with_value
          ( with_value_context
          , initial_accumulation

          , lookup
          , LOOKUP_NODE_CVALUE(lookup, child)
          , child

          , break_iteration
          );

        if (*break_iteration)
          return initial_accumulation;
      }
    }

    /* Parent. */
    initial_accumulation = with_value
      ( with_value_context
      , initial_accumulation

      , lookup
      , LOOKUP_NODE_CVALUE(lookup, node)
      , node

      , break_iteration
      );

    return initial_accumulation;
  }
  /* ---------------------------------------------------------------- */
  else if (order.breadth_first)
  {
    size_t level_num = 1 << breadth_level;

    /* Creating a queue of each of parent's children? */
    if (building_queue)
    {
      size_t         next_ref;
      const bnode_t *next;

      void *new_current_queue[3];
      new_current_queue[0] = (void *) &children[0];
      new_current_queue[1] = (void *) &children[1];
      new_current_queue[2] = (void *) current_queue;

      current_queue = new_current_queue;

      next_ref = *(size_t *) tail_index(2, parent_queue, size_minus(level_num, building_queue));

      if (!BNODE_IS_LEAF(next_ref))
        next = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(next_ref));
      else
        next = NULL;

      initial_accumulation = lookup_iterate_tree_from_step
        ( lookup
        , next

        , order

        , with_value
        , with_value_context

        , initial_accumulation

        , break_iteration

        , breadth_level
        , building_queue - 1
        , parent_queue
        , current_queue
        );

      return initial_accumulation;
    }
    else
    {
      int children_remaining = 0;
      const bnode_t *next;

      void *new_current_queue[] = { NULL };

      parent_queue = current_queue;
      current_queue = new_current_queue;

      next = NULL;

      for (i = 0; i < level_num; ++i)
      {
        size_t ref = *(size_t *) tail_index(2, (void **) parent_queue, i);

        if (!BNODE_IS_LEAF(ref))
        {
          const bnode_t *child = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(ref));

          children_remaining = 1;

          if (i == 0)
            next = child;

          initial_accumulation = with_value
            ( with_value_context
            , initial_accumulation

            , lookup
            , LOOKUP_NODE_CVALUE(lookup, child)
            , child

            , break_iteration
            );

          if (*break_iteration)
            return initial_accumulation;
        }
      }

      if (children_remaining)
        initial_accumulation = lookup_iterate_tree_from_step
          ( lookup
          , next

          , order

          , with_value
          , with_value_context

          , initial_accumulation

          , break_iteration

          , breadth_level + 1
          , level_num << 1
          , parent_queue
          , current_queue
          );

      return initial_accumulation;
    }
  }
  /* ---------------------------------------------------------------- */
  else
  {
    /* Children and parent. */
    for (i = 0; i < ARRAY_NUM(children); ++i)
    {
      if (!BNODE_IS_LEAF(children[i]))
      {
        const bnode_t *child = LOOKUP_INDEX_CORDER(lookup, BNODE_GET_REF(children[i]));

        /* Child. */
        initial_accumulation = with_value
          ( with_value_context
          , initial_accumulation

          , lookup
          , LOOKUP_NODE_CVALUE(lookup, child)
          , child

          , break_iteration
          );

        if (*break_iteration)
          return initial_accumulation;

        /* Grandchildren. */
        initial_accumulation = lookup_iterate_tree_from_step
          ( lookup
          , child

          , order

          , with_value
          , with_value_context

          , initial_accumulation

          , break_iteration
          , 0, 0, NULL, NULL
          );

        if (*break_iteration)
          return initial_accumulation;
      }

      /* Parent. */
      if (i == 0)
      {
        initial_accumulation = with_value
          ( with_value_context
          , initial_accumulation

          , lookup
          , LOOKUP_NODE_CVALUE(lookup, node)
          , node

          , break_iteration
          );

        if (*break_iteration)
          return initial_accumulation;
      }
    }

    return initial_accumulation;
  }
}

void *lookup_iterate_tree_from
  ( const lookup_t *lookup
  , const bnode_t  *root

  , lookup_iteration_order_t order

  , lookup_iteration_callback_fun_t  with_value
    , void                          *with_value_context

  , void *initial_accumulation
  )
{
  int break_iteration;

#ifdef ERROR_CHECKING
  if (!lookup)
    return  NULL;
  if (!with_value)
    return  NULL;
#endif /* #if ERROR_CHECKING  */

  if (LOOKUP_EMPTY(lookup))
    return initial_accumulation;

  LOOKUP_OPTIONAL_CNODE(lookup, root);

  break_iteration = 0;

  if (!order.breadth_first)
  {
    initial_accumulation =
      lookup_iterate_tree_from_step
        ( lookup
        , root

        , order

        , with_value
        , with_value_context

        , initial_accumulation

        , &break_iteration
        , 0, 0, NULL, NULL
        );
  }
  else
  {
    size_t root_index;
    void *parent_queue[]  = { NULL };
    void *current_queue[2];
    root_index = LOOKUP_GET_NODE_INDEX(lookup, root);
    current_queue[0] = &root_index;
    current_queue[1] = NULL;
    initial_accumulation =
      lookup_iterate_tree_from_step
        ( lookup
        , root

        , order

        , with_value
        , with_value_context

        , initial_accumulation

        , &break_iteration
        , 0
        , 0
        , parent_queue
        , current_queue
        );
  }

  return initial_accumulation;
}

void *lookup_iterate_tree
  ( const lookup_t *lookup

  , lookup_iteration_order_t order

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context

  , void *initial_accumulation
  )
{
  return
    lookup_iterate_tree_from
      ( lookup
      , NULL

      , order

      , with_value
      , with_value_context

      , initial_accumulation
      );
}

void *lookup_iterate_values_unordered
  ( const lookup_t *lookup

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_break_iteration)
  , void *context

  , void *initial_accumulation
  )
{
  size_t i;
  size_t end;

#ifdef ERROR_CHECKING
  if (!lookup)
    return  NULL;
  if (!with_value)
    return  NULL;
#endif /* #if ERROR_CHECKING  */

  if (!LOOKUP_IS_RECYCLING(lookup))
    end = LOOKUP_LEN(lookup);
  else
    end = LOOKUP_CAPACITY(lookup) + 1;

  for (i = 0; i < end; ++i)
  {
    if (LOOKUP_GET_VALUE_IN_USE_BIT(lookup, i))
    {
      int break_iteration;

      break_iteration = 0;
      initial_accumulation = with_value(context, initial_accumulation, LOOKUP_INDEX_CVALUE(lookup, i), &break_iteration);

      if (break_iteration)
        break;
    }
  }

  return initial_accumulation;
}

static void *lookup_iterate_node_from_step
  ( const lookup_t *lookup
  , const bnode_t  *node
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, const bnode_t *node, int *out_break_iteration)
  , void *context

  , void *initial_accumulation

  , int *break_iteration
  )
{
  void *value;

  int   break_iteration_working;


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

  LOOKUP_OPTIONAL_NODE(lookup, node);

  value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));

  if (!break_iteration)
  {
    break_iteration = &break_iteration_working;
    *break_iteration = 0;
  }

  if (*break_iteration)
    return initial_accumulation;

  reverse_direction = IS_TRUE(reverse_direction);

  children[ reverse_direction] = node->left;
  children[!reverse_direction] = node->right;

  for (i = 0; i < ARRAY_NUM(children) && !*break_iteration; ++i)
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

          , break_iteration
          );

    if (i == 0 && !*break_iteration)
      accumulation = with_value(context, accumulation, value, node, break_iteration);
  }

  return accumulation;
}

void *lookup_iterate_node_from
  ( const lookup_t *lookup
  , const bnode_t  *root
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, const bnode_t *node, int *out_break_iteration)
  , void *context

  , void *initial_accumulation
  )
{
  return
    lookup_iterate_node_from_step
      ( lookup
      , root
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

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, const bnode_t *node, int *out_break_iteration)
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

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_break_iteration)
  , void *context

  , void *initial_accumulation

  , int *break_iteration
  )
{
  void *value;

  int   break_iteration_working;


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

  LOOKUP_OPTIONAL_NODE(lookup, node);

  value = LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));

  if (!break_iteration)
  {
    break_iteration = &break_iteration_working;
    *break_iteration = 0;
  }

  if (*break_iteration)
    return initial_accumulation;

  reverse_direction = IS_TRUE(reverse_direction);

  children[ reverse_direction] = node->left;
  children[!reverse_direction] = node->right;

  for (i = 0; i < ARRAY_NUM(children) && !*break_iteration; ++i)
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

          , break_iteration
          );

    if (i == 0 && !*break_iteration)
      accumulation = with_value(context, accumulation, value, break_iteration);
  }

  return accumulation;
}

void *lookup_iterate_from
  ( const lookup_t *lookup
  , const bnode_t  *root
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_break_iteration)
  , void *context

  , void *initial_accumulation
  )
{
  return
    lookup_iterate_from_step
      ( lookup
      , root
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

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_break_iteration)
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

/* ---------------------------------------------------------------- */

lookup_t *lookup_minsert
  ( lookup_t           *lookup
  , const void         *val
  , int                 add_when_exists

  , callback_compare_t  cmp

  , const memory_manager_t *memory_manager

  , size_t             *out_value_index
  , int                *out_is_duplicate
  )
{
#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  lookup = lookup_auto_resize(lookup, memory_manager);
  if (!lookup)
    return NULL;

  return
    lookup_insert
      ( lookup
      , val
      , add_when_exists

      , cmp

      , out_value_index
      , out_is_duplicate
      );
}

lookup_t *lookup_mdelete
  ( lookup_t           *lookup
  , const void         *val

  , callback_compare_t  cmp

  , const memory_manager_t *memory_manager

  , size_t             *out_num_deleted
  )
{
#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  lookup =
    lookup_delete
      ( lookup
      , val

      , cmp

      , out_num_deleted
      );
  if (!lookup)
    return NULL;

  return lookup_auto_resize(lookup, memory_manager);
}

/* ---------------------------------------------------------------- */

const void *lookup_min(lookup_t *lookup, bnode_t *root, bnode_t **out_end)
{
  bnode_t *node = root;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return NULL;

  LOOKUP_OPTIONAL_NODE(lookup, node);

  while (!BNODE_IS_LEAF(node->left))
    node = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->left));

  WRITE_OUTPUT(out_end, node);

  return LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));
}

const void *lookup_max(lookup_t *lookup, bnode_t *root, bnode_t **out_end)
{
  bnode_t *node = root;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return NULL;

  LOOKUP_OPTIONAL_NODE(lookup, node);

  while (!BNODE_IS_LEAF(node->right))
    node = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right));

  WRITE_OUTPUT(out_end, node);

  return LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));
}

const void *lookup_cmin(const lookup_t *lookup, const bnode_t *root, const bnode_t **out_end)
{
  const bnode_t *node = root;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return NULL;

  LOOKUP_OPTIONAL_CNODE(lookup, node);

  while (!BNODE_IS_LEAF(node->left))
    node = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->left));

  WRITE_OUTPUT(out_end, node);

  return LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));
}

const void *lookup_cmax(const lookup_t *lookup, const bnode_t *root, const bnode_t **out_end)
{
  const bnode_t *node = root;

#if ERROR_CHECKING
  if (!lookup)
    return NULL;
#endif /* #if ERROR_CHECKING  */

  if (lookup_empty(lookup))
    return NULL;

  LOOKUP_OPTIONAL_CNODE(lookup, node);

  while (!BNODE_IS_LEAF(node->right))
    node = LOOKUP_INDEX_ORDER(lookup, BNODE_GET_REF(node->right));

  WRITE_OUTPUT(out_end, node);

  return LOOKUP_INDEX_VALUE(lookup, BNODE_GET_VALUE(node->value));
}

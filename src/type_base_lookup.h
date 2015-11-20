/*
 * opencurry: type_base_lookup.h
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
 * type_base_lookup.h
 * ------
 *
 * Self-balancing binary search arrays.
 */

#ifndef TYPE_BASE_LOOKUP_H
#define TYPE_BASE_LOOKUP_H
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
#include "type_base_compare.h"

/* ---------------------------------------------------------------- */
/* lookup_t and children.                                           */
/* ---------------------------------------------------------------- */

#define BNODE_BLACK_BIT 0
#define BNODE_RED_BIT   1

const type_t *bnode_type(void);
extern const type_t bnode_type_def;
typedef struct bnode_s bnode_t;
struct bnode_s
{
  /* Last bit encodes node color: 0: black; 1: red. */
  size_t value;

  /* Last bit of each is reserved:                                         */
  /*  left:  whether this node is in use.                                  */
  /*  right: whether the value with the same index as this node is in use. */
  /*                                                                       */
  /* Then indices are + 1, 0 when NULL. */
  size_t left;
  size_t right;
};

/* NULL value and child references. */
#define BNODE_DEFAULTS \
  {                    \
    /* value */ 0      \
                       \
  , /* left  */ 0      \
  , /* right */ 0      \
  }
extern const bnode_t bnode_defaults;

/* ---------------------------------------------------------------- */

/* A self-balancing binary search tree. */
const type_t *lookup_type(void);
extern const type_t lookup_type_def;
typedef struct lookup_s lookup_t;
struct lookup_s
{
  typed_t type;

  /* Size of each array in terms of element sizes. */
  size_t   capacity;

  void    *values;
  size_t   value_size;
  size_t   next_value;

  bnode_t *order;
  size_t   next_order;

  size_t   len;
};

#define LOOKUP_DEFAULTS   \
  { lookup_type           \
                          \
  , /* capacity   */ 0    \
                          \
  , /* values     */ NULL \
  , /* value_size */ 0    \
                          \
  , /* order      */ NULL \
  }
extern const lookup_t lookup_defaults;

/* ---------------------------------------------------------------- */
/* bnode_t methods.                                                 */
/* ---------------------------------------------------------------- */

void bnode_init(bnode_t *node);
void bnode_init_array(bnode_t *node, size_t num);

/* ---------------------------------------------------------------- */

/* bnode_t field constructors. */

/* Construct values for the "value" field. */
#define BNODE_BLACK_VALUE( index)         ((index << 1) | (BNODE_BLACK_BIT))
#define BNODE_RED_VALUE(   index)         ((index << 1) | (BNODE_RED_BIT)  )
#define BNODE_COLORED_VALUE(index, color) ((index << 1) | (color)          )
size_t bnode_black_value  (size_t index);
size_t bnode_red_value    (size_t index);
size_t bnode_colored_value(size_t index, size_t color);

/* Construct values for the "left" and "right" fields. */
/* The final bit will still need to be set.            */
#define BNODE_LEAF()     (0)
#define BNODE_REF(index) (((index) + 1) << 1)
size_t bnode_leaf(void);
size_t bnode_ref (size_t index);

#define BNODE_SET_ORDER_IN_USE_BIT(node, bit) (node)->left  = (SET_BIT((0), (bit), ((node)->left)) )
#define BNODE_SET_VALUE_IN_USE_BIT(node, bit) (node)->right = (SET_BIT((0), (bit), ((node)->right)))
size_t bnode_set_order_in_use_bit(bnode_t *node, size_t bit);
size_t bnode_set_value_in_use_bit(bnode_t *node, size_t bit);

#define BNODE_GET_CHILD(node, ordering) \
  (SIGN_CASE(ordering, &(node)->left, &(node)->left, &(node)->right))
size_t *bnode_get_child(bnode_t *node, int ordering);

#define BNODE_LINK_SET_LEAF(link) \
  *(link) = ( ((*(link)) & 1) | ( BNODE_LEAF(       ) ) )
#define BNODE_LINK_SET_REF( link, index) \
  *(link) = ( ((*(link)) & 1) | ( BNODE_REF ((index)) ) )
size_t bnode_link_set_leaf(size_t *link);
size_t bnode_link_set_ref (size_t *link, size_t index);

#define BNODE_SET_LEAF(node, ordering) \
  BNODE_LINK_SET_LEAF( (BNODE_GET_CHILD((node), (ordering))) )
#define BNODE_SET_REF( node, ordering, index) \
  BNODE_LINK_SET_REF ( (BNODE_GET_CHILD((node), (ordering))), index )
size_t bnode_set_leaf(bnode_t *node, int ordering);
size_t bnode_set_ref (bnode_t *node, int ordering, size_t index);

/* ---------------------------------------------------------------- */

/* bnode_t field interpretors. */

#define BNODE_GET_COLOR(value) ((value) & 1)
#define BNODE_IS_BLACK( value) ((BNODE_GET_COLOR(value)) ^ (BNODE_RED_BIT))
#define BNODE_IS_RED(   value) ((BNODE_GET_COLOR(value)) ^ (BNODE_BLACK_BIT))
size_t bnode_get_color(size_t value);
int    bnode_is_black (size_t value);
int    bnode_is_red   (size_t value);

#define BNODE_GET_VALUE(value) ((value) >> 1)
size_t bnode_get_value(size_t value);

#define BNODE_IS_LEAF(encoded) (!((encoded) >> 1))
#define BNODE_GET_REF(encoded) ((((encoded) >> 1)) - 1)
int    bnode_is_leaf(size_t ref);
size_t bnode_get_ref(size_t ref);

#define BNODE_GET_ORDER_IN_USE_BIT(node) ((node)->left  & 1)
#define BNODE_GET_VALUE_IN_USE_BIT(node) ((node)->right & 1)
size_t bnode_get_order_in_use_bit(const bnode_t *node);
size_t bnode_get_value_in_use_bit(const bnode_t *node);

/* ---------------------------------------------------------------- */
/* lookup_t methods.                                                */
/* ---------------------------------------------------------------- */

void lookup_init_empty(lookup_t *lookup, size_t value_size);
void lookup_deinit
  ( lookup_t *lookup

  , void  (*free)(void *context, void *area)
  , void   *free_context
  );

/* ---------------------------------------------------------------- */

/* Capacity of the lookup tree. */
#define LOOKUP_CAPACITY(lookup) ((lookup)->capacity)
size_t lookup_capacity    (const lookup_t *lookup);
#define LOOKUP_NULL(lookup) (!(LOOKUP_CAPACITY((lookup))))
int    lookup_null        (const lookup_t *lookup);

/* Number of elements contained in the lookup tree. */
#define LOOKUP_LEN(lookup) ((lookup)->len)
size_t lookup_len         (const lookup_t *lookup);
#define LOOKUP_EMPTY(lookup) (!(LOOKUP_LEN((lookup))))
int    lookup_empty       (const lookup_t *lookup);

#define LOOKUP_MAX_CAPACITY(lookup) \
  ((LOOKUP_LEN((lookup))) >= (LOOKUP_CAPACITY((lookup))))
int    lookup_max_capacity(const lookup_t *lookup);

/* ---------------------------------------------------------------- */

#define LOOKUP_VALUE_SIZE(lookup) ((lookup)->value_size)
size_t lookup_value_size(const lookup_t *lookup);

/* ---------------------------------------------------------------- */

#define LOOKUP_SET_ORDER_IN_USE_BIT(lookup, index, bit) BNODE_SET_ORDER_IN_USE_BIT(( LOOKUP_INDEX_ORDER((lookup), (index)) ), (bit))
#define LOOKUP_SET_VALUE_IN_USE_BIT(lookup, index, bit) BNODE_SET_VALUE_IN_USE_BIT(( LOOKUP_INDEX_ORDER((lookup), (index)) ), (bit))
size_t lookup_set_order_in_use_bit(lookup_t *lookup, size_t index, size_t bit);
size_t lookup_set_value_in_use_bit(lookup_t *lookup, size_t index, size_t bit);

#define LOOKUP_GET_ORDER_IN_USE_BIT(lookup, index) (BNODE_GET_ORDER_IN_USE_BIT(( LOOKUP_INDEX_CORDER((lookup), (index))) ))
#define LOOKUP_GET_VALUE_IN_USE_BIT(lookup, index) (BNODE_GET_VALUE_IN_USE_BIT(( LOOKUP_INDEX_CORDER((lookup), (index))) ))
size_t lookup_get_order_in_use_bit(const lookup_t *lookup, size_t index);
size_t lookup_get_value_in_use_bit(const lookup_t *lookup, size_t index);

/* ---------------------------------------------------------------- */

lookup_t *lookup_expand
  ( lookup_t *lookup
  , size_t    capacity

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context
  );

/* If we're either at max capacity or recycling, then double the capacity. */
lookup_t *lookup_auto_expand
  ( lookup_t *lookup
  , int       auto_defragment

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , int *out_expanded
  , int *out_defragmented
  );

lookup_t *lookup_auto_expand_simple
  ( lookup_t *lookup
  , int       auto_defragment

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context
  );

void      lookup_defragment(lookup_t *lookup);
#ifdef TODO /* TODO */
lookup_t *lookup_shrink
  ( lookup_t *lookup
  , size_t    capacity

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , void  (*free)(void *context, void *area)
  , void   *free_context
  );

lookup_t *lookup_resize
  ( lookup_t *lookup
  , size_t    capacity

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , void  (*free)(void *context, void *area)
  , void   *free_context
  );
#endif /* #ifdef TODO /-* TODO *-/ */

/* ---------------------------------------------------------------- */

void *lookup_iterate_node_from
  ( const lookup_t *lookup
  , const bnode_t  *node
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, const bnode_t *node, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  );

void *lookup_iterate_node
  ( const lookup_t *lookup
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, const bnode_t *node, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  );

void *lookup_iterate_from
  ( const lookup_t *lookup
  , const bnode_t  *node
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  );

void *lookup_iterate
  ( const lookup_t *lookup
  , int             reverse_direction

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  );

/* ---------------------------------------------------------------- */

/* Check whether the lookup container has reached the end of its capacity and
 * is reusing previously used elements.
 *
 * Defragmenting will move memory to remove gaps.
 */
#define LOOKUP_IS_RECYCLING(lookup)    \
  (  LOOKUP_IS_VALUE_RECYCLING(lookup) \
  || LOOKUP_IS_ORDER_RECYCLING(lookup) \
  )
#define LOOKUP_IS_VALUE_RECYCLING(lookup) (((lookup)->next_value) < (LOOKUP_LEN((lookup))))
#define LOOKUP_IS_ORDER_RECYCLING(lookup) (((lookup)->next_order) < (LOOKUP_LEN((lookup))))
int lookup_is_recycling      (const lookup_t *lookup);
int lookup_is_value_recycling(const lookup_t *lookup);
int lookup_is_order_recycling(const lookup_t *lookup);

/* ---------------------------------------------------------------- */

#define LOOKUP_INDEX_VALUE(lookup, index) ((void *) (((unsigned char *) ((lookup)->values)) + ((ptrdiff_t) ((lookup)->value_size * (index)))))
#define LOOKUP_INDEX_ORDER(lookup, index) (&(lookup)->order[(index)])
#define LOOKUP_NODE_VALUE( lookup, node)  (LOOKUP_INDEX_VALUE((lookup), (BNODE_GET_VALUE((node)->value))))
void    *lookup_index_value(lookup_t *lookup, size_t         index);
bnode_t *lookup_index_order(lookup_t *lookup, size_t         index);
void    *lookup_node_value (lookup_t *lookup, const bnode_t *node);

#define LOOKUP_INDEX_CVALUE(lookup, index) ((const void *) (((const unsigned char *) ((lookup)->values)) + ((ptrdiff_t) ((lookup)->value_size * (index)))))
#define LOOKUP_INDEX_CORDER(lookup, index) ((const bnode_t *) (&(lookup)->order[(index)]))
#define LOOKUP_NODE_CVALUE( lookup, node)  (LOOKUP_INDEX_CVALUE((lookup), (BNODE_GET_VALUE((node)->value))))
const void    *lookup_index_cvalue(const lookup_t *lookup, size_t         index);
const bnode_t *lookup_index_corder(const lookup_t *lookup, size_t         index);
const void    *lookup_node_cvalue (const lookup_t *lookup, const bnode_t *node);

/* ---------------------------------------------------------------- */

#define LOOKUP_IS_VALUE_FREE(lookup, index) ((LOOKUP_GET_VALUE_IN_USE_BIT((lookup), (index))) ^ 1)
#define LOOKUP_IS_ORDER_FREE(lookup, index) ((LOOKUP_GET_ORDER_IN_USE_BIT((lookup), (index))) ^ 1)
int lookup_is_value_free(const lookup_t *lookup, size_t value);
int lookup_is_order_free(const lookup_t *lookup, size_t order);

size_t lookup_next_value(lookup_t *lookup);
size_t lookup_next_order(lookup_t *lookup);

#define LOOKUP_SET_IS_VALUE_FREE(lookup, index, bit) LOOKUP_SET_VALUE_IN_USE_BIT((lookup), (index), ((bit) ^ 1))
#define LOOKUP_SET_IS_ORDER_FREE(lookup, index, bit) LOOKUP_SET_ORDER_IN_USE_BIT((lookup), (index), ((bit) ^ 1))
size_t lookup_set_is_value_free(lookup_t *lookup, size_t index, size_t bit);
size_t lookup_set_is_order_free(lookup_t *lookup, size_t index, size_t bit);

/* ---------------------------------------------------------------- */

#define BALANCED_BTREE_MAX_NODES(height) \
  (ONE_BIT_REPEAT(((size_t) (MAX(0, (height) + 1)))))
size_t balanced_btree_max_nodes (int height);
int    balanced_btree_max_height(size_t num_nodes);

int    lookup_max_height(size_t num_nodes);

int lookup_height_from(const lookup_t *lookup, const bnode_t *node);
int lookup_height(const lookup_t *lookup);

/* ---------------------------------------------------------------- */

lookup_t *lookup_insert
  ( lookup_t           *lookup
  , const void         *val
  , int                 add_when_exists

  , callback_compare_t  cmp

  , int                *out_is_duplicate
  );

const void *lookup_retrieve
  ( const lookup_t     *lookup
  , const void         *val

  , callback_compare_t  cmp

  , const bnode_t      **out_node
  );

void *lookup_min(lookup_t *lookup);
void *lookup_max(lookup_t *lookup);

const void *lookup_cmin(const lookup_t *lookup);
const void *lookup_cmax(const lookup_t *lookup);

lookup_t *lookup_delete
  ( lookup_t           *lookup
  , const void         *val

  , callback_compare_t  cmp

  , int                *out_num_deleted
  );

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_LOOKUP_H */

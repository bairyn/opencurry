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
 * Internal use: use "mlookup" API (based on this) for coupled memory
 * management, etc.
 *
 * Self-balancing binary search arrays.
 *
 * TODO: actually self-balancing.
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
bnode_t *bnode_copy(bnode_t *dest, const bnode_t *src);

/* ---------------------------------------------------------------- */

/* bnode_t field constructors. */

/* Construct values for the "value" field. */
#define BNODE_BLACK_VALUE( index)         (((index) << 1) | (BNODE_BLACK_BIT))
#define BNODE_RED_VALUE(   index)         (((index) << 1) | (BNODE_RED_BIT)  )
#define BNODE_COLORED_VALUE(index, color) (((index) << 1) | (color)          )
size_t bnode_black_value  (size_t index);
size_t bnode_red_value    (size_t index);
size_t bnode_colored_value(size_t index, size_t color_bit);

/* The final bit will still need to be set. */
#define BNODE_VALUE(index) ((index) << 1)
size_t bnode_value(size_t index);
#define BNODE_COLOR(color_bit) (color_bit)
size_t bnode_color(size_t color_bit);

#define BNODE_SET_VALUE(node, index) \
  (node)->value = ((((node)->value) & 1) | (BNODE_VALUE((index))) )
#define BNODE_SET_COLOR(node, color_bit) \
  (node)->value = (SET_BIT(0, (color_bit), (node)->value))
#define BNODE_SET_BLACK(node) BNODE_SET_COLOR(node, BNODE_BLACK_BIT)
#define BNODE_SET_RED(  node) BNODE_SET_COLOR(node, BNODE_RED_BIT  )
size_t bnode_set_value(bnode_t *node, size_t index);
size_t bnode_set_color(bnode_t *node, size_t color_bit);
size_t bnode_set_black(bnode_t *node);
size_t bnode_set_red  (bnode_t *node);

/* Construct values for the "left" and "right" fields. */
/* The final bit will still need to be set.            */
#define BNODE_LEAF()     (0)
#define BNODE_REF(index) (((index) + 1) << 1)
size_t bnode_leaf(void);
size_t bnode_ref (size_t index);

#define BNODE_SET_ORDER_IN_USE_BIT(node, bit) (node)->left  = (SET_BIT(0, (bit), (node)->left) )
#define BNODE_SET_VALUE_IN_USE_BIT(node, bit) (node)->right = (SET_BIT(0, (bit), (node)->right))
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

size_t lookup_copy_value_buffer(void    *dest, size_t dest_size, const lookup_t *lookup, size_t start, size_t num_values);
size_t lookup_copy_order_buffer(bnode_t *dest, size_t dest_num,  const lookup_t *lookup, size_t start, size_t num_values);
lookup_t *lookup_copy
  (       lookup_t *dest
  , const lookup_t *src


  , void *(*calloc)(void *context, size_t nmemb, size_t size)
    , void   *calloc_context
  , void *(*realloc)(void *context, void *area, size_t size)
    , void   *realloc_context
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

#define DEFRAGMENT_NONE    (0)
#define DEFRAGMENT_ORDER   (1 << 0)
#define DEFRAGMENT_VALUES  (1 << 1)
#define DEFRAGMENT_ALL     ((DEFRAGMENT_ORDER) | (DEFRAGMENT_VALUES))

#define DEFRAGMENT_DEFAULT (DEFRAGMENT_ORDER)
lookup_t *lookup_defragment
  ( lookup_t *lookup
  , int       defragment_which

  , void *(*on_new_node_index) (void *on_new_order_index_context, void *last_accumulation, lookup_t *lookup, size_t new_node_index,  size_t old_node_index,  int *out_iteration_break)
  , void *on_new_node_index_context
  , void *on_new_node_index_initial_accumulation

  , void *(*on_new_value_index)(void *on_new_value_index_context, void *last_accumulation, lookup_t *lookup, size_t new_value_index, size_t old_value_index, int *out_iteration_break)
  , void *on_new_value_index_context
  , void *on_new_value_index_initial_accumulation

  , void **out_on_new_node_index_final_accumulation
  , void **out_on_new_value_index_final_accumulation
  );

lookup_t *lookup_defragment_simple
  ( lookup_t *lookup
  , int       defragment_which
  );

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

#define LOOKUP_AUTO_MIN_CAPACITY               16

/* When recycling. */
#define LOOKUP_AUTO_EXPAND_THRESHOLD(capacity) (2 * ((capacity) / 3))   /* 2/3 * capacity        */
#define LOOKUP_AUTO_EXPAND_CAPACITY( capacity) (3 * ((capacity) >> 1))  /* 3/2 * capacity (+50%) */

#define LOOKUP_AUTO_SHRINK_THRESHOLD(capacity) (capacity >> 4)          /* capacity / 8          */
#define LOOKUP_AUTO_SHRINK_CAPACITY( capacity) (LOOKUP_AUTO_EXPAND_CAPACITY((LOOKUP_AUTO_SHRINK_THRESHOLD((capacity)))))

#define LOOKUP_AUTO_DEFRAGMENT_WHICH           (DEFRAGMENT_ORDER)
lookup_t *lookup_auto_resize
  ( lookup_t *lookup

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
    , void   *calloc_context
  , void *(*realloc)(void *context, void *area, size_t size)
    , void   *realloc_context
  , void  (*free)(void *context, void *area)
    , void   *free_context
  );

typedef size_t (*lookup_capacity_fun_t)(void *context, size_t capacity);

size_t lookup_auto_min_capacity    (void *context);
size_t lookup_auto_expand_threshold(void *context, size_t capacity);
size_t lookup_auto_expand_threshold(void *context, size_t capacity);
size_t lookup_auto_shrink_threshold(void *context, size_t capacity);
size_t lookup_auto_shrink_threshold(void *context, size_t capacity);
int lookup_auto_defragment_which   (void *context);

extern void * const lookup_auto_min_capacity_context;
extern void * const lookup_auto_expand_threshold_context;
extern void * const lookup_auto_expand_capacity_context;
extern void * const lookup_auto_shrink_threshold_context;
extern void * const lookup_auto_shrink_capacity_context;
extern void * const lookup_auto_defragment_which_context;

lookup_t *lookup_auto_resize_controlled
  ( lookup_t *lookup

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
    , void   *calloc_context
  , void *(*realloc)(void *context, void *area, size_t size)
    , void   *realloc_context
  , void  (*free)(void *context, void *area)
    , void   *free_context

  , int (*min_capacity)(void *context)
    , void                  *min_capacity_context
  , lookup_capacity_fun_t  expand_threshold
    , void                  *expand_threshold_context
  , lookup_capacity_fun_t  expand_capacity
    , void                  *expand_capacity_context
  , lookup_capacity_fun_t  shrink_threshold
    , void                  *shrink_threshold_context
  , lookup_capacity_fun_t  shrink_capacity
    , void                  *shrink_capacity_context
  , size_t (*defragment_which)(void *context)
    , void                  *defragment_which_context

  , void *(*on_new_node_index) (void *on_new_order_index_context, void *last_accumulation, lookup_t *lookup, size_t new_node_index,  size_t old_node_index,  int *out_iteration_break)
    , void *on_new_node_index_context
    , void *on_new_node_index_initial_accumulation

  , void *(*on_new_value_index)(void *on_new_value_index_context, void *last_accumulation, lookup_t *lookup, size_t new_value_index, size_t old_value_index, int *out_iteration_break)
    , void *on_new_value_index_context
    , void *on_new_value_index_initial_accumulation

  , int    *out_expanding
  , int    *out_shrinking
  , int    *out_defragmenting
  , size_t *out_new_capacity

  , void  **out_on_new_node_index_final_accumulation
  , void  **out_on_new_value_index_final_accumulation
  );

/* ---------------------------------------------------------------- */

enum lookup_tree_traversal_direction_e
{
  lttd_current = 0,
  lttd_break   = 1,
  lttd_parent  = 2,
  lttd_left    = 3,
  lttd_right   = 4,

  lttd_end,

  /* 1: 0- 1 */
  /* 2: 0- 3 */
  /* 3: 0- 7 */
  /* 4: 0-15 */
  lttd_bits     = 3,
  lttd_end_mask = ONE_BIT_REPEAT(lttd_bits)
};
typedef enum lookup_tree_traversal_direction_e lookup_tree_traversal_direction_t;

extern const lookup_tree_traversal_direction_t ltd_current;
extern const lookup_tree_traversal_direction_t ltd_break;
extern const lookup_tree_traversal_direction_t ltd_parent;
extern const lookup_tree_traversal_direction_t ltd_left;
extern const lookup_tree_traversal_direction_t ltd_right;
extern const lookup_tree_traversal_direction_t ltd_end;
extern const lookup_tree_traversal_direction_t ltd_bits;
extern const lookup_tree_traversal_direction_t ltd_end_mask;

/* ---------------------------------------------------------------- */

typedef
  void *(*lookup_iteration_callback_fun_t)
            ( void *context
            , void *last_accumulation

            , const lookup_t *lookup
            , const void     *value
            , const bnode_t  *node

            , int *out_iteration_break
            );

/* N.B.: "breadth_first" recommended only for "mlookup_t" trees.  Without
 * associated memory management, a recursive function call is made for each
 * value in the tree, pushing the limits of the stack, which might overflow,
 * for primitive "lookup_t" trees.
 */
const type_t *lookup_iteration_order_type(void);
extern const type_t lookup_iteration_order_type_def;
typedef struct lookup_iteration_order_s lookup_iteration_order_t;
struct lookup_iteration_order_s
{
  typed_t type;

  unsigned int right_to_left : 1;

  unsigned int parent_first  : 1;  /* overrides leaves_first  */
  unsigned int leaves_first  : 1;  /* overrides breadth_first */
  unsigned int breadth_first : 1;
  unsigned int               : 8;
};

#define LOOKUP_ITERATION_ORDER_DEFAULTS \
  { lookup_iteration_order_type         \
                                        \
  , /* right_to_left */ 0               \
                                        \
  , /* parent_first  */ 0               \
  , /* leaves_first  */ 0               \
  , /* breadth_first */ 0               \
  }
extern const lookup_iteration_order_t lookup_iteration_order_defaults;

/* ---------------------------------------------------------------- */

/* out_iteration_break: Set to lookup_tree_traversal_direction_t. */
void *lookup_traverse_tree_from
  ( const lookup_t *lookup
  , const bnode_t  *root

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  , void                            *initial_accumulation
  );

/* out_iteration_break: Set to lookup_tree_traversal_direction_t. */
void *lookup_traverse_values_from
  ( const lookup_t *lookup
  , const bnode_t  *root

  , int right_to_left

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  , void                            *initial_accumulation
  );

typedef struct lookup_traverser_values_subtree_context_s lookup_traverser_values_subtree_context_t;
struct lookup_traverser_values_subtree_context_s 
{
  const bnode_t *root;

  callback_compare_t cmp;

  int left_to_right;

  lookup_iteration_callback_fun_t  with_value;
  void                            *with_value_context;

  int         first;
  int         moving;
  int         parent;
  const void *last_value;
};
extern const lookup_iteration_callback_fun_t lookup_traverser_values_subtree;
lookup_traverser_values_subtree_context lookup_traverser_values_subtree_initial_context
  ( const bnode_t *root

  , callback_compare_t cmp;

  , int left_to_right

  , lookup_iteration_callback_fun_t  with_value;
  , void                            *with_value_context;
  );

void *lookup_traverse_values_subtree
  ( const lookup_t *lookup
  , const bnode_t  *root

  , callback_compare_t cmp

  , int right_to_left

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  , void                            *initial_accumulation
  );

void *lookup_traverse_values
  ( const lookup_t *lookup

  , callback_compare_t cmp

  , int right_to_left

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context
  , void                            *initial_accumulation
  );

/* ---------------------------------------------------------------- */

void *lookup_iterate_tree_from
  ( const lookup_t *lookup
  , const bnode_t  *root

  , lookup_iteration_order_t order

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context

  , void *initial_accumulation
  );

void *lookup_iterate_tree
  ( const lookup_t *lookup

  , lookup_iteration_order_t order

  , lookup_iteration_callback_fun_t  with_value
  , void                            *with_value_context

  , void *initial_accumulation
  );

void *lookup_iterate_values_unordered
  ( const lookup_t *lookup

  , void *(*with_value)(void *context, void *last_accumulation, const void *value, int *out_iteration_break)
  , void *context

  , void *initial_accumulation
  );

void *lookup_iterate_node_from
  ( const lookup_t *lookup
  , const bnode_t  *root
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
  , const bnode_t  *root
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

#define LOOKUP_INDEX_VALUE(lookup, index) ((void *) (((unsigned char *) ((lookup)->values)) + ((ptrdiff_t) ((LOOKUP_VALUE_SIZE((lookup))) * (index)))))
#define LOOKUP_INDEX_ORDER(lookup, index) (&(lookup)->order[(index)])
#define LOOKUP_NODE_VALUE( lookup, node)  (LOOKUP_INDEX_VALUE((lookup), (BNODE_GET_VALUE((node)->value))))
void    *lookup_index_value(lookup_t *lookup, size_t         index);
bnode_t *lookup_index_order(lookup_t *lookup, size_t         index);
void    *lookup_node_value (lookup_t *lookup, const bnode_t *node);

#define LOOKUP_INDEX_CVALUE(lookup, index) ((const void *) (((const unsigned char *) ((lookup)->values)) + ((ptrdiff_t) ((LOOKUP_VALUE_SIZE((lookup))) * (index)))))
#define LOOKUP_INDEX_CORDER(lookup, index) ((const bnode_t *) (&(lookup)->order[(index)]))
#define LOOKUP_NODE_CVALUE( lookup, node)  (LOOKUP_INDEX_CVALUE((lookup), (BNODE_GET_VALUE((node)->value))))
const void    *lookup_index_cvalue(const lookup_t *lookup, size_t         index);
const bnode_t *lookup_index_corder(const lookup_t *lookup, size_t         index);
const void    *lookup_node_cvalue (const lookup_t *lookup, const bnode_t *node);


#define LOOKUP_ROOT_NODE(lookup)  ((      bnode_t *) (&((lookup)->order[0])) )
#define LOOKUP_ROOT_CNODE(lookup) ((const bnode_t *) (&((lookup)->order[0])) )
      bnode_t *lookup_root_node (      lookup_t *lookup);
const bnode_t *lookup_root_cnode(const lookup_t *lookup);


#define LOOKUP_GET_VALUE_INDEX(lookup, value_ref)                                                          \
  ( (size_t)                                                                                               \
    ( ( ((ptrdiff_t) (((const unsigned char *) (value_ref)) - ((const unsigned char *) (lookup)->values))) \
      / (LOOKUP_VALUE_SIZE((lookup)))                                                                      \
      )                                                                                                    \
    )                                                                                                      \
  )
#define LOOKUP_GET_NODE_INDEX(lookup, node)                    \
  ( (size_t)                                                   \
    ( (ptrdiff_t) ( ((node)) - (LOOKUP_ROOT_CNODE((lookup))) ) \
    )                                                          \
  )
size_t lookup_get_value_index(const lookup_t *lookup, const void    *value_ref);
size_t lookup_get_node_index (const lookup_t *lookup, const bnode_t *node);


#define LOOKUP_NODE_OR_ROOT(lookup, node) \
  do                                      \
  {                                       \
    if (!(node))                          \
      node = LOOKUP_ROOT_NODE(lookup);    \
  } while(0)
#define LOOKUP_NODE_OR_CROOT(lookup, node) \
  do                                       \
  {                                        \
    if (!(node))                           \
      node = LOOKUP_ROOT_CNODE(lookup);    \
  } while(0)
      bnode_t *lookup_node_or_root (      lookup_t *lookup,       bnode_t *node,       bnode_t **out_node);
const bnode_t *lookup_node_or_croot(const lookup_t *lookup, const bnode_t *node, const bnode_t **out_node);

#define LOOKUP_OPTIONAL_NODE( lookup, node) LOOKUP_NODE_OR_ROOT ((lookup), (node))
#define LOOKUP_OPTIONAL_CNODE(lookup, node) LOOKUP_NODE_OR_CROOT((lookup), (node))

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

#define LOOKUP_FIND_VARIABLE_DECLARATIONS \
  bnode_t    *grandparent;                \
  size_t     *grandparent_link;           \
  bnode_t    *parent;                     \
  size_t     *parent_link;                \
  bnode_t    *node;                       \
  size_t     *node_link;                  \
                                          \
  const void *node_val;                   \
                                          \
  int         grandparent_ordering;       \
  int         parent_ordering;            \
  int         ordering

#define LOOKUP_FIND_FROM_STD(lookup, root, val, cmp) \
  lookup_find_from                                   \
    ( lookup                                         \
    , root                                           \
    , val                                            \
                                                      \
    , cmp                                            \
                                                     \
    , &grandparent                                   \
    , &grandparent_link                              \
    , &parent                                        \
    , &parent_link                                   \
    , &node                                          \
    , &node_link                                     \
                                                     \
    , &node_val                                      \
                                                     \
    , &grandparent_ordering                          \
    , &parent_ordering                               \
    , &ordering                                      \
    )
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
  );

#define LOOKUP_CFIND_VARIABLE_DECLARATIONS \
  const bnode_t *grandparent;              \
  const size_t  *grandparent_link;         \
  const bnode_t *parent;                   \
  const size_t  *parent_link;              \
  const bnode_t *node;                     \
  const size_t  *node_link;                \
                                           \
  const void    *node_val;                 \
                                           \
  int            grandparent_ordering;     \
  int            parent_ordering;          \
  int            ordering

#define LOOKUP_CFIND_FROM_STD(lookup, root, val, cmp) \
  lookup_cfind_from                                   \
    ( lookup                                          \
    , root                                            \
    , val                                             \
                                                      \
    , cmp                                             \
                                                      \
    , &grandparent                                    \
    , &grandparent_link                               \
    , &parent                                         \
    , &parent_link                                    \
    , &node                                           \
    , &node_link                                      \
                                                      \
    , &node_val                                       \
                                                      \
    , &grandparent_ordering                           \
    , &parent_ordering                                \
    , &ordering                                       \
    )
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
  );

/* ---------------------------------------------------------------- */

lookup_t *lookup_insert
  ( lookup_t           *lookup
  , const void         *val
  , int                 add_when_exists

  , callback_compare_t  cmp

  , size_t             *out_value_index
  , int                *out_is_duplicate
  );

const void *lookup_retrieve
  ( const lookup_t     *lookup
  , const void         *val

  , callback_compare_t  cmp
  );

size_t lookup_retrieve_multiple
  ( const lookup_t     *lookup
  , const void         *val

  , callback_compare_t  cmp

  , void               *out_val
  , size_t              out_val_num_max
  );

lookup_t *lookup_delete
  ( lookup_t           *lookup
  , const void         *val

  , callback_compare_t  cmp

  , size_t             *out_num_deleted
  );

#ifdef TODO
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

lookup_t *lookup_minsert
  ( lookup_t           *lookup
  , const void         *val
  , int                 add_when_exists

  , callback_compare_t  cmp

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
    , void   *calloc_context
  , void *(*realloc)(void *context, void *area, size_t size)
    , void   *realloc_context
  , void  (*free)(void *context, void *area)
    , void   *free_context

  , size_t             *out_value_index
  , int                *out_is_duplicate
  );

lookup_t *lookup_mdelete
  ( lookup_t           *lookup
  , const void         *val

  , callback_compare_t  cmp

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
    , void   *calloc_context
  , void *(*realloc)(void *context, void *area, size_t size)
    , void   *realloc_context
  , void  (*free)(void *context, void *area)
    , void   *free_context

  , size_t             *out_num_deleted
  );

/* ---------------------------------------------------------------- */

const void *lookup_min(lookup_t *lookup, bnode_t *root, bnode_t **out_end);
const void *lookup_max(lookup_t *lookup, bnode_t *root, bnode_t **out_end);

const void *lookup_cmin(const lookup_t *lookup, const bnode_t *root, const bnode_t **out_end);
const void *lookup_cmax(const lookup_t *lookup, const bnode_t *root, const bnode_t **out_end);

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_LOOKUP_H */

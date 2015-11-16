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

  /* Indices are + 1, 0 when NULL. */
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

  bnode_t *order;

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

void bnode_init(bnode_t *bnode);
void bnode_init_array(bnode_t *bnode, size_t num);

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
#define BNODE_LEAF()       (0)
#define BNODE_CHILD(index) ((index) + 1)
size_t bnode_leaf (void);
size_t bnode_child(size_t index);

/* ---------------------------------------------------------------- */

/* bnode_t field interpretors. */

#define BNODE_GET_COLOR(value) ((value) & 1)
#define BNODE_IS_BLACK(value) ((BNODE_GET_COLOR(value)) ^ (BNODE_RED_BIT))
#define BNODE_IS_RED(  value) ((BNODE_GET_COLOR(value)) ^ (BNODE_BLACK_BIT))
size_t bnode_get_color(size_t value);
int    bnode_is_black (size_t value);
int    bnode_is_red   (size_t value);

#define BNODE_GET_VALUE(value) ((value) >> 1)
size_t bnode_get_value(size_t value);

#define BNODE_IS_LEAF(  ref) (!(ref))
#define BNODE_GET_CHILD(ref) ((ref) - 1)
int    bnode_is_leaf  (size_t ref);
size_t bnode_get_child(size_t ref);

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
int    lookup_null        (const lookup_t *lookup);

/* Number of elements contained in the lookup tree. */
#define LOOKUP_LEN(lookup) ((lookup)->len)
size_t lookup_len         (const lookup_t *lookup);
int    lookup_empty       (const lookup_t *lookup);

int    lookup_max_capacity(const lookup_t *lookup);

/* ---------------------------------------------------------------- */

#define LOOKUP_VALUE_SIZE(lookup) ((lookup)->value_size)
size_t lookup_value_size(const lookup_t *lookup);

/* ---------------------------------------------------------------- */

lookup_t *lookup_expand
  ( lookup_t *lookup
  , size_t    capacity

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context
  );

#ifdef TODO /* TODO */
void      lookup_defragment(lookup_t *lookup);
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

#define LOOKUP_INDEX_VALUE(lookup, index) ((void *) (((unsigned char *) ((lookup)->values)) + ((ptrdiff_t) ((lookup)->value_size * (index)))))
#define LOOKUP_INDEX_ORDER(lookup, index) (&(lookup)->order[(index)])
void    *lookup_index_value(lookup_t *lookup, size_t index);
bnode_t *lookup_index_order(lookup_t *lookup, size_t index);

/* ---------------------------------------------------------------- */

lookup_t *lookup_insert_controlled
  ( lookup_t           *lookup
  , const void         *val
  , int                 add_when_exists

  , callback_compare_t  cmp

  , int                *out_already_exists
  , int                *out_max_capacity
  );

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_LOOKUP_H */

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
 *   - size_t
 */
#include <stddef.h>

#include "base.h"

/* ---------------------------------------------------------------- */
/* Dependencies.                                                    */
/* ---------------------------------------------------------------- */

#include "type_base_prim.h"
#include "type_base_typed.h"

/* ---------------------------------------------------------------- */
/* lookup_t and children.                                           */
/* ---------------------------------------------------------------- */

const type_t *bnode_type(void);
extern const type_t bnode_type_def;
typedef struct bnode_s bnode_t;
struct bnode_s
{
  /* Indices are + 1, 0 when NULL. */

  size_t value;

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

/* A self-balancing binary search tree. */
const type_t *lookup_type(void);
extern const type_t lookup_type_def;
typedef struct lookup_s lookup_t;
struct lookup_s
{
  typed_t type;

  /* Size of each array in terms of element sizes. */
  size_t   num;

  void    *values;
  size_t   value_size;

  bnode_t *order;
};

#define LOOKUP_DEFAULTS   \
  { lookup_type           \
                          \
  , /* num        */ 0    \
                          \
  , /* values     */ NULL \
  , /* value_size */ 0    \
                          \
  , /* order      */ NULL \
  }
extern const lookup_t lookup_defaults;

/* ---------------------------------------------------------------- */
/* lookup_t methods.                                                */
/* ---------------------------------------------------------------- */

void      lookup_init_empty(lookup_t       *lookup, size_t value_size);

void      lookup_deinit
  ( lookup_t *lookup

  , void  (*free)(void *context, void *area)
  , void   *free_context
  );

size_t    lookup_num       (const lookup_t *lookup);
lookup_t *lookup_expand
  ( lookup_t *lookup
  , size_t    num

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context
  );

#ifdef TODO /* TODO */
void      lookup_defragment(lookup_t *lookup);
lookup_t *lookup_shrink
  ( lookup_t *lookup
  , size_t    num

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , void  (*free)(void *context, void *area)
  , void   *free_context
  );

lookup_t *lookup_resize
  ( lookup_t *lookup
  , size_t    num

  , void *(*calloc)(void *context, size_t nmemb, size_t size)
  , void   *calloc_context

  , void *(*realloc)(void *context, void *area, size_t size)
  , void   *realloc_context

  , void  (*free)(void *context, void *area)
  , void   *free_context
  );
#endif /* #ifdef TODO /-* TODO *-/ */

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_LOOKUP_H */

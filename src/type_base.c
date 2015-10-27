/*
 * opencurry: type_base.c
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

/* stdlib.h:
 *   , calloc
 *   - free
 *   - malloc
 *   , realloc
 */
#include <stdlib.h>

#include "base.h"
#include "type_base.h"

/* ---------------------------------------------------------------- */
/* tval                                                             */
/* ---------------------------------------------------------------- */

/* TODO:
const type_t *typed_type(void);
*/

/* ---------------------------------------------------------------- */
/* Memory managers.                                                 */
/* ---------------------------------------------------------------- */

/* memory_manager type. */

static const type_t        *memory_manager_type_typed    (const type_t *self);
static const char          *memory_manager_type_name     (const type_t *self);
static const char          *memory_manager_type_info     (const type_t *self);
static size_t               memory_manager_type_size     (const type_t *self, const tval *val);
static const struct_info_t *memory_manager_type_is_struct(const type_t *self);
static typed_t              memory_manager_type_cons_type(const type_t *self);
static tval                *memory_manager_type_init     (const type_t *self, tval *cons);
static void                 memory_manager_type_free     (const type_t *self, tval *val);
static tval                *memory_manager_type_dup      (const type_t *self, tval *dest, const tval *src, int rec);

const type_t memory_manager_type_def =
  { type_type

    /* memory_tracker_default */
  , /* memory    */ { memory_tracker_type, { memory_manager_type } }

  , /* typed     */ memory_manager_type_typed

  , /* name      */ memory_manager_type_name
  , /* info      */ memory_manager_type_info
  , /* size      */ memory_manager_type_size
  , /* is_struct */ memory_manager_type_is_struct

  , /* cons_type */ memory_manager_type_cons_type
  , /* init      */ memory_manager_type_init
  , /* free      */ memory_manager_type_free
  , /* dup       */ memory_manager_type_dup

  , /* parity    */ ""
  };

const type_t *memory_manager_type(void)
  { return &memory_manager_type_def; }

static const type_t        *memory_manager_type_typed    (const type_t *self)
  { return type_typed(self); }
static const char          *memory_manager_type_name     (const type_t *self)
  { return "memory_manager"; }

static const char          *memory_manager_type_info     (const type_t *self)
  { return "typedef struct memory_manager_s memory_manager_t"; }

static size_t               memory_manager_type_size     (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *memory_manager_type_is_struct(const type_t *self)
  {
    static const memory_manager_t empty;

    static const struct_info_t struct_info =
    { struct_info_type

    , {
      /* typed_t type */
        { field_info_type
        , offsetof(memory_manager_t, type)
        , sizeof(empty.type)
        , /* TODO */ NULL

        , NULL
        }

      /* void *(*malloc) (const memory_manager_t *self, size_t  size); */
      , { field_info_type
        , offsetof(memory_manager_t, malloc)
        , sizeof(empty.malloc)
        , /* TODO */ NULL

        , NULL
        }

      /* void  (*free)   (const memory_manager_t *self, void   *ptr); */
      , { field_info_type
        , offsetof(memory_manager_t, free)
        , sizeof(empty.free)
        , /* TODO */ NULL

        , NULL
        }

      /* void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size); */
      , { field_info_type
        , offsetof(memory_manager_t, calloc)
        , sizeof(empty.calloc)
        , /* TODO */ NULL

        , NULL
        }

      /* void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size); */
      , { field_info_type
        , offsetof(memory_manager_t, realloc)
        , sizeof(empty.realloc)
        , /* TODO */ NULL

        , NULL
        }

      /* void   *state; */
      , { field_info_type
        , offsetof(memory_manager_t, state)
        , sizeof(empty.state)
        , /* TODO */ NULL

        , NULL
        }

      /* size_t  state_size; */
      , { field_info_type
        , offsetof(memory_manager_t, state_size)
        , sizeof(empty.state_size)
        , /* TODO */ NULL

        , NULL
        }
      }
    , 7

    , NULL

    /* No memory tracker field. */
    , 0
    , 0
    };

    return &struct_info;
  }

static typed_t              memory_manager_type_cons_type(const type_t *self)
  { return template_cons_type; }

static tval                *memory_manager_type_init     (const type_t *self, tval *cons)
  {
    static memory_manager_t *defaults = NULL;
    if (!defaults)
    {
      static memory_manager_t memory_manager_defaults;

      memory_manager_defaults = default_manager;

      defaults = &memory_manager_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 memory_manager_type_free     (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static tval                *memory_manager_type_dup      (const type_t *self, tval *dest, const tval *src, int rec)
  {
    struct_dup(self->is_struct(self), dest, src);

    if (rec)
    {
      if (rec < 0)
        ++rec;

      /* Recursively dup select fields. */
    }

    return dest;
  }

/* ---------------------------------------------------------------- */

static void *malloc_manager_malloc (const memory_manager_t *self, size_t  size);
static void  malloc_manager_free   (const memory_manager_t *self, void   *ptr);
static void *malloc_manager_calloc (const memory_manager_t *self, size_t  nmemb, size_t size);
static void *malloc_manager_realloc(const memory_manager_t *self, void   *ptr,   size_t size);

const memory_manager_t malloc_manager =
  { memory_manager_type

  , malloc_manager_malloc
  , malloc_manager_free
  , malloc_manager_calloc
  , malloc_manager_realloc

  , NULL
  , 0
  };

void *malloc_manager_malloc (const memory_manager_t *self, size_t  size)
  { return malloc (size); }
void  malloc_manager_free   (const memory_manager_t *self, void   *ptr)
  {        free   (ptr); }
void *malloc_manager_calloc (const memory_manager_t *self, size_t  nmemb, size_t size)
  { return calloc (nmemb, size); }
void *malloc_manager_realloc(const memory_manager_t *self, void   *ptr,   size_t size)
  { return realloc(ptr,   size); }

/* ---------------------------------------------------------------- */

const memory_manager_t default_manager =
  { memory_manager_type

  , NULL
  , NULL
  , NULL
  , NULL

  , NULL
  , 0
  };

/* ---------------------------------------------------------------- */

void *memory_manager_malloc(const memory_manager_t *memory_manager, size_t size)
{
  if (!memory_manager || !memory_manager->malloc)
    memory_manager = &malloc_manager;

  return memory_manager->malloc(memory_manager, size);
}

void memory_manager_free(const memory_manager_t *memory_manager, void *ptr)
{
  if (!memory_manager || !memory_manager->free)
    memory_manager = &malloc_manager;

  memory_manager->free(memory_manager, ptr);
}

void *memory_manager_calloc(const memory_manager_t *memory_manager, size_t nmemb, size_t size)
{
  if (!memory_manager)
    memory_manager = &malloc_manager;

  if (memory_manager->calloc)
  {
    return memory_manager->calloc(memory_manager, nmemb, size);
  }
  else
  {
    size_t total_size;

    total_size = nmemb * size;

    return memory_manager_malloc(memory_manager, total_size);
  }
}

void *memory_manager_realloc(const memory_manager_t *memory_manager, void *ptr, size_t size)
{
  if (!memory_manager)
    memory_manager = &malloc_manager;

  if (memory_manager->realloc)
  {
    return memory_manager->realloc(memory_manager, ptr, size);
  }
  else
  {
    if (!memory_manager->malloc || !memory_manager->free)
      memory_manager = &malloc_manager;

    memory_manager_free(memory_manager, ptr);

    return memory_manager_malloc(memory_manager, size);
  }
}

/* ---------------------------------------------------------------- */

/*
 * Default memory tracker:
 *   - Default memory manager.
 *   - Associated value not allocated on heap.
 *   - No allocated buffers to track dynamically allocated memory.
 */
const memory_tracker_t memory_tracker_default =
  { memory_tracker_type

  /* default_manager */
  , { memory_manager_type }


  , 0

  , NULL
  , 0
  , 0
  };

/* ---------------------------------------------------------------- */
/* struct_info_t and field_info_t                                   */
/* ---------------------------------------------------------------- */

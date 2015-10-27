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

  , /* user      */ NULL

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
  { return sizeof(memory_manager_t); }

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

        , 0
        , NULL
        }

      /* void *(*malloc) (const memory_manager_t *self, size_t  size); */
      , { field_info_type
        , offsetof(memory_manager_t, malloc)
        , sizeof(empty.malloc)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* void  (*free)   (const memory_manager_t *self, void   *ptr); */
      , { field_info_type
        , offsetof(memory_manager_t, free)
        , sizeof(empty.free)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size); */
      , { field_info_type
        , offsetof(memory_manager_t, calloc)
        , sizeof(empty.calloc)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size); */
      , { field_info_type
        , offsetof(memory_manager_t, realloc)
        , sizeof(empty.realloc)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* void   *state; */
      , { field_info_type
        , offsetof(memory_manager_t, state)
        , sizeof(empty.state)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* size_t  state_size; */
      , { field_info_type
        , offsetof(memory_manager_t, state_size)
        , sizeof(empty.state_size)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* terminating_field_info: end of fields. */
      , { field_info_type, (size_t) (-1) }
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
    struct_dup(self->is_struct(self), dest, src, rec);

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

/* memory_tracker type. */

static const type_t        *memory_tracker_type_typed    (const type_t *self);
static const char          *memory_tracker_type_name     (const type_t *self);
static const char          *memory_tracker_type_info     (const type_t *self);
static size_t               memory_tracker_type_size     (const type_t *self, const tval *val);
static const struct_info_t *memory_tracker_type_is_struct(const type_t *self);
static typed_t              memory_tracker_type_cons_type(const type_t *self);
static tval                *memory_tracker_type_init     (const type_t *self, tval *cons);
static void                 memory_tracker_type_free     (const type_t *self, tval *val);
static tval                *memory_tracker_type_dup      (const type_t *self, tval *dest, const tval *src, int rec);

const type_t memory_tracker_type_def =
  { type_type

    /* memory_tracker_default */
  , /* memory    */ { memory_tracker_type, { memory_manager_type } }

  , /* typed     */ memory_tracker_type_typed

  , /* name      */ memory_tracker_type_name
  , /* info      */ memory_tracker_type_info
  , /* size      */ memory_tracker_type_size
  , /* is_struct */ memory_tracker_type_is_struct

  , /* cons_type */ memory_tracker_type_cons_type
  , /* init      */ memory_tracker_type_init
  , /* free      */ memory_tracker_type_free
  , /* dup       */ memory_tracker_type_dup

  , /* user      */ NULL

  , /* parity    */ ""
  };

const type_t *memory_tracker_type(void)
  { return &memory_tracker_type_def; }

static const type_t        *memory_tracker_type_typed    (const type_t *self)
  { return type_typed(self); }

static const char          *memory_tracker_type_name     (const type_t *self)
  { return "memory_tracker"; }

static const char          *memory_tracker_type_info     (const type_t *self)
  { return "typedef struct memory_tracker_s memory_tracker_t"; }

static size_t               memory_tracker_type_size     (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *memory_tracker_type_is_struct(const type_t *self)
  {
    static const memory_tracker_t empty;

    static const struct_info_t struct_info =
    { struct_info_type

    , {
      /* typed_t type */
        { field_info_type
        , offsetof(memory_tracker_t, type)
        , sizeof(empty.type)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* memory_manager_t memory_manager; */
      , { field_info_type
        , offsetof(memory_tracker_t, memory_manager)
        , sizeof(empty.memory_manager)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* int      is_heap_allocated; */
      , { field_info_type
        , offsetof(memory_tracker_t, is_heap_allocated)
        , sizeof(empty.is_heap_allocated)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* void   **dynamically_allocated_buffers; */
      , { field_info_type
        , offsetof(memory_tracker_t, dynamically_allocated_buffers)
        , sizeof(empty.dynamically_allocated_buffers)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* size_t   dynamically_allocated_buffers_num; */
      , { field_info_type
        , offsetof(memory_tracker_t, dynamically_allocated_buffers_num)
        , sizeof(empty.dynamically_allocated_buffers_num)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* size_t   dynamically_allocated_buffers_size; */
      , { field_info_type
        , offsetof(memory_tracker_t, dynamically_allocated_buffers_size)
        , sizeof(empty.dynamically_allocated_buffers_size)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* terminating_field_info: end of fields. */
      , { field_info_type, (size_t) (-1) }
      }
    , 6

    , NULL

    /* No memory tracker field. */
    , 0
    , 0
    };

    return &struct_info;
  }

static typed_t              memory_tracker_type_cons_type(const type_t *self)
  { return template_cons_type; }

static tval                *memory_tracker_type_init     (const type_t *self, tval *cons)
  {
    static memory_tracker_t *defaults = NULL;
    if (!defaults)
    {
      static memory_tracker_t memory_tracker_defaults;

      memory_tracker_defaults = memory_tracker_default;

      defaults = &memory_tracker_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 memory_tracker_type_free     (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static tval                *memory_tracker_type_dup      (const type_t *self, tval *dest, const tval *src, int rec)
  {
    struct_dup(self->is_struct(self), dest, src, rec);

    return dest;
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

/* field_info type. */

static const type_t        *field_info_type_typed    (const type_t *self);
static const char          *field_info_type_name     (const type_t *self);
static const char          *field_info_type_info     (const type_t *self);
static size_t               field_info_type_size     (const type_t *self, const tval *val);
static const struct_info_t *field_info_type_is_struct(const type_t *self);
static typed_t              field_info_type_cons_type(const type_t *self);
static tval                *field_info_type_init     (const type_t *self, tval *cons);
static void                 field_info_type_free     (const type_t *self, tval *val);
static tval                *field_info_type_dup      (const type_t *self, tval *dest, const tval *src, int rec);

const type_t field_info_type_def =
  { type_type

    /* memory_tracker_default */
  , /* memory    */ { memory_tracker_type, { memory_manager_type } }

  , /* typed     */ field_info_type_typed

  , /* name      */ field_info_type_name
  , /* info      */ field_info_type_info
  , /* size      */ field_info_type_size
  , /* is_struct */ field_info_type_is_struct

  , /* cons_type */ field_info_type_cons_type
  , /* init      */ field_info_type_init
  , /* free      */ field_info_type_free
  , /* dup       */ field_info_type_dup

  , /* user      */ NULL

  , /* parity    */ ""
  };

const type_t *field_info_type(void)
  { return &field_info_type_def; }

static const type_t        *field_info_type_typed    (const type_t *self)
  { return type_typed(self); }

static const char          *field_info_type_name     (const type_t *self)
  { return "field_info"; }

static const char          *field_info_type_info     (const type_t *self)
  { return "typedef struct field_info_s field_info_t"; }

static size_t               field_info_type_size     (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *field_info_type_is_struct(const type_t *self)
  {
    static const field_info_t empty;

    static const struct_info_t struct_info =
    { struct_info_type

    , {
      /* typed_t type */
        { field_info_type
        , offsetof(field_info_t, type)
        , sizeof(empty.type)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* size_t        field_pos; */
      , { field_info_type
        , offsetof(field_info_t, field_pos)
        , sizeof(empty.field_pos)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* size_t        field_size; */
      , { field_info_type
        , offsetof(field_info_t, field_size)
        , sizeof(empty.field_size)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* const type_t *field_type; */
      , { field_info_type
        , offsetof(field_info_t, field_type)
        , sizeof(empty.field_type)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* int           is_recurse; */
      , { field_info_type
        , offsetof(field_info_t, is_recurse)
        , sizeof(empty.is_recurse)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* int (*initializer_check_default)(field_info_t *self, void *dest_mem, const void *src_mem); */
      , { field_info_type
        , offsetof(field_info_t, initializer_check_default)
        , sizeof(empty.initializer_check_default)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* terminating_field_info: end of fields. */
      , { field_info_type, (size_t) (-1) }
      }
    , 6

    , NULL

    /* No memory tracker field. */
    , 0
    , 0
    };

    return &struct_info;
  }

static typed_t              field_info_type_cons_type(const type_t *self)
  { return template_cons_type; }

static tval                *field_info_type_init     (const type_t *self, tval *cons)
  {
    static field_info_t *defaults = NULL;
    if (!defaults)
    {
      static field_info_t field_info_defaults;

      field_info_defaults = default_field_info;

      defaults = &field_info_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 field_info_type_free     (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static tval                *field_info_type_dup      (const type_t *self, tval *dest, const tval *src, int rec)
  {
    struct_dup(self->is_struct(self), dest, src, rec);

    return dest;
  }

const field_info_t default_field_info =
  { field_info_type

  , 0
  , 0
  , NULL /* TODO: void_type? */

  , 0
  , NULL
  };

/* ---------------------------------------------------------------- */

/* struct_info type. */

static const type_t        *struct_info_type_typed    (const type_t *self);
static const char          *struct_info_type_name     (const type_t *self);
static const char          *struct_info_type_info     (const type_t *self);
static size_t               struct_info_type_size     (const type_t *self, const tval *val);
static const struct_info_t *struct_info_type_is_struct(const type_t *self);
static typed_t              struct_info_type_cons_type(const type_t *self);
static tval                *struct_info_type_init     (const type_t *self, tval *cons);
static void                 struct_info_type_free     (const type_t *self, tval *val);
static tval                *struct_info_type_dup      (const type_t *self, tval *dest, const tval *src, int rec);

const type_t struct_info_type_def =
  { type_type

    /* memory_tracker_default */
  , /* memory    */ { memory_tracker_type, { memory_manager_type } }

  , /* typed     */ struct_info_type_typed

  , /* name      */ struct_info_type_name
  , /* info      */ struct_info_type_info
  , /* size      */ struct_info_type_size
  , /* is_struct */ struct_info_type_is_struct

  , /* cons_type */ struct_info_type_cons_type
  , /* init      */ struct_info_type_init
  , /* free      */ struct_info_type_free
  , /* dup       */ struct_info_type_dup

  , /* user      */ NULL

  , /* parity    */ ""
  };

const type_t *struct_info_type(void)
  { return &struct_info_type_def; }

static const type_t        *struct_info_type_typed    (const type_t *self)
  { return type_typed(self); }

static const char          *struct_info_type_name     (const type_t *self)
  { return "struct_info"; }

static const char          *struct_info_type_info     (const type_t *self)
  { return "typedef struct struct_info_s struct_info_t"; }

static size_t               struct_info_type_size     (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *struct_info_type_is_struct(const type_t *self)
  {
    static const struct_info_t empty;

    static const struct_info_t struct_info =
    { struct_info_type

    , {
      /* typed_t type */
        { struct_info_type
        , offsetof(struct_info_t, type)
        , sizeof(empty.type)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* field_info_t fields[STRUCT_INFO_NUM_FIELDS]; */
      , { struct_info_type
        , offsetof(struct_info_t, fields)
        , sizeof(empty.fields)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* size_t       fields_len; */
      , { struct_info_type
        , offsetof(struct_info_t, fields_len)
        , sizeof(empty.fields_len)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* struct_info_t *tail; */
      , { struct_info_type
        , offsetof(struct_info_t, tail)
        , sizeof(empty.tail)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* int       has_memory_tracker; */
      , { struct_info_type
        , offsetof(struct_info_t, has_memory_tracker)
        , sizeof(empty.has_memory_tracker)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* size_t    memory_tracker_field; */
      , { struct_info_type
        , offsetof(struct_info_t, memory_tracker_field)
        , sizeof(empty.memory_tracker_field)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* terminating_field_info: end of fields. */
      , { field_info_type, (size_t) (-1) }
      }
    , 6

    , NULL

    /* No memory tracker field. */
    , 0
    , 0
    };

    return &struct_info;
  }

static typed_t              struct_info_type_cons_type(const type_t *self)
  { return template_cons_type; }

static tval                *struct_info_type_init     (const type_t *self, tval *cons)
  {
    static struct_info_t *defaults = NULL;
    if (!defaults)
    {
      static struct_info_t struct_info_defaults;

      struct_info_defaults = default_struct_info;

      defaults = &struct_info_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 struct_info_type_free     (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static tval                *struct_info_type_dup      (const type_t *self, tval *dest, const tval *src, int rec)
  {
    struct_dup(self->is_struct(self), dest, src, rec);

    return dest;
  }

const struct_info_t default_struct_info =
  { struct_info_type

  , { { field_info_type, (size_t) (-1) } }
  , 0

  , NULL

  , 0
  , 0
  };

const field_info_t terminating_field_info =
  { field_info_type

  , (size_t) (-1)
  , 0
  , NULL /* TODO: void_type? */

  , 0
  , NULL
  };

/* ---------------------------------------------------------------- */
/* type_t                                                           */
/* ---------------------------------------------------------------- */

/* type type. */

static const type_t        *type_type_typed    (const type_t *self);
static const char          *type_type_name     (const type_t *self);
static const char          *type_type_info     (const type_t *self);
static size_t               type_type_size     (const type_t *self, const tval *val);
static const struct_info_t *type_type_is_struct(const type_t *self);
static typed_t              type_type_cons_type(const type_t *self);
static tval                *type_type_init     (const type_t *self, tval *cons);
static void                 type_type_free     (const type_t *self, tval *val);
static tval                *type_type_dup      (const type_t *self, tval *dest, const tval *src, int rec);

const type_t type_type_def =
  { type_type

    /* memory_tracker_default */
  , /* memory    */ { memory_tracker_type, { memory_manager_type } }

  , /* typed     */ type_type_typed

  , /* name      */ type_type_name
  , /* info      */ type_type_info
  , /* size      */ type_type_size
  , /* is_struct */ type_type_is_struct

  , /* cons_type */ type_type_cons_type
  , /* init      */ type_type_init
  , /* free      */ type_type_free
  , /* dup       */ type_type_dup

  , /* user      */ NULL

  , /* parity    */ ""
  };

const type_t *type_type(void)
  { return &type_type_def; }

static const type_t        *type_type_typed    (const type_t *self)
  { return type_typed(self); }
static const char          *type_type_name     (const type_t *self)
  { return "type"; }

static const char          *type_type_info     (const type_t *self)
  { return "typedef struct type_s type_t"; }

static size_t               type_type_size     (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

static const struct_info_t *type_type_is_struct(const type_t *self)
  {
    static const type_t empty;

    static const struct_info_t struct_info =
    { struct_info_type

    , {
      /* typed_t type */
        { type_type
        , offsetof(type_t, type)
        , sizeof(empty.type)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* memory_tracker_t memory; */
      , { type_type
        , offsetof(type_t, memory)
        , sizeof(empty.memory)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* const type_t        *(*typed)    (const type_t *self); */
      , { type_type
        , offsetof(type_t, typed)
        , sizeof(empty.typed)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* const char          *(*name)     (const type_t *self); */
      , { type_type
        , offsetof(type_t, name)
        , sizeof(empty.name)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* const char          *(*info)     (const type_t *self); */
      , { type_type
        , offsetof(type_t, info)
        , sizeof(empty.info)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* size_t               (*size)     (const type_t *self, const tval *val); */
      , { type_type
        , offsetof(type_t, size)
        , sizeof(empty.size)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* const struct_info_t *(*is_struct)(const type_t *self); */
      , { type_type
        , offsetof(type_t, is_struct)
        , sizeof(empty.is_struct)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* typed_t              (*cons_type)(const type_t *self); */
      , { type_type
        , offsetof(type_t, cons_type)
        , sizeof(empty.cons_type)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* tval                *(*init)     (const type_t *self, tval *cons); */
      , { type_type
        , offsetof(type_t, init)
        , sizeof(empty.init)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* void                 (*free)     (const type_t *self, tval *val); */
      , { type_type
        , offsetof(type_t, free)
        , sizeof(empty.free)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* tval                *(*dup)      (const type_t *self, tval *dest, const tval *src, int rec); */
      , { type_type
        , offsetof(type_t, dup)
        , sizeof(empty.dup)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* void *user; */
      , { type_type
        , offsetof(type_t, user)
        , sizeof(empty.user)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* const char *parity; */
      , { type_type
        , offsetof(type_t, parity)
        , sizeof(empty.parity)
        , /* TODO */ NULL

        , 0
        , NULL
        }

      /* terminating_field_info: end of fields. */
      , { field_info_type, (size_t) (-1) }
      }
    , 13

    , NULL

    /* No memory tracker field. */
    , 1
    , 1
    };

    return &struct_info;
  }

static typed_t              type_type_cons_type(const type_t *self)
  { return template_cons_type; }

static tval                *type_type_init     (const type_t *self, tval *cons)
  {
    static type_t *defaults = NULL;
    if (!defaults)
    {
      static type_t type_defaults;

      type_defaults = default_type;

      defaults = &type_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 type_type_free     (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static tval                *type_type_dup      (const type_t *self, tval *dest, const tval *src, int rec)
  {
    struct_dup(self->is_struct(self), dest, src, rec);

    return dest;
  }

/* ---------------------------------------------------------------- */

static const type_t        *default_type_typed    (const type_t *self);
/* static const char          *default_type_name     (const type_t *self);                  */
/* static const char          *default_type_info     (const type_t *self);                  */
/* static size_t               default_type_size     (const type_t *self, const tval *val); */
/* static const struct_info_t *default_type_is_struct(const type_t *self);                  */
static typed_t              default_type_cons_type(const type_t *self);
/* static tval                *default_type_init     (const type_t *self, tval *cons);      */
static void                 default_type_free     (const type_t *self, tval *val);
static tval                *default_type_dup      (const type_t *self, tval *dest, const tval *src, int rec);

const type_t default_type =
  { type_type

    /* memory_tracker_default */
  , /* memory    */ { memory_tracker_type, { memory_manager_type } }

  , /* typed     */ default_type_typed

  , /* name      */ NULL
  , /* info      */ NULL
  , /* size      */ NULL
  , /* is_struct */ NULL

  , /* cons_type */ default_type_cons_type
  , /* init      */ NULL
  , /* free      */ default_type_free
  , /* dup       */ default_type_dup

  , /* user      */ NULL

  , /* parity    */ ""
  };

static const type_t        *default_type_typed    (const type_t *self)
  { return type_typed(self); }

/*
static const char          *default_type_name     (const type_t *self)
  { return "this"; }

static const char          *default_type_info     (const type_t *self)
  { return "typedef struct this_s this_t"; }

static size_t               default_type_size     (const type_t *self, const tval *val)
  { return sizeof(this_t); }

static const struct_info_t *default_type_is_struct(const type_t *self)
  {
    static const this_t empty;

    static const struct_info_t struct_info =
    { struct_info_type

    , {
      /-* typed_t type *-/
        { field_info_type
        , offsetof(this_t, type)
        , sizeof(empty.type)
        , /-* TODO *-/ NULL

        , 0
        , NULL
        }

      /-* void field0 *-/
      , { field_info_type
        , offsetof(this_t, field0)
        , sizeof(empty.field0)
        , /-* TODO *-/ NULL

        , 0
        , NULL
        }

      /-* void field1 *-/
      , { field_info_type
        , offsetof(this_t, field1)
        , sizeof(empty.field1)
        , /-* TODO *-/ NULL

        , 0
        , NULL
        }

      /-* terminating_field_info: end of fields. *-/
      , { field_info_type, (size_t) (-1) }
      }
    , 3

    , NULL

    /-* No memory tracker field. *-/
    , 0
    , 0
    };

    return &struct_info;
  }
*/

static typed_t              default_type_cons_type(const type_t *self)
  { return template_cons_type; }

/*
static tval                *default_type_init     (const type_t *self, tval *cons)
  {
    static default_t *defaults = NULL;
    if (!defaults)
    {
      static default_t this_defaults;

      this_defaults = default_this;

      defaults = &this_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }
*/

static void                 default_type_free     (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static tval                *default_type_dup      (const type_t *self, tval *dest, const tval *src, int rec)
  {
    struct_dup(self->is_struct(self), dest, src, rec);

    return dest;
  }

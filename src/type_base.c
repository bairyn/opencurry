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

/*
 * stdio.h:
 *  - fprintf
 *  - stderr
 */
#include <stdio.h>

/* stdlib.h:
 *   , calloc
 *   - free
 *   - malloc
 *   , realloc
 */
#include <stdlib.h>

/* string.h:
 *   - memset
 *   - strlen
 */
#include <string.h>

#include "base.h"
#include "type_base.h"

#include "util.h"

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

static const type_t        *memory_manager_type_typed      (const type_t *self);
static const char          *memory_manager_type_name       (const type_t *self);
static const char          *memory_manager_type_info       (const type_t *self);
static size_t               memory_manager_type_size       (const type_t *self, const tval *val);
static const struct_info_t *memory_manager_type_is_struct  (const type_t *self);
static typed_t              memory_manager_type_cons_type  (const type_t *self);
static tval                *memory_manager_type_init       (const type_t *self, tval *cons);
static void                 memory_manager_type_free       (const type_t *self, tval *val);
static tval                *memory_manager_type_dup        (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata);

const type_t memory_manager_type_def =
  { type_type

    /* memory_tracker_defaults */
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
  { return sizeof(memory_manager_t); }

static int memory_manager_type_is_struct_initializer_check_default(const field_info_t *self, void *dest_mem, const void *src_mem)
{
  static const memory_manager_t empty;

  static const unsigned char *base = (const unsigned char *) &empty;

  /* typed_t type */
  if      (base + self->field_pos == (const unsigned char *) &empty.type)
  {
    typedef typed_t field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    const field_type src_when_default  = NULL;
    const field_type dest_when_default = memory_manager_type;

    if (*src == src_when_default)
    {
      if (dest) *dest = dest_when_default;
      return 1;
    }
    else
    {
      if (dest) *dest = *src;
      return 0;
    }
  }

  /* void *(*malloc) (const memory_manager_t *self, size_t  size); */
  else if (base + self->field_pos == (const unsigned char *) &empty.malloc)
  {
    typedef void *(*field_type)(const memory_manager_t *self, size_t  size);

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* void  (*free)   (const memory_manager_t *self, void   *ptr); */
  else if (base + self->field_pos == (const unsigned char *) &empty.free)
  {
    typedef void  (*field_type)(const memory_manager_t *self, void   *ptr);

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size); */
  else if (base + self->field_pos == (const unsigned char *) &empty.calloc)
  {
    typedef void *(*field_type)(const memory_manager_t *self, size_t  nmemb, size_t size);

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size); */
  else if (base + self->field_pos == (const unsigned char *) &empty.realloc)
  {
    typedef void *(*field_type)(const memory_manager_t *self, void   *ptr,   size_t size);

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* void   *state; */
  else if (base + self->field_pos == (const unsigned char *) &empty.state)
  {
    typedef void   *field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* size_t  state_size; */
  else if (base + self->field_pos == (const unsigned char *) &empty.state_size)
  {
    typedef size_t  field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* Unrecognized field. */
  else
  {
    return -1;
  }
}
static size_t memory_manager_type_is_struct_default_value(const field_info_t *self, void *dest_mem)
{
}
static size_t memory_manager_type_is_struct_template_unused_value(const field_info_t *self, void *dest_mem)
{
  /* General case for all other fields, and unrecognized fields. */
  else
  {
  }
}
static const struct_info_t *memory_manager_type_is_struct(const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const memory_manager_t  empty;

        field_info_t           *field;
        size_t                  fields_len;

        const size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        const size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = memory_manager_type_is_struct_default_value;
        template_unused_value = memory_manager_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void *(*malloc) (const memory_manager_t *self, size_t  size); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.malloc);
          field->field_size            = sizeof            (empty.malloc);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void  (*free)   (const memory_manager_t *self, void   *ptr); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.free);
          field->field_size            = sizeof            (empty.free);

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.calloc);
          field->field_size            = sizeof            (empty.calloc);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.realloc);
          field->field_size            = sizeof            (empty.realloc);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          fielfield->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void  (*on_oom) (const memory_manager_t *self, size_t      size); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.on_oom);
          field->field_size            = sizeof            (empty.on_oom);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          fielfield->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void  (*on_err) (const memory_manager_t *self, const char *msg); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.on_err);
          field->field_size            = sizeof            (empty.on_err);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          fielfield->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void   *state; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.state);
          field->field_size            = sizeof            (empty.state);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t  state_size; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.state_size);
          field->field_size            = sizeof            (empty.state_size);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* No memory tracker field. */
        struct_info_def.has_memory_tracker   = 0;
        struct_info_def.memory_tracker_field = 0;
      }
    }

    return struct_info;
  }

static typed_t              memory_manager_type_cons_type(const type_t *self)
  { return template_cons_type; }

static tval                *memory_manager_type_init     (const type_t *self, tval *cons)
  {
    static memory_manager_t *defaults = NULL;
    if (!defaults)
    {
      static memory_manager_t type_defaults;

      type_defaults = memory_manager_defaults;

      defaults = &type_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 memory_manager_type_free     (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static tval                *memory_manager_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata)
  {
    struct_dup(self->is_struct(self), dest, src, copy, rec);

    return dest;
  }

/* ---------------------------------------------------------------- */

void memory_manager_default_on_oom(const memory_manager_t *self, size_t      size)
{
  static char         buf[4096];
  static const size_t buf_size = sizeof(buf) / sizeof(buf[0]);

  snprintf
    ( (const char *) buf, (size_t) buf_size
    , "*** (EE) Error: memory_manager_default_on_oom: Out of memory!  Requested '%d' bytes.\n"
    , (int) size
    );

  buf[buf_size - 1] = 0;
  malloc_manager_on_err(self, buf);
}
void memory_manager_default_on_err(const memory_manager_t *self, const char *msg)
{
  size_t len;

  fprintf
    ( stderr
    , "malloc_manager_on_err: %s"
    , msg
    );

  /* If "msg" lacks a trailing newline character, add one. */
  len = strlen(msg);
  if (len >= 1 && msg[len - 1] != '\n')
  {
    fprintf
      ( stderr
      , "\n"
      );
  }
}

void on_memory_manager_oom_do_nothing(const memory_manager_t *self, size_t      size)
{
}

void on_memory_manager_err_do_nothing(const memory_manager_t *self, const char *msg)
{
}

/* ---------------------------------------------------------------- */

const memory_manager_t memory_manager_defaults =
  { memory_manager_type

  , NULL
  , NULL
  , NULL
  , NULL

  , memory_manager_default_on_oom
  , memory_manager_default_on_orr

  , NULL
  , 0
  };

/* ---------------------------------------------------------------- */

const memory_manager_t * const default_memory_manager = &malloc_manager;

static void *malloc_manager_malloc (const memory_manager_t *self, size_t  size);
static void  malloc_manager_free   (const memory_manager_t *self, void   *ptr);
static void *malloc_manager_calloc (const memory_manager_t *self, size_t  nmemb, size_t size);
static void *malloc_manager_realloc(const memory_manager_t *self, void   *ptr,   size_t size);
static void  malloc_manager_on_oom (const memory_manager_t *self, size_t      size);
static void  malloc_manager_on_err (const memory_manager_t *self, const char *msg);

const memory_manager_t malloc_manager =
  { memory_manager_type

  , malloc_manager_malloc
  , malloc_manager_free
  , malloc_manager_calloc
  , malloc_manager_realloc

  , NULL
  , 0
  };

static void *malloc_manager_malloc (const memory_manager_t *self, size_t  size)
  { return malloc (size); }
static void  malloc_manager_free   (const memory_manager_t *self, void   *ptr)
  {        free   (ptr); }
static void *malloc_manager_calloc (const memory_manager_t *self, size_t  nmemb, size_t size)
  { return calloc (nmemb, size); }
static void *malloc_manager_realloc(const memory_manager_t *self, void   *ptr,   size_t size)
  { return realloc(ptr,   size); }
static void  malloc_manager_on_oom (const memory_manager_t *self, size_t      size)
{
  static char         buf[4096];
  static const size_t buf_size = sizeof(buf) / sizeof(buf[0]);

  snprintf
    ( (const char *) buf, (size_t) buf_size
    , "*** (EE) Error: malloc_manager_on_oom: Out of memory!  Requested '%d' bytes.\n"
    , (int) size
    );

  buf[buf_size - 1] = 0;
  malloc_manager_on_err(self, buf);
}
static void  malloc_manager_on_err (const memory_manager_t *self, const char *msg)
{
  size_t len;

  fprintf
    ( stderr
    , "malloc_manager_on_err: %s"
    , msg
    );

  /* If "msg" lacks a trailing newline character, add one. */
  len = strlen(msg);
  if (len >= 1 && msg[len - 1] != '\n')
  {
    fprintf
      ( stderr
      , "\n"
      );
  }
}

/* ---------------------------------------------------------------- */

void *memory_manager_malloc(const memory_manager_t *memory_manager, size_t size)
{
  void *mem;

  if (!memory_manager || !memory_manager->malloc)
    memory_manager = &malloc_manager;

  if (!memory_manager->malloc || !memory_manager->free)
  {
    if (memory_manager->malloc)
      memory_manager_on_err
        ( memory_manager
        , "error: a memory manager cannot have \"malloc\" without \"free\"!"
        );
    else
      memory_manager_on_err
        ( memory_manager
        , "error: a memory manager cannot have \"free\" without \"malloc\"!"
        );

    return NULL;
  }

  mem = memory_manager->malloc(memory_manager, size);

  if (!mem)
    memory_manager_on_oom(memory_manager, size);

  return mem;
}

void memory_manager_free(const memory_manager_t *memory_manager, void *ptr)
{
  if (!memory_manager || !memory_manager->free)
    memory_manager = &malloc_manager;

  if (!memory_manager->malloc || !memory_manager->free)
  {
    if (memory_manager->malloc)
      memory_manager_on_err
        ( memory_manager
        , "error: a memory manager cannot have \"malloc\" without \"free\"!"
        );
    else
      memory_manager_on_err
        ( memory_manager
        , "error: a memory manager cannot have \"free\" without \"malloc\"!"
        );

    return NULL;
  }

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

void  memory_manager_on_oom (const memory_manager_t *memory_manager, size_t      size)
{
  void (*on_oom)(const memory_manager_t *self, size_t size);

  if (!memory_manager || !memory_manager->on_oom)
    memory_manager = &malloc_manager;

  on_oom = memory_manager->on_oom;
  if (!on_oom)
    on_oom = memory_manager_default_on_oom;

  on_oom(memory_manager, size);
}

void  memory_manager_on_err (const memory_manager_t *memory_manager, const char *msg)
{
  void (*on_err)(const memory_manager_t *self, size_t size);

  if (!memory_manager || !memory_manager->on_err)
    memory_manager = &malloc_manager;

  on_err = memory_manager->on_err;
  if (!on_err)
    on_err = memory_manager_default_on_err;

  on_err(memory_manager, msg);
}

/* ---------------------------------------------------------------- */

/* memory_tracker type. */

static const type_t        *memory_tracker_type_typed      (const type_t *self);
static const char          *memory_tracker_type_name       (const type_t *self);
static const char          *memory_tracker_type_info       (const type_t *self);
static size_t               memory_tracker_type_size       (const type_t *self, const tval *val);
static const struct_info_t *memory_tracker_type_is_struct  (const type_t *self);
static typed_t              memory_tracker_type_cons_type  (const type_t *self);
static tval                *memory_tracker_type_init       (const type_t *self, tval *cons);
static void                 memory_tracker_type_free       (const type_t *self, tval *val);
static tval                *memory_tracker_type_dup        (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata);

const type_t memory_tracker_type_def =
  { type_type

    /* memory_tracker_defaults */
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

static int memory_tracker_type_is_struct_initializer_check_default(const field_info_t *self, void *dest_mem, const void *src_mem)
{
  static const memory_manager_t empty;

  static const unsigned char *base = (const unsigned char *) &empty;

  /* typed_t type */
  if      (base + self->field_pos == (const unsigned char *) &empty.type)
  {
    typedef typed_t field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    const field_type src_when_default  = NULL;
    const field_type dest_when_default = memory_manager_type;

    if (*src == src_when_default)
    {
      *dest = dest_when_default;
      return 1;
    }
    else
    {
      if (dest) *dest = *src;
      return 0;
    }
  }

  /* memory_manager_t memory_manager; */
  else if (base + self->field_pos == (const unsigned char *) &empty.memory_manager)
  {
    typedef memory_manager_t field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* int      is_heap_allocated; */
  else if (base + self->field_pos == (const unsigned char *) &empty.is_heap_allocated)
  {
    typedef int      field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* void   **dynamically_allocated_buffers; */
  else if (base + self->field_pos == (const unsigned char *) &empty.dynamically_allocated_buffers)
  {
    typedef void   **field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* size_t   dynamically_allocated_buffers_num; */
  else if (base + self->field_pos == (const unsigned char *) &empty.dynamically_allocated_buffers_num)
  {
    typedef size_t   field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* size_t   dynamically_allocated_buffers_size; */
  else if (base + self->field_pos == (const unsigned char *) &empty.dynamically_allocated_buffers_size)
  {
    typedef size_t   field_type;

          field_type *dest = dest_mem;
    const field_type *src  = src_mem;

    if (dest) *dest = *src;
    return 0;
  }

  /* Unrecognized field. */
  else
  {
    return -1;
  }
}
static const struct_info_t *memory_tracker_type_is_struct(const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const memory_tracker_t  empty;

        field_info_t           *field;
        size_t                  fields_len;

        const size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        const size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = memory_tracker_type_is_struct_default_value;
        template_unused_value = memory_tracker_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* memory_manager_t memory_manager; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.memory_manager);
          field->field_size            = sizeof            (empty.memory_manager);
          field->field_type            = memory_manager_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void *dynamically_allocated_container; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_container);
          field->field_size            = sizeof            (empty.dynamically_allocated_container);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void   **dynamically_allocated_buffers; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t   dynamically_allocated_buffers_num; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers_num);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers_num);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t   dynamically_allocated_buffers_size; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers_size);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers_size);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t   dynamically_allocated_buffers_last_even; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers_last_even);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers_last_even);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t   dynamically_allocated_buffers_last_odd; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dynamically_allocated_buffers_last_odd);
          field->field_size            = sizeof            (empty.dynamically_allocated_buffers_last_odd);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* No memory tracker field. */
        struct_info_def.has_memory_tracker   = 0;
        struct_info_def.memory_tracker_field = 0;
      }
    }

    return struct_info;
  }

static typed_t              memory_tracker_type_cons_type(const type_t *self)
  { return template_cons_type; }

static tval                *memory_tracker_type_init     (const type_t *self, tval *cons)
  {
    static memory_tracker_t *defaults = NULL;
    if (!defaults)
    {
      static memory_tracker_t type_defaults;

      type_defaults = memory_tracker_default;

      defaults = &type_defaults;
    }

    return template_cons_dup_struct(cons, defaults, self->is_struct(self));
  }

static void                 memory_tracker_type_free     (const type_t *self, tval *val)
  {
    template_cons_free_struct(val, self->is_struct(self));
  }

static tval                *memory_tracker_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata)
  {
    struct_dup(self->is_struct(self), dest, src, copy, rec);

    return dest;
  }

/* ---------------------------------------------------------------- */

/*
 * Default memory tracker:
 *   - Default memory manager.
 *   - Associated value not allocated on heap.
 *   - No allocated buffers to track dynamically allocated memory.
 */
const memory_tracker_t memory_tracker_defaults =
  { memory_tracker_type

  /* memory_manager_defaults */
  , /* memory_manager                          */ { memory_manager_type }

  , /* dynamically_allocated_container         */ NULL
  , /* dynamically_allocated_buffers           */ NULL
  , /* dynamically_allocated_buffers_num       */ 0
  , /* dynamically_allocated_buffers_size      */ 0
  , /* dynamically_allocated_buffers_last_even */ 0
  , /* dynamically_allocated_buffers_last_odd  */ 0
  };

const char *memory_tracker_initialize_no_buffers(memory_tracker_t *memory_tracker, const memory_manager_t *memory_manager, void *dynamically_allocated_container)
{
  if (!memory_tracker)
  {
    return "error: memory_tracker_no_buffers: "memory_tracker" is NULL!\n"
  }

  if (!memory_manager)
    memory_manager = default_memory_manager;

  memory_tracker->memory_manager                          = memory_manager;
  memory_tracker->dynamically_allocated_container         = dynamically_allocated_container;
  memory_tracker->dynamically_allocated_buffers           = NULL
  memory_tracker->dynamically_allocated_buffers_num       = 0;
  memory_tracker->dynamically_allocated_buffers_size      = 0;
  memory_tracker->dynamically_allocated_buffers_last_even = 0;
  memory_tracker->dynamically_allocated_buffers_last_odd  = 0;

  return "";
}

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
static tval                *field_info_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata);

const type_t field_info_type_def =
  { type_type

    /* memory_tracker_defaults */
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

static int field_info_type_is_struct_initializer_check_default(const field_info_t *self, void *dest_mem, const void *src_mem)
{
}
static const struct_info_t *field_info_type_is_struct(const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const field_info_t      empty;

        field_info_t           *field;
        size_t                  fields_len;

        const size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        const size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = field_info_type_is_struct_default_value;
        template_unused_value = field_info_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* ptrdiff_t     field_pos; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.field_pos);
          field->field_size            = sizeof            (empty.field_pos);
          field->field_type            = ptrdiff_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t        field_size; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.field_size);
          field->field_size            = sizeof            (empty.field_size);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const type_t *field_type; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.field_type);
          field->field_size            = sizeof            (empty.field_type);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* int           is_metadata; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.is_metadata);
          field->field_size            = sizeof            (empty.is_metadata);
          field->field_type            = int_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* int           is_recurse; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.is_recurse);
          field->field_size            = sizeof            (empty.is_recurse);
          field->field_type            = int_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t (*write_default)(const field_info_t *self, void *dest_mem); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.write_default);
          field->field_size            = sizeof            (empty.write_default);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t (*write_template_unused)(const field_info_t *self, void *dest_mem); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.write_template_unused);
          field->field_size            = sizeof            (empty.write_template_unused);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* No memory tracker field. */
        struct_info_def.has_memory_tracker   = 0;
        struct_info_def.memory_tracker_field = 0;
      }
    }

    return struct_info;
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

static tval                *field_info_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata)
  {
    struct_dup(self->is_struct(self), dest, src, copy, rec);

    return dest;
  }

const field_info_t field_info_defaults =
  { field_info_type

  , /* field_pos             */ (ptrdiff_t) 0
  , /* field_size            */ (size_t)    0
  , /* field_type            */ NULL

  , /* is_metadata           */ 0
  , /* is_recurse            */ 0

  , /* default_value         */ default_value_zero
  , /* template_unused_value */ template_unused_value_zero
  };

/* The field has a default value of that represented by 0 bytes. */
size_t default_value_zero(const field_info_t *self, void *dest_mem)
{
  size_t size = self->field_size;

  if (dest_mem)
    memset(dest_mem, 0, size);

  return size;
}

/* The field has an unused value of that represented by 0 bytes. */
size_t template_unused_value_zero(const field_info_t *self, void *dest_mem)
{
  size_t size = self->field_size;

  if (dest_mem)
    memset(dest_mem, 0, size);

  return size;
}

/* The field has no special unused value. */
size_t template_no_unused_value(const field_info_t *self, void *dest_mem)
{
  return 0;
}

int field_memcmp(const field_info_t *field_info, const void *s1, const void *s2)
{
  size_t i;
  size_t size = self->field_size;

  if (!s1)
    return FIELD_MEMCMP_ERR_NULL_S1;
  if (!s2)
    return FIELD_MEMCMP_ERR_NULL_S2;

  for (i = 0; i < size; ++i)
  {
    unsigned char b1 = ((const unsigned char *) s1)[i];
    unsigned char b2 = ((const unsigned char *) s2)[i];

    if      (b1 > b2)
    {
      int i1         = (int) b1;
      int i2         = (int) b2;

      int difference = i2 - i1;

      if (difference <= 0)
        difference = 1;

      return difference;
    }
    else if (b1 < b2)
    {
      int i1         = (int) b1;
      int i2         = (int) b2;

      int difference = i2 - i1;

      if (difference >= 0)
        difference = -1;

      return difference;
    }
  }

  return 0;
}

int is_field_template_unused(const field_info_t *field_info, const void *src_mem, void *working_mem, const memory_manager_t *memory_manager, void *out_default_mem)
{
  int      is_unused;

  int      need_free = 0;
  size_t   has_unused;
  size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

  if (!field_info)
    return -4;
  if (!src_mem)
    return -2;

  /* Allocate resources. */
  {
    need_free = 0;
    if (!working_mem)
    {
      need_free = 1;

      if (!memory_manager)
        memory_manager = malloc_manager;

      working_mem = memory_manager_malloc(memory_manager, field_info->field_size());

      if (!working_mem)
        return -1;
    }
  }
  /* Body: use resources. */
  {
    /* Read in the field's special unused value, to "working_mem". */
    template_unused_value = field_info->template_unused_value;
    if (!template_unused_value)
    {
      template_unused_value = field_info_defaults.template_unused_value;
      if (!template_unused_value)
        template_unused_value = template_unused_value_zero;
    }

    has_unused = template_unused_value(field_info, working_mem);

    /* Does the field have a special unused value for equality checking? */
    if (!has_unused)
    {
      /* No; we'll just return 0. */
      is_unused = 0;
    }
    else
    {
      /* Yes.  Is "src" equal to this special unused value? */
      if (field_memcmp(field_info, src_mem, working_mem) != 0)
      {
        /* No; we'll just return 0. */
        is_unused = 0;
      }
      else
      {
        /* Yes.  We'll return >= 1, and output the default value if an output
         * buffer is provided.
         * */
        if (!out_default_mem)
          is_unused = 1;

        if (out_default_mem)
        {
          /* Write the field's default value to "out_default_mem". */

          size_t (*default_value)(const field_info_t *self, void *dest_mem);

          is_unused = 2;

          default_value = field_info->default_value;
          if (!default_value)
          {
            default_value = field_info_defaults.default_value;
            if (!default_value)
              default_value = default_value_zero;
          }

          default_value(field_info, out_default_mem);
        }
      }
    }
  }
  /* Free resources. */
  if (need_free)
  {
    memory_manager_free(memory_manager, working_mem);
  }

  return is_unused;
}

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
static tval                *struct_info_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata);

const type_t struct_info_type_def =
  { type_type

    /* memory_tracker_defaults */
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

static int struct_info_type_is_struct_initializer_check_default(const field_info_t *self, void *dest_mem, const void *src_mem)
{
}
static const struct_info_t *struct_info_type_is_struct(const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const struct_info_t     empty;

        field_info_t           *field;
        size_t                  fields_len;

        const size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        const size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = struct_info_type_is_struct_default_value;
        template_unused_value = struct_info_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* field_info_t fields[STRUCT_INFO_NUM_FIELDS]; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.fields);
          field->field_size            = sizeof            (empty.fields);
          field->field_type            = array_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t       fields_len; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.fields_len);
          field->field_size            = sizeof            (empty.fields_len);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* struct_info_t *tail; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.tail);
          field->field_size            = sizeof            (empty.tail);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* int       has_memory_tracker; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.has_memory_tracker);
          field->field_size            = sizeof            (empty.has_memory_tracker);
          field->field_type            = int_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t    memory_tracker_field; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.memory_tracker_field);
          field->field_size            = sizeof            (empty.memory_tracker_field);
          field->field_type            = size_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* No memory tracker field. */
        struct_info_def.has_memory_tracker   = 0;
        struct_info_def.memory_tracker_field = 0;
      }
    }

    return struct_info;
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

static tval                *struct_info_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata)
  {
    struct_dup(self->is_struct(self), dest, src, copy, rec);

    return dest;
  }

const struct_info_t struct_info_defaults =
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

memory_tracker_t *struct_value_has_memory_tracker(const struct_info *struct_info, void *src_mem)
{
  if (!struct_info || !src_mem)
    return NULL;

  if (struct_info->has_memory_tracker)
  {
    const field_info_t *memory_tracker_field = struct_info->fields[struct_info->memory_tracker_field];

    memory_tracker_t *memory_tracker;

    memory_tracker = (memory_tracker_t *) field_cref(memory_tracker_field->field_pos, src_mem);

    return memory_tracker;
  }
  else
  {
    return NULL;
  }
}

/* const char *struct_dup(const struct_info_t *struct_info, void *dest, const void *src, int force_no_defaults, int rec_copy, int dup_metadata, dup_traversal_t *dup_traversal); */

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
static tval                *type_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata);

const type_t type_type_def =
  { type_type

    /* memory_tracker_defaults */
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

static int type_type_is_struct_initializer_check_default(const field_info_t *self, void *dest_mem, const void *src_mem)
{
}
static const struct_info_t *type_type_is_struct(const type_t *self)
  {
    /* Procedurally initialize the type's struct_info. */
    static const struct_info_t *struct_info = NULL;
    if (!struct_info)
    {
      static struct_info_t struct_info_def;
      struct_info = &struct_info_def;

      /* ---------------------------------------------------------------- */

      {
        const type_t            empty;
        size_t                  memory_tracker_field;

        field_info_t           *field;
        size_t                  fields_len;

        const size_t (*default_value)        (const field_info_t *self, void *dest_mem);
        const size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

        default_value         = type_type_is_struct_default_value;
        template_unused_value = type_type_is_struct_template_unused_value;

        /* struct_info->type */
        struct_info_def.type = struct_info_type;

        /* struct_info->fields */
        fields_len = 0;
        {
          /* typed_t type */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.type);
          field->field_size            = sizeof            (empty.type);
          field->field_type            = typed_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* memory_tracker_t memory; */
          memory_tracker_field = fields_len;
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.memory);
          field->field_size            = sizeof            (empty.memory);
          field->field_type            = memory_tracker_type();

          field->is_metadata           = 1;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const type_t        *(*typed)    (const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.typed);
          field->field_size            = sizeof            (empty.typed);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const char          *(*name)     (const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.name);
          field->field_size            = sizeof            (empty.name);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const char          *(*info)     (const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.info);
          field->field_size            = sizeof            (empty.info);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* size_t               (*size)     (const type_t *self, const tval *val); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.size);
          field->field_size            = sizeof            (empty.size);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const struct_info_t *(*is_struct)(const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.is_struct);
          field->field_size            = sizeof            (empty.is_struct);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* typed_t              (*cons_type)(const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.cons_type);
          field->field_size            = sizeof            (empty.cons_type);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* tval                *(*init)     (const type_t *self, tval *cons); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.init);
          field->field_size            = sizeof            (empty.init);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* void                 (*free)     (const type_t *self, tval *val); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.free);
          field->field_size            = sizeof            (empty.free);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const tval          *(*has_default)(const type_t *self); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.has_default);
          field->field_size            = sizeof            (empty.has_default);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* memory_tracker_t    *(*mem)        (const type_t *self, tval *val_raw); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.mem);
          field->field_size            = sizeof            (empty.mem);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* tval                *(*dup)        ( const type_t *self               */
          /*                                    , tval *dest                       */
          /*                                    , const tval *src                  */
          /*                                    , int rec_copy                     */
          /*                                    , int dup_metadata                 */
          /*                                    , dup_traversal_t *dup_traversal); */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.dup);
          field->field_size            = sizeof            (empty.dup);
          field->field_type            = funp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;

          /* const char *parity; */
          field = &struct_info_def.fields[fields_len++];

          field->type                  = field_info_type;

          field->field_pos             = field_pos(&empty, &empty.parity);
          field->field_size            = sizeof            (empty.parity);
          field->field_type            = objp_type();

          field->is_metadata           = 0;
          field->is_recurse            = 0;
          field->default_value         = default_value;
          field->template_unused_value = template_unused_value;


          /* terminating_field_info: end of fields. */
          struct_info_def.fields[fields_len] = terminating_field_info;
        }

        /* struct_info->... */
        struct_info_def.fields_len           = fields_len;
        struct_info_def.tail                 = NULL;

        /* Has memory tracker field. */
        struct_info_def.has_memory_tracker   = 1;
        struct_info_def.memory_tracker_field = memory_tracker_field;
      }
    }

    return struct_info;
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

static tval                *type_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata)
  {
    struct_dup(self->is_struct(self), dest, src, copy, rec);

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
static tval                *default_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata);

const type_t type_defaults =
  { type_type

    /* memory_tracker_defaults */
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

static tval                *default_type_dup      (const type_t *self, tval *dest, const tval *src, int rec_copy, int dup_metadata)
  {
    struct_dup(self->is_struct(self), dest, src, copy, rec);

    return dest;
  }

/* ---------------------------------------------------------------- */

const type_t *type_typed(const type_t *self)
{
  return self;
}

const type_t *type_untyped(const type_t *self)
{
  return NULL;
}

const tval *type_has_no_default(const type_t *self)
{
  return NULL;
}

/* ---------------------------------------------------------------- */
/* Template constructors, available for types to use.               */
/* ---------------------------------------------------------------- */

const template_cons_t * const default_template_cons = &template_cons_defaults;

const template_cons_t template_cons_defaults =
  { template_cons_type

  , /* dest                   */ NULL
  , /* memory_manager         */ NULL
  , /* initials               */ NULL
  , /* force_no_defaults      */ 0
  , /* initials_copy_rec      */ 0
  , /* dup_traversal          */ NULL
  , /* preserve_nocopy        */ 0
  , /* user                   */ NULL
  , /* allocate_only_with_num */ (size_t) 0
  , /* out_init_error_msg     */ NULL
  , /* init_error_msg_size    */ (size_t) 0
  };

tval *template_cons_dup_struct
  ( const template_cons_t   *cons
  , size_t                   size
  , const tval              *default_initials
  , const struct_info_t     *struct_info
  , memory_tracker_t      *(*mem)(tval *val_raw)
  )
{
  tval             *dest;

  int               is_allocate;
  int               is_allocate_only;

  const char       *is_err;

  memory_manager_t *memory_manager;

  if (!cons)
  {
    cons = default_template_cons;
    if (!cons)
      cons = &template_cons_defaults;
  }

  memory_manager = cons->memory_manager;
  if (!memory_manager)
    memory_manager = default_memory_manager;

  is_allocate_only = cons->allocate_only_with_num >= 1;

  is_allocate = !cons_dest || is_allocate_only;

  /* Do we need to dynamically allocate memory for this value? */
  if (is_allocate)
  {
    int    is_array;
    size_t number_to_allocate;

    if (cons->preserve_metadata)
    {
      size_t init_error_msg_size_terminator;

      init_error_msg_size_terminator = cons->init_error_msg_size - 1;
      if (init_error_msg_size_terminator < 0)
        init_error_msg_size_terminator = 0;

      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) init_error_msg_size_terminator
          , "Error: template_cons_dup_struct: preserve_metadata flag is set, but the constructor dynamically allocates a value.\n"
            "  Since "preserve_metadata" skips memory tracker initialization, tracking this dynamic allocation can't be added to it.\n"
            "  Failed to initialize value.\n"
          );

      return NULL;
    }

    if (!mem && (!struct_info || !struct_info->has_memory_tracker))
    {
      size_t init_error_msg_size_terminator;

      init_error_msg_size_terminator = cons->init_error_msg_size - 1;
      if (init_error_msg_size_terminator < 0)
        init_error_msg_size_terminator = 0;

      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) init_error_msg_size_terminator
          , "Error: template_cons_dup_struct: constructor dynamically allocates memory, but there is no associated memory tracker!.\n"
            "  Failed to initialize new value, because \"mem\" is NULL and no \"struct_info\" with a memory tracker field reference was provided.\n"
          );

      return NULL;
    }

    number_to_allocate = cons->allocate_only_with_num;
    if (number_to_allocate <= 0)
      number_to_allocate = 1;

    is_array = number_to_allocate >= 2;

    if (!is_array && number_to_allocate == 1)
    {
      dest = memory_manager_malloc(memory_manager, size);
    }
    else
    {
      dest = memory_manager_calloc(memory_manager, number_to_allocate, size);
    }

    if (!dest)
    {
      size_t init_error_msg_size_terminator;

      init_error_msg_size_terminator = cons->init_error_msg_size - 1;
      if (init_error_msg_size_terminator < 0)
        init_error_msg_size_terminator = 0;

      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) init_error_msg_size_terminator
          , "Error: template_cons_dup_struct: failed to allocate value memory!  Requested a total of '%d' bytes.\n"
          , (int) (number_to_allocate * size)
          );

      return NULL;
    }
  }

  /* Should we skip initialization? */
  if (is_allocate_only)
  {
    return dest;
  }

  if (!struct_info)
  {
    size_t init_error_msg_size_terminator;

    init_error_msg_size_terminator = cons->init_error_msg_size - 1;
    if (init_error_msg_size_terminator < 0)
      init_error_msg_size_terminator = 0;

    if (cons->out_init_error_msg)
      snprintf
        ( (char *) cons->out_init_error_msg, (size_t) init_error_msg_size_terminator
        , "Error: template_cons_dup_struct: \"struct_info\" is NULL!\n"
          "  Failed to initialize a value without a representation of the struct.\n"
        , (int) (number_to_allocate * size)
        );

    if (is_allocate)
      memory_manager_free(memory_manager, dest);

    return NULL;
  }

  /* Initialize memory tracker if available or necessary. */
  if (cons->preserve_metadata)
  {
    /* preserve_metadata is set: Skip memory tracker initialization. */
  }
  else
  {
    memory_tracker_t *memory_tracker;

    /* Note: if "mem" is provided, we should disregard     */
    /* "struct_info"'s references because "mem" overrides. */
    if      (mem)
    {
      memory_tracker = mem(dest);
    }
    else if (struct_info->has_memory_tracker)
    {
      memory_tracker = struct_value_has_memory_tracker(struct_info, dest);
    }

    /* Is a memory tracker required, but none available? */
    if (is_allocate && !memory_tracker)
    {
      size_t init_error_msg_size_terminator;

      init_error_msg_size_terminator = cons->init_error_msg_size - 1;
      if (init_error_msg_size_terminator < 0)
        init_error_msg_size_terminator = 0;

      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) init_error_msg_size_terminator
          , "Error: template_cons_dup_struct: constructor dynamically allocates memory, but there is no associated memory tracker!.\n"
            "  Failed to initialize new value, because %s.\n"
          ,   mem
            ? "\"mem\" indicates that there should be no associated innate memory tracker for this value"
            : "\"mem\" is NULL and \"struct_info\" lacks an associating reference to a memory tracker field"
          );

      memory_manager_free(memory_manager, dest);

      return NULL;
    }

    /* Initialize memory tracker. */
    if (memory_tracker)
    {
      if (is_allocate)
        is_err = memory_tracker_initialize_no_buffers(memory_tracker, memory_manager, dest);
      else
        is_err = memory_tracker_initialize_no_buffers(memory_tracker, memory_manager, NULL);

      if (is_err)
      {
        size_t error_msg_len;
        int    error_msg_has_trailing_newline;

        size_t init_error_msg_size_terminator;

        init_error_msg_size_terminator = cons->init_error_msg_size - 1;
        if (init_error_msg_size_terminator < 0)
          init_error_msg_size_terminator = 0;

        error_msg_len = strlen(is_err);
        if (error_msg_len <= 0)
          error_msg_has_trailing_newline = 0;
        else
          error_msg_has_trailing_newline = is_err[error_msg_len - 1] == '\n';

        if (cons->out_init_error_msg)
          snprintf
            ( (char *) cons->out_init_error_msg, (size_t) init_error_msg_size_terminator
            , "Error: template_cons_dup_struct: initializing the memory tracker with \"memory_tracker_initialize_no_buffers\" failed!\n"
              "  Failed to initialize a value without successful initialization of its associated memory tracker.\n"
              "Error message returned from \"memory_tracker_initialize_no_buffers\": %s%s"
            , (const char *) is_err
            , error_msg_has_trailing_newline ? "" : "\n"
            );

        if (is_allocate)
          memory_manager_free(memory_manager, dest);

        return NULL;
      }
    }
  }

  /* Initialize fields. */
  is_err =
    struct_dup
      ( struct_info
      , dest
      , cons->initials
      , cons->force_no_defaults
      , cons->initials_copy_rec
      , cons->preserve_metadata
      , cons->dup_traversal
      );

  if (is_err)
  {
    size_t error_msg_len;
    int    error_msg_has_trailing_newline;

    size_t init_error_msg_size_terminator;

    init_error_msg_size_terminator = cons->init_error_msg_size - 1;
    if (init_error_msg_size_terminator < 0)
      init_error_msg_size_terminator = 0;

    error_msg_len = strlen(is_err);
    if (error_msg_len <= 0)
      error_msg_has_trailing_newline = 0;
    else
      error_msg_has_trailing_newline = is_err[error_msg_len - 1] == '\n';

    if (cons->out_init_error_msg)
      snprintf
        ( (char *) cons->out_init_error_msg, (size_t) init_error_msg_size_terminator
        , "Error: template_cons_dup_struct: assigning the struct fields with \"struct_dup\" failed!\n"
          "  Failed to initialize a value without successful assignment of struct fields.\n"
          "Error message returned from \"struct_dup\": %s%s"
        , (const char *) is_err
        , error_msg_has_trailing_newline ? "" : "\n"
        );

    if (is_allocate)
      memory_manager_free(memory_manager, dest);

    return NULL;
  }

  /* Done! */
  return dest;
}

memory_tracker_t *(*)(tval *val) template_cons_get_type_mem(const type_t *type)
{
  if (!type)
    return NULL;

  return type->mem;
}

/* ---------------------------------------------------------------- */
/* Utility functions.                                               */
/* ---------------------------------------------------------------- */

ptrdiff_t   field_pos (const void *base, const void *field)
{
  return ((const unsigned char *) field) - ((const unsigned char *) base);
}

void       *field_ref (ptrdiff_t   pos,  void       *base)
{
  return (void *) (((unsigned char *) base) + pos);
}

const void *field_cref(ptrdiff_t   pos,  const void *base)
{
  return (const void *) (((const unsigned char *) base) + pos);
}

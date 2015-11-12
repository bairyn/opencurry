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
 *   - memcmp
 *   - memcpy
 *   - memmove
 *   - memset
 *   - strlen
 */
#include <string.h>

/* Headers defining types we provide representations for. */
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "base.h"
#include "type_base.h"

#include "type_base_ext.h"

#include "util.h"

/* ---------------------------------------------------------------- */
/* tval                                                             */
/* ---------------------------------------------------------------- */

/* typed type. */

const type_t *typed_type(void)
  { return &typed_type_def; }

static const char          *typed_type_name       (const type_t *self);
static size_t               typed_type_size       (const type_t *self, const tval *val);
static const tval          *typed_type_has_default(const type_t *self);

const type_t typed_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ typed_type_name
  , /* info                   */ NULL
  , /* @size                  */ typed_type_size
  , /* @is_struct             */ type_is_not_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ typed_type_has_default
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

static const char          *typed_type_name       (const type_t *self)
  { return "typed_t"; }

static size_t               typed_type_size       (const type_t *self, const tval *val)
  { return sizeof(typed_t); }

static const tval          *typed_type_has_default(const type_t *self)
  { return &typed_default; }

/* ---------------------------------------------------------------- */

const typed_t typed_default =
  TYPED_DEFAULT;

/* ---------------------------------------------------------------- */

/*
 * Obtain the type of a "tval *".
 */
const type_t *tval_type(const tval *val)
{
  return (const type_t *) val;
}

/* ---------------------------------------------------------------- */
/* Memory managers.                                                 */
/* ---------------------------------------------------------------- */

/* memory_manager type. */

/*
 * memory_manager_type: memory_manager_t (struct memory_manager_s)
 *
 * This type definition, like many others, is a standard representation for a
 * struct, defining how new values of this struct are initialized, allocated,
 * freed, and copied, and also fully represents each field in the struct,
 * allowing general programming based on the definition of the struct, rather
 * than ad-hoc programming handling each individual struct on an individual
 * basis.
 *
 * The "type_struct" module abstracts this pattern.  But here we hand-code
 * "memory_manager_type" at a low-level instead because it is a foundational
 * type definition in "type_base".  (As a bonus, this also illustrates how
 * types are defined directly.)
 *
 * memory_manager_type's most important subcomponent is
 * "memory_manager_is_struct", which describes "struct memory_manager_s".
 *
 * ----------------------------------------------------------------
 *
 * Normally, the "type_struct" module can be used to define "struct" type_t's.
 */

/*
 * memory_manager_type: The core part of a type definition.
 *  .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .
 *
 * This is just a C function that returns the "const type_t *" represent the
 * "memory_manager_t" type.
 *
 * "memory_manager_type" is how this type is accessed.
 *
 * ----------------------------------------------------------------
 *
 * .   .   .   .   .   .   .   .   .   .   .   .   .   .
 * # > const type_t *memory_manager_type(void);
 * # VS
 * # > const type_t  memory_manager_type;
 * .   .   .   .   .   .   .   .   .   .   .   .   .   .
 *
 * So why isn't this just "const type_t memory_manager_type"?  Why are
 * "type_t"'s generally obtained from functions, rather than from direct
 * references to the type?
 *
 * Because a type obtain from a function allows the type_t to be *procedurally*
 * generated, allowing it to initialize itself with code, rather than only
 * static definitions.
 *
 * "memory_manager_type" itself doesn't need this.  Like many other types, its
 * "type_t" can be defined at the top-level with a constant initializer.
 *
 * As a convention, because "memory_manager_type" is available at the top-level
 * without procedural generation, this is also available without a function, as
 * "const type_t memory_manager_type_def", which "memomry_manager_type" just
 * returns a reference to.  But not all types do.
 */
const type_t *memory_manager_type(void)
  { return &memory_manager_type_def; }

/*
 * "type_t" is a struct mostly of function pointer fields.
 *
 * Our struct is defined with function pointers to these functions.
 * These functions are "static", so they're only locally available here.
 *
 * (But they can still be accessed through "memory_manager_type"'s fields.)
 */
/* static const type_t        *memory_manager_type_typed      (const type_t *self);                  */
static const char          *memory_manager_type_name       (const type_t *self);
/* static const char          *memory_manager_type_info       ( const type_t *self                   */
/*                                                            , char         *out_info_buf           */
/*                                                            , size_t        info_buf_size          */
/*                                                            );                                     */
static size_t               memory_manager_type_size       (const type_t *self, const tval *val);
static const struct_info_t *memory_manager_type_is_struct  (const type_t *self);
/* static tval                *memory_manager_type_is_mutable (const type_t *self, const tval *val); */
/* static const type_t        *memory_manager_type_is_subtype ( const type_t *self                   */
/*                                                            , const type_t *is_subtype             */
/*                                                            );                                     */
/* static const type_t        *memory_manager_type_is_supertype                                      */
/*                                                            ( const type_t *self                   */
/*                                                            , const type_t *is_supertype           */
/*                                                            );                                     */
/* static typed_t              memory_manager_type_cons_type  (const type_t *self);                  */
/* static tval                *memory_manager_type_init       (const type_t *self, tval *cons);      */
/* static void                 memory_manager_type_free       (const type_t *self, tval *val);       */
static const tval          *memory_manager_type_has_default(const type_t *self);
/* static memory_tracker_t    *memory_manager_type_mem        (const type_t *self, tval *val_raw);   */
/* static void                *memory_manager_type_mem_init   ( const type_t *self                   */
/*                                                            , tval *val_raw                        */
/*                                                            , int is_dynamically_allocated         */
/*                                                            );                                     */
/* static int                  memory_manager_type_mem_is_dyn ( const type_t *self                   */
/*                                                            , tval         *val                    */
/*                                                            );                                     */
/* static int                  memory_manager_type_mem_free   ( const type_t *self                   */
/*                                                            , tval         *val                    */
/*                                                            );                                     */
/* static const memory_manager_t                                                                     */
/*                     *memory_manager_type_default_memory_manager                                   */
/*                                                            ( const type_t *self                   */
/*                                                            , tval *val                            */
/*                                                            );                                     */
/* static tval                *memory_manager_type_dup        ( const type_t *self                   */
/*                                                            , tval *dest                           */
/*                                                            , const tval *src                      */
/*                                                            , int defaults_src_unused              */
/*                                                            , int rec_copy                         */
/*                                                            , int dup_metadata                     */
/*                                                            , ref_traversal_t *vals                */
/*                                                            );                                     */
/* static void                  *memory_manager_type_user     (const type_t *self, tval *val);       */
/* static const void            *memory_manager_type_cuser    (const type_t *self, const tval *val); */
/* static int                    memory_manager_type_cmp      ( const type_t *self                   */
/*                                                            , const tval *check                    */
/*                                                            , const tval *baseline                 */
/*                                                            , int deep                             */
/*                                                            , ref_traversal_t *vals                */
/*                                                            );                                     */

/*
 * "memory_manager_type"'s type_t definition.
 *
 * The main way to access "memory_manager_type"'s type_t is via
 * "memory_manager_type", but since we can provide a "type_t" for
 * memory_manager, as a convenience, we provide a globally-accessible
 * definition of the "type_t" value here.
 *
 * Not all types are defined with a constant struct initializer.  For example,
 * a type might want to call other functions, e.g. those from "type_struct", to
 * define itself.  A "const struct mystruct_s myvalue = { ... };" at the
 * top-level cannot call functions.
 *
 * Even those types that can be defined with a constant struct initializer
 * don't need to provide a global reference to it.  We only do it here, for
 * "memory_manager_type", for convenience.
 *
 * The convention for type definitions that decide to use and provide a
 * constant struct initializer is to name the "type_t" value
 * "<type_name>_type_def".
 *
 * A function that generates a "const type_t *" reference is called a
 * "typed_t":
 *
 * > typedef const type_t *(*typed_t)(void);
 *
 * Example:
 *
 * >       typed_t     our_type;
 * > const type_t     *our_type_struct;
 * > const type_t     *also_our_type_struct;
 * >
 * > memory_manager_t  our_manager;
 * > template_cons_t   cons = { template_cons_type, &our_manager };
 * >
 * > our_type             = memory_manager;
 * >
 * > our_type_struct      = our_type();
 * > also_our_type_struct = memory_manager();
 * >
 * > type_init(our_type_struct, cons);
 */
const type_t memory_manager_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ type_is_typed /* memory_manager_type_typed          */

  , /* @name                  */ memory_manager_type_name
  , /* info                   */ NULL /* memory_manager_type_info                    */
  , /* @size                  */ memory_manager_type_size
  , /* @is_struct             */ memory_manager_type_is_struct
  , /* is_mutable             */ NULL /* memory_manager_type_is_mutable              */
  , /* is_subtype             */ NULL /* memory_manager_type_is_subtype              */
  , /* is_supertype           */ NULL /* memory_manager_type_is_supertype            */

  , /* cons_type              */ NULL /* memory_manager_type_cons_type               */
  , /* init                   */ NULL /* memory_manager_type_init                    */
  , /* free                   */ NULL /* memory_manager_type_free                    */
  , /* has_default            */ memory_manager_type_has_default
  , /* mem                    */ NULL /* memory_manager_type_mem                     */
  , /* mem_init               */ NULL /* memory_manager_type_mem_init                */
  , /* mem_is_dyn             */ NULL /* memory_manager_type_mem_is_dyn              */
  , /* mem_free               */ NULL /* memory_manager_type_mem_free                */
  , /* default_memory_manager */ NULL /* memory_manager_type_default_memory_manager  */

  , /* dup                    */ NULL /* memory_manager_type_dup                     */

  , /* user                   */ NULL /* memory_manager_type_user                    */
  , /* cuser                  */ NULL /* memory_manager_type_cuser                   */
  , /* cmp                    */ NULL /* memory_manager_type_cmp                     */

  , /* parity                 */ ""
  };

/*
 * Just return the name of the type.
 *
 * "memory_manager".
 */
static const char          *memory_manager_type_name       (const type_t *self)
  { return "memory_manager_t"; }

/*
 * Return the size of values of the represented type.
 */
static size_t               memory_manager_type_size       (const type_t *self, const tval *val)
  { return sizeof(memory_manager_t); }

/*
> DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(memory_manager)

=>

> static size_t memory_manager_default_field_value(const field_info_t *self, void *dest_field_mem)
> {
>   return field_default_value_from_type(self, dest_field_mem, memory_manager_type());
> }
*/

/*
 * "memory_manager_t" is a struct.
 *
 * Thus this function returns a representation of each field in the struct.
 *
 * This is the most important part of the definiton of
 */
DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(memory_manager)
static const struct_info_t *memory_manager_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(memory_manager);

    /* typed_t type */
    STRUCT_INFO_RADD(typed_type(), type);

    /* void *(*malloc) (const memory_manager_t *self, size_t  size); */
    STRUCT_INFO_RADD(funp_type(),  malloc);

    /* void  (*free)   (const memory_manager_t *self, void   *ptr); */
    STRUCT_INFO_RADD(funp_type(),  free);

    /* void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size); */
    STRUCT_INFO_RADD(funp_type(),  calloc);

    /* void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size); */
    STRUCT_INFO_RADD(funp_type(),  realloc);

    /* void  (*on_oom) (const memory_manager_t *self, size_t      size); */
    STRUCT_INFO_RADD(funp_type(),  on_oom);

    /* void  (*on_err) (const memory_manager_t *self, const char *msg); */
    STRUCT_INFO_RADD(funp_type(),  on_err);

    /* void   *state; */
    STRUCT_INFO_RADD(objp_type(),  state);

    /* size_t  state_size; */
    STRUCT_INFO_RADD(size_type(),  state_size);

    STRUCT_INFO_DONE();
  }

static const tval          *memory_manager_type_has_default(const type_t *self)
  { return type_has_default_value(self, &memory_manager_defaults); }

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

  , malloc_manager_on_oom
  , malloc_manager_on_err

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
    ( (char *) buf, (size_t) buf_size
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

void memory_manager_default_on_oom(const memory_manager_t *self, size_t      size)
{
  static char         buf[4096];
  static const size_t buf_size = sizeof(buf) / sizeof(buf[0]);

  snprintf
    ( (char *) buf, (size_t) buf_size
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
  MEMORY_MANAGER_DEFAULTS;

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
  void (*on_err)(const memory_manager_t *self, const char *msg);

  if (!memory_manager || !memory_manager->on_err)
    memory_manager = &malloc_manager;

  on_err = memory_manager->on_err;
  if (!on_err)
    on_err = memory_manager_default_on_err;

  on_err(memory_manager, msg);
}

/* ---------------------------------------------------------------- */

/* memory_tracker type. */

const type_t *memory_tracker_type(void)
  { return &memory_tracker_type_def; }

static const char          *memory_tracker_type_name       (const type_t *self);
static size_t               memory_tracker_type_size       (const type_t *self, const tval *val);
static const struct_info_t *memory_tracker_type_is_struct  (const type_t *self);
static const tval          *memory_tracker_type_has_default(const type_t *self);

const type_t memory_tracker_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ NULL

  , /* @name                  */ memory_tracker_type_name
  , /* info                   */ NULL
  , /* @size                  */ memory_tracker_type_size
  , /* @is_struct             */ memory_tracker_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ memory_tracker_type_has_default
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

static const char          *memory_tracker_type_name       (const type_t *self)
  { return "memory_tracker_t"; }

static size_t               memory_tracker_type_size       (const type_t *self, const tval *val)
  { return sizeof(memory_tracker_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(memory_tracker)
static const struct_info_t *memory_tracker_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(memory_tracker);

    /* typed_t type */
    STRUCT_INFO_RADD(typed_type(), type);

    /* memory_manager_t memory_manager; */
    STRUCT_INFO_RADD(memory_manager_type(), memory_manager);

    /* void *dynamically_allocated_container; */
    STRUCT_INFO_RADD(objp_type(), dynamically_allocated_container);

    /* void   **dynamically_allocated_buffers;      */
    /* size_t   dynamically_allocated_buffers_num;  */
    /* size_t   dynamically_allocated_buffers_size; */
    STRUCT_INFO_RADD(objp_type(), dynamically_allocated_buffers);
    STRUCT_INFO_RADD(size_type(), dynamically_allocated_buffers_num);
    STRUCT_INFO_RADD(size_type(), dynamically_allocated_buffers_size);

    /* size_t   dynamically_allocated_buffers_last_even; */
    /* size_t   dynamically_allocated_buffers_last_odd;  */
    STRUCT_INFO_RADD(size_type(), dynamically_allocated_buffers_last_even);
    STRUCT_INFO_RADD(size_type(), dynamically_allocated_buffers_last_odd);

    STRUCT_INFO_DONE();
  }

static const tval          *memory_tracker_type_has_default(const type_t *self)
  { return type_has_default_value(self, &memory_tracker_defaults); }

/* ---------------------------------------------------------------- */

/*
 * Default memory tracker:
 *   - Default memory manager.
 *   - Associated value not allocated on heap.
 *   - No allocated buffers to track dynamically allocated memory.
 */
const memory_tracker_t memory_tracker_defaults =
  MEMORY_TRACKER_DEFAULTS;

memory_tracker_t global_memory_tracker =
  MEMORY_TRACKER_DEFAULTS;

memory_tracker_t global_typed_dyn_memory_tracker =
  MEMORY_TRACKER_DEFAULTS;

const char *memory_tracker_initialize_empty_with_container(memory_tracker_t *memory_tracker, const memory_manager_t *memory_manager, void *dynamically_allocated_container)
{
  if (!memory_tracker)
  {
    return "error: memory_tracker_no_buffers: \"memory_tracker\" is NULL!\n";
  }

  if (!memory_manager)
    memory_manager = default_memory_manager;

  memory_tracker->type = memory_tracker_type;

  memory_tracker->memory_manager                          = *memory_manager;

  memory_tracker->dynamically_allocated_container         = dynamically_allocated_container;

  memory_tracker->dynamically_allocated_buffers           = NULL;
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

const type_t *field_info_type(void)
  { return &field_info_type_def; }

static const char          *field_info_type_name       (const type_t *self);
static size_t               field_info_type_size       (const type_t *self, const tval *val);
static const struct_info_t *field_info_type_is_struct  (const type_t *self);
static const tval          *field_info_type_has_default(const type_t *self);

const type_t field_info_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ NULL

  , /* @name                  */ field_info_type_name
  , /* info                   */ NULL
  , /* @size                  */ field_info_type_size
  , /* @is_struct             */ field_info_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ field_info_type_has_default
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

static const char          *field_info_type_name       (const type_t *self)
  { return "field_info_t"; }

static size_t               field_info_type_size       (const type_t *self, const tval *val)
  { return sizeof(field_info_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(field_info)
static const struct_info_t *field_info_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(field_info);

    /* typed_t type */
    STRUCT_INFO_RADD(typed_type(), type);

    /* ptrdiff_t     field_pos;  */
    /* size_t        field_size; */
    /* const type_t *field_type; */
    STRUCT_INFO_RADD(ptrdiff_type(), field_pos);
    STRUCT_INFO_RADD(size_type(),    field_size);
    STRUCT_INFO_RADD(objp_type(),    field_type);

    /* int           is_metadata; */
    STRUCT_INFO_RADD(int_type(), is_metadata);

    /* int           is_recursible_ref; */
    STRUCT_INFO_RADD(int_type(), is_recursible_ref);

    /* size_t      (*default_value)        (const field_info_t *self, void *dest_field_mem); */
    STRUCT_INFO_RADD(funp_type(), default_value);

    /* size_t      (*template_unused_value)(const field_info_t *self, void *dest_field_mem); */
    STRUCT_INFO_RADD(funp_type(), template_unused_value);

    STRUCT_INFO_DONE();
  }

static const tval          *field_info_type_has_default(const type_t *self)
  { return type_has_default_value(self, &field_info_defaults); }

/* ---------------------------------------------------------------- */

const field_info_t field_info_defaults =
  FIELD_INFO_DEFAULTS;

/* ---------------------------------------------------------------- */

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

int field_info_cmp(const field_info_t *a, const field_info_t *b)
{
  if (!a)
    return -3;
  if (!b)
    return -2;

  if (a->type                  != b->type)
    return -1;
  if (a->field_pos             != b->field_pos)
    return -1;
  if (a->field_size            != b->field_size)
    return -1;
  if (a->field_type            != b->field_type)
    return -1;
  if (a->is_metadata           != b->is_metadata)
    return -1;
  if (a->is_recursible_ref     != b->is_recursible_ref)
    return -1;
  if (a->default_value         != b->default_value)
    return -1;
  if (a->template_unused_value != b->template_unused_value)
    return -1;

  return 0;
}

int is_field_terminator(const field_info_t *field_info)
{
  if (!field_info || !field_terminator)
    return -1;

  if (field_info_cmp(field_info, field_terminator) == 0)
    return 1;
  else
    return 0;
}

void       *field_info_ref (const field_info_t *field_info,  void       *base)
{
  if (!field_info || !base)
    return NULL;

  return field_ref(field_info->field_pos, base);
}

const void *field_info_cref(const field_info_t *field_info,  const void *base)
{
  if (!field_info || !base)
    return NULL;

  return field_cref(field_info->field_pos, base);
}

int field_memcmp(const field_info_t *field_info, const void *field_val1, const void *field_val2)
{
  size_t i;
  size_t size = field_info->field_size;

  if (!field_val1)
    return FIELD_MEMCMP_ERR_NULL_FIELD_VAL1;
  if (!field_val2)
    return FIELD_MEMCMP_ERR_NULL_FIELD_VAL2;

  for (i = 0; i < size; ++i)
  {
    unsigned char b1 = ((const unsigned char *) field_val1)[i];
    unsigned char b2 = ((const unsigned char *) field_val2)[i];

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

void *field_memcpy(const field_info_t *field_info, void *dest, const void *src)
{
  if (!dest)
    return NULL;
  if (!src)
    return NULL;

  return memcpy(field_info_ref(field_info, dest), field_info_cref(field_info, src), field_info->field_size);
}

void *field_memmove(const field_info_t *field_info, void *dest, const void *src)
{
  if (!dest)
    return NULL;
  if (!src)
    return NULL;

  return memmove(field_info_ref(field_info, dest), field_info_cref(field_info, src), field_info->field_size);
}

int is_field_template_unused(const field_info_t *field_info, const void *src_field_mem, void *field_working_mem, const memory_manager_t *memory_manager, void *out_field_default_mem)
{
  int      is_unused;

  int      need_free = 0;
  size_t   has_unused;
  size_t (*template_unused_value)(const field_info_t *self, void *dest_mem);

  if (!field_info)
    return -4;
  if (!src_field_mem)
    return -2;

  /* Allocate resources. */
  {
    need_free = 0;
    if (!field_working_mem)
    {
      need_free = 1;

      if (!memory_manager)
        memory_manager = &malloc_manager;

      field_working_mem = memory_manager_malloc(memory_manager, field_info->field_size);

      if (!field_working_mem)
        return -1;
    }
  }
  /* Body: use resources. */
  {
    /* Read in the field's special unused value, to "field_working_mem". */
    template_unused_value = field_info->template_unused_value;
    if (!template_unused_value)
    {
      template_unused_value = field_info_defaults.template_unused_value;
      if (!template_unused_value)
        template_unused_value = template_unused_value_zero;
    }

    has_unused = template_unused_value(field_info, field_working_mem);

    /* Does the field have a special unused value for equality checking? */
    if (!has_unused)
    {
      /* No; we'll just return 0. */
      is_unused = 0;
    }
    else
    {
      /* Yes.  Is "src" equal to this special unused value? */
      if (field_memcmp(field_info, src_field_mem, field_working_mem) != 0)
      {
        /* No; we'll just return 0. */
        is_unused = 0;
      }
      else
      {
        /* Yes.  We'll return >= 1, and output the default value if an output
         * buffer is provided.
         */
        if (!out_field_default_mem)
          is_unused = 1;

        if (out_field_default_mem)
        {
          /* Write the field's default value to "out_field_default_mem". */

          size_t (*default_value)(const field_info_t *self, void *dest_mem);

          is_unused = 2;

          default_value = field_info->default_value;
          if (!default_value)
          {
            default_value = field_info_defaults.default_value;
            if (!default_value)
              default_value = default_value_zero;
          }

          default_value(field_info, out_field_default_mem);

          /* We've written to "out_field_default_mem", so we can't write */
          /* to "field_working_mem" anymore in case it overlaps with     */
          /* "out_field_default_mem".                                    */
          if (need_free)
          {
            need_free = 0;
            memory_manager_free(memory_manager, field_working_mem);
          }
          field_working_mem = NULL;
        }
      }
    }
  }
  /* Free resources. */
  if (need_free)
  {
    memory_manager_free(memory_manager, field_working_mem);
  }

  return is_unused;
}

verify_field_info_status_t verify_field_info(const field_info_t *field_info, char *out_err, size_t err_size)
{
  size_t field_type_size;

  if (!field_info)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_field_info: \"field_info\" is NULL.\n"
        );
    return verify_field_info_null_field_info;
  }

  /* Ensure a type is present. */
  if (!field_info->type)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_field_info: \"field_info\" lacks a type!\n"
        );
    return verify_field_info_no_type;
  }

  /* Ensure the type's size matches field_size if the field's type size is
   * known. */
  field_type_size = type_size(field_info->field_type, NULL);
  if (field_type_size > 0 && field_info->field_size != field_type_size)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_field_info: \"field_size\" is different from the type's size!!\n"
        , "  field_info->field_size:      %d\n"
        , "  type_size(field_info->type): %d\n"
        , (int) field_info->field_size
        , (int) field_type_size
        );
    return verify_field_info_size_mismatch;
  }

  /* The conditions we checked for passed. */
  return verify_field_info_success;
}


/* NULL on success. */
/* TODO: memory_manager! */
const char *field_dup(const field_info_t *field_info, void *dest, const void *src, int defaults_src_unused, int rec_copy, int dup_metadata, ref_traversal_t *vals)
{
  static char         err_buf[DEFAULT_ERR_BUF_SIZE];
  static const size_t err_buf_size = sizeof(err_buf);
  /* static const size_t err_buf_num  = sizeof(err_buf) / sizeof(err_buf[0]); */

  verify_field_info_status_t verify_status;

  if (dest && src && dest == src)
    return NULL;

  /* TODO: We're verifying this each time a field is dup'd! */
  verify_status = verify_field_info(field_info, err_buf, err_buf_size);
  if (!verify_status)
    return err_buf;

  if (!dest)
    return "Error: field_dup: \"dest\" is NULL.\n";
  if (!src)
    return "Error: field_dup: \"src\" is NULL.\n";

  if (field_info->is_metadata)
  {
    if (!dup_metadata)
      return NULL;
  }

  if (field_info->is_recursible_ref && rec_copy != 0)
  {
    tval *type_dup_status;

    if (rec_copy < 0)
      ++rec_copy;

    type_dup_status =
      type_dup
        ( field_info->field_type
        , field_info_ref (field_info, dest)
        , field_info_cref(field_info, src)
        , defaults_src_unused
        , rec_copy
        , dup_metadata
        , vals
        );

    if (!type_dup_status)
    {
      return "Error: field_dup: recursively copying field failed!  \"type_dup\" returned NULL.\n";
    }

    return NULL;
  }
  else
  {
    void *field_memmove_status;

    if (!defaults_src_unused)
    {
      int wrote_default;

      wrote_default = is_field_template_unused(field_info, src, dest, NULL, dest);

      if (wrote_default < 0)
      {
        snprintf
          ( (char *) err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: field_dup: \"is_field_template_unused\" failed!\n"
            "  \"is_field_template_unused\" return error code: %d\n"
          , (int) wrote_default
          );

        return err_buf;
      }

      if (wrote_default)
      {
        return NULL;
      }
    }

    field_memmove_status = field_memmove(field_info, dest, src);
    if (!field_memmove_status)
    {
      return "Error: field_dup: field_memcpy returned NULL, indicating failure.\n";
    }

    /* Done. */
    return NULL;
  }
}

/* ---------------------------------------------------------------- */

/* struct_info type. */

const type_t *struct_info_type(void)
  { return &struct_info_type_def; }

static const char          *struct_info_type_name       (const type_t *self);
static size_t               struct_info_type_size       (const type_t *self, const tval *val);
static const struct_info_t *struct_info_type_is_struct  (const type_t *self);
static const tval          *struct_info_type_has_default(const type_t *self);

const type_t struct_info_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ NULL

  , /* @name                  */ struct_info_type_name
  , /* info                   */ NULL
  , /* @size                  */ struct_info_type_size
  , /* @is_struct             */ struct_info_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ struct_info_type_has_default
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

static const char          *struct_info_type_name       (const type_t *self)
  { return "struct_info_t"; }

static size_t               struct_info_type_size       (const type_t *self, const tval *val)
  { return sizeof(struct_info_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(struct_info)
static const struct_info_t *struct_info_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(struct_info);

    /* typed_t type */
    STRUCT_INFO_RADD(typed_type(), type);

    /* field_info_t fields[STRUCT_INFO_NUM_FIELDS]; */
    /* size_t       fields_len;                     */
    STRUCT_INFO_RADD(array_type(), fields);
    STRUCT_INFO_RADD(size_type(),  fields_len);

    /* struct_info_t *tail; */
    STRUCT_INFO_RADD(objp_type(),  tail);

    /* int       has_memory_tracker; */
    /* size_t    memory_tracker_field; */
    STRUCT_INFO_RADD(int_type(),   has_memory_tracker);
    STRUCT_INFO_RADD(size_type(),  memory_tracker_field);

    STRUCT_INFO_DONE();
  }

static const tval          *struct_info_type_has_default(const type_t *self)
  { return type_has_default_value(self, &struct_info_defaults); }

/* ---------------------------------------------------------------- */

const struct_info_t struct_info_defaults =
  { struct_info_type

  , { { field_info_type, (size_t) (-1) } }
  , 0

  , NULL

  , 0
  , 0
  };

const field_info_t * const field_terminator = &terminating_field_info;

const field_info_t terminating_field_info =
  { field_info_type

  , (size_t) (-1)
  , 0
  , NULL /* TODO: void_type? */

  , 0
  , NULL
  };

/* ---------------------------------------------------------------- */

struct_info_t *struct_info_init
  ( struct_info_t *struct_info
  , size_t      (*default_value)        (const field_info_t *self, void *dest_field_mem)
  , size_t      (*template_unused_value)(const field_info_t *self, void *dest_field_mem)
  )
{
  field_info_t *final_field;

  if (!struct_info)
    return NULL;

  struct_info->type = struct_info_type;

  struct_info->fields_len = 0;

  struct_info->tail = NULL;

  struct_info->has_memory_tracker   = 0;
  struct_info->memory_tracker_field = 0;

  /* Store default_value and template_unused_value in the last element until
   * done writing the struct_info.
   */
  final_field = ARRAY_FINAL_REF(struct_info->fields);
  final_field->type                  = field_info_type;
  final_field->default_value         = default_value;
  final_field->template_unused_value = template_unused_value;

  return struct_info;
}

struct_info_t *struct_info_add_field_info(struct_info_t *struct_info, const field_info_t *field_info)
{
  if (!struct_info)
    return NULL;
  if (!field_info)
    return  NULL;

  if (struct_info->fields_len >= STRUCT_INFO_NUM_FIELDS)
    return NULL;

  if
    (  !struct_info->has_memory_tracker
    && is_subtype(field_info->field_type, memory_tracker_type())
    )
  {
    struct_info->has_memory_tracker   = 1;
    struct_info->memory_tracker_field = struct_info->fields_len;
  }

  struct_info->fields[struct_info->fields_len] = *field_info;
  if (!is_field_terminator(field_info))
    ++struct_info->fields_len;

  return struct_info;
}

struct_info_t *struct_info_add_field
  ( struct_info_t *struct_info
  , ptrdiff_t      field_pos
  , size_t         field_size
  , const type_t  *field_type
  )
{
  field_info_t field_info_def;

  if (!struct_info)
    return NULL;

  field_info_def.type = field_info_type;

  field_info_def.field_pos  = field_pos;
  field_info_def.field_size = field_size;
  field_info_def.field_type = field_type;

  if
    (!
      (  !struct_info->has_memory_tracker
      && is_subtype(field_type, memory_tracker_type())
      )
    )
  {
    field_info_def.is_metadata = 0;
  }
  else
  {
    field_info_def.is_metadata = 1;

    struct_info->has_memory_tracker   = 1;
    struct_info->memory_tracker_field = struct_info->fields_len;
  }

  field_info_def.is_recursible_ref = 0;

  field_info_def.default_value         = ARRAY_FINAL_ELEM(struct_info->fields).default_value;
  field_info_def.template_unused_value = ARRAY_FINAL_ELEM(struct_info->fields).template_unused_value;

  return struct_info_add_field_info(struct_info, &field_info_def);
}

struct_info_t *struct_info_add_field_terminator(struct_info_t *struct_info)
{
  return struct_info_add_field_info(struct_info, field_terminator);
}

field_info_t *struct_info_get_last_field_elem_ref(struct_info_t *struct_info)
{
  if (!struct_info)
    return NULL;

  if (struct_info->fields_len <= 0)
    return NULL;

  return struct_info_index_field_mutable(struct_info, size_less_null(struct_info->fields_len));
}

/* ---------------------------------------------------------------- */

void *struct_info_iterate_chunks
  ( const struct_info_t *struct_info
  , void *(*with_chunk)(void *context, void *last_accumulation, const struct_info_t *struct_info_chunk, int *out_iteration_break)
  , void *context
  , void *initial_accumulation
  )
{
  void *accumulation;
  ref_traversal_t *struct_infos, ref_traversal;

  if (!struct_info)
    return NULL;
  if (!with_chunk)
    return NULL;

  accumulation = initial_accumulation;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, (void *) struct_info)
    ; struct_info
    ; struct_info  = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    int break_iteration = 0;

    accumulation = with_chunk(context, accumulation, struct_info, &break_iteration);

    if (break_iteration)
      break;
  }

  tval_free(struct_infos);

  return accumulation;
}

/*
 * For each field, call the "with_field" callback method with the given
 * context, the accumulation value returned by the last invocation to the
 * "with_field" callback method, or "initial_accumulation" on the first
 * invocation, and the current field, and then return the last accumulation
 * value returned by the last invocation, or "initial_accumulation" if the
 * struct_info is empty, i.e. there are no fields to iterate.
 *
 * Write a non-zero value to "out_iteration_break" to treat the current field as
 * the last.
 */
void *struct_info_iterate_fields
  ( const struct_info_t *struct_info
  , void *(*with_field)(void *context, void *last_accumulation, const field_info_t *field_info, int *out_iteration_break)
  , void *context
  , void *initial_accumulation
  )
{
  void *accumulation;
  ref_traversal_t *struct_infos, ref_traversal;

  if (!struct_info)
    return NULL;
  if (!with_field)
    return NULL;

  accumulation = initial_accumulation;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, (void *) struct_info)
    ; struct_info
    ; struct_info  = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    int break_iteration = 0;

    size_t i;

    if (struct_info->fields_len > STRUCT_INFO_NUM_FIELDS)
      return NULL;

    for (i = 0; i < struct_info->fields_len; ++i)
    {
      accumulation = with_field(context, accumulation, &struct_info->fields[i], &break_iteration);

      if (break_iteration)
        break;
    }

    if (break_iteration)
      break;
  }

  tval_free(struct_infos);

  return accumulation;
}

void *struct_info_iterate_chunks_mutable
  ( struct_info_t *struct_info
  , void *(*with_chunk)(void *context, void *last_accumulation, struct_info_t *struct_info_chunk, int *out_iteration_break)
  , void *context
  , void *initial_accumulation
  )
{
  void *accumulation;
  ref_traversal_t *struct_infos, ref_traversal;

  if (!struct_info)
    return NULL;
  if (!with_chunk)
    return NULL;

  accumulation = initial_accumulation;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, (void *) struct_info)
    ; struct_info
    ; struct_info  = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    int break_iteration = 0;

    accumulation = with_chunk(context, accumulation, struct_info, &break_iteration);

    if (break_iteration)
      break;
  }

  tval_free(struct_infos);

  return accumulation;
}

/*
 * For each field, call the "with_field" callback method with the given
 * context, the accumulation value returned by the last invocation to the
 * "with_field" callback method, or "initial_accumulation" on the first
 * invocation, and the current field, and then return the last accumulation
 * value returned by the last invocation, or "initial_accumulation" if the
 * struct_info is empty, i.e. there are no fields to iterate.
 *
 * Write a non-zero value to "out_iteration_break" to treat the current field as
 * the last.
 */
void *struct_info_iterate_fields_mutable
  ( struct_info_t *struct_info
  , void *(*with_field)(void *context, void *last_accumulation, field_info_t *field_info, int *out_iteration_break)
  , void *context
  , void *initial_accumulation
  )
{
  void *accumulation;
  ref_traversal_t *struct_infos, ref_traversal;

  if (!struct_info)
    return NULL;
  if (!with_field)
    return NULL;

  accumulation = initial_accumulation;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, (void *) struct_info)
    ; struct_info
    ; struct_info  = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    int break_iteration = 0;

    size_t i;

    if (struct_info->fields_len > STRUCT_INFO_NUM_FIELDS)
      return NULL;

    for (i = 0; i < struct_info->fields_len; ++i)
    {
      accumulation = with_field(context, accumulation, &struct_info->fields[i], &break_iteration);

      if (break_iteration)
        break;
    }

    if (break_iteration)
      break;
  }

  tval_free(struct_infos);

  return accumulation;
}

/* Get a "struct_info"'s total number of fields, traversing its tails. */
size_t struct_info_num_fields(const struct_info_t *struct_info)
{
  size_t           num_defined_fields;
  ref_traversal_t *struct_infos, ref_traversal;

  num_defined_fields = 0;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, (void *) struct_info)
    ; struct_info
    ; struct_info  = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    if (struct_info->fields_len >= STRUCT_INFO_NUM_FIELDS)
    {
      /* Error: struct_info's fields_len is too big for this chunk. */
      tval_free(struct_infos);
      return 0;
    }

    num_defined_fields += struct_info->fields_len;
  }

  tval_free(struct_infos);

  return num_defined_fields;
}

/* Get a "struct_info"'s total number tails. */
size_t struct_info_num_tails(const struct_info_t *struct_info)
{
  size_t num_chunks;
  ref_traversal_t *struct_infos, ref_traversal;

  if (!struct_info)
    return 0;

  num_chunks = 0;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, (void *) struct_info)
    ; struct_info
    ; struct_info  = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    ++num_chunks;
  }

  tval_free(struct_infos);

  return (size_t) (size_less_null(num_chunks));
}

/* Get a "struct_info"'s field, returning NULL if "index" is out of bounds for
 * this "struct_info".
 */
const field_info_t *struct_info_index_field(const struct_info_t *struct_info, size_t index)
{
  const field_info_t *field_info;
  ref_traversal_t    *struct_infos, ref_traversal;

  field_info = NULL;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, (void *) struct_info)
    ; struct_info
    ; struct_info  = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    if (struct_info->fields_len >= STRUCT_INFO_NUM_FIELDS)
    {
      /* Error: struct_info's fields_len is too big for this chunk. */
      field_info = NULL;
      break;
    }

    /* Is "index" refer to a field in this chunk? */
    if (!(index < struct_info->fields_len))
    {
      index = size_minus(index, struct_info->fields_len);
    }
    else
    {
      /* Found the field. */
      field_info = &struct_info->fields[index];

      break;
    }
  }

  tval_free(struct_infos);

  return field_info;
}

/* Get a "struct_info"'s field, returning NULL if "index" is out of bounds for
 * this "struct_info".
 */
field_info_t *struct_info_index_field_mutable(struct_info_t *struct_info, size_t index)
{
  field_info_t *field_info;
  ref_traversal_t    *struct_infos, ref_traversal;

  field_info = NULL;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, struct_info)
    ; struct_info
    ; struct_info  = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    if (struct_info->fields_len >= STRUCT_INFO_NUM_FIELDS)
    {
      /* Error: struct_info's fields_len is too big for this chunk. */
      field_info = NULL;
      break;
    }

    /* Is "index" refer to a field in this chunk? */
    if (!(index < struct_info->fields_len))
    {
      index = size_minus(index, struct_info->fields_len);
    }
    else
    {
      /* Found the field. */
      field_info = &struct_info->fields[index];

      break;
    }
  }

  tval_free(struct_infos);

  return field_info;
}

const field_info_t *struct_info_has_typed_field(const struct_info_t *struct_info)
{
  const field_info_t *field_info;

  if (!struct_info)
      return NULL;

  field_info = struct_info_index_field(struct_info, STRUCT_INFO_TYPED_FIELD);

  if (!field_info)
    return NULL;

  if (field_info->field_type != STRUCT_INFO_TYPED_FIELD_TYPE)
    return  NULL;

  return field_info;
}

const field_info_t *struct_info_has_memory_tracker(const struct_info_t *struct_info)
{
  if (!struct_info)
    return NULL;

  if (!struct_info->has_memory_tracker)
    return NULL;

  return struct_info_index_field(struct_info, struct_info->memory_tracker_field);
}

typed_t struct_value_has_typed_field(const struct_info_t *struct_info, const void *val)
{
  struct { typed_t fun; } fun;
  const void **funp = (const void **) &fun;

  const field_info_t *field_info;

  if (!struct_info || !val)
    return NULL;

  field_info = struct_info_has_typed_field(struct_info);

  if (!field_info)
    return NULL;

  *funp = field_info_cref(field_info, val);

  return fun.fun;
}

memory_tracker_t *struct_value_has_memory_tracker(const struct_info_t *struct_info, void *val)
{
  const field_info_t *field_info;

  if (!struct_info || !val)
    return NULL;

  field_info = struct_info_has_memory_tracker(struct_info);

  if (!field_info)
    return NULL;

  return field_info_ref(field_info, val);
}

verify_struct_info_status_t verify_struct_info_chunk(const struct_info_t *struct_info, char *out_err, size_t err_size)
{
  size_t i;
  size_t num_fields;

  if (!struct_info)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_struct_info: \"struct_info\" is NULL.\n"
        );
    return verify_struct_info_null_struct_info;
  }

  num_fields = struct_info->fields_len;

  /* Make sure fields_len isn't too big.   */
  if (num_fields >  STRUCT_INFO_NUM_FIELDS)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_struct_info: fields_len is too big!  Exceeds STRUCT_INFO_NUM_FIELDS capacity.\n"
          "  fields_len:             %d\n"
          "  STRUCT_INFO_NUM_FIELDS: %d\n"
        , struct_info->fields_len
        , STRUCT_INFO_NUM_FIELDS
        );
    return verify_struct_info_need_tail_max;
  }

  /* Make sure fields_len isn't too big.   */
  /*                                       */
  /* Ideally, struct_infos would still be  */
  /* terminated at max capacity before its */
  /* tail is filled, but so long as it has */
  /* a tail, we'll permit this.            */
  /*                                       */
  /* We'll still verify tails, so          */
  /* ultimately there still must be a      */
  /* field terminator.                     */
  if (num_fields >= STRUCT_INFO_NUM_FIELDS && !struct_info->tail)
  {
    if (out_err)
      snprintf
        ( (char *) out_err, (size_t) terminator_size(err_size)
        , "Error: verify_struct_info: fields is not terminated with field_terminator!\n"
          "  fields is at maximum capacity, but there is no tail, so there is no space\n"
          "  for a field_terminator.\n"
          "\n"
          "  fields_len:             %d\n"
          "  STRUCT_INFO_NUM_FIELDS: %d\n"
          "  tail is NULL.\n"
        , struct_info->fields_len
        , STRUCT_INFO_NUM_FIELDS
        );
    return verify_struct_info_need_tail_excess;
  }

  /* Make sure fields is field-terminated. */
  if (num_fields < STRUCT_INFO_NUM_FIELDS)
  {
    int terminated;

    terminated = is_field_terminator(&struct_info->fields[num_fields]);
    if (terminated < 0)
    {
      /* Something went wrong. */
      if (out_err)
        snprintf
          ( (char *) out_err, (size_t) terminator_size(err_size)
          , "Error: verify_struct_info: verifying fields is field-terminated failed!\n"
            "  \"is_field_terminator\" failed with a general error code: %d\n"
            "\n"
            "  fields_len:             %d\n"
            "  STRUCT_INFO_NUM_FIELDS: %d\n"
          , terminated
          , (int) struct_info->fields_len
          , (int) STRUCT_INFO_NUM_FIELDS
          );
      return verify_struct_info_is_field_terminator_error;
    }

    if (!terminated)
    {
      /* A field_terminator does not occur at the end. */
      if (out_err)
        snprintf
          ( (char *) out_err, (size_t) terminator_size(err_size)
          , "Error: verify_struct_info: the field array does not end with field_terminator!\n"
            "  A struct_info's array of fields must be immediately followed by \"field_terminator\".\n"
            "  \"field_terminator\" helps ensure that a \"struct_info\" is initialized correctly, with\n"
            "  a correct length value.\n"
            "\n"
            "  Was fields_len set to the correct length?\n"
            "\n"
            "  fields_len:             %d\n"
            "  STRUCT_INFO_NUM_FIELDS: %d\n"
          , (int) struct_info->fields_len
          , (int) STRUCT_INFO_NUM_FIELDS
          );
      return verify_struct_info_is_field_terminator_error;
    }
  }

  /* Verify each field. */
  for (i = 0; i < num_fields; ++i)
  {
    static const char           prefix[]    =
      "Error: verify_struct_info: verification of a field failed!\n"
      "  \"verify_field_info\" failed.  Its error message is provided below:\n"
      "\n"
      "----------------------------------------------------------------\n"
      ;
    /* static const size_t         prefix_size = sizeof(prefix); */
    size_t                      prefix_len;

    verify_field_info_status_t field_status;

    prefix_len = strlen(prefix);

    if (out_err)
      field_status = verify_field_info(&struct_info->fields[i], out_err + prefix_len, size_minus(err_size, prefix_len));
    else
      field_status = verify_field_info(&struct_info->fields[i], out_err, err_size);
    if (field_status != verify_field_info_success)
    {
      verify_struct_info_status_t status;

      status =
          verify_struct_info_verify_field_info_error
        | (field_status << verify_struct_info_bits)
        ;

      if (out_err)
      {
        int    width;
        size_t err_len;

        /* Write prefix.  */
        memcpy(out_err, prefix, min_size(prefix_len, terminator_size(err_size)));

        /* verify_field_info already wrote its error message. */

        /* Append suffix. */
        err_len = strlen(out_err);

        /* (Get width.) */
        snprintf
          ( (char *) (out_err + err_len), (size_t) terminator_size(size_minus(err_size, err_len))
          , "%d"
          , max_int((int) i, max_int((int) struct_info->fields_len, (int) STRUCT_INFO_NUM_FIELDS))
          );
        if (terminator_size(size_minus(err_size, err_len)) > 0)
          width = strlen(out_err + err_len);
        else
          width = 0;
        snprintf
          ( (char *) (out_err + err_len), (size_t) terminator_size(size_minus(err_size, err_len))
          , ""
          );

        snprintf
          ( (char *) (out_err + err_len), (size_t) terminator_size(size_minus(err_size, err_len))
          , "----------------------------------------------------------------\n"
            "\n"
            "  field number  - - - - - - - - - - - - - - - : %*d\n"
            "  field numbers total - - - - - - - - - - - - : %*d\n"
            "  STRUCT_INFO_NUM_FIELDS  - - - - - - - - - - : %*d\n"
            "  verify_field_info error code  - - - - - - - : %d\n"
            "  verify_struct_info_verify_field_info_error  : %d\n"
            "  verify_struct_info_bits - - - - - - - - - - : %d\n"
            "  verify_struct_info returning with error code: %d\n"

          , (int) width
          , (int) i

          , (int) width
          , (int) struct_info->fields_len

          , (int) width
          , (int) STRUCT_INFO_NUM_FIELDS

          , (int) field_status
          , (int) verify_struct_info_verify_field_info_error
          , (int) verify_struct_info_bits
          , (int) status
          );
      }

      return status;
    }
  }

  /* Verify field_pos increases. */
  /* TODO: also check between tails! */
  for (i = 1; i < num_fields; ++i)
  {
    if
      ( struct_info->fields[i-1].field_pos
      < struct_info->fields[i  ].field_pos
      )
    {
      continue;
    }
    else
    {
      if (out_err)
      {
        snprintf
          ( (char *) out_err, (size_t) terminator_size(err_size)
          , "Error: verify_struct_info: a field's field_pos is equal or lesser than that of its previous.\n"
          , "  Previous field index: %d\n"
          , "  Current  field index: %d\n"
          , "  Previous field pos  : %d\n"
          , "  Current  field pos  : %d\n"

          , (int) (i-1)
          , (int) (i)
          , (int) (struct_info->fields[i-1].field_pos)
          , (int) (struct_info->fields[i  ].field_pos)
          );
      }

      return verify_struct_info_invalid_field_pos;
    }
  }

  /* The conditions we checked for passed. */
  return verify_struct_info_success;
}

verify_struct_info_status_t verify_struct_info(const struct_info_t *struct_info, char *out_err, size_t err_size)
{
  verify_struct_info_status_t status;
  ref_traversal_t *struct_infos, ref_traversal;

  if (!struct_info)
    return verify_struct_info_null_struct_info;

  status = verify_struct_info_success;
  for
    ( struct_infos = ref_traversal_init_with_one(&ref_traversal, (void *) struct_info)
    ; struct_info
    ; struct_info       = ref_traversal_add(struct_infos, struct_info->tail)
    )
  {
    status = verify_struct_info_chunk(struct_info, out_err, err_size);

    if (status != verify_struct_info_success)
      break;
  }

  tval_free(struct_infos);

  return status;
}

/* NULL on success. */
static const char *struct_dup_recurse(const struct_info_t *struct_info, void *dest, const void *src, int defaults_src_unused, int rec_copy, int dup_metadata, ref_traversal_t *vals)
{
  /* FIXME: don't use global state to write errors! */
  static char         err_buf[DEFAULT_ERR_BUF_SIZE];
  static const size_t err_buf_size = sizeof(err_buf);
  /* static const size_t err_buf_num  = sizeof(err_buf) / sizeof(err_buf[0]); */

  size_t i;

  verify_struct_info_status_t verify_status;

  if (!struct_info || !dest || !src || !vals)
    return "Error: struct_dup_recurse: missing input; an argument is NULL.\n";

  if (dest == src)
    return NULL;

  if (!ref_traversal_tagged_add(vals, STRUCT_DUP_VALS_TAG_SRC,  (void *) src))
    return "Error: struct_dup_recurse: infinite loop in_recursible_ref field in src.\n";
  if (!ref_traversal_tagged_add(vals, STRUCT_DUP_VALS_TAG_DEST, (void *) dest))
    return "Error: struct_dup_recurse: infinite loop in_recursible_ref field in dest.\n";

  /* TODO: We're verifying this each time a struct is dup'd! */
  verify_status = verify_struct_info(struct_info, err_buf, err_buf_size);
  if (!verify_status)
  {
    return err_buf;
  }

  /* Copy each field. */
  for (i = 0; i < struct_info->fields_len; ++i)
  {
    const char *field_error_status;

    field_error_status = field_dup(&struct_info->fields[i], dest, src, defaults_src_unused, rec_copy, dup_metadata, vals);

    if (field_error_status)
      return field_error_status;
  }

  /* Recurse. */
  if (struct_info->tail)
  {
    return struct_dup(struct_info->tail, dest, src, defaults_src_unused, rec_copy, dup_metadata, vals);
  }

  /* Done. */
  return NULL;
}

const char *struct_dup(const struct_info_t *struct_info, void *dest, const void *src, int defaults_src_unused, int rec_copy, int dup_metadata, ref_traversal_t *vals)
{
  if (vals)
  {
    return struct_dup_recurse(struct_info, dest, src, defaults_src_unused, rec_copy, dup_metadata, vals);
  }
  else
  {
    const char *result;

    ref_traversal_t vals_def;

    ref_traversal_init_empty(&vals_def);
    {
      result = struct_dup_recurse(struct_info, dest, src, defaults_src_unused, rec_copy, dup_metadata, vals);
    } tval_free(&vals_def);

    return result;
  }
}

typedef struct
{ const void *check;
  const void *baseline;
  int deep;
  ref_traversal_t *vals;
} struct_cmp_context_t;
typedef struct
{ int result;
} struct_cmp_accumulation_t;
static const struct_cmp_accumulation_t struct_cmp_initial = { 0 };

static void *struct_cmp_with_field(void *context, void *last_accumulation, const field_info_t *field_info, int *out_iteration_break)
{
  struct_cmp_context_t      *tcontext;
  struct_cmp_accumulation_t *taccumulation;

  if (!context || !last_accumulation || !field_info)
  {
    WRITE_OUTPUT(out_iteration_break, 1);
    return NULL;
  }

  tcontext      = context;
  taccumulation = last_accumulation;

  if (taccumulation->result != 0)
  {
    /* Already found a difference. */
    WRITE_OUTPUT(out_iteration_break, 1);
    return taccumulation;
  }

  if (field_info->is_metadata)
  {
    /* Skip this field. */
    return taccumulation;
  }

  /* Recurse? */
  if
    (  (field_info->is_recursible_ref)
    && (tcontext->deep != 0)
    )
  {
    int subdeep;

    subdeep = tcontext->deep;
    if (subdeep < 0)
      ++subdeep;

    taccumulation->result =
      type_cmp
        ( field_info->field_type
        , field_info_cref(field_info, tcontext->check)
        , field_info_cref(field_info, tcontext->baseline)
        , subdeep
        , tcontext->vals
        );
  }
  else
  {
    taccumulation->result =
      field_memcmp
        ( field_info
        , field_info_cref(field_info, tcontext->check)
        , field_info_cref(field_info, tcontext->baseline)
        );
  }

  if (taccumulation->result != 0)
    WRITE_OUTPUT(out_iteration_break, 1);

  return taccumulation;
}

static int struct_cmp_recurse(const struct_info_t *struct_info, const void *check, const void *baseline, int deep, ref_traversal_t *vals)
{
  struct_cmp_context_t      context;
  struct_cmp_accumulation_t accumulation =
    struct_cmp_initial;

  struct_cmp_accumulation_t *result;

  context.check    = check;
  context.baseline = baseline;
  context.deep     = deep;
  context.vals     = vals;

  /* Missing input. */
  if (!struct_info || !check || !baseline || !vals)
    return -1;

  if (check == baseline)
    return 0;

  /* Infinite recursion in "copyable_ref" field. */
  if (!ref_traversal_tagged_add(vals, STRUCT_CMP_VALS_TAG_CHECK,    (void *) check))
    return -1;
  if (!ref_traversal_tagged_add(vals, STRUCT_CMP_VALS_TAG_BASELINE, (void *) baseline))
    return -1;

  result = struct_info_iterate_fields(struct_info, struct_cmp_with_field, &context, &accumulation);
  if (!result)
    return -1;

  return result->result;
}

int struct_cmp(const struct_info_t *struct_info, const void *check, const void *baseline, int deep, ref_traversal_t *vals)
{
  if (vals)
  {
    return struct_cmp_recurse(struct_info, check, baseline, deep, vals);
  }
  else
  {
    int result;

    ref_traversal_t vals_def;

    ref_traversal_init_empty(&vals_def);
    {
      result = struct_cmp_recurse(struct_info, check, baseline, deep, &vals_def);
    } tval_free(&vals_def);

    return result;
  }
}

/* ---------------------------------------------------------------- */
/* type_t                                                           */
/* ---------------------------------------------------------------- */

/* type type. */

const type_t *type_type(void)
  { return &type_type_def; }

static const char          *type_type_name       (const type_t *self);
static size_t               type_type_size       (const type_t *self, const tval *val);
static const struct_info_t *type_type_is_struct  (const type_t *self);
static const tval          *type_type_has_default(const type_t *self);
static void                *type_type_user       (const type_t *self, tval *val);
static const void          *type_type_cuser      (const type_t *self, const tval *val);

const type_t type_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ type_is_typed

  , /* @name                  */ type_type_name
  , /* info                   */ NULL
  , /* @size                  */ type_type_size
  , /* @is_struct             */ type_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ type_type_has_default
  , /* mem                    */ NULL
  , /* mem_init               */ NULL
  , /* mem_is_dyn             */ NULL
  , /* mem_free               */ NULL
  , /* default_memory_manager */ NULL

  , /* dup                    */ NULL

  , /* user                   */ type_type_user
  , /* cuser                  */ type_type_cuser
  , /* cmp                    */ NULL

  , /* parity                 */ ""
  };

static const char          *type_type_name       (const type_t *self)
  { return "type_t"; }

static size_t               type_type_size       (const type_t *self, const tval *val)
  { return sizeof(type_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(type)
static const struct_info_t *type_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(type);

    /* typed_t type */
    STRUCT_INFO_RADD(typed_type(), type);

    /* memory_tracker_t memory; */
    STRUCT_INFO_RADD(memory_tracker_type(), memory);

    /* type_t *self_is_mutable; */
    STRUCT_INFO_RADD(objp_type(), self_is_mutable);

    /* const type_t        *(*typed)      (const type_t *self); */
    STRUCT_INFO_RADD(funp_type(), typed);

    /* const char          *(*name)       (const type_t *self);                           */
    /* const char          *(*info)       ( const type_t *self                            */
    /*                                    , char         *out_info_buf                    */
    /*                                    , size_t        info_buf_size                   */
    /*                                    );                                              */
    /* size_t               (*size)       (const type_t *self, const tval *val);          */
    /* const struct_info_t *(*is_struct)  (const type_t *self);                           */
    /* tval                *(*is_mutable) (const type_t *self, const tval *val);          */
    /* const type_t        *(*is_subtype) ( const type_t *self                            */
    /*                                    , const type_t *is_subtype                      */
    /*                                    );                                              */
    /* const type_t        *(*is_supertype)                                               */
    /*                                    ( const type_t *self                            */
    /*                                    , const type_t *is_supertype                    */
    /*                                    );                                              */
    STRUCT_INFO_RADD(funp_type(), name);
    STRUCT_INFO_RADD(funp_type(), info);
    STRUCT_INFO_RADD(funp_type(), size);
    STRUCT_INFO_RADD(funp_type(), is_struct);
    STRUCT_INFO_RADD(funp_type(), is_mutable);
    STRUCT_INFO_RADD(funp_type(), is_subtype);
    STRUCT_INFO_RADD(funp_type(), is_supertype);

    /* typed_t              (*cons_type)  (const type_t *self);                             */
    /* tval                *(*init)       (const type_t *self, tval *cons);                 */
    /* void                 (*free)       (const type_t *self, tval *val);                  */
    /* const tval          *(*has_default)(const type_t *self);                             */
    /* memory_tracker_t    *(*mem)        (const type_t *self, tval *val_raw);              */
    /* void                *(*mem_init)   ( const type_t *self                              */
    /*                                    , tval         *val_raw                           */
    /*                                    , int           is_dynamically_allocated          */
    /*                                    );                                                */
    /* int                  (*mem_is_dyn) ( const type_t *self                              */
    /*                                    , tval         *val                               */
    /*                                    );                                                */
    /* int                  (*mem_free)   ( const type_t *self                              */
    /*                                    , tval         *val                               */
    /*                                    );                                                */
    /* const memory_manager_t                                                               */
    /*                     *(*default_memory_manager)                                       */
    /*                                    ( const type_t *self                              */
    /*                                    , tval *val                                       */
    /*                                    );                                                */
    STRUCT_INFO_RADD(funp_type(), cons_type);
    STRUCT_INFO_RADD(funp_type(), init);
    STRUCT_INFO_RADD(funp_type(), free);
    STRUCT_INFO_RADD(funp_type(), has_default);
    STRUCT_INFO_RADD(funp_type(), mem);
    STRUCT_INFO_RADD(funp_type(), mem_init);
    STRUCT_INFO_RADD(funp_type(), mem_is_dyn);
    STRUCT_INFO_RADD(funp_type(), mem_free);
    STRUCT_INFO_RADD(funp_type(), default_memory_manager);

    /* tval                *(*dup)        ( const type_t *self             */
    /*                                    , tval *dest                     */
    /*                                    , const tval *src                */
    /*                                    , int defaults_src_unused        */
    /*                                    , int rec_copy                   */
    /*                                    , int dup_metadata               */
    /*                                    , ref_traversal_t *vals          */
    /*                                    );                               */
    STRUCT_INFO_RADD(funp_type(), dup);

    /* void                *(*user)       (const type_t *self, tval *val);       */
    /* const void          *(*cuser)      (const type_t *self, const tval *val); */
    /* int                  (*cmp)        ( const type_t *self                   */
    /*                                    , const tval *check                    */
    /*                                    , const tval *baseline                 */
    /*                                    , int deep                             */
    /*                                    , ref_traversal_t *vals                */
    /*                                    );                                     */
    STRUCT_INFO_RADD(funp_type(), user);
    STRUCT_INFO_RADD(funp_type(), cuser);
    STRUCT_INFO_RADD(funp_type(), cmp);

    /* const char *parity; */
    STRUCT_INFO_RADD(objp_type(), parity);

    STRUCT_INFO_DONE();
  }

static const tval          *type_type_has_default(const type_t *self)
  { return type_has_default_value(self, &type_defaults); }

static void                *type_type_user       (const type_t *self, tval *val)
{
  if (!self || !val)
    return NULL;

  return type_user(val, NULL);
}

static const void          *type_type_cuser      (const type_t *self, const tval *val)
{
  if (!self || !val)
    return NULL;

  return type_cuser(val, NULL);
}

/* ---------------------------------------------------------------- */

/*
 * Procedures on or for "type_t"'s.
 */

void tval_free(tval *val)
{
  type_free(tval_type(val), val);
}

/* ---------------------------------------------------------------- */
/* type_t: Various methods and method helpers.                      */
/* ---------------------------------------------------------------- */

/* typed */

/*
 * type_is_typed_from_struct:
 *
 * Values of this type are "typed" if and only if the type is a struct -
 * "is_struct" returns non-NULL - and its first field has type "typed".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > /-* "mytype" in this case is "typed", because it assigns *-/
 * > /-* "type_is_typed_from_struct" and "mytype" is a struct *-/
 * > /-* whose first field has type "typed" ("typed_t type"). *-/
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...   *-/ ...
 * >     , /-* typed *-/ type_is_typed_from_struct
 * >     , /-* ...   *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
const type_t *type_is_typed_from_struct(const type_t *self)
{
  const struct_info_t *struct_info;

  struct_info = type_is_struct(self);

  if      (!struct_info)
  {
    return type_is_untyped(self);
  }
  else if (!struct_info_has_typed_field(struct_info))
  {
    return type_is_untyped(self);
  }
  else
  {
    return type_is_typed(self);
  }
}

/*
 * Values of this type are "typed", meaning references can be treated as
 * "tval *", indicating that they can be treated as "type_t *" to obtain the
 * type of the value.
 *
 * If the type is a struct, this means that the struct's first field is "typed
 * <name_of_field (conventionally "type">".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...   *-/ ...
 * >     , /-* typed *-/ type_is_typed
 * >     , /-* ...   *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
const type_t *type_is_typed(const type_t *self)
{
  return self;
}

/*
 * Values of this type are not "typed", meaning references cannot necessarily
 * be "tval *".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...   *-/ ...
 * >     , /-* typed *-/ type_is_untyped
 * >     , /-* ...   *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
const type_t *type_is_untyped(const type_t *self)
{
  return NULL;
}

/* name */
const char *type_has_name(const type_t *self, char *out_info_name, size_t name_buf_size, const char *name)
{
  return name;
}

const char *type_has_static_name(const type_t *self, const char *name)
{
  return name;
}

/* info */
const char *type_has_no_info(const type_t *self, char *out_info_buf, size_t info_buf_size)
{
  return NULL;
}

/* size */
size_t type_has_unknown_size(const type_t *self, const tval *val)
{
  if (!val)
    return ((size_t) (TYPE_SIZE_VARIABLE_WIDTH_VALUE));
  else
    return ((size_t) (TYPE_SIZE_UNKNOWN_VALUE));
}

/* is_struct */
const struct_info_t *type_is_not_struct(const type_t *self)
{
  return NULL;
}

/* is_mutable */

/* If the type has a struct, check the following fields for an objp whose
 * value is equal to "val", returning it if so, otherwise NULL:
 *
 * - The field immediately following any designated "memory_tracker" field.
 * - The field immediately following any designated "is_typed" field.
 * - The first field.
 */
tval *type_mutable_from_struct(const type_t *self, const tval *val)
{
  tval *mutable;

  const struct_info_t *struct_info;
  const field_info_t  *field_info;

  /* See whether we have a struct_info. */

  if (!self || val)
    return NULL;

  struct_info = type_is_struct(self);
  if (!struct_info)
    return NULL;

  /* Check each field. */

  /* + 1: The field terminator requirement makes this safe. */
  if ((field_info = struct_info_has_memory_tracker(struct_info)))
    if ((mutable = type_mutable_field(self, val, field_info + 1)))
      return mutable;

  /* + 1: The field terminator requirement makes this safe. */
  if ((field_info = struct_info_has_typed_field(struct_info)))
    if ((mutable = type_mutable_field(self, val, field_info + 1)))
      return mutable;

  if (struct_info_num_fields(struct_info) >= 1)
    if ((field_info = struct_info_index_field(struct_info, 0)))
      if ((mutable = type_mutable_field(self, val, field_info)))
        return mutable;

  return NULL;
}

tval *type_mutable_field(const type_t *self, const tval *val, const field_info_t *self_reference)
{
  tval *mutable;

  if (!self || !val || !self_reference)
    return NULL;

  if (!is_subtype(objp_type(), self_reference->field_type))
    return NULL;

  mutable = (void *) field_info_cref(self_reference, val);

  if ((const tval *) mutable != val)
    return NULL;

  return mutable;
}

tval *type_no_mutable   (const type_t *self, const tval *val)
{
  return NULL;
}

/* is_subtype */
const type_t *type_has_no_nonextensible_subtypes(const type_t *self, const type_t *is_subtype)
{
  if (!self || !is_subtype)
    return NULL;

  if (self == is_subtype)
    return is_subtype;

  return NULL;
}

/* is_supertype */
const type_t *type_has_no_extensible_supertypes(const type_t *self, const type_t *is_supertype)
{
  if (!self || !is_supertype)
    return NULL;

  if (self == is_supertype)
    return self;

  return NULL;
}

/* cons_type */

/*
 * The type's initializer uses "template_cons_t" references.
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...       *-/ ...
 * >     , /-* cons_type *-/ type_has_template_cons_type
 * >     , /-* ...       *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
typed_t type_has_template_cons_type(const type_t *self)
{
  return template_cons_type;
}

/* init */

/*
 * type_has_template_cons_basic_initializer:
 *
 * The type uses conventional "template_cons_t"-based initialization.
 *
 * This function invokes "template_cons_basic_initializer" and nothing else.
 *
 * "template_cons_basic_initializer" uses data, that we obtain here from the
 * type, and the data provided in "cons" to handle dynamic memory allocation
 * when requested and initialization of an existing value from a source value
 * by copying struct fields, if the type is a struct.
 */
tval *type_has_template_cons_basic_initializer(const type_t *type, tval *cons)
{
  static const int allow_alternate_memory_manager =
    1;
  return template_cons_basic_initializer(type, (template_cons_t *) cons, allow_alternate_memory_manager);
}

/*
 * type_has_template_cons_basic_initializer_force_memory_manager:
 *
 * Like "type_has_template_cons_basic_initializer", but prevent a memory
 * manager different from the type's "default_memory_manager" from being used.
 *
 * Normally, in other cases, a memory manager can be provided in a
 * "template_cons_t".
 */
tval *type_has_template_cons_basic_initializer_force_memory_manager(const type_t *type, tval *cons)
{
  static const int allow_alternate_memory_manager =
    0;
  return template_cons_basic_initializer(type, (template_cons_t *) cons, allow_alternate_memory_manager);
}

/*
 * template_cons_basic_initializer:
 *
 * Perform standard, minimum initialization from a "template_cons_t"
 * constructor.
 *
 * A type may want to perform more additionalization once this is finished.
 *
 * This invokes "template_cons_dup_struct" with initialization data obtained
 * from the type.
 *
 * Note: this is not an alternative to a type's initializer, even if it it uses
 * "template_cons_t", because the type may need to perform additional
 * initialization.  Erroneously using this to initialize a value *externally*
 * may result in a corrupt, possibly only partially initialized value.
 */
tval *template_cons_basic_initializer(const type_t *type, template_cons_t *cons, int allow_alternate_memory_manager)
{
  size_t                     size;
  const tval                *default_initials;
  const struct_info_t       *struct_info;
  void                    *(*mem_init)( const tval *self
                                      , tval       *val_raw
                                      , int         is_dynamically_allocated
                                      );
  const tval                *mem_init_object;
  const memory_manager_t    *def_memory_manager;

  if (!type)
  {
    if (cons && cons->out_init_error_msg)
      snprintf
        ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
        , "Error: template_cons_basic_initializer: \"type\" argument is NULL!\n"
          "  Failed to initialize a value without a provided type.\n"
        );

    return NULL;
  }

  size               = type_size(type, NULL);
  default_initials   = type_has_default(type);
  struct_info        = type_is_struct(type);
  mem_init           = template_cons_dup_struct_meminit_type;
  mem_init_object    = (const tval *) type;
  def_memory_manager = type_default_memory_manager(type, NULL);

  return template_cons_dup_struct(cons, size, default_initials, struct_info, mem_init, mem_init_object, def_memory_manager, allow_alternate_memory_manager);
}

/* free */

/*
 * type_has_template_cons_basic_freer:
 *
 * The type uses conventional "template_cons_t"-based initialization,
 * optionally with additional initialization that does not allocate additional
 * resources that require being freed.
 *
 * This function invokes "template_cons_basic_freer" and nothing else.
 */
int type_has_template_cons_basic_freer(const type_t *type, tval *cons)
{
  return template_cons_basic_freer(type, cons);
}

/*
 * template_cons_basic_freer:
 *
 * Free resources allocated by "template_cons_basic_initializer".
 *
 * This can be used by types that use conventional "template_cons_t"-based
 * initialization with "template_cons_basic_initializer".
 *
 * If a type's initializer can allocate additional resources beyond those
 * allocated by "template_cons_basic_initializer", the type's "free" method
 * should also free those that are allocated before returning.
 *
 * This function invokes "template_cons_basic_free" and nothing further.
 *
 * Note: this is not an alternative to a type's freer, even if it it uses
 * "template_cons_t", because the type may have performed additional
 * initialization.  Erroneously using this to free a value *externally*
 * may result in memory leaks or corruption.
 */
int template_cons_basic_freer(const type_t *type, tval *val)
{
  int                      (*mem_free)( const tval *self
                                      , tval       *val
                                      );
  const tval                *mem_free_object;

  if (!type)
  {
    return -6;
  }
  mem_free           = template_cons_free_struct_memfree_type;
  mem_free_object    = (const tval *) type;

  return template_cons_free_struct(val, mem_free, mem_free_object);
}

/* has_default */

/*
 * type_has_no_default_value:
 *
 * This can be used when the type has no default value.
 *
 * Simply returns NULL.
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...         *-/ ...
 * >     , /-* has_default *-/ type_has_no_default_value
 * >     , /-* ...         *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 */
const tval *type_has_no_default_value(const type_t *self)
{
  return NULL;
}

/*
 * type_has_default_value:
 *
 * This can be used when the type has a default value.
 *
 * Simply returns "val".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > static const tval *mytype_has_default(const type_t *self);
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...         *-/ ...
 * >     , /-* has_default *-/ mytype_has_default
 * >     , /-* ...         *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 * >
 * > static const tval *mytype_has_default(const type_t *self)
 * >   { return type_has_default_value(self, mytype_defaults); }
 * >
 * > ...
 * >
 * > const mytype_t mytype_defaults =
 * >   { mytype_type
 * >
 * >   , /-* a *-/ 0
 * >   , /-* b *-/ 0
 * >   };
 */
const tval *type_has_default_value(const type_t *self, const tval *val)
{
  return val;
}

/* mem */

/*
 * type_mem_struct_or_global_dyn:
 *
 * Default memory tracker lookup method.
 *
 * Use the type's "struct_info" to obtain a value's "memory_tracker" field,
 * defaulting to the global memory tracker used valuelessly when absent.
 *
 * The type has no valueless memory unless it lacks a "struct_info" with a
 * designated "memory_tracker" field.
 *
 * This can be used with a default "mem_init" when dynamically allocated values
 * should be tracked inside the value itself.
 *
 * If the type isn't a struct, or if its "struct_info" doesn't reference a
 * memory tracker, then the type has no memory tracker, and we'll return NULL.
 *
 * Otherwise return a reference to the value's memory tracker field.
 */
memory_tracker_t *type_mem_struct_or_global_dyn(const type_t *self, tval *val_raw)
{
  const struct_info_t *struct_info;
  const field_info_t  *memory_tracker_field;

  /* Does the type have a struct_info with a designated memory tracker field?
   */

  struct_info = type_is_struct(self);
  if (!struct_info)
  {
    /* The type uses the global-dynamic-typed-allocations memory tracker valuelessly. */
    return &global_typed_dyn_memory_tracker;
  }

  memory_tracker_field = struct_info_has_memory_tracker(struct_info);
  if (!memory_tracker_field)
  {
    /* The type uses the global-dynamic-typed-allocations memory tracker valuelessly. */
    return &global_typed_dyn_memory_tracker;
  }

  /* Yes, so track memory inside values. */

  if (!val_raw)
  {
    /* The type has no valueless memory tracker. */
    return NULL;
  }

  /* The type has a memory tracker.  */
  /*                                 */
  /* Return a reference to the       */
  /* value's memory tracker field.   */
  return
    (memory_tracker_t *)
      field_info_ref
        ( memory_tracker_field
        , val_raw
        );
}

/*
 * type_mem_valueless:
 *
 * The type has a valueless memory tracker.
 *
 * (If the memory tracker passed is NULL, this behaves like
 * "type_mem_struct_or_global_dyn".)
 */
memory_tracker_t *type_mem_valueless(const type_t *self, tval *val_raw, memory_tracker_t *valueless_memory_tracker)
{
  if (!valueless_memory_tracker)
    return type_mem_struct_or_global_dyn(self, val_raw);

  return valueless_memory_tracker;
}

/* mem_init */

/*
 * type_supports_dynamical_allocation:
 *
 * Default allocation tracking method.
 *
 * This is an alias for "type_mem_init_valueless_or_inside_value".
 *
 * The type has standard memory initialization behaviour, tracking value
 * allocations in the value structs or with the type's memory tracker returned
 * from "type_mem(type, NULL)", depending on whether the type has one.
 *
 * The "mem_init" field for types that support dynamic allocation can use this.
 *
 * This type supports dynamical allocation for any value of this type.
 *
 * If memory tracking is per-value, "val_memory_manager", or
 * "default_memory_manager" if NULL, is used to initialize the value's memory
 * tracker with "memory_tracker_initialize_empty_with_container".
 *
 * This is a wrapper around "mem_init_type_valueless_or_inside_value".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...      *-/ ...
 * >     , /-* mem_init *-/ mytype_mem_init
 * >     , /-* ...      *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 * >
 * > static memory_tracker_t *mytype_mem_init
 * >   ( const type_t *self
 * >   , tval *val_raw
 * >   , int is_dynamically_allocated
 * >   )
 * > {
 * >   return type_supports_dynamical_allocation
 * >     ( self
 * >     , val_raw
 * >     , is_dynamically_allocated
 * >     );
 * > }
 */
void *type_supports_dynamic_allocation
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  )
{
  /* "type_supports_dynamic_allocation" is an alias of */
  /* "type_mem_init_valueless_or_inside_value".        */
  return type_mem_init_valueless_or_inside_value(self, val_raw, is_dynamically_allocated);
}

/*
 * type_supports_dynamical_allocation_only_some_vals:
 *
 * The type has standard memory initialization behaviour, tracking value
 * allocations in the value structs or with the type's memory tracker returned
 * from "type_mem(type, NULL)", depending on whether the type has one.
 *
 * The "mem_init" field for types that support dynamic allocation can use this.
 *
 * This type supports dynamical allocation for only some values of this type.
 *
 * If memory tracking is per-value, "val_memory_manager", or
 * "default_memory_manager" if NULL, is used to initialize the value's memory
 * tracker with "memory_tracker_initialize_empty_with_container".
 *
 * This is a wrapper around "mem_init_type_valueless_or_inside_value".
 */
void *type_supports_dynamic_allocation_only_some_vals
  ( const type_t *self
  , tval *val_raw
  , int   is_dynamically_allocated
  )
{
  static const int are_all_vals_supported =
    0;

  const memory_manager_t *val_memory_manager =
    type_default_memory_manager(self, NULL);

  return
    mem_init_type_valueless_or_inside_value
      ( self
      , val_raw
      , is_dynamically_allocated
      , are_all_vals_supported
      , val_memory_manager

      , NULL
      , 0
      );
}

/*
 * type_mem_init_valueless_or_inside_value:
 *
 * The type has standard memory initialization behaviour, tracking value
 * allocations in the value structs or with the type's memory tracker returned
 * from "type_mem(type, NULL)", depending on whether the type has one.
 *
 * The "mem_init" field for types that support dynamic allocation can use this.
 *
 * This type supports dynamical allocation for any value of this type.
 *
 * If memory tracking is per-value, "val_memory_manager", or
 * "default_memory_manager" if NULL, is used to initialize the value's memory
 * tracker with "memory_tracker_initialize_empty_with_container".
 *
 * This is a wrapper around "mem_init_type_valueless_or_inside_value".
 *
 * Example:
 *
 * > typedef struct mytype_s mytype_t;
 * > struct mytype_s
 * > {
 * >   typed_t type;
 * >
 * >   int a;
 * >   int b;
 * >   /-* ... *-/
 * > };
 * >
 * > const type_t *mytype(void)
 * > {
 * >   static const type_t mytype_def =
 * >     { type_type
 * >
 * >     , /-* ...      *-/ ...
 * >     , /-* mem_init *-/ mytype_mem_init
 * >     , /-* ...      *-/ ...
 * >     };
 * >
 * >   return &mytype_def;
 * > }
 * >
 * > static memory_tracker_t *mytype_mem_init
 * >   ( const type_t *self
 * >   , tval *val_raw
 * >   , int is_dynamically_allocated
 * >   )
 * > {
 * >   return type_supports_dynamical_allocation
 * >     ( self
 * >     , val_raw
 * >     , is_dynamically_allocated
 * >     );
 * > }
 */
void *type_mem_init_valueless_or_inside_value
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  )
{
  static const int are_all_vals_supported =
    1;

  const memory_manager_t *val_memory_manager =
    type_default_memory_manager(self, NULL);

  return
    mem_init_type_valueless_or_inside_value
      ( self
      , val_raw
      , is_dynamically_allocated
      , are_all_vals_supported
      , val_memory_manager

      , NULL
      , 0
      );
}

/*
 * Standard memory initialization behaviour.
 *
 * This is used for "mem_init", which is used for 2 purposes:
 *   1) To track newly dynamically allocated values.
 *   2) To determine whether such dynamic allocation is supported for all
 *      values in general, where "val_raw" is NULL.
 *
 * If the type has its own memory tracker, it is used to track each dynamic
 * value allocation.  A type has its own memory tracker if
 * "type_mem(type, NULL)" returns a "memory_tracker_t".
 *
 * Otherwise, call "type_mem(type, val_raw)" to get a reference to the value's
 * memory tracker, and initialize it with
 * "memory_tracker_initialize_empty_with_container", with "val_memory_manager",
 * or with the type's "default_memory_manager" if none is provided, otherwise
 * with the global "default_memory_manager".
 *
 * This is inappropriate for types that lack a valueless tracker and that
 * associate via "mem" independent memory trackers for each value trackers that
 * exist *outside* the value.
 *
 * val_raw:
 * + NULL:
 *     Check whether we support dynamic memory allocation for all values in
 *     general.
 *
 *     Returns:
 *       NULL:     no such support (or an error occurred).
 *       non-NULL: we can dynamically allocate a value.
 *
 * + otherwise:
 *     Track a new value that might be dynamically allocated.
 *
 *     Returns:
 *       NULL:     either an error occured, or dynamic memory allocation for
 *                 this type is not supported.
 *       non-NULL: success, and dynamic memory allocation for this type is
 *                 supported.
 */
void *mem_init_type_valueless_or_inside_value
  ( const type_t           *type
  , tval                   *val_raw
  , int                     is_dynamically_allocated
  , int                     are_all_vals_supported
  , const memory_manager_t *val_memory_manager

  , char   *out_err_buf
  , size_t  err_buf_size
  )
{
  int               check_dynamic_allocation_support_only;
  memory_tracker_t *memory_tracker;

  check_dynamic_allocation_support_only = 0;
  if (!val_raw)
    check_dynamic_allocation_support_only = 1;

  if (!type)
  {
    /* Error: we need a type! */

    if (check_dynamic_allocation_support_only)
    {
      /* We'll write to err_buf if available, and then just return NULL,
       * indicating no dynamic memory allocation is support.
       */

      /* An error occured. */
      if (out_err_buf)
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_init_type_valueless_or_inside_value: called with a NULL \"type\" argument!\n"
            "  Failed to determine whether dynamic allocation is supported\n"
            "  without a type.\n"
            "  Without a type, returning NULL to indicate\n"
            "  no dynamic memory allocation is supported, without a type.\n"
          );

      /* Error occurred: return NULL. */
      return NULL;
    }
    else
    {
      /* We'll return NULL, indicating failure.
       */

      /* An error occured. */
      if (out_err_buf)
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_init_type_valueless_or_inside_value: called with a NULL \"type\" argument!\n"
            "  Failed to initialize a value's associated memory tracker without a \"type\".\n"
          );

      /* Error occurred: return NULL. */
      return NULL;
    }
  }

  /* Does the type have its own associated memory tracker for tracking values?
   */
  memory_tracker = type_mem(type, NULL);
  if (memory_tracker)
  {
    /* Yes, it has an associated memory tracker. */

    if (check_dynamic_allocation_support_only)
    {
      /* We're checking whether we support dynamic memory allocation.
       *
       * Yes: return non-NULL:
       *
       * "mem_init_type_valueless_or_inside_value" considers the type to
       * support dynamic memory allocation, because the type has an associated
       * memory tracker and "mem_init_type_valueless_or_inside_value" uses it
       * to track newly initialize values.
       */
      return memory_tracker;
    }
    else
    {
      /* We need to handle memory tracking. */
      if (is_dynamically_allocated)
      {
        /* The value was dynamically allocated, so track it. */

        /* TODO: once snprintf_prepend is written, replace with the following: */
        /* memory_tracker = memory_tracker_track_allocation(memory_tracker, val_raw, out_err_buf, err_buf_size); */
        memory_tracker = memory_tracker_track_allocation(memory_tracker, val_raw, NULL, 0);

        if (!memory_tracker)
        {
          /* Failed to track allocation; we'll return NULL. */
          if (out_err_buf)
          {
            /* TODO: snprintf_prepend */
            snprintf
              ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
              , "Error: mem_init_type_valueless_or_inside_value: failed to track dynamic allocation!\n"
                "  Failed to initialize a value's associated memory tracker without successful tracking.\n"
                "  memory_tracker_track_allocation returned NULL, indicating failure.\n"
                "TODO Error message returned from \"memory_tracker_initialize_empty_with_container\": "
              );

            ensure_ascii_ends_in_char
              ( (char *) out_err_buf
              , (size_t) err_buf_size
              , '\n'
              );
          }

          /* Error occurred: return NULL. */
          return NULL;
        }
        else
        {
          /* Tracked allocation, and we have nothing further to check.  */
          /* Return success (non-NULL).                                 */
          return memory_tracker;
        }
      }
      else
      {
        /* We have nothing further to check; return success (non-NULL). */
        return memory_tracker;
      }
    }
  }
  else
  {
    /*
     * The type has no associated memory tracker, but it might associate one to
     * a value.
     */

    if (check_dynamic_allocation_support_only)
    {
      /* val_raw might be NULL.  (In this particular context, it is always
       * NULL.)
       */
      memory_tracker = type_mem(type, val_raw);

      if      (memory_tracker)
      {
        /* Yes, this type supports dynamic memory allocation for "val_raw": */
        /* return non-NULL.                                                 */
        return memory_tracker;
      }
      else if (are_all_vals_supported)
      {
        /* Yes, this type supports dynamic memory allocation,             */
        /* according to the caller via "are_all_vals_supported",          */
        /* so this procedure can be called with a non-NULL "val_raw".     */
        /*                                                                */
        /* Yes: return non-NULL.                                          */
        return (void *) type;
      }
      else
      {
        /* No, this type does not support dynamic memory allocation       */
        /* according to the caller via "are_all_vals_supported".          */
        /*                                                                */
        /* No: return NULL.                                               */
        return NULL;
      }
    }
    else
    {
      /* val_raw might be NULL.  (In this particular context, it is never
       * NULL.)
       */
      memory_tracker = type_mem(type, val_raw);

      if (memory_tracker)
      {
        /* Track the value. */
        const char *is_err;

        if (!val_memory_manager)
          val_memory_manager = type_default_memory_manager(type, NULL);
        if (!val_memory_manager)
          val_memory_manager = default_memory_manager;

        if (is_dynamically_allocated)
          is_err = memory_tracker_initialize_empty_with_container(memory_tracker, val_memory_manager, val_raw);
        else
          is_err = memory_tracker_initialize_empty_with_container(memory_tracker, val_memory_manager, NULL);

        if (is_err)
        {
          /* An error occured. */
          if (out_err_buf)
            snprintf
              ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
              , "Error: mem_init_type_valueless_or_inside_value: initializing the memory tracker with \"memory_tracker_initialize_empty_with_container\" failed!\n"
                "  Failed to initialize a value's associated memory tracker.\n"
                "Error message returned from \"memory_tracker_initialize_empty_with_container\": %s%s"
              , (const char *) is_err
              , (const char *) (ascii_ends_in_char(is_err, '\n') ? "" : "\n")
              );

          /* Error occurred: return NULL. */
          return NULL;
        }
        else
        {
          /* We've succesfully tracked the value. */
          return memory_tracker;
        }
      }
      else
      {
        /* There is no memory tracker associated with the value. */
        if (are_all_vals_supported)
        {
          /* Error: are_all_vals_supported is incorrect! */
          if (out_err_buf)
            snprintf
              ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
              , "Error: mem_init_type_valueless_or_inside_value: the type doesn't associated a memory_tracker via \"mem\" for this value, contrary to the caller!\n"
                "  The caller invoked \"mem_init_type_valueless_or_inside_value\" with the \"are_all_vals_supported\" incorrect *enabled*.\n"
                "  But the type doesn't support a memory tracker for this value (\"val_raw\").\n"
                "  \n"
                "  are_all_vals_supported argument: %d\n"
              , (int) are_all_vals_supported
              );

          return NULL;
        }
        else
        {
          /* Success, but dynamic memory allocation is not supported
           * for this particular value.
           */
          return NULL;
        }
      }
    }
  }
}

/* mem_is_dyn */


/*
 * type_is_dyn_valueless_or_inside_value:
 *
 * According to whether the type has a valueless memory tracker, check whether
 * a value was dynamically allocated, with no further computations.
 *
 * This only calls "mem_is_dyn_valueless_or_inside_value" and does no further processing.
 */
int type_is_dyn_valueless_or_inside_value
  ( const type_t *self
  , tval         *val
  )
{
  return mem_is_dyn_valueless_or_inside_value(self, val, NULL, 0);
}

/*
 * mem_is_dyn_valueless_or_inside_value:
 *
 * According to whether the type has a valueless memory tracker, check whether
 * a value was dynamically allocated.
 */
int mem_is_dyn_valueless_or_inside_value
  ( const type_t *type
  , tval         *val

  , char   *out_err_buf
  , size_t  err_buf_size
  )
{
  memory_tracker_t *valueless_memory_tracker;

  if (!type)
  {
    /* Error: we need a type! */

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_is_dyn_valueless_or_inside_value: called with a NULL \"type\" argument!\n"
          "  Failed to determine whether a value was dynamically allocated, with no provided type.\n"
        );

    /* Error occurred: return <= -1. */
    return -1;
  }

  if (!val)
  {
    /* Error: we need a val! */

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_is_dyn_valueless_or_inside_value: called with a NULL \"val\" argument!\n"
          "  Failed to determine whether a value was dynamically allocated, with no provided value reference.\n"
        );

    /* Error occurred: return <= -1. */
    return -2;
  }

  /* Does the type have its own associated memory tracker for tracking
   * values?
   */
  valueless_memory_tracker = type_mem(type, NULL);
  if (valueless_memory_tracker)
  {
    int is_value_tracked;

    /* TODO use "out_err_buf" once snprintf_prepend is implemented. */
    is_value_tracked = memory_tracker_is_allocation_tracked(valueless_memory_tracker, val, NULL, 0);

    if (is_value_tracked < 0)
    {
      /* Error: memory_tracker_is_allocation_tracked failed! */

      int error_code;

      if (out_err_buf)
      {
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_is_dyn_valueless_or_inside_value: calling \"memory_tracker_is_allocation_tracked\" failed!\n"
            "  Failed to determine whether a value was dynamically allocated,\n"
            "  without a successful invocation to \"memory_tracker_is_allocation_tracked\"\n"
            "  for the valuess type-associated memory tracker.\n"
            "\n"
            "  \"memory_tracker_is_allocation_tracked\" returned this error code: %d\n"
            "\n"
            "TODO memory_tracker_is_allocation_tracked's error message: "

          , (int) is_value_tracked
          );

          ensure_ascii_ends_in_char
            ( (char *) out_err_buf
            , (size_t) err_buf_size
            , '\n'
            );
      }

      /* Error occurred: return <= -1. */
      error_code = is_value_tracked - 16;
      if (!(error_code <= 0))
        error_code = -16;
      return (int) error_code;
    }

    if (!is_value_tracked)
    {
      /* No, the value is not dynamically allocated. */

      return 0;
    }
    else
    {
      /* Yes, the value is dynamically allocated. */

      int success_code_yes;

      success_code_yes = is_value_tracked + 8 + 1;
      if (!(success_code_yes >= 1))
        success_code_yes = 8;
      return success_code_yes;
    }
  }
  else
  {
    memory_tracker_t *memory_tracker;
    void             *memory_tracker_container;

    memory_tracker = type_mem(type, val);

    if (!memory_tracker)
    {
      /* Error: the value should have a memory tracker! */

      if (out_err_buf)
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_is_dyn_valueless_or_inside_value: the type doesn't associate the value with a memory tracker!\n"
            "  Failed to determine whether a value was dynamically allocated,\n"
            "  without a valueless type-associated memory tracker or a memory tracker inside the value.\n"
          );

      /* Error occurred: return <= -1. */
      return -3;
    }

    memory_tracker_container = memory_tracker->dynamically_allocated_container;

    if      (!memory_tracker_container)
    {
      /* This value was not dynamically allocated. */
      return 0;
    }
    else if (memory_tracker_container == (void *) val)
    {
      /* This value was dynamically allocated. */
      return 1;
    }
    else
    {
      /* Error: "dynamically_allocated_container" should */
      /* reference the value, but it doesn't!            */

      if (out_err_buf)
        snprintf
          ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
          , "Error: mem_is_dyn_valueless_or_inside_value: the type's memory tracker has a container that differs from the value!\n"
            "  Failed to determine whether a value was dynamically allocated,\n"
            "  with an incorrect \"dynamically_allocated_container\" value.\n"
            "\n"
            "  If the container is non-NULL, these references should be equal, but aren't.\n"
            "\n"
            "  memory_tracker->dynamically_allocated_container: %p\n"
            "  val (the value)  - - - - - - - - - - - - - - - : %p\n"

          , (void *) memory_tracker_container
          , (void *) val
          );

      /* Error occurred: return <= -1. */
      return -4;
    }
  }
}

/* mem_free */

/*
 * type_mem_free_valueless_or_inside_value_allocation:
 *
 * The type uses standard dynamic allocation, only tracking the allocation
 * buffer for dynamically allocated values.
 *
 * If a type lacks a valueless memory tracker, then values are assumed to
 * contain the memory tracker, and the value itself is freed to free the memory
 * tracker, with "memory_tracker_free_container".
 *
 * This will free tracked memory allocated by the
 * "type_supports_dynamic_allocation*" methods.
 *
 * Don't assign a type's "mem_free" method to this if the type lacks a
 * valueless memory tracker and values themselves also lack a memory tracker;
 * in this case dynamic memory allocation is usually not supported for this
 * type (not recommended).
 */
int type_mem_free_valueless_or_inside_value_allocation
  ( const type_t *type
  , tval         *val
  )
{
  return mem_free_valueless_or_inside_value_allocation(type, val, NULL, 0);
}

/* TODO TODO TODO TODO FIXME FIXME FIXME FIXME: if "container" is not equal to
 * the value, e.g. if "container" points to the memory tracker itself, then
 * what?
 * Oh, oh, add a new field to memory_tracker_t!
 *
 * AFFECTS: at *least* mem_free and is_dyn methods!!
 */
/*
 * mem_free_valueless_or_inside_value_allocation:
 *
 * Free memory allocated by "mem_init_type_valueless_or_inside_value".
 *
 * If a type lacks a valueless memory tracker, then values are assumed to
 * contain the memory tracker, and the value itself is freed to free the memory
 * tracker, with "memory_tracker_free_container".
 *
 * If a type performs additional memory tracking for dynamic allocation (as
 * opposed to other memory tracking, e.g. at runtime for dynamically allocated
 * fields; this is something that "free" should handle), it should free these
 * too after calling "mem_free_valueless_or_inside_value_allocation" before
 * returning, but usually this isn't the case.
 *
 * Like "mem_init_type_valueless_or_inside_value", the behaviour of this
 * procedure depends on whether the type is associated with its own valueless
 * memory tracker ("type_mem(type, NULL) != NULL").  If so, remove the
 * reference from the type's valueless memory tracker when freeing after
 * freeing a dynamically allocated value and then return (>= 1).  Otherwise,
 * just free the value after freeing its trakced memory.
 */
int mem_free_valueless_or_inside_value_allocation
  ( const type_t *type
  , tval          *val

  , char   *out_err_buf
  , size_t  err_buf_size
  )
{
  int is_dynamically_allocated;

  if (!type)
  {
    /* Error: we need a type! */

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_free_valueless_or_inside_value_allocation: called with a NULL \"type\" argument!\n"
          "  Failed to free a value's dynamically allocated memory with no provided type.\n"
        );

    /* Error occurred: return <= -1. */
    return -2;
  }

  if (!val)
  {
    /* Error: we need a val! */

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_free_valueless_or_inside_value_allocation: called with a NULL \"val\" argument!\n"
          "  Failed to free a value's dynamically allocated memory with no provided value reference.\n"
        );

    /* Error occurred: return <= -1. */
    return -3;
  }

  is_dynamically_allocated = type_mem_is_dyn(type, val);
  if (is_dynamically_allocated < 0)
  {
    /* Error: the type needs mem_is_dyn! */

    int error_code;

    if (out_err_buf)
      snprintf
        ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
        , "Error: mem_free_valueless_or_inside_value_allocation: calling the type's \"mem_is_dyn\" method failed!\n"
          "  Failed to free a value's dynamically allocated memory without succesfully\n"
          "  determining whether this value was dynamically allocated.\n"
          "\n"
          "  The type's \"mem_is_dyn\" method returned this error code: %d\n"

        , (int) is_dynamically_allocated
        );

    /* Error occurred: return <= -1. */
    error_code = is_dynamically_allocated - 16;
    if (!(error_code <= 0))
      error_code = -16;
    return (int) error_code;
  }

  if (!is_dynamically_allocated)
  {
    return 0;
  }
  else
  {
    /* Does the type have its own associated memory tracker for tracking
     * values?
     */

    memory_tracker_t *valueless_memory_tracker;
    
    valueless_memory_tracker = type_mem(type, NULL);
    if (valueless_memory_tracker)
    {
      int success_code;
      int free_allocation_result;

      /* TODO use "out_err_buf" once snprintf_prepend is implemented. */
      free_allocation_result = memory_tracker_free_allocation(valueless_memory_tracker, val, 0, NULL, 0);

      if (free_allocation_result <= -1)
      {
        /* "memory_tracker_free_allocation" failed. */

        int error_code;

        if (out_err_buf)
        {
          /* TODO use "snprintf_prepend" once snprintf_prepend is implemented. */
          snprintf
            ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
            , "Error: mem_free_valueless_or_inside_value_allocation: the call to \"memory_tracker_free_allocation\" failed!\n"
              "  Failed to free a value's valueless-type-memory-tracker allocation\n"
              "  of dynamically allocated memory without a successful invocation\n"
              "  of \"memory_tracker_free_allocation\".\n"
              "\n"
              "  memory_tracker_free_allocation returned error code: %d\n"
              "\n"
              "TODO memory_tracker_free_allocation's error message: "

            , (int) free_allocation_result
            );

            ensure_ascii_ends_in_char
              ( (char *) out_err_buf
              , (size_t) err_buf_size
              , '\n'
              );
        }

        /* Return failure. */
        error_code = free_allocation_result - 64;
        if (!(error_code <= -1))
          error_code = -64;
        return error_code;
      }

      /* Done freeing. */
      success_code = free_allocation_result + 16 + 1;
      if (!(success_code >= 1))
        success_code = 16;
      return success_code;
    }
    else
    {
      int success_code;
      int free_container_result;

      memory_tracker_t *memory_tracker;

      memory_tracker = type_mem(type, val);
      if (!memory_tracker)
      {
        /* Uh oh: we need an associated memory tracker! */

        if (out_err_buf)
          snprintf
            ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
            , "Error: mem_free_valueless_or_inside_value_allocation: the type lacks a memory tracker association for the value!\n"
              "  Failed to free a value's dynamically allocated memory with no associated memory tracker.\n"
            );

        /* Error occurred: return <= -1. */
        return -4;
      }

      /* ---------------------------------------------------------------- */

      /* TODO use "out_err_buf" once snprintf_prepend is implemented. */
      free_container_result = memory_tracker_free_container(memory_tracker, NULL, 0);

      if (free_container_result <= -1)
      {
        /* "memory_tracker_free_container" failed. */

        int error_code;

        if (out_err_buf)
        {
          /* TODO use "snprintf_prepend" once snprintf_prepend is implemented. */
          snprintf
            ( (char *) out_err_buf, (size_t) terminator_size(err_buf_size)
            , "Error: mem_free_valueless_or_inside_value_allocation: the call to \"memory_tracker_free_container\" failed!\n"
              "  Failed to free a no-valueless-memory-tracker-typed value's\n"
              "  dynamically allocated memory without a successful invocation\n"
              "  of \"memory_tracker_free_container\".\n"
              "\n"
              "  memory_tracker_free_container returned error code: %d\n"
              "\n"
              "TODO memory_tracker_free_container's error message: "

            , (int) free_container_result
            );

            ensure_ascii_ends_in_char
              ( (char *) out_err_buf
              , (size_t) err_buf_size
              , '\n'
              );
        }

        /* Return failure. */
        error_code = free_container_result - 32;
        if (!(error_code <= -1))
          error_code = -32;
        return error_code;
      }

      /* Done freeing. */
      success_code = free_container_result + 32 + 1;
      if (!(success_code >= 1))
        success_code = 32;
      return success_code;
    }
  }
}

/* default_memory_manager */

const memory_manager_t *type_has_no_default_memory_manager(const type_t *self, tval *val)
{
  return type_prefers_given_memory_manager(self, val, NULL);
}

const memory_manager_t *type_prefers_given_memory_manager(const type_t *self, tval *val, const memory_manager_t *memory_manager)
{
  if (val)
  {
    memory_tracker_t *memory_tracker;

    memory_tracker = type_mem(self, val);

    if (memory_tracker)
    {
      /* Return the value's memory tracker's memory manager. */
      return &memory_tracker->memory_manager;
    }
    else
    {
      memory_tracker = type_mem(self, NULL);

      if (memory_tracker)
      {
        /* Return the type's memory tracker's memory manager. */
        return &memory_tracker->memory_manager;
      }
      else
      {
        return memory_manager;
      }
    }
  }
  else
  {
    return memory_manager;
  }
}

const memory_manager_t *type_prefers_default_memory_manager(const type_t *self, tval *val)
{
  return type_prefers_given_memory_manager(self, val, default_memory_manager);
}

const memory_manager_t *type_prefers_malloc_memory_manager(const type_t *self, tval *val)
{
  return type_prefers_given_memory_manager(self, val, &malloc_manager);
}

/* dup */

/*
 * type_has_struct_dup_allow_malloc:
 *
 * Copy "src" into "dest" with "dup_struct".
 *
 * If "src" is NULL, use the type's default value.  (If this is also NULL, fail
 * and return NULL.)
 *
 * Allows malloc: if "dest" is NULL, request allocation of a new one with
 * "type_init(self, NULL)".
 */
tval *type_has_struct_dup_allow_malloc( const type_t *self
                                      , tval *dest
                                      , const tval *src
                                      , int defaults_src_unused
                                      , int rec_copy
                                      , int dup_metadata
                                      , ref_traversal_t *ref_traversal
                                      )
{
  int dyn_alloc;
  const struct_info_t *struct_info;

  const char *struct_dup_is_err;

  struct_info = type_is_struct(self);

  if (!src)
  {
    src  = type_has_default(self);
  }
  dyn_alloc = 0;
  if (!dest)
  {
    dyn_alloc = 1;

    dest = type_init(self, NULL);

    if (!dest)
      return NULL;
  }

  if (!dest || !src || !struct_info)
  {
    if (dyn_alloc)
    {
      type_free(self, dest);

      dyn_alloc = 0;
    }

    return NULL;
  }

  struct_dup_is_err =
    struct_dup( struct_info
              , dest
              , src
              , defaults_src_unused
              , rec_copy
              , dup_metadata
              , ref_traversal
              );

  if (struct_dup_is_err)
  {
    if (dyn_alloc)
    {
      type_free(self, dest);

      dyn_alloc = 0;
    }

    return NULL;
  }

  return dest;
}

/*
 * type_has_struct_dup_never_malloc:
 *
 * Copy "src" into "dest" with "dup_struct".
 *
 * If "src" is NULL, use the type's default value.  (If this is also NULL, fail
 * and return NULL.)
 *
 * If "dest" is NULL, returns NULL.
 */
tval *type_has_struct_dup_never_malloc( const type_t *self
                                      , tval *dest
                                      , const tval *src
                                      , int defaults_src_unused
                                      , int rec_copy
                                      , int dup_metadata
                                      , ref_traversal_t *ref_traversal
                                      )
{
  const struct_info_t *struct_info;

  const char *struct_dup_is_err;

  struct_info = type_is_struct(self);

  if (!src)
    src  = type_has_default(self);

  if (!dest || !src || !struct_info)
    return NULL;

  struct_dup_is_err =
    struct_dup( struct_info
              , dest
              , src
              , defaults_src_unused
              , rec_copy
              , dup_metadata
              , ref_traversal
              );

  if (struct_dup_is_err)
    return NULL;

  return dest;
}

/* user */
void *type_has_no_user_data(const type_t *self, tval *val)
{
  return NULL;
}

/* cuser */
const void *type_has_no_cuser_data(const type_t *self, const tval *val)
{
  return NULL;
}

/* cmp */
/* Compare fields if struct, otherwise compare memory. */
int type_has_standard_cmp(const type_t *self, const tval *check, const tval *baseline, int deep, ref_traversal_t *vals)
{
  const struct_info_t *struct_info;

  if (!self || !check || !baseline)
    return -1;

  if ((struct_info = type_is_struct(self)))
  {
    return struct_cmp(struct_info, check, baseline, deep, vals);
  }
  else
  {
    size_t check_size;
    size_t baseline_size;

    check_size    = type_size(self, check);
    baseline_size = type_size(self, baseline);

    if (check_size && baseline_size && check_size == baseline_size)
    {
      return memcmp(check, baseline, min_size(check_size, baseline_size));
    }
    else
    {
      return -1;
    }
  }
}

/* ---------------------------------------------------------------- */
/* type_t: Defaults.                                                */
/* ---------------------------------------------------------------- */

/*
 * This section in the "type" module contains default definitions for types.
 *
 * This section is designed to also serve as a template to cargo cult
 * type_t definitions that override the defaults.  A template for 3 fields is
 * provided below, commented out.  A type definition can copy this and make the
 * appropriate changes, starting by replacing "this" with the name of the type.
 *
 * Also, the default definitions can also be copied to override default
 * behaviour, or more commonly to add to the default behaviour, e.g. by
 * performing additional initialization on new values.
 *
 * ----------------------------------------------------------------
 *
 * A "type_t" definition of a type is a struct that contains function pointers
 * to C functions that handle the type.  A type must specify a minimum of 3
 * fields.
 *
 * Every type should be defined and accessed through a function like this:
 *
 * > const type_t *foo_type(void);
 *
 * A function pointer to a function with this type is called "typed_t".
 *
 * > typed_t which_type_is_it = foo_type;
 *
 * A "typed_t" is the core, fundamental part of a type's API.  A public type
 * exports at least this; other parts are optional, e.g. a top-level "typed_t"
 * reference.  Some types are procedurally generated, because they invoke other
 * procedures to generate the type definition, and so can't use a constant
 * initializer.
 *
 * So a minimal type definition requires at least this:
 *
 * > const type_t *<name_of_type>_type(void);
 *
 * This is a function declaration that "typed_t" function pointers can be
 * assigned to.
 *
 * ----------------------------------------------------------------
 *
 * For convenience, some type_t implementations also provide an optional top-level
 * "type_t" reference, but a const "type_t" definition requires a constant
 * initializer, precluding e.g. using the API provided by "type_structs".
 *
 * For example:
 *
 * > const type_t *foo_type(void);
 * > extern const type_t foo_type_def; /-* optional *-/
 * >
 * > ...
 * >
 * > const type_t *foo_type { return &foo_type_def };
 * > const type_t foo_type_def =
 * >   { type_type
 * >
 * >   , ...
 * >   }
 *
 * ----------------------------------------------------------------
 *
 * Typically, a low-level "type_t" definition will look like this:
 *
 * > /-* intpair.h *-/
 * >
 * > ...
 * >
 * > /-* "intpair_type" *-/
 * > const type_t *intpair_type(void);
 * > /-* optional: *-/ extern const type_t intpair_type_def;
 * > typedef struct intpair_s intpair_t;
 * > struct intpair_s
 * > {
 * >   typed_t type;
 * >
 * >   memory_tracker memory;
 * >
 * >   int a;
 * >   int b;
 * > };
 *
 * > /-* intpair.c *-/
 * >
 * > ...
 * >
 * > const type_t *intpair_type(void)
 * >   { return &intpair_type_def; }
 * >
 * > static const char          *intpair_type_name     (const type_t *self);
 * > static size_t               intpair_type_size     (const type_t *self, const tval *val);
 * > static const struct_info_t *intpair_type_is_struct(const type_t *self);
 * >
 * > const type_t intpair_type_def =
 * >   { type_type
 * > 
 * >     /-* @: Required.           *-/
 * > 
 * >   , /-* memory                 *-/ MEMORY_TRACKER_DEFAULTS
 * >   , /-* is_self_mutable        *-/ NULL
 * > 
 * >   , /-* typed                  *-/ NULL
 * > 
 * >   , /-* @name                  *-/ intpair_type_name
 * >   , /-* info                   *-/ NULL
 * >   , /-* @size                  *-/ intpair_type_size
 * >   , /-* @is_struct             *-/ intpair_type_is_struct
 * >   , /-* is_mutable             *-/ NULL
 * >   , /-* is_subtype             *-/ NULL
 * >   , /-* is_supertype           *-/ NULL
 * > 
 * >   , /-* cons_type              *-/ NULL
 * >   , /-* init                   *-/ NULL
 * >   , /-* free                   *-/ NULL
 * >   , /-* has_default            *-/ NULL
 * >   , /-* mem                    *-/ NULL
 * >   , /-* mem_init               *-/ NULL
 * >   , /-* mem_is_dyn             *-/ NULL
 * >   , /-* mem_free               *-/ NULL
 * >   , /-* default_memory_manager *-/ NULL
 * >
 * >   , /-* dup                    *-/ NULL
 * >
 * >   , /-* user                   *-/ NULL
 * >   , /-* cuser                  *-/ NULL
 * >   , /-* cmp                    *-/ NULL
 * > 
 * >   , /-* parity                 *-/ ""
 * >   };
 * >
 * > static const char          *intpair_type_name     (const type_t *self)
 * >   { return "memory_manager"; }
 * >
 * > static size_t               intpair_type_size     (const type_t *self, const tval *val)
 * >   { return sizeof(intpair_t); }
 * >
 * > static const struct_info_t *intpair_type_is_struct(const type_t *self)
 * >   {
 * >     ...
 * >
 * >     return struct_info;
 * >   }
 *
 * ----------------------------------------------------------------
 */

/* TODO: now that you've written common methods and method helpers, you're working on "type_defaults". */
/*
 * Default fields for types.
 *
 * A minimum of 3 fields is required for each type:
 *  - name      (returns const char *):
 *      The name of the type.
 *
 *      Example: "memory_manager".
 *
 *  - size      (returns size_t):
 *      The size of the type.
 *
 *      Example: "sizeof(memory_manager_t)".
 *
 *  - is_struct (returns const struct_info_t *):
 *      The fields of the type's struct, or NULL if the type isn't a struct.
 *
 * The remaining default values, used when a "type_t" method is NULL for each
 * "type_t" method, specify a type that has the following characteristics:
 *
 *   - has_default:
 *       "type_has_no_default":
 *
 *       By default, a type lacks a default value.  A type can specify a
 *       default value by assigning a method that returns it.
 *
 *   - "template_cons_t" constructors:
 *       Initialization uses "template_cons_t" and is based on standard
 *       "template_cons_t"-based methods.
 *
 *   - standard memory tracking:
 *       If the type is a struct and its "struct_info" (i.e. that returned by
 *       "is_struct") references a memory tracker field, it is used when the
 *       type doesn't assign itself a valueless memory tracker (which would
 *       allow the struct to optionall omit a "memory_tracker" field, relocating tracking
 *       of dynamically allocated values to this valueless memory tracker).
 *
 *       (A type can choose a valueless memory tracker by assigning its "mem"
 *       method to a function that wraps "type_mem_valueless", which it calls
 *       with the additional argument of the memory tracker to use as its
 *       valueless memory tracker.)
 *
 *       If a type is not a struct and lacks a valueless memory tracker, the
 *       global memory tracker "global_typed_dyn_memory_tracker" is used to
 *       track dynamic memory allocations.
 *
 *   - whether the type is typed (values are "tval *"s):
 *       "type_is_typed_from_struct":
 *
 *       By default, the type is considered to be "typed" if and only if the
 *       type is a struct whose first field has type "typed", for example
 *       "typed_t type".
 *
 *   - type info:
 *       "type_has_no_info":
 *
 *       By default, a type has no information string.
 */

const type_t type_defaults =
  TYPE_DEFAULTS;

const type_t        *default_type_typed      (const type_t *self)
  { return type_is_typed_from_struct(self); }

/*
const char          *default_type_name       (const type_t *self)
  { return "this"; }
*/

const char          *default_type_info       ( const type_t *type
                                                    , char         *out_info_buf
                                                    , size_t        info_buf_size
                                                    )
  { return type_has_no_info(type, out_info_buf, info_buf_size); }

/*
size_t               default_type_size       (const type_t *self, const tval *val)
  { return sizeof(this_t); }
*/

/*
DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(this)
const struct_info_t *default_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(this);

    /-* typed_t type *-/
    STRUCT_INFO_RADD(typed_type(), type);

    /-* int foo *-/
    STRUCT_INFO_RADD(int_type(),   foo);

    /-* int bar *-/
    STRUCT_INFO_RADD(int_type(),   bar);

    STRUCT_INFO_DONE();
  }
*/

tval                *default_type_is_mutable (const type_t *self, const tval *val)
  { return type_mutable_from_struct(self, val); }

const type_t        *default_type_is_subtype ( const type_t *self
                                             , const type_t *is_subtype
                                             )
  { return type_has_no_nonextensible_subtypes(self, is_subtype); }

const type_t        *default_type_is_supertype
                                             ( const type_t *self
                                             , const type_t *is_supertype
                                             )
  { return type_has_no_extensible_supertypes(self, is_supertype); }

typed_t              default_type_cons_type  (const type_t *self)
  { return type_has_template_cons_type(self); }

tval                *default_type_init       (const type_t *self, tval *cons)
  { return type_has_template_cons_basic_initializer(self, cons); }

void                 default_type_free       (const type_t *self, tval *val)
  {        type_has_template_cons_basic_freer(self, val); }

const tval          *default_type_has_default(const type_t *self)
  { return type_has_no_default_value(self); }

memory_tracker_t    *default_type_mem        (const type_t *self, tval *val_raw)
  { return type_mem_struct_or_global_dyn(self, val_raw); }

void                *default_type_mem_init   ( const type_t *self
                                             , tval *val_raw
                                             , int is_dynamically_allocated
                                             )
  { return type_supports_dynamic_allocation(self, val_raw, is_dynamically_allocated); }

int                  default_type_mem_is_dyn ( const type_t *self
                                             , tval         *val
                                             )
  { return type_is_dyn_valueless_or_inside_value(self, val); }

int                  default_type_mem_free   ( const type_t *self
                                             , tval         *val
                                             )
  { return type_mem_free_valueless_or_inside_value_allocation(self, val); }

const memory_manager_t
                    *default_type_default_memory_manager
                                             ( const type_t *self
                                             , tval *val
                                             )
  { return type_has_no_default_memory_manager(self, val); }

tval                *default_type_dup        ( const type_t *self
                                             , tval *dest
                                             , const tval *src
                                             , int defaults_src_unused
                                             , int rec_copy
                                             , int dup_metadata
                                             , ref_traversal_t *ref_traversal
                                             )
  {
    return
      type_has_struct_dup_allow_malloc
        ( self
        , dest
        , src
        , defaults_src_unused
        , rec_copy
        , dup_metadata
        , ref_traversal
        );
  }

void                *default_type_user       (const type_t *self, tval *val)
  { return type_has_no_user_data(self, val); }

const void          *default_type_cuser      (const type_t *self, const tval *val)
  { return type_has_no_cuser_data(self, val); }

int                  default_type_cmp        ( const type_t *self
                                             , const tval *check
                                             , const tval *baseline
                                             , int deep
                                             , ref_traversal_t *vals
                                             )
  { return type_has_standard_cmp(self, check, baseline, deep, vals); }

/* ---------------------------------------------------------------- */

/*
 * Fundamental "type_t" accessors.
 */

const type_t        *type_typed      (const type_t *type)
{
  if (!type || !type->typed)
    return type_defaults.typed(type);
  else
    return type->typed(type);
}

const char          *type_name       (const type_t *type)
{
  if (!type || !type->name)
    return NULL;
  else
    return type->name(type);
}

const char          *type_info       ( const type_t *type
                                     , char         *out_info_buf
                                     , size_t        info_buf_size
                                     )
{
  if (!type || !type->info)
    return type_defaults.info(type, out_info_buf, info_buf_size);
  else
    return type->info(type, out_info_buf, info_buf_size);
}

size_t               type_size       (const type_t *type, const tval *val)
{
  if (!type || !type->size)
    return 0;
  else
    return type->size(type, val);
}

const struct_info_t *type_is_struct  (const type_t *type)
{
  if (!type || !type->is_struct)
    return NULL;
  else
    return type->is_struct(type);
}

tval                *type_is_mutable (const type_t *type, const tval *val)
{
  if (!type || !type->is_mutable)
    return type_defaults.is_mutable(type, val);
  else
    return type->is_mutable(type, val);
}

const type_t        *type_is_subtype ( const type_t *type
                                     , const type_t *is_subtype
                                     )
{
  if (!type || !type->is_subtype)
    return type_defaults.is_subtype(type, is_subtype);
  else
    return type->is_subtype(type, is_subtype);
}

const type_t        *type_is_supertype
                                     ( const type_t *type
                                     , const type_t *is_supertype
                                     )
{
  if (!type || !type->is_supertype)
    return type_defaults.is_supertype(type, is_supertype);
  else
    return type->is_supertype(type, is_supertype);
}

typed_t              type_cons_type  (const type_t *type)
{
  if (!type || !type->cons_type)
    return type_defaults.cons_type(type);
  else
    return type->cons_type(type);
}

tval                *type_init       (const type_t *type, tval *cons)
{
  if (!type || !type->init)
    return type_defaults.init(type, cons);
  else
    return type->init(type, cons);
}

void                 type_free       (const type_t *type, tval *val)
{
  if (!type || !type->free)
    type_defaults.free(type, val);
  else
    type->free(type, val);
}

const tval          *type_has_default(const type_t *type)
{
  if (!type || !type->has_default)
    return type_defaults.has_default(type);
  else
    return type->has_default(type);
}

memory_tracker_t    *type_mem        (const type_t *type, tval *val_raw)
{
  if (!type || !type->mem)
    return type_defaults.mem(type, val_raw);
  else
    return type->mem(type, val_raw);
}

void                *type_mem_init   ( const type_t *type
                                     , tval *val_raw
                                     , int is_dynamically_allocated
                                     )
{
  if (!type || !type->mem_init)
    return type_defaults.mem_init(type, val_raw, is_dynamically_allocated);
  else
    return type->mem_init(type, val_raw, is_dynamically_allocated);
}

int                  type_mem_is_dyn ( const type_t *type
                                     , tval         *val
                                     )
{
  if (!type || !type->mem_is_dyn)
    return type_defaults.mem_is_dyn(type, val);
  else
    return type->mem_is_dyn(type, val);
}

int                  type_mem_free   ( const type_t *type
                                     , tval         *val
                                     )
{
  if (!type || !type->mem_free)
    return type_defaults.mem_free(type, val);
  else
    return type->mem_free(type, val);
}

const memory_manager_t
                    *type_default_memory_manager
                                     ( const type_t *type
                                     , tval *val
                                     )
{
  if (!type || !type->default_memory_manager)
    return type_defaults.default_memory_manager(type, val);
  else
    return type->default_memory_manager(type, val);
}

tval                *type_dup        ( const type_t *type
                                     , tval *dest
                                     , const tval *src
                                     , int defaults_src_unused
                                     , int rec_copy
                                     , int dup_metadata
                                     , ref_traversal_t *vals
                                     )
{
  if (!type || !type->dup)
    return
      type_defaults.dup
        ( type
        , dest
        , src
        , defaults_src_unused
        , rec_copy
        , dup_metadata
        , vals
        );
  else
    return
      type->dup
        ( type
        , dest
        , src
        , defaults_src_unused
        , rec_copy
        , dup_metadata
        , vals
        );
}

void                *type_user       (const type_t *type, tval *val)
{
  if (!type || !type->user)
    return type_defaults.user(type, val);
  else
    return type->user(type, val);
}

const void          *type_cuser      (const type_t *type, const tval *val)
{
  if (!type || !type->cuser)
    return type_defaults.cuser(type, val);
  else
    return type->cuser(type, val);
}

int                  type_cmp        ( const type_t *type
                                     , const tval *check
                                     , const tval *baseline
                                     , int deep
                                     , ref_traversal_t *vals
                                     )
{
  if (!type || !type->cmp)
    return type_defaults.cmp(type, check, baseline, deep, vals);
  else
    return type->cmp(type, check, baseline, deep, vals);
}

/* ---------------------------------------------------------------- */

/*
 * Compositional "type_t" accessors.
 */

const type_t *is_subtype(const type_t *sub, const type_t *super)
{
  const type_t *is_sub;

  if (!sub || !super)
    return NULL;

  if ((is_sub = type_is_subtype(super, sub)))
  {
    return is_sub;
  }
  else
  {
    if ((is_sub = type_is_supertype(sub, super)))
      return is_sub;
  }

  return NULL;
}

const type_t *is_type_equivalent(const type_t *this, const type_t *that)
{
  const type_t *this_subof_that;
  const type_t *that_subof_this;

  if
    (  (this_subof_that = is_subtype(this, that))
    && (that_subof_this = is_subtype(that, this))
    )
    return is_subtype(this_subof_that, that_subof_this);
  else
    return NULL;
}

const type_t *is_subtype_via(const type_t *sub, const type_t *mid, const type_t *super)
{
  const type_t *is_sub;

  if ((is_sub = is_subtype(is_subtype(sub, mid), super)))
  {
    return is_sub;
  }
  else
  {
    if ((is_sub = is_subtype(sub, is_subtype(mid, super))))
      return is_sub;
  }

  return NULL;
}

const type_t *is_proper_subtype(const type_t *sub, const type_t *super)
{
  if (!sub || !super)
    return NULL;

  if (!is_type_equivalent(sub, super))
  {
    return NULL;
  }
  else
  {
    return is_subtype(sub, super);
  }
}

const type_t *is_supertype(const type_t *super, const type_t *sub)
{
  return is_subtype(sub, super);
}

const type_t *is_supertype_via(const type_t *super, const type_t *mid, const type_t *sub)
{
  return is_subtype_via(sub, mid, super);
}

const type_t *is_proper_supertype(const type_t *super, const type_t *sub)
{
  return is_proper_subtype(sub, super);
}

/* ---------------------------------------------------------------- */
/* Template constructors, available for types to use.               */
/* ---------------------------------------------------------------- */

/* template_cons type. */

const type_t *template_cons_type(void)
  { return &template_cons_type_def; }

static const char          *template_cons_type_name       (const type_t *self);
static size_t               template_cons_type_size       (const type_t *self, const tval *val);
static const struct_info_t *template_cons_type_is_struct  (const type_t *self);
static const tval          *template_cons_type_has_default(const type_t *self);

const type_t template_cons_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ NULL

  , /* @name                  */ template_cons_type_name
  , /* info                   */ NULL
  , /* @size                  */ template_cons_type_size
  , /* @is_struct             */ template_cons_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ template_cons_type_has_default
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

static const char          *template_cons_type_name       (const type_t *self)
  { return "template_cons_t"; }

static size_t               template_cons_type_size       (const type_t *self, const tval *val)
  { return sizeof(template_cons_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(template_cons)
static const struct_info_t *template_cons_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(template_cons);

    /* typed_t type */
    STRUCT_INFO_RADD(typed_type(), type);

    /* tval *dest; */
    STRUCT_INFO_RADD(objp_type(), dest);

    /* const memory_manager_t *memory_manager; */
    STRUCT_INFO_RADD(objp_type(), memory_manager);

    /* const tval *initials; */
    STRUCT_INFO_RADD(objp_type(), initials);

    /* int force_no_defaults; */
    STRUCT_INFO_RADD(int_type(), force_no_defaults);

    /* int initials_copy_rec; */
    STRUCT_INFO_RADD(int_type(), initials_copy_rec);

    /* ref_traversal_t *ref_traversal; */
    STRUCT_INFO_RADD(objp_type(), ref_traversal);

    /* int preserve_metadata; */
    STRUCT_INFO_RADD(int_type(), preserve_metadata);

    /* void *user; */
    STRUCT_INFO_RADD(objp_type(), user);

    /* size_t allocate_only_with_num; */
    STRUCT_INFO_RADD(size_type(), allocate_only_with_num);

    /* char   *out_init_error_msg;  */
    /* size_t  init_error_msg_size; */
    STRUCT_INFO_RADD(objp_type(), out_init_error_msg);
    STRUCT_INFO_RADD(size_type(), init_error_msg_size);

    STRUCT_INFO_DONE();
  }

static const tval          *template_cons_type_has_default(const type_t *self)
  { return type_has_default_value(self, &template_cons_defaults); }

/* ---------------------------------------------------------------- */

const template_cons_t * const default_template_cons = &template_cons_defaults;

const template_cons_t template_cons_defaults =
  TEMPLATE_CONS_DEFAULTS;

/* ---------------------------------------------------------------- */

/*
 * template_cons_dup_struct:
 *
 * TODO
 */
tval *template_cons_dup_struct
  ( const template_cons_t     *cons
  , size_t                     size
  , const tval                *default_initials
  , const struct_info_t       *struct_info
  , void                    *(*mem_init)( const tval *self
                                        , tval       *val_raw
                                        , int         is_dynamically_allocated
                                        )
  , const tval                *mem_init_object
  , const memory_manager_t    *def_memory_manager
  , int                        allow_alternate_memory_manager
  )
{
  /* ---------------------------------------------------------------- */
  /* Setup.                                                           */
  /* ---------------------------------------------------------------- */

  tval             *dest;

  int               is_allocate;
  int               is_allocate_only;

  const char       *is_err;

  const memory_manager_t *memory_manager;

  /* Is "cons" provided? */
  if (!cons)
  {
    cons = default_template_cons;
    if (!cons)
      cons = &template_cons_defaults;
  }

  /* Set "memory_manager" as specified. */
  if (allow_alternate_memory_manager)
  {
    memory_manager = cons->memory_manager;
    if (!memory_manager)
      memory_manager = def_memory_manager;
    if (!memory_manager)
      memory_manager = default_memory_manager;
  }
  else
  {
    memory_manager = def_memory_manager;
    if (!memory_manager)
      memory_manager = default_memory_manager;

    if (cons->memory_manager)
    {
      if (cons->memory_manager != def_memory_manager)
      {
        if (cons->out_init_error_msg)
          snprintf
            ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
            , "Error: template_cons_dup_struct: a single memory_manager is required, but an alternate one was requested.\n"
              "  Failed to initialize a value with a prohibited alternate memory manager.\n"
              "\n"
              "  allow_alternate_memory_manager: %d.\n"
            , (int) allow_alternate_memory_manager
            );

        return NULL;
      }
      else
      {
        if (cons->out_init_error_msg)
        {
          snprintf
            ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
            , "Warning: template_cons_dup_struct: a single memory_manager is required, but an alternate one was requested, but it is the same.\n"
              "  The constructor should not contain a memory_manager_t, but since it is the same as the required.\n"
              "  memory manager, we'll continue anyway.\n"
              "\n"
              "  allow_alternate_memory_manager: %d.\n"
            , (int) allow_alternate_memory_manager
            );

          /* out_init_error_msg will be overwritten in case of further */
          /* warnings or errors.                                       */
        }
      }
    }
  }

  /* ---------------------------------------------------------------- */
  /* Dynamic allocation.                                              */
  /* ---------------------------------------------------------------- */

  /* Are we allocating memory for this value, or only initializing? */
  is_allocate_only = cons->allocate_only_with_num >= 1;
  is_allocate      = !cons->dest || is_allocate_only;

  /* Do we need to dynamically allocate memory for this value? */
  if (is_allocate)
  {
    int    is_array;
    size_t number_to_allocate;

    /* Would "cons->dest" be ignored? */
    if (cons->dest)
    {
      /* Error: "cons->dest" was provided, but since we're dynamically allocating
       * a new value, it would be ignored.
       */
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: \"cons\" provides \"dest\" reference to a value to initialize while enabling allocation.\n"
            "  Aborting cautiously, rather than ignoring \"dest\".\n"
          );

      return NULL;
    }

    /* preserve_metadata is incompatible with allocation. */
    if (cons->preserve_metadata)
    {
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: preserve_metadata flag is set, but the constructor dynamically allocates a value.\n"
            "  Since \"preserve_metadata\" skips memory tracker initialization, tracking this dynamic allocation can't be added to it.\n"
            "  Failed to initialize value.\n"
          );

      return NULL;
    }

    /* We need either "mem_init" or a memory tracker field to track this
     * allocation. */
    if (!mem_init && (!struct_info || !struct_info_has_memory_tracker(struct_info)))
    {
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: constructor dynamically allocates memory, but there is no associated memory tracker!.\n"
            "  Failed to initialize new value, because \"mem_init\" is NULL and no \"struct_info\" with a memory tracker field reference was provided.\n"
          );

      return NULL;
    }

    /* How many values are we allocating? */
    number_to_allocate = cons->allocate_only_with_num;
    if (number_to_allocate <= 0)
      number_to_allocate = 1;

    /* Are we allocating an array or a single value */
    is_array = number_to_allocate >= 2;

    /* Dynamically allocate memory. */
    if (!is_array && number_to_allocate == 1)
    {
      dest = memory_manager_malloc(memory_manager, size);
    }
    else
    {
      dest = memory_manager_calloc(memory_manager, number_to_allocate, size);
    }

    /* Allocation error checking. */
    if (!dest)
    {
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: failed to allocate value memory!  Requested a total of '%d' bytes.\n"
          , (int) (number_to_allocate * size)
          );

      return NULL;
    }
  }
  else
  {
    /* Don't dynamically allocate memory. */

    dest = cons->dest;

    if (!dest)
    {
      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: \"cons\" provides no \"dest\" reference to the value to initialize!\n"
            "  Failed to initialize new value without dynamic allocation without a reference to memory to initialize.\n"
          );

      return NULL;
    }
  }

  /* ---------------------------------------------------------------- */
  /* Dynamic allocation tracking.                                     */
  /* ---------------------------------------------------------------- */

  /* Initialize memory tracker if available or necessary. */
  if (cons->preserve_metadata)
  {
    /* preserve_metadata is set: Skip memory tracker initialization. */

    if (is_allocate)
    {
      if (is_allocate)
        memory_manager_free(memory_manager, dest);

      if (cons->out_init_error_msg)
        snprintf
          ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
          , "Error: template_cons_dup_struct: preserve_metadata is incompatible with memory tracking!\n"
            "  Failed to initialize new value without tracking it in a way that would not interfere with metadata.\n"
          );

      return NULL;
    }
  }
  else
  {
    if (is_allocate)
    {
      /* We need to initialize the memory tracker. */

      /* Does the type provide its own method of dynamic memory allocation
       * tracking?
       */
      if (mem_init)
      {
        void *mem_init_result;

        mem_init_result = mem_init(mem_init_object, dest, is_allocate);

        if (!mem_init_result)
        {
          if (is_allocate)
            memory_manager_free(memory_manager, dest);

          if (cons->out_init_error_msg)
            snprintf
              ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
              , "Error: template_cons_dup_struct: we dynamically allocated memory, and calling \"mem_init\" returned NULL, indicating failure!\n"
                "  Failed to initialize new value without successful memory tracking addition.\n"
              );

          return NULL;
        }
      }
      else
      {
        if (is_allocate)
          memory_manager_free(memory_manager, dest);

        if (cons->out_init_error_msg)
          snprintf
            ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
            , "Error: template_cons_dup_struct: we dynamically allocated memory, but \"mem_init\" is NULL!\n"
              "  Failed to initialize new value without a method to track the newly allocated value memory.\n"
            );

        return NULL;
      }
    }
    else
    {
      /* Are we able to initialize the memory tracker? */
      if (mem_init)
      {
        void *mem_init_result;

        mem_init_result = mem_init(mem_init_object, dest, is_allocate);

        if (!mem_init_result)
        {
          if (is_allocate)
            memory_manager_free(memory_manager, dest);

          if (cons->out_init_error_msg)
            snprintf
              ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
              , "Error: template_cons_dup_struct: we dynamically allocated memory, and calling \"mem_init\" returned NULL, indicating failure!\n"
                "  Failed to initialize new value without successful memory tracking addition.\n"
              );

          return NULL;
        }
      }
      else
      {
        /* No, but we don't need to. */
      }
    }
  }

  /* ---------------------------------------------------------------- */
  /* Struct initialization.                                           */
  /* ---------------------------------------------------------------- */

  /* Should we skip initialization? */
  if (is_allocate_only)
  {
    return dest;
  }

  /* Initialize fields. */
  if (!struct_info)
  {
    if (is_allocate)
      memory_manager_free(memory_manager, dest);

    if (cons->out_init_error_msg)
      snprintf
        ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
        , "Error: template_cons_dup_struct: \"struct_info\" is NULL!\n"
          "  Failed to initialize a value without a representation of the struct.\n"
        );

    return NULL;
  }

  is_err =
    struct_dup
      ( struct_info
      , dest
      , cons->initials
      , cons->force_no_defaults
      , cons->initials_copy_rec
      , cons->preserve_metadata
      , cons->ref_traversal
      );

  if (is_err)
  {
    if (is_allocate)
      memory_manager_free(memory_manager, dest);

    if (cons->out_init_error_msg)
      snprintf
        ( (char *) cons->out_init_error_msg, (size_t) terminator_size(cons->init_error_msg_size)
        , "Error: template_cons_dup_struct: assigning the struct fields with \"struct_dup\" failed!\n"
          "  Failed to initialize a value without successful assignment of struct fields.\n"
          "Error message returned from \"struct_dup\": %s%s"
        , (const char *) is_err
        , (const char *) (ascii_ends_in_char(is_err, '\n') ? "" : "\n")
        );

    return NULL;
  }

  /* Done! */
  return dest;
}

/*
 * Treat the "self" object as a "type_t" in the method argument "mem_init" for
 * "template_cons_dup_struct".
 */
void *template_cons_dup_struct_meminit_type
  ( const tval *self
  , tval       *val_raw
  , int         is_dynamically_allocated
  )
{
  const type_t *self_type = (const type_t *) self;

  return type_mem_init(self_type, val_raw, is_dynamically_allocated);
}

/*
 * template_cons_free_struct:
 *
 * Frees resources allocated by "template_cons_basic_initializer".
 *
 * Returns >= 0 on success (0 if not dynamically allocated, >= 1 otherwise),
 * and <= -1 on failure.
 */
int   template_cons_free_struct
  ( tval *val
  , int                      (*mem_free)( const tval *self
                                        , tval       *val
                                        )
  , const tval                *mem_free_object
  )
{
  /* The only resources allocation done */
  /* uses "mem_init", so, conversely,   */
  /* free the value using "mem_free".   */
  if (!mem_free)
    return -1;

  return mem_free(mem_free_object, val);
}

/*
 * Treat the "self" object as a "type_t" in the method argument "mem_free" for
 * "template_cons_free_struct".
 */
int template_cons_free_struct_memfree_type
  ( const tval *self
  , tval       *val
  )
{
  const type_t *self_type = (const type_t *) self;

  return type_mem_free(self_type, val);
}

/* ---------------------------------------------------------------- */
/* Casting between pointer types.                                   */
/* ---------------------------------------------------------------- */

funp_cast_t objp_to_funp(objp_cast_t ptr)
{
  struct { funp_cast_t fun; } fun;

  *((objp_cast_t *) (&fun)) = ptr;

  return fun.fun;
}

objp_cast_t funp_to_objp(funp_cast_t ptr)
{
  struct { funp_cast_t fun; } fun;

  fun.fun = ptr;

  return *((objp_cast_t *) (&fun));
}

size_t     objp_to_size   (void     *ptr)
{
  size_t    ptr_rep;
  ptrdiff_t distance;

  distance = (ptrdiff_t) (((unsigned char *) ptr) - ((unsigned char *) NULL));

  ptr_rep = (size_t) distance;

  return ((size_t) ptr_rep);
}

void      *size_to_objp   (size_t    ptr_rep)
{
  unsigned char *ptr;
  ptrdiff_t      distance;

  distance = (ptrdiff_t) ptr_rep;

  ptr = (unsigned char *) (((unsigned char *) NULL) + ((ptrdiff_t) distance));

  return ((void *) ptr);
}

ptrdiff_t  objp_to_ptrdiff(void     *ptr)
{
  ptrdiff_t distance;

  distance = (ptrdiff_t) (((unsigned char *) ptr) - ((unsigned char *) NULL));

  return ((ptrdiff_t) distance);
}

void      *ptrdiff_to_objp(ptrdiff_t ptr_rep)
{
  unsigned char *ptr;
  ptrdiff_t      distance;

  distance = (ptrdiff_t) ptr_rep;

  ptr = (unsigned char *) (((unsigned char *) NULL) + ((ptrdiff_t) distance));

  return ((void *) ptr);
}

/* ---------------------------------------------------------------- */
/* Universal type.                                                  */
/* ---------------------------------------------------------------- */

/* Universal type. */
const type_t *universal_type(void);

extern const type_t universal_type_def;

const type_t *universal_type(void)
  { return &universal_type_def; }

static const char   *universal_type_name      (const type_t *self);
static const type_t *universal_type_is_subtype( const type_t *self
                                              , const type_t *is_subtype
                                              );

const type_t universal_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ universal_type_name
  , /* info                   */ NULL
  , /* @size                  */ type_has_unknown_size
  , /* @is_struct             */ type_is_not_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ universal_type_is_subtype
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ NULL
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

static const char   *universal_type_name      (const type_t *self)
  { return "universal_t"; }

static const type_t *universal_type_is_subtype( const type_t *self
                                              , const type_t *is_subtype
                                              )
  { if (!self) return NULL; else return is_subtype; }

/* ---------------------------------------------------------------- */
/* Primitive C data types.                                          */
/* ---------------------------------------------------------------- */

/* General type. */
const type_t *void_type(void)
  { return &void_type_def; }

static const char *void_type_name(const type_t *self);

const type_t void_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ void_type_name
  , /* info                   */ NULL
  , /* @size                  */ type_has_unknown_size
  , /* @is_struct             */ type_is_not_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ NULL
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

static const char *void_type_name(const type_t *self)
  { return "void_t"; }

/* ---------------------------------------------------------------- */

/* General pointers. */
PRIM_TYPE(objp, objp_t, type_has_default_value(self, &objp_default))
PRIM_TYPE(funp, funp_t, type_has_default_value(self, &funp_default))

const objp_t objp_default = (objp_t) NULL;
const funp_t funp_default = (funp_t) NULL;

/* ---------------------------------------------------------------- */

/* Scalar types. */
PRIM_TYPE(char,    char_t,    type_has_default_value(self, &char_default))
PRIM_TYPE(schar,   schar_t,   type_has_default_value(self, &schar_default))
PRIM_TYPE(uchar,   uchar_t,   type_has_default_value(self, &uchar_default))

PRIM_TYPE(short,   short_t,   type_has_default_value(self, &short_default))
PRIM_TYPE(ushort,  ushort_t,  type_has_default_value(self, &ushort_default))

PRIM_TYPE(int,     int_t,     type_has_default_value(self, &int_default))
PRIM_TYPE(uint,    uint_t,    type_has_default_value(self, &uint_default))

PRIM_TYPE(long,    long_t,    type_has_default_value(self, &long_default))
PRIM_TYPE(ulong,   ulong_t,   type_has_default_value(self, &ulong_default))

PRIM_TYPE(float,   float_t,   type_has_default_value(self, &float_default))
PRIM_TYPE(double,  double_t,  type_has_default_value(self, &double_default))
PRIM_TYPE(ldouble, ldouble_t, type_has_default_value(self, &ldouble_default))

const char_t    char_default    = (char)          '\x00';
const schar_t   schar_default   = (signed char)   '\x00';
const uchar_t   uchar_default   = (unsigned char) '\x00';

const short_t   short_default   = (short)          0;
const ushort_t  ushort_default  = (unsigned short) 0U;

const int_t     int_default     = (int)            0;
const uint_t    uint_default    = (unsigned int)   0U;

const long_t    long_default    = (long)           0L;
const ulong_t   ulong_default   = (unsigned long)  0LU;

const float_t   float_default   = (float)          0.0f;
const double_t  double_default  = (double)         0.0;
const ldouble_t ldouble_default = (long double)    0.0L;

/* ---------------------------------------------------------------- */

/* Derived types. */

/* array type. */

const type_t *array_type(void)
  { return &array_type_def; }

static const char          *array_type_name       (const type_t *self);
static const tval          *array_type_has_default(const type_t *self);

const type_t array_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ array_type_name
  , /* info                   */ NULL
  , /* @size                  */ type_has_unknown_size
  , /* @is_struct             */ type_is_not_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ array_type_has_default
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

static const char          *array_type_name       (const type_t *self)
  { return "array_t"; }

static const tval          *array_type_has_default(const type_t *self)
  { return &array_default; }

const array_t array_default      = { 0 };
const size_t  array_default_size = ARRAY_SIZE   (array_default);
const size_t  array_default_num  = ARRAY_NUM    (array_default);
const size_t  array_default_len  = ARRAY_LEN_ALL(array_default);

/* ---------------------------------------------------------------- */

/* <math.h> */

/* div type. */

const type_t *div_type(void)
  { return &div_type_def; }

static const char          *div_type_name       (const type_t *self);
static size_t               div_type_size       (const type_t *self, const tval *val);
static const struct_info_t *div_type_is_struct  (const type_t *self);
static const tval          *div_type_has_default(const type_t *self);

const type_t div_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ div_type_name
  , /* info                   */ NULL
  , /* @size                  */ div_type_size
  , /* @is_struct             */ div_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ div_type_has_default
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

static const char          *div_type_name       (const type_t *self)
  { return "primdiv_t"; }

static size_t               div_type_size       (const type_t *self, const tval *val)
  { return sizeof(primdiv_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(div)
static const struct_info_t *div_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(div);

    /* int quot */
    STRUCT_INFO_RADD(int_type(), quot);

    /* int rem */
    STRUCT_INFO_RADD(int_type(), rem);

    STRUCT_INFO_DONE();
  }

static const tval          *div_type_has_default(const type_t *self)
  { return &div_default; }

const primdiv_t div_default = { 0, 0 };

/* ldiv type. */

const type_t *ldiv_type(void)
  { return &ldiv_type_def; }

static const char          *ldiv_type_name       (const type_t *self);
static size_t               ldiv_type_size       (const type_t *self, const tval *val);
static const struct_info_t *ldiv_type_is_struct  (const type_t *self);
static const tval          *ldiv_type_has_default(const type_t *self);

const type_t ldiv_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL

  , /* typed                  */ type_is_untyped

  , /* @name                  */ ldiv_type_name
  , /* info                   */ NULL
  , /* @size                  */ ldiv_type_size
  , /* @is_struct             */ ldiv_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ NULL
  , /* free                   */ NULL
  , /* has_default            */ ldiv_type_has_default
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

static const char          *ldiv_type_name       (const type_t *self)
  { return "primldiv_t"; }

static size_t               ldiv_type_size       (const type_t *self, const tval *val)
  { return sizeof(primldiv_t); }

DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(ldiv)
static const struct_info_t *ldiv_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(ldiv);

    /* long quot */
    STRUCT_INFO_RADD(long_type(), quot);

    /* long rem */
    STRUCT_INFO_RADD(long_type(), rem);

    STRUCT_INFO_DONE();
  }

static const tval          *ldiv_type_has_default(const type_t *self)
  { return &ldiv_default; }

const primldiv_t ldiv_default = { 0L, 0L };

/* ---------------------------------------------------------------- */

/* <setjmp.h> */

PRIM_TYPE(jmp_buf, jmp_buf_t, type_has_default_value(self, &jmpbuf_default))

const jmp_buf_t jump_buffer_placeholder, jmpbuf_default;

/* ---------------------------------------------------------------- */

/* <signal.h> */

PRIM_TYPE(sig_atomic, primsig_atomic_t, type_has_no_default_value(self))

/* ---------------------------------------------------------------- */

/* <stdarg.h> */

PRIM_TYPE(va_list, va_list_t, type_has_no_default_value(self))

/* ---------------------------------------------------------------- */

/* <stddef.h> */

PRIM_TYPE(size,    primsize_t,    type_has_default_value(self, &size_default))
PRIM_TYPE(ptrdiff, primptrdiff_t, type_has_default_value(self, &ptrdiff_default))

const primsize_t    size_default    = (size_t)    0U;
const primptrdiff_t ptrdiff_default = (ptrdiff_t) 0;

/* ---------------------------------------------------------------- */

/* <stdio.h> */

PRIM_TYPE(file, file_t,     type_has_no_default_value(self))
PRIM_TYPE(fpos, primfpos_t, type_has_no_default_value(self))

/* ---------------------------------------------------------------- */

/* <time.h> */

PRIM_TYPE(tm,    tm_t,        type_has_no_default_value(self))
PRIM_TYPE(time,  primtime_t,  type_has_no_default_value(self))
PRIM_TYPE(clock, primclock_t, type_has_no_default_value(self))

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

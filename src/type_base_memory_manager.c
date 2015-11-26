/*
 * opencurry: type_base_memory_manager.c
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
 *   - strlen
 */
#include <string.h>

#include "base.h"
#include "type_base_prim.h"
#include "type_base_memory_manager.h"

#include "type_base_typed.h"
#include "type_base_tval.h"
#include "type_base_type.h"

#include "util.h"

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
/* static const type_t        *memory_manager_type_self       (const type_t *self);                  */
/* static typed_t              memory_manager_type_container  (const type_t *self);                  */
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
/* static size_t               memory_manager_type_free       (const type_t *self, tval *val);       */
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
  , /* @indirect              */ memory_manager_type

  , /* self                   */ NULL /* memory_manager_type_self                    */
  , /* container              */ NULL /* memory_manager_type_container               */

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

    /* manager_mmalloc_fun_t  mmalloc;  */
    /* manager_mfree_fun_t    mfree;    */
    /* manager_mrealloc_fun_t mrealloc; */
    /* manager_mcalloc_fun_t  mcalloc;  */
    STRUCT_INFO_RADD(funp_type(), mmalloc);
    STRUCT_INFO_RADD(funp_type(), mfree);
    STRUCT_INFO_RADD(funp_type(), mrealloc);
    STRUCT_INFO_RADD(funp_type(), mcalloc);

    /* manager_on_oom_fun_t on_oom; */
    /* manager_on_err_fun_t on_err; */
    STRUCT_INFO_RADD(funp_type(), on_oom);
    STRUCT_INFO_RADD(funp_type(), on_err);

    /* void   *state; */
    STRUCT_INFO_RADD(objp_type(),  state);

    /* size_t  state_size; */
    STRUCT_INFO_RADD(size_type(),  state_size);

    STRUCT_INFO_DONE();
  }

static const tval          *memory_manager_type_has_default(const type_t *self)
  { return type_has_default_value(self, &memory_manager_defaults); }

/* ---------------------------------------------------------------- */

const memory_manager_t memory_manager_defaults =
  MEMORY_MANAGER_DEFAULTS;

/* ---------------------------------------------------------------- */

void memory_manager_default_on_oom(const memory_manager_t *self, size_t      size)
{
  static char         buf[4096];
  static const size_t buf_size = sizeof(buf) / sizeof(buf[0]);

  snprintf
    ( (char *) buf, (size_t) terminator_size(buf_size), ""
      "*** (EE) Error: memory_manager_default_on_oom: Out of memory!  Requested '%d' bytes.\n"

    , (int) size
    );

  buf[buf_size - 1] = 0;
  memory_manager_on_err(self, buf);
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

const memory_manager_t * const default_memory_manager = &malloc_manager;

static void   *malloc_manager_mmalloc (const memory_manager_t *self, size_t  size);
static size_t  malloc_manager_mfree   (const memory_manager_t *self, void   *ptr);
static void   *malloc_manager_mrealloc(const memory_manager_t *self, void   *ptr,   size_t size);
static void   *malloc_manager_mcalloc (const memory_manager_t *self, size_t  nmemb, size_t size);
static void    malloc_manager_on_oom  (const memory_manager_t *self, size_t      size);
static void    malloc_manager_on_err  (const memory_manager_t *self, const char *msg);

const memory_manager_t malloc_manager =
  { memory_manager_type

  , malloc_manager_mmalloc
  , malloc_manager_mfree
  , malloc_manager_mrealloc
  , malloc_manager_mcalloc

  , malloc_manager_on_oom
  , malloc_manager_on_err

  , NULL
  , 0
  };

static void   *malloc_manager_mmalloc (const memory_manager_t *self, size_t  size)
  { return malloc (size); }
static size_t  malloc_manager_mfree   (const memory_manager_t *self, void   *ptr)
{
#if ERROR_CHECKING
  if (!ptr)
    return 0;
#endif /* #if ERROR_CHECKING */

  free(ptr);
  return 1;
}
static void   *malloc_manager_mrealloc(const memory_manager_t *self, void   *ptr,   size_t size)
  { return realloc(ptr,   size); }
static void   *malloc_manager_mcalloc (const memory_manager_t *self, size_t  nmemb, size_t size)
  { return calloc (nmemb, size); }
static void    malloc_manager_on_oom  (const memory_manager_t *self, size_t      size)
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
static void    malloc_manager_on_err  (const memory_manager_t *self, const char *msg)
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

memory_manager_t *memory_manager_init
  ( memory_manager_t *dest

  , manager_mmalloc_fun_t  mmalloc
  , manager_mfree_fun_t    mfree
  , manager_mrealloc_fun_t mrealloc
  , manager_mcalloc_fun_t  mcalloc
  )
{
  int dynamically_allocated;

  UNUSED(dynamically_allocated);

  /* Minimum required fields, unless all fields are NULL: */
  /*   - mmalloc                                          */
  /*   - mfree                                            */
  /*   - mrealloc                                         */
  if (mmalloc || mfree || mfree || mrealloc || mcalloc)
  {
    if (!mmalloc || !mfree || !mrealloc)
    {
      /* Error: missing at least one field. */
      return NULL;
    }
  }

  if (!dest)
  {
    dynamically_allocated = 1;

    if (!mmalloc)
      dest = mmalloc(dest, sizeof(*dest));
    else
      dest = memory_manager_mmalloc(default_memory_manager, sizeof(*dest));

    if (!dest)
    {
      /* Error: failed to dynamically allocate dest! */
      return NULL;
    }
  }
  else
  {
    dynamically_allocated = 0;
  }

  dest->type = memory_manager_type;

  dest->mmalloc    = mmalloc;
  dest->mfree      = mfree;
  dest->mrealloc   = mrealloc;
  dest->mcalloc    = mcalloc;

  dest->on_oom     = memory_manager_default_on_oom;
  dest->on_err     = memory_manager_default_on_err;

  dest->state      = NULL;
  dest->state_size = 0;

  return dest;
}

size_t memory_manager_deinit(memory_manager_t *memory_manager)
{
#if ERROR_CHECKING
  if (!memory_manager)
    return 0;
#endif /* #if ERROR_CHECKING */

  memory_manager->state_size = 0;
  memory_manager->state      = NULL;

  memory_manager->on_err     = NULL;
  memory_manager->on_oom     = NULL;

  memory_manager->mcalloc    = NULL;
  memory_manager->mrealloc   = NULL;
  memory_manager->mfree      = NULL;
  memory_manager->mmalloc    = NULL;

  memory_manager->type = NULL;

  return 1;
}

memory_manager_t *memory_manager_copy(memory_manager_t *dest, const memory_manager_t *src)
{
  int dynamically_allocated;

  UNUSED(dynamically_allocated);

#if ERROR_CHECKING
  if (!src)
    return 0;
#endif /* #if ERROR_CHECKING */

  if (!dest)
  {
    dynamically_allocated = 1;

    dest = memory_manager_mmalloc(src, sizeof(*dest));

    if (!dest)
    {
      /* Error: failed to dynamically allocate dest! */
      return NULL;
    }
  }
  else
  {
    dynamically_allocated = 0;
  }

  dest->type = src->type;

  dest->mmalloc    = src->mmalloc;
  dest->mfree      = src->mfree;
  dest->mrealloc   = src->mrealloc;
  dest->mcalloc    = src->mcalloc;

  dest->on_oom     = src->on_oom;
  dest->on_err     = src->on_err;

  dest->state      = src->state;
  dest->state_size = src->state_size;

  return dest;
}

/* ---------------------------------------------------------------- */

void *memory_manager_mmalloc(const memory_manager_t *memory_manager, size_t size)
{
  void *mem;

  if (!memory_manager || !memory_manager->mmalloc)
    memory_manager = &malloc_manager;

  if (!memory_manager->mmalloc || !memory_manager->mfree)
  {
    if (memory_manager->mmalloc)
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

  mem = memory_manager->mmalloc(memory_manager, size);

  if (!mem)
    memory_manager_on_oom(memory_manager, size);

  return mem;
}

size_t memory_manager_mfree(const memory_manager_t *memory_manager, void *ptr)
{
  if (!memory_manager || !memory_manager->mfree)
    memory_manager = &malloc_manager;

  if (!memory_manager->mmalloc || !memory_manager->mfree)
  {
    if (memory_manager->mmalloc)
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

  return memory_manager->mfree(memory_manager, ptr);
}

void *memory_manager_mrealloc(const memory_manager_t *memory_manager, void *ptr, size_t size)
{
  if (!memory_manager)
    memory_manager = &malloc_manager;

  if (memory_manager->mrealloc)
  {
    return memory_manager->mrealloc(memory_manager, ptr, size);
  }
  else
  {
    memory_manager_on_err
      ( memory_manager
      , "error: a memory manager lacks \"realloc\"!"
      );

    return NULL;
  }
}

void *memory_manager_mcalloc(const memory_manager_t *memory_manager, size_t nmemb, size_t size)
{
  if (!memory_manager)
    memory_manager = &malloc_manager;

  if (memory_manager->mcalloc)
  {
    return memory_manager->mcalloc(memory_manager, nmemb, size);
  }
  else
  {
    size_t total_size;

    total_size = nmemb * size;

    return memory_manager_mmalloc(memory_manager, total_size);
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

/* If "memory_manager" is NULL, return the default memory manager. */
const memory_manager_t *require_memory_manager(const memory_manager_t *memory_manager)
{
  if (!memory_manager)
    return default_memory_manager;

  return memory_manager;
}

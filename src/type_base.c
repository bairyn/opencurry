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

/* TODO: do we need NULL? */
/* stddef.h:
 *   - NULL
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

/* TODO */
#ifdef TODO

/* ---------------------------------------------------------------- */
/* type_t                                                           */
/* ---------------------------------------------------------------- */

type_t type_type =
  { &type_type

  , TODO
  }

/* ---------------------------------------------------------------- */
/* Memory managers.                                                 */
/* ---------------------------------------------------------------- */

type_t memory_manager_type =
  { &type_type

  , TODO
  }

const memory_manager_t malloc_manager =
  { &memory_manaager_type

  , malloc
  , free
  , calloc
  , realloc
  };

void *memory_manager_malloc(const memory_manager_t *memory_manager, size_t size)
{
  void *(*malloc_func)(size_t size);

  if (!memory_manager)
    memory_manager = malloc_manager;

  if (memory_manager->malloc)
  {
    malloc_func = memory_manager->malloc;
  }
  else
  {
    malloc_func = malloc_manager->malloc;
  }

  return malloc_func(size);
}

void *memory_manager_free(const memory_manager_t *memory_manager, void *ptr)
{
  void *(*free_func)(void *ptr);

  if (!memory_manager)
    memory_manager = malloc_manager;

  if (memory_manager->free)
  {
    free_func = memory_manager->free;
  }
  else
  {
    free_func = free_manager->malloc;
  }

  return free_func(ptr);
}

void *memory_manager_calloc(const memory_manager_t *memory_manager, size_t nmemb, size_t size)
{
  if (!memory_manager)
    memory_manager = malloc_manager;

  if (memory_manager->calloc)
  {
    return memory_manager->calloc(nmemb, size);
  }
  else
  {
    size_t total_size;

    total_size = nmemb * size;

    return memory_manager_malloc(memory_manager, total_size)
  }
}

void *memory_manager_realloc(const memory_manager_t *memory_manager, void *ptr, size_t size)
{
  if (!memory_manager)
    memory_manager = malloc_manager;

  if (memory_manager->realloc)
  {
    return memory_manager->realloc(ptr, size);
  }
  else
  {
    memory_manager_free(memory_manager, ptr);

    return memory_manager_malloc(memory_manager, size);
  }
}

/* ---------------------------------------------------------------- */
/* Template constructors, available for types to use.               */
/* ---------------------------------------------------------------- */

type_t template_cons_type =
  { &type_type

  , TODO
  }

type_t memory_tracker_type =
  { &type_type

  , TODO
  }







































/* TODO */
#ifdef TODO

/* ---------------------------------------------------------------- */
/* The type "type_t".                                               */
/* ---------------------------------------------------------------- */

static type_t             *(*type_id)              (type_t *self);
static type_t             *(*type_init_type)       (type_t *self);
static void                (*type_init)            (type_t *self, type_t *val,                  const type_t *initialization);
static void                (*type_free)            (type_t *self, type_t *val);
static int                 (*type_verify_init)     (type_t *self, const type_t *initialization, char *out_err_msg,          size_t err_msg_size);
static type_t             *(*type_new)             (type_t *self, const type_t *initialization);
static void                (*type_deep_dup)        (type_t *self, type_t *dest,                 const type_t *src);
static void                (*type_shallow_dup)     (type_t *self, type_t *dest,                 const type_t *src);
static void                (*type_direct_dup)      (type_t *self, type_t *dest,                 const type_t *src);
static type_t             *(*type_new_deep_copy)   (type_t *self, const type_t *src);
static type_t             *(*type_new_shallow_copy)(type_t *self, const type_t *src);
static type_t             *(*type_new_direct_copy) (type_t *self, const type_t *src);
static const char        **(*type_name)            (type_t *self);
static const char        **(*type_info)            (type_t *self);
static size_t              (*type_size)            (type_t *self);
static int                 (*type_valid_tval)      (type_t *self);
static void               *(*type_user_buf)        (type_t *self);
static size_t              (*type_user_buf_num)    (type_t *self);
static size_t              (*type_user_buf_size)   (type_t *self);

static unsigned char         type_user_static[USER_STATIC_SIZE];
static size_t                type_user_static_num   = sizeof(type_user_static) / sizeof(type_user_static[0]);
static size_t                type_user_static_size  = sizeof(type_user_static);
static void                 *type_user_dynamic      = NULL;
static size_t                type_user_dynamic_size = 0;
static type_heap_info_t      type_heap_info         = type_heap_none;

const type_t type_type =
  { &type_type

  , type_id
  , type_init_type
  , type_init
  , type_free
  , type_verify_init
  , type_new
  , type_deep_dup
  , type_shallow_dup
  , type_direct_dup
  , type_new_deep_copy
  , type_new_shallow_copy
  , type_new_direct_copy
  , type_name
  , type_info
  , type_size
  , type_size
  , type_valid_tval
  , type_user_buf
  , type_user_buf_size

  , type_user_static
  , type_user_static_num;
  , type_user_static_size;
  , type_user_dynamic;    
  , type_user_dynamic_size;
  , type_heap_info;
  };

/* ---------------------------------------------------------------- */
/* The type "type_init_t".                                          */
/* ---------------------------------------------------------------- */

static type_t             *(*type_init_id)              (type_t *self);
static type_t             *(*type_init_init_type)       (type_t *self);
static void                (*type_init_init)            (type_t *self, type_init_t *val,                  const type_init_t *initialization);
static void                (*type_init_free)            (type_t *self, type_init_t *val);
static int                 (*type_init_verify_init)     (type_t *self, const type_init_t *initialization, char *out_err_msg,          size_t err_msg_size);
static type_init_t        *(*type_init_new)             (type_t *self, const type_init_t *initialization);
static void                (*type_init_deep_dup)        (type_t *self, type_init_t *dest,                 const type_init_t *src);
static void                (*type_init_shallow_dup)     (type_t *self, type_init_t *dest,                 const type_init_t *src);
static void                (*type_init_direct_dup)      (type_t *self, type_init_t *dest,                 const type_init_t *src);
static type_init_t        *(*type_init_new_deep_copy)   (type_t *self, const type_init_t *src);
static type_init_t        *(*type_init_new_shallow_copy)(type_t *self, const type_init_t *src);
static type_init_t        *(*type_init_new_direct_copy) (type_t *self, const type_init_t *src);
static const char        **(*type_init_name)            (type_t *self);
static const char        **(*type_init_info)            (type_t *self);
static size_t              (*type_init_size)            (type_t *self);
static int                 (*type_init_valid_tval)      (type_t *self);
static void               *(*type_init_user_buf)        (type_t *self);
static size_t              (*type_init_user_buf_size)   (type_t *self);

static unsigned char         type_init_user_static[USER_STATIC_SIZE];
static size_t                type_init_user_static_num   = sizeof(type_user_static) / sizeof(type_user_static[0]);
static size_t                type_init_user_static_size  = sizeof(type_user_static);
static void                 *type_init_user_dynamic      = NULL;
static size_t                type_init_user_dynamic_size = 0;
static type_heap_info_t      type_init_heap_info         = type_heap_none;

const type_t type_init_type =
  { &type_type

  , type_init_id
  , type_init_init_type
  , type_init_init
  , type_init_free
  , type_init_verify_init
  , type_init_new
  , type_init_deep_dup
  , type_init_shallow_dup
  , type_init_direct_dup
  , type_init_new_deep_copy
  , type_init_new_shallow_copy
  , type_init_new_direct_copy
  , type_init_name
  , type_init_info
  , type_init_size
  , type_init_size
  , type_init_valid_tval
  , type_init_user_buf
  , type_init_user_buf_size

  , type_init_user_static
  , type_init_user_static_num;
  , type_init_user_static_size;
  , type_init_user_dynamic;    
  , type_init_user_dynamic_size;
  , type_init_heap_info;
  };

/* ---------------------------------------------------------------- */
/* "type_t" definitions.                                            */
/* ---------------------------------------------------------------- */

static type_t             *(*type_id)              (type_t *self)
{
  return self;
}

static type_t             *(*type_init_type)       (type_t *self)
{
  return type_init_type;
}

static void                (*type_init)            (type_t *self, type_t *val,                  const type_t *initialization)
{
  TODO
}

static void                (*type_free)            (type_t *self, type_t *val)
{
  if (val->heap_info & type_heap_dynamic)
  {
    val->user_dynamic_size = 0;

    val->heap_info &= ~ type_heap_dynamic;
    free(val->user_dynamic);
    val->user_dynamic = NULL;
  }

  if (val->heap_info & type_heap_self)
  {
    val->heap_info &= ~ type_heap_self;
    free(val);
    val = NULL;

    return;
  }
}

static int                 (*type_verify_init)     (type_t *self, const type_t *initialization, char *out_err_msg,          size_t err_msg_size)
{
  TODO
}

static type_t             *(*type_new)             (type_t *self, const type_t *initialization)
{
  TODO
}

static void                (*type_deep_dup)        (type_t *self, type_t *dest,                 const type_t *src)
{
  TODO
}

static void                (*type_shallow_dup)     (type_t *self, type_t *dest,                 const type_t *src)
{
  TODO
}

static void                (*type_direct_dup)      (type_t *self, type_t *dest,                 const type_t *src)
{
  TODO
}

static type_t             *(*type_new_deep_copy)   (type_t *self, const type_t *src)
{
  TODO
}

static type_t             *(*type_new_shallow_copy)(type_t *self, const type_t *src)
{
  TODO
}

static type_t             *(*type_new_direct_copy) (type_t *self, const type_t *src)
{
  TODO
}

static const char        **(*type_name)            (type_t *self)
{
  return "type";
}

static const char        **(*type_info)            (type_t *self)
{
  return "typedef struct type_s type_t";
}

static size_t              (*type_size)            (type_t *self)
{
  return ;
}

static int                 (*type_valid_tval)      (type_t *self)
{
  return TYPE_VALID_TVAL;
}

static void               *(*type_user_buf)        (type_t *self)
{
  return NULL;
}

static size_t              (*type_user_buf_num)    (type_t *self)
{
  return 0;
}

static size_t              (*type_user_buf_size)   (type_t *self)
{
  return 0;
}

/* ---------------------------------------------------------------- */
/* "type_init_t" definitions.                                       */
/* ---------------------------------------------------------------- */





















































type_t             *(*type_id)              (type_t *self);
type_t             *(*type_init_type)       (type_t *self);
void                (*type_init)            (type_t *self, tval *val,                  const tval *initialization);
void                (*type_free)            (type_t *self, tval *val);
int                 (*type_verify_init)     (type_t *self, const tval *initialization, char *out_err_msg,          size_t err_msg_size);
tval               *(*type_new)             (type_t *self, const tval *initialization);
void                (*type_deep_dup)        (type_t *self, tval *dest,                 const tval *src);
void                (*type_shallow_dup)     (type_t *self, tval *dest,                 const tval *src);
void                (*type_direct_dup)      (type_t *self, tval *dest,                 const tval *src);
tval               *(*type_new_deep_copy)   (type_t *self, const tval *src);
tval               *(*type_new_shallow_copy)(type_t *self, const tval *src);
const char        **(*type_name)            (type_t *self);
const char        **(*type_info)            (type_t *self);
size_t              (*type_size)            (type_t *self);
int                 (*type_valid_tval)      (type_t *self);
void               *(*type_user_buf)        (type_t *self);
size_t              (*type_user_buf_size)   (type_t *self);
unsigned char         type_user_static[USER_STATIC_SIZE];
size_t                type_user_static_size;
void                 *type_user_dynamic;
size_t                type_user_dynamic_size;


static const unsigned char user_static_type[] = { 0 };
static const size_t        user_static_type_size = sizeof(user_static_type) / sizeof(user_static_type[0]);

static const unsigned char user_static_type_init[] = { 0 };
static const size_t        user_static_type_init_size = sizeof(user_static_type_init) / sizeof(user_static_type_init[0]);

/* The type "type_t". */

/* The type "type_init_t". */
const type_t type_init_type =
  { &type_init_type

  , &type_init_type

  , init_type_init
  , free_type_init

  , new_type_init

  , deep_dup_type_init
  , shallow_dup_type_init
  , direct_dup_type_init

  , new_deep_copy_type_init
  , new_shallow_copy_type_init
  , new_direct_copy_type_init


  , valid_tval_type_init


  , type_name_type_init
  , type_info_type_init
  , type_size_type_init

  , user_static_type_init
  , user_static_size_type_init

  , NULL
  , 0
  };

/* ---------------------------------------------------------------- */

void init_type(type_t *self, type_t *val, const type_init_t *initialization)
{
  val->type = self;
}

void free_type(type_t *self, type_t *val)
{
}


type_t *new_type(type_t *self, const type_init_t *initialization)
{
  type_t *type;

  type = malloc(self->type_size(self));

  if (!type)
  {
    return NULL;
  }

  type_init(self, type, initialization);
  ((unsigned char *) type->user_static)[0] = TYPE_ON_HEAP;

  return type;
}

void dup_type(type_t *self, type_t *dest, const type_t *src)
{
  dest->type = src->type;

  /* ---------------------------------------------------------------- */

  dest->init_type         = src->init_type;

  dest->init              = src->init;
  dest->free              = src->free;

  dest->new               = src->new;

  dest->dup               = src->dup;


  dest->type_name         = src->type_name;
  dest->type_info         = src->type_info;
  dest->type_size         = src->type_size;

  dest->user_static       = src->user_static;
  dest->user_static_size  = src->user_static_size;

  dest->user_dynamic      = src->user_dynamic;
  dest->user_dynamic_size = src->user_dynamic_size;
}



const char *type_name_type(type_t *self)
{
  return "type";
}

const char *type_info_type(type_t *self)
{
  return "typedef struct type_s type_t";
}

size_t type_size_type(type_t *self)
{
  return sizeof(type_t);
}

/* ---------------------------------------------------------------- */

void init_type_init(type_t *self, type_t *val, const type_init_t *initialization)
{
  val->type = self;

  if (!initialization)
  {
    dup_type_init(val, default_init_type);

    return;
  }
  else
  {
    dup_type_init(val, initialization);

    return;
  }
}

void free_type_init(type_t *self, type_init_t *val)
{
}


type_init_t *new_type_init(type_t *self, const type_init_t *initialization)
{
  type_init_t *type_init;

  type_init = malloc(self->type_size(self));

  if (!type_init)
  {
    return NULL;
  }

  type_init_init(self, type_init, initialization);
  ((unsigned char *) type_init->user_static)[0] = TYPE_INIT_ON_HEAP;

  return type_init;
}

void dup_type_init(type_t *self, type_init_t *dest, const type_init_t *src)
{
  dest->type = src->type;

  /* ---------------------------------------------------------------- */

  dest->override_init              = src->override_init;
  dest->override_free              = src->override_free;
  dest->override_new               = src->override_new;
  dest->override_dup               = src->override_dup;
  dest->override_type_name         = src->override_type_name;
  dest->override_type_info         = src->override_type_info;
  dest->override_type_size         = src->override_type_size;
  dest->override_user_static       = src->override_user_static;
  dest->override_user_static_size  = src->override_user_static_size;
  dest->override_user_dynamic      = src->user_dynamic;
  dest->override_user_dynamic_size = src->user_dynamic_size;


  dest->init                       = src->init;
  dest->free                       = src->free;

  dest->new                        = src->new;

  dest->dup                        = src->dup;


  dest->type_name                  = src->type_name;
  dest->type_info                  = src->type_info;
  dest->type_size                  = src->type_size;

  dest->user_static                = src->user_static;
  dest->user_static_size           = src->user_static_size;

  dest->user_dynamic               = src->user_dynamic;
  dest->user_dynamic_size          = src->user_dynamic_size;
}



const char *type_name_type_init(type_t *self)
{
  return "type_init"
}

const char *type_info_type_init(type_t *self)
{
  return "typedef struct type_init_s type_init_t"
}

size_t type_size_type_init(type_t *self)
{
  return sizeof(type_init_t);
}
/* TODO */
#endif /* TODO */
/* TODO */
#endif /* TODO */

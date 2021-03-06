/*
 * opencurry: type_base_memory_manager.h
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
 * type_base_memory_manager.h
 * ------
 */

#ifndef TYPE_BASE_MEMORY_MANAGER_H
#define TYPE_BASE_MEMORY_MANAGER_H
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
/* Memory managers.                                                 */
/* ---------------------------------------------------------------- */

/*
 * memory_manager_t:
 *
 * Methods to allocate and free raw bytes of memory.
 */

/* TODO: types! */
typedef void   *(*mmalloc_fun_t) (void *context, size_t  size);
typedef size_t  (*mfree_fun_t)   (void *context, void   *ptr);
typedef void   *(*mrealloc_fun_t)(void *context, void   *ptr,   size_t size);
typedef void   *(*mcalloc_fun_t) (void *context, size_t  nmemb, size_t size);

typedef void    (*on_oom_fun_t)  (void *context, size_t      size);
typedef void    (*on_err_fun_t)  (void *context, const char *msg);

/* Forward declaration: memory_manager_t. */
typedef struct memory_manager_s memory_manager_t;


typedef void   *(*manager_mmalloc_fun_t) (const memory_manager_t *self, size_t  size);
typedef size_t  (*manager_mfree_fun_t)   (const memory_manager_t *self, void   *ptr);
typedef void   *(*manager_mrealloc_fun_t)(const memory_manager_t *self, void   *ptr,   size_t size);
typedef void   *(*manager_mcalloc_fun_t) (const memory_manager_t *self, size_t  nmemb, size_t size);

typedef void    (*manager_on_oom_fun_t)  (const memory_manager_t *self, size_t      size);
typedef void    (*manager_on_err_fun_t)  (const memory_manager_t *self, const char *msg);


const type_t *memory_manager_type(void);
extern const type_t memory_manager_type_def;
/* Forward declaration: memory_manager_t. */
struct memory_manager_s
{
  typed_t type;

  /* 4 fields: all NULL, or minimum of: */
  /*   - mmalloc                        */
  /*   - mfree                          */
  /*   - mrealloc                       */
  /*   - mcalloc                        */
  manager_mmalloc_fun_t  mmalloc;
  manager_mfree_fun_t    mfree;
  manager_mrealloc_fun_t mrealloc;
  manager_mcalloc_fun_t  mcalloc;

  manager_on_oom_fun_t on_oom;
  manager_on_err_fun_t on_err;

  void   *state;
  size_t  state_size;
};

#define MEMORY_MANAGER_DEFAULTS   \
  { memory_manager_type           \
                                  \
  , NULL                          \
  , NULL                          \
  , NULL                          \
  , NULL                          \
                                  \
  , memory_manager_default_on_oom \
  , memory_manager_default_on_err \
                                  \
  , NULL                          \
  , 0                             \
  }

/* A "memory_manager_t" with NULLS. */
extern const memory_manager_t memory_manager_defaults;

/* ---------------------------------------------------------------- */

/* Print a message to stderr and return. */
void memory_manager_default_on_oom(const memory_manager_t *self, size_t      size);
void memory_manager_default_on_err(const memory_manager_t *self, const char *msg);

/* Do nothing and return. */
void on_memory_manager_oom_do_nothing(const memory_manager_t *self, size_t      size);
void on_memory_manager_err_do_nothing(const memory_manager_t *self, const char *msg);

/* ---------------------------------------------------------------- */

extern const memory_manager_t * const default_memory_manager;

extern const memory_manager_t malloc_manager;

/* ---------------------------------------------------------------- */

memory_manager_t *memory_manager_init
  ( memory_manager_t *dest

  , manager_mmalloc_fun_t  mmalloc
  , manager_mfree_fun_t    mfree
  , manager_mrealloc_fun_t mrealloc
  , manager_mcalloc_fun_t  mcalloc
  );

size_t memory_manager_deinit(memory_manager_t *memory_manager);

memory_manager_t *memory_manager_copy(memory_manager_t *dest, const memory_manager_t *src);

/* ---------------------------------------------------------------- */

/* Both the reference and individual function pointers may be NULL. */
/*                                                                  */
/* Behaviour in case of NULL "calloc" and "realloc" pointers will   */
/* be based on the behaviour of the "malloc" and "free" fields.     */
/*                                                                  */
/* If only one of "malloc" and "free" is NULL, behaviour is         */
/* undefined.                                                       */
void   *memory_manager_mmalloc (const memory_manager_t *memory_manager, size_t size);
size_t  memory_manager_mfree   (const memory_manager_t *memory_manager, void   *ptr);
void   *memory_manager_mrealloc(const memory_manager_t *memory_manager, void   *ptr,  size_t size);
void   *memory_manager_mcalloc (const memory_manager_t *memory_manager, size_t nmemb, size_t size);

void    memory_manager_on_oom  (const memory_manager_t *memory_manager, size_t      size);
void    memory_manager_on_err  (const memory_manager_t *memory_manager, const char *msg);

/* ---------------------------------------------------------------- */

const memory_manager_t *require_memory_manager(const memory_manager_t *memory_manager);

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_MEMORY_MANAGER_H */

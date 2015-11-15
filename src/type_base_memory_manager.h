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

const type_t *memory_manager_type(void);
extern const type_t memory_manager_type_def;
typedef struct memory_manager_s memory_manager_t;
struct memory_manager_s
{
  typed_t type;

  void *(*malloc) (const memory_manager_t *self, size_t  size);
  void  (*free)   (const memory_manager_t *self, void   *ptr);
  void *(*calloc) (const memory_manager_t *self, size_t  nmemb, size_t size);
  void *(*realloc)(const memory_manager_t *self, void   *ptr,   size_t size);

  void  (*on_oom) (const memory_manager_t *self, size_t      size);
  void  (*on_err) (const memory_manager_t *self, const char *msg);

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

extern const memory_manager_t * const default_memory_manager;
extern const memory_manager_t malloc_manager;

/* Print a message to stderr and return. */
void memory_manager_default_on_oom(const memory_manager_t *self, size_t      size);
void memory_manager_default_on_err(const memory_manager_t *self, const char *msg);

/* Do nothing and return. */
void on_memory_manager_oom_do_nothing(const memory_manager_t *self, size_t      size);
void on_memory_manager_err_do_nothing(const memory_manager_t *self, const char *msg);

/* Both the reference and individual function pointers may be NULL. */
/*                                                                  */
/* Behaviour in case of NULL "calloc" and "realloc" pointers will   */
/* be based on the behaviour of the "malloc" and "free" fields.     */
/*                                                                  */
/* If only one of "malloc" and "free" is NULL, behaviour is         */
/* undefined.                                                       */
void *memory_manager_malloc (const memory_manager_t *memory_manager, size_t size);
void  memory_manager_free   (const memory_manager_t *memory_manager, void   *ptr);
void *memory_manager_calloc (const memory_manager_t *memory_manager, size_t nmemb, size_t size);
void *memory_manager_realloc(const memory_manager_t *memory_manager, void   *ptr,  size_t size);

void  memory_manager_on_oom (const memory_manager_t *memory_manager, size_t      size);
void  memory_manager_on_err (const memory_manager_t *memory_manager, const char *msg);

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_MEMORY_MANAGER_H */

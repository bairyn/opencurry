/*
 * opencurry: type_base_memory_tracker.h
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
 * type_base_memory_tracker.h
 * ------
 */

#ifndef TYPE_BASE_MEMORY_TRACKER_H
#define TYPE_BASE_MEMORY_TRACKER_H
/* stddef.h:
 *   - NULL
 *   - size_t
 */
#include <stddef.h>

#include "base.h"

#include "bits.h"

/* ---------------------------------------------------------------- */
/* Dependencies.                                                    */
/* ---------------------------------------------------------------- */

#include "type_base_prim.h"
#include "type_base_typed.h"
#include "type_base_tval.h"
#include "type_base_compare.h"
#include "type_base_lookup.h"
#include "type_base_memory_manager.h"

/* ---------------------------------------------------------------- */
/* Allocation types.                                                */
/* ---------------------------------------------------------------- */

enum allocation_type_e
{
  a_t_byte   = 0,
  a_t_tval   = 1,
  a_t_manual = 2,

  a_t_end,

  /* 1: 0-1 */
  /* 2: 0-3 */
  /* 3: 0-7 */
  a_t_bits     = 2,
  a_t_end_mask = ONE_BIT_REPEAT(a_t_bits)
};
typedef enum allocation_type_e allocation_type_t;

typedef void *byte_allocation_t;

typedef tval *tval_allocation_t;

typedef struct manual_allocation_s manual_allocation_t;
struct manual_allocation_s
{
  void (*cleanup)(void *context);
  void *context;
};

#define MANUAL_ALLOCATION_DEFAULTS \
  {                                \
    /* cleanup */ NULL             \
  , /* context */ NULL             \
  }
extern const manual_allocation_t manual_allocation_defaults;

/*
 * allocation_dependency_t:
 *
 * A relation between two allocations of either of the multiple types inside a
 * lookup_t container for each type.
 *
 * Both "parent" and "dependent" refer to an allocation in this container:
 *   1) The 2 (a_t_bits) last, i.e. least significant, bits, encode the
 *      allocation type, indicating which "lookup_t" container to index.
 *   2) the remaining bits encode the index to the value within this
 *      lookup container.
 */
typedef struct allocation_dependency_s allocation_dependency_t;
struct allocation_dependency_s
{
  /* Last two bits of each reserved for the respective type. */
  size_t parent;
  size_t dependent;
};

/* ---------------------------------------------------------------- */
/* Comparers on allocations.                                        */

extern const callback_compare_t cmp_byte_allocation;
extern const callback_compare_t cmp_tval_allocation;
extern const callback_compare_t cmp_manual_allocation;
extern const callback_compare_t cmp_allocation_dependency;
/* Key-based allocation dependency comparer on "parent". */
extern const callback_compare_t cmp_allocation_dependency_parent;

/* ---------------------------------------------------------------- */

/* Given an allocation_dependency_t's "parent" or "dependent", obtain its
 * allocation type or its index.
 */
#define GET_ALLOC_DEP_TYPE( ref) ((ref) & a_t_end_mask)
#define GET_ALLOC_DEP_INDEX(ref) ((ref) >> a_t_bits)
allocation_type_t get_alloc_dep_type (size_t ref);
size_t            get_alloc_dep_index(size_t ref);

/* Create an allocation_dependency "parent" or "dependent" value from an
 * allocation type and index.
 */
#define ALLOC_DEP_REF(type, index) (((type) & a_t_end_mask) | ((index) << a_t_bits))
size_t alloc_dep_ref(allocation_type_t type, size_t index);

/* "ALLOC_DEP_REF" for each type of allocation. */
#define ALLOC_DEP_BYTE(  index) (ALLOC_DEP_REF(a_t_byte,   (index)))
#define ALLOC_DEP_TVAL(  index) (ALLOC_DEP_REF(a_t_tval,   (index)))
#define ALLOC_DEP_MANUAL(index) (ALLOC_DEP_REF(a_t_manual, (index)))
size_t alloc_dep_byte  (size_t index);
size_t alloc_dep_tval  (size_t index);
size_t alloc_dep_manual(size_t index);

/* ---------------------------------------------------------------- */
/* Memory and value allocation management.                          */
/* ---------------------------------------------------------------- */

const type_t *memory_tracker_type(void);
extern const type_t memory_tracker_type_def;
typedef struct memory_tracker_s memory_tracker_t;
struct memory_tracker_s
{
  typed_t type;

  /* ---------------------------------------------------------------- */

  memory_manager_t memory_manager;

  /* ---------------------------------------------------------------- */

  /* Meta tracking. */

  /* Whether this tracker resides in dynamically         */
  /* allocated memory.                                   */
  /*                                                     */
  /* When this memory tracker is freed, this is freed    */
  /* last.                                               */
  void *dynamic_container;

  /* ---------------------------------------------------------------- */

  /* void * */
  lookup_t *byte_allocations;

  /* tval * */
  lookup_t *tval_allocations;

  /* manual_allocation_t */
  lookup_t *manual_allocations;

  /* allocation_dependency_t */
  lookup_t *dependency_graph;
};

#define MEMORY_TRACKER_DEFAULTS                      \
  { memory_tracker_type                              \
                                                     \
  /* memory_manager_defaults */                      \
  , /* memory_manager     */ MEMORY_MANAGER_DEFAULTS \
                                                     \
  , /* dynamic_container  */ NULL                    \
  , /* byte_allocations   */ NULL                    \
  , /* tval_allocations   */ NULL                    \
  , /* manual_allocations */ NULL                    \
  , /* dependency_graph   */ NULL                    \
  }

/* ---------------------------------------------------------------- */

/* A default memory tracker appropriate for top-level declarations. */
/* Uses "default_memory_manager" with a NULL buffer-array pointer.  */
/* 0 dynamically allocated buffer slots.                            */
extern const memory_tracker_t memory_tracker_defaults;

/* ---------------------------------------------------------------- */

/* A global memory tracker for general use. */
extern memory_tracker_t global_memory_tracker;

/* A global memory tracker for dynamic typed-value allocations, */
/* for types to use as a default.                               */
extern memory_tracker_t global_typed_dyn_memory_tracker;

/* ---------------------------------------------------------------- */

memory_tracker_t *memory_tracker_init(memory_tracker_t *dest, const memory_manager_t *memory_manager, void *dynamic_container);
size_t            memory_tracker_free(memory_tracker_t *tracker);

memory_tracker_t *memory_tracker_copy(memory_tracker_t *dest, const memory_tracker_t *src);

memory_tracker_t *memory_tracker_require_containers(memory_tracker_t *tracker);
size_t            memory_tracker_free_containers   (memory_tracker_t *tracker);

/* ---------------------------------------------------------------- */

/*   track methods: returns index >= 0 on success.  Duplicates are errors.  */
/* untrack methods: returns the allocation pointer when it exists.          */
/* tracked methods: returns index if tracked, -1 if not.                    */
/*    free methods: returns number of allocations freed.                    */

/* untrack methods ignore dependencies! */

int                   track_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation);
byte_allocation_t   untrack_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation);
int                 tracked_byte_allocation  (const memory_tracker_t *tracker, byte_allocation_t   allocation);
size_t                 free_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation);
size_t    free_byte_allocation_dependencies  (      memory_tracker_t *tracker, byte_allocation_t   allocation);

int                   track_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation);
tval_allocation_t   untrack_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation);
int                 tracked_tval_allocation  (const memory_tracker_t *tracker, tval_allocation_t   allocation);
size_t                 free_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation);
size_t    free_tval_allocation_dependencies  (      memory_tracker_t *tracker, tval_allocation_t   allocation);

int                   track_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation);
manual_allocation_t untrack_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation);
int                 tracked_manual_allocation(const memory_tracker_t *tracker, manual_allocation_t allocation);
size_t                 free_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation);
size_t    free_manual_allocation_dependencies(      memory_tracker_t *tracker, manual_allocation_t allocation);

/* ---------------------------------------------------------------- */

/* track:   returns index.                                                  */
/* untrack: takes a dependency index.                                       */
/* tracked: returns index; sets first dependent.                            */
int                       track_dependency(      memory_tracker_t *tracker, allocation_type_t parent_type, int parent, allocation_type_t dependent_type, int dependent);
allocation_dependency_t untrack_dependency(      memory_tracker_t *tracker, int index);
int                     tracked_dependency(const memory_tracker_t *tracker, allocation_type_t parent_type, int parent, allocation_type_t *out_dependent_type, int *out_dependent);

/* ---------------------------------------------------------------- */

/* >= 1 when exists. */
int untrack_allocation(memory_tracker_t *memory_tracker, allocation_type_t type, int index);

/* Returns number of freed allocations. */
size_t free_allocation             (memory_tracker_t *tracker, allocation_type_t type, int index);
size_t free_allocation_dependencies(memory_tracker_t *tracker, allocation_type_t type, int index);

/* ---------------------------------------------------------------- */

#define MEMORY_TRACKER_MANAGER( tracker) ((      memory_manager_t *) (ARROW_REF((tracker), memory_manager)))
#define MEMORY_TRACKER_CMANAGER(tracker) ((const memory_manager_t *) (ARROW_REF((tracker), memory_manager)))
      memory_manager_t *memory_tracker_manager (      memory_tracker_t *tracker);
const memory_manager_t *memory_tracker_cmanager(const memory_tracker_t *tracker);

#define MEMORY_TRACKER_DYNAMIC_CONTAINER(tracker) ((void *) (ARROW_REF((tracker), dynamic_container)))
void *memory_tracker_dynamic_container (const memory_tracker_t *tracker);

/* ---------------------------------------------------------------- */

int    track_malloc (memory_tracker_t *tracker, size_t  size);
int    track_calloc (memory_tracker_t *tracker, size_t  nmemb, size_t size);
int    track_realloc(memory_tracker_t *tracker, void   *ptr,   size_t size);
size_t track_free   (memory_tracker_t *tracker, void   *ptr);

/* ---------------------------------------------------------------- */

size_t track_tval_free(memory_tracker_t *tracker, tval *val);

/* ---------------------------------------------------------------- */

size_t track_manual_allocation_free(memory_tracker_t *tracker, manual_allocation_t allocation);

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_MEMORY_TRACKER_H */

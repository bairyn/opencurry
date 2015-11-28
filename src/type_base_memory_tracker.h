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

/* TODO: structs and methods for:         */
/*   1) allocations of any type           */
/*   2) allocation references of any type */

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
  size_t (*cleanup)(void *context);
  void    *context;
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

extern const manual_allocation_t null_manual_allocation;
int is_manual_allocation_null(manual_allocation_t manual_allocation);

extern const allocation_dependency_t null_allocation_dependency;
int is_allocation_dependency_null(allocation_dependency_t manual_allocation);

manual_allocation_t manual_allocation(size_t (*cleanup)(void *context), void *context);

allocation_dependency_t allocation_dependency    (size_t parent, size_t dependent);
allocation_dependency_t allocation_dependency_key(size_t parent);

allocation_dependency_t allocation_depends    (allocation_type_t parent_type, int parent_index, allocation_type_t dependent_type, int dependent_index);
allocation_dependency_t allocation_depends_key(allocation_type_t parent_type, int parent_index);

/* ---------------------------------------------------------------- */
/* Comparers on allocations.                                        */

extern const callback_compare_t cmp_byte_allocation;
extern const callback_compare_t cmp_tval_allocation;
extern const callback_compare_t cmp_manual_allocation;
extern const callback_compare_t cmp_allocation_dependency;
/* Key-based allocation dependency comparer on "parent". */
extern const callback_compare_t cmp_allocation_dependency_key;

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
  /*                                                     */
  /* This behaves like a standard memory, byte,          */
  /* allocation, so is freed by the memory_manager's     */
  /* "mfree" method.                                     */
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

/*   track methods: returns index >= 0 on success.  Duplicates are nops.    */
/* untrack methods: returns the allocation pointer when it exists.          */
/* tracked methods: returns index if tracked, -1 if not.                    */
/*    free methods: returns number of allocations freed.                    */

/* replace methods: preserving dependencies,                                */
/*                  and returning the index of dest on success;             */
/*                  untrack src and then track dest if untracked.           */

/* untrack methods ignore dependencies! */

/* TODO: free_dependencies: handle loops! */

#define UNTRACKED -1

int                   track_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation);
int                 replace_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation_src, byte_allocation_t   allocation_dest);
byte_allocation_t   untrack_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation);
byte_allocation_t       get_byte_allocation  (      memory_tracker_t *tracker, int                 index     );
int                 tracked_byte_allocation  (const memory_tracker_t *tracker, byte_allocation_t   allocation);
size_t                 free_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation);
size_t    free_byte_allocation_dependencies  (      memory_tracker_t *tracker, byte_allocation_t   allocation);

int                   track_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation);
int                 replace_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation_src, tval_allocation_t   allocation_dest);
tval_allocation_t   untrack_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation);
tval_allocation_t       get_tval_allocation  (      memory_tracker_t *tracker, int                 index     );
int                 tracked_tval_allocation  (const memory_tracker_t *tracker, tval_allocation_t   allocation);
size_t                 free_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation);
size_t    free_tval_allocation_dependencies  (      memory_tracker_t *tracker, tval_allocation_t   allocation);

int                   track_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation);
int                 replace_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation_src, manual_allocation_t allocation_dest);
manual_allocation_t untrack_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation);
manual_allocation_t     get_manual_allocation(      memory_tracker_t *tracker, int                 index     );
int                 tracked_manual_allocation(const memory_tracker_t *tracker, manual_allocation_t allocation);
size_t                 free_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation);
size_t    free_manual_allocation_dependencies(      memory_tracker_t *tracker, manual_allocation_t allocation);

/* ---------------------------------------------------------------- */

int replace_with_byte_allocation  (memory_tracker_t *tracker, allocation_type_t src_type, int src_index, byte_allocation_t   allocation_dest);
int replace_with_tval_allocation  (memory_tracker_t *tracker, allocation_type_t src_type, int src_index, tval_allocation_t   allocation_dest);
int replace_with_manual_allocation(memory_tracker_t *tracker, allocation_type_t src_type, int src_index, manual_allocation_t allocation_dest);

/* ---------------------------------------------------------------- */

/* track:   returns index.                                                  */
/* untrack: takes a dependency index.                                       */
/* tracked: returns index; sets first dependent.                            */
int                           track_dependency(      memory_tracker_t *tracker, allocation_dependency_t dependency);
allocation_dependency_t     untrack_dependency(      memory_tracker_t *tracker, allocation_dependency_t dependency);
allocation_dependency_t         get_dependency(      memory_tracker_t *tracker, int index);
int                         tracked_dependency(const memory_tracker_t *tracker, allocation_dependency_t dependency);
size_t                         free_dependency(      memory_tracker_t *tracker, allocation_dependency_t dependency);

/* Return value corresponds to first match. */
allocation_dependency_t untrack_dependency_key(      memory_tracker_t *tracker, allocation_type_t parent_type, int parent_index, allocation_dependency_t *out_dependencies,       size_t dependencies_num_max,       size_t *out_num_dependencies);
int                     tracked_dependency_key(const memory_tracker_t *tracker, allocation_type_t parent_type, int parent_index, int                     *out_dependency_indices, size_t dependency_indices_num_max, size_t *out_num_dependencies);
size_t                     free_dependency_key(      memory_tracker_t *tracker, allocation_type_t parent_type, int parent_index);

int                              track_depends(      memory_tracker_t *tracker, allocation_type_t parent_type, int parent_index, allocation_type_t dependent_type, int dependent_index);
allocation_dependency_t        untrack_depends(      memory_tracker_t *tracker, allocation_type_t parent_type, int parent_index, allocation_type_t dependent_type, int dependent_index);
int                            tracked_depends(const memory_tracker_t *tracker, allocation_type_t parent_type, int parent_index, allocation_type_t dependent_type, int dependent_index);
size_t                            free_depends(      memory_tracker_t *tracker, allocation_type_t parent_type, int parent_index, allocation_type_t dependent_type, int dependent_index);

/* Returns number of dependency replacements >=0 performed on success. */
/* Performs no tracking or untracking.                                 */
/* Both "src" and "dest" should exist for successful replacement.      */
int dependency_replace_allocation(memory_tracker_t *tracker, allocation_type_t src_type, int src_index, allocation_type_t dest_type, int dest_index);

/* ---------------------------------------------------------------- */

/* >= 1 when exists. */
int untrack_allocation(memory_tracker_t *tracker, allocation_type_t type, int index);

int replace_with_existing_allocation(memory_tracker_t *tracker, allocation_type_t src_type, int src_index, allocation_type_t dest_type, int dest_index);

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

void   *track_mmalloc  (memory_tracker_t *tracker, size_t size,               int *out_index);
void   *track_mcalloc  (memory_tracker_t *tracker, size_t nmemb, size_t size, int *out_index);
void   *track_mrealloc (memory_tracker_t *tracker, void   *ptr,  size_t size, int *out_index);
size_t  track_mfree    (memory_tracker_t *tracker, void   *ptr);

/* ---------------------------------------------------------------- */

tval   *track_tval_init(memory_tracker_t *tracker, const type_t *type, tval *cons, int *out_index);
size_t  track_tval_free(memory_tracker_t *tracker, tval *val);

/* ---------------------------------------------------------------- */

size_t track_manual_free(memory_tracker_t *tracker, manual_allocation_t cleanup);

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_MEMORY_TRACKER_H */

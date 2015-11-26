/*
 * opencurry: type_base_memory_tracker.c
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

#include "base.h"
#include "type_base_prim.h"
#include "type_base_memory_tracker.h"

#include "type_base_typed.h"
#include "type_base_tval.h"
#include "type_base_memory_manager.h"
#include "type_base_type.h"

#include "cpp.h"
#include "util.h"

/* ---------------------------------------------------------------- */
/* Allocation types.                                                */
/* ---------------------------------------------------------------- */

/* TODO: types */

const manual_allocation_t manual_allocation_defaults =
  MANUAL_ALLOCATION_DEFAULTS;

/* ---------------------------------------------------------------- */
/* Comparers on allocations.                                        */

static int compare_byte_allocation(void *context, const void * const *check, const void * const *baseline)
{
  return compare_objp(compare_objp_context(), check, baseline);
}

static int compare_tval_allocation(void *context, const tval * const *check, const void * const *baseline)
{
  return compare_objp(compare_objp_context(), check, baseline);
}

static int compare_manual_allocation(void *context, const manual_allocation_t *check, const manual_allocation_t *baseline)
{
  int ordering;

  if (!(ordering = compare_funp(compare_funp_context(), (const funp_cast_t *) &check->cleanup, (const funp_cast_t *) &baseline->cleanup)))
    return ordering;

  return compare_objp(compare_objp_context(), (const objpc_cast_t *) &check->context, (const objpc_cast_t *) &baseline->context);
}

static int compare_allocation_dependency(void *context, const allocation_dependency_t *check, const allocation_dependency_t *baseline)
{
  int ordering;

  if (!(ordering = compare_size(compare_size_context(), &check->parent, &baseline->parent)))
    return ordering;

  return compare_size(compare_size_context(), &check->dependent, &baseline->dependent);
}

static int compare_allocation_dependency_key(void *context, const allocation_dependency_t *check, const allocation_dependency_t *baseline)
{
  return compare_size(compare_size_context(), &check->parent, &baseline->parent);
}

const callback_compare_t cmp_byte_allocation =
  { callback_compare_type

  , /* comparer         */ (comparer_t) compare_byte_allocation
  , /* comparer_context */ NULL
  };

const callback_compare_t cmp_tval_allocation =
  { callback_compare_type

  , /* comparer         */ (comparer_t) compare_tval_allocation
  , /* comparer_context */ NULL
  };

const callback_compare_t cmp_manual_allocation =
  { callback_compare_type

  , /* comparer         */ (comparer_t) compare_manual_allocation
  , /* comparer_context */ NULL
  };

const callback_compare_t cmp_allocation_dependency =
  { callback_compare_type

  , /* comparer         */ (comparer_t) compare_allocation_dependency
  , /* comparer_context */ NULL
  };

const callback_compare_t cmp_allocation_dependency_key =
  { callback_compare_type

  , /* comparer         */ (comparer_t) compare_allocation_dependency_key
  , /* comparer_context */ NULL
  };

/* ---------------------------------------------------------------- */

allocation_type_t get_alloc_dep_type (size_t ref)
{
  return GET_ALLOC_DEP_TYPE(ref);
}

size_t            get_alloc_dep_index(size_t ref)
{
  return GET_ALLOC_DEP_INDEX(ref);
}


/* Create an allocation_dependency "parent" or "dependent" value from an
 * allocation type and index.
 */
size_t alloc_dep_ref(allocation_type_t type, size_t index)
{
  return ALLOC_DEP_REF(type, index);
}


/* "alloc_dep_ref" for each type of allocation. */
size_t alloc_dep_byte  (size_t index)
{
  return ALLOC_DEP_BYTE(index);
}

size_t alloc_dep_tval  (size_t index)
{
  return ALLOC_DEP_TVAL(index);
}

size_t alloc_dep_manual(size_t index)
{
  return ALLOC_DEP_MANUAL(index);
}

/* ---------------------------------------------------------------- */
/* Memory and value allocation management.                          */
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
  , /* @indirect              */ memory_tracker_type

  , /* self                   */ NULL
  , /* container              */ NULL

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

    /* void *dynamic_container; */
    STRUCT_INFO_RADD(objp_type(), dynamic_container);

    /* lookup_t *byte_allocations;   */
    /* lookup_t *tval_allocations;   */
    /* lookup_t *manual_allocations; */
    /* lookup_t *dependency_graph;   */
    STRUCT_INFO_RADD(objp_type(), byte_allocations);
    STRUCT_INFO_RADD(objp_type(), tval_allocations);
    STRUCT_INFO_RADD(objp_type(), manual_allocations);
    STRUCT_INFO_RADD(objp_type(), dependency_graph);

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

/* ---------------------------------------------------------------- */

/*
 * Global memory trackers.
 */

memory_tracker_t global_memory_tracker =
  MEMORY_TRACKER_DEFAULTS;

memory_tracker_t global_typed_dyn_memory_tracker =
  MEMORY_TRACKER_DEFAULTS;

/* ---------------------------------------------------------------- */

memory_tracker_t *memory_tracker_init(memory_tracker_t *dest, const memory_manager_t *memory_manager, void *dynamic_container)
{
  int dynamically_allocated;

  UNUSED(dynamically_allocated);

  memory_manager = require_memory_manager(memory_manager);

  if (!dest)
  {
    dynamically_allocated = TRUE();

    if (dynamic_container)
    {
      /* Error: Cannot dynamically allocate a memory           */
      /* tracker with an existing "dynamic_container" present. */
      return NULL;
    }

    dest = memory_manager_mmalloc(memory_manager, sizeof(*dest));

    if (!dest)
    {
      /* Error: failed to allocate memory! */
      return NULL;
    }
  }
  else
  {
    dynamically_allocated = FALSE();

    dest->dynamic_container = dynamic_container;
  }

  dest->type = memory_tracker_type;

  dest->memory_manager     = *memory_manager;
  /* dest->dynamic_container */
  dest->byte_allocations   = NULL;
  dest->tval_allocations   = NULL;
  dest->manual_allocations = NULL;
  dest->dependency_graph   = NULL;

  dest = memory_tracker_require_containers(dest);

  if (!dest)
  {
    /* Error: failed to allocate memory for trackers. */
    return NULL;
  }

  /* Success! =) */
  return dest;
}

size_t memory_tracker_free(memory_tracker_t *tracker)
{
  size_t num_freed;

  memory_manager_t  manager;
  void             *dynamic_container;

#if ERROR_CHECKING
  if (!tracker)
    return 0;
#endif /* #if ERROR_CHECKING */

  num_freed = 0;

  memory_manager_copy(&manager, require_memory_manager(MEMORY_TRACKER_CMANAGER(tracker)));
  dynamic_container = MEMORY_TRACKER_DYNAMIC_CONTAINER(tracker);

  /* ---------------------------------------------------------------- */

  /* Free containers. */
  num_freed += memory_tracker_free_containers(tracker);

  /* Cleanup memory_manager. */
  num_freed += memory_manager_deinit(&tracker->memory_manager);

  /* ---------------------------------------------------------------- */

  tracker->dependency_graph   = NULL;
  tracker->manual_allocations = NULL;
  tracker->tval_allocations   = NULL;
  tracker->byte_allocations   = NULL;
  tracker->dynamic_container  = NULL;
  /* tracker->memory_manager */

  tracker->type = NULL;

  ++num_freed;

  /* ---------------------------------------------------------------- */

  if (dynamic_container)
  {
    memory_manager_mfree(&manager, dynamic_container);
      ++num_freed;
  }

  return num_freed;
}

/* Special case: can dynamically allocate "dest" if "src" lacks a dynamic
 * container; then "dest"'s dynamic container refers to the newly allocated
 * "dest".
 *
 * N.B. Otherwise, "dest->dynamic_container" is copied from "src"!
 */
memory_tracker_t *memory_tracker_copy(memory_tracker_t *dest, const memory_tracker_t *src)
{
  int dynamically_allocated;

  UNUSED(dynamically_allocated);

#if ERROR_CHECKING
  if (!src)
    return NULL;
#endif /* #if ERROR_CHECKING */

  if (!dest)
  {
    dynamically_allocated = TRUE();

    if (src->dynamic_container)
    {
      /* Error: Cannot dynamically allocate a memory             */
      /* when "src" has an existing "dynamic_container" present. */
      return NULL;
    }

    dest = memory_manager_mmalloc(MEMORY_TRACKER_CMANAGER(src), sizeof(*dest));

    if (!dest)
    {
      /* Error: failed to allocate memory! */
      return NULL;
    }

    /* Special case: assign dest->dynamic_container. */

    dest->dynamic_container = dest;
  }
  else
  {
    dynamically_allocated = FALSE();

    dest->dynamic_container = src->dynamic_container;
  }

  dest->type = src->type;

  memory_manager_copy(&dest->memory_manager, &src->memory_manager);
  /* dest->dynamic_container */
  dest->byte_allocations   = NULL;
  dest->tval_allocations   = NULL;
  dest->manual_allocations = NULL;
  dest->dependency_graph   = NULL;

  return dest;
}

memory_tracker_t *memory_tracker_require_containers(memory_tracker_t *tracker)
{
#if ERROR_CHECKING
  if (!tracker)
    return NULL;
#endif /* #if ERROR_CHECKING */

  if (!tracker->byte_allocations)
  {
    int is_duplicate = -1;

    lookup_t     *lookup;
    const size_t  value_size = sizeof(byte_allocation_t);

    const memory_manager_t *memory_manager = MEMORY_TRACKER_CMANAGER(tracker);

    tracker->byte_allocations =
      lookup = memory_manager_mmalloc(memory_manager, sizeof(*tracker->byte_allocations));

    if (lookup)
      lookup = lookup_init_empty(lookup, value_size);

    if (lookup)
      lookup = lookup_minsert
        ( tracker->byte_allocations
        , &lookup
        , LOOKUP_NO_ADD_DUPLICATES

        , cmp_byte_allocation

        , memory_manager

        , NULL
        , &is_duplicate
        );

    if (is_duplicate)
      lookup = NULL;

    if (!lookup)
    {
      /* Error: failed to initialize lookup container! */

      if (tracker->byte_allocations)
      {
        lookup_deinit(tracker->byte_allocations, memory_manager);
        memory_manager_mfree
          ( memory_manager
          , tracker->byte_allocations
          );
        tracker->byte_allocations = NULL;
      }

      return NULL;
    }

    tracker->byte_allocations = lookup;
  }

  if (!tracker->tval_allocations)
  {
    int is_duplicate = -1;

    lookup_t     *lookup;
    const size_t  value_size = sizeof(tval_allocation_t);

    const memory_manager_t *memory_manager = MEMORY_TRACKER_CMANAGER(tracker);

    tracker->tval_allocations =
      lookup = memory_manager_mmalloc(memory_manager, sizeof(*tracker->tval_allocations));

    if (lookup)
      lookup = lookup_init_empty(lookup, value_size);

    if (lookup)
      if (!lookup_minsert
            ( tracker->byte_allocations
            , &lookup
            , LOOKUP_NO_ADD_DUPLICATES

            , cmp_byte_allocation

            , memory_manager

            , NULL
            , &is_duplicate
            )
         )
        lookup = NULL;

    if (is_duplicate)
      lookup = NULL;

    if (!lookup)
    {
      /* Error: failed to initialize lookup container! */

      lookup_deinit(tracker->byte_allocations, memory_manager);
      memory_manager_mfree
        ( memory_manager
        , tracker->byte_allocations
        );
      tracker->byte_allocations = NULL;

      if (tracker->tval_allocations)
      {
        lookup_deinit(tracker->tval_allocations, memory_manager);
        memory_manager_mfree
          ( memory_manager
          , tracker->tval_allocations
          );
        tracker->tval_allocations = NULL;
      }

      return NULL;
    }

    tracker->tval_allocations = lookup;
  }

  if (!tracker->manual_allocations)
  {
    int is_duplicate = -1;

    lookup_t     *lookup;
    const size_t  value_size = sizeof(manual_allocation_t);

    const memory_manager_t *memory_manager = MEMORY_TRACKER_CMANAGER(tracker);

    tracker->manual_allocations =
      lookup = memory_manager_mmalloc(memory_manager, sizeof(*tracker->manual_allocations));

    if (lookup)
      lookup = lookup_init_empty(lookup, value_size);

    if (lookup)
      if (!lookup_minsert
            ( tracker->byte_allocations
            , &lookup
            , LOOKUP_NO_ADD_DUPLICATES

            , cmp_byte_allocation

            , memory_manager

            , NULL
            , &is_duplicate
            )
         )
        lookup = NULL;

    if (is_duplicate)
      lookup = NULL;

    if (!lookup)
    {
      /* Error: failed to initialize lookup container! */

      lookup_deinit(tracker->byte_allocations, memory_manager);
      memory_manager_mfree
        ( memory_manager
        , tracker->byte_allocations
        );
      tracker->byte_allocations = NULL;

      lookup_deinit(tracker->tval_allocations, memory_manager);
      memory_manager_mfree
        ( memory_manager
        , tracker->tval_allocations
        );
      tracker->tval_allocations = NULL;

      if (tracker->manual_allocations)
      {
        lookup_deinit(tracker->manual_allocations, memory_manager);
        memory_manager_mfree
          ( memory_manager
          , tracker->manual_allocations
          );
        tracker->manual_allocations = NULL;
      }

      return NULL;
    }

    tracker->tval_allocations = lookup;
  }

  if (!tracker->dependency_graph)
  {
    int is_duplicate = -1;

    lookup_t     *lookup;
    const size_t  value_size = sizeof(allocation_dependency_t);

    const memory_manager_t *memory_manager = MEMORY_TRACKER_CMANAGER(tracker);

    tracker->dependency_graph =
      lookup = memory_manager_mmalloc(memory_manager, sizeof(*tracker->dependency_graph));

    if (lookup)
      lookup = lookup_init_empty(lookup, value_size);

    if (lookup)
      if (!lookup_minsert
            ( tracker->byte_allocations
            , &lookup
            , LOOKUP_NO_ADD_DUPLICATES

            , cmp_byte_allocation

            , memory_manager

            , NULL
            , &is_duplicate
            )
         )
        lookup = NULL;

    if (is_duplicate)
      lookup = NULL;

    if (!lookup)
    {
      /* Error: failed to initialize lookup container! */

      lookup_deinit(tracker->byte_allocations, memory_manager);
      memory_manager_mfree
        ( memory_manager
        , tracker->byte_allocations
        );
      tracker->byte_allocations = NULL;

      lookup_deinit(tracker->tval_allocations, memory_manager);
      memory_manager_mfree
        ( memory_manager
        , tracker->tval_allocations
        );
      tracker->tval_allocations = NULL;

      lookup_deinit(tracker->manual_allocations, memory_manager);
      memory_manager_mfree
        ( memory_manager
        , tracker->manual_allocations
        );
      tracker->manual_allocations = NULL;

      if (tracker->dependency_graph)
      {
        lookup_deinit(tracker->dependency_graph, memory_manager);
        memory_manager_mfree
          ( memory_manager
          , tracker->dependency_graph
          );
        tracker->dependency_graph = NULL;
      }

      return NULL;
    }

    tracker->tval_allocations = lookup;
  }

  return tracker;
}

size_t memory_tracker_free_containers(memory_tracker_t *tracker)
{
  size_t num_freed;

  const memory_manager_t *manager;

  void *free_byte_allocations   = NULL;
  void *free_tval_allocations   = NULL;
  void *free_manual_allocations = NULL;
  void *free_dependency_graph   = NULL;

  lookup_t            *lookup;
  const bnode_t       *root;

  byte_allocation_t    byte_value;
  tval_allocation_t    tval_value;
  manual_allocation_t  manual_value;

  size_t               result_num;
  byte_allocation_t    result_byte;

#if ERROR_CHECKING
  if (!tracker)
    return 0;
#endif /* #if ERROR_CHECKING */

  num_freed = 0;

  manager = MEMORY_TRACKER_CMANAGER(tracker);

  /* ---------------------------------------------------------------- */
  /* Free all non--container allocations.                             */

  if (tracker->byte_allocations)
  {
    lookup = tracker->byte_allocations;

    while(!LOOKUP_EMPTY(tracker->byte_allocations))
    {
      root       =
        ( (LOOKUP_ROOT_CNODE(lookup)) );
      byte_value = DEREF_PTR
        ( (const byte_allocation_t *)
          (LOOKUP_NODE_VALUE(lookup, root))
        );

      if      (byte_value == tracker->byte_allocations   && !free_byte_allocations)
      {
        free_byte_allocations   = byte_value;
      }
      else if (byte_value == tracker->tval_allocations   && !free_tval_allocations)
      {
        free_tval_allocations   = byte_value;
      }
      else if (byte_value == tracker->manual_allocations && !free_manual_allocations)
      {
        free_manual_allocations = byte_value;
      }
      else if (byte_value == tracker->dependency_graph   && !free_dependency_graph)
      {
        free_dependency_graph   = byte_value;
      }
      else
      {
        /* Standard allocation.  Free it. */

        result_num = free_byte_allocation(tracker, byte_value);
        if (!result_num)
        {
          /* Error: failed to free a byte allocation! */
#if ERROR_CHECKING
          report_bug
            ( "Error: memory_tracker_free_containers: line #"
              CURRENT_LINE_STR
              "failed to free a byte allocation!\n"
            );
#endif /* #if ERROR_CHECKING */
          break;
        }

        num_freed += result_num;
        continue;
      }

      /* Just remove the allocation container reference. */
      /*                                                 */
      /* We'll free the container once we're done        */
      /* freeing all non-container allocations.          */

      result_byte = untrack_byte_allocation(tracker, byte_value);
      if (!result_byte)
      {
          /* Error: failed to untrack a container! */
#if ERROR_CHECKING
          report_bug
            ( "Error: memory_tracker_free_containers: line #"
              CURRENT_LINE_STR
              "failed to untrack container!\n"
            );
#endif /* #if ERROR_CHECKING */
          break;
      }

      continue;
    }
  }

  if (tracker->tval_allocations)
  {
    lookup = tracker->tval_allocations;

    while(!LOOKUP_EMPTY(tracker->tval_allocations))
    {
      root       =
        ( (LOOKUP_ROOT_CNODE(lookup)) );
      tval_value = DEREF_PTR
        ( (const tval_allocation_t *)
          (LOOKUP_NODE_VALUE(lookup, root))
        );

      result_num = free_tval_allocation(tracker, tval_value);
      if (!result_num)
      {
        /* Error: failed to free a tval allocation! */
#if ERROR_CHECKING
        report_bug
          ( "Error: memory_tracker_free_containers: line #"
            CURRENT_LINE_STR
            "failed to free a tval allocation!\n"
          );
#endif /* #if ERROR_CHECKING */
        break;
      }

      num_freed += result_num;
    }
  }

  if (tracker->manual_allocations)
  {
    lookup = tracker->manual_allocations;

    while(!LOOKUP_EMPTY(tracker->manual_allocations))
    {
      root         =
        ( (LOOKUP_ROOT_CNODE(lookup)) );
      manual_value = DEREF_DEFAULT
        ( (const manual_allocation_t *)
          (LOOKUP_NODE_VALUE(lookup, root))
        , manual_allocation_defaults
        );

      result_num = free_manual_allocation(tracker, manual_value);
      if (!result_num)
      {
        /* Error: failed to free a manual allocation! */
#if ERROR_CHECKING
        report_bug
          ( "Error: memory_tracker_free_containers: line #"
            CURRENT_LINE_STR
            "failed to free a manual allocation!\n"
          );
#endif /* #if ERROR_CHECKING */
        break;
      }

      num_freed += result_num;
    }
  }

  /* ---------------------------------------------------------------- */
  /* Free containers.                                                 */

  if ((lookup = tracker->byte_allocations))
    num_freed += lookup_deinit(lookup, manager);
  if ((lookup = tracker->tval_allocations))
    num_freed += lookup_deinit(lookup, manager);
  if ((lookup = tracker->manual_allocations))
    num_freed += lookup_deinit(lookup, manager);
  if ((lookup = tracker->dependency_graph))
    num_freed += lookup_deinit(lookup, manager);

  if (free_byte_allocations)
    num_freed += memory_manager_mfree(manager, free_byte_allocations);
  if (free_tval_allocations)
    num_freed += memory_manager_mfree(manager, free_tval_allocations);
  if (free_manual_allocations)
    num_freed += memory_manager_mfree(manager, free_manual_allocations);
  if (free_dependency_graph)
    num_freed += memory_manager_mfree(manager, free_dependency_graph);

  /* ---------------------------------------------------------------- */
  /* Unset containers.                                                */

  tracker->byte_allocations   = NULL;
  tracker->tval_allocations   = NULL;
  tracker->manual_allocations = NULL;
  tracker->dependency_graph   = NULL;

  /* ---------------------------------------------------------------- */
  /* Success!                                                         */

  return num_freed;
}

/* ---------------------------------------------------------------- */
/* byte_allocation tracking.                                        */

int track_byte_allocation(memory_tracker_t *tracker, byte_allocation_t allocation)
{
  size_t value_index  = (size_t) -1;
  int    is_duplicate =          -1;

  lookup_t *lookup;

  UNUSED(is_duplicate);

#if ERROR_CHECKING
  if (!tracker)
    return -2;
#endif /* #if ERROR_CHECKING */

  if (!memory_tracker_require_containers(tracker))
    return -3;

  lookup = tracker->byte_allocations;

  if (!allocation)
    return -4;

  lookup =
    lookup_minsert
      ( lookup
      , (void *) &allocation
      , LOOKUP_NO_ADD_DUPLICATES

      , cmp_byte_allocation

      , MEMORY_TRACKER_CMANAGER(tracker)

      , &value_index
      , &is_duplicate
      );

  if (!lookup)
    return -5;

  return (int) value_index;
}

byte_allocation_t untrack_byte_allocation(memory_tracker_t *tracker, byte_allocation_t allocation)
{
  size_t num_deleted;

  lookup_t *lookup;

#if ERROR_CHECKING
  if (!tracker)
    return NULL;
#endif /* #if ERROR_CHECKING */

  if (!memory_tracker_require_containers(tracker))
    return NULL;

  lookup = tracker->byte_allocations;

  if (!allocation)
    return NULL;

  lookup =
    lookup_mdelete
      ( lookup
      , (void *) &allocation
      , LOOKUP_UNLIMITED

      , cmp_byte_allocation

      , MEMORY_TRACKER_CMANAGER(tracker)

      , &num_deleted
      );

  if (!lookup)
    return NULL;

  if (num_deleted <= 0)
    return NULL;

  return allocation;
}

int tracked_byte_allocation(const memory_tracker_t *tracker, byte_allocation_t allocation)
{
  int         index;
  const void *value;

  const lookup_t *lookup;

#if ERROR_CHECKING
  if (!tracker)
    return UNTRACKED - 1;
#endif /* #if ERROR_CHECKING */

  lookup = tracker->byte_allocations;

  if (!lookup)
    return UNTRACKED - 2;

  if (!allocation)
    return UNTRACKED - 3;

  value =
    lookup_retrieve
      ( lookup
      , (void *) &allocation

      , cmp_byte_allocation
      );

  if (!value)
    return UNTRACKED;

  index = (int) LOOKUP_GET_VALUE_INDEX(lookup, value);

  return index;
}

size_t free_byte_allocation(memory_tracker_t *tracker, byte_allocation_t allocation)
{
  size_t num_freed;

  size_t            result_num             = 0;
  byte_allocation_t result_byte_allocation = NULL;

  const memory_manager_t *manager;

#if ERROR_CHECKING
  if (!tracker)
    return 0;
#endif /* #if ERROR_CHECKING */

  if (!memory_tracker_require_containers(tracker))
    return 0;

  if (!allocation)
    return 0;

  manager = MEMORY_TRACKER_CMANAGER(tracker);

  /* ---------------------------------------------------------------- */

  num_freed = 0;

  /* ---------------------------------------------------------------- */
  /* Untrack byte allocation.                                         */

  result_byte_allocation = untrack_byte_allocation(tracker, allocation);
  if (!result_byte_allocation)
    return num_freed;
  ++num_freed;

  /* ---------------------------------------------------------------- */
  /* Free dependencies.                                               */

  num_freed += free_byte_allocation_dependencies(tracker, allocation);

  /* ---------------------------------------------------------------- */
  /* Free allocation.                                                 */

  result_num = memory_manager_mfree(manager, allocation);
  if (!result_num)
  {
    --num_freed;
    result_num = track_byte_allocation(tracker, allocation);
    if (!result_num)
      return num_freed;

    return num_freed;
  }

  num_freed += result_num;

  /* ---------------------------------------------------------------- */
  /* Done!                                                            */

  return num_freed;
}

size_t free_byte_allocation_dependencies(memory_tracker_t *tracker, byte_allocation_t allocation)
{
  size_t num_freed;

  int index;

  const void *value;
  const allocation_dependency_t *
    const     dependency = (const void * const) &value;

  allocation_dependency_t key;

  size_t result_num;
  size_t subresult_num;

  lookup_t               *lookup;
  const memory_manager_t *manager;

#if ERROR_CHECKING
  if (!tracker)
    return 0;
#endif /* #if ERROR_CHECKING */

  if (!memory_tracker_require_containers(tracker))
    return 0;

  lookup = tracker->byte_allocations;

  if (!allocation)
    return 0;

  manager = MEMORY_TRACKER_CMANAGER(tracker);

  index = tracked_byte_allocation(tracker, allocation);
  if (index < 0)
    return 0;

  /* ---------------------------------------------------------------- */

  num_freed = 0;

  key = allocation_dependency_key(alloc_dep_byte((size_t) index));

  while((value = lookup_retrieve(lookup, &key, cmp_allocation_dependency_key)))
  {
    lookup =
      lookup_mdelete(lookup, value, LOOKUP_UNLIMITED, cmp_allocation_dependency_key, manager, &result_num);
    if (!lookup)
      return num_freed;
#if ERROR_CHECKING
    if (!result_num)
      return num_freed;
#endif /* #if ERROR_CHECKING */

    subresult_num =
      free_allocation
        ( tracker
        , GET_ALLOC_DEP_TYPE (dependency->dependent)
        , GET_ALLOC_DEP_INDEX(dependency->dependent)
        );
#if ERROR_CHECKING
    if (!subresult_num)
    {
      /* Error: Couldn't free dependent! */
      lookup_minsert(lookup, value, LOOKUP_ADD_DUPLICATES, cmp_allocation_dependency, manager, NULL, NULL);
      return num_freed;
    }
#endif /* #if ERROR_CHECKING */

    num_freed += result_num;
    num_freed += subresult_num;
  }

  return num_freed;
}

#ifdef TODO
/* ---------------------------------------------------------------- */
/* tval_allocation tracking.                                        */

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

/*   track methods: returns index >= 0 on success.  Duplicates are errors.  */
/* untrack methods: returns the allocation pointer when it exists.          */
/* tracked methods: returns index if tracked, -1 if not.                    */
int                   track_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation)
{
  int    is_duplicate;
  size_t node_index;

#if ERROR_CHECKING
  if (!tracker)
    return -1;
#endif /* #if ERROR_CHECKING */

  if (!memory_tracker_require_containers(tracker))
    return -2;

  if (!lookup_minsert
         ( tracker->byte_allocations
         , (void *) &allocation
         , 0

         , compare_callback_byte_allocation

         , tracker->memory_manager.calloc
           , &tracker->memory_manager
         , tracker->memory_manager.realloc
           , &tracker->memory_manager
         , tracker->memory_manager.free
           , &tracker->memory_manager

         , &value_index
         , &is_duplicate
         )
     )
    return -3;

  if (is_duplicate)
    return -4;

  return (int) value_index;
}

byte_allocation_t   untrack_byte_allocation  (      memory_tracker_t *tracker, byte_allocation_t   allocation)
{
}

int                 tracked_byte_allocation  (const memory_tracker_t *tracker, byte_allocation_t   allocation)
{
}


int                   track_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation)
{
  int    is_duplicate;
  size_t node_index;

#if ERROR_CHECKING
  if (!tracker)
    return -1;
#endif /* #if ERROR_CHECKING */

  if (!memory_tracker_require_containers(tracker))
    return -2;

  if (!lookup_minsert
         ( tracker->tval_allocations
         , (void *) &allocation
         , 0

         , compare_callback_tval_allocation

         , tracker->memory_manager.calloc
           , &tracker->memory_manager
         , tracker->memory_manager.realloc
           , &tracker->memory_manager
         , tracker->memory_manager.free
           , &tracker->memory_manager

         , &value_index
         , &is_duplicate
         )
     )
    return -3;

  if (is_duplicate)
    return -4;

  return (int) value_index;
}

tval_allocation_t   untrack_tval_allocation  (      memory_tracker_t *tracker, tval_allocation_t   allocation)
{
}

int                 tracked_tval_allocation  (const memory_tracker_t *tracker, tval_allocation_t   allocation)
{
}


int                   track_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation)
{
  int    is_duplicate;
  size_t node_index;

#if ERROR_CHECKING
  if (!tracker)
    return -1;
#endif /* #if ERROR_CHECKING */

  if (!memory_tracker_require_containers(tracker))
    return -2;

  if (!lookup_minsert
         ( tracker->manual_allocations
         , (void *) &allocation
         , 0

         , compare_callback_manual_allocation

         , tracker->memory_manager.calloc
           , &tracker->memory_manager
         , tracker->memory_manager.realloc
           , &tracker->memory_manager
         , tracker->memory_manager.free
           , &tracker->memory_manager

         , &value_index
         , &is_duplicate
         )
     )
    return -3;

  if (is_duplicate)
    return -4;

  return (int) value_index;
}

manual_allocation_t untrack_manual_allocation(      memory_tracker_t *tracker, manual_allocation_t allocation)
{
}

int                 tracked_manual_allocation(const memory_tracker_t *tracker, manual_allocation_t allocation)
{
}

/* ---------------------------------------------------------------- */




#endif /* #ifdef TODO */


































































#ifdef TODO /* TODO */
/* ---------------------------------------------------------------- */
/* memory_tracker                                                   */
/* ---------------------------------------------------------------- */

allocation_dependency_t depending_parent(size_t parent)
{
  allocation_dependency_t dep;

  dep.parent     = parent;
  dep.dependency = 0;

  return dep;
}

allocation_dependency_t allocation_dependency(size_t parent, size_t dependent)
{
  allocation_dependency_t dep;

  dep.parent     = parent;
  dep.dependency = dependennt;

  return dep;
}

allocation_type_t get_alloc_type(size_t ref)
{
  return GET_ALLOC_TYPE(ref);
}

size_t get_alloc_index(size_t ref)
{
  return GET_ALLOC_INDEX(ref);
}

size_t alloc_type_byte(size_t index)
{
  return ALLOC_TYPE_BYTE(index);
}

size_t alloc_type_tval(size_t index)
{
  return ALLOC_TYPE_TVAL(index);
}

size_t alloc_type_manual(size_t index)
{
  return ALLOC_TYPE_MANUAL(index);
}

#ifdef TODO
/* ---------------------------------------------------------------- */
/* memory_tracker                                                   */
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
  , /* @indirect              */ memory_tracker_type

  , /* self                   */ NULL
  , /* container              */ NULL

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

/* ---------------------------------------------------------------- */

/*
 * Global memory trackers.
 */

memory_tracker_t global_memory_tracker =
  MEMORY_TRACKER_DEFAULTS;

memory_tracker_t global_typed_dyn_memory_tracker =
  MEMORY_TRACKER_DEFAULTS;

/* ---------------------------------------------------------------- */

/*
 * Memory tracker methods.
 */

#ifndef TODO
int               memory_tracker_is_allocation_tracked  (memory_tracker_t *memory_tracker, const void *buffer_allocation, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return 0; }
memory_tracker_t *memory_tracker_track_allocation       (memory_tracker_t *memory_tracker, void *buffer_allocation, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return buffer_allocation; }
void             *memory_tracker_malloc_allocation      (memory_tracker_t *memory_tracker, size_t size, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return NULL; }
void             *memory_tracker_calloc_allocation      (memory_tracker_t *memory_tracker, size_t nmemb, size_t size, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return NULL; }
void             *memory_tracker_realloc_allocation     (memory_tracker_t *memory_tracker, void *buffer_allocation, size_t size, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return NULL; }
int               memory_tracker_untrack_allocation     (memory_tracker_t *memory_tracker, void *buffer_allocation, int allow_untracked, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return 0; }
int               memory_tracker_untrack_all_allocations(memory_tracker_t *memory_tracker, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return 0; }
int               memory_tracker_free_allocation        (memory_tracker_t *memory_tracker, void *buffer_allocation, int allow_untracked, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return 0; }
int               memory_tracker_free_all_allocations   (memory_tracker_t *memory_tracker, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return 0; }
int               memory_tracker_free_container         (memory_tracker_t *memory_tracker, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return 0; }
int               memory_tracker_get_tag                (memory_tracker_t *memory_tracker, void *buffer_allocation, size_t *out_tag, char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return 0; }
int               memory_tracker_set_tag                (memory_tracker_t *memory_tracker, void *buffer_allocation, size_t tag,      char *out_err_buf, size_t err_buf_size)
{ /* TODO */ return 0; }
#endif /* #ifndef TODO */

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
#endif /* #ifdef TODO */
#endif /* #ifdef TODO */

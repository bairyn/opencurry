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

/* ---------------------------------------------------------------- */
/* Dependencies.                                                    */
/* ---------------------------------------------------------------- */

#include "type_base_prim.h"
#include "type_base_typed.h"
#include "type_base_tval.h"
#include "type_base_memory_manager.h"

/* ---------------------------------------------------------------- */
/* Value allocation management.                                     */
/* ---------------------------------------------------------------- */

/*
 * memory_tracker_t:
 *
 * A memory tracker is associated with one value, or alternatively a collection
 * of values.
 *
 * A memory tracker is used primarily to track which values are dynamically
 * allocated, and dynamic allocation dependencies.
 *
 * A memory tracker contains the following information:
 *   - A collection
 *
 * A memory tracker contains the following information:
 */

/* TODO: Remove "buffers"; just use "allocations". */
const type_t *memory_tracker_type(void);
extern const type_t memory_tracker_type_def;
typedef struct memory_tracker_s memory_tracker_t;
struct memory_tracker_s
{
  typed_t type;

  memory_manager_t memory_manager;

#ifdef TODO
  /* Meta tracking. */

  /* Whether this tracker resides in dynamically         */
  /* allocated memory.                                   */
  /*                                                     */
  /* When this memory tracker is freed, this is freed    */
  /* last.                                               */
  void *dynamic_container;

  tval **dynamic_dependents;
#endif /* #ifdef TODO */


  /* If this memory tracker exists in dynamically        */
  /* allocated memory, this refers to it; otherwise this */
  /* is NULL.                                            */
  /*                                                     */
  /* Most commonly, this could be a reference to a       */
  /* dynamically allocated value of an associated struct */
  /* containing a memory_tracker_t, or it could be a     */
  /* reference to this memory tracker itself if it is    */
  /* dynamically allocated.                              */
  /*                                                     */
  /* This must be freed last.                            */
  void *dynamically_allocated_container;

  /* Array of "void *".  NULL elements are unused slots. */
  /*                                                     */
  /* This itself may be NULL.                            */
  /*                                                     */
  /* Usually, the first element refers to                */
  /* "dynamically_allocated_buffers" itself, so it       */
  /* must be freed after all other references in used    */
  /* slots are.                                          */
  /*                                                     */
  /* More generally, this rule applies to every other    */
  /* slot.  An even-indexed slot cannot be freed until   */
  /* all even-indexed slots that occur later in the      */
  /* array are freed.                                    */
  /*                                                     */
  /* The last even-indexed slot is reserved for an       */
  /* internally managed reference to a "tail"            */
  /* memory_tracker_t as one way to dynamically allocate */
  /* additional buffer references.                       */
  void   **dynamically_allocated_buffers;
  size_t   dynamically_allocated_buffers_num;
  size_t   dynamically_allocated_buffers_size;

  /* Index of the last used slots, or 0 if none.         */
  size_t   dynamically_allocated_buffers_last_even;
  size_t   dynamically_allocated_buffers_last_odd;
};

#define MEMORY_TRACKER_DEFAULTS                                           \
  { memory_tracker_type                                                   \
                                                                          \
  /* memory_manager_defaults */                                           \
  , /* memory_manager                          */ MEMORY_MANAGER_DEFAULTS \
                                                                          \
  , /* dynamically_allocated_container         */ NULL                    \
  , /* dynamically_allocated_buffers           */ NULL                    \
  , /* dynamically_allocated_buffers_num       */ 0                       \
  , /* dynamically_allocated_buffers_size      */ 0                       \
  , /* dynamically_allocated_buffers_last_even */ 0                       \
  , /* dynamically_allocated_buffers_last_odd  */ 0                       \
  }

/* A default memory tracker appropriate for top-level declarations. */
/* Uses "default_manager" with a NULL buffer-array pointer.         */
/* 0 dynamically allocated buffer slots.                            */
extern const memory_tracker_t memory_tracker_defaults;

/* A global memory tracker for general use. */
extern memory_tracker_t global_memory_tracker;

/* A global memory tracker for dynamic typed-value allocations, for types to use as a default. */
extern memory_tracker_t global_typed_dyn_memory_tracker;

/*
 * Initialize a "memory_tracker" with no allocated buffers.
 *
 * If the container is dynamically allocated, e.g. if an associated value, pass
 * it as an argument; otherwise set "dynamically_allocated_container" to NULL.
 *
 * If "memory_manager" is NULL, a reasonable default will be chosen.
 *
 * Returns NULL on success, and a pointer to an error message on failure.
 */
const char *memory_tracker_initialize_empty_with_container(memory_tracker_t *memory_tracker, const memory_manager_t *memory_manager, void *dynamically_allocated_container);


/* Is this allocation being tracked? */
int               memory_tracker_is_allocation_tracked  (memory_tracker_t *memory_tracker, const void *buffer_allocation, char *out_err_buf, size_t err_buf_size);

/* Adds an already-malloc'd buffer to the memory tracker, returning NULL on failure, otherwise returning "memory_tracker". */
memory_tracker_t *memory_tracker_track_allocation       (memory_tracker_t *memory_tracker, void *buffer_allocation, char *out_err_buf, size_t err_buf_size);

/* Returns a pointer to the new allocation. */
void             *memory_tracker_malloc_allocation      (memory_tracker_t *memory_tracker, size_t size, char *out_err_buf, size_t err_buf_size);
void             *memory_tracker_calloc_allocation      (memory_tracker_t *memory_tracker, size_t nmemb, size_t size, char *out_err_buf, size_t err_buf_size);
void             *memory_tracker_realloc_allocation     (memory_tracker_t *memory_tracker, void *buffer_allocation, size_t size, char *out_err_buf, size_t err_buf_size);

/* Returns <= -1 on error, >= 0 on success: 0 if allow_untracked and */
/* untracked, >=1 otherwise.                                         */
int               memory_tracker_untrack_allocation     (memory_tracker_t *memory_tracker, void *buffer_allocation, int allow_untracked, char *out_err_buf, size_t err_buf_size);
int               memory_tracker_untrack_all_allocations(memory_tracker_t *memory_tracker, char *out_err_buf, size_t err_buf_size);

/* Returns <= -1 on error, >= 0 on success: 0 if allow_untracked and */
/* untracked, >=1 otherwise.                                         */
int               memory_tracker_free_allocation        (memory_tracker_t *memory_tracker, void *buffer_allocation, int allow_untracked, char *out_err_buf, size_t err_buf_size);
int               memory_tracker_free_all_allocations   (memory_tracker_t *memory_tracker, char *out_err_buf, size_t err_buf_size);

/* Returns <= -1 on error, >= 0 on success: 0 when the tracker itself is not */
/* dynamically allocated, i.e. its container field value is NULL (all of its */
/* allocations are still freed), and >= 1 otherwise.                         */
int               memory_tracker_free_container         (memory_tracker_t *memory_tracker, char *out_err_buf, size_t err_buf_size);

/* Returns <= -1 on error, >= 0 on success:      */
/* 0 when no tag is set, >= 1 when a tag is set. */
int               memory_tracker_get_tag                (memory_tracker_t *memory_tracker, void *buffer_allocation, size_t *out_tag, char *out_err_buf, size_t err_buf_size);
int               memory_tracker_set_tag                (memory_tracker_t *memory_tracker, void *buffer_allocation, size_t tag,      char *out_err_buf, size_t err_buf_size);

/* ---------------------------------------------------------------- */
/* Post-dependencies.                                               */
/* ---------------------------------------------------------------- */

#ifdef TODO
#include "type_base_type.h"
#endif /* #ifdef TODO */

#endif /* ifndef TYPE_BASE_MEMORY_TRACKER_H */

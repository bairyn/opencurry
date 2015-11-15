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
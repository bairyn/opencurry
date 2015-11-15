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

/* ---------------------------------------------------------------- */
/* ref_traversal_t                                                  */
/* ---------------------------------------------------------------- */

/* TODO */
#ifdef TODO
/* TODO */
const type_t *ref_traversal_type(void);
extern const type_t ref_traversal_type_def;
typedef struct ref_traversal_s ref_traversal_t;
struct ref_traversal_s
{
  typed_t type;

  memory_tracker_t memory;

  /* TODO void TODO_on_loop; */

  void   **history;
  size_t   history_num;
  size_t   history_size;
  size_t   history_len;
};

/* ref_traversal type. */

const type_t *ref_traversal_type(void)
  { return &ref_traversal_type_def; }

static const char          *ref_traversal_type_name       (const type_t *self);
static size_t               ref_traversal_type_size       (const type_t *self, const tval *val);
static const struct_info_t *ref_traversal_type_is_struct  (const type_t *self);
static tval                *ref_traversal_type_init       (const type_t *self, tval *cons);
static const tval          *ref_traversal_type_has_default(const type_t *self);

const type_t ref_traversal_type_def =
  { type_type

    /* @: Required.           */

  , /* memory                 */ MEMORY_TRACKER_DEFAULTS
  , /* is_self_mutable        */ NULL
  , /* @indirect              */ ref_traversal_type

  , /* self                   */ NULL
  , /* container              */ NULL

  , /* typed                  */ NULL

  , /* @name                  */ ref_traversal_type_name
  , /* info                   */ NULL
  , /* @size                  */ ref_traversal_type_size
  , /* @is_struct             */ ref_traversal_type_is_struct
  , /* is_mutable             */ NULL
  , /* is_subtype             */ NULL
  , /* is_supertype           */ NULL

  , /* cons_type              */ NULL
  , /* init                   */ ref_traversal_type_init
  , /* free                   */ NULL
  , /* has_default            */ ref_traversal_type_has_default
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

static const char          *ref_traversal_type_name       (const type_t *self)
  { return "ref_traversal_t"; }

static size_t               ref_traversal_type_size       (const type_t *self, const tval *val)
  { return sizeof(ref_traversal_t); }

/* TODO */
#ifdef TODO
DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(ref_traversal)
static const struct_info_t *ref_traversal_type_is_struct  (const type_t *self)
  {
    STRUCT_INFO_BEGIN(ref_traversal);

    /* typed_t type */
    STRUCT_INFO_RADD(typed_type(), type);

    /* ptrdiff_t     field_pos;  */
    STRUCT_INFO_RADD(objp_type(),    TODO);

    STRUCT_INFO_DONE();
  }
#endif /* #ifdef TODO */

/*
static tval                *ref_traversal_type_init       (const type_t *self, tval *cons)
{
  ref_traversal_t *val;

  if ((!val = type_has_template_cons_basic_initializer(self, cons)))
    return NULL;

  val->is_mutable = val;

  TODO
}
*/

static const tval          *ref_traversal_type_has_default(const type_t *self)
  { return type_has_default_value(self, &ref_traversal_defaults); }

/* ---------------------------------------------------------------- */

const ref_traversal_t ref_traversal_defaults =
  REF_TRAVERSAL_DEFAULTS;
#endif /* #ifdef TODO */

/* ---------------------------------------------------------------- */

/* ref_traversal methods.
 */
#ifndef TODO
ref_traversal_t *ref_traversal_init_empty(ref_traversal_t *dest)
{ /* TODO */ return NULL; }
ref_traversal_t *ref_traversal_init_with_one(ref_traversal_t *dest, void *reference)
{ /* TODO */ return NULL; }
void *ref_traversal_add   (      ref_traversal_t *ref_traversal, void *reference)
{ /* TODO */ return NULL; }
void *ref_traversal_remove(      ref_traversal_t *ref_traversal, void *reference)
{ /* TODO */ return NULL; }
void *ref_traversal_exists(const ref_traversal_t *ref_traversal, void *reference)
{ /* TODO */ return NULL; }
void *ref_traversal_tagged_add   (      ref_traversal_t *ref_traversal, unsigned char tag, void *reference)
{ /* TODO */ return NULL; }
void *ref_traversal_tagged_remove(      ref_traversal_t *ref_traversal, unsigned char tag, void *reference)
{ /* TODO */ return NULL; }
void *ref_traversal_tagged_take  (      ref_traversal_t *ref_traversal, unsigned char tag, void *reference)
{ /* TODO */ return NULL; }
void *ref_traversal_tagged_exists(const ref_traversal_t *ref_traversal, unsigned char tag, void *reference)
{ /* TODO */ return NULL; }
#endif /* #ifndef TODO */

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
  , /* @indirect              */ field_info_type

  , /* self                   */ NULL
  , /* container              */ NULL

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
  , /* @indirect              */ struct_info_type

  , /* self                   */ NULL
  , /* container              */ NULL

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
  , /* @indirect              */ template_cons_type

  , /* self                   */ NULL
  , /* container              */ NULL

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
  , /* @indirect              */ template_cons_type

  , /* self                   */ NULL
  , /* container              */ NULL

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
  , /* @indirect              */ void_type

  , /* self                   */ NULL
  , /* container              */ NULL

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
  , /* @indirect              */ array_type

  , /* self                   */ NULL
  , /* container              */ NULL

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
  , /* @indirect              */ div_type

  , /* self                   */ NULL
  , /* container              */ NULL

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
  , /* @indirect              */ ldiv_type

  , /* self                   */ NULL
  , /* container              */ NULL

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

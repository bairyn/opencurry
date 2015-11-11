/*
 * opencurry: type_base.h
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
 * type_base.h
 * -----------
 */

#ifndef TYPE_BASE_H
#define TYPE_BASE_H
/* stddef.h:
 *   - offsetof
 *   - ptrdiff_t
 *   - size_t
 */
#include <stddef.h>

#include "base.h"

#include "bits.h"

/* TODO: type_t: is_const! */
/* TODO: type_t: has_const_type, and has_unconst_type */

/* ---------------------------------------------------------------- */
/* Out-of-order forward declarations.                               */
/* ---------------------------------------------------------------- */

/* Declarations for types used before their definitions. */

typedef struct type_s type_t;

typedef struct template_cons_s template_cons_t;

/* ---------------------------------------------------------------- */
/* tval                                                             */
/* ---------------------------------------------------------------- */

/* typed type. */

/* TODO: update documentation to reflect change from "type_t *" to "typed" for
 * procedurally generated types! */
const type_t *typed_type(void);
extern const type_t typed_type_def;
typedef const type_t *(*typed_t)(void);

#define TYPED_DEFAULT  \
  typed_type
extern const typed_t typed_default;

/*
 * tval:
 *
 * Label for so-called "typed values", values associated with a "type_t".
 *
 * "tvals" are generally values of a struct whose first field is "type_t *".
 *
 * Example:
 *
 * > extern type_t intpair_type;
 * >
 * > typedef struct intpair_s intpair_t;
 * > struct intpair_s
 * > {
 * >   type_t *type;
 * >
 * >   int     a;
 * >   int     b;
 * > };
 * >
 * > void debug_print(const char *fmt, ...);
 * >
 * > void debug_tval(const tval *val);
 * >
 * > int intpair_sum(intpair_t *pair, int is_debug)
 * > {
 * >   int sum;
 * >
 * >   sum = pair->a + pair->b;
 * >
 * >   if (is_debug)
 * >   {
 * >     debug_print
 * >       ( "intpair_sum:\n"
 * >         "  %d + %d => %d\n"
 * >       , pair->a
 * >       , pair->b
 * >       , sum
 * >       );
 * >
 * >     debug_tval(pair);
 * >   }
 * >
 * >   return sum;
 * > }
 * >
 * > void debug_tval(const tval *val)
 * > {
 * >   const type_t *type = (const type_t *) val;
 * >
 * >   size_t      type_size;
 * >   const char *type_name;
 * >
 * >   type_size = type->size(type);
 * >   type_name = type->name(type);
 * >
 * >   debug_print
 * >     ( "debug_tval:\n"
 * >       "  type name: %s\n"
 * >       "  type size: %d\n"
 * >     , (const char *) type_name
 * >     , (size_t)       type_size
 * >     );
 * > }
 * >
 * > static intpair_t my_intpair =
 * >   { intpair_type
 * >
 * >   , 7
 * >   , 8
 * >   };
 * >
 * > static tval_t *my_intpair_val  = &my_intpair;
 * > static type_t *my_intpair_type = (type_t *) my_intpair_val;
 *
 * ----------------------------------------------------------------
 *
 * A "tval" is a value of a struct whose first field is "type_t *", a reference
 * to the type .
 *
 * --
 *
 * Given:
 *
 * > tval *foo
 *
 * The type "tval" is intended to denote that the type used for "foo" is, or is
 * equivalent to, a struct whose first field has type "type_t *".
 *
 * This allows users to obtain the type of "foo", by dereferencing the first
 * field of "foo":
 *
 * > type_t *type_of_foo;
 * >
 * > type_of_foo = *((type_t **) foo);
 *
 * "type_t *" can also indirectly be the first field: a valid alternative is a
 * concrete struct, but not a pointer to a concrete struct, whose first field
 * has type "type_t *".
 */
typedef void tval;

const type_t *tval_type(const tval *val);

/* TODO: add new type like "tval" but will also work for "untyped" values, and
 * then use this inside "type_t"'s methods.
 */

/* ---------------------------------------------------------------- */
/* Memory managers.                                                 */
/* ---------------------------------------------------------------- */

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
void *memory_manager_realloc(const memory_manager_t *memory_manager, void   *ptr,   size_t size);

void  memory_manager_on_oom (const memory_manager_t *memory_manager, size_t      size);
void  memory_manager_on_err (const memory_manager_t *memory_manager, const char *msg);


/* TODO: Remove "buffers"; just use "allocations". */
const type_t *memory_tracker_type(void);
extern const type_t memory_tracker_type_def;
typedef struct memory_tracker_s memory_tracker_t;
struct memory_tracker_s
{
  typed_t type;

  memory_manager_t memory_manager;


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
/* struct_info_t, field_info_t, and ref_traversal_t                 */
/* ---------------------------------------------------------------- */

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

ref_traversal_t *ref_traversal_init_empty(ref_traversal_t *dest);
ref_traversal_t *ref_traversal_init_with_one(ref_traversal_t *dest, void *reference);

/* Add the reference to "ref_traversal".  If it already exists, return NULL;
 * else return the reference.
 */
void *ref_traversal_add   (      ref_traversal_t *ref_traversal, void *reference);

/* Remove the reference from "ref_traversal".  If it exists and is removed,
 * return NULL; else return the reference.
 */
void *ref_traversal_remove(      ref_traversal_t *ref_traversal, void *reference);

/* Remove the reference from "ref_traversal".  If it exists and is removed,
 * return the reference; else return NULL.
 */
void *ref_traversal_take  (      ref_traversal_t *ref_traversal, void *reference);

/* A NULL "ref_traversal" is treated as an empty "ref_traversal_t". */
void *ref_traversal_exists(const ref_traversal_t *ref_traversal, void *reference);

/* TODO */

/* ---------------------------------------------------------------- */

/* TODO */

/* TODO: it's unclear which arguments refer to base structures from which the
 * field is derived and which refer to the fields themselves!
 *
 * TODO: if possible, parameters should consistently be base structures?
 */

/* TODO: update field_info_type's struct_info */
const type_t *field_info_type(void);
extern const type_t field_info_type_def;
typedef struct field_info_s field_info_t;
struct field_info_s
{
  typed_t type;

  ptrdiff_t     field_pos;
  size_t        field_size;
  const type_t *field_type;

  /* Optional information. */

  /* Whether this field is specific to each value.       */
  /*                                                     */
  /* Should "dup" skip this value when duplicating or    */
  /* copying this field unless otherwise requested?      */
  /*                                                     */
  /* This is appropriate for fields that should be       */
  /* unique to each value, like a value's memory         */
  /* tracker.                                            */
  int           is_metadata;

  /* Is this field a reference that "dup" should         */
  /* recursively copy with the field's type's "dup"      */
  /* function when copying is requested?                 */
  int           is_copyable_ref;

  /* Write the default value for this field.             */
  /*                                                     */
  /* NULL values default to writing 0's in               */
  /* memory.                                             */
  size_t      (*default_value)        (const field_info_t *self, void *dest_field_mem);

  /* Write the value, usually 0 or NULL, that indicates  */
  /* that "template_cons_t"-based initializers should    */
  /* compare against to determine whether to assign the  */
  /* value from the source value or the default value.   */
  /*                                                     */
  /* This should return 0 when the field is relevant for */
  /* all values.                                         */
  /*                                                     */
  /* A NULL reference defaults to comparing the memory   */
  /* against 0's.                                        */
  size_t      (*template_unused_value)(const field_info_t *self, void *dest_field_mem);
};

#define FIELD_INFO_DEFAULTS                                \
  { field_info_type                                        \
                                                           \
  , /* field_pos             */ (ptrdiff_t) 0              \
  , /* field_size            */ (size_t)    0              \
  , /* field_type            */ NULL                       \
                                                           \
  , /* is_metadata           */ 0                          \
  , /* is_copyable_ref       */ 0                          \
                                                           \
  , /* default_value         */ default_value_zero         \
  , /* template_unused_value */ template_unused_value_zero \
  }
extern const field_info_t field_info_defaults;

/*
 * Common "default_value" abstractions.
 */

/* TODO: Wait, why can't we use the field_infos themselves for sub-types? */
/*
 * Standard use case for default field values:
 *
 * Use the "type" to determine whether it has a default_value ("has_default"),
 * and, if so, reference this to assign a default value for each field, equal
 * to the value of the type's default value.
 *
 * Otherwise, it has no default value, so first use "type" to determine whether the
 * struct's first field is a "typed_t" ("type_typed"), and if so, assign the
 * first field a default of "type", and then second recursively apply the same
 * algorithm for each field, obtaining a field's default value from its type.
 *
 * In the latter case, for rare mutually recursive types, leading to a virtual loop
 * in a recursion representation, a default value of 0/NULL is chosen nearest
 * the beginning of such a loop.  (These cases are handled correctly; infinite
 * loops are avoided by tracking visited defaults through "ref", which is
 * usually NULL when called at the root level.)
 *
 * TODO: update:
 * Thus:
 *   type_has_default(type):
 *     yes:  default_value_from_struct (self, dest_mem, type_has_default(type))
 *     NULL: default_value_type_or_zero(self, dest_mem, type)
 *
 * Example:
 *
 * > const tval *mystruct_type(void);
 * > typedef struct mystruct_s mystruct_t;
 * > struct mystruct_s
 * > {
 * >   typed_t type;
 * >
 * >   double size;
 * >   int a;
 * >   int b;
 * > };
 * >
 * > const mystruct_t mystruct_defaults =
 * > { mystruct_type
 * >
 * > , 1.0
 * > , 0.0
 * > , 0.0
 * > }
 * >
 * > /-* ... *-/
 * >
 * > static size_t mystruct_type_is_struct_default_value(const field_info_t *self, void *dest_field_mem)
 * > {
 * >   return default_value_from_struct(self, dest_field_mem, mystruct_type, NULL);
 * > }
 */
size_t default_value_from_type(const field_info_t *self, void *dest_mem, typed_t type, ref_traversal_t *ref);

/*
 * Each field has a default value of 0 / NULL.
 *
 * This is the default for field_info's that lack a provided "default_value"
 * value (i.e. it is NULL).  (However, most default procedures that *generate*
 * field_info's or struct_info's choose a more reasonable default for
 * field_info, where there is usually more context available, e.g. a "type_t".)
 *
 * (Not recommended for general use, because "typed_t" is recommended to
 * default to the type of the struct.)
 *
 * (An alternative function that handles the "typed" field is
 * "default_value_type_or_zero".)
 *
 * (However, a struct's "default_value" function might use this for all fields
 * except for the "typed_t type" field.)
 *
 * (For example:)
 *
 * > const tval *mystruct_type(void);
 * > typedef struct mystruct_s mystruct_t;
 * > struct mystruct_s
 * > {
 * >   typed_t type;
 * >
 * >   double size;
 * >   int a;
 * >   int b;
 * > };
 * >
 * > /-* ... *-/
 * >
 * > static size_t mystruct_type_is_struct_default_value(const field_info_t *self, void *dest_field_mem)
 * > {
 * >   static const mystruct_t empty;
 * > 
 * >   /-* typed_t type                                                *-/
 * >   if      (field_info_cref(self, &empty) == &empty.type)
 * >   {
 * >     typedef typed_t field_type;
 * > 
 * >     field_type *dest_field = (field_type *) dest_field_mem;
 * > 
 * >     if (dest_field) *dest_field =
 * >       memory_manager_type;
 * > 
 * >     return sizeof(field_type);
 * >   }
 * > 
 * >   /-* General case for all other fields, and unrecognized fields. *-/
 * >   else
 * >   {
 * >     return default_value_zero(self, dest_field_mem);
 * >   }
 * > }
 *
 * The above example is abstracted with "default_value_type_or_zero".
 */
size_t default_value_zero(const field_info_t *self, void *dest_mem);

/*
 * Each field has a default value equal to the field values in "defaults".
 *
 * Example:
 *
 * > const tval *mystruct_type(void);
 * > typedef struct mystruct_s mystruct_t;
 * > struct mystruct_s
 * > {
 * >   typed_t type;
 * >
 * >   double size;
 * >   int a;
 * >   int b;
 * > };
 * >
 * > const mystruct_t mystruct_defaults =
 * > { mystruct_type
 * >
 * > , 1.0
 * > , 0.0
 * > , 0.0
 * > }
 * >
 * > /-* ... *-/
 * >
 * > static size_t mystruct_type_is_struct_default_value(const field_info_t *self, void *dest_field_mem)
 * > {
 * >   return default_value_from_struct(self, dest_field_mem, &mystruct_defaults);
 * > }
 *
 * If an associated "type_t" is available, it may be more idiomatic to use
 * "default_value_from_type_defaults".
 *
 * If "defaults" is NULL, then this behaves like "default_value_zero".
 */
size_t default_value_from_struct(const field_info_t *self, void *dest_field_mem, const void *defaults);
/*
 * TODO: also implement this?
 * Return a function pointer suitable for "default_value" that returns the
 * field from the struct.
 *
 * All fields have default values equal to the input struct's field values.
 *
 * If the struct has a value containing default values at each field, then
 * "default_value" can be assigned to this.
 *
 * TODO document
 */
/*
size_t (*
  default_value_from_struct(const void *defaults)
  )(const field_info_t *self, void *dest_field_mem);
*/

/*
 * Each field has a default value of the field's type's default value if it has
 * one, otherwise 0 / NULL.
 *
 * (Not recommended for general use, because "typed_t" is recommended to
 * default to the type of the struct, not to the default type of "type_type"!
 * ("type_type" is the type of "type_t", the type of type representation
 * structs.)
 *
 * (An alternative function that handles the "typed" field is
 * "default_value_type_or_fieldwise_defaults".)
 *
 * Like "default_value_type_or_zero", except for all fields, other than the
 * first if the struct is "typed" (i.e. the first field is "typed_t"), first
 * use the field's "type" to check whether it has a default value, and if so,
 * use it, otherwise assign 0/NULL for this field.
 *
 * Thus, for all fields excepting the "typed" field if it exists:
 *   type_has_default(type):
 *     yes:  default_value_from_struct (self, dest_mem, type_has_default(type))
 *     NULL: default_value_type_or_zero(self, dest_mem, type)
 */
size_t default_value_fieldwise_defaults(const field_info_t *self, void *dest_mem, typed_t typed_field_default);

/*
 * Like "default_value_zero", each field has a default value of 0, except
 * assign a default type for the type field if "typed_field_default" reports
 * that values are "tval *"s, i.e., in the case of a struct, the struct's first
 * field is "typed_t"; otherwise behave like "default_value_zero".
 *
 * Example:
 *
 * > const tval *mystruct_type(void);
 * > typedef struct mystruct_s mystruct_t;
 * > struct mystruct_s
 * > {
 * >   typed_t type;
 * >
 * >   double size;
 * >   int a;
 * >   int b;
 * > };
 * >
 * > /-* ... *-/
 * >
 * > static size_t mystruct_type_is_struct_default_value(const field_info_t *self, void *dest_field_mem)
 * > {
 * >   return default_value_type_or_zero(self, dest_field_mem, mystruct_type);
 * > }
 *
 * The field is effectively checked whether it is equal to
 * "&type_is_struct(mystruct_type)->fields[0]" if "type_field_default" is
 * typed.
 */
size_t default_value_type_or_zero(const field_info_t *self, void *dest_mem, typed_t typed_field_default);

/*
 * Use the "type" to determine whether it has a default_value ("has_default"),
 * and, if so, use this to assign a default value for each field.
 *
 * Otherwise, it has no default value, so first use "type" to determine whether the
 * struct's first field is a "typed_t" ("type_typed"), and if so, assign the
 * first field a default of "type", and then second assign a default "zero"
 * value for all other fields.
 *
 * TODO: update:
 * Thus:
 *   type_has_default(type):
 *     yes:  default_value_from_struct (self, dest_mem, type_has_default(type))
 *     NULL: default_value_type_or_zero(self, dest_mem, type)
 *
 * If "type" is NULL, this behaves like "default_value_zero".
 */
size_t default_value_from_type_defaults(const field_info_t *self, void *dest_mem, typed_t type);

/*
 * Like "default_value_type_or_zero", except for all fields, other than the
 * first if the struct is "typed" (i.e. the first field is "typed_t"), first
 * use the field's "type" to check whether it has a default value, and if so,
 * use it, otherwise assign 0/NULL for this field.
 *
 * TODO: update:
 * Thus, for all fields excepting the "typed" field if it exists:
 *   type_has_default(type):
 *     yes:  default_value_from_struct (self, dest_mem, type_has_default(type))
 *     NULL: default_value_type_or_zero(self, dest_mem, type)
 */
size_t default_value_type_or_fieldwise_defaults(const field_info_t *self, void *dest_mem, typed_t typed_field_default);









size_t template_unused_value_zero(const field_info_t *self, void *dest_mem);

size_t template_no_unused_value(const field_info_t *self, void *dest_mem);

int field_info_cmp(const field_info_t *a, const field_info_t *b);

int is_field_terminator(const field_info_t *field_info);

void       *field_info_ref (const field_info_t *field_info,  void       *base);
const void *field_info_cref(const field_info_t *field_info,  const void *base);

#define FIELD_MEMCMP_ERR_NULL_S1 ((int) -0x37ED)
#define FIELD_MEMCMP_ERR_NULL_S2 ((int) -0x37EE)
/*
 * Directly compare the memory represents two values in a field.
 *
 * Returns:
 *   = 0: Values are equal.
 *   > 0: First byte in "s1" is greater than first byte in "s2".
 *   < 0: First byte in "s1" is less    than first byte in "s2".
 */
int field_memcmp(const field_info_t *field_info, const void *s1, const void *s2);

void *field_memcpy(const field_info_t *field_info, void *dest, const void *src);
void *field_memmove(const field_info_t *field_info, void *dest, const void *src);

/*
 * Determine whether the source field value is equal to the field's "special
 * unused value" if it has one.
 *
 * If source is equal to the special unused value, then write the field's
 * default value to "out_default_mem" if "out_default_mem" is provided.
 *
 * Returns:
 *       0: Source is not equal to the field's special unused value,
 *          or the field lacks a special unused value.
 *   >=  1: Source is equal to the field's special unused value.
 *   <= -1: An error occurred:
 *          -1: Failed to allocate memory.  The "malloc" of the memory manager,
 *              or the default one if NULL, returned NULL.
 *
 *              If "working_mem" is provided, no memory allocation is
 *              performed.
 *          -2: "src_mem" is NULL.
 *          ...
 *
 * "working_mem" can overlap with "dest" if its original contents aren't
 * needed.  If they overlap, then writing a default value to "dest" will still
 * succeed (returns >= 1).  But if no default value is written (returns 0) then
 * "dest" could end up with unspecified data.
 */
int is_field_template_unused(const field_info_t *field_info, const void *src_field_mem, void *field_working_mem, const memory_manager_t *memory_manager, void *out_field_default_mem);

enum verify_field_info_status_e
{
  /* The checks that were requested to run passed.                */
  verify_field_info_success         = 0,

  /* "verify_field" was called with a NULL "field_info" argument. */
  verify_field_info_null_field_info = 1,

  /* The "field_info"'s "type" field value is NULL.               */
  verify_field_info_no_type         = 2,

  verify_field_info_size_mismatch   = 3,

  verify_field_info_end,

  /* 1: 0-1 */
  /* 2: 0-3 */
  /* 3: 0-7 */
  verify_field_info_bits     = 2,
  verify_field_info_end_mask = ONE_BIT_REPEAT(verify_field_info_bits)
};
typedef enum verify_field_info_status_e verify_field_info_status_t;

verify_field_info_status_t verify_field_info(const field_info_t *field_info, char *out_err, size_t err_size);

const char *field_dup(const field_info_t *field_info, void *dest, const void *src, int force_no_default, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal);

/* ---------------------------------------------------------------- */

/* TODO */

#define STRUCT_INFO_NUM_FIELDS 1024
const type_t *struct_info_type(void);
extern const type_t struct_info_type_def;
typedef struct struct_info_s struct_info_t;
struct struct_info_s
{
  typed_t type;

  /* Must be terminated by "field_terminator". */
  field_info_t fields[STRUCT_INFO_NUM_FIELDS];
  /* "fields_len" doesn't include the tail. */
  size_t       fields_len;

  struct_info_t *tail;


  /* Optional information, for this chunk (excludes tail). */

  int       has_memory_tracker;

  /* Index into "fields". */
  size_t    memory_tracker_field;
};

#define STRUCT_INFO_DEFAULTS  \
  { struct_info_type          \
                              \
  , { FIELD_INFO_DEFAULTS }   \
  , 0                         \
                              \
  , NULL                      \
                              \
  , 0                         \
  , 0                         \
  }
extern const struct_info_t struct_info_defaults;

/* For redundantly verifying "fields_len" is correct. */
extern const field_info_t * const field_terminator;

extern const field_info_t terminating_field_info;

/* ---------------------------------------------------------------- */

struct_info_t *struct_info_init
  ( struct_info_t *struct_info
  , size_t      (*default_value)        (const field_info_t *self, void *dest_field_mem)
  , size_t      (*template_unused_value)(const field_info_t *self, void *dest_field_mem)
  );

struct_info_t *struct_info_add_field_info(struct_info_t *struct_info, const field_info_t *field_info);

#define OFFSET_FIELD(type, field) offsetof(type, field)
#define SIZEOF_FIELD(type, field) sizeof(((const type *) NULL)->field)

struct_info_t *struct_info_add_field
  ( struct_info_t *struct_info
  , ptrdiff_t      field_pos
  , size_t         field_size
  , const type_t  *field_type
  );

struct_info_t *struct_info_add_field_terminator(struct_info_t *struct_info);

field_info_t *struct_info_get_last_field_elem_ref(struct_info_t *struct_info);

/* ---------------------------------------------------------------- */

void *struct_info_iterate_chunks
  ( const struct_info_t *struct_info
  , void *(*with_chunk)(void *context, void *last_accumulation, const struct_info_t *struct_info_chunk, int *break_iteration)
  , void *context
  , void *initial_accumulation
  );

void *struct_info_iterate_fields
  ( const struct_info_t *struct_info
  , void *(*with_field)(void *context, void *last_accumulation, const field_info_t *field_info, int *break_iteration)
  , void *context
  , void *initial_accumulation
  );

void *struct_info_iterate_chunks_mutable
  ( struct_info_t *struct_info
  , void *(*with_chunk)(void *context, void *last_accumulation, struct_info_t *struct_info_chunk, int *break_iteration)
  , void *context
  , void *initial_accumulation
  );

void *struct_info_iterate_fields_mutable
  ( struct_info_t *struct_info
  , void *(*with_field)(void *context, void *last_accumulation, field_info_t *field_info, int *break_iteration)
  , void *context
  , void *initial_accumulation
  );

/* Returns NULL when "index" is greater than        */
/* the number of fields the "struct_info" contains. */
size_t              struct_info_num_fields         (const struct_info_t *struct_info);
size_t              struct_info_num_tails          (const struct_info_t *struct_info);
const field_info_t *struct_info_index_field        (const struct_info_t *struct_info, size_t index);
field_info_t       *struct_info_index_field_mutable(      struct_info_t *struct_info, size_t index);

#define STRUCT_INFO_TYPED_FIELD      (0)
#define STRUCT_INFO_TYPED_FIELD_TYPE (typed_type())
const field_info_t *struct_info_has_typed_field   (const struct_info_t *struct_info);
const field_info_t *struct_info_has_memory_tracker(const struct_info_t *struct_info);

/*
 * If the "struct_info" associates a memory tracker field, then given a value
 * of that struct, return it.  Otherwise, return NULL.
 *
 * With no value, return NULL.
 */
typed_t           struct_value_has_typed_field   (const struct_info_t *struct_info, const void *val);
memory_tracker_t *struct_value_has_memory_tracker(const struct_info_t *struct_info, void       *val);

enum verify_struct_info_status_e
{
  /* The checks that were requested to run passed.                       */
  verify_struct_info_success          = 0,

  /* "verify_struct_info" was called with a NULL "struct_info" argument. */
  verify_struct_info_null_struct_info = 1,

  /* fields_len is too big; excess fields need to be moved to the tail.  */
  verify_struct_info_need_tail_excess = 2,

  /* fields_len is at max capacity with no field terminator.             */
  /* At least one field needs to be moved to the tail.                   */
  verify_struct_info_need_tail_max    = 3,

  /* A call to "is_field_terminator" failed with a general error code.   */
  verify_struct_info_is_field_terminator_error
                                      = 4,
  /* Invalid field: a call to "verify_field_info" failed.                */
  /*                                                                     */
  /* The struct_info_field_error's bits are OR'd onto this beyond        */
  /* verify_struct_info_bits.                                            */
  verify_struct_info_verify_field_info_error       
                                      = 5,

  /* A field's field_pos is equal or lesser than that of its previous.   */
  verify_struct_info_invalid_field_pos
                                      = 6,

  verify_struct_info_end,

  /* 1: 0- 1 */
  /* 2: 0- 3 */
  /* 3: 0- 7 */
  /* 4: 0-15 */
  verify_struct_info_bits     = 3,
  verify_struct_info_end_mask = ONE_BIT_REPEAT(verify_struct_info_bits)
};
typedef enum verify_struct_info_status_e verify_struct_info_status_t;

/* TODO: verify_struct_info: check for duplicates! */

#define DEFAULT_BUF_SIZE_VERIFY_STRUCT_INFO 4096
/*
 * Returns 0 on success.
 */
verify_struct_info_status_t verify_struct_info_chunk(const struct_info_t *struct_info, char *out_err, size_t err_size);
verify_struct_info_status_t verify_struct_info(const struct_info_t *struct_info, char *out_err, size_t err_size);

/*
 * struct_dup:
 *
 * Assign each field in "dest" the value of the field in "src" according to
 * "struct_info".
 *
 * Behaviour for default values:
 *   If "struct_info" describes how to check whether a field in "src"
 *   indicates a default value should be chosen, and if so, what the default
 *   value is, it will be used.
 *
 *   Normally "struct_info" does this by supplying a default value for "src"
 *   field values equal to "0" or NULL.
 *
 * rec_copy:
 *   Whether to recursively duplicate subfields marked in "struct_info" for
 *   recursive traversal.
 *
 *   Set to a negative number to limit traversal depth.
 *
 * dup_metadata:
 *   Whether to not skip assignment of fields specially marked as "metadata",
 *   which should be unique to each value.
 *
 *   This is used for fields such as memory trackers.
 *
 * Returns NULL on success, and an error message on failure.
 */
const char *struct_dup(const struct_info_t *struct_info, void *dest, const void *src, int force_no_default, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal);

/* ---------------------------------------------------------------- */
/* type_t                                                           */
/* ---------------------------------------------------------------- */

/*
 * type_t: Type representations.
 *
 * A "type_t" describes a kind for values, possibly with information about its
 * name, that about its size, procedures to initialize values and allocate
 * memory for values of this type, and optionally, if the type is a struct,
 * information about the struct's fields.
 */
const type_t *type_type(void);
extern const type_t type_type_def;
/* Forward declaration: typedef struct type_s type_t; */
struct type_s
{
  /* Make "type_t" itself a type by setting the first field as "type_t *".  */
  /* All initialized "type_t" values should have "type" set to "type_type". */
  typed_t type;

  /* ---------------------------------------------------------------- */

  memory_tracker_t memory;

  /* ---------------------------------------------------------------- */
  /* Basic type info.                                                 */
  /* ---------------------------------------------------------------- */

  /* Whether values of this type are "tval"s.                         */
  /*                                                                  */
  /* "self":                                                          */
  /*   Normally they are; in this case "id" should return "self".     */
  /*                                                                  */
  /* "NULL":                                                          */
  /*   Sometimes, however, e.g. in the case of "type_t" for C's       */
  /*   primitive data types, value pointers lack polymorphism with    */
  /*   "type_t *".  In this case, "id" should return NULL.            */
  const type_t        *(*typed)      (const type_t *self);

  /* Name of the type.  (Not necessarily unique.)                     */
  const char          *(*name)       (const type_t *self);

  /* TODO: update field! */
  /* General information about the type.                              */
  /*                                                                  */
  /* Both the result of "info" and "info" itself may be NULL.         */
  /*                                                                  */
  /* This procedure can depend upon "out_info", returning NULL when   */
  /* it isn't provided.                                               */
  const char          *(*info)       ( const type_t *self
                                     , char         *out_info_buf
                                     , size_t        info_buf_size
                                     );

  /* Size of a value of this type.                                    */
  /*                                                                  */
  /* Types can be variable-width or constant-width:                   */
  /*                                                                  */
  /*   Variable-width types should return "0" when "val" is NULL.     */
  /*                                                                  */
  /*   Constant-width types should return the size of any value when  */
  /*   "val" is NULL.                                                 */
  size_t               (*size)       (const type_t *self, const tval *val);

  /* If this type describes a struct, return information about its    */
  /* fields; otherwise return NULL.                                   */
  const struct_info_t *(*is_struct)  (const type_t *self);


  /* ---------------------------------------------------------------- */
  /* Initializers.                                                    */
  /* ---------------------------------------------------------------- */

  /*
   * Initializers:
   * -------------
   *
   * A type contains methods that initialize memory allocated for a value of
   * the represented type.
   *
   * How "init" initializes a value depends on the so-called "constructor",
   * passed as the "cons" parameter.
   *
   *
   * Constructors:
   * -------------
   *
   * In the most common case, "cons" contains:
   *   1) Whether to "malloc" a new value, or initialize an already-allocated
   *     "tval *" (e.g. it's on the stack, or it was "malloc" externally.)
   *
   *      If "init" calls "malloc" on the new value, then "free" should
   *      likewise free this memory.
   *
   *   2) Initial values for some fields.
   *
   * Example:
   *
   * > extern type_t intpair_type;
   * >
   * > typedef struct intpair_s intpair_t;
   * > struct intpair_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Whether this pair exists on the heap. *-/
   * >   /-* intpair_type's "free" checks this.    *-/
   * >   int is_malloc : 1;
   * >
   * >   int a;
   * >   int b;
   * > };
   *
   * > typedef struct intpair_cons_s intpair_cons_t;
   * > struct intpair_cons_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Set to NULL to "malloc" a new one. *-/
   * >   intpair_t *val;
   * >
   * >   /-* Initial values for "a" and "b".    *-/
   * >   int     a;
   * >   int     b;
   * > };
   *
   * Constructor convention #1: ad-hoc initial values
   * ------------------------------------------------
   *
   * Here we establish a convention for the contents of "constructor"
   * structures:
   *   1) > type_t *type
   *
   *      for the associated type.
   *
   *   2) > <type *>val
   *
   *      Or
   *
   *      > <type *>val_non_null
   *
   *      If this is non-NULL, initializers should assume the memory is already
   *      being managed indepentently and should initialize "val".
   *
   *      Sometimes developers don't want to track whether values have been
   *      dynamically allocated.  If initializers don't support dynamic
   *      allocation, this value should be non-NULL.
   *
   *      If dynamic memory allocation is supported, this value may be NULL.
   *
   *   3) If the initializer supports dynamic memory allocation:
   *
   *      Whether and how to "malloc" and "free" a new value.  In the simplest
   *      case, this can be a pointer used as described in the above example.
   *
   *      However, we recommend for non-trivial cases also including a more
   *      versatile pair of function pointers:
   *
   *      > <type *> (*malloc)(size_t  size);
   *      > void     (*free)  (type   *val);
   *
   *      More complicated constructers might instead include 2 function
   *      pointers for allocating
   *
   *   3) Fields to calculate essential initial values.
   *
   *      Values of 0 or NULL should indicate that a reasonable default should
   *      be chosen.
   *
   *      If it's important to allow convenient non-default initial 0 or NULL
   *      values, additional fields can be added for this; for example:
   *
   *      > typedef struct foo_cons_s foo_cons_t
   *      > struct foo_cons_s
   *      > {
   *      >   type_t *type;
   *      >
   *      >   /-* Set to NULL to "malloc" a new one.             *-/
   *      >   intpair_t *val;
   *      >
   *      >   /-* Set to NULL to default to "malloc" and "free". *-/
   *      >   intpair_t (*malloc)(size_t     size);
   *      >   void      (*free)  (intpair_t *val);
   *      >
   *      >   double     size;
   *      >   double    *pos;  int force_pos;
   *      >   double    *vel;  int force_vel;
   *      > };
   *
   *      (Here we assume a "size" of "0.0" represents a default "size" value,
   *      perhaps because a "size" of "0.0" doesn't make sense.)
   *
   *      Note: it is often unnecessary to include a copy of *every* field,
   *      because in relevant use cases, values could simply be directly
   *      constructed by hand, or copied from another value.
   *
   *   4) Any ad-hoc extra fields.
   *
   * Example:
   *
   * > extern type_t intpair_type;
   * >
   * > typedef struct intpair_s intpair_t;
   * > struct intpair_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Can be NULL (or a no-op) if this pair was not dynamically *-/
   * >   /-* allocated.                                                *-/
   * >   void (*free)(intpair_t *val);
   * >
   * >   int a;
   * >   int b;
   * > };
   *
   * > typedef struct intpair_cons_s intpair_cons_t;
   * > struct intpair_cons_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Set to NULL to "malloc" a new one.             *-/
   * >   intpair_t *val;
   * >
   * >   /-* Set to NULL to default to "malloc" and "free". *-/
   * >   intpair_t (*malloc)(size_t     size);
   * >   void      (*free)  (intpair_t *val);
   * >
   * >   /-* Initial values for "a" and "b".                *-/
   * >   int a;
   * >   int b;
   * > };
   *
   * #2: Same-type constructors:
   * ---------------------------
   *
   * For some types, it is simpler to set the constructor type as the same type
   * as the type itself.
   *
   * In this case, the conventional behaviour is this:
   *
   *   The constructor itself is initialized.
   *
   *   No dynamic memory allocation is done, e.g. with "malloc".
   *
   *   All "0" and "NULL" fields are interpreted to represent the default
   *   values.
   *
   * The constructor type of constructor types themselves are usually the same.
   *
   * #3: Template constructurors:
   * ----------------------------
   *
   * This convention for constructor types affords the simplicity of the 2nd
   * type described above along with the functionality of the 1st.
   *
   * Rather than providing ad-hoc initial values, a pointer to the struct is
   * provided instead, where each "0" and "NULL" field indicates that an
   * uninitialized field, and that a default or newly initialized one should be
   * assigned.
   *
   * Example:
   *
   * > typedef struct intpair_cons_s intpair_cons_t;
   * > struct intpair_cons_s
   * > {
   * >   type_t *type;
   * >
   * >   /-* Set to NULL to "malloc" a new one.             *-/
   * >   intpair_t *val;
   * >
   * >   /-* Set to NULL to default to "malloc" and "free". *-/
   * >   intpair_t (*malloc)(size_t     size);
   * >   void      (*free)  (intpair_t *val);
   * >
   * >   intpair_t *initials;
   * >   intpair_t *force_non_defaults;
   * > };
   *
   * A second reference, "force_non_defaults" is also included.  This can be
   * NULL.
   *
   * A non-default, i.e. non-zero/non-NULL field, value is unconditionally
   * respected.
   *
   * For all default values in "initials", if force_non_defaults is not NULL,
   * its value is checked.  If the value is any non-zero/non-NULL value, the
   * the value in "initials" forced to be its 0/NULL value.
   *
   * Several general template constructor types are provided below, but some
   * types might want to define their own variant.
   */

  /* The type of the "cons" constructor parameter for "init".         */
  /*                                                                  */
  /* "init" must be called with a "cons" value of this type.          */
  typed_t              (*cons_type)  (const type_t *self);

  /* Initialize a value of this type.                                 */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* Initializers might support both initializing an already          */
  /* allocated value, and also allocated new memory with dynamically  */
  /* allocated.                                                       */
  /*                                                                  */
  /* Which forms of initialization are support are reflected in the   */
  /* type of "cons", the constructor.                                 */
  /*                                                                  */
  /* Any allocation performed by "init" should be properly freed by   */
  /* "free".  If a new "tval *" is allocated with "malloc", for       */
  /* example, then "free" should free this memory.                    */
  /*                                                                  */
  /* Some initializers support NULL constructor arguments,            */
  /* generally indicating all default values.                         */
  /*                                                                  */
  /* Generally, multiple calls to "init" results in undefined         */
  /* behaviour.                                                       */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* Note: the type of "cons" is not necessarily the same as this     */
  /* type.                                                            */
  /*                                                                  */
  /* Each type defines its own constructor ("cons") type, which       */
  /* may be different.                                                */
  /*                                                                  */
  /* A common constructor type is "template_cons_t".                  */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* Returns NULL on failure to initialize.                           */
  tval                *(*init)       (const type_t *self, tval *cons);

  /* Deinitialization a value, freeing resources allocated by "init". */
  /*                                                                  */
  /* Support for idempotence (multiple calls to "free") is            */
  /* recommended but not required.                                    */
  void                 (*free)       (const type_t *self, tval *val);

  /* A type can be associated with a designated default value,        */
  /* returned by this method.                                         */
  /*                                                                  */
  /* A type that doesn't should return NULL.                          */
  const tval          *(*has_default)(const type_t *self);

  /* ---------------------------------------------------------------- */
  /* Dynamic memory allocation.                                       */
  /* ---------------------------------------------------------------- */

  /* How to track memory allocation for values of this type.          */
  /*                                                                  */
  /* "mem" should return a reference to the memory tracker associated */
  /* with "val_raw" if it exists, otherwise NULL.                     */
  /*                                                                  */
  /* "val" might not be initialized, in which case the memory tracker */
  /* returned probably isn't either.                                  */
  /*                                                                  */
  /* "mem" should properly handle uninitialized "val_raw" values, but */
  /* it isn't required to return one for a "val" that isn't           */
  /* initialized (perhaps, for instance, a type keeps track of        */
  /* which values of the type are initialized externally from any     */
  /* individual value, which allows the type to check whether         */
  /* a reference to any given value is one present in some container  */
  /* of initialized values of this type.                              */
  /*                                                                  */
  /* In the simplest case, the type has a "memory_tracker_t"          */
  /* reference that this method initializes and returns.              */
  /*                                                                  */
  /* Alternatively, some types might want to reference a memory       */
  /* tracker outside values.                                          */
  /*                                                                  */
  /* This can be NULL, or can always return NULL, if this type        */
  /* isn't associated with a memory tracker.                          */
  /*                                                                  */
  /* A type itself can be associated with its own unique memory       */
  /* tracker, which it returns it when "val" is NULL; but usually     */
  /* this is not done (and just NULL is returned in this case.)       */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* One of the primary uses of this is to relocate information about */
  /* whether value of a given type was dynamically allocated.         */
  /*                                                                  */
  /* "free" determines whether it should free a dynamically allocated */
  /* value.  In a simple case, the value itself contains a field      */
  /* indicating whether the value was dynamically allocated, e.g.     */
  /* with "malloc".                                                   */
  /*                                                                  */
  /* But "mem" allows such tracking to have data that resides outside */
  /* values.  In this case, "mem" returns the appropriate memory      */
  /* tracker.                                                         */
  /*                                                                  */
  /* A simple way to do this is to set "mem" to return a top-level    */
  /* memory-tracker associated to the type itself.                    */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* If this returns a memory tracker with a NULL "val_raw", then     */
  /* conventional initializers, including                             */
  /* "template_cons_basic_initializer", will not initialize a new     */
  /* memory tracker.                                                  */
  memory_tracker_t    *(*mem)        (const type_t *self, tval *val_raw);

  /* TODO: update field_info! */
  /* TODO: also does_type_support_dyn_alloc_all_values? */
  /* How the memory trackers associated with new values should be     */
  /* initialized during the initialization of values.                 */
  /*                                                                  */
  /* This should return NULL when dynamicaly memory allocation        */
  /* is not supported, for all values in general when "val_raw" is    */
  /* NULL, and for particular values otherwise.                       */
  /*                                                                  */
  /* Otherwise its return value is type-specific.  In simple cases,   */
  /* this return value can simply be the associated memory tracker,   */
  /* but it can also be any other non-NULL reference.                 */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* Thus mainly this is used by standard initializers for 2          */
  /* purposes:                                                        */
  /*   1) To track newly dynamically allocated values.                */
  /*   2) To determine whether such dynamic allocation is supported   */
  /*      for all values of this type in general,                     */
  /*      where "val_raw" is NULL.                                    */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* This can be NULL                                                 */
  void                *(*mem_init)   ( const type_t *self
                                     , tval         *val_raw
                                     , int           is_dynamically_allocated
                                     );

  /* TODO: update field_info! */
  /* Check whether a value is dynamically allocated.                  */
  /*                                                                  */
  /* Returns 1 if so, 0 otherwise, and <= -1 on error.                */
  int                  (*mem_is_dyn) ( const type_t *self
                                     , tval         *val
                                     );

  /* TODO: update field_info! */
  /* Check whether a value is dynamically allocated and, if so, free  */
  /* it, otherwise do nothing.                                        */
  /*                                                                  */
  /* Returns 1 if freed, 0 if not dynamically allocated,              */
  /* and <= -1 on error.                                              */
  int                  (*mem_free)   ( const type_t *self
                                     , tval         *val
                                     );

  /* TODO: update field_info! */
  /* Default memory manager for this type.                            */
  /*                                                                  */
  /* 1) Which memory manager default should be chosen for new values  */
  /*    when no memory manager is explicitly requested; "val_raw" is  */
  /*    null in this case                                             */
  /*                                                                  */
  /* 2) For a given initialized value, which memory manager should be */
  /*    used to access it by default, e.g. when "mem" doesn't         */
  /*    associate the value with a memory_tracker, in which case it   */
  /*    is used instead, by obtaining this memory tracker's memory    */
  /*    manager.                                                      */
  /*                                                                  */
  /* ---------------------------------------------------------------- */
  /*                                                                  */
  /* "val_raw" might be NULL, and it might be uninitialized.          */
  /*                                                                  */
  /* If NULL, "mem_manager" can return a default memory manager,      */
  /* or alternatively a memory manager used for all values of this    */
  /* type if this type forces all values to use a specific            */
  /* memory manager.                                                  */
  const memory_manager_t
                      *(*default_memory_manager)
                                     ( const type_t *self
                                     , tval *val
                                     );

  /* ---------------------------------------------------------------- */
  /* Copying.                                                         */
  /* ---------------------------------------------------------------- */

  /* Pass non-zero argument to recursively make copies of subfields.  */
  /*                                                                  */
  /* Some implementations support NULL "dest" pointers, indicating    */
  /* a new one should be dynamically allocated, using management      */
  /* associated with "src" in case of differences.                    */
  /*                                                                  */
  /* Returns NULL on failure.                                         */

  /* Copy one value into another.                                     */
  /*                                                                  */
  /* Some implementations support NULL "dest" pointers, indicating    */
  /* a new one should be dynamically allocated, using management      */
  /* associated with "src" in case of differences.                    */
  /*                                                                  */
  /* defaults_src_unused:                                             */
  /*   Choose a default value in "dest" for each field in "src" that  */
  /*   is 0 or NULL, unless the type specifies a value other than 0   */
  /*   or NULL as its "special unused value" in which case check      */
  /*   against that, or none at all, in which case don't supply a     */
  /*   default value.                                                 */
  /*                                                                  */
  /*   For each field in "src", if the type's field has a             */
  /*   special "unused value", check whether it is equal to the field */
  /*   value in "src", and, if so, choose a default one instead.      */
  /*                                                                  */
  /*   Usually, the special "unused value" is zero / NULL, so this    */
  /*   flag determines whether to choose a default value when a field */
  /*   is 0 or NULL.                                                  */
  /*                                                                  */
  /*   When this flag is disabled, just copy from "source", without   */
  /*   choosing default values.                                       */
  /*                                                                  */
  /* rec_copy:                                                        */
  /*   0 to skip recursively calling "dup" on subcomponents that are  */
  /*   references designated as copyable.                             */
  /*                                                                  */
  /*   For subcomponents that are references and designated as        */
  /*   copyable, "rec_copy" determines whether to recursively copy    */
  /*   these.                                                         */
  /*                                                                  */
  /*   If this type is a struct, then the type's struct_info returned */
  /*   by "is_struct" associates each field in the struct with a flag */
  /*   that indicates whether it is a copyable reference.             */
  /*                                                                  */
  /*   Each field is also associated with a "type_t *" reference,     */
  /*   whose "dup" procedure is used to recursively copy the field.   */
  /*   (If the type lacks a "dup" procedure, behaviour will be as if  */
  /*   "rec_copy" were 0.                                             */
  /*                                                                  */
  /*   Set this to a negative value to limit traversal depth equal    */
  /*   to the inverse of the limit.                                   */
  /*                                                                  */
  /* dup_metadata:                                                    */
  /*   Whether to assign all subcomponents, and not skip those        */
  /*   designated as "metadata", data which is specific to each       */
  /*   value, such as fields like memory trackers that are usually    */
  /*   unique to each value.                                          */
  /*                                                                  */
  /*   It is usually an error to set this flag without a "dest"       */
  /*   argument, because of a lack of a direct appropriate memory     */
  /*   tracker.                                                       */
  /*                                                                  */
  /* ref_traversal:                                                   */
  /*   History of values duplicated, used to avoid loops during       */
  /*   recursive copying.                                             */
  /*                                                                  */
  /*   When "dup" is called at the root level, this can be NULL.      */
  /*                                                                  */
  /*   Users can also preinitialize a ref_traversal_t before calling. */
  /*   One possible useful purpose for this is to set a callback      */
  /*   for when a loop is detected.                                   */
  /*                                                                  */
  /*   (At non-root-level recursive calls, passing a "ref_traversal"  */
  /*   is optional but highly recommended.)                           */
  /*                                                                  */
  /* Returns NULL on failure.                                         */
  tval                *(*dup)        ( const type_t *self
                                     , tval *dest
                                     , const tval *src
                                     , int defaults_src_unused
                                     , int rec_copy
                                     , int dup_metadata
                                     , ref_traversal_t *ref_traversal
                                     );

  /* ---------------------------------------------------------------- */

  const char *parity;
};

/* ---------------------------------------------------------------- */

/*
 * Various "type_t" methods and method helpers.
 */

/* "typed" */
const type_t *type_is_typed_from_struct(const type_t *self);

const type_t *type_is_typed(const type_t *self);
const type_t *type_is_untyped(const type_t *self);

/* name */
const char *type_has_name(const type_t *self, char *out_info_name, size_t name_buf_size, const char *name);
const char *type_has_static_name(const type_t *self, const char *name);

/* info */
const char *type_has_no_info(const type_t *self, char *out_info_buf, size_t info_buf_size);

/* size */

/* is_struct */
#define STRUCT_INFO_CACHE(struct_info)          \
  static struct_info_t *struct_info = NULL;     \
  if (struct_info)                              \
  {                                             \
    return (const struct_info_t *) struct_info; \
  }                                             \
  else                                          \
  {                                             \
    static struct_info_t struct_info_def;       \
                                                \
    struct_info = &struct_info_def;             \
  }

#define STRUCT_INFO_INIT_COMPLEX(struct_info, field_default_value, field_template_unused_value) \
  do                                                                                            \
  {                                                                                             \
    /* Initialize struct_info. */                                                               \
    struct_info = struct_info_init                                                              \
      ( struct_info                                                                             \
      , field_default_value                                                                     \
      , field_template_unused_value                                                             \
      );                                                                                        \
  } while(0)

#define STRUCT_INFO_DONE_COMPLEX(struct_info)                                   \
  do                                                                            \
  {                                                                             \
    /* Done adding fields. */                                                   \
    struct_info = struct_info_add_field_terminator(struct_info);                \
                                                                                \
    if (verify_struct_info(struct_info, NULL, 0) != verify_struct_info_success) \
      struct_info = NULL;                                                       \
                                                                                \
    return ((const struct_info_t *) struct_info);                               \
  } while(0)

/* -- */

#define STRUCT_INFO_BEGIN_COMPLEX(struct_type, struct_info, field_default_value, field_template_unused_value) \
  /* typedef mystruct_t struct_type; */                                                                       \
  typedef struct_type struct_##type;                                                                          \
                                                                                                              \
  /* struct_info_t *struct_info; */                                                                           \
  STRUCT_INFO_CACHE(struct_info)                                                                              \
                                                                                                              \
  do                                                                                                          \
  {                                                                                                           \
    /* Initialize struct_info. */                                                                             \
    STRUCT_INFO_INIT_COMPLEX(struct_info, field_default_value, field_template_unused_value);                  \
  } while(0)

#define STRUCT_INFO_ADD_COMPLEX(struct_info, struct_type, field_name, field_type_rep) \
  do                                                                                  \
  {                                                                                   \
    struct_info = struct_info_add_field                                               \
      ( struct_info                                                                   \
      , (size_t) (OFFSET_FIELD(struct_type, field_name))                              \
      , (size_t) (SIZEOF_FIELD(struct_type, field_name))                              \
      , field_type_rep                                                                \
      );                                                                              \
  } while(0)

#define STRUCT_INFO_REINIT_WITH_COMPLEX(field_default_value)                                           \
  STRUCT_INFO_INIT_COMPLEX(struct_info, field_default_value, field_template_unused_value_zero)

#define STRUCT_INFO_REINIT_WITH_COMPLEX_BOTH(field_default_value, field_template_unused_value)         \
  STRUCT_INFO_INIT_COMPLEX(struct_info, field_default_value, field_template_unused_value)

/* -- */

/*
 * An "is_struct" method can use these 4 macros to describe a struct's fields.
 *
 * Start with BEGIN, ADD each field, and then call DONE.
 *
 * To supply a custom field_default_value or field_template_unused_value, call
 * "REINIT_WITH" after BEGIN.
 *
 * Example:
 */

#define STRUCT_INFO_BEGIN(type_name) \
  STRUCT_INFO_BEGIN_COMPLEX(CAT(type_name, _t), struct_info, CAT(type_name, _default_field_value), field_template_unused_value_zero)

#define STRUCT_INFO_ADD(field_name, field_type_rep)                             \
  STRUCT_INFO_ADD_COMPLEX(struct_info, struct_type, field_name, field_type_rep)

#define STRUCT_INFO_RADD(field_type_rep, field_name) \
  STRUCT_INFO_ADD(field_name, field_type_rep)

#define STRUCT_INFO_DONE() \
  STRUCT_INFO_DONE_COMPLEX(struct_info)

/* -- */

#define DEF_FIELD_DEFAULT_VALUE_FROM_TYPE(type_name)                                                 \
  static size_t CAT(type_name, _default_field_value)(const field_info_t *self, void *dest_field_mem) \
  {                                                                                                  \
    return field_default_value_from_type(self, dest_field_mem, CAT(type_name, _type)());             \
  }

/* -- */

const struct_info_t *type_is_not_struct(const type_t *self);

/* TODO: from_type: use the type's has_default, otherwise from_field_type. */
size_t field_default_value_from_type      (const field_info_t *self, void *dest_field_mem, const type_t *type);
size_t field_default_value_from_field_type(const field_info_t *self, void *dest_field_mem);
size_t field_default_value_zero           (const field_info_t *self, void *dest_field_mem);

size_t field_template_unused_value_zero(const field_info_t *self, void *dest_field_mem);

/* cons_type" */
typed_t type_has_template_cons_type(const type_t *self);

/* init */
tval *type_has_template_cons_basic_initializer(const type_t *type, tval *cons);
tval *type_has_template_cons_basic_initializer_force_memory_manager(const type_t *type, tval *cons);

tval *template_cons_basic_initializer(const type_t *type, template_cons_t *cons, int allow_alternate_memory_manager);

/* free */
int type_has_template_cons_basic_freer(const type_t *type, tval *val);

int template_cons_basic_freer(const type_t *type, tval *cons);

/* has_default */
const tval   *type_has_no_default(const type_t *self);
const tval   *type_has_default_value(const type_t *self, const tval *val);

/* mem */
memory_tracker_t *type_mem_struct_or_global_dyn(const type_t *self, tval *val_raw);
memory_tracker_t *type_mem_valueless(const type_t *self, tval *val_raw, memory_tracker_t *valueless_memory_tracker);

/* mem_init */

/* Standard optional-struct_info-based "mem_init" methods. */
void *type_supports_dynamic_allocation
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  );
void *type_supports_dynamic_allocation_only_some_vals
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  );

void *type_mem_init_valueless_or_inside_value
  ( const type_t *self
  , tval         *val_raw
  , int           is_dynamically_allocated
  );

/* Procedure for standard "mem_init" initialization. */
/* TODO: rename */
void *mem_init_type_valueless_or_inside_value
  ( const type_t           *type
  , tval                   *val_raw
  , int                     is_dynamically_allocated
  , int                     are_all_vals_supported
  , const memory_manager_t *val_memory_manager

  , char   *out_err_buf
  , size_t  err_buf_size
  );

/* mem_is_dyn */
int type_is_dyn_valueless_or_inside_value
  ( const type_t *self
  , tval         *val
  );

/* Standard optional-struct_info-based "mem_is_dyn" methods. */
int mem_is_dyn_valueless_or_inside_value
  ( const type_t *type
  , tval         *val

  , char   *out_err_buf
  , size_t  err_buf_size
  );

/* mem_free */
int type_mem_free_valueless_or_inside_value_allocation
  ( const type_t *type
  , tval         *val
  );

/* Standard optional-struct_info-based "mem_free" methods. */
int mem_free_valueless_or_inside_value_allocation
  ( const type_t *self
  , tval         *val

  , char   *out_err_buf
  , size_t  err_buf_size
  );

/* default_memory_manager */

const memory_manager_t *type_has_no_default_memory_manager(const type_t *self, tval *val);

const memory_manager_t *type_prefers_given_memory_manager(const type_t *self, tval *val, const memory_manager_t *memory_manager);
const memory_manager_t *type_prefers_default_memory_manager(const type_t *self, tval *val);
const memory_manager_t *type_prefers_malloc_memory_manager(const type_t *self, tval *val);

/* If the type has no global memory tracker, just prefer default_memory_manager, after
 * usual "struct_info" checking, making it equivalent to
 * "type_prefers_default_memory_manager" in this case.
 */
const memory_manager_t *type_prefers_global_memory_manager(const type_t *type, tval *val_raw);

/* dup */
tval *type_has_struct_dup_allow_malloc( const type_t *self
                                      , tval *dest
                                      , const tval *src
                                      , int defaults_src_unused
                                      , int rec_copy
                                      , int dup_metadata
                                      , ref_traversal_t *ref_traversal
                                      );
tval *type_has_struct_dup_never_malloc( const type_t *self
                                      , tval *dest
                                      , const tval *src
                                      , int defaults_src_unused
                                      , int rec_copy
                                      , int dup_metadata
                                      , ref_traversal_t *ref_traversal
                                      );

/* ---------------------------------------------------------------- */

/*
 * "type_t" defaults.
 */

extern const type_t type_defaults;

/* ---------------------------------------------------------------- */

/*
 * Fundamental "type_t" accessors.
 */

const type_t        *type_typed      (const type_t *type);
const char          *type_name       (const type_t *type);
const char          *type_info       ( const type_t *type
                                     , char         *out_info_buf
                                     , size_t        info_buf_size
                                     );
size_t               type_size       (const type_t *type, const tval *val);
const struct_info_t *type_is_struct  (const type_t *type);
typed_t              type_cons_type  (const type_t *type);
tval                *type_init       (const type_t *type, tval *cons);
void                 type_free       (const type_t *type, tval *val);
const tval          *type_has_default(const type_t *type);
memory_tracker_t    *type_mem        (const type_t *type, tval *val_raw);
void                *type_mem_init   ( const type_t *type
                                     , tval *val_raw
                                     , int is_dynamically_allocated
                                     );
int                  type_mem_is_dyn ( const type_t *type
                                     , tval         *val
                                     );
int                  type_mem_free   ( const type_t *type
                                     , tval         *val
                                     );
const memory_manager_t
                    *type_default_memory_manager
                                     ( const type_t *type
                                     , tval *val
                                     );
tval                *type_dup        ( const type_t *type
                                     , tval *dest
                                     , const tval *src
                                     , int defaults_src_unused
                                     , int rec_copy
                                     , int dup_metadata
                                     , ref_traversal_t *ref_traversal
                                     );

/* ---------------------------------------------------------------- */

/*
 * Procedures on or for "type_t"'s.
 */

void              tval_free(tval *val);

memory_tracker_t *type_val_has_individual_mem(const type_t *type, tval *val);

/* ---------------------------------------------------------------- */
/* Template constructors, available for types to use.               */
/* ---------------------------------------------------------------- */

const type_t *template_cons_type(void);
extern const type_t template_cons_type_def;
/* Forward declaration: typedef struct template_cons_s template_cons_t; */
struct template_cons_s
{
  typed_t type;

  /*
   * The value to initialize.
   *
   * Set to NULL to tell the initializer to "malloc" a new one.
   */
  tval *dest;

  /*
   * Set NULL to tell the initializer to select
   * reasonable defaults, usually "malloc_manager".
   *
   * Set individual components of the "memory_manager_t" value to NULL to tell
   * the initializer to select a reasonable default for that field.
   *
   * If "calloc" and "realloc" are NULL, the initializer should default to
   * behaviour that depends on its behaviour for "malloc" and "free".
   *
   * This means that only "malloc" and "free" can be provided with a guarantee
   * that conforming initializers will not use external memory management
   * algorithms for "calloc" and "realloc", instead using the procedures
   * provided.
   */
  const memory_manager_t *memory_manager;

  /*
   * The initial subcomponent values will be chosen from this value, which must
   * be of the type as the value being constructed.
   *
   * If the associated type is a struct, then the subcomponent values
   * correspond to the struct's fields.
   *
   * Set to "NULL" to force all default values.
   */
  const tval *initials;

  /*
   * Never interpreted a value in "initials" as the "special unused value",
   * and selecting a default value for that field, instead always assigning
   * each field to the value of the field in "initials".
   *
   * If this is true, and "initials is NULL", then the value must be
   * "zeroed-out", with the result the value represented by memory filled with
   * "0" bytes.
   */
  int force_no_defaults;

  /*
   * Whether to recursively copy certain fields in "initials", rather than
   * simply copying each reference as such.
   *
   * Set to a negative value to limit traversal depth.
   *
   * Defaults to disabled (0).
   */
  int initials_copy_rec;

  /*
   * Often omitted, but when present specifies an initial
   * "initials_copy_traversal" for assignment.
   *
   * Allows callbacks in case of recursive copy loops.
   */
  ref_traversal_t *ref_traversal;

  /*
   * When assigning struct fields, don't skip assignment of specially
   * designated metadata fields by calling "struct_dup" with a "copy" value of
   * 0, and skip memory tracker initialization.
   *
   * It is an error to set this flag in constructors that dynamically allocate
   * values, because of the lack of an associated memory tracker.
   */
  int preserve_metadata;

  /*
   * Additional type-dependent information for value initialization.
   */
  void *user;

  /*
   * Don't initialize the value, and only allocate memory.
   *
   * Set to a number greater than 1 to allocate an array of values.
   *
   * If non-zero, all above fields are ignored.
   */
  size_t allocate_only_with_num;

  /*
   * Optional: can be NULL.
   *
   * This allows initializers to write informative error messages in case
   * initialization fails.
   */
  char   *out_init_error_msg;
  size_t  init_error_msg_size;
};

#define TEMPLATE_CONS_DEFAULTS              \
  { template_cons_type                      \
                                            \
  , /* dest                   */ NULL       \
  , /* memory_manager         */ NULL       \
  , /* initials               */ NULL       \
  , /* force_no_defaults      */ 0          \
  , /* initials_copy_rec      */ 0          \
  , /* ref_traversal          */ NULL       \
  , /* preserve_nocopy        */ 0          \
  , /* user                   */ NULL       \
  , /* allocate_only_with_num */ (size_t) 0 \
  , /* out_init_error_msg     */ NULL       \
  , /* init_error_msg_size    */ (size_t) 0 \
  }
extern const template_cons_t template_cons_defaults;

extern const template_cons_t * const default_template_cons;

/* ---------------------------------------------------------------- */

/*
 * Standard constructor generators.
 */
template_cons_t  template_cons_default(tval *dest);
template_cons_t  template_cons_initials(tval *dest, const tval *initials);
/* force_no_defaults, rec_copy. */
template_cons_t  template_cons_copy    (tval *dest, const tval *src);

template_cons_t  template_cons_set_error_output  (template_cons_t cons, char *out_init_error_msg, size_t init_error_msg_size);
template_cons_t  template_cons_set_memory_manager(template_cons_t cons, const memory_manager_t *memory_manager);

/* Here, "tval *"'s must not be NULL, because the type is obtained from the
 * tval.
 */
tval            *template_cons_init_tval_defaults(tval *dest,                       const memory_manager_t *memory_manager, char *out_init_error_msg, size_t init_error_msg_size);
tval            *template_cons_init_tval_initials(tval *dest, const tval *initials, const memory_manager_t *memory_manager, char *out_init_error_msg, size_t init_error_msg_size);
tval            *template_cons_init_tval_copy    (tval *dest, const tval *src,      const memory_manager_t *memory_manager, char *out_init_error_msg, size_t init_error_msg_size);

/* ---------------------------------------------------------------- */

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
  );
void *template_cons_dup_struct_meminit_type
  ( const tval *self
  , tval       *val_raw
  , int         is_dynamically_allocated
  );

int template_cons_free_struct
  ( tval *val
  , int                      (*mem_free)( const tval *self
                                        , tval       *val
                                        )
  , const tval                *mem_free_object
  );
int template_cons_free_struct_memfree_type
  ( const tval *self
  , tval       *val
  );

/* ---------------------------------------------------------------- */
/* Primitive C data types.                                          */
/* ---------------------------------------------------------------- */

/* http://en.cppreference.com/w/c/header */

/* General type. */
const type_t *void_type(void);

/* General pointers. */
const type_t *funp_type(void);
const type_t *objp_type(void);

/* Scalar types. */
const type_t *char_type(void);
const type_t *schar_type(void);
const type_t *uchar_type(void);

const type_t *short_type(void);
const type_t *ushort_type(void);

const type_t *int_type(void);
const type_t *uint_type(void);

const type_t *long_type(void);
const type_t *ulong_type(void);

const type_t *float_type(void);
const type_t *double_type(void);
const type_t *ldouble_type(void);

/* Derived types. */
const type_t *array_type(void);

/* <math.h> */
const type_t *div_type(void);
const type_t *ldiv_type(void);

/* <setjmp.h> */
const type_t *jmpbuf_type(void);

/* <stdarg.h> */
const type_t *va_list_type(void);

/* <stddef.h> */
const type_t *size_type(void);
const type_t *ptrdiff_type(void);

/* <stdlib.h> */
const type_t *sig_atomic_type(void);

/* <stdio.h> */
const type_t *file_type(void);
const type_t *fpos_type(void);

/* <time.h> */
const type_t *tm_type(void);
const type_t *time_type(void);
const type_t *clock_type(void);

/* ---------------------------------------------------------------- */
/* Utility functions.                                               */
/* ---------------------------------------------------------------- */

ptrdiff_t   field_pos (const void *base, const void *field);
void       *field_ref (ptrdiff_t   pos,  void       *base);
const void *field_cref(ptrdiff_t   pos,  const void *base);

/* ---------------------------------------------------------------- */

typedef void   *objp_cast_t;
typedef void *(*funp_cast_t)(void *, ...);

funp_cast_t objp_to_funp(objp_cast_t ptr);
objp_cast_t funp_to_objp(funp_cast_t ptr);

/* ---------------------------------------------------------------- */

#endif /* ifndef TYPE_BASE_H */

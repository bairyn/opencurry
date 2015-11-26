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
#include "util.h"

/* ---------------------------------------------------------------- */

#include "type_base_prim.h"

#include "type_base_typed.h"

#include "type_base_tval.h"

#include "type_base_compare.h"

#include "type_base_memory_manager.h"

#include "type_base_lookup.h"

#include "type_base_memory_tracker.h"

/* ---------------------------------------------------------------- */
/* ref_traversal_t                                                  */
/* ---------------------------------------------------------------- */

/* TODO */
const type_t *ref_traversal_type(void);
extern const type_t ref_traversal_type_def;
typedef struct ref_traversal_s ref_traversal_t;
struct ref_traversal_s
{
  typed_t type;

  memory_tracker_t memory;

  ref_traversal_t *is_mutable;

  void   **history;
  size_t   history_size;
  size_t   history_num;
  size_t   history_len;
};

/* ---------------------------------------------------------------- */

#define REF_TRAVERSAL_DEFAULTS                 \
  { ref_traversal_type                         \
                                               \
  , /* memory       */ MEMORY_TRACKER_DEFAULTS \
                                               \
  , /* is_mutable   */ NULL                    \
                                               \
  , /* history      */ NULL                    \
  , /* history_size */ 0                       \
  , /* history_num  */ 0                       \
  , /* history_len  */ 0                       \
  }
extern const ref_traversal_t ref_traversal_defaults;

/* ---------------------------------------------------------------- */

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


void *ref_traversal_tagged_add   (      ref_traversal_t *ref_traversal, unsigned char tag, void *reference);
void *ref_traversal_tagged_remove(      ref_traversal_t *ref_traversal, unsigned char tag, void *reference);
void *ref_traversal_tagged_take  (      ref_traversal_t *ref_traversal, unsigned char tag, void *reference);
void *ref_traversal_tagged_exists(const ref_traversal_t *ref_traversal, unsigned char tag, void *reference);

/* ---------------------------------------------------------------- */
/* struct_info_t and field_info_t                                   */
/* ---------------------------------------------------------------- */

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
  int           is_recursible_ref;

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
  , /* is_recursible_ref     */ 0                          \
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

#define FIELD_MEMCMP_ERR_NULL_FIELD_VAL1 ((int) -0x37ED)
#define FIELD_MEMCMP_ERR_NULL_FIELD_VAL2 ((int) -0x37EE)

/*
 * Directly compare the memory represents two values in a field.
 *
 * Returns:
 *   = 0: Values are equal.
 *   > 0: First byte in "s1" is greater than first byte in "s2".
 *   < 0: First byte in "s1" is less    than first byte in "s2".
 */
int field_memcmp(const field_info_t *field_info, const void *field_val1, const void *field_val2);

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
  , void *(*with_chunk)(void *context, void *last_accumulation, const struct_info_t *struct_info_chunk, int *out_iteration_break)
  , void *context
  , void *initial_accumulation
  );

void *struct_info_iterate_fields
  ( const struct_info_t *struct_info
  , void *(*with_field)(void *context, void *last_accumulation, const field_info_t *field_info, int *out_iteration_break)
  , void *context
  , void *initial_accumulation
  );

void *struct_info_iterate_chunks_mutable
  ( struct_info_t *struct_info
  , void *(*with_chunk)(void *context, void *last_accumulation, struct_info_t *struct_info_chunk, int *out_iteration_break)
  , void *context
  , void *initial_accumulation
  );

void *struct_info_iterate_fields_mutable
  ( struct_info_t *struct_info
  , void *(*with_field)(void *context, void *last_accumulation, field_info_t *field_info, int *out_iteration_break)
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
#define STRUCT_DUP_VALS_TAG_DEST 0
#define STRUCT_DUP_VALS_TAG_SRC  1
const char *struct_dup(const struct_info_t *struct_info, void *dest, const void *src, int force_no_default, int rec_copy, int dup_metadata, ref_traversal_t *ref_traversal);

#define STRUCT_CMP_VALS_TAG_CHECK    0
#define STRUCT_CMP_VALS_TAG_BASELINE 1
int struct_cmp(const struct_info_t *struct_info, const void *check, const void *baseline, int deep, ref_traversal_t *vals);

#include "type_base_type.h"

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

size_t template_cons_free_struct
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

#include "type_base_universal.h"

/* ---------------------------------------------------------------- */
/* Primitive C data types.                                          */
/* ---------------------------------------------------------------- */

/* http://en.cppreference.com/w/c/header */

#define PRIM_TYPE(name, type, default)                                                           \
  const type_t *CAT(name, _type)(void)                                                           \
    { return &CAT(name, _type_def); }                                                            \
                                                                                                 \
  static const char          *CAT(name, _type_name)       (const type_t *self);                  \
  static size_t               CAT(name, _type_size)       (const type_t *self, const tval *val); \
  static const tval          *CAT(name, _type_has_default)(const type_t *self);                  \
                                                                                                 \
  const type_t CAT(name, _type_def) =                                                            \
    { type_type                                                                                  \
                                                                                                 \
      /* @: Required.           */                                                               \
                                                                                                 \
    , /* memory                 */ MEMORY_TRACKER_DEFAULTS                                       \
    , /* is_self_mutable        */ NULL                                                          \
    , /* @indirect              */ CAT(name, _type)                                              \
                                                                                                 \
    , /* self                   */ NULL                                                          \
    , /* container              */ NULL                                                          \
                                                                                                 \
    , /* typed                  */ type_is_untyped                                               \
                                                                                                 \
    , /* @name                  */ CAT(name, _type_name)                                         \
    , /* info                   */ NULL                                                          \
    , /* @size                  */ CAT(name, _type_size)                                         \
    , /* @is_struct             */ type_is_not_struct                                            \
    , /* is_mutable             */ NULL                                                          \
    , /* is_subtype             */ NULL                                                          \
    , /* is_supertype           */ NULL                                                          \
                                                                                                 \
    , /* cons_type              */ NULL                                                          \
    , /* init                   */ NULL                                                          \
    , /* free                   */ NULL                                                          \
    , /* has_default            */ CAT(name, _type_has_default)                                  \
    , /* mem                    */ NULL                                                          \
    , /* mem_init               */ NULL                                                          \
    , /* mem_is_dyn             */ NULL                                                          \
    , /* mem_free               */ NULL                                                          \
    , /* default_memory_manager */ NULL                                                          \
                                                                                                 \
    , /* dup                    */ NULL                                                          \
                                                                                                 \
    , /* user                   */ NULL                                                          \
    , /* cuser                  */ NULL                                                          \
    , /* cmp                    */ NULL                                                          \
                                                                                                 \
    , /* parity                 */ ""                                                            \
    };                                                                                           \
                                                                                                 \
  static const char          *CAT(name, _type_name)       (const type_t *self)                   \
    { return STR(name); }                                                                        \
                                                                                                 \
  static size_t               CAT(name, _type_size)       (const type_t *self, const tval *val)  \
    { return sizeof(type); }                                                                     \
                                                                                                 \
  static const tval          *CAT(name, _type_has_default)(const type_t *self)                   \
    { return default; }

/* General type. */
const type_t *void_type(void);
typedef void void_t;

extern const type_t void_type_def;

/* General pointers. */
const type_t *objp_type(void);
const type_t *funp_type(void);

typedef void *(*objp_t)(void *, ...);
typedef void   *funp_t;

typedef       void *(*       objpm_t)(void *, ...);
typedef       void   *       funpm_t;
typedef const void *(*       objpc_t)(void *, ...);
typedef const void   *       funpc_t;

typedef       void *(*       objmp_t)(void *, ...);
typedef       void   *       funmp_t;
typedef       void *(* const objcp_t)(void *, ...);
typedef       void   * const funcp_t;

typedef       void *(*       objmpm_t)(void *, ...);
typedef       void   *       funmpm_t;
typedef const void *(*       objmpc_t)(void *, ...);
typedef const void   *       funmpc_t;
typedef       void *(* const objcpm_t)(void *, ...);
typedef       void   * const funcpm_t;
typedef const void *(* const objcpc_t)(void *, ...);
typedef const void   * const funcpc_t;


extern const type_t objp_type_def;
extern const type_t funp_type_def;

extern const objp_t objp_default;
extern const funp_t funp_default;


const type_t *genp_type(void);
extern const type_t genp_type_def;
typedef union genp_u genp_t;
union genp_u
{
  void        *ptr;
  const void  *ptrc;

  /* ---------------------------------------------------------------- */

  size_t       size;
  ptrdiff_t    ptrdiff;

  /* ---------------------------------------------------------------- */

  objp_t       objp;
  funp_t       funp;
  funp_t      *funpr;

  /* ---------------------------------------------------------------- */

  objpc_t      objpc;
  funpc_t      funpc;
  funpc_t     *funpcr;

#if 0
  objcp_t      objcp;
  funcp_t      funcp;
  funcp_t     *funcpr;

  objcpc_t     objcpc;
  funcpc_t     funcpc;
  funcpc_t    *funcpcr;
#endif


  objpm_t      objpm;
  funpm_t      funpm;
  funpm_t     *funpmr;

  objmp_t      objmp;
  funmp_t      funmp;
  funmp_t     *funmpr;

  objmpm_t     objmpm;
  funmpm_t     funmpm;
  funmpm_t    *funmpmr;

  objmpc_t     objmpc;
  funmpc_t     funmpc;
  funmpc_t    *funmpcr;

#if 0
  objcpm_t     objcpm;
  funcpm_t     funcpm;
  funcpm_t    *funcpmr;
#endif

  /* ---------------------------------------------------------------- */

  objp_cast_t  objp_cast;
  funp_cast_t  funp_cast;
  funp_cast_t *funpr_cast;

  /* ---------------------------------------------------------------- */

  objpc_cast_t  objpc_cast;
  funpc_cast_t  funpc_cast;
  funpc_cast_t *funpcr_cast;


  objpm_cast_t  objpm_cast;
  funpm_cast_t  funpm_cast;
  funpm_cast_t *funpmr_cast;
};

#define GENP_DEFAULT \
  { NULL }
extern const genp_t genp_default;


const type_t *genpm_type(void);
extern const type_t genpm_type_def;
typedef union genpm_u genpm_t;
union genpm_u
{
  void        *ptr;

  /* ---------------------------------------------------------------- */

  size_t       size;
  ptrdiff_t    ptrdiff;

  /* ---------------------------------------------------------------- */

  objp_t       objp;
  funp_t       funp;
  funp_t      *funpr;

  /* ---------------------------------------------------------------- */

#if 0
  objcp_t      objcp;
  funcp_t      funcp;
  funcp_t     *funcpr;
#endif


  objpm_t      objpm;
  funpm_t      funpm;
  funpm_t     *funpmr;

  objmp_t      objmp;
  funmp_t      funmp;
  funmp_t     *funmpr;

  objmpm_t     objmpm;
  funmpm_t     funmpm;
  funmpm_t    *funmpmr;

#if 0
  objcpm_t     objcpm;
  funcpm_t     funcpm;
  funcpm_t    *funcpmr;
#endif

  /* ---------------------------------------------------------------- */

  objp_cast_t  objp_cast;
  funp_cast_t  funp_cast;
  funp_cast_t *funpr_cast;

  /* ---------------------------------------------------------------- */

  objpc_cast_t  objpc_cast;
  funpc_cast_t  funpc_cast;
  funpc_cast_t *funpcr_cast;


  objpm_cast_t  objpm_cast;
  funpm_cast_t  funpm_cast;
  funpm_cast_t *funpmr_cast;
};

#define GENPM_DEFAULT \
  { NULL }
extern const genpm_t genpm_default;


const type_t *genpc_type(void);
extern const type_t genpc_type_def;
typedef union genpc_u genpc_t;
union genpc_u
{
  const void  *ptrc;

  /* ---------------------------------------------------------------- */

  size_t       size;
  ptrdiff_t    ptrdiff;

  /* ---------------------------------------------------------------- */

  objpc_t      objpc;
  funpc_t      funpc;
  funpc_t     *funpcr;

#if 0
  objcp_t      objcp;
  funcp_t      funcp;
  funcp_t     *funcpr;

  objcpc_t     objcpc;
  funcpc_t     funcpc;
  funcpc_t    *funcpcr;
#endif


  objmpc_t     objmpc;
  funmpc_t     funmpc;
  funmpc_t    *funmpcr;

#if 0
  objcpm_t     objcpm;
  funcpm_t     funcpm;
  funcpm_t    *funcpmr;
#endif

  /* ---------------------------------------------------------------- */

  objpc_cast_t  objpc_cast;
  funpc_cast_t  funpc_cast;
  funpc_cast_t *funpcr_cast;
};

#define GENPC_DEFAULT \
  { NULL }
extern const genpc_t genpc_default;

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


typedef char           char_t;
typedef signed char    schar_t;
typedef unsigned char  uchar_t;

typedef short          short_t;
typedef unsigned short ushort_t;

typedef int            int_t;
typedef unsigned int   uint_t;

typedef long           long_t;
typedef unsigned long  ulong_t;

typedef float          float_t;
typedef double         double_t;
typedef long double    ldouble_t;


extern const type_t char_type_def;
extern const type_t schar_type_def;
extern const type_t uchar_type_def;

extern const type_t short_type_def;
extern const type_t ushort_type_def;

extern const type_t int_type_def;
extern const type_t uint_type_def;

extern const type_t long_type_def;
extern const type_t ulong_type_def;

extern const type_t float_type_def;
extern const type_t double_type_def;
extern const type_t ldouble_type_def;


extern const char_t    char_default;
extern const schar_t   schar_default;
extern const uchar_t   uchar_default;

extern const short_t   short_default;
extern const ushort_t  ushort_default;

extern const int_t     int_default;
extern const uint_t    uint_default;

extern const long_t    long_default;
extern const ulong_t   ulong_default;

extern const float_t   float_default;
extern const double_t  double_default;
extern const ldouble_t ldouble_default;

/* Enumerations. */
const type_t *enum_type(void);

enum enum_e
{
  enum_zero = 0
};
typedef enum enum_e enum_t;

extern const type_t enum_type_def;

#define ENUM_DEFAULT \
  enum_zero
extern const enum_t enum_default;

/* Derived types. */
const type_t *array_type(void);

typedef unsigned char array_t[];

extern const type_t array_type_def;

extern const array_t array_default;
#if 0
extern const size_t  array_default_size;
extern const size_t  array_default_num;
extern const size_t  array_default_len;
#endif

/* */
const type_t *union_type(void);
extern const type_t union_type_def;

/* <math.h> */
const type_t *div_type(void);
const type_t *ldiv_type(void);

extern const type_t div_type_def;
extern const type_t ldiv_type_def;

/* <setjmp.h> */
const type_t *jmp_buf_type(void);

extern const type_t jmp_buf_type_def;

/* <signal.h> */
const type_t *sig_atomic_type(void);

extern const type_t sig_atomic_type_def;

/* <stdarg.h> */
const type_t *va_list_type(void);

extern const type_t va_list_type_def;

/* <stddef.h> */
const type_t *size_type(void);
const type_t *ptrdiff_type(void);

extern const type_t size_type_def;
extern const type_t ptrdiff_type_def;

/* <stdio.h> */
const type_t *file_type(void);
const type_t *fpos_type(void);

extern const type_t file_type_def;
extern const type_t fpos_type_def;

/* <time.h> */
const type_t *tm_type(void);
const type_t *time_type(void);
const type_t *clock_type(void);

extern const type_t tm_type_def;
extern const type_t time_type_def;
extern const type_t clock_type_def;

/* ---------------------------------------------------------------- */

typedef void_t    primvoid_t;

typedef objp_t    primobjp_t;
typedef funp_t    primfunp_t;
typedef objpm_t   primobjpm_t;
typedef funpm_t   primfunpm_t;
typedef objpc_t   primobjpc_t;
typedef funpc_t   primfunpc_t;
typedef objmp_t   primobjmp_t;
typedef funmp_t   primfunmp_t;
typedef objcp_t   primobjcp_t;
typedef funcp_t   primfuncp_t;
typedef objmpm_t  primobjmpm_t;
typedef funmpm_t  primfunmpm_t;
typedef objmpc_t  primobjmpc_t;
typedef funmpc_t  primfunmpc_t;
typedef objcpm_t  primobjcpm_t;
typedef funcpm_t  primfuncpm_t;
typedef objcpc_t  primobjcpc_t;
typedef funcpc_t  primfuncpc_t;

typedef char_t    primchar_t;
typedef schar_t   primschar_t;
typedef uchar_t   primuchar_t;
typedef short_t   primshort_t;
typedef ushort_t  primushort_t;
typedef int_t     primint_t;
typedef uint_t    primuint_t;
typedef long_t    primlong_t;
typedef ulong_t   primulong_t;
typedef float_t   primfloat_t;
typedef double_t  primdouble_t;
typedef ldouble_t primldouble_t;

typedef array_t   primarray_t;

/* ---------------------------------------------------------------- */

/* Mutable vs. const typedefs. */

/* "DUPCONST" is an empty tag that indicates the type being typedef's is
 * already const. */
#define DUPCONST

typedef          void_t    mvoid_t;
typedef const    void_t    cvoid_t;

typedef          objp_t    mobjp_t;
typedef          funp_t    mfunp_t;
typedef          objpm_t   mobjpm_t;
typedef          funpm_t   mfunpm_t;
typedef          objpc_t   mobjpc_t;
typedef          funpc_t   mfunpc_t;
typedef          objmp_t   mobjmp_t;
typedef          funmp_t   mfunmp_t;
typedef          objcp_t   mobjcp_t;
typedef          funcp_t   mfuncp_t;
typedef          objmpm_t  mobjmpm_t;
typedef          funmpm_t  mfunmpm_t;
typedef          objmpc_t  mobjmpc_t;
typedef          funmpc_t  mfunmpc_t;
typedef          objcpm_t  mobjcpm_t;
typedef          funcpm_t  mfuncpm_t;
typedef          objcpc_t  mobjcpc_t;
typedef          funcpc_t  mfuncpc_t;
typedef const    objp_t    cobjp_t;
typedef const    funp_t    cfunp_t;
typedef const    objpm_t   cobjpm_t;
typedef const    funpm_t   cfunpm_t;
typedef const    objpc_t   cobjpc_t;
typedef const    funpc_t   cfunpc_t;
typedef const    objmp_t   cobjmp_t;
typedef const    funmp_t   cfunmp_t;
typedef DUPCONST objcp_t   cobjcp_t;
typedef DUPCONST funcp_t   cfuncp_t;
typedef const    objmpm_t  cobjmpm_t;
typedef const    funmpm_t  cfunmpm_t;
typedef const    objmpc_t  cobjmpc_t;
typedef const    funmpc_t  cfunmpc_t;
typedef DUPCONST objcpm_t  cobjcpm_t;
typedef DUPCONST funcpm_t  cfuncpm_t;
typedef DUPCONST objcpc_t  cobjcpc_t;
typedef DUPCONST funcpc_t  cfuncpc_t;

typedef          char_t    mchar_t;
typedef          schar_t   mschar_t;
typedef          uchar_t   muchar_t;
typedef          short_t   mshort_t;
typedef          ushort_t  mushort_t;
typedef          int_t     mint_t;
typedef          uint_t    muint_t;
typedef          long_t    mlong_t;
typedef          ulong_t   mulong_t;
typedef          float_t   mfloat_t;
typedef          double_t  mdouble_t;
typedef          ldouble_t mldouble_t;
typedef const    char_t    cchar_t;
typedef const    schar_t   cschar_t;
typedef const    uchar_t   cuchar_t;
typedef const    short_t   cshort_t;
typedef const    ushort_t  cushort_t;
typedef const    int_t     cint_t;
typedef const    uint_t    cuint_t;
typedef const    long_t    clong_t;
typedef const    ulong_t   culong_t;
typedef const    float_t   cfloat_t;
typedef const    double_t  cdouble_t;
typedef const    ldouble_t cldouble_t;

typedef          array_t   marray_t;
typedef const    array_t   carray_t;

typedef          primvoid_t    mprimvoid_t;
typedef          primobjp_t    mprimobjp_t;
typedef          primfunp_t    mprimfunp_t;
typedef          primobjpm_t   mprimobjpm_t;
typedef          primfunpm_t   mprimfunpm_t;
typedef          primobjpc_t   mprimobjpc_t;
typedef          primfunpc_t   mprimfunpc_t;
typedef          primobjmp_t   mprimobjmp_t;
typedef          primfunmp_t   mprimfunmp_t;
typedef          primobjcp_t   mprimobjcp_t;
typedef          primfuncp_t   mprimfuncp_t;
typedef          primobjmpm_t  mprimobjmpm_t;
typedef          primfunmpm_t  mprimfunmpm_t;
typedef          primobjmpc_t  mprimobjmpc_t;
typedef          primfunmpc_t  mprimfunmpc_t;
typedef          primobjcpm_t  mprimobjcpm_t;
typedef          primfuncpm_t  mprimfuncpm_t;
typedef          primobjcpc_t  mprimobjcpc_t;
typedef          primfuncpc_t  mprimfuncpc_t;
typedef          primchar_t    mprimchar_t;
typedef          primschar_t   mprimschar_t;
typedef          primuchar_t   mprimuchar_t;
typedef          primshort_t   mprimshort_t;
typedef          primushort_t  mprimushort_t;
typedef          primint_t     mprimint_t;
typedef          primuint_t    mprimuint_t;
typedef          primlong_t    mprimlong_t;
typedef          primulong_t   mprimulong_t;
typedef          primfloat_t   mprimfloat_t;
typedef          primdouble_t  mprimdouble_t;
typedef          primldouble_t mprimldouble_t;
typedef          primarray_t   mprimarray_t;
typedef const    primvoid_t    cprimvoid_t;
typedef const    primobjp_t    cprimobjp_t;
typedef const    primfunp_t    cprimfunp_t;
typedef const    primobjpm_t   cprimobjpm_t;
typedef const    primfunpm_t   cprimfunpm_t;
typedef const    primobjpc_t   cprimobjpc_t;
typedef const    primfunpc_t   cprimfunpc_t;
typedef const    primobjmp_t   cprimobjmp_t;
typedef const    primfunmp_t   cprimfunmp_t;
typedef DUPCONST primobjcp_t   cprimobjcp_t;
typedef DUPCONST primfuncp_t   cprimfuncp_t;
typedef const    primobjmpm_t  cprimobjmpm_t;
typedef const    primfunmpm_t  cprimfunmpm_t;
typedef const    primobjmpc_t  cprimobjmpc_t;
typedef const    primfunmpc_t  cprimfunmpc_t;
typedef DUPCONST primobjcpm_t  cprimobjcpm_t;
typedef DUPCONST primfuncpm_t  cprimfuncpm_t;
typedef DUPCONST primobjcpc_t  cprimobjcpc_t;
typedef DUPCONST primfuncpc_t  cprimfuncpc_t;
typedef const    primchar_t    cprimchar_t;
typedef const    primschar_t   cprimschar_t;
typedef const    primuchar_t   cprimuchar_t;
typedef const    primshort_t   cprimshort_t;
typedef const    primushort_t  cprimushort_t;
typedef const    primint_t     cprimint_t;
typedef const    primuint_t    cprimuint_t;
typedef const    primlong_t    cprimlong_t;
typedef const    primulong_t   cprimulong_t;
typedef const    primfloat_t   cprimfloat_t;
typedef const    primdouble_t  cprimdouble_t;
typedef const    primldouble_t cprimldouble_t;
typedef const    primarray_t   cprimarray_t;

/* ---------------------------------------------------------------- */
/* Utility functions.                                               */
/* ---------------------------------------------------------------- */

ptrdiff_t   field_pos (const void *base, const void *field);
void       *field_ref (ptrdiff_t   pos,  void       *base);
const void *field_cref(ptrdiff_t   pos,  const void *base);

/* ---------------------------------------------------------------- */

#endif /* ifndef TYPE_BASE_H */
